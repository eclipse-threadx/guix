#!/bin/bash
# Copyright (c) 2026 Eclipse ThreadX contributors
#
# This program and the accompanying materials are made available under the
# terms of the MIT License which is available at
# https://opensource.org/licenses/MIT.
#
# SPDX-License-Identifier: MIT

set -euo pipefail
cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.."
mapfile -t profiles < <(python3 test/guix_test/cmake/report.py profiles)
[[ ${#profiles[@]} -ge 7 && ${profiles[0]} == default_build_coverage ]]
./scripts/build.sh "${profiles[@]:6}"
cp test/guix_test/cmake/build/build-profiles.txt test/guix_test/cmake/build/final-build-profiles.txt
cp test/guix_test/cmake/build/build-times.txt test/guix_test/cmake/build/final-build-times.txt
GX_CI_CONCURRENT_TESTS=1 ./scripts/test.sh all
