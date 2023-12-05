# -*- coding: utf-8 -*-
import os
import sys
import time
import test_utils
import test_constants
import filecmp

display_1_name = "main_display"
display_2_name = "second_display"
source_folder_1 = "source_folder_1"
source_folder_2 = "source_folder_2"
target_folder_1 = "target_folder_1"
target_folder_2 = "target_folder_2"

project_import_output_files_test1 = [
"test_project_import_target_main_display_resources.c",
"test_project_import_target_main_display_resources.h",
"test_project_import_target_second_display_resources.c",
"test_project_import_target_second_display_resources.h",
"test_project_import_target_specifications.c",
"test_project_import_target_specifications.h"]

project_import_output_files_test2 = [
"test_compile_imported_output_resources.c",
"test_compile_imported_output_resources.h",
"test_compile_imported_output_specifications.c",
"test_compile_imported_output_specifications.h"]



def get_test_header():
    notes =  "*          Project Import Test                        *\n"
    notes += "*                                                     *\n"
    notes += "* Import screens from select project                  *\n"
    notes += "* Import screens that based on template               *\n"
    notes += "* Import duplicate strings                            *\n"
    notes += "* Import duplicate colors                             *\n"
    notes += "* Import screens to project that have multiple themes *\n"
    return notes
    
def add_target_colors():
    test_utils.click_resource_group('COLOR_GROUP')
    test_utils.add_color()
    test_utils.set_color_name('green')
    test_utils.set_color_red(15)
    test_utils.set_color_green(235)
    test_utils.set_color_blue(16)
    test_utils.save_color_edit()

    test_utils.add_color()
    test_utils.set_color_name('red')
    test_utils.set_color_hue(10)
    test_utils.set_color_sat(60)
    test_utils.set_color_lum(49)
    test_utils.save_color_edit()
    test_utils.click_resource_group('COLOR_GROUP')
    
def add_source_colors():
    test_utils.click_resource_group('COLOR_GROUP')
    test_utils.add_color()
    test_utils.set_color_name('green')
    test_utils.set_color_red(15)
    test_utils.set_color_green(100)
    test_utils.set_color_blue(16)
    test_utils.save_color_edit()

    test_utils.add_color()
    test_utils.set_color_name('red')
    test_utils.set_color_hue(12)
    test_utils.set_color_sat(88)
    test_utils.set_color_lum(49)
    test_utils.save_color_edit()
    test_utils.click_resource_group('COLOR_GROUP')
    
def add_target_strings():
    test_utils.click_resource_group('STRING_GROUP')
    test_utils.edit_string()
    
    test_utils.add_string()
    test_utils.edit_string_id('TEST_STRING_ID_1')
    test_utils.edit_top_string('target_string_1')
    
    test_utils.add_string()
    test_utils.edit_string_id('TEST_STRING_ID_2')
    test_utils.edit_top_string('target_string_2')
    
    test_utils.add_string()
    test_utils.edit_string_id('TEST_STRING_ID_3')
    test_utils.edit_top_string('target_string_3')
    
    test_utils.save_string_edit()
    test_utils.click_resource_group('STRING_GROUP')
    
def add_source_strings():
    test_utils.click_resource_group('STRING_GROUP')
    test_utils.edit_string()
    
    test_utils.add_string()
    test_utils.edit_string_id('TEST_STRING_ID_1')
    test_utils.edit_top_string('source_string_4')
    
    test_utils.add_string()
    test_utils.edit_string_id('TEST_STRING_ID_2')
    test_utils.edit_top_string('source_string_5')
    
    test_utils.add_string()
    test_utils.edit_string_id('TEST_STRING_ID_3')
    test_utils.edit_top_string('source_string_6')
    
    test_utils.save_string_edit()
    test_utils.click_resource_group('STRING_GROUP')

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
    test_utils.select_project_tree_node("default_folder")
    test_utils.delete_folder(0)
    
    # create folder
    test_utils.select_project_tree_node(display_1_name)
    if project_name == 'test_project_import_source':
        test_utils.insert_folder()
        test_utils.rename_folder(source_folder_1)
    else:
        test_utils.insert_folder()
        test_utils.rename_folder(target_folder_1)
    
    test_utils.select_project_tree_node(display_2_name)
    if project_name == 'test_project_import_source':
        test_utils.insert_folder()
        test_utils.rename_folder(source_folder_2)
    else:
        test_utils.insert_folder()
        test_utils.rename_folder(target_folder_2)
        
    test_utils.select_project_tree_node(display_1_name)
        
    # configure themes
    test_utils.configure_themes()
    test_utils.select_display_name('display_1')
    test_utils.add_theme()
    test_utils.add_theme()
    test_utils.save_theme_configure() 
    
    # add fonts
    test_utils.click_resource_group('FONT_GROUP')
    
    test_utils.add_font() 
    test_utils.set_font_path('..\\..\\fonts\\Vera.ttf')
    test_utils.set_font_name('NORMAL_FONT')
    test_utils.set_font_format('GX_FONT_FORMAT_8BPP')
    test_utils.set_font_height(28)  
    test_utils.save_font_edit()

    test_utils.add_font() 
    test_utils.set_font_path('..\\..\\fonts\\Vera.ttf')
    test_utils.set_font_name('MIDDLE_FONT')
    test_utils.set_font_format('GX_FONT_FORMAT_8BPP')
    test_utils.set_font_height(30)  
    test_utils.save_font_edit()

    test_utils.click_resource_group('FONT_GROUP')
    
    test_utils.add_pixelmaps('Custom', '..\\pixelmaps\\', 'red_apple.png')
    
def edit_source_display_1():
    #---------------------------------------#
    #edit display 1                         #
    #---------------------------------------#
    
    test_utils.select_project_tree_node(source_folder_1)
    test_utils.add_widget('GX_TYPE_WINDOW')
    test_utils.edit_widget_props("ID_XPOS", 0)
    test_utils.edit_widget_props("ID_YPOS", 0)
    test_utils.edit_widget_props("ID_WIDTH", 640)
    test_utils.edit_widget_props("ID_HEIGHT", 480)
    test_utils.edit_widget_props("ID_PIXELMAP_0", "RED_APPLE")
    test_utils.edit_widget_props("ID_WIDGET_NAME","base")
    test_utils.edit_widget_props("ID_TEMPLATE", 1)
    
    test_utils.select_project_tree_node(source_folder_1)
    test_utils.add_template("base")
    test_utils.edit_widget_props("ID_WIDGET_NAME","base_0")
    test_utils.edit_widget_props("ID_TEMPLATE", 1)
    
    test_utils.select_project_tree_node("base_0")
    test_utils.add_widget('GX_TYPE_TEXT_BUTTON')
    test_utils.edit_widget_props("ID_BORDER_COMBO", test_constants.BORDER_TYPE_RAISED)
    test_utils.edit_widget_props("ID_XPOS", 293)
    test_utils.edit_widget_props("ID_YPOS", 31)
    test_utils.edit_widget_props("ID_WIDTH", 279)
    test_utils.edit_widget_props("ID_HEIGHT", 151)
    test_utils.edit_widget_props("ID_FONT_SELECT_0", "NORMAL_FONT")
    test_utils.edit_widget_props("ID_NORMAL_COLOR", "green")
    test_utils.edit_widget_props("ID_SELECTED_COLOR", "red")
    test_utils.edit_widget_props("ID_STRING_ID", "TEST_STRING_ID_1")
    
    test_utils.select_project_tree_node(source_folder_1)
    test_utils.add_template("base_0")
    test_utils.edit_widget_props("ID_WIDGET_NAME","base_1")
    test_utils.edit_widget_props("ID_TEMPLATE", 1)
    
    test_utils.select_project_tree_node("base_1")
    test_utils.add_widget('GX_TYPE_TEXT_BUTTON')
    test_utils.edit_widget_props("ID_BORDER_COMBO", test_constants.BORDER_TYPE_RAISED)
    test_utils.edit_widget_props("ID_XPOS", 288)
    test_utils.edit_widget_props("ID_YPOS", 235)
    test_utils.edit_widget_props("ID_WIDTH", 304)
    test_utils.edit_widget_props("ID_HEIGHT", 178)
    test_utils.edit_widget_props("ID_FONT_SELECT_0", "MIDDLE_FONT")
    test_utils.edit_widget_props("ID_STRING_ID", "TEST_STRING_ID_2")
    
    test_utils.select_project_tree_node(source_folder_1)
    test_utils.add_template("base_1")
    test_utils.edit_widget_props("ID_WIDGET_NAME","derived_1")
    
    test_utils.select_project_tree_node(source_folder_1)
    test_utils.add_template("base_1")
    test_utils.edit_widget_props("ID_WIDGET_NAME","derived_2")

def edit_source_display_2():
    #---------------------------------------#
    #edit display 2                         #
    #---------------------------------------#
    
    test_utils.select_project_tree_node(source_folder_2)
    test_utils.add_widget('GX_TYPE_WINDOW')
    test_utils.edit_widget_props("ID_XPOS", 160)
    test_utils.edit_widget_props("ID_YPOS", 120)
    test_utils.edit_widget_props("ID_WIDTH", 320)
    test_utils.edit_widget_props("ID_HEIGHT", 240)
    test_utils.add_widget('GX_TYPE_BUTTON')
    test_utils.edit_widget_props("ID_BORDER_COMBO", test_constants.BORDER_TYPE_RAISED)
    
def edit_target_display_1():
    import_project_name = os.path.abspath(test_utils.DEFAULT_OUTPUT_FILE_PATH) + '\\test_project_import_source.gxp'
    test_utils.select_project_tree_node(display_1_name)
    test_utils.select_project_tree_node(target_folder_1)
    test_utils.import_project(import_project_name)
    test_utils.save_project_import_dialog(test_utils.CLOSE_NO_WAIT)
    test_utils.select_option(0)
    test_utils.select_option(1)
    test_utils.wait_dialog_close('IMPORT_PROJECT_DIALOG')
    test_utils.select_project_tree_node("base")
    test_utils.compare_result()
    test_utils.select_project_tree_node("base_0")
    test_utils.compare_result()
    test_utils.select_project_tree_node("base_1")
    test_utils.compare_result()
    test_utils.select_project_tree_node("derived_1")
    test_utils.compare_result()
    test_utils.select_project_tree_node("derived_2")
    test_utils.compare_result()
    
    test_utils.select_project_tree_node(target_folder_1)
    test_utils.import_project(import_project_name)
    test_utils.check_import_screen("main_display", "base", 0)
    test_utils.check_import_screen("main_display", "base_0", 0)
    test_utils.check_import_screen("main_display", "base_1", 0)
    test_utils.check_import_screen("main_display", "derived_1", 0)
    test_utils.save_project_import_dialog(test_utils.CLOSE_NO_WAIT)
    test_utils.select_option(2)
    test_utils.wait_dialog_close('IMPORT_PROJECT_DIALOG')

    # select window to make sure the import is success and screenshot will not be changed if studio is changed.
    test_utils.select_project_tree_node('window_1')
    test_utils.compare_result()
    
def edit_target_display_2():
    import_project_name = os.path.abspath(test_utils.DEFAULT_OUTPUT_FILE_PATH) + '\\test_project_import_source.gxp'
    test_utils.select_project_tree_node(target_folder_2)
    test_utils.import_project(import_project_name)
    test_utils.check_import_screen("main_display", "base", 0)
    test_utils.check_import_screen("main_display", "base_0", 0)
    test_utils.check_import_screen("main_display", "base_1", 0)
    test_utils.check_import_screen("main_display", "derived_1", 0)
    test_utils.save_project_import_dialog()
    
    # select window to make sure the import is success and screenshot will not be changed if studio is changed.
    test_utils.select_project_tree_node('window_1')
    test_utils.compare_result()
    
def test_compile_imported_output():
    # create a new project
    test_utils.toolbar_create()
    test_utils.set_new_project_path(test_utils.DEFAULT_OUTPUT_FILE_PATH)
    test_utils.set_new_project_name("test_compile_imported_output")
    test_utils.save_new_project_create()
    
    # configure project
    test_utils.configure_project()
    test_utils.configure_display_num(1)
    test_utils.select_display_index(1)
    test_utils.configure_x_res(640)
    test_utils.configure_y_res(480)
    test_utils.configure_display_name(display_1_name)
    test_utils.save_project_configure()
    
    # create folder
    test_utils.close_message_dialog()
    test_utils.select_project_tree_node("window")
    test_utils.delete_widget(0)
        
    test_utils.select_project_tree_node("default_folder")
    test_utils.add_widget('GX_TYPE_WINDOW')
    test_utils.edit_widget_props("ID_XPOS", 0)
    test_utils.edit_widget_props("ID_YPOS", 0)
    test_utils.edit_widget_props("ID_WIDTH", 320)
    test_utils.edit_widget_props("ID_HEIGHT", 240)
    test_utils.select_project_tree_node("default_folder")
    test_utils.add_widget('GX_TYPE_WINDOW')
    test_utils.edit_widget_props("ID_XPOS", 160)
    test_utils.edit_widget_props("ID_YPOS", 120)
    test_utils.edit_widget_props("ID_WIDTH", 320)
    test_utils.edit_widget_props("ID_HEIGHT", 240)
    test_utils.edit_widget_props("ID_WIDGET_NAME","window_2")
    test_utils.toolbar_save()
    
    import_project_name = os.path.abspath(test_utils.DEFAULT_OUTPUT_FILE_PATH) + "\\test_compile_imported_output.gxp"
    test_utils.select_project_tree_node("default_folder")
    test_utils.import_project(import_project_name)
    test_utils.save_project_import_dialog()
    test_utils.toolbar_save()
    test_utils.generate_all()
    test_utils.click_generate_button()
    test_utils.cmp_output_files(project_import_output_files_test2)

    
def run_project_import_tests(generate, screenshot):
    test_utils.output_test_header(get_test_header())
    test_utils.setup(generate, screenshot, 'test_project_import_golden_file')
    
    print('create source project')
    run_new_project_create("test_project_import_source")
    add_source_colors()
    add_source_strings()
    
    edit_source_display_1()
    edit_source_display_2()
    
    test_utils.toolbar_save()
    
    print('create target project')
    run_new_project_create("test_project_import_target")
    add_target_colors()
    add_target_strings()
    
    #just to help with testing
    test_utils.toolbar_save();

    edit_target_display_1()
    edit_target_display_2()
    
    #test undo project import
    test_utils.undo()
    test_utils.undo()
    
    #test if template info is deleted when delete a folder
    test_utils.select_project_tree_node("target_folder_1")
    test_utils.delete_folder(0)
    test_utils.select_project_tree_node("target_folder_2")
    test_utils.undo()
    
    test_utils.toolbar_save()
    test_utils.generate_all()
    test_utils.click_generate_button()
    test_utils.cmp_output_files(project_import_output_files_test1)
    
    #test of compile the output after import: test bug fixed in revistion 12591
    test_compile_imported_output()

    test_utils.write_end('Project Import Tests')