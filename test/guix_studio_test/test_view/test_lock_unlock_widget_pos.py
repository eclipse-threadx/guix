import os
import sys
import time
import test_utils
import test_constants
import logging

def get_test_header():

    notes =  "*      Studio Lock/Unlock Widget Position Tests       *\n"
    notes += "*                                                     *\n"
    notes += "* Test widget move with widget position been locked   *\n"
    notes += "* Test drag resize with widget position been locked   *\n"
    notes += "* Test position edit with widget position been locked *\n"
    notes += "* Test following toolbar buttons                      *\n"
    notes += "*   with widget position been locked:                 *\n"
    notes += "*   align left                                        *\n"
    notes += "*   align right                                       *\n"
    notes += "*   align top                                         *\n"
    notes += "*   align bottom                                      *\n"
    notes += "*   vertical space equally                            *\n"
    notes += "*   horizontal space equally                          *\n"
    notes += "*   equal width                                       *\n"
    notes += "*   equal height                                      *\n"
    notes += "*   size to fit                                       *"
    return notes

def select_three_widgets():
    test_utils.select_multi_widgets('window_1')
    test_utils.select_multi_widgets('prompt_1')
    test_utils.select_multi_widgets('text_view_1')
    
def run_lock_unlock_widget_pos_tests(generate, screenshot):

    test_utils.output_test_header(get_test_header()) 
    test_utils.setup(generate, screenshot, 'test_lock_unlock_widget_pos_golden_file')
    
    # open test project
    test_utils.open_project('generic_16bpp.gxp')
    
    # lock widget position
    test_utils.lock_unlock_widget_positions(1)

    #lock widget position
    widget_name = 'prompt_2'
    test_utils.select_project_tree_node(widget_name)
    test_utils.toolbar_size_to_fit()
    
    test_utils.select_project_tree_node(widget_name)
    test_utils.drag_left_top(widget_name, -5, -8, test_utils.CLOSE_NOTIFICATION_MSG)
    test_utils.drag_left_top(widget_name, 1, 1, test_utils.CLOSE_NOTIFICATION_MSG)
    test_utils.drag_right_top(widget_name, 11, -10, test_utils.CLOSE_NOTIFICATION_MSG)
    test_utils.drag_right_top(widget_name, -3, 3, test_utils.CLOSE_NOTIFICATION_MSG)
    test_utils.drag_left_bottom(widget_name, -5, 8, test_utils.CLOSE_NOTIFICATION_MSG)
    test_utils.drag_left_bottom(widget_name, 5, -6, test_utils.CLOSE_NOTIFICATION_MSG)
    test_utils.drag_right_bottom(widget_name, 19, 19, test_utils.CLOSE_NOTIFICATION_MSG)
    test_utils.drag_right_bottom(widget_name, -9, -9, test_utils.CLOSE_NOTIFICATION_MSG)
    test_utils.drag_move(widget_name, 10, 10, 0) 
    test_utils.compare_result()
    
    # select two more widgets
    test_utils.select_multi_widgets('window_1')
    test_utils.select_multi_widgets('prompt_1')
    test_utils.select_multi_widgets('text_view_1')
    
    # Test resize with toolbar buttons with widget position been locked
    test_utils.toolbar_vspace_equally()
    select_three_widgets()
    test_utils.toolbar_hspace_equally()
    select_three_widgets()
    test_utils.toolbar_equal_width()
    select_three_widgets()
    test_utils.toolbar_equal_height()
    select_three_widgets()
    test_utils.toolbar_align_left()
    select_three_widgets()
    test_utils.toolbar_align_top()
    select_three_widgets()
    test_utils.toolbar_align_right()
    select_three_widgets()
    test_utils.toolbar_align_bottom()
    
    test_utils.select_project_tree_node('window')

    test_utils.edit_widget_props_post("ID_XPOS", 7)# a notify message will show up to tell the widget is locked, resize is not allowed
    test_utils.close_message_dialog()
    test_utils.edit_widget_props_post("ID_YPOS", 7)# a notify message will show up to tell the widget is locked, resize is not allowed
    test_utils.close_message_dialog()
    test_utils.edit_widget_props_post("ID_WIDTH", 600)# a notify message will show up to tell the widget is locked, resize is not allowed
    test_utils.close_message_dialog()
    test_utils.edit_widget_props_post("ID_HEIGHT", 400)# a notify message will show up to tell the widget is locked, resize is not allowed
    test_utils.close_message_dialog()
    
    #unlock widget position
    test_utils.lock_unlock_widget_positions(0)
    test_utils.select_project_tree_node('window')
    test_utils.edit_widget_props("ID_XPOS", 7)
    test_utils.compare_result()
    
    test_utils.write_end("Test Lock/Unlock Widget Position")

