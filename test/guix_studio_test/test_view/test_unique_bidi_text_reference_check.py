import os
import sys
import time
import test_utils
import test_constants
import logging

test_unique_bidi_text_reference_check_output_files = [
"test_unique_bidi_text_reference_check_resources.c",
"test_unique_bidi_text_reference_check_resources.h"]

def get_test_header():
    notes =  "*      Unique Bidi-Text Reference Check Tests         *\n"
    notes += "*                                                     *\n"
    notes += "* Test bidi-text unique reference check for ml-view   *\n"
    notes += "*   template                                          *\n"
    return notes
    
def run_new_project_create(project_name):

    # create a new project
    test_utils.toolbar_create()
    test_utils.set_new_project_path(test_utils.DEFAULT_OUTPUT_FILE_PATH)
    test_utils.set_new_project_name(project_name)
    test_utils.save_new_project_create()
    
    # close message dialog when create new
    test_utils.close_message_dialog()
    
def run_ml_view_template_bidi_text_unique_check():
    run_new_project_create("test_unique_bidi_text_reference_check")
    
    # configure lanuage to support generate reordered bidi text
    test_utils.configure_languages()
    test_utils.select_language_id("Hebrew")
    test_utils.check_support_bidi_text(1)
    test_utils.check_reorder_bidi_text(1)
    test_utils.save_language_configure()
    
    # add a multi line text view template
    test_utils.select_project_tree_node("default_folder")
    test_utils.add_widget('GX_TYPE_MULTI_LINE_TEXT_VIEW')
    test_utils.edit_widget_props("ID_TEMPLATE", 1)
    test_utils.edit_widget_props("ID_WIDGET_NAME", "ml_view_template")
    
    # add a widget based on multi line text view template
    test_utils.select_project_tree_node("default_folder")
    test_utils.add_template("ml_view_template")
    
    test_utils.generate_all()
    test_utils.click_generate_button(1)
    
    test_utils.select_project_tree_node("default_folder")
    test_utils.add_widget('GX_TYPE_MULTI_LINE_TEXT_VIEW')
    
    test_utils.generate_all()
    test_utils.click_generate_button(1)
    test_utils.cmp_output_files(test_unique_bidi_text_reference_check_output_files)
    
def run_unique_bidi_text_reference_check_tests(generate, screenshot):

    test_utils.output_test_header(get_test_header()) 
    test_utils.setup(generate, screenshot, 'test_unique_bidi_text_reference_check_golden_file')
    
    print("run ml-view template bidi text unique reference check")
    run_ml_view_template_bidi_text_unique_check()

    test_utils.write_end("Test Unique Bidi-Text Reference Check")