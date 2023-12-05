import os
import sys
import time
import test_utils
import test_constants
import logging

def get_test_header():
    notes =  "*         Studio Language Edit Tests                  *\n"
    notes += "*                                                     *\n"
    notes += "* Test edit language                                  *\n"
    notes += "* Test thai glyph shaping setting                     *\n"
    return notes

def run_new_project_create(name):
    # create a new project
    test_utils.toolbar_create()
    test_utils.set_new_project_path(test_utils.DEFAULT_OUTPUT_FILE_PATH)
    test_utils.set_new_project_name(name)
    test_utils.save_new_project_create()
    
    # configure project
    test_utils.configure_project()
    test_utils.configure_x_res(640)
    test_utils.configure_y_res(480)
    test_utils.configure_display_name('display_1')
    test_utils.configure_canvas_allocate(0);
    test_utils.save_project_configure()
    
    # close message dialog when create new and delete default folder
    test_utils.close_message_dialog()
    test_utils.select_project_tree_node("default_folder")
    test_utils.add_widget("GX_TYPE_PROMPT")

def run_thai_glyph_shaping_setting_test():
    test_utils.open_project("..\\..\\example_internal\\thai_glyph_shaping\\thai_glyph_shaping.gxp")
    test_utils.compare_result()
    
    # Turn off thai glyph shaping
    test_utils.configure_languages()
    test_utils.check_support_thai_glyph_shapping(0)
    test_utils.save_language_configure()
    test_utils.compare_result()
    
    # Turn on thai glyph shaping
    test_utils.configure_languages()
    test_utils.check_support_thai_glyph_shapping(1)
    test_utils.save_language_configure()
    test_utils.compare_result()
    
    # Change thai font size
    test_utils.click_resource_group('FONT_GROUP')
    test_utils.edit_font("THAI")
    test_utils.set_font_height(25)
    test_utils.save_font_edit()
    test_utils.click_resource_group('FONT_GROUP')
    test_utils.compare_result()
    
    # Configure language without any changes
    test_utils.configure_languages()
    test_utils.save_language_configure()
    test_utils.compare_result()
    
    # Change thai font size
    test_utils.click_resource_group('FONT_GROUP')
    test_utils.edit_font("THAI")
    test_utils.set_font_height(30)
    test_utils.save_font_edit()
    test_utils.click_resource_group('FONT_GROUP')
    test_utils.compare_result()
    
def run_language_edit_tests(generate, screenshot):

    test_utils.output_test_header(get_test_header()) 
    test_utils.setup(generate, screenshot, 'test_language_edit_golden_file')
    
    # create new project for testing
    run_new_project_create("test_language_edit")
    
    # configure languages
    test_utils.configure_languages()  
    test_utils.add_language()
    test_utils.select_language_index(2)
    test_utils.select_language_id('Latin')
    test_utils.save_language_configure()
    test_utils.compare_result()
    
    test_utils.increment_active_language_index()
    test_utils.edit_string()
    test_utils.select_string(0)
    test_utils.toggle_three_column_mode()
    test_utils.increment_trans_language()
    test_utils.edit_bottom_string("test")
    test_utils.save_string_edit()
    test_utils.compare_result()
    
    test_utils.toolbar_save()
    
    # Test thai glyph shaping setting
    run_thai_glyph_shaping_setting_test()
    
    test_utils.write_end("Test Language Edit")
    

