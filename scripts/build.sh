#!/bin/bash
# Free space on the runner before anything is built, so the margin the full run
# leaves can be read off the log: test.sh prints it again before and after.
df -h / .
cmake_dir=$(dirname `realpath $0`)/../test/guix_test/cmake
$cmake_dir/run.sh build all
status=$?
# Some test targets do not compile in every configuration, so on this branch the
# build is expected to fail, and a failed step would skip the test step and with
# it every configuration's results and the coverage merge. The status is kept for
# test.sh, which fails if the build did. A target that did not build also shows
# in the test step as Not Run.
mkdir -p $cmake_dir/build || exit 1
echo $status > $cmake_dir/build/build_status
echo "Build status: $status"
exit 0
