# -*- coding: utf-8 -*-
import os
import sys
import time
import test_utils
import test_constants
import filecmp

def get_test_header():
    notes =  "*           Synergy Setting Test                      *\n"
    notes += "*                                                     *\n"
    notes += "* Edit project cofiguration and change cput type      *\n"
    notes += "* Save synergy advanced setting and cancel project    *\n"
    notes += "*   configuration                                     *\n"
    notes += "* Save synergy advanced setting and save project      *\n"
    notes += "*   configuration                                     *\n"
    return notes

def run_synergy_setting_test():
    # configure project
    
    # set cpu type to synergy and color depth to 565rgb    
    test_utils.configure_project()
    test_utils.configure_display_color_format('GX_COLOR_FORMAT_565RGB')
    test_utils.configure_cpu_type('synergy')
    test_utils.save_project_configure()
    
    #---------------------------------------------------------------------#
    # add a new font with compression option enabled                      #
    #---------------------------------------------------------------------#
    
    # open font folder
    test_utils.click_resource_group('FONT_GROUP')

    # open font NotoSansHans-Regular.otf
    test_utils.add_font()   
    test_utils.set_font_path('..\\fonts\\NotoSansHans-Regular.otf')
    test_utils.set_font_name(test_constants.FONT_NOTOSANSHANS)
    test_utils.set_font_format('GX_FONT_FORMAT_8BPP')
    test_utils.set_font_height(40)
    test_utils.set_font_compression(1)
    test_utils.save_font_edit()
    
    # cloce font folder
    test_utils.click_resource_group('FONT_GROUP')
    
    #select button screen
    test_utils.select_project_tree_node("button_screen")
    test_utils.compare_result()
    
    #disable 2d drawing engine and cancel project configuration
    test_utils.configure_project()
    test_utils.open_advanced_settings()
    test_utils.check_2d_drawing_engine(0)
    test_utils.save_synergy_settings_dialog()
    test_utils.cancel_project_configure()
    
    #select button screen
    test_utils.select_project_tree_node("button_screen")
    test_utils.compare_result()
    
    #disable 2d drawing enigne and save project configuration
    test_utils.configure_project()
    test_utils.open_advanced_settings()
    test_utils.check_2d_drawing_engine(0)
    test_utils.save_synergy_settings_dialog()
    test_utils.save_project_configure(test_utils.CLOSE_NO_WAIT)
    
    test_utils.close_message_dialog()#message about 2d drawing engine is disabled, pixelmaps is reset to default
    test_utils.wait_dialog_close(test_utils.CONFIGURE_PROJECT_DIALOG)
    #select button screen
    test_utils.select_project_tree_node("button_screen")
    test_utils.compare_result()

def run_synergy_setting_tests(generate, screenshot):

    test_utils.output_test_header(get_test_header())
    test_utils.setup(generate, screenshot, 'test_synergy_setting_golden_file')
    
    print('open all_widgets_execute.gxp')
    test_utils.open_project("all_widgets_execute.gxp")

    print('run synergy setting tests')
    run_synergy_setting_test()

    test_utils.write_end('Synergy Setting Tests')