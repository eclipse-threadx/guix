# Copyright (c) 2024 Microsoft Corporation
# Copyright (c) 2026 Eclipse ThreadX contributors
#
# This program and the accompanying materials are made available under the
# terms of the MIT License which is available at
# https://opensource.org/licenses/MIT.
#
# SPDX-License-Identifier: MIT

# Name of the target
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m3)

set(THREADX_ARCH "cortex_m3")
set(THREADX_TOOLCHAIN "gnu")

set(MCPU_FLAGS "-mthumb -mcpu=cortex-m3")
set(VFP_FLAGS "")
set(SPEC_FLAGS "--specs=nosys.specs")
# set(LD_FLAGS "-nostartfiles")

include(${CMAKE_CURRENT_LIST_DIR}/arm-none-eabi.cmake)