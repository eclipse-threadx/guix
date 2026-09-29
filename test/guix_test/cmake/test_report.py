#!/usr/bin/env python3
# Copyright (c) 2026 Eclipse ThreadX contributors
# SPDX-License-Identifier: MIT
"""Regression tests for coverage artifacts and profile selection."""
import json
from pathlib import Path
import tempfile
import unittest
from unittest import mock
import report


class CoverageReports(unittest.TestCase):
    """Exercise report validation using independent minimal artifacts."""

    def setUp(self):
        """Isolate generated artifacts from real measurements."""
        self.directory = tempfile.TemporaryDirectory()
        self.addCleanup(self.directory.cleanup)
        self.root = Path(self.directory.name)
        self.patch = mock.patch.multiple(report, HERE=self.root, BUILD=self.root / 'build', REPORT=self.root / 'coverage_report')
        self.patch.start()
        self.addCleanup(self.patch.stop)

    def artifact(self, name='one', lines=(10, 20)):
        """Write matching JSON, Cobertura, and HTML input reports."""
        base = report.REPORT / 'per_configuration' / name
        base.mkdir(parents=True, exist_ok=True)
        base.with_suffix('.json').write_text(json.dumps({'files': [{
            'file': 'common/src/example.c', 'lines': [{'line_number': n, 'count': 1} for n in lines]}]}))
        base.with_suffix('.xml').write_text('<coverage><sources><source>.</source></sources><packages>'
            '<package><classes><class filename="common/src/example.c"><lines>' +
            ''.join(f'<line number="{n}" hits="1"/>' for n in lines) +
            '</lines></class></classes></package></packages></coverage>')
        (base / 'index.html').write_text('<html>coverage</html>')
        return base

    def test_missing_and_empty_formats(self):
        """Every expected format must exist and contain data."""
        for suffix in ('.json', '.xml', '/index.html'):
            for missing in (True, False):
                with self.subTest(suffix=suffix, missing=missing):
                    base = self.artifact()
                    path = Path(str(base) + suffix)
                    path.unlink() if missing else path.write_text('')
                    with self.assertRaises(ValueError):
                        report.reports(base)

    def test_missing_html_detail(self):
        """A summary cannot link to a missing source detail page."""
        base = self.artifact()
        (base / 'index.html').write_text('<a href="missing.html">source</a>')
        with self.assertRaises(ValueError):
            report.reports(base)

    def test_empty_denominator(self):
        """An empty trace must not be accepted as full coverage."""
        with self.assertRaisesRegex(ValueError, 'Empty source-line'):
            report.reports(self.artifact(lines=()))

    def test_mismatched_xml(self):
        """XML cannot silently omit lines present in raw JSON."""
        base = self.artifact()
        path = base.with_suffix('.xml')
        path.write_text(path.read_text().replace('<line number="20" hits="1"/>', ''))
        with self.assertRaisesRegex(ValueError, 'denominators differ'):
            report.reports(base)

    def test_external_paths(self):
        """Only repository-relative common source paths are admissible."""
        for name in ('/common/src/example.c', 'common/src/../example.c', 'test/example.c'):
            base = self.artifact()
            path = base.with_suffix('.json')
            path.write_text(path.read_text().replace('common/src/example.c', name))
            with self.assertRaises(ValueError):
                report.reports(base)

    def test_union_and_partial_selection(self):
        """Partial selections retain their exact union without a complete label."""
        first = self.artifact('one', (10, 20))
        self.artifact('two', (20, 30))
        (report.REPORT / 'profiles.json').write_text(json.dumps({'profiles': ['one', 'two']}))
        expected = {('common/src/example.c', line) for line in (10, 20, 30)}
        with mock.patch.object(report, 'profiles', return_value=['one', 'two', 'three']):
            with mock.patch.object(report, 'gcovr', return_value=expected) as collect:
                report.coverage(['--merge'])
                self.assertEqual(collect.call_args.args[1].name, 'partial')
                self.assertFalse(json.loads((report.REPORT / 'profiles.json').read_text())['complete'])
            with mock.patch.object(report, 'gcovr', return_value=report.trace_lines(first.with_suffix('.json'))):
                with self.assertRaisesRegex(ValueError, 'differs from input union'):
                    report.coverage(['--merge'])

    def test_missing_profile_aborts_merge(self):
        """A merge cannot hide a selected profile that produced no report."""
        self.artifact('one')
        (report.REPORT / 'profiles.json').write_text(json.dumps({'profiles': ['one', 'two']}))
        with mock.patch.object(report, 'profiles', return_value=['one', 'two']):
            with mock.patch.object(report, 'gcovr') as collect:
                with self.assertRaises(ValueError):
                    report.coverage(['--merge'])
                collect.assert_not_called()

    def test_stale_reports_and_counters(self):
        """A subset cannot retain test or coverage artifacts from earlier profiles."""
        stale = []
        for name in ('one', 'two'):
            for suffix in (name + '.xml', name + '.txt', 'Testing/JUnit.xml',
                           'Testing/old/Test.xml', 'guix/object.gcda'):
                path = report.BUILD / name / suffix
                path.parent.mkdir(parents=True, exist_ok=True)
                path.write_text('stale')
                stale.append(path)
            path = report.BUILD / (name + '.txt')
            path.write_text('stale')
            stale.append(path)
        for name in ('results.txt', 'test.txt'):
            path = report.BUILD / name
            path.write_text('stale')
            stale.append(path)
        self.artifact('old')
        with mock.patch.object(report, 'profiles', return_value=['one', 'two']), \
             mock.patch.object(report, 'audit'), mock.patch.object(report, 'inventory'):
            report.prepare(['one'])
        self.assertTrue(all(not path.exists() for path in stale))
        self.assertFalse((report.REPORT / 'per_configuration').exists())
        self.assertEqual(json.loads((report.REPORT / 'profiles.json').read_text()),
                         {'profiles': ['one'], 'complete': False})

    def test_complete_coverage_floors(self):
        """Complete runs enforce line and branch floors independently."""
        base = self.artifact('one')
        expected = report.trace_lines(base.with_suffix('.json'))
        (report.HERE / 'coverage_floors.json').write_text('{"line": 90, "branch": 80}')
        (report.REPORT / 'profiles.json').write_text('{"profiles": ["one"]}')
        with mock.patch.object(report, 'profiles', return_value=['one']), \
             mock.patch.object(report, 'gcovr', return_value=expected):
            for line, branch, failure in ((0.89, 1, 'line'), (1, 0.79, 'branch'), (0.9, 0.8, None)):
                (report.REPORT / 'merged.xml').write_text(
                    f'<coverage line-rate="{line}" branch-rate="{branch}"/>')
                if failure:
                    with self.assertRaisesRegex(ValueError, failure + ' coverage'):
                        report.coverage(['--merge'])
                else:
                    report.coverage(['--merge'])

    def test_profile_selection(self):
        """Reject unknown, repeated, and empty manual selections."""
        with mock.patch.object(report, 'profiles', return_value=['one', 'two']):
            for selected in ([], ['unknown'], ['one', 'one']):
                with self.assertRaises(ValueError):
                    report.validate(selected)
            report.validate(['two'])


if __name__ == '__main__':
    unittest.main()
