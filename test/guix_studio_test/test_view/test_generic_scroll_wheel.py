# -*- coding: utf-8 -*-
import os
import sys
import time
import test_utils
import test_constants
import filecmp

display_name = "main_display"
folder_name = "default_folder"
screen_name = "main_screen"

test_generic_scroll_wheel_output_files = [
"test_generic_scroll_wheel_specifications.c",
"test_generic_scroll_wheel_specifications.h"]

def get_test_header():
    notes =  "*          Generic Scroll Wheel Test                  *\n"
    notes += "*                                                     *\n"
    notes += "* Add and Edit Generic Scroll Wheel                   *\n"
    return notes
    
def run_new_project_create(project_name):

    # create a new project
    test_utils.toolbar_create()
    test_utils.set_new_project_path(test_utils.DEFAULT_OUTPUT_FILE_PATH)
    test_utils.set_new_project_name(project_name)
    test_utils.save_new_project_create()
    
    # configure project
    test_utils.configure_project()
    test_utils.configure_x_res(640)
    test_utils.configure_y_res(480)
    test_utils.configure_minor_version(1)
    test_utils.configure_service_pack(7)
    test_utils.configure_display_name(display_name)
    test_utils.save_project_configure()
        
    # close message dialog when create new project
    test_utils.close_message_dialog()
    
    test_utils.toolbar_save()
    
def add_main_screen():
    #---------------------------------------#
    #add main screen to display             #
    #---------------------------------------#
    
    test_utils.select_project_tree_node("window")

    test_utils.edit_widget_props("ID_XPOS", 0)
    test_utils.edit_widget_props("ID_YPOS", 0)
    test_utils.edit_widget_props("ID_WIDTH", 640)
    test_utils.edit_widget_props("ID_HEIGHT", 480)
    test_utils.edit_widget_props("ID_WIDGET_NAME", screen_name)
    
    test_utils.add_pixelmaps('Custom', '..\\pixelmaps\\', 'fill.png')
    test_utils.add_pixelmaps('Custom', '..\\pixelmaps\\', 'yellow_bg.png')    

def add_edit_generic_scroll_wheel():
    #---------------------------------------#
    #add and edti widget scroll wheel       #
    #---------------------------------------#
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_GENERIC_SCROLL_WHEEL')
    widget_name = "generic_scroll_wheel"
    
    test_utils.edit_widget_props("ID_XPOS", 16)
    test_utils.edit_widget_props("ID_YPOS", 19)
    test_utils.edit_widget_props("ID_WIDTH", 179)
    test_utils.edit_widget_props("ID_HEIGHT", 213)
    test_utils.edit_widget_props("ID_WIDGET_NAME", widget_name)
    test_utils.edit_widget_props('ID_SCROLL_WHEEL_SELECTED_ROW', 0)
    test_utils.edit_widget_props('ID_SCROLL_WHEEL_ROW_HEIHGT', 40)
    test_utils.edit_widget_props('ID_SCROLL_WHEEL_START_ALPHA', 240)
    test_utils.edit_widget_props('ID_SCROLL_WHEEL_END_ALPHA', 20)
    test_utils.edit_widget_props('ID_SCROLL_WHEEL_TOTAL_ROWS', 8)
    
    # set wallpaper
    test_utils.edit_widget_props("ID_PIXELMAP_0", "YELLOW_BG")
    
    # set selected background
    test_utils.edit_widget_props("ID_PIXELMAP_1", "FILL")
    test_utils.compare_result()
    
    # Add child widget to widget scroll wheel
    for index in range(0, 8):
        test_utils.select_project_tree_node(widget_name)
        test_utils.add_widget('GX_TYPE_NUMERIC_PROMPT')
        test_utils.edit_widget_props('ID_TRANSPARENT', 1)
        test_utils.edit_widget_props('ID_NUMERIC_PROMPT_VALUE', index)
        test_utils.compare_result()
    
    test_utils.select_project_tree_node(widget_name)
    test_utils.edit_widget_props('ID_SCROLL_WHEEL_ROW_HEIHGT', 45)
    test_utils.compare_result()
    
    test_utils.select_project_tree_node(widget_name)
    test_utils.edit_widget_props('ID_SCROLL_WHEEL_SELECTED_ROW', 4)
    test_utils.compare_result()
    
    test_utils.select_project_tree_node(widget_name)
    test_utils.edit_widget_props('ID_SCROLL_WHEEL_SELECTED_ROW', 8)
    
    test_utils.select_project_tree_node(widget_name)
    test_utils.edit_widget_props('ID_SCROLL_WHEEL_SELECTED_ROW', 7)
    test_utils.compare_result()

    test_utils.select_project_tree_node(widget_name)
    test_utils.toolbar_copy()
    test_utils.select_project_tree_node(screen_name)
    test_utils.toolbar_paste()
    test_utils.select_project_tree_node(widget_name + "_1")
    test_utils.edit_widget_props("ID_XPOS", 16)
    test_utils.edit_widget_props("ID_YPOS", 243)
    test_utils.compare_result()

    #-----------------------------------------------------------#
    # add generic scroll wheel with callback, without wrap style#
    #-----------------------------------------------------------#
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_GENERIC_SCROLL_WHEEL')
    widget_name = "generic_scroll_wheel_nowrap"
    
    test_utils.edit_widget_props("ID_XPOS", 230)
    test_utils.edit_widget_props("ID_YPOS", 101)
    test_utils.edit_widget_props("ID_WIDTH", 188)
    test_utils.edit_widget_props("ID_HEIGHT", 239)
    test_utils.edit_widget_props("ID_WIDGET_NAME", widget_name)
    test_utils.edit_widget_props('ID_SCROLL_WHEEL_TOTAL_ROWS', 8)
    test_utils.edit_widget_props('ID_CALLBACK_NAME', 'generic_scroll_callback')
    test_utils.edit_widget_props("ID_PIXELMAP_0", "YELLOW_BG")
    test_utils.edit_widget_props("ID_PIXELMAP_1", "FILL")
    test_utils.compare_result()
    
    #----------------------------------------------------------#
    # add generic scroll wheel with callback, with wrap style  #
    #----------------------------------------------------------#
    test_utils.select_project_tree_node(widget_name)
    test_utils.toolbar_copy()
    test_utils.select_project_tree_node(screen_name)
    test_utils.toolbar_paste()
    test_utils.select_project_tree_node(widget_name + "_1")
    test_utils.edit_widget_props("ID_XPOS", 433)
    test_utils.edit_widget_props("ID_YPOS", 101)
    test_utils.edit_widget_props("ID_LIST_WRAP", 1)
    test_utils.compare_result()
    

def run_generic_scroll_wheel_tests(generate, screenshot):

    test_utils.output_test_header(get_test_header())
    test_utils.setup(generate, screenshot, 'test_generic_scroll_wheel_golden_file')

    print('create new project')
    run_new_project_create("test_generic_scroll_wheel")
    
    print('add main screen')
    add_main_screen()
    
    print('add and edit generic scroll wheel')
    add_edit_generic_scroll_wheel()
 
    test_utils.toolbar_save()
    test_utils.generate_all()
    test_utils.click_generate_button()
    test_utils.cmp_output_files(test_generic_scroll_wheel_output_files)
    
    test_utils.write_end('Generic Scroll Wheel Tests')