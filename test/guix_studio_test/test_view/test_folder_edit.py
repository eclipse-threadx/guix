import os
import sys
import time
import test_utils
import test_constants
import logging

test_folder_edit_output_files = [
"test_folder_edit_folder_3.c",
"test_folder_edit_specifications.c",
"test_folder_edit_specifications.h"]

display_name = "display_1"

def get_test_header():
    notes =  "*         Studio Folder Edit Tests                    *\n"
    notes += "*                                                     *\n"
    notes += "* Test folder output                                  *\n"
    return notes

def run_folder_output_test():
    #insert folder "folder_1", check on ouputfile without set output file name
    test_utils.select_project_tree_node(display_name)
    test_utils.insert_folder()
    test_utils.edit_folder_properties()
    test_utils.set_folder_name("folder_1")
    test_utils.check_specify_output_file(1)
    test_utils.save_folder_name_edit()
    
    #insert widget to folder "folder_1"
    test_utils.select_project_tree_node("folder_1")
    test_utils.add_widget('GX_TYPE_WINDOW')
    
    #inder folder "folder_2", check off outpfile
    test_utils.select_project_tree_node(display_name)
    test_utils.insert_folder()
    test_utils.edit_folder_properties()
    test_utils.set_folder_name("folder_2")
    test_utils.check_specify_output_file(1)
    test_utils.set_folder_output_file_name("test_folder_edit_folder_3")
    test_utils.check_specify_output_file(0)
    test_utils.save_folder_name_edit()
    
    #insert widget to folder "folder_2"
    test_utils.select_project_tree_node("folder_2")
    test_utils.add_widget('GX_TYPE_WINDOW')
    
    #inser folder "folder_3", check on output file, set output file name
    test_utils.select_project_tree_node(display_name)
    test_utils.insert_folder()
    test_utils.edit_folder_properties()
    test_utils.set_folder_name("folder_3")
    test_utils.check_specify_output_file(1)
    test_utils.set_folder_output_file_name("test_folder_edit_folder_3")
    test_utils.save_folder_name_edit()
    
    #insert widget to folder "folder_3"
    test_utils.select_project_tree_node("folder_3")
    test_utils.add_widget('GX_TYPE_WINDOW')
    
    test_utils.toolbar_save()
    
    test_utils.generate_all()
    test_utils.click_generate_button()
    test_utils.cmp_output_files(test_folder_edit_output_files)

def run_folder_edit_tests(generate, screenshot):

    test_utils.output_test_header(get_test_header()) 
    test_utils.setup(generate, screenshot, 'test_folder_edit_golden_file')
    
    # open test project
    test_utils.new_project_create("test_folder_edit", display_name)

    print("run folder output test")
    run_folder_output_test()
    
    test_utils.write_end("Test Folder Edit")
    

