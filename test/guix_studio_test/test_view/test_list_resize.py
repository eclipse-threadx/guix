# -*- coding: utf-8 -*-
import os
import sys
import time
import test_utils
import test_constants

folder_hane = "default_folder"
screen_name = "window"

def get_test_header(): 
    notes =  "*           Studio List Resize Test                   *\n"
    notes += "*                                                     *\n"
    notes += "* Test vertical list resizing                         *\n"
    notes += "* Test horizontal list resizing                       *\n"
    return notes

def run_test_project_create(project_name):

    # create a new project
    test_utils.toolbar_create()
    test_utils.set_new_project_path(test_utils.DEFAULT_OUTPUT_FILE_PATH)
    test_utils.set_new_project_name(project_name)
    test_utils.save_new_project_create()
        
    # close message dialog when create new project
    test_utils.close_message_dialog()
    
    test_utils.select_project_tree_node(screen_name)
    
    #-----------------------------------------------#
    # add vertical list                             #
    #-----------------------------------------------#
    test_utils.add_widget('GX_TYPE_VERTICAL_LIST')
    test_utils.edit_widget_props("ID_WIDGET_NAME", "vertical_list")
    test_utils.edit_widget_props("ID_XPOS", 5)
    test_utils.edit_widget_props("ID_YPOS",9)
    test_utils.edit_widget_props("ID_WIDTH", 139)
    test_utils.edit_widget_props("ID_HEIGHT", 130)
    
    # add vertical list item 1
    test_utils.select_project_tree_node("vertical_list")
    test_utils.add_widget("GX_TYPE_WINDOW")
    test_utils.edit_widget_props("ID_WIDGET_NAME", "vlist_item_1")
    test_utils.edit_widget_props("ID_HEIGHT", 50)
    
    test_utils.select_project_tree_node("vlist_item_1")
    test_utils.add_widget("GX_TYPE_PROMPT")
    
    # add vertical list item 2
    test_utils.select_project_tree_node("vertical_list")
    test_utils.add_widget("GX_TYPE_WINDOW")
    test_utils.edit_widget_props("ID_WIDGET_NAME", "vlist_item_2")
    test_utils.edit_widget_props("ID_HEIGHT", 50)
    
    test_utils.select_project_tree_node("vlist_item_2")
    test_utils.add_widget("GX_TYPE_PROMPT")
    
    #-----------------------------------------------#
    # add horizontal list                           #
    #-----------------------------------------------#
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_HORIZONTAL_LIST')
    test_utils.edit_widget_props("ID_WIDGET_NAME", "horizontal_list")
    test_utils.edit_widget_props("ID_XPOS", 6)
    test_utils.edit_widget_props("ID_YPOS",163)
    test_utils.edit_widget_props("ID_WIDTH", 220)
    test_utils.edit_widget_props("ID_HEIGHT", 49)
    
    # add horizontal list item 1
    test_utils.select_project_tree_node("horizontal_list")
    test_utils.add_widget("GX_TYPE_WINDOW")
    test_utils.edit_widget_props("ID_WIDGET_NAME", "hlist_item_1")
    test_utils.edit_widget_props("ID_WIDTH", 96)
    
    test_utils.select_project_tree_node("hlist_item_1")
    test_utils.add_widget("GX_TYPE_PROMPT")
    
    # add horizontal list item 2
    test_utils.select_project_tree_node("horizontal_list")
    test_utils.add_widget("GX_TYPE_WINDOW")
    test_utils.edit_widget_props("ID_WIDGET_NAME", "hlist_item_2")
    test_utils.edit_widget_props("ID_WIDTH", 96)
    
    test_utils.select_project_tree_node("hlist_item_2")
    test_utils.add_widget("GX_TYPE_PROMPT")
    
    test_utils.toolbar_save()
    
def run_list_resize_tests(generate, screenshot):

    test_utils.output_test_header(get_test_header())
    test_utils.setup(generate, screenshot, 'test_list_reisze_golden_file')
    
    project_name = "test_list_resize";
    
    print("create test project")
    run_test_project_create(project_name)
    
    print("resize the height of the first vertical list item")
    test_utils.select_project_tree_node('vlist_item_1')
    test_utils.edit_widget_props("ID_HEIGHT", 80)
    
    print("resize the width of the first horizontal list item")
    test_utils.select_project_tree_node('hlist_item_1')
    test_utils.edit_widget_props("ID_WIDTH", 120)
    test_utils.compare_result()
    test_utils.toolbar_save()
    
    print("save test project")
    test_utils.close_project(1)
    
    print("open test project")
    test_utils.open_project(test_utils.DEFAULT_OUTPUT_FILE_PATH+project_name+".gxp")
    test_utils.compare_result()
    
    test_utils.write_end('List Resize Tests')
