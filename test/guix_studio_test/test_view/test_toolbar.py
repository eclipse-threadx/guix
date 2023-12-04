import os
import sys
import time
import test_utils
import test_constants

def get_test_header():

    notes =  "*          Studio Toolbar Tests                       *\n"
    notes += "*                                                     *\n"
    notes += "* Test grid and snap setting dialog                   *\n"
    notes += "* Test grid and snap function                         *\n"
    notes += "* Test toolbar button before project open             *\n"
    notes += "* Test following toolbar buttons:                     *\n"
    notes += "*   align left                                        *\n"
    notes += "*   align right                                       *\n"
    notes += "*   align top                                         *\n"
    notes += "*   align bottom                                      *\n"
    notes += "*   vertical space equally                            *\n"
    notes += "*   horizontal space equally                          *\n"
    notes += "*   equal width                                       *\n"
    notes += "*   equal height                                      *\n"
    notes += "*   move to front                                     *\n"
    notes += "*   move to back                                      *\n"
    notes += "*   zoom in                                           *\n"
    notes += "*   zoom out                                          *\n"
    return notes
    
def run_drag_resize():
    # drag left
    widget_name = 'text_input_2'
    test_utils.select_project_tree_node(widget_name)
    test_utils.drag_left(widget_name, -13)
    test_utils.drag_left(widget_name, 2)
    test_utils.drag_right(widget_name, 12)
    test_utils.drag_right(widget_name, -3)
    test_utils.drag_top(widget_name, -13)
    test_utils.drag_top(widget_name, 1)
    test_utils.drag_bottom(widget_name, 12)
    test_utils.drag_bottom(widget_name, -5)
    
    widget_name = 'prompt_2'
    test_utils.select_project_tree_node(widget_name)
    test_utils.drag_left_top(widget_name, -5, -8)
    test_utils.drag_left_top(widget_name, 1, 1)
    test_utils.drag_right_top(widget_name, 11, -10)
    test_utils.drag_right_top(widget_name, -3, 3)
    test_utils.drag_left_bottom(widget_name, -5, 8)
    test_utils.drag_left_bottom(widget_name, 5, -6)
    test_utils.drag_right_bottom(widget_name, 19, 19)
    test_utils.drag_right_bottom(widget_name, -9, -9)
    
    test_utils.compare_result()
    
#test toolbar button click when no widget been selected
def run_toolbar_button_click():
    test_utils.toolbar_cut()
    test_utils.toolbar_copy()
    test_utils.toolbar_paste()
    test_utils.toolbar_align_left()
    test_utils.toolbar_align_right()
    test_utils.toolbar_align_top()
    test_utils.toolbar_align_bottom()
    test_utils.toolbar_vspace_equally()
    test_utils.toolbar_hspace_equally()
    test_utils.toolbar_equal_width()
    test_utils.toolbar_equal_height()
    test_utils.toolbar_move_to_front()
    test_utils.toolbar_move_to_back()
    test_utils.toolbar_size_to_fit()
    test_utils.toolbar_zoom_in()
    test_utils.toolbar_zoom_out()
    test_utils.toolbar_record_macro()
    test_utils.close_dialog(test_utils.RECORD_MACRO_DIALOG)
    test_utils.toolbar_playback_macro()
    test_utils.close_dialog(test_utils.PLAYBACK_MACRO_DIALOG)
    
def run_toolbar_function():

    #move widget that already in back
    test_utils.select_project_tree_node('window')
    test_utils.toolbar_move_to_back()

    #move widget that already in front
    test_utils.select_project_tree_node("window")
    test_utils.toolbar_move_to_front()

    test_utils.zoom_in(100)
    test_utils.zoom_in(200)
    test_utils.zoom_in(300)
    test_utils.zoom_in(400)
    test_utils.zoom_in(300)
    test_utils.zoom_in(200)
    test_utils.zoom_in(100)
    
    # click zoom in button 5 times
    test_utils.toolbar_zoom_in()
    test_utils.toolbar_zoom_in()
    test_utils.toolbar_zoom_in()
    test_utils.toolbar_zoom_in()
    test_utils.toolbar_zoom_in()
    
    # click zoom out button 5 times
    test_utils.toolbar_zoom_out()
    test_utils.toolbar_zoom_out()
    test_utils.toolbar_zoom_out()
    test_utils.toolbar_zoom_out()
    test_utils.toolbar_zoom_out()
    
    # select 2 widgets
    test_utils.select_multi_widgets('window_1')
    test_utils.select_multi_widgets('prompt_1')
    test_utils.toolbar_align_left()
    
    test_utils.select_multi_widgets('window_1')
    test_utils.select_multi_widgets('prompt_1')
    test_utils.toolbar_align_right()
    
    test_utils.select_multi_widgets('window_1')
    test_utils.select_multi_widgets('prompt_1')
    test_utils.toolbar_align_bottom()
    
    test_utils.select_multi_widgets('window_1')
    test_utils.select_multi_widgets('prompt_1')
    test_utils.toolbar_align_top()
    
    test_utils.select_multi_widgets('window_1')
    test_utils.select_multi_widgets('prompt_1')
    test_utils.toolbar_equal_width()
    
    test_utils.select_multi_widgets('window_1')
    test_utils.select_multi_widgets('prompt_1')
    test_utils.toolbar_equal_height()
    
    # select 3 wigets
    test_utils.select_multi_widgets('prompt_2')
    test_utils.select_multi_widgets('text_input_2')
    test_utils.select_multi_widgets('button_2')
    test_utils.toolbar_vspace_equally()
    
    test_utils.select_multi_widgets('prompt_2')
    test_utils.select_multi_widgets('text_input_2')
    test_utils.select_multi_widgets('text_view_1')
    test_utils.toolbar_hspace_equally()
    
    test_utils.select_project_tree_node('prompt_1')
    test_utils.toolbar_move_to_back()

    test_utils.select_project_tree_node("window_1")
    test_utils.select_project_tree_node('prompt_1')
    test_utils.toolbar_move_to_front()
    
    test_utils.select_project_tree_node("window_1")
    test_utils.select_project_tree_node('prompt_1')
    test_utils.toolbar_size_to_fit()
    
    test_utils.select_project_tree_node("window_1")
    test_utils.select_project_tree_node('text_input_1')
    test_utils.toolbar_size_to_fit()
    
    test_utils.select_project_tree_node("window_1")
    test_utils.select_project_tree_node('prompt_2')
    test_utils.toolbar_size_to_fit()
    
    test_utils.select_project_tree_node("window_1")
    test_utils.select_project_tree_node('button_1')
    test_utils.toolbar_size_to_fit()
    
    test_utils.select_project_tree_node("window_1")
    test_utils.select_project_tree_node('button_2')
    test_utils.toolbar_size_to_fit()
    
    test_utils.select_project_tree_node("window_1")
    test_utils.select_project_tree_node('multi_line_button_1')
    test_utils.toolbar_size_to_fit()
    
    test_utils.select_project_tree_node("window_1")
    test_utils.select_project_tree_node('checkbox_1')
    test_utils.toolbar_size_to_fit()
    
    test_utils.select_project_tree_node("window_1")
    test_utils.select_project_tree_node('checkbox_2')
    test_utils.toolbar_size_to_fit()
    
    test_utils.select_project_tree_node("window_1")
    test_utils.select_project_tree_node('text_view_1')
    test_utils.toolbar_size_to_fit()
    
    test_utils.select_project_tree_node("window_1")
    test_utils.select_project_tree_node('progress_bar_1')
    test_utils.toolbar_size_to_fit()
    
    test_utils.select_project_tree_node("window_1")
    test_utils.select_project_tree_node('pixelmap_slider_1')
    test_utils.toolbar_size_to_fit()
    
    test_utils.select_project_tree_node("window_1")
    test_utils.select_project_tree_node('pixelmap_slider_2')
    test_utils.toolbar_size_to_fit()
    
    test_utils.select_project_tree_node("window_1")
    test_utils.select_project_tree_node('icon_button_1')
    test_utils.toolbar_size_to_fit()
    
    test_utils.select_project_tree_node("window_1")
    test_utils.select_project_tree_node('sprite_1')
    test_utils.toolbar_size_to_fit()
    
    test_utils.select_project_tree_node("window_1")
    test_utils.select_project_tree_node('horizontal_list_1')
    test_utils.toolbar_size_to_fit()
    
    test_utils.select_project_tree_node("window_1")
    test_utils.select_project_tree_node('drop_list_1')
    test_utils.toolbar_size_to_fit()
    
    test_utils.select_project_tree_node("window_1")
    test_utils.select_project_tree_node('vertical_list_1')
    test_utils.toolbar_size_to_fit()
    
    test_utils.select_project_tree_node("window_1")
    test_utils.select_project_tree_node('radio_button_1')
    test_utils.toolbar_size_to_fit()
    
def run_toolbar_tests(generate, screenshot):
    
    test_utils.output_test_header(get_test_header())
    test_utils.setup(generate, screenshot, 'test_toolbar_golden_file')
 
    test_utils.close_project()
    print('run toolbar button click')
    run_toolbar_button_click()

    test_utils.open_project('generic_16bpp.gxp')
 
    print('run drag resize function')
    run_drag_resize()
    
    # open grid and snap setting dialog
    test_utils.grid_snap_setting()
    test_utils.check_show_grid(1)
    test_utils.check_snap(1)
    test_utils.save_grid_snap_setting()  
    run_drag_resize()
    
    test_utils.grid_snap_setting()
    test_utils.set_grid_spacing(13)
    test_utils.set_snap_spacing(13)
    test_utils.save_grid_snap_setting()  
    run_drag_resize()
    
    test_utils.zoom_in(200)
    run_drag_resize()
    
    print('run toolbar function')
    run_toolbar_function()
    
    test_utils.write_end('Toolbar Tests')
