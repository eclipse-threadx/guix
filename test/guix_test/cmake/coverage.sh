#!/bin/bash
# Copyright (c) 2024 Microsoft Corporation
# Copyright (c) 2026 Eclipse ThreadX contributors
#
# This program and the accompanying materials are made available under the
# terms of the MIT License which is available at
# https://opensource.org/licenses/MIT.
#
# SPDX-License-Identifier: MIT


set -e

cd $(dirname $0)
mkdir -p coverage_report/$1
gcovr --object-directory=build/$1/guix/CMakeFiles/guix.dir/common/src -r ../../../common/src --xml-pretty --output coverage_report/$1.xml
gcovr --object-directory=build/$1/guix/CMakeFiles/guix.dir/common/src -r ../../../common/src --html --html-details --output coverage_report/$1/index.html
