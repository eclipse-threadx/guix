#!/bin/bash

set -e

cd $(dirname $0)

# gcovr 8.6 aborts a report outright when a line's execution count exceeds its
# "suspicious hits" threshold, which defaults to 2**32. The heuristic exists to
# catch the garbage gcov emits under GCC PR 68080, and here it fires on correct
# data instead: the inner pixel-fill loops of the 8bpp, 16bpp and 32bpp
# horizontal line drivers reach 8,510,851,270 once 730 test runs accumulate into
# one set of .gcda. That is 2.0x the default and nine orders of magnitude below
# the 2**63 range a wrapped counter lands in.
#
# The threshold is raised rather than the error ignored.
# --gcov-ignore-parse-errors=suspicious_hits.* does not skip the check, it sets
# the offending line's count to *zero*, so a covered line is reported uncovered.
# Measured here: it costs 6 lines and 3 branches the suite does cover.
#
# 2**40 is bounded on both sides and matches the value the other suites in this
# baseline use. It is ~11x above what this machine can physically count in a
# suite that runs for ~100 seconds, so it cannot fire on real data; and it is
# ~8.4 million times below 2**63, so wrapped-counter garbage is still caught.
# Nothing here reads a count's magnitude -- every figure derives from count > 0.
suspicious_hits_threshold=1099511627776

# Both -r and -f are absolute. A relative root resolved against whatever
# directory gcovr was started in is how a report containing zero files and an
# exit status of 0 is produced. Rooting every configuration at the repository
# root also makes the file names repo-relative, so the same source file is named
# identically in every configuration's report -- which is what lets --merge
# below union them at all, and what lets coverage_union.py key on the name.
repo_root=$(cd ../../.. && pwd)
filter=$repo_root/common/src

# The configurations the merge must contain, read from CMakeLists.txt with the
# parser the shared runner uses to decide what to build, so the two cannot
# disagree about what the configuration set is.
configurations()
{
    sed -n "/(BUILD_CONFIGURATIONS/,/)/p" CMakeLists.txt \
        | sed ':label;N;s/\n/ /;b label' \
        | grep -Pzo "[a-zA-Z0-9_]*build[a-zA-Z0-9_]*\s*" | tr -d '\0'
}

# Configurations that are built, tested and reported, but whose coverage does
# not enter the merge. This is the one place the exclusion is declared: the
# per-configuration report goes to per_configuration/informational/, a path the
# merge never reads, and the merge's expected set leaves these out by reading
# the same list.
#
# _5_4_0_compatible_no_checking_build cannot build its only test. Three of
# gx_api.h's 5.4.0 macros on the unchecked path are wrong, so the library
# compiles and nothing that calls it does, and its report is the whole
# library at zero. Left in the merge it would add nothing to the union while
# sitting in the expected set as though it contributed. The 5.4.0 code it
# compiles is compiled and covered by _5_4_0_compatible_build. It leaves this
# list when the header compiles in this configuration.
INFORMATIONAL="_5_4_0_compatible_no_checking_build"

informational()
{
    case " $INFORMATIONAL " in
        *" $1 "*) return 0 ;;
    esac
    return 1
}

if [ "$1" = "--merge" ]; then
    # The merge set has to be complete, and that is asserted rather than
    # trusted. Instrumentation has two switches -- TX_COVERAGE, and the
    # build-type name match kept for a configuration built by hand -- so a run
    # with TX_COVERAGE unset still instruments and reports the two
    # configurations whose names end in _coverage. Without this check that run
    # would union two of eighteen, print a plausible percentage, and nothing
    # would say it was not GUIX's figure. The same applies to a run in which one
    # configuration failed to build and left no tracefile.
    #
    # The inputs are the expected names, not a glob of the directory.
    # coverage_report/ is not cleaned between runs, so a tracefile left by an
    # earlier run of a configuration that has since been renamed or removed
    # would otherwise be merged in without anything saying so.
    inputs=()
    absent=()
    expected=0
    for configuration in $(configurations); do
        informational "$configuration" && continue
        expected=$((expected + 1))
        tracefile=coverage_report/per_configuration/$configuration.json
        if [ -f "$tracefile" ]; then
            inputs+=("$tracefile")
        else
            absent+=("$configuration")
        fi
    done

    name=merged
    if [ ${#absent[@]} -ne 0 ]; then
        echo "coverage.sh --merge: the merge set is incomplete -- ${#inputs[@]} tracefile(s)" >&2
        echo "for $expected configurations. Missing:" >&2
        for configuration in "${absent[@]}"; do
            echo "    $configuration" >&2
        done
        echo "A union over part of the configuration set is not the GUIX figure." >&2
        echo "Re-run the missing configurations with TX_COVERAGE=ON, or set" >&2
        echo "GX_COVERAGE_ALLOW_PARTIAL=1 to compute an explicitly partial figure." >&2
        [ "${GX_COVERAGE_ALLOW_PARTIAL:-0}" = "1" ] || exit 1
        [ ${#inputs[@]} -ne 0 ] || exit 1

        # A partial figure is useful while iterating on one feature, and it must
        # never be able to occupy the path the complete figure occupies. So it is
        # written under a different name rather than flagged on an otherwise
        # identical artefact: a stderr line does not survive being copied into an
        # evidence pack, and a file name does.
        name=merged_PARTIAL
        echo "coverage.sh --merge: PARTIAL -- ${#inputs[@]} of $expected configurations." >&2
        echo "Writing $name.xml. This is not the GUIX coverage figure." >&2
    fi

    add_args=()
    for t in "${inputs[@]}"; do
        add_args+=(--add-tracefile "$t")
    done

    # gcovr's own merged report, published unchanged beside the union below. Its
    # branch denominator is not the source's: --add-tracefile keys each branch by
    # the basic-block pair gcov assigned it, and a file that compiles to a
    # different amount of code in two configurations has its blocks renumbered,
    # so one source branch arrives under several identities. Publishing only the
    # union would put a post-processed number in the evidence with the tool's
    # output withheld; publishing only the merged figure would quote a
    # denominator that is not the source's. So both.
    #
    # No suspicious-hits threshold here, and none is needed. That check runs
    # where gcovr parses gcov's output, which is the per-configuration pass
    # below; reading tracefiles back in does not repeat it. Measured: a
    # tracefile carrying a count of 11,100,173,915 merges with exit status 0 at
    # gcovr's default.
    mkdir -p coverage_report/$name
    gcovr -r "$repo_root" "${add_args[@]}" --xml-pretty --output coverage_report/$name.xml
    gcovr -r "$repo_root" "${add_args[@]}" --html --html-details --output coverage_report/$name/index.html

    if ! grep -q "<class " coverage_report/$name.xml; then
        echo "coverage.sh --merge: the merged report contains no files." >&2
        exit 1
    fi

    echo "coverage.sh --merge: $name, ${#inputs[@]} configuration(s):"
    for t in "${inputs[@]}"; do
        echo "    $(basename "$t" .json)"
    done

    # The gate is a ratchet: it sits at the union the complete run measures on
    # CI, 46376/47112 lines and 25452/26110 branches over 1310 files, which
    # coverage_union.py prints rounded down to 98.43% and 97.47%. It is compared
    # against the unrounded ratio, so a run passes at this figure and fails if a
    # single line or branch it covers is lost, and it is raised as coverage
    # grows. The same figure came out of a workstation run and a local run in
    # the pinned container. It is the union, not gcovr's merged report, because
    # the union counts branches in the source while the merged report counts one
    # per basic-block numbering.
    min_line=${GX_COVERAGE_MIN_LINE:-98.43}
    min_branch=${GX_COVERAGE_MIN_BRANCH:-97.47}

    # Run after the reports are written, so a failed check still leaves behind
    # the reports that explain it. Staged, so the union reads exactly the files
    # merged above and nothing else the directory happens to hold.
    staged=$(mktemp -d)
    trap 'rm -rf "$staged"' EXIT
    for t in "${inputs[@]}"; do
        cp "$t" "$staged/"
    done
    python3 ./coverage_union.py "$staged" "$min_line" "$min_branch"
    exit $?
fi

# The positional search path is what scopes the report to one configuration.
# --object-directory does not: with -r at the repository root gcovr still walks
# the whole tree, finds every configuration's .gcda and reports their union, so
# every configuration would produce the same report. It is absolute for the
# same reason -r is.
objdir=$PWD/build/$1/guix/CMakeFiles/guix.dir/common/src

# Per-configuration output sits in a subdirectory of its own and the merged
# report at the top, so the top level holds exactly the report a publisher
# expects and per-configuration directories cannot collide with it.
out=coverage_report/per_configuration
informational "$1" && out=$out/informational
mkdir -p $out/$1
gcovr --gcov-suspicious-hits-threshold $suspicious_hits_threshold \
      -r "$repo_root" -f "$filter" "$objdir" \
      --json $out/$1.json \
      --xml-pretty --output $out/$1.xml
gcovr --gcov-suspicious-hits-threshold $suspicious_hits_threshold \
      -r "$repo_root" -f "$filter" "$objdir" \
      --html --html-details --output $out/$1/index.html

# An empty report is not an error as far as gcovr is concerned: it warns, exits
# 0, and advertises line-rate="1.0" alongside lines-valid="0", so every
# downstream consumer reads "no data at all" as "100% covered". No threshold can
# catch that, so the assertion belongs here, next to the paths that would cause
# it.
if ! grep -q "<class " $out/$1.xml; then
    echo "coverage.sh: the report for '$1' contains no files." >&2
    echo "Expected gcda files under $objdir." >&2
    exit 1
fi
