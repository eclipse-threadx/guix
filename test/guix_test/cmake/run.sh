#!/bin/bash
# Copyright (c) 2024 Microsoft Corporation
# Copyright (c) 2026 Eclipse ThreadX contributors
#
# This program and the accompanying materials are made available under the
# terms of the MIT License which is available at
# https://opensource.org/licenses/MIT.
#
# SPDX-License-Identifier: MIT

set -euo pipefail
cd -- "$(dirname -- "${BASH_SOURCE[0]}")"
export CC=gcc-14 CXX=g++-14
export CMAKE_BUILD_PARALLEL_LEVEL=${CMAKE_BUILD_PARALLEL_LEVEL:-4}
if [[ ${GX_CI_CONCURRENT_TESTS:-0} == 1 ]]; then
    unset CTEST_PARALLEL_LEVEL
else
    export CTEST_PARALLEL_LEVEL=${CTEST_PARALLEL_LEVEL:-4}
fi
export CTEST_REPEAT_FAIL=1
export TX_COVERAGE=ON
revision=b37cd4a81a1cb8c2ebefc438220ab7f009e13362

# Serialize checkout and dependency builds, including recursive CMake calls.
mkdir -p build
exec 9>build/dependency.lock
flock -w 300 9
fresh_checkout=false
if [[ ! -d threadx/.git ]]; then
    timeout 180 git clone --no-checkout https://github.com/eclipse-threadx/threadx.git threadx
    fresh_checkout=true
fi
if ! git -C threadx cat-file -e "$revision^{commit}"; then
    timeout 180 git -C threadx fetch origin "$revision"
fi
if [[ $fresh_checkout == false && -n $(git -C threadx status --porcelain --untracked-files=no) ]]; then
    echo 'ThreadX checkout contains local modifications.' >&2
    exit 1
fi
git -C threadx checkout --quiet --detach "$revision"
[[ $(git -C threadx rev-parse HEAD) == "$revision" ]]
ln -sfn threadx/scripts/cmake_bootstrap.sh .run.sh
if [[ ${1:-} == build_libs ]]; then
    timeout 300 ./.run.sh build_libs
    exit
fi
flock -u 9
exec 9>&-

command=${1:-}
[[ $command == build || $command == test ]] || { echo 'Usage: run.sh build|test [all|profiles...]' >&2; exit 2; }
shift
mapfile -t available < <(python3 report.py profiles)
if [[ $# == 0 ]]; then
    selected=("${available[0]}")
elif [[ $* == all ]]; then
    selected=("${available[@]}")
else
    selected=("$@")
fi
python3 report.py validate "${selected[@]}"
printf '%s\n' "${selected[@]}" > "build/${command}-profiles.txt"
status=0
if [[ $command == build ]]; then
    : > build/build-times.txt
    for profile in "${selected[@]}"; do
        start=$SECONDS
        timeout 900 ./.run.sh build "$profile" > "build/$profile-build.txt" 2>&1 || status=$?
        cat "build/$profile-build.txt"
        printf '%s\t%d\n' "$profile" "$((SECONDS-start))" >> build/build-times.txt
    done
    python3 report.py audit "${selected[@]}" || status=$?
else
    python3 report.py prepare "${selected[@]}"
    timeout 3300 ./.run.sh test "${selected[@]}" 2>&1 | tee build/test.txt || status=$?
    python3 report.py results "${selected[@]}" || status=$?
fi
exit "$status"
