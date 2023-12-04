# -*- coding: utf-8 -*-
import os
import sys
import time
import test_utils
import test_constants

test_pixelmap_edit_output_files = [
"test_pixelmap_edit_resources.c",
"test_pixelmap_edit_resources.h"]

test_batch_pixelmap_edit_output_files = [
"test_batch_pixelmap_edit_folder_2_resource.c",
"test_batch_pixelmap_edit_folder_3_resource.c",
"test_batch_pixelmap_edit_folder_4_resource.c",
"test_batch_pixelmap_edit_resources.c",
"test_batch_pixelmap_edit_resources.h"]

test_synergy_pixelmap_edit_output_files = [
"test_synergy_pixelmap_edit_resources.c",
"test_synergy_pixelmap_edit_resources.h"]

test_pixelmap_folder_enable_disable_output_files = [
"test_pixelmap_folder_enable_disable_resources.c",
"test_pixelmap_folder_enable_disable_resources.h"]

test_pixelmap_group_enable_output_files = [
"test_pixelmap_group_enable_resources.c",
"test_pixelmap_group_enable_resources.h"]

test_pixelmap_group_disable_output_files = [
"test_pixelmap_group_disable_resources.c",
"test_pixelmap_group_disable_resources.h"]

display_name = "main_display"
screen_name = "window"

def get_test_header(): 
    notes =  "*      Studio Add/Edit Pixelmap Test                  *\n"
    notes += "*                                                     *\n"
    notes += "* Test duplicate pixelmap add                         *\n"
    notes += "* Test synergy pixelmap edit                          *\n"
    notes += "* Test batch pixelmap edit                            *\n"
    notes += "* Test pixelmap folder enable disable                 *\n"
    notes += "* Test pixelmap group enable disable                  *\n"
    
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
    test_utils.save_project_configure()
    
    # close message dialog when create new
    test_utils.close_message_dialog()

def run_duplicate_pixlemap_add():
    test_utils.add_pixelmaps('Custom', '..\\pixelmaps\\', 'save_icon.png,yellow_bg.png')
    test_utils.add_pixelmaps('Custom', '..\\pixelmaps\\', 'save_icon.png,yellow_bg.png', test_utils.CLOSE_NO_WAIT)
    
    test_utils.click_yes_to_please_confirm_dialog()
    test_utils.click_no_to_please_confirm_dialog()

def run_synergy_pixelmap_edit():
    test_utils.open_project('synergy_16bpp.gxp');
    test_utils.add_pixelmaps('Custom', '..\\pixelmaps\\', 'fish_small_411.jpg');
    test_utils.edit_pixelmap('FISH_SMALL_411');
    test_utils.check_raw_format(1, test_utils.CLOSE_ERROR_MSG); # Test crash when edit raw format
    test_utils.cancel_pixelmap_edit();
    test_utils.edit_pixelmap('_565RGB_COMPRESSED_FISH')
    test_utils.check_raw_format(1, test_utils.CLOSE_ERROR_MSG); # Test crash when edit raw format
    test_utils.cancel_pixelmap_edit();
    test_utils.close_project()

    # create a new project
    test_utils.toolbar_create()
    test_utils.set_new_project_path(test_utils.DEFAULT_OUTPUT_FILE_PATH)
    test_utils.set_new_project_name('test_synergy_pixelmap_edit')
    test_utils.save_new_project_create()
    
    # configure project
    test_utils.configure_project()
    test_utils.configure_x_res(640)
    test_utils.configure_y_res(480)
    test_utils.configure_display_name(display_name)
    test_utils.configure_cpu_type('synergy')
    test_utils.configure_display_color_format('GX_COLOR_FORMAT_32ARGB')
    test_utils.save_project_configure()
    
    # close message dialog when create new
    test_utils.close_message_dialog()
    
    test_utils.add_pixelmaps('Custom', '..\\pixelmaps\\', 'i_patientlist_lg.png')
    test_utils.edit_pixelmap('I_PATIENTLIST_LG')
    test_utils.check_alpha(0)
    test_utils.check_alpha(1)
    test_utils.save_pixelmap_edit()
    
def run_batch_pixelmap_edit():
    test_utils.add_pixelmap_folder()
    test_utils.set_folder_name('folder_1')
    test_utils.save_folder_name_edit()
    
    test_utils.add_pixelmap_folder()
    test_utils.set_folder_name('folder_2')
    test_utils.save_folder_name_edit()
    
    test_utils.add_pixelmap_folder()
    test_utils.set_folder_name('folder_3')
    test_utils.save_folder_name_edit()
    
    test_utils.add_pixelmap_folder()
    test_utils.set_folder_name('folder_4')
    test_utils.save_folder_name_edit()
    
    test_utils.add_pixelmaps('folder_1', '..\\pixelmaps\\', 'save_icon.png')
    test_utils.add_pixelmaps('folder_2', '..\\pixelmaps\\', 'save_icon.png,yellow_bg.png')
    test_utils.click_yes_to_please_confirm_dialog()
    test_utils.add_pixelmaps('folder_3', '..\\pixelmaps\\', 'black_pause.png,button_disabled.png')
    test_utils.add_pixelmaps('folder_4', '..\\pixelmaps\\', 'collapse.png,expand.png')
   
    test_utils.click_pixelmap_folder('CUSTOM_PIXELMAP_FOLDER', 'folder_1')
    test_utils.edit_pixelmaps()
    test_utils.check_alpha(1)
    test_utils.save_pixelmap_edit()
    
    test_utils.click_pixelmap_folder('CUSTOM_PIXELMAP_FOLDER', 'folder_2')
    test_utils.edit_pixelmaps()
    test_utils.check_custom_output(1)
    test_utils.set_custom_output_file("test_batch_pixelmap_edit_folder_2_resource.c")
    test_utils.set_output_format('GX_COLOR_FORMAT_565RGB')
    test_utils.save_pixelmap_edit()
    
    test_utils.edit_pixelmap('BLACK_PAUSE')
    test_utils.set_output_format('GX_COLOR_FORMAT_565RGB')
    test_utils.save_pixelmap_edit()

    test_utils.click_pixelmap_folder('CUSTOM_PIXELMAP_FOLDER', 'folder_3')
    test_utils.edit_pixelmaps()
    test_utils.check_custom_output(1)
    test_utils.set_custom_output_file("test_batch_pixelmap_edit_folder_3_resource.c")
    test_utils.check_alpha(0)
    test_utils.check_compress(0)
    test_utils.check_dither(1)
    test_utils.save_pixelmap_edit()
    
    test_utils.click_pixelmap_folder('DEFAULT_PIXELMAP_FOLDER', 'System')
    test_utils.edit_pixelmaps()
    test_utils.set_output_format('GX_COLOR_FORMAT_8BIT_PALETTE')
    test_utils.set_palette_type('PALETTE_TYPE_PRIVATE')
    test_utils.save_pixelmap_edit()
    
    test_utils.click_pixelmap_folder('CUSTOM_PIXELMAP_FOLDER', 'folder_4')
    test_utils.edit_pixelmaps()
    test_utils.check_custom_output(1)
    test_utils.set_custom_output_file("test_batch_pixelmap_edit_folder_4_resource.c")
    test_utils.check_raw_format(1)
    test_utils.check_alpha(1)
    test_utils.check_compress(1)
    test_utils.check_dither(1)
    test_utils.save_pixelmap_edit()
    
def run_pixelmap_folder_enable_disable():
    # create a new project
    run_new_project_create('test_pixelmap_folder_enable_disable')
    
    test_utils.add_pixelmap_folder()
    test_utils.set_folder_name('folder_1')
    test_utils.save_folder_name_edit()
    
    test_utils.add_pixelmap_folder()
    test_utils.set_folder_name('folder_2')
    test_utils.save_folder_name_edit()
    
    test_utils.add_pixelmap_folder()
    test_utils.set_folder_name('folder_3')
    test_utils.save_folder_name_edit()
    
    test_utils.add_pixelmaps('folder_1', '..\\pixelmaps\\', 'save_icon.png,yellow_bg.png')
    test_utils.add_pixelmaps('folder_2', '..\\pixelmaps\\', 'black_pause.png,button_disabled.png')
    test_utils.add_pixelmaps('folder_3', '..\\pixelmaps\\', 'bg_title.png')
    
    # disable default pixelmap folder
    test_utils.click_pixelmap_folder('DEFAULT_PIXELMAP_FOLDER', 'System')
    test_utils.disable_pixelmap_folder()
    
    # disable 'SAVE_ICON' under 'folder_1', enable 'folder_1'
    test_utils.disable_pixelmap('SAVE_ICON')
    test_utils.click_pixelmap_folder('CUSTOM_PIXELMAP_FOLDER', 'folder_1')
    test_utils.enable_pixelmap_folder()
    
    # disable 'folder_2'
    test_utils.click_pixelmap_folder('CUSTOM_PIXELMAP_FOLDER', 'folder_2')
    test_utils.disable_pixelmap_folder()
    
    #generate output files and compare result with golden files
    test_utils.toolbar_save()
    test_utils.generate_all()
    test_utils.click_generate_button()
    test_utils.cmp_output_files(test_pixelmap_folder_enable_disable_output_files)
    
def run_pixelmap_group_enable():
    # create a new project
    run_new_project_create('test_pixelmap_group_enable')
    
    test_utils.click_resource_group('PIXELMAP_GROUP')
    test_utils.disable_pixelmap('CHECKBOX_OFF')
    test_utils.enable_pixelmap_folder()
    
    #generate output files and compare result with golden files
    test_utils.toolbar_save()
    test_utils.generate_all()
    test_utils.click_generate_button()
    test_utils.cmp_output_files(test_pixelmap_group_enable_output_files)
    
def run_pixelmap_group_disable():
    # create a new project
    run_new_project_create('test_pixelmap_group_disable')
    
    test_utils.click_resource_group('PIXELMAP_GROUP')
    test_utils.disable_pixelmap_folder()
    
    #generate output files and compare result with golden files
    test_utils.toolbar_save()
    test_utils.generate_all()
    test_utils.click_generate_button()
    test_utils.cmp_output_files(test_pixelmap_group_disable_output_files)

def run_pixelmap_folder_delete():
    print('create new project')
    run_new_project_create('test_pixelmap_folder_delete')
    
    test_utils.add_pixelmap_folder()
    test_utils.set_folder_name('folder_1')
    test_utils.save_folder_name_edit()
    
    test_utils.add_pixelmap_folder()
    test_utils.set_folder_name('folder_2')
    test_utils.save_folder_name_edit()
    
    test_utils.add_pixelmap_folder()
    test_utils.set_folder_name('sprite')
    test_utils.save_folder_name_edit()
    
    test_utils.add_pixelmap_folder()
    test_utils.set_folder_name('scrollbar')
    test_utils.save_folder_name_edit()
    
    test_utils.add_pixelmaps('folder_1', '..\\pixelmaps\\', 'save_icon.png,yellow_bg.png,tfield_fill_small.png,wheel_dot_green.png,top_bar_background.png')
    test_utils.add_pixelmaps('folder_2', '..\\pixelmaps\\', 'black_pause.png,collapse.png,expand.png,fill.png,i_emptyfill_middle.png')
    test_utils.add_pixelmaps('sprite', '..\\pixelmaps\\', 'frame_000.png,frame_001.png')
    test_utils.add_pixelmaps('sprite', '..\\pixelmaps\\gif\\', 'sprite.gif')
    test_utils.add_pixelmaps('scrollbar', '..\\pixelmaps\\', 'i_emptyfill_top.png,i_indicator.png,i_orangefill_bottom.png,i_orangefill_middle.png')
    
    #add icon
    test_utils.select_project_tree_node(screen_name)
    test_utils.edit_widget_props("ID_WIDTH", 640)
    test_utils.edit_widget_props("ID_HEIGHT", 480)
    
    test_utils.add_widget('GX_TYPE_ICON')
    test_utils.edit_widget_props("ID_XPOS", 32)
    test_utils.edit_widget_props("ID_YPOS", 18)
    test_utils.edit_widget_props("ID_PIXELMAP_0", "SAVE_ICON")
    
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_ICON')
    test_utils.edit_widget_props("ID_XPOS", 89)
    test_utils.edit_widget_props("ID_YPOS", 18)
    test_utils.edit_widget_props("ID_PIXELMAP_0", "TFIELD_FILL_SMALL")
    
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_ICON')
    test_utils.edit_widget_props("ID_XPOS", 148)
    test_utils.edit_widget_props("ID_YPOS", 18)
    test_utils.edit_widget_props("ID_PIXELMAP_0", "TOP_BAR_BACKGROUND")
    
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_ICON')
    test_utils.edit_widget_props("ID_XPOS", 219)
    test_utils.edit_widget_props("ID_YPOS", 18)
    test_utils.edit_widget_props("ID_PIXELMAP_0", "WHEEL_DOT_GREEN")
    
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_ICON')
    test_utils.edit_widget_props("ID_XPOS", 483)
    test_utils.edit_widget_props("ID_YPOS", 111)
    test_utils.edit_widget_props("ID_PIXELMAP_0", "YELLOW_BG")
    
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_ICON')
    test_utils.edit_widget_props("ID_XPOS", 32)
    test_utils.edit_widget_props("ID_YPOS", 111)
    test_utils.edit_widget_props("ID_PIXELMAP_0", "BLACK_PAUSE")
    
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_ICON')
    test_utils.edit_widget_props("ID_XPOS", 89)
    test_utils.edit_widget_props("ID_YPOS", 111)
    test_utils.edit_widget_props("ID_PIXELMAP_0", "COLLAPSE")
    
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_ICON')
    test_utils.edit_widget_props("ID_XPOS", 148)
    test_utils.edit_widget_props("ID_YPOS", 111)
    test_utils.edit_widget_props("ID_PIXELMAP_0", "EXPAND")
    
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_ICON')
    test_utils.edit_widget_props("ID_XPOS", 219)
    test_utils.edit_widget_props("ID_YPOS", 111)
    test_utils.edit_widget_props("ID_PIXELMAP_0", "FILL")
    
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_ICON')
    test_utils.edit_widget_props("ID_XPOS", 300)
    test_utils.edit_widget_props("ID_YPOS", 111)
    test_utils.edit_widget_props("ID_PIXELMAP_0", "I_EMPTYFILL_MIDDLE")
    
    #add and edti sprite
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_SPRITE')
    test_utils.edit_widget_props("ID_XPOS", 184)
    test_utils.edit_widget_props("ID_YPOS", 228)
    test_utils.edit_widget_props_post('ID_SPRITE_EDIT_FRAMES', 1)
    test_utils.wait_dialog_open(test_utils.SPRITE_FRAME_EDIT_DIALOG)
    test_utils.edit_sprite_total_frames(2)
    test_utils.edit_sprite_frame(1, 'FRAME_000')
    test_utils.edit_sprite_frame_alpha(255)
    test_utils.edit_sprite_frame(2, 'FRAME_001')
    test_utils.save_sprite_frame_edit()
    
    # add second sprite widget
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_SPRITE')
    test_utils.edit_widget_props("ID_XPOS", 47)
    test_utils.edit_widget_props("ID_YPOS", 228)
    test_utils.edit_widget_props("ID_PIXELMAP_0", 'SPRITE')
    
    # add vertical scrollbar
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_VERTICAL_SCROLL')
    test_utils.edit_widget_props('ID_PIXELMAP_0', 'I_ORANGEFILL_MIDDLE')
    test_utils.edit_widget_props('ID_PIXELMAP_1', 'I_INDICATOR')
    test_utils.edit_widget_props('ID_PIXELMAP_2', 'I_EMPTYFILL_TOP')
    test_utils.edit_widget_props('ID_PIXELMAP_3', 'I_ORANGEFILL_BOTTOM')
    test_utils.edit_widget_props('ID_SCROLL_END_BUTTONS', 0)
    test_utils.edit_widget_props('ID_SCROLL_TILE_BKGND', 1)
    
    test_utils.compare_result()
    
    test_utils.toolbar_save()
    
    test_utils.remove_pixelmap_folder('folder_1')
    
    test_utils.compare_result()
    
def run_pixelmap_edit_tests(generate, screenshot):

    test_utils.output_test_header(get_test_header())
    test_utils.setup(generate, screenshot, 'test_pixlemap_edit_golden_file')
         
    print('create new project')
    run_new_project_create('test_pixelmap_edit')
    
    print('test duplicate pixlemap add')
    run_duplicate_pixlemap_add()
    
    #generate output files and compare result with golden files
    test_utils.toolbar_save()
    test_utils.generate_all()
    test_utils.click_generate_button()
    test_utils.cmp_output_files(test_pixelmap_edit_output_files)
    
    print('test synergy pixelmap edit')
    run_synergy_pixelmap_edit()
    
    #generate output files and compare result with golden files
    test_utils.toolbar_save()
    test_utils.generate_all()
    test_utils.click_generate_button()
    test_utils.cmp_output_files(test_synergy_pixelmap_edit_output_files)
    
    print('create new project')
    run_new_project_create('test_batch_pixelmap_edit')
    
    print('test batch pixelmap edit')
    run_batch_pixelmap_edit()
    
    #generate output files and compare result with golden files
    test_utils.toolbar_save()
    test_utils.generate_all()
    test_utils.click_generate_button()
    test_utils.cmp_output_files(test_batch_pixelmap_edit_output_files)
    
    print('test pixelmap folder enable disable')
    run_pixelmap_folder_enable_disable()
    
    print('test pixelmap group enable')
    run_pixelmap_group_enable()
    
    print('test pixelmap group disable')
    run_pixelmap_group_disable()
    
    print('test pixelmap folder delete')
    run_pixelmap_folder_delete()
    
    test_utils.write_end('Pixelmap Edit Tests')
