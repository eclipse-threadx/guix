# -*- coding: utf-8 -*-
import os
import sys
import time
import test_utils
import test_constants

folder_hane = "default_folder"
screen_name = "window"
widget_name = "gauge"

def get_test_header():
    notes =  "*                    Gauge Test                       *\n"
    notes += "*                                                     *\n"
    notes += "* Edit Gauge Needle Pixelmap                          *\n"
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
    test_utils.add_pixelmaps('Custom', '..\\pixelmaps\\', 'Gaugemeter.png')
    test_utils.add_pixelmaps('Custom', '..\\pixelmaps\\', 'needle.png')

def add_edit_gauge():
    #---------------------------------------#
    #add and edti circular gauge            #
    #---------------------------------------#
    test_utils.select_project_tree_node(screen_name)
    
    test_utils.add_widget('GX_TYPE_CIRCULAR_GAUGE')
    test_utils.edit_widget_props("ID_WIDGET_NAME", widget_name)
    test_utils.edit_widget_props("ID_XPOS", 0)
    test_utils.edit_widget_props("ID_YPOS",0)
    test_utils.edit_widget_props("ID_WIDTH", 280)
    test_utils.edit_widget_props("ID_HEIGHT", 280)
    test_utils.edit_widget_props("ID_TRANSPARENT", 1)
    test_utils.edit_widget_props("ID_GAUGE_START_ANGLE", 0)
    test_utils.edit_widget_props("ID_GAUGE_ANIMATION_STEPS", 30)
    test_utils.edit_widget_props("ID_GAUGE_ANIMATION_DELAY", 1)
    test_utils.edit_widget_props("ID_GAUGE_NEEDLE_XPOS", 140)
    test_utils.edit_widget_props("ID_GAUGE_NEEDLE_YPOS", 140)
    test_utils.edit_widget_props("ID_GAUGE_NEEDLE_XCOR", 20)
    test_utils.edit_widget_props("ID_GAUGE_NEEDLE_YCOR", 99)
    test_utils.edit_widget_props("ID_PIXELMAP_0", 'GAUGEMETER')
    test_utils.edit_widget_props("ID_PIXELMAP_1", 'GAUGEMETER')
    test_utils.edit_widget_props("ID_PIXELMAP_2", 'NEEDLE')
    
    test_utils.compare_result()
    
    # Switch to another sreen
    test_utils.select_project_tree_node(folder_hane)
    test_utils.add_widget('GX_TYPE_WINDOW')
    
    # Switch back to gauge screen
    test_utils.select_project_tree_node(screen_name)
    test_utils.compare_result()
    
    
def run_gauge_tests(generate, screenshot):

    test_utils.output_test_header(get_test_header())
    test_utils.setup(generate, screenshot, 'test_gauge_golden_file')

    print('create new project')
    run_new_project_create("test_gauge")
    
    print('add pixelmaps')
    add_pixelmaps()
    
    print('add and edit gauge')
    add_edit_gauge()

    test_utils.toolbar_save()
    
    test_utils.write_end('Gauge Tests')