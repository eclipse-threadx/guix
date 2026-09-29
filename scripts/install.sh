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
timeout 180 sudo apt-get -o Acquire::Retries=3 -o Acquire::http::Timeout=30 update
timeout 360 sudo env DEBIAN_FRONTEND=noninteractive apt-get install -y \
    gcc-14 g++-14 gcc-14-multilib g++-14-multilib cmake ninja-build \
    python3-venv git unifdef p7zip-full tofrodos gawk
python3 -m venv .venv-ci
timeout 180 .venv-ci/bin/python -m pip install --disable-pip-version-check \
    --timeout 30 --retries 2 'gcovr==8.6'
gcc-14 --version
gcov-14 --version
.venv-ci/bin/gcovr --version
