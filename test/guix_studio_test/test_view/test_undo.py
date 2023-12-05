# -*- coding: utf-8 -*-
import os
import sys
import time
import test_utils
import test_constants


display_1_name = 'display_1'
display_2_name = 'display_2'
folder_name = 'default_folder'


def get_test_header():
    notes =  "*                 Undo Test                           *\n"
    notes += "*                                                     *\n"
    notes += "* Undo widget delete operation from project           *\n"
    return notes

def run_new_project_create(project_name):

    # create a new project
    test_utils.toolbar_create()
    test_utils.set_new_project_path(test_utils.DEFAULT_OUTPUT_FILE_PATH)
    test_utils.set_new_project_name(project_name)
    test_utils.save_new_project_create()
    
    # configure project
    test_utils.configure_project()
    test_utils.configure_display_num(2)
    test_utils.select_display_index(1)
    test_utils.configure_x_res(640)
    test_utils.configure_y_res(480)
    test_utils.configure_display_name(display_1_name)
    test_utils.select_display_index(2)
    test_utils.configure_x_res(640)
    test_utils.configure_y_res(480)
    test_utils.configure_display_name(display_2_name)
    test_utils.save_project_configure()
    
    # close message dialog when create new and delete default_folder
    test_utils.close_message_dialog()

        
   
def undo_delete_widget():
    #insert window to delete
    window_name = 'target_window'
    test_utils.select_project_tree_node(display_2_name)
    test_utils.insert_folder()
    test_utils.add_widget('GX_TYPE_WINDOW')
    test_utils.edit_widget_props("ID_WIDGET_NAME", window_name)
    test_utils.edit_widget_props("ID_XPOS", 160)
    test_utils.edit_widget_props("ID_YPOS", 120)
    test_utils.edit_widget_props("ID_WIDTH", 320)
    test_utils.edit_widget_props("ID_HEIGHT", 240)
    test_utils.delete_widget()
    test_utils.undo()
    test_utils.compare_result()
   
def run_undo_tests(generate, screenshot):
    test_utils.output_test_header(get_test_header())
    test_utils.setup(generate, screenshot, 'test_undo_golden_file')
    
    print('create new project')
    run_new_project_create("test_undo")
    
    
    #test bug that while undo widget delete, incorrect parent is pasted to.
    undo_delete_widget();
    
    
    test_utils.write_end('Undo Tests')