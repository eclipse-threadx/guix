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
import test_constants
import logging

def get_test_header():
    notes =  "*            Studio Template Tests                    *\n"
    notes += "*                                                     *\n"
    notes += "* Test range check                                    *\n"
    return notes

def run_range_check_tests(generate, screenshot):

    test_utils.output_test_header(get_test_header()) 
    test_utils.setup(generate, screenshot, 'test_range_check_golden_file')
    
    # open test project
    test_utils.open_project('test_range_check.gxp')
    test_utils.select_project_tree_node('window')
    test_utils.compare_result()
    
    test_utils.write_end("Test Range Check")
    

