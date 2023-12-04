# -*- coding: utf-8 -*-
import os
import sys
import time
import test_utils

def get_test_header():
    notes =  "*          Text Input Text Set Test                   *\n"
    notes += "*                                                     *\n"
    notes += "* Set Chinese text(larger than input buffer size) to  *\n"
    notes += "*   multi line text input                             *\n"
    notes += "* Set Chinese text(larger than input buffer size) to  *\n"
    notes += "*   single line text input                            *\n"
    notes += "* Resize multi line text input to make the text total *\n"
    notes += "*   lines larger than maximum cached lines            *\n"
    notes += "* Set English text(larger than input buffer size) to  *\n"
    notes += "*   multi line text input                             *\n"
    notes += "* Set English text(larger than input buffer size) to  *\n"
    notes += "*   single line text input                            *\n"
    notes += "* Test if input text update when edit string table    *\n"
    return notes

def run_text_input_text_set_tests(generate, screenshot):

    test_utils.output_test_header(get_test_header())
    test_utils.setup(generate, screenshot, 'test_text_input_text_set_golden_file')

    test_utils.open_project('test_text_input_text_set.gxp')
    
    # set Chinese text that is larger than input buffer size to ML input widget
    test_utils.select_project_tree_node('ml_text_input')
    test_utils.edit_widget_props("ID_STRING_ID", "MULTI_LINE_TEXT")
    test_utils.compare_result()
    
    # resize multi line text input to make the text total lines larger than maximum cached lines
    test_utils.select_project_tree_node('ml_text_input')
    test_utils.edit_widget_props("ID_WIDTH", 69)
    test_utils.compare_result()
    
    # set Chinese text that is larger than input buffer size to SL input widget
    test_utils.select_project_tree_node('sl_text_input')
    test_utils.edit_widget_props("ID_STRING_ID", "SINGLE_LINE_TEXT")
    test_utils.compare_result()
    
    # set English text that is larger than input buffer size
    test_utils.select_project_tree_node('ml_text_input')
    test_utils.edit_widget_props('ID_BUFFER_SIZE', 3)
    
    test_utils.edit_string()
    test_utils.select_string(0)
    test_utils.edit_top_string('123')
    test_utils.select_string(0)
    test_utils.save_string_edit()
    test_utils.compare_result()
    
    test_utils.edit_string()
    test_utils.select_string(0)
    test_utils.edit_top_string('000')
    test_utils.select_string(0)
    test_utils.cancel_string_edit()
    test_utils.compare_result()
    
    # set English text that is larget than input buffer size to SL input widget
    test_utils.select_project_tree_node('sl_text_input')
    test_utils.edit_widget_props('ID_BUFFER_SIZE', 3)
    
    test_utils.edit_string()
    test_utils.select_string(1)
    test_utils.edit_top_string('123')
    test_utils.select_string(1)
    test_utils.save_string_edit()
    test_utils.compare_result()
    
    test_utils.edit_string()
    test_utils.select_string(1)
    test_utils.edit_top_string('000')
    test_utils.select_string(1)
    test_utils.cancel_string_edit()
    test_utils.compare_result()
    
    test_utils.write_end('Text Input Text Set Tests')