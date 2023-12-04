# -*- coding: utf-8 -*-
import os
import sys
import time
import test_utils
import test_constants

display_name = "main_display"
folder_name = "default_folder"
screen_name = "main_screen"
widget_name = "rich_view"

def get_test_header():
    notes =  "*          Rich Text View Widget Test                 *\n"
    notes += "*                                                     *\n"
    notes += "* Add and Edit Rich Text View                         *\n"
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
    test_utils.set_font_path('..\\..\\fonts\\VeraBd.ttf')
    test_utils.set_font_name('BOLD')
    test_utils.set_font_format('GX_FONT_FORMAT_8BPP') 
    test_utils.save_font_edit()
    
    # add selected font
    test_utils.add_font() 
    test_utils.set_font_path('..\\..\\fonts\\VeraBI.ttf')
    test_utils.set_font_name('BOLD_ITALIC')
    test_utils.set_font_format('GX_FONT_FORMAT_8BPP') 
    test_utils.save_font_edit()
    
    # add selected font
    test_utils.add_font() 
    test_utils.set_font_path('..\\..\\fonts\\VeraIt.ttf')
    test_utils.set_font_name('ITALIC')
    test_utils.set_font_format('GX_FONT_FORMAT_8BPP')
    test_utils.save_font_edit()
    
    # close font folder
    test_utils.click_resource_group('FONT_GROUP')
    
    # open color group folder
    test_utils.click_resource_group('COLOR_GROUP')
    
    # test cancel button
    test_utils.add_color()
    
    # edit color
    test_utils.set_color_name('RED')
    test_utils.set_color_hue(12)
    test_utils.set_color_sat(88)
    test_utils.set_color_lum(49)
    test_utils.save_color_edit()
    
    # open color edit dialog
    test_utils.add_color()
    
    # edit color
    test_utils.set_color_name('GREEN')
    test_utils.set_color_red(15)
    test_utils.set_color_green(235)
    test_utils.set_color_blue(16)
    test_utils.save_color_edit()
    
    # close color group folder
    test_utils.click_resource_group('COLOR_GROUP')
    
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
    

def add_edit_rich_text_view():
    #---------------------------------------#
    #add and edti numeric scroll wheel      #
    #---------------------------------------#
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_RICH_TEXT_VIEW')

    test_utils.edit_widget_props("ID_XPOS", 10)
    test_utils.edit_widget_props("ID_YPOS", 10)
    test_utils.edit_widget_props("ID_WIDTH", 160)
    test_utils.edit_widget_props("ID_HEIGHT", 240)
    test_utils.edit_widget_props("ID_WIDGET_NAME", widget_name)
    test_utils.compare_result()
    
    # edit numeric scroll wheel properties
    test_utils.select_project_tree_node(widget_name)
    test_utils.edit_widget_props("ID_NORMAL_TEXT_COLOR", "TEXT")
    test_utils.edit_widget_props('ID_FONT_SELECT_0', 'TEXT_INPUT')
    test_utils.edit_widget_props_post('ID_RICH_TEXT_VIEW_TEXT_EDIT', 1)
    test_utils.wait_dialog_open(test_utils.RICH_TEXT_EDIT_DIALOG)
    string  = "<b>bold</b>\r"
    string += "<i>italic</i>\r"
    string += "<b><i>bold italic</i></b>\r"
    string += "<u>underline</u>\r"
    string += "<align left>left align</align>\r"
    string += "<align right>right align</align>\r"
    string += "<f GX_FONT_ID_BOLD_ITALIC>font</f>\r"
    string += "<hc GX_COLOR_ID_GREEN>highlight</hc>\r"
    string += "<c GX_COLOR_ID_RED>text color</c>"
    test_utils.edit_rich_text_view_string(string)
    test_utils.save_rich_text_edit()
    test_utils.edit_widget_props('ID_FONT_SELECT_1', 'BOLD')
    test_utils.edit_widget_props('ID_FONT_SELECT_2', 'ITALIC')
    test_utils.edit_widget_props('ID_FONT_SELECT_3', 'BOLD_ITALIC')
    
    test_utils.compare_result()
    
    # test copy paste
    test_utils.select_project_tree_node(widget_name)
    test_utils.toolbar_copy()
    test_utils.select_project_tree_node(screen_name)
    test_utils.toolbar_paste()

    # test undo
    # undo paste
    test_utils.undo()
    #undo bold italic font set
    test_utils.undo()
    #undo italic set
    test_utils.undo()
    #undo bold font set
    test_utils.undo()
    #undo string set
    test_utils.undo()
    
    test_utils.select_project_tree_node(widget_name)
    test_utils.edit_widget_props("ID_STRING_ID", "STRING_2")
    test_utils.edit_widget_props('ID_FONT_SELECT_1', 'BOLD')
    

def edit_rich_text_view_fonts():
    # change font height
    test_utils.select_project_tree_node(widget_name)
    test_utils.click_resource_group('FONT_GROUP')
    test_utils.edit_font('BOLD_ITALIC') 
    test_utils.set_font_path('..\\..\\fonts\\VeraBI.ttf')
    test_utils.set_font_name('BOLD_ITALIC')
    test_utils.set_font_height(26) 
    test_utils.save_font_edit()
    test_utils.compare_result()
    
    # edit font name
    test_utils.edit_font('BOLD_ITALIC')
    test_utils.set_font_name('NEW_BOLD_ITALIC')
    test_utils.save_font_edit()
    test_utils.compare_result()
    
    # delete font
    test_utils.delete_font('BOLD')
    test_utils.compare_result()
    test_utils.click_resource_group('FONT_GROUP')
    
def edit_rich_text_view_colors():
    # change color
    test_utils.click_resource_group('COLOR_GROUP')
    test_utils.edit_color('RED')
    test_utils.set_color_name('RED')
    test_utils.set_color_hue(100)
    test_utils.set_color_sat(88)
    test_utils.set_color_lum(49)
    test_utils.save_color_edit()
    test_utils.compare_result()
    
    # edit color name
    test_utils.edit_color('RED')
    test_utils.set_color_name('NEW_RED')
    test_utils.save_color_edit()
    test_utils.compare_result()
    
    # delete color
    test_utils.delete_color('GREEN')
    test_utils.compare_result()
    test_utils.click_resource_group('COLOR_GROUP')
    
def run_rich_text_view_tests(generate, screenshot):

    test_utils.output_test_header(get_test_header())
    test_utils.setup(generate, screenshot, 'test_rich_text_view_golden_file')

    print('create new project')
    run_new_project_create("test_rich_text_view")
    
    print('add main screen')
    add_main_screen()
    
    print('add and edit rich text view widget')
    add_edit_rich_text_view()
        
    print('edit rich text view colors')
    edit_rich_text_view_colors()
    
    print('edit rich text view fonts')
    edit_rich_text_view_fonts()
    
    test_utils.toolbar_save()
    test_utils.write_end('Rich Text View Widget Tests')