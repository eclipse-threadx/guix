import os
import sys
import time
import test_utils
import test_constants

theme_names = ['theme0', 'theme1', 'theme2']
bad_theme_name = "#badname"

multi_themes_output_files = [
"multi_themes_resources.c",
"multi_themes_resources.h",
"multi_themes_specifications.c",
"multi_themes_specifications.h",
"multi_themes_resources_big_endian.c",
"multi_themes_resources_big_endian.h"]

def get_test_header():

    notes =  "*          Studio Multi Themes Tests                  *\n"
    notes += "*                                                     *\n"
    notes += "* Add and Edit multiple themes                        *\n"
    notes += "* Little endian source format resource generation     *\n"
    notes += "* Big endian source format resource generation        *\n"
    notes += "* Binary format generation                            *\n"
    notes += "* Test if delete pixelmap folder is in sync           *\n"
    notes += "* Test if add pixelmap folder is in sync              *\n"
    notes += "* Test if edit pixelmap folder name is in sync        *\n"
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
    test_utils.delete_folder(0)
    
    # configure languages
    test_utils.configure_languages()  
    test_utils.add_language()
    test_utils.add_language() 
    test_utils.select_language_index(2)
    test_utils.select_language_id('Latin')
    test_utils.select_language_index(3)
    test_utils.select_language_id('French')
    test_utils.save_language_configure()
    
    test_utils.toolbar_save()
   
def run_configure_themes():
    test_utils.configure_themes()
    test_utils.select_display_name('display_1')
    test_utils.add_theme()
    test_utils.add_theme()
    
    # bad name testing
    test_utils.select_theme_index(2)
    test_utils.set_theme_name(bad_theme_name)
    test_utils.post_select_theme_index(3)
    # a message will popup to notify that the theme name is
    # not meet ANSCII C name rules
    test_utils.close_message_dialog()
    
    test_utils.post_select_theme_index(1)
    test_utils.close_message_dialog()

    test_utils.save_theme_configure(test_utils.CLOSE_NO_WAIT)
    test_utils.close_message_dialog()
    test_utils.set_theme_name("theme_name")
 
    for theme in theme_names:
        test_utils.select_theme_index(theme_names.index(theme) + 1)
        test_utils.set_theme_name(theme)

    test_utils.save_theme_configure() 
    
def set_active_theme(theme_name):
    test_utils.configure_themes()
    test_utils.set_active_theme(theme_name)
    test_utils.save_theme_configure()
    
def run_add_resources():
    # add font Vera.ttf
    test_utils.click_resource_group('FONT_GROUP')
    test_utils.add_font() 
    test_utils.set_font_path('..\\..\\fonts\\Vera.ttf')
    test_utils.set_font_name('BIG_FONT')
    test_utils.set_font_format('GX_FONT_FORMAT_8BPP')
    test_utils.set_font_height(30)  
    test_utils.save_font_edit()
    
    test_utils.add_font() 
    test_utils.set_font_path('..\\..\\fonts\\Vera.ttf')
    test_utils.set_font_name('SMALL_FONT')
    test_utils.set_font_format('GX_FONT_FORMAT_8BPP')
    test_utils.set_font_height(10)  
    test_utils.save_font_edit()
    
    test_utils.click_resource_group('FONT_GROUP')
      
    # add color
    test_utils.click_resource_group('COLOR_GROUP')
    test_utils.add_color()
    test_utils.set_color_name('GREEN')
    test_utils.set_color_red(102)
    test_utils.set_color_green(201)
    test_utils.set_color_blue(159)
    test_utils.save_color_edit()
    
    test_utils.add_color()
    test_utils.set_color_name('RED')
    test_utils.set_color_red(216)
    test_utils.set_color_green(121)
    test_utils.set_color_blue(140)
    test_utils.save_color_edit()
    test_utils.click_resource_group('COLOR_GROUP')
    
    #Add pixelmap folder
    test_utils.add_pixelmap_folder()
    test_utils.set_folder_name('folder_2')
    test_utils.save_folder_name_edit()
    
    test_utils.add_pixelmap_folder()
    test_utils.set_folder_name('folder_3')
    test_utils.save_folder_name_edit()
    
    #Add pixelmaps
    test_utils.click_resource_group('PIXELMAP_GROUP')
    test_utils.add_pixelmaps('Custom', '..\\pixelmaps\\', 'save_icon.png')
    test_utils.add_pixelmaps('folder_2', '..\\pixelmaps\\', 'orange_button.png')
    test_utils.add_pixelmaps('folder_3', '..\\pixelmaps\\', 'bg_tile.png')
    test_utils.click_resource_group('PIXELMAP_GROUP')
    
    test_utils.click_resource_group('PIXELMAP_GROUP')
    test_utils.remove_pixelmap_folder('folder_3')
    test_utils.click_resource_group('PIXELMAP_GROUP')
    
    #Add strings
    test_utils.edit_string()
    test_utils.sort_string(test_constants.STRING_SORT_TYPE_ID)
    test_utils.add_string()
    test_utils.edit_string_id('STRING_0')
    test_utils.edit_top_string('string 0')
    test_utils.add_string()
    test_utils.edit_string_id('STRING_8')
    test_utils.edit_top_string('string 8')
    test_utils.add_string()
    test_utils.edit_string_id('STRING_2')
    test_utils.edit_top_string('string 2')
    test_utils.add_string()
    test_utils.edit_string_id('STRING_3')
    test_utils.edit_top_string('string 3')
    test_utils.save_string_edit()
    
def run_add_widgets():
    test_utils.select_project_tree_node('display_1')
    
    #create default folder to add widget
    test_utils.insert_folder()
    test_utils.select_project_tree_node('default_folder')

    test_utils.add_widget('GX_TYPE_WINDOW')
    test_utils.edit_widget_props("ID_WIDGET_NAME", "simple_window")
    test_utils.edit_widget_props("ID_NORMAL_COLOR", "RED") 
    test_utils.edit_widget_props("ID_XPOS", 160)
    test_utils.edit_widget_props("ID_YPOS", 120)
    test_utils.edit_widget_props("ID_WIDTH", 320)
    test_utils.edit_widget_props("ID_HEIGHT", 240)
    
    test_utils.select_project_tree_node('simple_window')
    test_utils.add_widget('GX_TYPE_ICON')
    test_utils.edit_widget_props("ID_PIXELMAP_0", 'SAVE_ICON')
    test_utils.edit_widget_props("ID_XPOS", 207)
    test_utils.edit_widget_props("ID_YPOS", 148)
    test_utils.edit_widget_props("ID_WIDTH", 18)
    test_utils.edit_widget_props("ID_HEIGHT", 18)
    
    test_utils.select_project_tree_node('simple_window')
    test_utils.add_widget('GX_TYPE_PIXELMAP_PROMPT')
    test_utils.edit_widget_props("ID_PIXELMAP_0", 'ORANGE_BUTTON')
    test_utils.edit_widget_props("ID_FONT_SELECT_0", 'SMALL_FONT')
    test_utils.edit_widget_props("ID_NORMAL_COLOR", "GREEN") 
    test_utils.edit_widget_props("ID_XPOS", 252)
    test_utils.edit_widget_props("ID_YPOS", 212)
    test_utils.edit_widget_props("ID_WIDTH", 141)
    test_utils.edit_widget_props("ID_HEIGHT", 70)
    
    test_utils.select_project_tree_node('simple_window')
    test_utils.add_widget('GX_TYPE_PROMPT')
    test_utils.edit_widget_props("ID_STRING_ID", "STRING_8")
    test_utils.edit_widget_props("ID_XPOS", 292)
    test_utils.edit_widget_props("ID_YPOS", 147)
    
    test_utils.select_project_tree_node('simple_window')
    test_utils.add_widget('GX_TYPE_PROMPT')
    test_utils.edit_widget_props("ID_STRING_ID", "STRING_2")
    test_utils.edit_widget_props("ID_XPOS", 186)
    test_utils.edit_widget_props("ID_YPOS", 310)
    
def edit_resource_in_theme_2():
    #edit color
    test_utils.click_resource_group('COLOR_GROUP')
    test_utils.delete_color('GREEN')
    test_utils.edit_color('RED')
    test_utils.set_color_name('NEW_RED')
    test_utils.set_color_red(47)
    test_utils.set_color_green(136)
    test_utils.set_color_blue(200)
    test_utils.save_color_edit()
    test_utils.click_resource_group('COLOR_GROUP')
    
    #edit font
    test_utils.click_resource_group('FONT_GROUP')
    test_utils.delete_font('BIG_FONT')
    test_utils.edit_font('SMALL_FONT')
    test_utils.set_font_name('NEW_SMALL_FONT')
    test_utils.set_font_height(30)  
    test_utils.save_font_edit()
    test_utils.click_resource_group('FONT_GROUP')
        
    #edit pixelmap
    test_utils.click_resource_group('PIXELMAP_GROUP')
    test_utils.click_pixelmap_folder('DEFAULT_PIXELMAP_FOLDER', 'System')
    test_utils.disable_pixelmap('CHECKBOX_OFF')
    test_utils.disable_pixelmap('CHECKBOX_ON')
    test_utils.click_pixelmap_folder('DEFAULT_PIXELMAP_FOLDER', 'System')
    
    test_utils.click_pixelmap_folder('CUSTOM_PIXELMAP_FOLDER', 'Custom')
    test_utils.delete_pixelmap('SAVE_ICON')
    test_utils.click_pixelmap_folder('CUSTOM_PIXELMAP_FOLDER', 'Custom')
    
    test_utils.click_pixelmap_folder('CUSTOM_PIXELMAP_FOLDER', 'folder_2')
    test_utils.edit_pixelmap('ORANGE_BUTTON')
    test_utils.set_image_path('..\\..\\pixelmaps\\black_pause.png')
    test_utils.set_image_id_name('NEW_ORANGE_BUTTON')
    test_utils.save_pixelmap_edit();
    test_utils.click_pixelmap_folder('CUSTOM_PIXELMAP_FOLDER', 'folder_2')
    test_utils.click_resource_group('PIXELMAP_GROUP')
    
def edit_resource_in_theme_0():
    #edit color
    test_utils.click_resource_group('COLOR_GROUP')
    test_utils.edit_color('NEW_RED')
    test_utils.set_color_red(192)
    test_utils.set_color_green(80)
    test_utils.set_color_blue(53)
    test_utils.save_color_edit()
    test_utils.click_resource_group('COLOR_GROUP')
    
    #edit font
    test_utils.click_resource_group('FONT_GROUP')
    test_utils.edit_font('NEW_SMALL_FONT')
    test_utils.set_font_height(20)  
    test_utils.save_font_edit()
    test_utils.click_resource_group('FONT_GROUP')
    
    #edit pixelmap
    test_utils.click_resource_group('PIXELMAP_GROUP')
    test_utils.click_pixelmap_folder('CUSTOM_PIXELMAP_FOLDER', 'folder_2')
    test_utils.edit_pixelmap('NEW_ORANGE_BUTTON')
    test_utils.set_image_path('..\\..\\pixelmaps\\red_apple.png')
    test_utils.click_pixelmap_folder('CUSTOM_PIXELMAP_FOLDER', 'folder_2')
    test_utils.save_pixelmap_edit()
    test_utils.click_resource_group('PIXELMAP_GROUP')
    
def add_delete_pixelmap_folder():
    #set 'NEW_ORANGE_BUTTON' to 8bit palette format
    test_utils.click_resource_group('PIXELMAP_GROUP')
    test_utils.click_pixelmap_folder('CUSTOM_PIXELMAP_FOLDER', 'folder_2')
    test_utils.edit_pixelmap('NEW_ORANGE_BUTTON')
    test_utils.set_output_format('GX_COLOR_FORMAT_8BIT_PALETTE')
    test_utils.set_palette_type('PALETTE_TYPE_SHARED')
    test_utils.save_pixelmap_edit()
    test_utils.click_pixelmap_folder('CUSTOM_PIXELMAP_FOLDER', 'folder_2')
    test_utils.click_resource_group('PIXELMAP_GROUP')
    
    #copy 'prompt'
    test_utils.select_project_tree_node('prompt')
    test_utils.toolbar_copy()
    
    #delete 'Custom' pixelmap folder
    test_utils.click_resource_group('PIXELMAP_GROUP')
    test_utils.remove_pixelmap_folder('folder_2')
    test_utils.compare_result()
    
    set_active_theme(theme_names[1])
    test_utils.compare_result()
    
    set_active_theme(theme_names[0])
    test_utils.compare_result()
    
    #paste 'prompt' to 'simple_window'
    test_utils.select_project_tree_node('simple_window')
    test_utils.toolbar_paste()
    
    #reset active theme
    set_active_theme(theme_names[1])
    test_utils.compare_result()
    
    #reset active theme
    set_active_theme(theme_names[2])
    test_utils.compare_result()
    
    #add a new pixelmap folder 'test_folder'
    test_utils.add_pixelmap_folder()
    test_utils.set_folder_name('test_folder')
    test_utils.save_folder_name_edit()

    #reset active theme
    set_active_theme(theme_names[1])
    
    #add a pixlemap to 'test_folder'
    test_utils.click_resource_group('PIXELMAP_GROUP')
    test_utils.click_pixelmap_folder('CUSTOM_PIXELMAP_FOLDER', 'test_folder')
    test_utils.add_pixelmaps('test_folder', '..\\pixelmaps\\', 'save_icon.png')
    test_utils.click_pixelmap_folder('CUSTOM_PIXELMAP_FOLDER', 'test_folder')
    test_utils.click_resource_group('PIXELMAP_GROUP')
    
    #set new pixelmap to 'simple_window'
    test_utils.select_project_tree_node('simple_window')
    test_utils.edit_widget_props("ID_PIXELMAP_0", test_constants.PIXELMAP_SAVE_ICON)
    test_utils.compare_result()
    

def run_multi_themes_tests(generate, screenshot):
    
    test_utils.output_test_header(get_test_header())
    test_utils.setup(generate, screenshot, 'test_multi_themes_golden_file')
    
    run_new_project_create("multi_themes")
    run_configure_themes()
    set_active_theme(theme_names[1])
    test_utils.compare_result()
    
    run_add_resources()
    run_add_widgets()
    test_utils.compare_result()

    set_active_theme(theme_names[0])
    test_utils.compare_result()
    
    set_active_theme(theme_names[2])
    test_utils.compare_result()
    
    edit_resource_in_theme_2()
    test_utils.compare_result()

    set_active_theme(theme_names[1])
    test_utils.compare_result()

    set_active_theme(theme_names[0])
    test_utils.compare_result()
    
    edit_resource_in_theme_0()
    test_utils.compare_result()
        
    set_active_theme(theme_names[1])
    test_utils.compare_result()

    set_active_theme(theme_names[2])
    test_utils.compare_result()
    
    #test if configure language will broke string table
    test_utils.configure_languages()
    test_utils.save_language_configure()
    test_utils.edit_string()
    test_utils.cancel_string_edit()
    
    test_utils.generate_all()
    test_utils.check_resource('display_1', 'theme0', 0)
    test_utils.check_resource('Languages', 'Latin', 0)
    test_utils.click_generate_button()
    
    test_utils.configure_project()
    test_utils.configure_big_endian(1)
    test_utils.save_project_configure()
    
    add_delete_pixelmap_folder()
    
    test_utils.generate_resources()
    test_utils.set_custom_resource_name("multi_themes_resources_big_endian")
    test_utils.click_generate_button()
    
    test_utils.cmp_output_files(multi_themes_output_files)
    test_utils.close_project
    
    test_utils.write_end('Multi-Themes Tests')
