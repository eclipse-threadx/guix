# -*- coding: utf-8 -*-
import os
import sys
import time
import test_utils
import test_constants

project_name = "test_font"
display_name = "main_display"
folder_name = 'default_folder'
test_font_output_files = [
"test_font_custom_output_file.c",
"test_font_resources.c",
"test_font_resources.h",
"test_font_specifications.c",
"test_font_specifications.h"]
    
def run_font_add():
    # open font folder
    test_utils.click_resource_group('FONT_GROUP')
    
    # test cancel button
    test_utils.add_font()
    test_utils.cancel_font_edit()
    
    # add font 1bit raw
    test_utils.add_font()   
    test_utils.set_font_path('..\\..\\fonts\\Vera.ttf')
    test_utils.set_font_name('FONT_1BIT_RAW')
    test_utils.set_font_format('GX_FONT_FORMAT_1BPP')
    test_utils.set_font_height(200)
    test_utils.check_font_custom_output(1)
    test_utils.set_font_custom_output_file('test_font_custom_output_file.c')
    test_utils.save_font_edit()
    
    # add font 1bit compressed
    test_utils.add_font() 
    test_utils.set_font_path('..\\..\\fonts\\Vera.ttf')
    test_utils.set_font_name('FONT_1BIT_COMPRESSED')
    test_utils.set_font_compression(1)
    test_utils.set_font_format('GX_FONT_FORMAT_1BPP')
    test_utils.set_font_height_post(256, True) 
    test_utils.set_font_height(200)
    test_utils.check_font_custom_output(1)
    test_utils.set_font_custom_output_file('test_font_custom_output_file.c')
    test_utils.save_font_edit()
    
    test_utils.edit_font('FONT_1BIT_COMPRESSED')
    test_utils.check_font_custom_output(0)
    test_utils.save_font_edit()
    
    test_utils.edit_font('FONT_1BIT_COMPRESSED')
    test_utils.check_font_custom_output(1)
    test_utils.set_font_custom_output_file('test_font_custom_output_file.c')
    test_utils.cancel_font_edit()
    
    # save and close project to test custom output file information write to project file
    test_utils.toolbar_save()
    test_utils.close_project(0)
    
    # reopen project to test custom output file information read from project file
    test_utils.open_project(test_utils.DEFAULT_OUTPUT_FILE_PATH + '\\' + project_name + '.gxp', 0) 
    
    # add font 4bit raw
    test_utils.add_font()
    test_utils.set_font_path('..\\..\\fonts\\VeraBd.ttf')
    test_utils.set_font_name('FONT_4BIT_RAW')
    test_utils.set_font_format('GX_FONT_FORMAT_4BPP')
    test_utils.set_font_height(100)
    test_utils.save_font_edit()
    
    # add font 4bit compressed
    test_utils.add_font()  
    test_utils.set_font_path('..\\..\\fonts\\VeraBd.ttf')
    test_utils.set_font_name('FONT_4BIT_COMPRESSED')
    test_utils.set_font_compression(1)
    test_utils.set_font_format('GX_FONT_FORMAT_4BPP')
    test_utils.set_font_height(100)
    test_utils.save_font_edit()
    
    # add font 8bit raw
    test_utils.add_font()  
    test_utils.set_font_path('..\\..\\fonts\\VeraBI.ttf')
    test_utils.set_font_name('FONT_8BIT_RAW')
    test_utils.set_font_format('GX_FONT_FORMAT_8BPP')
    test_utils.save_font_edit()
    
    # add font 8bit compressed
    test_utils.add_font()  
    test_utils.set_font_path('..\\..\\fonts\\VeraBI.ttf')
    test_utils.set_font_name('FONT_8BIT_COMPRESSED')
    test_utils.set_font_compression(1)
    test_utils.set_font_format('GX_FONT_FORMAT_8BPP')
    test_utils.save_font_edit()
    
    # add font that contain extended characters
    test_utils.add_font()
    test_utils.set_font_path('..\\..\\fonts\\Italica Vetus 1-201.otf')
    test_utils.set_font_name('FONT_OLD_ITALIC')
    test_utils.check_extended_unicode(1)
    test_utils.set_font_height(20)
    test_utils.set_page_range(46, 1, '10300', '10323')
    test_utils.set_page_range(47, 1, '10fffd', '10fffd')
    test_utils.save_font_edit()
    
    # cloce font folder
    test_utils.click_resource_group('FONT_GROUP')

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
    test_utils.configure_display_color_format('GX_COLOR_FORMAT_24XRGB')
    test_utils.configure_cpu_type('synergy')
    test_utils.save_project_configure()
    
    # close message dialog when create new
    test_utils.close_message_dialog()
    
def run_widgets_add():
    test_utils.select_project_tree_node("window")  
    screen_name = "main_window"
    
    test_utils.edit_widget_props("ID_WIDGET_NAME", screen_name)
    test_utils.edit_widget_props("ID_XPOS", 0)
    test_utils.edit_widget_props("ID_YPOS", 0)
    test_utils.edit_widget_props("ID_WIDTH", 640)
    test_utils.edit_widget_props("ID_HEIGHT", 480)
    
    #Add a prompt and set it's font to FONT_1BIT_RAW
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_PROMPT')
    test_utils.edit_widget_props("ID_XPOS", 0)
    test_utils.edit_widget_props("ID_YPOS", 0)
    test_utils.edit_widget_props("ID_WIDTH", 640)
    test_utils.edit_widget_props("ID_HEIGHT", 165)
    test_utils.edit_widget_props("ID_TEXT_ALIGN", 'Left')
    test_utils.edit_widget_props("ID_STRING_VAL", "1 bit raw")
    test_utils.edit_widget_props("ID_FONT_SELECT_0", "FONT_1BIT_RAW")
    
    #Add a prompt and set it's font to FONT_1BIT_COMPRESSED
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_PROMPT')
    test_utils.edit_widget_props("ID_XPOS", 0)
    test_utils.edit_widget_props("ID_YPOS", 145)
    test_utils.edit_widget_props("ID_WIDTH", 640)
    test_utils.edit_widget_props("ID_HEIGHT", 136)
    test_utils.edit_widget_props("ID_TEXT_ALIGN", 'Left')
    test_utils.edit_widget_props("ID_STRING_VAL", "200height")
    test_utils.edit_widget_props("ID_FONT_SELECT_0", "FONT_1BIT_COMPRESSED")
    
    #Add a prompt and set it's font to FONT_4BIT_RAW
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_PROMPT')
    test_utils.edit_widget_props("ID_XPOS", 0)
    test_utils.edit_widget_props("ID_YPOS", 269)
    test_utils.edit_widget_props("ID_WIDTH", 640)
    test_utils.edit_widget_props("ID_HEIGHT", 81)
    test_utils.edit_widget_props("ID_TEXT_ALIGN", 'Left')
    test_utils.edit_widget_props("ID_STRING_VAL", "4 bit raw")
    test_utils.edit_widget_props("ID_FONT_SELECT_0", "FONT_4BIT_RAW")
    
    #Add a prompt and set it's font to FONT_4BIT_COMPRESSED
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_PROMPT')
    test_utils.edit_widget_props("ID_XPOS", 0)
    test_utils.edit_widget_props("ID_YPOS", 331)
    test_utils.edit_widget_props("ID_WIDTH", 640)
    test_utils.edit_widget_props("ID_HEIGHT", 90)
    test_utils.edit_widget_props("ID_TEXT_ALIGN", 'Left')
    test_utils.edit_widget_props("ID_STRING_VAL", "4 bit compressed")
    test_utils.edit_widget_props("ID_FONT_SELECT_0", "FONT_4BIT_COMPRESSED")
    
    #Add a prompt and set it's font to FONT_8BIT_RAW
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_PROMPT')
    test_utils.edit_widget_props("ID_XPOS", 3)
    test_utils.edit_widget_props("ID_YPOS", 421)
    test_utils.edit_widget_props("ID_WIDTH", 189)
    test_utils.edit_widget_props("ID_HEIGHT", 56)
    test_utils.edit_widget_props("ID_STRING_VAL", "font 8 bit raw")
    test_utils.edit_widget_props("ID_FONT_SELECT_0", "FONT_8BIT_RAW")
    
    #Add a prompt and set it's font to FONT_8BIT_COMPRESSED
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_PROMPT')
    test_utils.edit_widget_props("ID_XPOS", 197)
    test_utils.edit_widget_props("ID_YPOS", 421)
    test_utils.edit_widget_props("ID_WIDTH", 267)
    test_utils.edit_widget_props("ID_HEIGHT", 56)
    test_utils.edit_widget_props("ID_STRING_VAL", "font 8 bit compressed")
    test_utils.edit_widget_props("ID_FONT_SELECT_0", "FONT_8BIT_COMPRESSED")
 
    #Add a prompt to test extended unicode support feature
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_PROMPT')
    test_utils.edit_widget_props("ID_XPOS", 467)
    test_utils.edit_widget_props("ID_YPOS", 421)
    test_utils.edit_widget_props("ID_WIDTH", 170)
    test_utils.edit_widget_props("ID_HEIGHT", 56)
    test_utils.edit_widget_props("ID_STRING_VAL", "𐌀𐌁𐌂𐌃𐌄𐌅𐌞𐌟𐌠𐌡𐌢𐌣􏿽")
    test_utils.edit_widget_props("ID_FONT_SELECT_0", "FONT_OLD_ITALIC")
    
def add_text_color():
    # open color group folder
    test_utils.click_resource_group('COLOR_GROUP')
    
    # test cancel button
    test_utils.add_color()
    test_utils.set_color_name('TEXT_COLOR')
    test_utils.set_color_red(0)
    test_utils.set_color_green(0)
    test_utils.set_color_blue(0)
    test_utils.save_color_edit()
    
    test_utils.edit_color('TEXT')
    test_utils.set_color_red(0)
    test_utils.set_color_green(0)
    test_utils.set_color_blue(0)
    test_utils.save_color_edit()
    
    # close color group folder
    test_utils.click_resource_group('COLOR_GROUP')
    
def run_kerning_font_test():
    # open font folder
    test_utils.click_resource_group('FONT_GROUP')

    #Add new font which doesn't contain a kerning table but the checkbox 'include kerning info' is selected on.
    test_utils.add_font()
    test_utils.set_font_path('..\\..\\fonts\\Italica Vetus 1-201.otf')
    test_utils.set_font_name('FONT_WITHOUT_KERNING_INFO')
    test_utils.set_font_compression(0)
    test_utils.set_font_format('GX_FONT_FORMAT_8BPP')
    test_utils.set_font_generate_kerning(1)
    test_utils.save_font_edit()
    test_utils.close_message_dialog()
    test_utils.delete_font('FONT_WITHOUT_KERNING_INFO')
    
    #edit font and set kerning true to make font generate kerning table
    test_utils.add_font()
    test_utils.set_font_path('..\\..\\fonts\\Vera.ttf')
    test_utils.set_font_name('FONT_1BIT')
    test_utils.set_font_format('GX_FONT_FORMAT_1BPP')
    test_utils.set_font_generate_kerning(0)
    test_utils.save_font_edit()
    
    test_utils.add_font()
    test_utils.set_font_path('..\\..\\fonts\\Vera.ttf')
    test_utils.set_font_name('FONT_1BIT_KERNING')
    test_utils.set_font_format('GX_FONT_FORMAT_1BPP')
    test_utils.set_font_generate_kerning(1)
    test_utils.save_font_edit()

    test_utils.add_font()
    test_utils.set_font_path('..\\..\\fonts\\Vera.ttf')
    test_utils.set_font_name('FONT_4BIT')
    test_utils.set_font_format('GX_FONT_FORMAT_4BPP')
    test_utils.set_font_height(50)
    test_utils.set_font_generate_kerning(0)
    test_utils.save_font_edit()
    
    test_utils.add_font()
    test_utils.set_font_path('..\\..\\fonts\\Vera.ttf')
    test_utils.set_font_name('FONT_4BIT_KERNING')
    test_utils.set_font_format('GX_FONT_FORMAT_4BPP')
    test_utils.set_font_height(50)
    test_utils.set_font_generate_kerning(1)
    test_utils.save_font_edit()
    
    test_utils.add_font()
    test_utils.set_font_path('..\\..\\fonts\\Vera.ttf')
    test_utils.set_font_name('FONT_8BIT')
    test_utils.set_font_format('GX_FONT_FORMAT_8BPP')
    test_utils.set_font_height(150)
    test_utils.set_font_generate_kerning(0)
    test_utils.save_font_edit()
    
    test_utils.add_font()
    test_utils.set_font_path('..\\..\\fonts\\Vera.ttf')
    test_utils.set_font_name('FONT_8BIT_KERNING')
    test_utils.set_font_format('GX_FONT_FORMAT_8BPP')
    test_utils.set_font_height(150)
    test_utils.set_font_generate_kerning(1)
    test_utils.save_font_edit()
    
    #Add widget to test the kerning glyph draw
    test_utils.select_project_tree_node(folder_name)
    test_utils.add_widget('GX_TYPE_WINDOW')
    screen_name = "Font_kerning_test_window"
    
    test_utils.edit_widget_props("ID_WIDGET_NAME", screen_name)
    test_utils.edit_widget_props("ID_XPOS", 0)
    test_utils.edit_widget_props("ID_YPOS", 0)
    test_utils.edit_widget_props("ID_WIDTH", 640)
    test_utils.edit_widget_props("ID_HEIGHT", 480)
    test_utils.edit_widget_props("ID_BORDER_COMBO", test_constants.BORDER_TYPE_NONE)
    
    #Add a prompt and set it's font to FONT_8BIT
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_PROMPT')
    test_utils.edit_widget_props("ID_XPOS", 0)
    test_utils.edit_widget_props("ID_YPOS", 0)
    test_utils.edit_widget_props("ID_WIDTH", 640)
    test_utils.edit_widget_props("ID_HEIGHT", 160)
    test_utils.edit_widget_props("ID_TEXT_ALIGN", 'Left')
    test_utils.edit_widget_props("ID_BORDER_COMBO", test_constants.BORDER_TYPE_NONE)
    test_utils.edit_widget_props("ID_STRING_VAL", "FAWAVAYLTLV_8BIT")
    test_utils.edit_widget_props("ID_FONT_SELECT_0", "FONT_8BIT")
    
    #Add a prompt and set it's font to FONT_8BIT_KERNING
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_PROMPT')
    test_utils.edit_widget_props("ID_XPOS", 0)
    test_utils.edit_widget_props("ID_YPOS", 160)
    test_utils.edit_widget_props("ID_WIDTH", 640)
    test_utils.edit_widget_props("ID_HEIGHT", 160)
    test_utils.edit_widget_props("ID_TEXT_ALIGN", 'Left')
    test_utils.edit_widget_props("ID_BORDER_COMBO", test_constants.BORDER_TYPE_NONE)
    test_utils.edit_widget_props("ID_STRING_VAL", "FAWAVAYLTLV_8BIT_KERNING")
    test_utils.edit_widget_props("ID_FONT_SELECT_0", "FONT_8BIT_KERNING")
    
    #Add a prompt and set it's font to FONT_4BIT
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_PROMPT')
    test_utils.edit_widget_props("ID_XPOS", 0)
    test_utils.edit_widget_props("ID_YPOS", 320)
    test_utils.edit_widget_props("ID_WIDTH", 640)
    test_utils.edit_widget_props("ID_HEIGHT", 60)
    test_utils.edit_widget_props("ID_TEXT_ALIGN", 'Left')
    test_utils.edit_widget_props("ID_BORDER_COMBO", test_constants.BORDER_TYPE_NONE)
    test_utils.edit_widget_props("ID_STRING_VAL", "FAWAVAYLTLV_4BIT")
    test_utils.edit_widget_props("ID_FONT_SELECT_0", "FONT_4BIT")
    
    #Add a prompt and set it's font to FONT_4BIT_KERNING
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_PROMPT')
    test_utils.edit_widget_props("ID_XPOS", 0)
    test_utils.edit_widget_props("ID_YPOS", 380)
    test_utils.edit_widget_props("ID_WIDTH", 640)
    test_utils.edit_widget_props("ID_HEIGHT", 60)
    test_utils.edit_widget_props("ID_TEXT_ALIGN", 'Left')
    test_utils.edit_widget_props("ID_BORDER_COMBO", test_constants.BORDER_TYPE_NONE)
    test_utils.edit_widget_props("ID_STRING_VAL", "FAWAVAYLTLV_4BIT_KERNING")
    test_utils.edit_widget_props("ID_FONT_SELECT_0", "FONT_4BIT_KERNING")
    
    #Add a prompt and set it's font to FONT_1BIT
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_PROMPT')
    test_utils.edit_widget_props("ID_XPOS", 0)
    test_utils.edit_widget_props("ID_YPOS", 440)
    test_utils.edit_widget_props("ID_WIDTH", 640)
    test_utils.edit_widget_props("ID_HEIGHT", 20)
    test_utils.edit_widget_props("ID_TEXT_ALIGN", 'Left')
    test_utils.edit_widget_props("ID_BORDER_COMBO", test_constants.BORDER_TYPE_NONE)
    test_utils.edit_widget_props("ID_STRING_VAL", "FAWAVAYLTLV_1BIT")
    test_utils.edit_widget_props("ID_FONT_SELECT_0", "FONT_1BIT")
    
    #Add a prompt and set it's font to FONT_1BIT_KERNING
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_PROMPT')
    test_utils.edit_widget_props("ID_XPOS", 0)
    test_utils.edit_widget_props("ID_YPOS", 460)
    test_utils.edit_widget_props("ID_WIDTH", 640)
    test_utils.edit_widget_props("ID_HEIGHT", 20)
    test_utils.edit_widget_props("ID_TEXT_ALIGN", 'Left')
    test_utils.edit_widget_props("ID_BORDER_COMBO", test_constants.BORDER_TYPE_NONE)
    test_utils.edit_widget_props("ID_STRING_VAL", "FAWAVAYLTLV_1BIT_KERNING")
    test_utils.edit_widget_props("ID_FONT_SELECT_0", "FONT_1BIT_KERNING")
    test_utils.compare_result()

    #test the output will not be changed
    print('save project')
    test_utils.toolbar_save()
    test_utils.generate_all()
    test_utils.click_generate_button(1)
    test_utils.cmp_output_files(test_font_output_files)
    
def test_custom_system_font():
    test_utils.open_project("test_system_font.gxp")
    test_utils.compare_result()

def test_font_height():
    test_utils.open_project("test_font_height.gxp")
    test_utils.compare_result()

def get_test_header(): 
    notes =  "*          Studio Add/Edit Font Test                  *\n"
    notes += "*                                                     *\n"
    notes += "* Test compressed font                                *\n"
    notes += "* Test extended unicode support                       *\n"
    notes += "* Test font height                                    *\n"
    return notes
    
def run_font_tests(generate, screenshot):

    test_utils.output_test_header(get_test_header())
    test_utils.setup(generate, screenshot, 'test_font_golden_file')
    
    print('create new project')
    run_new_project_create(project_name)
    
    print('add fonts')
    run_font_add()
    
    print('add widgets')
    run_widgets_add()
    test_utils.compare_result()
    
    screen_name = "main_window"
    
    # change color format to 8
    test_utils.configure_project()
    test_utils.configure_display_color_format('GX_COLOR_FORMAT_8BIT_PALETTE')
    test_utils.set_aa_text_colors(16)
    test_utils.save_project_configure()
    add_text_color()
    test_utils.select_project_tree_node('prompt_2')
    test_utils.edit_widget_props("ID_NORMAL_TEXT_COLOR", 'TEXT_COLOR')
    test_utils.select_project_tree_node(screen_name)
    test_utils.compare_result()
    
    # disable dave2d drawing
    test_utils.configure_project()
    test_utils.open_advanced_settings()
    test_utils.check_2d_drawing_engine(0)
    test_utils.save_synergy_settings_dialog()
    test_utils.save_project_configure()
    test_utils.select_project_tree_node(screen_name)
    test_utils.compare_result()
    
    # change cpu type to generic
    test_utils.configure_project()
    test_utils.configure_cpu_type('generic')
    test_utils.save_project_configure()
    test_utils.select_project_tree_node(screen_name)
    test_utils.compare_result()
    
    print('run kerning font test')
    test_utils.configure_project()
    #Change display color format to 16bpp first.
    test_utils.configure_display_color_format('GX_COLOR_FORMAT_565RGB')
    test_utils.save_project_configure()
    test_utils.select_project_tree_node(screen_name)
    run_kerning_font_test()
    
    print('run custom system font test')
    test_custom_system_font()
    
    print('run font height test')
    test_font_height()
    
    test_utils.write_end('Font Tests')
