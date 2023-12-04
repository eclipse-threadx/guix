import os
import sys
import time
import test_utils
import test_constants
import logging

def get_test_header():

    notes =  "*                     App Run Tests                   *\n"
    notes += "*                                                     *\n"
    notes += "* Test run application in studio                      *\n"
    return notes

def run_app_tests(generate, screenshot):

    test_utils.output_test_header(get_test_header()) 
    test_utils.setup(generate, screenshot, 'test_app_run_golden_file')
    
    # open test project
    test_utils.open_project('test_app_run.gxp')
    
    test_utils.select_project_tree_node('main_window')
    
    test_utils.run_application()
    test_utils.terminate_app_execution()
    test_utils.compare_result()
    
    test_utils.select_project_tree_node('second_main_window')
    test_utils.compare_result()
    
    test_utils.run_application()
    test_utils.terminate_app_execution()
    test_utils.compare_result()
    
    test_utils.write_end("Test App Run")

