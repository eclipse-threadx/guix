# -*- coding: utf-8 -*-
# Copyright (c) 2024 Microsoft Corporation
# Copyright (c) 2026 Eclipse ThreadX contributors
#
# This program and the accompanying materials are made available under the
# terms of the MIT License which is available at
# https://opensource.org/licenses/MIT.
#
# SPDX-License-Identifier: MIT

import os
import sys
import time
import test_utils

project_name = "test_font"
display_name = "main_display"

def get_test_header(): 
    notes =  "*          Studio Font Kerning Flag Test              *\n"
    notes += "*                                                     *\n"
    notes += "* Test font kerning flag                              *\n"
    return notes
    
def run_kerning_flag_tests(generate, screenshot):

    test_utils.output_test_header(get_test_header())
    test_utils.setup(generate, screenshot, 'test_kerning_flag_golden_file')
    
    test_utils.open_project("test_kerning_flag.gxp")
    test_utils.compare_result()
    
    test_utils.write_end('Kerning Flag Tests')
