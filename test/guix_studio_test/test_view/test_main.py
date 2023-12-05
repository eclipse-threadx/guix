import os
import sys
import subprocess
import argparse
import test_utils
import logging
import time
import atexit
from test_color_formats import run_color_format_tests
from test_resources_edit import run_resources_edit_tests
from test_add_widget import run_add_widget_tests
from test_toolbar import run_toolbar_tests
from test_copy_paste import run_copy_paste_tests
from test_copy_paste_between_projects import run_copy_paste_between_projects_tests
from test_copy_paste_color import run_copy_paste_color_tests
from test_copy_paste_multi import run_copy_paste_multi_tests
from test_font import run_font_tests
from test_multi_themes import run_multi_themes_tests
from test_scroll_wheel_widget import run_scroll_wheel_widget_tests
from test_menu_widget import run_menu_widget_tests
from test_project_import import run_project_import_tests
from test_trigger_edit import run_trigger_edit_tests
from test_bidi_text import run_bidi_text_tests
from test_undo import run_undo_tests
from test_lock_unlock_widget_pos import run_lock_unlock_widget_pos_tests
from test_widget_name import run_widget_name_tests
from test_app_run import run_app_tests
from test_text_input_text_set import run_text_input_text_set_tests
from test_pixelmap_edit import run_pixelmap_edit_tests
from test_radial_slider_widget import run_radial_slider_widget_tests
from test_project_display_configure import run_project_display_configure_tests
from test_string_import_export import run_string_import_export_tests
from test_template import run_template_tests
from test_synergy_setting import run_synergy_setting_tests
from test_trigger_target_rename import run_trigger_target_rename_tests
from test_theme_edit import run_theme_edit_tests
from test_language_edit import run_language_edit_tests
from test_guix_lib_version_set import run_guix_lib_version_set_tests
from test_rich_text_view import run_rich_text_view_tests
from test_sprite import run_sprite_tests
from test_gauge import run_gauge_tests
from test_ml_view import run_ml_view_tests
from test_generic_scroll_wheel import run_generic_scroll_wheel_tests
from test_unique_bidi_text_reference_check import run_unique_bidi_text_reference_check_tests
from test_range_check import run_range_check_tests
from test_list_resize import run_list_resize_tests
from test_kerning_flag import run_kerning_flag_tests
from test_resource_xml import run_resource_xml_tests
try:
    from junit_xml import TestSuite
except ImportError:
    print("junit_xml module not found. Please install it by running 'pip install junit_xml' if you want to generate JUnit XML report.")

def exit_handler():
    test_utils.close_project()
    test_utils.close_window("main_frame")
    test_utils.delete_temp_files()

atexit.register(exit_handler)

def output_file_header():
    logging.info("*****************************************************")
    logging.info("*       Studio application usage/view test          *")
    logging.info("*                                                   *")
    logging.info("*                                                   *")
    logging.info("*****************************************************")
    logging.info("  Test date: %s", time.asctime())
    logging.info("  Test directory: %s", os.getcwd())
            
def __main__():

    Format = '%(message)s'
    logging.basicConfig(stream=test_utils.test_log_stream, level=logging.DEBUG, format=Format)
   
    msbuild_exe_path = "C:/Windows/Microsoft.NET/Framework64/v4.0.30319/MSBuild.exe"
    project_sln_path = "../../../guix_studio/build/vs_2019/studiox.sln"
    
    parser = argparse.ArgumentParser(description = "Test Studio UI")
    parser.add_argument('-b', action='store_true', dest='build_studio', help='Build Studio')
    parser.add_argument('-c', action='store_true', dest='run_color_format_tests', help = 'Run color format tests')
    parser.add_argument('--resources_edit', action='store_true', dest='run_resources_edit_tests', help = 'Run resources edit tests')
    parser.add_argument('-w', action='store_true', dest='run_widget_tests', help = 'Run Widget tests')
    parser.add_argument('--menu_widget', action='store_true', dest='run_menu_widget_tests', help = 'Run menu widget tests')
    parser.add_argument('--project_import', action='store_true', dest='run_project_import_tests', help = 'Run project import tests')
    parser.add_argument('--scroll_wheel_widget', action='store_true', dest='run_scroll_wheel_widget_tests', help = 'Run scroll wheel widget tests')
    parser.add_argument('-t', action='store_true', dest='run_toolbar_tests', help = 'Run toolbar tests')
    parser.add_argument('-g', action='store_true', dest='generate', help = 'Generate golden files')
    parser.add_argument('-s', action='store_true', dest="screenshot", help = "Generate canvas screen shot")
    parser.add_argument('-p', action='store_true', dest='run_copy_paste_tests', help = 'Run copy/paste tests')
    parser.add_argument('--copy_paste_between_projects', action='store_true', dest='run_copy_paste_between_projects_tests', help = 'Run copy/paste between projects tests')
    parser.add_argument('--copy_paste_color', action='store_true', dest='run_copy_paste_color_tests', help = 'Run copy/paste color tests')
    parser.add_argument('--copy_paste_multi', action='store_true', dest='run_copy_paste_multi_tests', help = 'Run copy/paste multi tests')
    parser.add_argument('-f', action='store_true', dest='run_font_tests', help = 'Run font tests')
    parser.add_argument('-m', action='store_true', dest='run_multi_themes_tests', help = 'Run Multi-Themes Tests')
    parser.add_argument('--trigger_edit', action='store_true', dest='run_trigger_edit_tests', help = 'Run trigger edit tests')
    parser.add_argument('--trigger_target_rename', action='store_true', dest='run_trigger_target_rename_tests', help = 'Run trigger target rename tests')
    parser.add_argument('--bidi_text', action='store_true', dest='run_bidi_text_tests', help = 'Run bidi text')
    parser.add_argument('-u', action='store_true', dest='run_undo_tests', help = 'Run undo tests')
    parser.add_argument('--lock_unlock_widget_pos', action='store_true', dest='run_lock_unlock_widget_pos_tests', help='Run lock/unlock widget pos tests')
    parser.add_argument('--widget_name', action='store_true', dest='run_widget_name_tests', help = 'Run widget name tests')
    parser.add_argument('--run_app', action='store_true', dest='run_app_tests', help='Run app tests')
    parser.add_argument('--input_text_set', action='store_true', dest='run_text_input_text_set_tests', help = 'Run text input text set tests')
    parser.add_argument('--pixelmap_edit', action='store_true', dest='run_pixelmap_edit_tests', help='Run pixelmap edit tests')
    parser.add_argument('--radial_slider_widget', action='store_true', dest='run_radial_slider_widget_tests', help='Run radial slider widget tests')
    parser.add_argument('--display_configure', action='store_true', dest='run_project_display_configure_tests', help='Run project display configure tests')
    parser.add_argument('--string_import_export', action='store_true', dest='run_string_import_export_tests', help='Run string import export tests')
    parser.add_argument('--template', action='store_true', dest='run_template_tests', help='Run template tests')
    parser.add_argument('--synergy_setting', action='store_true', dest='run_synergy_setting_tests', help='Run synergy setting tests')
    parser.add_argument('--theme_edit', action='store_true', dest='run_theme_edit_tests', help='Run theme edit tests')
    parser.add_argument('--language_edit', action='store_true', dest='run_language_edit_tests', help='Run language edit tests')
    parser.add_argument('--guix_lib_version', action='store_true', dest='run_guix_lib_version_set_tests', help='Run Guix Lib Version Set Tests')
    parser.add_argument('--rich_text_view', action='store_true', dest='run_rich_text_view_tests', help='Run Rich Text View Tests')
    parser.add_argument('--sprite', action='store_true', dest='run_sprite_tests', help='Run Sprite Tests')
    parser.add_argument('--gauge', action='store_true', dest='run_gauge_tests', help='Run Gauge Tests')
    parser.add_argument('--ml_view', action='store_true', dest='run_ml_view_tests', help='Run Multi Line Text View Tests')
    parser.add_argument('--generic_scroll_wheel', action='store_true', dest='run_generic_scroll_wheel_tests', help='Run Generic Scroll Wheel Tests')
    parser.add_argument('--unique_bidi_text_reference', action='store_true', dest='run_unique_bidi_text_reference_check_tests', help='Run Unique Bidi-Text Reference Check Tests')
    parser.add_argument('--range_check', action='store_true', dest='run_range_check_tests', help='Run Range Check Tests')
    parser.add_argument('--list_resize', action='store_true', dest='run_list_resize_tests', help='Run List Resize Tests')
    parser.add_argument('--kerning_flag', action='store_true', dest='run_kerning_flag_tests', help='Run Kerning Flag Tests')
    parser.add_argument('--resource_xml', action='store_true', dest='run_resource_xml_tests', help='Run Resource XML Tests')
    parser.add_argument('--output_junit', action='store_true', dest='output_junit', help='Output JUnit XML file')
    args = parser.parse_args()
        
    # if the user doesn't specify tests to run, then run them all:
    if (args.run_color_format_tests is False and
        args.run_resources_edit_tests is False and
        args.run_widget_tests is False and
        args.run_scroll_wheel_widget_tests is False and
        args.run_menu_widget_tests is False and
        args.run_toolbar_tests is False and
        args.run_copy_paste_tests is False and
        args.run_copy_paste_between_projects_tests is False and
        args.run_font_tests is False and
        args.run_multi_themes_tests is False and
        args.run_project_import_tests is False and
        args.run_copy_paste_color_tests is False and
        args.run_copy_paste_multi_tests is False and
        args.run_trigger_edit_tests is False and
        args.run_trigger_target_rename_tests is False and
        args.run_bidi_text_tests is False and
        args.run_undo_tests is False and
        args.run_lock_unlock_widget_pos_tests is False and
        args.run_widget_name_tests is False and
        args.run_app_tests is False and
        args.run_widget_name_tests is False and
        args.run_text_input_text_set_tests is False and
        args.run_pixelmap_edit_tests is False and
        args.run_radial_slider_widget_tests is False and
        args.run_project_display_configure_tests is False and
        args.run_string_import_export_tests is False and
        args.run_template_tests is False and
        args.run_synergy_setting_tests is False and
        args.run_theme_edit_tests is False and
        args.run_language_edit_tests is False and
        args.run_guix_lib_version_set_tests is False and
        args.run_rich_text_view_tests is False and
        args.run_sprite_tests is False and
        args.run_gauge_tests is False and
        args.run_ml_view_tests is False and
        args.run_generic_scroll_wheel_tests is False and
        args.run_unique_bidi_text_reference_check_tests is False and
        args.run_range_check_tests is False and
        args.run_list_resize_tests is False and
        args.run_kerning_flag_tests is False and
        args.run_resource_xml_tests is False):
        args.run_color_format_tests = True
        args.run_resources_edit_tests = True
        args.run_widget_tests = True
        args.run_scroll_wheel_widget_tests = True
        args.run_menu_widget_tests = True
        args.run_toolbar_tests = True
        args.run_copy_paste_tests = True
        args.run_copy_paste_between_projects_tests = True
        args.run_font_tests = True
        args.run_multi_themes_tests = True
        args.run_project_import_tests = True
        args.run_copy_paste_color_tests = True
        args.run_copy_paste_multi_tests = True
        args.run_trigger_edit_tests = True
        args.run_trigger_target_rename_tests = True
        args.run_bidi_text_tests = True
        args.run_undo_tests = True
        args.run_lock_unlock_widget_pos_tests = True
        args.run_widget_name_tests = True
        args.run_app_tests = True
        args.run_text_input_text_set_tests = True
        args.run_pixelmap_edit_tests = True
        args.run_radial_slider_widget_tests= True
        args.run_project_display_configure_tests = True
        args.run_string_import_export_tests = True
        args.run_template_tests = True
        args.run_synergy_setting_tests = True
        args.run_theme_edit_tests = True
        args.run_language_edit_tests = True
        args.run_guix_lib_version_set_tests = True
        args.run_rich_text_view_tests = True
        args.run_sprite_tests = True
        args.run_gauge_tests = True
        args.run_ml_view_tests = True
        args.run_generic_scroll_wheel_tests = True
        args.run_unique_bidi_text_reference_check_tests = True
        args.run_range_check_tests = True
        args.run_list_resize_tests = True
        args.run_kerning_flag_tests = True
        args.run_resource_xml_tests = True
        
    output_file_header()
    
    if args.build_studio:        
        test_utils.studio_compile(msbuild_exe_path, project_sln_path)
    
    time_start = time.time()
    if not test_utils.find_studio_handles():
        test_utils.run_studio()
        while not test_utils.find_studio_handles():
            interval = time.time() - time_start
            if interval > 300:
                raise Exception("Unable to start and find Studio application")
                break                
          
    test_utils.read_constants()

    if args.output_junit is True:
        test_utils.test_log_xml = open("studio_view_test_log.xml", "w")

    test_utils.test_log_txt = open("studio_view_test_log.txt", "w")
    test_utils.test_log_txt.write(test_utils.test_log_stream.getvalue())
    
    #test_utils.clean_up()
    #run tests

    if args.run_app_tests is True:
        run_app_tests(args.generate, args.screenshot)
        
    if args.run_color_format_tests is True:
        run_color_format_tests(args.generate, args.screenshot)

    if args.run_resources_edit_tests is True:
        run_resources_edit_tests(args.generate, args.screenshot)
        
    if args.run_widget_tests is True:
        run_add_widget_tests(args.generate, args.screenshot)
        
    if args.run_scroll_wheel_widget_tests is True:
        run_scroll_wheel_widget_tests(args.generate, args.screenshot)
        
    if args.run_menu_widget_tests is True:
        run_menu_widget_tests(args.generate, args.screenshot)
        
    if args.run_toolbar_tests is True:
        run_toolbar_tests(args.generate, args.screenshot)
        
    if args.run_copy_paste_tests is True:
        run_copy_paste_tests(args.generate, args.screenshot)
        
    if args.run_copy_paste_between_projects_tests is True:
        run_copy_paste_between_projects_tests(args.generate, args.screenshot)
        
    if args.run_copy_paste_color_tests is True:
        run_copy_paste_color_tests(args.generate, args.screenshot)
    
    if args.run_copy_paste_multi_tests is True:
        run_copy_paste_multi_tests(args.generate, args.screenshot)
        
    if args.run_font_tests is True:
        run_font_tests(args.generate, args.screenshot)
        
    if args.run_multi_themes_tests is True:
        run_multi_themes_tests(args.generate, args.screenshot)
    
    if args.run_project_import_tests is True:
        run_project_import_tests(args.generate, args.screenshot)
    
    if args.run_trigger_edit_tests is True:
        run_trigger_edit_tests(args.generate, args.screenshot)
    
    if args.run_trigger_target_rename_tests is True:
        run_trigger_target_rename_tests(args.generate, args.screenshot)
        
    if args.run_bidi_text_tests is True:
        run_bidi_text_tests(args.generate, args.screenshot)
        
    if args.run_undo_tests is True:
        run_undo_tests(args.generate, args.screenshot)
    
    if args.run_lock_unlock_widget_pos_tests is True:
        run_lock_unlock_widget_pos_tests(args.generate, args.screenshot)

    if args.run_widget_name_tests is True:
        run_widget_name_tests(args.generate, args.screenshot)

    if args.run_text_input_text_set_tests is True:
        run_text_input_text_set_tests(args.generate, args.screenshot)
        
    if args.run_pixelmap_edit_tests is True:
        run_pixelmap_edit_tests(args.generate, args.screenshot)
        
    if args.run_radial_slider_widget_tests is True:
        run_radial_slider_widget_tests(args.generate, args.screenshot)
        
    if args.run_project_display_configure_tests is True:
        run_project_display_configure_tests(args.generate, args.screenshot)
        
    if args.run_string_import_export_tests is True:
        run_string_import_export_tests(args.generate, args.screenshot)
    
    if args.run_template_tests is True:
        run_template_tests(args.generate, args.screenshot)
    
    if args.run_synergy_setting_tests is True:
        run_synergy_setting_tests(args.generate, args.screenshot)
        
    if args.run_theme_edit_tests is True:
        run_theme_edit_tests(args.generate, args.screenshot)

    if args.run_language_edit_tests is True:
        run_language_edit_tests(args.generate, args.screenshot)
        
    if args.run_guix_lib_version_set_tests is True:
        run_guix_lib_version_set_tests(args.generate, args.screenshot)
        
    if args.run_rich_text_view_tests is True:
        run_rich_text_view_tests(args.generate, args.screenshot)
        
    if args.run_sprite_tests is True:
        run_sprite_tests(args.generate, args.screenshot)
        
    if args.run_gauge_tests is True:
        run_gauge_tests(args.generate, args.screenshot)
        
    if args.run_ml_view_tests is True:
        run_ml_view_tests(args.generate, args.screenshot)
        
    if args.run_generic_scroll_wheel_tests is True:
        run_generic_scroll_wheel_tests(args.generate, args.screenshot)
    
    if args.run_unique_bidi_text_reference_check_tests is True:
        run_unique_bidi_text_reference_check_tests(args.generate, args.screenshot)
        
    if args.run_range_check_tests is True:
        run_range_check_tests(args.generate, args.screenshot)
        
    if args.run_list_resize_tests is True:
        run_list_resize_tests(args.generate, args.screenshot)
    
    if args.run_kerning_flag_tests is True:
        run_kerning_flag_tests(args.generate, args.screenshot)

    if args.run_resource_xml_tests is True:
        run_resource_xml_tests(args.generate, args.screenshot)

    if args.output_junit is True:
        ts = TestSuite("my test suite", test_utils.test_cases)
        test_utils.test_log_xml.write(TestSuite.to_xml_string([ts]))
        test_utils.test_log_xml.close()
    
    test_utils.test_log_txt.close()

    if test_utils.get_total_failures() != 0:
        exit(-1)

    exit(0)
    
    

__main__()        

