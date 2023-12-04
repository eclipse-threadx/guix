# -*- coding: utf-8 -*-
import os
import sys
import time
import test_utils
import test_constants
import filecmp

project_name = "test_display_conf.gxp"

color_depth = [
32,
24,
16,
8,
4,
1
]
cpu_type = [
"generic",
"synergy"
]
rgb_bits = [
1555,
4444,
332
]

def get_test_header():
    notes =  "*          Project configure Test                     *\n"
    notes += "*                                                     *\n"

    return notes
    
def run_new_project_create():
    print('create new project')
    # create a new project
    test_utils.toolbar_create()
    test_utils.set_new_project_path(test_utils.DEFAULT_OUTPUT_FILE_PATH)
    test_utils.set_new_project_name('test_display_conf')
    test_utils.save_new_project_create()
    
    # configure project
    test_utils.configure_project()
    test_utils.configure_display_num(1)
    test_utils.select_display_index(1)
    test_utils.configure_x_res(640)
    test_utils.configure_y_res(480)
    test_utils.save_project_configure()
    
    test_utils.close_message_dialog()
    
def run_project_color_format_config():
    print('configure display')

    for cpu_index in cpu_type:
        test_utils.configure_project()
        test_utils.configure_cpu_type(cpu_index)
        if(cpu_index == "synergy"):
            test_utils.save_project_configure(test_utils.CLOSE_NO_WAIT)
            test_utils.close_message_dialog()
            test_utils.wait_dialog_close(test_utils.CONFIGURE_PROJECT_DIALOG)
        else:
            test_utils.save_project_configure()

        for color_index in color_depth:
            test_utils.configure_project()
            test_utils.configure_display_color_depth(color_index)
            test_utils.save_project_configure()
            for rgb_index in rgb_bits:
                for index in range(2):
                    test_utils.configure_project()
                    test_utils.configure_display_rgb_bits(rgb_index)
                    if cpu_index =="generic":
                        if color_index==16 and(rgb_index == 4444 or rgb_index == 1555):
                            test_utils.save_project_configure(test_utils.CLOSE_NO_WAIT)
                            test_utils.close_message_dialog() 
                            test_utils.wait_dialog_close(test_utils.CONFIGURE_PROJECT_DIALOG)
                        elif color_index==8 and rgb_index == 332:
                            test_utils.save_project_configure(test_utils.CLOSE_NO_WAIT)                    
                            test_utils.close_message_dialog()
                            test_utils.wait_dialog_close(test_utils.CONFIGURE_PROJECT_DIALOG)
                        else:
                            test_utils.save_project_configure()
                            
                        test_utils.toolbar_save()
                        test_utils.close_project()
                        test_utils.open_project(test_utils.DEFAULT_OUTPUT_FILE_PATH + project_name)
                    else:
                        test_utils.save_project_configure()
                        
                        test_utils.toolbar_save()
                        test_utils.close_project()
                        test_utils.open_project(test_utils.DEFAULT_OUTPUT_FILE_PATH + project_name)
                    

    test_utils.close_project()            
    
def run_project_display_configure_tests(generate, screenshot):
    test_utils.output_test_header(get_test_header())
    test_utils.setup(generate, screenshot, 'test_display_conf_golden_file')
    
    run_new_project_create()
    
    run_project_color_format_config()

    test_utils.write_end("Test display config")
