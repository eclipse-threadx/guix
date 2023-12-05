import os
import sys
import time
import test_utils
import test_constants
import logging

generic_16bpp_output_files = [
"generic_16bpp_apple.c",
"generic_16bpp_fish.c",
"generic_16bpp_foot.c",
"generic_16bpp_resources.c",
"generic_16bpp_resources.h",
"generic_16bpp_specifications.c",
"generic_16bpp_specifications.h"
]

#run through all the supported color depths
def run_color_depths():

    time.sleep(1)
    test_utils.configure_project()
    test_utils.configure_display_color_format('GX_COLOR_FORMAT_565RGB')
    test_utils.save_project_configure()
    test_utils.select_project_tree_node('window')
    test_utils.compare_result()
    test_utils.move_window(10)
    test_utils.move_window(-10)

    test_utils.configure_project()
    test_utils.configure_display_color_format('GX_COLOR_FORMAT_24XRGB')
    test_utils.save_project_configure()
    test_utils.select_project_tree_node('window')
    test_utils.compare_result()
    test_utils.move_window(10)
    test_utils.move_window(-10)
    
    test_utils.configure_project()
    test_utils.configure_display_color_format('GX_COLOR_FORMAT_32ARGB')
    test_utils.save_project_configure()
    test_utils.select_project_tree_node('window')
    test_utils.compare_result()
    test_utils.move_window(10)
    test_utils.move_window(-10)
 
    test_utils.configure_project()
    test_utils.configure_display_color_format('GX_COLOR_FORMAT_8BIT_PALETTE')
    test_utils.set_aa_text_colors(16)
    test_utils.save_project_configure()
    test_utils.select_project_tree_node('window')
    test_utils.compare_result()
    test_utils.move_window(10)
    test_utils.move_window(-10)
    
def run_cut_copy_paste_undo():
    time.sleep(1)
    test_utils.select_project_tree_node('window')
    test_utils.toolbar_cut()
    test_utils.select_project_tree_node('default_folder')
    test_utils.toolbar_paste()
    
    test_utils.select_project_tree_node('window')
    test_utils.toolbar_copy()
    test_utils.select_project_tree_node('default_folder')
    test_utils.toolbar_paste()
    test_utils.undo()
    
    test_utils.select_project_tree_node('pixelmap_button_1')
    test_utils.move_selected(15)
    test_utils.toolbar_copy()
    test_utils.select_project_tree_node('default_folder')
    test_utils.toolbar_paste()
    test_utils.undo()
    
    test_utils.select_project_tree_node('icon_1')
    test_utils.move_selected(-5)
    test_utils.toolbar_cut()
    test_utils.select_project_tree_node('window')
    test_utils.toolbar_paste()
    test_utils.undo()
    #test_utils.undo()

def get_test_header():

    notes =  "*          Studio Color Format Tests                  *\n"
    notes += "*                                                     *\n"
    notes += "* Verify correct drawing using all image types        *\n"
    notes += "* Verify correct drawing in all output color formats  *\n"
    notes += "* Verify correct drawing using all cpu type settings  *\n"
    return notes
         
def run_color_format_tests(generate, screenshot):

    test_utils.output_test_header(get_test_header()) 
    test_utils.setup(generate, screenshot, 'test_color_formats_golden_file')
    
    test_utils.open_project('generic_16bpp.gxp')
    test_utils.configure_project()
    test_utils.configure_cpu_type('generic')
    test_utils.save_project_configure()
    test_utils.select_project_tree_node('window')
    test_utils.compare_result()
    run_cut_copy_paste_undo()
    run_color_depths()

    test_utils.open_project('generic_16bpp.gxp')
    test_utils.configure_project()
    test_utils.configure_cpu_type('synergy')
    test_utils.save_project_configure()
    test_utils.select_project_tree_node('window')
    test_utils.compare_result()
    run_color_depths()
  
    test_utils.open_project('generic_32bpp.gxp')
    test_utils.configure_project()
    test_utils.configure_cpu_type('generic')
    test_utils.save_project_configure()
    test_utils.select_project_tree_node('window')
    test_utils.compare_result()
    run_color_depths()
    
    test_utils.open_project('generic_32bpp.gxp')
    test_utils.configure_project()
    test_utils.configure_cpu_type('synergy')
    test_utils.save_project_configure()
    test_utils.select_project_tree_node('window')
    test_utils.compare_result()
    run_color_depths()
    
    test_utils.open_project('generic_332rgb.gxp')
    test_utils.configure_project()
    test_utils.configure_cpu_type('generic')
    test_utils.save_project_configure()
    test_utils.select_project_tree_node('window')
    test_utils.compare_result()
    run_color_depths()
    
    test_utils.open_project('generic_8bpp.gxp')
    test_utils.configure_project()
    test_utils.configure_cpu_type('generic')
    test_utils.save_project_configure()
    run_color_depths()
    
    test_utils.open_project('generic_8bpp.gxp')
    test_utils.configure_project()
    test_utils.configure_cpu_type('synergy')
    test_utils.save_project_configure()
    test_utils.select_project_tree_node('window')
    test_utils.compare_result()
    run_color_depths()
    
    # test file generation when custome file is specified
    test_utils.generate('generic_16bpp.gxp', test_utils.studio_exe_path)
    test_utils.cmp_output_files(generic_16bpp_output_files)
    
    #4bpp driver is special case because message dialog while be shown if color depth is changed.
    #So test it here.
    test_utils.open_project('generic_4bpp.gxp')
    test_utils.configure_project()
    test_utils.configure_cpu_type('generic')
    test_utils.save_project_configure()
    test_utils.select_project_tree_node('window')
    test_utils.compare_result()
    test_utils.configure_project()
    test_utils.configure_display_color_format('GX_COLOR_FORMAT_565RGB')
    # test_utils.save_project_configure(test_utils.CLOSE_NO_WAIT)
    # test_utils.close_message_dialog()
    test_utils.save_project_configure()
    test_utils.select_project_tree_node('window')
    test_utils.compare_result()
    run_color_depths()
    test_utils.configure_project()
    test_utils.configure_display_color_format('GX_COLOR_FORMAT_4BIT_GRAY')
    # test_utils.save_project_configure(test_utils.CLOSE_NO_WAIT)
    # test_utils.close_message_dialog()
    test_utils.save_project_configure()
    test_utils.select_project_tree_node('window')
    test_utils.compare_result()
    
    #Same with 4bpp
    test_utils.open_project('generic_1bpp.gxp')
    test_utils.configure_project()
    test_utils.configure_cpu_type('generic')
    test_utils.save_project_configure()
    test_utils.select_project_tree_node('window')
    test_utils.compare_result()
    test_utils.configure_project()
    test_utils.configure_display_color_format('GX_COLOR_FORMAT_565RGB')
    #test_utils.save_project_configure(test_utils.CLOSE_NO_WAIT)
    #test_utils.close_message_dialog()
    test_utils.save_project_configure()
    test_utils.select_project_tree_node('window')
    test_utils.compare_result()
    run_color_depths()
    test_utils.configure_project()
    test_utils.configure_display_color_format('GX_COLOR_FORMAT_MONOCHROME')
    #test_utils.save_project_configure(test_utils.CLOSE_NO_WAIT)
    #test_utils.close_message_dialog()
    test_utils.save_project_configure()
    test_utils.select_project_tree_node('window')
    test_utils.compare_result()
    
    test_utils.open_project('synergy_16bpp.gxp')
    test_utils.configure_project()
    test_utils.configure_cpu_type('synergy')
    test_utils.save_project_configure()
    test_utils.select_project_tree_node('window')
    test_utils.compare_result()
    run_color_depths()

    test_utils.open_project('synergy_16bpp.gxp')
    test_utils.configure_project()
    test_utils.configure_cpu_type('generic')
    test_utils.save_project_configure()
    test_utils.select_project_tree_node('window')
    test_utils.compare_result()
    run_color_depths()
    
    test_utils.open_project('synergy_32bpp.gxp')
    test_utils.configure_project()
    test_utils.configure_cpu_type('synergy')
    test_utils.save_project_configure()
    test_utils.select_project_tree_node('window')
    test_utils.compare_result()
    run_color_depths()
    
    test_utils.open_project('synergy_32bpp.gxp')
    test_utils.configure_project()
    test_utils.configure_cpu_type('generic')
    test_utils.save_project_configure()
    test_utils.select_project_tree_node('window')
    test_utils.compare_result()
    run_color_depths()
    
    test_utils.write_end("Test Color Formats")

