#!/bin/bash
# Free space before and after the run. Every configuration is built and
# instrumented, and the build trees, coverage data and reports only grow until
# the run ends, so the figure after it is the run's peak. Measured locally, that
# is about 17 GB, against the 14 GB GitHub documents for a hosted runner's disk.
df -h / .
cmake_dir=$(dirname `realpath $0`)/../test/guix_test/cmake
# The bootstrap takes its serial configuration loop when CTEST_PARALLEL_LEVEL is
# set, and passes the value on to ctest.
CTEST_PARALLEL_LEVEL=4 $cmake_dir/run.sh test all
status=$?
df -h / .
# build.sh always succeeds and leaves its real status here. A missing file means
# the build never reached that point, which counts as a failed build.
build_status=$(cat $cmake_dir/build/build_status 2>/dev/null || echo 1)
echo "Build status: $build_status, test status: $status"
[ "$build_status" = 0 ] || [ "$status" != 0 ] || status=1
exit $status
