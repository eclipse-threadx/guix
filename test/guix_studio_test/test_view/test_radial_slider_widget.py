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

def get_test_header():
    notes =  "*             Radial Slider Test                      *\n"
    notes += "*                                                     *\n"
    notes += "* Add and Edit Radial Slider                          *\n"
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
    
    test_utils.add_pixelmaps('Custom', '..\\pixelmaps\\', 'bg_washer_on.png,wheel_dot_green.png')

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
    
def add_edit_radial_slider_widget():
    #---------------------------------------#
    #add and edti radial slider             #
    #---------------------------------------#
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_RADIAL_SLIDER')
    widget_name = "radial_slider"

    # edit radial slider properties
    test_utils.edit_widget_props("ID_WIDGET_NAME", widget_name)
    test_utils.select_project_tree_node(widget_name)
    test_utils.edit_widget_props('ID_PIXELMAP_0', 'BG_WASHER_ON')
    test_utils.toolbar_size_to_fit()
    
    test_utils.select_project_tree_node(widget_name)
    test_utils.edit_widget_props('ID_PIXELMAP_2', 'WHEEL_DOT_GREEN')
    test_utils.edit_widget_props('ID_RADIAL_SLIDER_XCENTER', 187)
    test_utils.edit_widget_props('ID_RADIAL_SLIDER_YCENTER', 131)
    test_utils.edit_widget_props('ID_RADIAL_SLIDER_RADIUS', 108)
    test_utils.edit_widget_props('ID_RADIAL_SLIDER_TRACK_WIDTH', 44)
    test_utils.edit_widget_props('ID_RADIAL_SLIDER_CURRENT_ANGLE', 242)
    test_utils.edit_widget_props('ID_RADIAL_SLIDER_MIN_ANGLE', -63)
    test_utils.edit_widget_props('ID_RADIAL_SLIDER_MAX_ANGLE', 242)
    test_utils.edit_widget_props('ID_RADIAL_SLIDER_TOTAL_STEPS', 20)
    test_utils.edit_widget_props('ID_RADIAL_SLIDER_DELAY', 1)
    test_utils.edit_widget_props('ID_RADIAL_SLIDER_ANIMATION_STYLE', 'circ ease out')
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

    #undo animation style set
    test_utils.undo()
    #undo animation delay set
    test_utils.undo()
    #undo animation total steps set
    test_utils.undo()
    #undo max angle set
    test_utils.undo()
    #undo min angle set
    test_utils.undo()
    #undo current angle set
    test_utils.undo()
    #undo track width set
    test_utils.undo()
    #undo radius set
    test_utils.undo()
    #undo ycenter set
    test_utils.undo()
    #undo xcenter set
    test_utils.undo()
    #undo needle pixelmap set
    test_utils.undo()
    #undo size to fit
    test_utils.undo()
    #unco background pixelmap set
    test_utils.undo()

def run_radial_slider_widget_tests(generate, screenshot):

    test_utils.output_test_header(get_test_header())
    test_utils.setup(generate, screenshot, 'test_radial_slider_widget_golden_file')
    
    print('create new project')
    run_new_project_create("test_radial_slider_widget")

    print('add main screen')
    add_main_screen()
    
    print('add and edit radial slider widget')
    add_edit_radial_slider_widget()

    test_utils.toolbar_save()
    test_utils.write_end('Radial Slider Widget Tests')