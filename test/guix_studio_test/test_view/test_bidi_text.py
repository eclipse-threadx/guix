import os
import sys
import time
import test_utils
import test_constants
import logging

bidi_text_draw_32bpp_output_files = [
"bidi_text_draw_32bpp_resources.c",
"bidi_text_draw_32bpp_resources.h",
"bidi_text_draw_32bpp_specifications.c",
"bidi_text_draw_32bpp_specifications.h"
]

def get_test_header():

    notes =  "*            Studio Bidi Text Tests                   *\n"
    notes += "*                                                     *\n"
    notes += "* Test bidi text configure                            *\n"
    notes += "* Test generate reordered bidi text                   *\n"
    return notes

def run_bidi_text_config():
    # open test project
    test_utils.open_project('bidi_text_draw_32bpp.gxp')
    
    test_utils.configure_languages()
    
    test_utils.check_reorder_bidi_text(1)#Support bidi text is disabled, should not work
    test_utils.select_language_index(2)
    
    test_utils.check_support_bidi_text(0)
    test_utils.check_reorder_bidi_text(0)#Support bidi text is disabled, should not work
    
    test_utils.select_language_index(1)
    test_utils.check_support_bidi_text(1)
    test_utils.check_reorder_bidi_text(0)
    
    #save modifications
    test_utils.save_language_configure() 
    
    test_utils.compare_result()
    
    test_utils.configure_languages()
    test_utils.select_language_index(1)
    test_utils.check_reorder_bidi_text(1)
    
    #save modifications
    test_utils.save_language_configure() 
    
    # test output files
    test_utils.generate_all()
    test_utils.click_generate_button()
    test_utils.cmp_output_files(bidi_text_draw_32bpp_output_files)
    
def run_reordered_bidi_text_generation():
    test_utils.open_project('bidi_text_line_break_static.gxp')
    test_utils.generate_all()
    test_utils.click_generate_button()
    
    test_utils.select_project_tree_node("ml_view_Hebrew")
    test_utils.edit_widget_props("ID_STRING_ID", "ENGLISH")
    test_utils.generate_all()
    test_utils.click_generate_button()
    
def run_bidi_text_tests(generate, screenshot):

    test_utils.output_test_header(get_test_header()) 
    test_utils.setup(generate, screenshot, 'test_bidi_text_golden_file')
    
    print("Run bidi text configuration")
    run_bidi_text_config()
    
    print("Run reordered bidi text generation")
    run_reordered_bidi_text_generation()

    test_utils.write_end("Test Bidi Text")

