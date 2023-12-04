import os
import sys
import time
import test_utils
import test_constants
import logging

trigger_target_rename_output_files = [
"trigger_target_rename_specifications.c",
"trigger_target_rename_specifications.h"
]

def get_test_header():

    notes =  "*      Studio Trigger Target Rename Tests             *\n"
    notes += "*                                                     *\n"
    notes += "* Test rename widget name that is referenced by a     *\n"
    notes += "*   trigger                                           *\n"
    return notes

def run_trigger_target_rename_tests(generate, screenshot):

    test_utils.output_test_header(get_test_header()) 
    test_utils.setup(generate, screenshot, 'test_trigger_target_rename_golden_file')
    
    # open test project
    test_utils.open_project('trigger_target_rename.gxp')
    
    test_utils.select_project_tree_node('popup_modal')
    test_utils.edit_widget_props_post('ID_WIDGET_NAME', 'popup_modal_test_name')
    
    # test output files
    test_utils.generate_all()
    test_utils.click_generate_button()
    test_utils.cmp_output_files(trigger_target_rename_output_files)
    
    test_utils.write_end("Test Trigger Target Rename")
    

