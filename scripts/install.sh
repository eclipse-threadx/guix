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

# Every command below reaches the network, and each already carries its own
# timeout. What was missing is a second attempt: a single slow mirror minute
# ended the run, and the package install is 41 MB across 34 packages, so it is
# the one that runs out. Three attempts with a widening pause between them.
#
# The worst case is no worse than before. If all three attempts fail the run
# fails, as it did with one; what the loop buys is the common case, where the
# mirror is slow once and not twice, and a log that names the attempt.
retry() {
    local attempt
    for attempt in 1 2 3; do
        if "$@"; then
            return 0
        fi
        echo "install.sh: '$*' failed or timed out on attempt ${attempt}" >&2
        sleep $((attempt * 10))
    done
    echo "install.sh: '$*' failed after 3 attempts" >&2
    return 1
}

retry timeout 180 sudo apt-get -o Acquire::Retries=3 -o Acquire::http::Timeout=30 update
retry timeout 360 sudo env DEBIAN_FRONTEND=noninteractive apt-get install -y \
    gcc-14 g++-14 gcc-14-multilib g++-14-multilib cmake ninja-build \
    python3-venv git unifdef p7zip-full tofrodos gawk
python3 -m venv .venv-ci
retry timeout 180 .venv-ci/bin/python -m pip install --disable-pip-version-check \
    --timeout 30 --retries 2 'gcovr==8.6'
gcc-14 --version
gcov-14 --version
.venv-ci/bin/gcovr --version
