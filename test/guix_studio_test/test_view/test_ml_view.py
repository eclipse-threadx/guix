# -*- coding: utf-8 -*-
import test_utils

display_name = "main_display"
folder_name = "default_folder"
screen_name = "main_screen"

def get_test_header():
    notes =  "*      Multi Line Text View Widget Test               *\n"
    notes += "*                                                     *\n"
    notes += "* Test line space range check                         *\n"
    notes += "* Test whitespace range check                         *\n"
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
    
def test_whitespace_linespace_range():
    #---------------------------------------#
    #add main screen to display             #
    #---------------------------------------#
    
    test_utils.select_project_tree_node("window")

    test_utils.edit_widget_props("ID_XPOS", 0)
    test_utils.edit_widget_props("ID_YPOS", 0)
    test_utils.edit_widget_props("ID_WIDTH", 640)
    test_utils.edit_widget_props("ID_HEIGHT", 480)
    test_utils.edit_widget_props("ID_WIDGET_NAME", screen_name)
    
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_MULTI_LINE_TEXT_VIEW')
    test_utils.edit_widget_props_post('ID_TEXT_VIEW_WHITESPACE', -1)
    test_utils.close_message_dialog()
    test_utils.edit_widget_props_post('ID_TEXT_VIEW_WHITESPACE', 256)
    test_utils.close_message_dialog()
    test_utils.edit_widget_props('ID_TEXT_VIEW_WHITESPACE', 10)
    
    test_utils.edit_widget_props_post('ID_TEXT_VIEW_LINE_SPACE', -129)
    test_utils.close_message_dialog()
    test_utils.edit_widget_props_post('ID_TEXT_VIEW_LINE_SPACE', 128)
    test_utils.close_message_dialog()
    test_utils.edit_widget_props('ID_TEXT_VIEW_LINE_SPACE', 127)
    test_utils.compare_result()

def run_ml_view_tests(generate, screenshot):

    test_utils.output_test_header(get_test_header())
    test_utils.setup(generate, screenshot, 'test_ml_view_golden_file')

    print('create new project')
    run_new_project_create("test_ml_view")
    
    print('test whitespace and linespace range')
    test_whitespace_linespace_range()
 
    test_utils.write_end('ML View Widget Tests')