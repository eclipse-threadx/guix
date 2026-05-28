#!/bin/bash
# Copyright (c) 2024 Microsoft Corporation
# Copyright (c) 2026 Eclipse ThreadX contributors
#
# This program and the accompanying materials are made available under the
# terms of the MIT License which is available at
# https://opensource.org/licenses/MIT.
#
# SPDX-License-Identifier: MIT


cd $(dirname $0)

# if threadx repo does not exist, clone it
[ -d threadx ] ||
git clone https://github.com/eclipse-threadx/threadx.git

[ -f .run.sh ] || ln -sf threadx/scripts/cmake_bootstrap.sh .run.sh
./.run.sh $*

#!/bin/bash
