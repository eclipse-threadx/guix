import os
import sys
import time
import test_utils
import test_constants
import logging

test_theme_edit_output_files = [
"test_theme_edit_resources.c",
"test_theme_edit_resources.h",
"test_theme_edit_specifications.c",
"test_theme_edit_specifications.h"]

def get_test_header():
    notes =  "*            Studio Theme Edit Tests                  *\n"
    notes += "*                                                     *\n"
    notes += "* Test edit theme                                     *\n"
    return notes

def run_theme_edit_tests(generate, screenshot):

    test_utils.output_test_header(get_test_header()) 
    test_utils.setup(generate, screenshot, 'test_theme_edit_golden_file')
    
    # open test project
    test_utils.open_project('test_theme_edit.gxp')
    
    # add theme 2
    test_utils.configure_themes()
    test_utils.add_theme()
    test_utils.set_active_theme("theme_2")
    test_utils.select_theme_index(2)
    
    # edit palette for theme 2 and save changes
    test_utils.edit_palette()
    
    test_utils.set_palette_color(0, 255, 100, 100)
    test_utils.save_palette_edit()
    test_utils.save_theme_configure()
    
    test_utils.compare_result()
    
    # add theme 3
    test_utils.configure_themes()
    test_utils.add_theme()
    test_utils.set_active_theme("theme_3")
    test_utils.select_theme_index(3)
    
    # edit palette for theme 2, save palette change, cancel theme edit
    test_utils.edit_palette()
    
    test_utils.set_palette_color(0, 255, 0, 0)
    test_utils.save_palette_edit()
    test_utils.cancel_theme_configure()
    
    test_utils.compare_result()
    
    # add theme 3
    test_utils.configure_themes()
    test_utils.add_theme()
    test_utils.set_active_theme("theme_3")
    test_utils.select_theme_index(3)
    
    # edit palette for theme 2, cancel palette change, save theme edit
    test_utils.edit_palette()
    
    test_utils.set_palette_color(0, 255, 0, 0)
    test_utils.cancel_palette_edit()
    test_utils.save_theme_configure()
    
    test_utils.compare_result()
    
    test_utils.generate_all()
    test_utils.click_generate_button()
    test_utils.cmp_output_files(test_theme_edit_output_files)
    
    test_utils.write_end("Test Theme Edit")
    

