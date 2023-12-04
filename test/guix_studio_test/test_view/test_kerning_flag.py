# -*- coding: utf-8 -*-
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
