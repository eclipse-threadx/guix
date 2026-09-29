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
cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.."
python3 test/guix_test/cmake/test_report.py
if [[ $# == 0 ]]; then set -- all; fi
exec test/guix_test/cmake/run.sh build "$@"
