# -*- coding: utf-8 -*-
import os
import sys
import time
import test_utils
import test_constants

display_name = "main_display"
screen_name = "window"
widget_name = "sprite"

test_scroll_wheel_widget_output_files = [
"test_sprite_specifications.c",
"test_sprite_specifications.h",
"test_sprite_resources.c",
"test_sprite_resources.h"]

def get_test_header():
    notes =  "*                   Sprite Test                       *\n"
    notes += "*                                                     *\n"
    notes += "* Edit Sprite Frames                                  *\n"
    notes += "* Import Frames From Pixelmap Folder                  *\n"
    return notes
    
def run_new_project_create(project_name):

    # create a new project
    test_utils.toolbar_create()
    test_utils.set_new_project_path(test_utils.DEFAULT_OUTPUT_FILE_PATH)
    test_utils.set_new_project_name(project_name)
    test_utils.save_new_project_create()
        
    # close message dialog when create new project
    test_utils.close_message_dialog()
    
    test_utils.toolbar_save()
    
def add_pixelmaps():
    #---------------------------------------#
    #add pixelmaps                          #
    #---------------------------------------#
    test_utils.add_pixelmaps('Custom', '..\\pixelmaps\\', 'frame_000.png')
    test_utils.add_pixelmaps('Custom', '..\\pixelmaps\\', 'frame_001.png')
    test_utils.add_pixelmaps('Custom', '..\\pixelmaps\\', 'fish.png')
    test_utils.add_pixelmaps('Custom', '..\\pixelmaps\\', 'black_pause.png') 
    test_utils.add_pixelmaps('Custom', '..\\pixelmaps\\gif\\', 'sprite.gif')
    
    test_utils.edit_pixelmap('FISH')
    test_utils.set_image_id_name('SPRITE')
    test_utils.save_pixelmap_edit(test_utils.CLOSE_NO_WAIT)
    test_utils.close_error_dialog()
    
    test_utils.set_image_id_name('SPRITE_F0')
    test_utils.save_pixelmap_edit(test_utils.CLOSE_NO_WAIT)
    test_utils.close_error_dialog()
    
    test_utils.set_image_id_name('SPRITE_FISH')
    test_utils.save_pixelmap_edit()
    

def add_edit_sprite():
    #---------------------------------------#
    #add and edti sprite                    #
    #---------------------------------------#
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_SPRITE')

    test_utils.edit_widget_props("ID_WIDGET_NAME", widget_name)
    test_utils.compare_result()
    
    # open sprite edit dialog
    test_utils.select_project_tree_node(widget_name)
    test_utils.edit_widget_props_post('ID_SPRITE_EDIT_FRAMES', 1)
    test_utils.wait_dialog_open(test_utils.SPRITE_FRAME_EDIT_DIALOG)
    
    test_utils.edit_sprite_total_frames(3)
    test_utils.edit_sprite_frame(1, 'FRAME_001')
    test_utils.edit_sprite_frame_alpha(255)
    test_utils.edit_sprite_frame(2, 'SPRITE_FISH')
    test_utils.edit_sprite_frame(3, 'FRAME_000')
    
    test_utils.save_sprite_frame_edit()
    test_utils.compare_result()
    
    # add second sprite widget
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_SPRITE')
    test_utils.edit_widget_props("ID_WIDGET_NAME", 'sprite_1')
    test_utils.edit_widget_props("ID_XPOS", 25)
    test_utils.edit_widget_props("ID_YPOS", 154)
    test_utils.edit_widget_props("ID_PIXELMAP_0", 'SPRITE')
    test_utils.toolbar_size_to_fit()
    test_utils.compare_result()
    
    test_utils.edit_pixelmap('SPRITE')
    test_utils.set_image_path('..\\..\\pixelmaps\\gif\\sprite_4_frames.gif')
    test_utils.save_pixelmap_edit()
    test_utils.compare_result()
    
    test_utils.delete_pixelmap('BLACK_PAUSE')
    
def test_import_frames():
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_SPRITE')
    test_utils.edit_widget_props("ID_XPOS", 0)
    test_utils.edit_widget_props("ID_YPOS", 0)
    
    test_utils.edit_widget_props_post('ID_SPRITE_EDIT_FRAMES', 1)
    test_utils.wait_dialog_open(test_utils.SPRITE_FRAME_EDIT_DIALOG)
    
    test_utils.import_sprite_frames()
    test_utils.save_sprite_frame_import_edit()
    
    test_utils.check_apply_to_all_frames(1)
    test_utils.edit_sprite_frame_alpha(200)
    test_utils.edit_sprite_frame_xoffset(5)
    test_utils.edit_sprite_frame_yoffset(8)
    test_utils.edit_sprite_frame_delay(10)
    
    test_utils.save_sprite_frame_edit()
    test_utils.compare_result()
    
def run_sprite_tests(generate, screenshot):

    test_utils.output_test_header(get_test_header())
    test_utils.setup(generate, screenshot, 'test_sprite_golden_file')

    print('create new project')
    run_new_project_create("test_sprite")
    
    print('add pixelmaps')
    add_pixelmaps()
    
    print('add and edit sprite')
    add_edit_sprite()
    
    print('test import frames')
    test_import_frames()

    test_utils.toolbar_save()
    test_utils.generate_all()
    test_utils.click_generate_button()
    test_utils.cmp_output_files(test_scroll_wheel_widget_output_files)
    
    test_utils.write_end('Sprite Tests')