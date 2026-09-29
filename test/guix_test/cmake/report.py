#!/usr/bin/env python3
# Copyright (c) 2026 Eclipse ThreadX contributors
# SPDX-License-Identifier: MIT
"""Validate GUIX profiles, test results, and raw coverage unions."""
import json
import os
from pathlib import Path
import re
import shlex
import shutil
import subprocess
import sys
import xml.etree.ElementTree as ET

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]
BUILD = HERE / 'build'
REPORT = HERE / 'coverage_report'


def profiles():
    """Read the authoritative CMake matrix."""
    return re.search(r'set\(BUILD_CONFIGURATIONS(.*?)\)',
                     (HERE / 'CMakeLists.txt').read_text(), re.S)[1].split()


def validate(selected):
    """Reject unknown, duplicate, or empty selections."""
    if not selected or len(set(selected)) != len(selected) or set(selected) - set(profiles()):
        raise ValueError('Invalid profile selection: ' + ' '.join(selected))


def audit(selected):
    """Check definitions and instrumentation in every GUIX compile command."""
    source = (HERE / 'CMakeLists.txt').read_text()
    mappings = {}
    for name in profiles():
        match = re.search(r'set\(' + re.escape(name) + r'\s+(.*?)\)', source, re.S)
        if not match:
            raise ValueError('Missing macro mapping: ' + name)
        mappings[name] = set(re.findall(r'-D(\S+)', match[1]))
    feature_macros = set().union(*mappings.values())
    for name in selected:
        commands = json.loads((BUILD / name / 'compile_commands.json').read_text())
        objects = [item for item in commands
                   if Path(item['file']).is_relative_to(ROOT / 'common' / 'src')]
        if not objects:
            raise ValueError('No GUIX compile commands: ' + name)
        for item in objects:
            args = shlex.split(item['command'])
            definitions = {arg[2:] for arg in args if arg.startswith('-D')}
            if definitions & feature_macros != mappings[name]:
                raise ValueError('Incorrect feature definitions: ' + name)
            if '-fprofile-arcs' not in args or '-ftest-coverage' not in args:
                raise ValueError('GUIX instrumentation missing: ' + name)
        print(name + ': ' + ' '.join(sorted(mappings[name])))


def inventory(name):
    """Require a nonempty CTest inventory with existing executables."""
    result = subprocess.run(['ctest', '--test-dir', str(BUILD / name),
                             '--show-only=json-v1'], check=True,
                            capture_output=True, text=True)
    tests = json.loads(result.stdout)['tests']
    if not tests or any(not item.get('command') for item in tests):
        raise ValueError('Missing tests or binaries: ' + name)
    return tests


def prepare(selected):
    """Clear previous reports and counters before starting a new selection."""
    if REPORT.exists():
        shutil.rmtree(REPORT)
    REPORT.mkdir()
    (REPORT / 'profiles.json').write_text(json.dumps({
        'profiles': selected, 'complete': set(selected) == set(profiles())}, indent=2))
    for path in BUILD.rglob('*.gcda'):
        path.unlink()
    for name in profiles():
        for suffix in ('.xml', '.txt'):
            (BUILD / name / (name + suffix)).unlink(missing_ok=True)
        (BUILD / (name + '.txt')).unlink(missing_ok=True)
        testing = BUILD / name / 'Testing'
        if testing.exists():
            shutil.rmtree(testing)
    for name in ('results.txt', 'test.txt'):
        (BUILD / name).unlink(missing_ok=True)
    audit(selected)
    for name in selected:
        inventory(name)


def nonempty(path):
    """Reject absent and empty artifacts."""
    if not path.is_file() or path.stat().st_size == 0:
        raise ValueError('Missing or empty report: ' + str(path.relative_to(HERE)))


def trace_lines(path):
    """Return the exact source-line denominator of an unfiltered raw trace."""
    nonempty(path)
    data = json.loads(path.read_text())
    lines = set()
    for item in data['files']:
        name = item['file']
        if not name.startswith('common/src/') or '..' in Path(name).parts or Path(name).is_absolute():
            raise ValueError('Coverage path is outside GUIX common sources: ' + name)
        lines.update((name, line['line_number']) for line in item['lines'])
    if not lines:
        raise ValueError('Empty source-line denominator: ' + path.name)
    return lines


def reports(base):
    """Validate all report formats and matching source-line denominators."""
    lines = trace_lines(base.with_suffix('.json'))
    xml = base.with_suffix('.xml')
    nonempty(xml)
    nonempty(base / 'index.html')
    for link in re.findall(r'href="([^"#]+\.html)(?:#[^"]*)?"', (base / 'index.html').read_text()):
        if '://' not in link:
            if Path(link).is_absolute() or '..' in Path(link).parts:
                raise ValueError('Non-relative HTML report link')
            nonempty(base / link)
    for detail in base.glob('*.html'):
        nonempty(detail)
    tree = ET.parse(xml).getroot()
    xml_lines = {(node.attrib['filename'], int(line.attrib['number']))
                 for node in tree.findall('.//class') for line in node.findall('./lines/line')}
    if xml_lines != lines:
        raise ValueError('XML and JSON source-line denominators differ: ' + base.name)
    for source in tree.findall('./sources/source'):
        if source.text not in ('.', ''):
            raise ValueError('Non-relative XML source root')
    return lines


def relative_xml(path):
    """Use the repository root as the portable Cobertura source root."""
    tree = ET.parse(path)
    for source in tree.findall('./sources/source'):
        source.text = '.'
    tree.write(path, encoding='utf-8', xml_declaration=True)


def gcovr(arguments, base):
    """Use one inclusion policy for collection and merging."""
    base.parent.mkdir(parents=True, exist_ok=True)
    base.mkdir(exist_ok=True)
    executable = ROOT / '.venv-ci' / 'bin' / 'gcovr'
    subprocess.run([str(executable), '--root', str(ROOT), '--filter', 'common/src/',
                    '--gcov-executable', 'gcov-14', '--gcov-suspicious-hits-threshold', '100000000000',
                    '--merge-mode-functions', 'merge-use-line-min',
                    '--json', str(base.with_suffix('.json')),
                    '--xml-pretty', '--xml', str(base.with_suffix('.xml')),
                    '--html-details', str(base / 'index.html'),
                    '--html-title', 'GUIX ' + base.name, '--print-summary', *arguments],
                   cwd=ROOT, check=True, timeout=600)
    relative_xml(base.with_suffix('.xml'))
    return reports(base)


def coverage(selected):
    """Collect each profile or merge exactly the profiles tested in this run."""
    metadata = json.loads((REPORT / 'profiles.json').read_text())
    names = metadata['profiles']
    validate(names)
    if selected != ['--merge']:
        if len(selected) != 1 or selected[0] not in names:
            raise ValueError('Coverage profile was not selected for testing')
        name = selected[0]
        objects = BUILD / name / 'guix' / 'CMakeFiles' / 'guix.dir'
        if not list(objects.rglob('*.gcda')):
            raise ValueError('No executed GUIX objects: ' + name)
        gcovr(['--object-directory', str(objects), str(objects)],
              REPORT / 'per_configuration' / name)
        return
    union = set()
    arguments = []
    contributions = {}
    for name in names:
        base = REPORT / 'per_configuration' / name
        lines = reports(base)
        contributions[name] = len(lines)
        union |= lines
        arguments += ['--add-tracefile', str(base.with_suffix('.json'))]
    complete = set(names) == set(profiles())
    base = REPORT / ('merged' if complete else 'partial')
    actual = gcovr(arguments, base)
    if union != actual:
        raise ValueError('Merged source-line denominator differs from input union')
    metadata.update(complete=complete, source_lines=len(union), contributions=contributions)
    (REPORT / 'profiles.json').write_text(json.dumps(metadata, indent=2))
    if complete:
        floors = json.loads((HERE / 'coverage_floors.json').read_text())
        xml = ET.parse(base.with_suffix('.xml')).getroot()
        for metric in ('line', 'branch'):
            measured = float(xml.attrib[metric + '-rate']) * 100
            if measured < floors[metric]:
                raise ValueError(f'{metric} coverage {measured:.4f}% is below {floors[metric]}%')
    else:
        print('PARTIAL coverage: ' + ', '.join(names))


def results(selected):
    """Record exact JUnit counts and elapsed durations for every selected profile."""
    summary = []
    failed = False
    for name in selected:
        path = BUILD / name / (name + '.xml')
        nonempty(path)
        (BUILD / name / 'Testing').mkdir(exist_ok=True)
        shutil.copyfile(path, BUILD / name / 'Testing' / 'JUnit.xml')
        suite = ET.parse(path).getroot()
        counts = {key: int(suite.attrib.get(key, 0)) for key in ('tests', 'failures', 'disabled', 'skipped')}
        if counts['tests'] != len(inventory(name)):
            raise ValueError('JUnit count does not match CTest inventory: ' + name)
        failed |= bool(counts['failures'] or counts['disabled'] or counts['skipped'])
        log = (BUILD / name / (name + '.txt')).read_text()
        elapsed = re.search(r'Total Test time \(real\) =\s*([0-9.]+)', log)
        summary.append(dict(profile=name, seconds=float(elapsed[1] if elapsed else suite.attrib['time']), **counts))
    (BUILD / 'results.txt').write_text(json.dumps(summary, indent=2))
    print(json.dumps(summary, indent=2))
    if failed:
        raise ValueError('CTest reported failing or unexecuted tests')


if __name__ == '__main__':
    try:
        command, *selection = sys.argv[1:]
        if command == 'profiles':
            print('\n'.join(profiles()))
        elif command in ('validate', 'audit', 'prepare', 'results'):
            validate(selection)
            globals()[command](selection)
        elif command == 'coverage':
            coverage(selection)
        else:
            raise ValueError('Unknown report command')
    except (ValueError, OSError, KeyError, subprocess.SubprocessError, ET.ParseError) as error:
        sys.exit(str(error))
