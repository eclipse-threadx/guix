# -*- coding: utf-8 -*-
import os
import sys
import time
import test_utils
import test_constants
import filecmp

first_display_name = "main_display"
second_display_name = "secondary_display"

widget_name_output_files = [
"test_widget_name_main_display_resources.c",
"test_widget_name_main_display_resources.h",
"test_widget_name_secondary_display_resources.c",
"test_widget_name_secondary_display_resources.h",
"test_widget_name_specifications.c",
"test_widget_name_specifications.h"]

def get_test_header():
    notes =  "*             Widget Name Test                        *\n"
    notes += "*                                                     *\n"
    notes += "* Edit duplicate top level and child widget name      *\n"
    notes += "*   (allowed)                                         *\n"
    notes += "* Edit duplicate child widget name under different    *\n"
    notes += "*   top level widget(allowed)                         *\n"
    notes += "* Edit duplicate top level widget name(not allowed)   *\n"
    notes += "* Edit duplicate child widget name under same top     *\n"
    notes += "*   level widget(not allowed)                         *\n"
    notes += "* Undo delete widget                                  *\n"
    notes += "* Import existing screens                             *\n"
    notes += "* Test if we can find base widget correctly when      *\n"
    notes += "*   base name is duplicated with child widget name    *\n"
    notes += "*   under other top level widgets                     *\n"
    return notes

def run_new_project_create(project_name):

    # create a new project
    test_utils.toolbar_create()
    test_utils.set_new_project_path(test_utils.DEFAULT_OUTPUT_FILE_PATH)
    test_utils.set_new_project_name(project_name)
    test_utils.save_new_project_create()
    test_utils.close_message_dialog()
    
    # configure project
    test_utils.configure_project()
    test_utils.configure_display_num(2)
    test_utils.wait_for_key()

    test_utils.select_display_index(1);
    test_utils.configure_x_res(640)
    test_utils.configure_y_res(480)
    test_utils.configure_display_name(first_display_name)
    test_utils.wait_for_key()

    test_utils.select_display_index(2);
    test_utils.configure_x_res(320)
    test_utils.configure_y_res(240)
    test_utils.configure_display_name(second_display_name)
    test_utils.wait_for_key()

    test_utils.save_project_configure()

def run_widget_name_tests(generate, screenshot):

    test_utils.output_test_header(get_test_header())
    test_utils.setup(generate, screenshot, 'test_widget_name_golden_file')
    
    print('create test widget name project')
    run_new_project_create("test_widget_name")

    test_utils.select_project_tree_node('window')
    test_utils.add_widget('GX_TYPE_WINDOW')
    test_utils.edit_widget_props("ID_WIDGET_NAME", 'window')#make the name the same as its parent
    
    test_utils.select_project_tree_node('window')
    test_utils.add_widget('GX_TYPE_WINDOW')
    test_utils.edit_widget_props_post('ID_WIDGET_NAME', 'window')#duplicate child widget name under the same top level widget, not allowed
    test_utils.close_error_dialog()
    
    test_utils.select_project_tree_node('window_1')
    test_utils.add_widget('GX_TYPE_WINDOW')
    test_utils.edit_widget_props_post('ID_WIDGET_NAME', 'window_1')#duplicate child widget name under the same top level widget, not allowed
    test_utils.close_error_dialog()

    test_utils.select_project_tree_node('window_1')
    test_utils.delete_widget()
    test_utils.undo()
    
    test_utils.select_project_tree_node('default_folder')
    test_utils.add_widget('GX_TYPE_WINDOW')
    test_utils.edit_widget_props_post("ID_WIDGET_NAME", 'window')#duplicate top level widget name, not allowed
    test_utils.close_error_dialog()#close message dialog, widge already existing
    
    test_utils.select_project_tree_node('window_3')
    test_utils.add_widget('GX_TYPE_WINDOW')
    test_utils.edit_widget_props('ID_WIDGET_NAME', 'window_2')#duplicate child widget name under different top level widget, allowed
    
    test_utils.toolbar_save()
    test_utils.select_project_tree_node('default_folder')
    
    #import existing screens
    import_project_name = os.path.abspath(test_utils.DEFAULT_OUTPUT_FILE_PATH) + '\\test_widget_name.gxp'
    test_utils.import_project(import_project_name)
    test_utils.save_project_import_dialog()
    
    test_utils.select_project_tree_node(first_display_name)
    test_utils.insert_folder()#default_folder_1
    test_utils.add_widget('GX_TYPE_WINDOW')
    test_utils.add_widget('GX_TYPE_PROMPT')
    test_utils.edit_widget_props('ID_WIDGET_NAME', 'base')
    
    test_utils.select_project_tree_node('default_folder_1')
    test_utils.add_widget('GX_TYPE_WINDOW')
    test_utils.edit_widget_props('ID_WIDGET_NAME', 'base')
    test_utils.edit_widget_props('ID_TEMPLATE', 1)

    test_utils.select_project_tree_node('default_folder_1')
    test_utils.add_widget('GX_TYPE_WINDOW')
    test_utils.add_widget('GX_TYPE_PROMPT')
    test_utils.edit_widget_props('ID_WIDGET_NAME', 'base')
    
    test_utils.select_project_tree_node('default_folder_1')
    test_utils.add_template('base')
    test_utils.edit_widget_props_post('ID_WIDGET_NAME', 'window')#duplicate top level widget name
    test_utils.close_error_dialog()
    test_utils.compare_result()
    
    # test rename widget on second display
    test_utils.select_project_tree_node(second_display_name)
    test_utils.wait_for_key('selected secondary display')
    test_utils.insert_folder() #default_folder
    test_utils.wait_for_key('inserted default folder')
    test_utils.add_widget('GX_TYPE_WINDOW')
    test_utils.add_widget('GX_TYPE_PROMPT')
    test_utils.wait_for_key('Added window and prompt')
    test_utils.edit_widget_props('ID_WIDGET_NAME', "secondary_prompt")
    test_utils.wait_for_key('renamed the prompt')

    test_utils.generate_all()
    test_utils.click_generate_button()
    test_utils.cmp_output_files(widget_name_output_files)

    test_utils.write_end('Widget Name Tests')