#!/bin/bash

set -e

cd $(dirname $0)
mkdir -p coverage_report/$1

# gcovr treats a hit count at or above 2^32 as gcov corruption (GCC PR 68080)
# and reports the line as uncovered. Accumulating 730 test runs into one set of
# .gcda takes three pixel-fill loops in the horizontal line drivers past 8.5e9,
# which is a real count and not corruption: left at the default the report loses
# 6 lines and 3 branches that the suite does cover. The bar is raised rather
# than the check disabled, and negative-hit detection -- which is what actually
# catches PR 68080 -- is untouched.
suspicious_hits_threshold=9223372036854775808
gcovr --gcov-suspicious-hits-threshold $suspicious_hits_threshold --object-directory=build/$1/guix/CMakeFiles/guix.dir/common/src -r ../../../common/src --xml-pretty --output coverage_report/$1.xml
gcovr --gcov-suspicious-hits-threshold $suspicious_hits_threshold --object-directory=build/$1/guix/CMakeFiles/guix.dir/common/src -r ../../../common/src --html --html-details --output coverage_report/$1/index.html
