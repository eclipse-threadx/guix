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
./scripts/install.sh
./scripts/build.sh default_build_coverage
cp test/guix_test/cmake/build/build-profiles.txt test/guix_test/cmake/build/install-build-profiles.txt
cp test/guix_test/cmake/build/build-times.txt test/guix_test/cmake/build/install-build-times.txt
