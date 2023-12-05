# -*- coding: utf-8 -*-
import os
import sys
import time
import test_utils
import test_constants

display_name = 'main_display'

test_string_import_export_output_files = [
"test_string_import_export_resources.c",
"test_string_import_export_resources.h"]

test_csv_export_files = [
"EnglishtoJapaneseExport.csv",
"EnglishtoChineseExport.csv"
]

def get_test_header():
    notes =  "*             String Import Export Test               *\n"
    notes += "*                                                     *\n"
    notes += "* Import string from csv format file                  *\n"
    notes += "* Export string to csv format file                    *\n"
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
    
    # close message dialog when create new and delete default_folder
    test_utils.close_message_dialog()

def csv_string_import_export():
    test_utils.edit_string()
    test_utils.sort_string(test_constants.STRING_SORT_TYPE_ID)
    test_utils.add_string()
    test_utils.edit_top_string('test_string')
    test_utils.save_string_edit()
    
    # import string from csv file
    test_utils.edit_string()
    test_utils.import_string('EnglishtoChineseImport2.csv')
    test_utils.cancel_string_edit()
    
    # test_utils.edit_string()
    test_utils.edit_string()
    test_utils.select_string(1)
    test_utils.toggle_three_column_mode()
    
    test_utils.import_string('EnglishtoChineseImport1.csv')
    test_utils.toggle_three_column_mode()
    
    #ABOUT_GUIX
    test_utils.select_string(1)
    test_utils.import_string('EnglishtoChineseImport2.csv')
    
    #export string, target language not exist
    test_utils.export_string()
    test_utils.set_string_export_src_language('English')
    test_utils.set_string_export_target_language('Japanese')
    test_utils.select_string_export_format('csv')
    test_utils.set_string_export_filename('EnglishtoJapaneseExport.csv')
    test_utils.set_string_export_path(test_utils.DEFAULT_OUTPUT_FILE_PATH)
    test_utils.save_string_export()
    
    #export string, target language exist
    test_utils.export_string()
    test_utils.set_string_export_src_language('English')
    test_utils.set_string_export_target_language('Chinese')
    test_utils.select_string_export_format('csv')
    test_utils.set_string_export_filename('EnglishtoChineseExport.csv')
    test_utils.set_string_export_path(test_utils.DEFAULT_OUTPUT_FILE_PATH)
    test_utils.save_string_export()
    
    test_utils.save_string_edit()
    
    #import string from command line
    print("import string from command line")
    test_utils.toolbar_save()
    test_utils.close_project(1)
    studio_release = test_utils.DEFAULT_OUTPUT_FILE_PATH + "../../../../guix_studio/build/vs_2019/Release/guix_studio.exe"
    project_pathname = test_utils.DEFAULT_OUTPUT_FILE_PATH + "/test_string_import_export.gxp"
    studio_release = os.path.abspath(studio_release)
    project_pathname = os.path.abspath(project_pathname)
    print(studio_release)
    print(project_pathname)
    
    os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -r -i ../EnglishtoJapaneseImport.xliff")
    test_utils.open_project(project_pathname, 0)
    
    #save project
    #est_utils.toolbar_save()
    
    #test_utils.generate_all()
    #test_utils.click_generate_button()
    test_utils.cmp_output_files(test_string_import_export_output_files)
    test_utils.cmp_normal_files(test_csv_export_files)
   
def run_string_import_export_tests(generate, screenshot):
    test_utils.output_test_header(get_test_header())
    test_utils.setup(generate, screenshot, 'test_string_import_export_golden_file')
    
    print('create new project')
    run_new_project_create("test_string_import_export")
    
    print('import export string from csv file')
    csv_string_import_export()
    
    test_utils.write_end('String Import Export Tests')