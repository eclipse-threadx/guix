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

test_scroll_wheel_widget_output_files = [
"test_scroll_wheel_widget_specifications.c",
"test_scroll_wheel_widget_specifications.h"]

def get_test_header():
    notes =  "*          Scroll Wheel Widget Test                   *\n"
    notes += "*                                                     *\n"
    notes += "* Add and Edit String Scroll Wheel                    *\n"
    notes += "* Add and Edit Numeric Scroll Wheel                   *\n"
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
    test_utils.configure_display_name(display_name)
    test_utils.save_project_configure()
        
    # close message dialog when create new project
    test_utils.close_message_dialog()
    
    # add normal font
    # open font folder
    test_utils.click_resource_group('FONT_GROUP')
    
    test_utils.add_font() 
    test_utils.set_font_path('..\\..\\fonts\\Vera.ttf')
    test_utils.set_font_name('NORMAL_FONT')
    test_utils.set_font_format('GX_FONT_FORMAT_8BPP')
    test_utils.set_font_height(28)  
    test_utils.save_font_edit()
    
    # add selected font
    test_utils.add_font() 
    test_utils.set_font_path('..\\..\\fonts\\Vera.ttf')
    test_utils.set_font_name('SELECTED_FONT')
    test_utils.set_font_format('GX_FONT_FORMAT_8BPP')
    test_utils.set_font_height(30)  
    test_utils.save_font_edit()
    
    # cloce font folder
    test_utils.click_resource_group('FONT_GROUP')
    
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
    

def add_edit_numeric_scroll_wheel_widget():
    #---------------------------------------#
    #add and edti numeric scroll wheel      #
    #---------------------------------------#
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_NUMERIC_SCROLL_WHEEL')
    widget_name = "numeric_scroll_wheel"
    
    test_utils.edit_widget_props("ID_XPOS", 10)
    test_utils.edit_widget_props("ID_YPOS", 10)
    test_utils.edit_widget_props("ID_WIDTH", 160)
    test_utils.edit_widget_props("ID_HEIGHT", 240)
    test_utils.edit_widget_props("ID_WIDGET_NAME", widget_name)
    test_utils.compare_result()
    
    # edit numeric scroll wheel properties
    test_utils.select_project_tree_node(widget_name)
    test_utils.edit_widget_props('ID_TEXT_SCROLL_WHEEL_NORMAL_FONT', 'NORMAL_FONT')
    test_utils.edit_widget_props('ID_TEXT_SCROLL_WHEEL_SELECTED_FONT', 'SELECTED_FONT')
    test_utils.edit_widget_props('ID_SCROLL_WHEEL_SELECTED_ROW', 9)
    test_utils.edit_widget_props('ID_SCROLL_WHEEL_ROW_HEIHGT', 40)
    test_utils.edit_widget_props('ID_SCROLL_WHEEL_START_ALPHA', 240)
    test_utils.edit_widget_props('ID_SCROLL_WHEEL_END_ALPHA', 20)
    test_utils.edit_widget_props('ID_TEXT_SCROLL_WHEEL_ROUND', 1)
    test_utils.edit_widget_props('ID_NUMERIC_SCROLL_WHEEL_START_VAL', -100)
    test_utils.edit_widget_props('ID_NUMERIC_SCROLL_WHEEL_END_VAL', 100)
    test_utils.edit_widget_props('ID_PIXELMAP_1', 'CHECKBOX_OFF')
    test_utils.edit_widget_props('ID_SCROLL_WHEEL_TOTAL_ROWS', 100)
    test_utils.compare_result()
    
    # test copy paste
    test_utils.select_project_tree_node(widget_name)
    test_utils.toolbar_copy()
    test_utils.select_project_tree_node(screen_name)
    test_utils.toolbar_paste()
    test_utils.compare_result()
    
    # test undo
    # undo paste
    test_utils.undo()
    #test_utils.undo()
    #undo total rows set
    test_utils.undo()
    #undo selected pixelmap set
    test_utils.undo()
    #undo end value set
    test_utils.undo()
    #undo start value set
    test_utils.undo()
    #undo round style set
    test_utils.undo()
    #undo end alpha set
    test_utils.undo()
    #undo start alpha set
    test_utils.undo()
    #undo row height set
    test_utils.undo()
    #undo selected row set
    test_utils.undo()
    #undo selected font set
    test_utils.undo()
    #undo normal font set
    test_utils.undo()
    
    test_utils.compare_result()
    
    test_utils.select_project_tree_node(widget_name)
    test_utils.edit_widget_props('ID_SCROLL_WHEEL_START_ALPHA', 240)
    test_utils.edit_widget_props('ID_SCROLL_WHEEL_END_ALPHA', 20)
    test_utils.edit_widget_props('ID_LIST_WRAP', 1)
    test_utils.edit_widget_props('ID_TEXT_ALIGN', 'Left')
    
def edit_string_scroll_wheel_widget():
    #---------------------------------------#
    #add and edti string scroll wheel       #
    #---------------------------------------#
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_STRING_SCROLL_WHEEL')
    widget_name = "string_scroll_wheel"
    
    #edit string scroll wheel properties
    test_utils.edit_widget_props("ID_XPOS", 200)
    test_utils.edit_widget_props("ID_YPOS", 10)
    test_utils.edit_widget_props("ID_WIDTH", 160)
    test_utils.edit_widget_props("ID_HEIGHT", 240)
    test_utils.edit_widget_props("ID_WIDGET_NAME", widget_name)
    test_utils.compare_result()
    
    test_utils.select_project_tree_node(widget_name)
    test_utils.edit_widget_props('ID_SCROLL_WHEEL_TOTAL_ROWS', 12)
    test_utils.edit_widget_props_post('ID_STRING_SCROLL_WHEEL_EDIT_STRINGS', 1)
    test_utils.wait_dialog_open(test_utils.STRING_SCROLL_WHEEL_EDIT_DIALOG)
    
    #edti string scroll wheel string
    test_utils.edit_string_scroll_wheel_string(0, 'January')
    test_utils.edit_string_scroll_wheel_string(2, 'March')
    test_utils.edit_string_scroll_wheel_string(3, 'April')
    test_utils.edit_string_scroll_wheel_string(4, 'May')
    test_utils.edit_string_scroll_wheel_string(5, 'June')
    test_utils.edit_string_scroll_wheel_string(6, 'July')
    test_utils.edit_string_scroll_wheel_string(7, 'August')
    test_utils.edit_string_scroll_wheel_string(8, 'September')
    test_utils.edit_string_scroll_wheel_string(9, 'October')
    test_utils.edit_string_scroll_wheel_string(10, 'Novenber')
    test_utils.edit_string_scroll_wheel_string(11, 'December')
    test_utils.save_string_scroll_wheel_edit()
    
    test_utils.compare_result()
    
    test_utils.select_project_tree_node(widget_name)
    test_utils.edit_widget_props('ID_LIST_WRAP', 1)
    test_utils.edit_widget_props('ID_TEXT_SCROLL_WHEEL_NORMAL_FONT', 'NORMAL_FONT')
    test_utils.edit_widget_props('ID_TEXT_SCROLL_WHEEL_SELECTED_FONT', 'SELECTED_FONT')
    test_utils.edit_widget_props('ID_SCROLL_WHEEL_SELECTED_ROW', 3)
    test_utils.edit_widget_props('ID_SCROLL_WHEEL_ROW_HEIHGT', 40)
    test_utils.edit_widget_props('ID_SCROLL_WHEEL_START_ALPHA', 240)
    test_utils.edit_widget_props('ID_SCROLL_WHEEL_END_ALPHA', 20)
    test_utils.edit_widget_props('ID_TEXT_SCROLL_WHEEL_ROUND', 1)
    test_utils.compare_result()
    
    #set callback function
    test_utils.select_project_tree_node(widget_name)
    test_utils.edit_widget_props('ID_CALLBACK_NAME', 'string_get_callback')
    test_utils.undo()
    
    test_utils.select_project_tree_node(widget_name);
    test_utils.toolbar_copy()
    test_utils.select_project_tree_node(screen_name);
    test_utils.toolbar_paste()
    
    #undo paste
    test_utils.undo()
    #test_utils.undo()
    #undo round style set
    test_utils.undo()
    #undo end alpha set
    test_utils.undo()
    #undo start alpha set
    test_utils.undo()
    #undo row height set
    test_utils.undo()
    #undo selected row set
    test_utils.undo()
    #undo selected font set
    test_utils.undo()
    #undo normal font set
    test_utils.undo()
    #undo wrap style set
    test_utils.undo()
    #undo string set
    test_utils.undo()
    #undo total rows set
    test_utils.undo()
    
    test_utils.select_project_tree_node(widget_name)
    test_utils.edit_widget_props('ID_TEXT_ALIGN', 'Right')

def run_string_scroll_wheel_font_text_set():
    test_utils.open_project('all_widgets_execute.gxp')
    test_utils.select_project_tree_node('scroll_wheel_screen')
    
    #delete font
    test_utils.click_resource_group('FONT_GROUP')
    test_utils.delete_font('VERABD')
    test_utils.click_resource_group('FONT_GROUP')
    
    test_utils.compare_result()
    
    #delete string
    test_utils.click_resource_group('STRING_GROUP')
    test_utils.edit_string()
    test_utils.select_string(2)
    test_utils.delete_string() 
    test_utils.compare_result()
    #cancel edit
    test_utils.cancel_string_edit()
    test_utils.click_resource_group('STRING_GROUP')
    test_utils.compare_result()
    
    test_utils.select_project_tree_node('month_wheel')

def string_scroll_wheel_with_callback_func():
    test_utils.open_project('test_scroll_wheel.gxp')
    test_utils.edit_string()
    test_utils.select_string(0)
    test_utils.delete_string()
    test_utils.cancel_string_edit()
    
    #test fit content button
    test_utils.select_project_tree_node('string_scroll_wheel')
    test_utils.toolbar_size_to_fit()
    test_utils.compare_result()
    
    test_utils.select_project_tree_node('string_scroll_wheel')
    test_utils.toolbar_copy()
    test_utils.select_project_tree_node('main_screen')
    test_utils.toolbar_paste()
    
    #remove callback function
    test_utils.select_project_tree_node('string_scroll_wheel')
    test_utils.edit_widget_props('ID_CALLBACK_NAME', '')
    test_utils.edit_widget_props_post('ID_STRING_SCROLL_WHEEL_EDIT_STRINGS', 1)
    test_utils.wait_dialog_open(test_utils.STRING_SCROLL_WHEEL_EDIT_DIALOG)
    
    #edti string scroll wheel string
    test_utils.edit_string_scroll_wheel_string(0, 'January')
    test_utils.save_string_scroll_wheel_edit()
    test_utils.compare_result()

def text_scroll_wheel_round_style():
    test_utils.open_project(test_utils.DEFAULT_OUTPUT_FILE_PATH + 'test_scroll_wheel_widget.gxp')
    test_utils.select_project_tree_node('numeric_scroll_wheel')
    
    #turn round transform style on
    test_utils.edit_widget_props('ID_TEXT_SCROLL_WHEEL_ROUND', 1)

    #configure project to set color format to 332rgb
    test_utils.configure_project()
    test_utils.configure_display_color_depth(8)
    test_utils.configure_display_rgb_bits(332)
    test_utils.save_project_configure()
    
    test_utils.compare_result()
    
    #configure project to set color format to 8bit palette
    test_utils.configure_project()
    test_utils.configure_display_color_depth(8)
    test_utils.configure_display_rgb_bits(332)
    test_utils.save_project_configure()
    
    test_utils.select_project_tree_node('numeric_scroll_wheel')
    test_utils.compare_result()  
    

def run_scroll_wheel_widget_tests(generate, screenshot):

    test_utils.output_test_header(get_test_header())
    test_utils.setup(generate, screenshot, 'test_scroll_wheel_widget_golden_file')

    print('create new project')
    run_new_project_create("test_scroll_wheel_widget")
    
    print('add main screen')
    add_main_screen()
    
    print('add and edit string scroll wheel widget')
    add_edit_numeric_scroll_wheel_widget()
    
    print('add and edit numeric scroll wheel widget')
    edit_string_scroll_wheel_widget()

    test_utils.toolbar_save()
    test_utils.generate_all()
    test_utils.click_generate_button()
    test_utils.cmp_output_files(test_scroll_wheel_widget_output_files)
    
    #close project with widget been selected
    test_utils.select_project_tree_node("numeric_scroll_wheel")
    test_utils.close_project()
    test_utils.open_project(test_utils.DEFAULT_OUTPUT_FILE_PATH + "\\test_scroll_wheel_widget.gxp")
    test_utils.select_project_tree_node("string_scroll_wheel")
    test_utils.close_project()
    
    print('run string scroll wheel font text set')
    run_string_scroll_wheel_font_text_set()
    
    print('test string scroll wheel with callback func')
    string_scroll_wheel_with_callback_func()

    print('test text scroll wheel round style')
    text_scroll_wheel_round_style()
    
    test_utils.write_end('Scroll Wheel Widget Tests')