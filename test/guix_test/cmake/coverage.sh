#!/bin/bash

set -e

cd $(dirname $0)
mkdir -p coverage_report/$1

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
gcovr --gcov-suspicious-hits-threshold $suspicious_hits_threshold --object-directory=build/$1/guix/CMakeFiles/guix.dir/common/src -r ../../../common/src --xml-pretty --output coverage_report/$1.xml
gcovr --gcov-suspicious-hits-threshold $suspicious_hits_threshold --object-directory=build/$1/guix/CMakeFiles/guix.dir/common/src -r ../../../common/src --html --html-details --output coverage_report/$1/index.html
