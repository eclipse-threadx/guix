import os
import logging
import filecmp

def compare(file_name_1, file_name_2):
    #write logging information
    logging.debug('Comparing: %s and %s', os.path.basename(file_name_1), os.path.basename(file_name_2))

    #test if file exist
    if os.path.exists(file_name_1) is False:
        logging.error("** ERROR ** %s does not exist", file_name_1)
        return False
        
    if os.path.exists(file_name_2) is False:
        logging.error("** ERROR ** %s does not exist", file_name_2)
        return False
    
    match_success = True
    
    if file_name_1.endswith('.bin'):
        match_success = filecmp.cmp(file_name_1, file_name_2, shallow=False)
        if match_success == False:
            logging.error("** ERROR ** Not match.")
    else:
        #find the line where "#include" is
        start_row_1 = 0
        file_1 = open(file_name_1,'r')
        lines_1 = file_1.readlines()
        for line in lines_1:
            if "#include" in line:
                start_row_1 = lines_1.index(line) + 1
                if "#include" in lines_1[start_row_1] is False:
                    break

        start_row_2 = 0
        file_2 = open(file_name_2,'r')
        lines_2 = file_2.readlines()
        for line in lines_2:
            if "#include" in line:
                start_row_2 = lines_2.index(line) + 1
                if "#include" in lines_2[start_row_2] is False:
                    break
        
        #compare from "#include"
        lines_1 = lines_1[start_row_1:]
        lines_2 = lines_2[start_row_2:]
        diff_count = 0
        
        for line_1, line_2 in zip(lines_1, lines_2):
            if line_1 != line_2:
                print('Does not match:\n=%s\n%s' %(line_1, line_2))
                logging.error('** ERROR ** Line #%d of %s does not match line #%d of %s\n%s\n%s' %(start_row_1, file_name_1, start_row_2, file_name_2, line_1, line_2))
                match_success = False

                diff_count += 1 

                if diff_count >= 10:
                    return False
                
            start_row_1 += 1
            start_row_2 += 1

    if match_success:
        logging.debug("Match success.")
        return True
    else:
        return False
 
def compare_output_files(project_path, golden_file_list):   
    cmp_result = True
    found = False

    if project_path.endswith('example_internal\\folder_output_test\\'):
        found = True
        if compare(project_path + "main_display_resources.c", project_path + "folder_output_test_MAIN_DISPLAY_resources.c") == False:
            logging.debug("Not Match.")
            cmp_result = False
            
        if compare(project_path + "main_display_resources.h", project_path + "folder_output_test_MAIN_DISPLAY_resources.h") == False:
            logging.debug("Not Match.")
            cmp_result = False
        
        #compare specified folder output 
        if compare(project_path + "secondary_resources.c", project_path + "folder_output_test_SECONDARY_resources.c") == False:
            logging.debug("Not Match.")
            cmp_result = False

        if compare(project_path + "secondary_resources.h", project_path + "folder_output_test_SECONDARY_resources.h") == False:
            logging.debug("Not Match.")
            cmp_result = False
            
        if compare(project_path + "specifications.h", project_path + "folder_output_test_specifications.h") == False:
            logging.debug("Not Match.")
            cmp_result = False
            
        #if compare(project_path + "specifications.c", project_path + "folder_output_test_specifications.c") == False:
        #    logging.debug("Not Match.")
        #    cmp_result = False

        if compare(project_path + "button_screen.c", project_path + "button_screen_specifications.c") == False:
            logging.debug("Not Match.")
            cmp_result = False
            
        if compare(project_path + "empty_folder.c", project_path + "empty_folder_specifications.c") == False:
            logging.debug("Not Match.")
            cmp_result = False
            
        if compare(project_path + "indicator_screen_runtime_allocate.c", project_path + "indicator_screen_runtime_allocate_specifications.c") == False:
            logging.debug("Not Match.")
            cmp_result = False
            
        if compare(project_path + "popup_modal.c", project_path + "popup_modal_specifications.c") == False:
            logging.debug("Not Match.")
            cmp_result = False

        if compare(project_path + "static_screens.c", project_path + "static_screens_specifications.c") == False:
            logging.debug("Not Match.")
            cmp_result = False

    else:
        for file in golden_file_list:
            test_file = ""

            if file.endswith('MAIN_DISPLAY_resources.bin'):
                test_file = 'MAIN_DISPLAY_resources.bin'
                
            elif file.endswith('SECONDARY_resources.bin'):
                test_file = 'SECONDARY_resources.bin'

            elif file.endswith('MAIN_DISPLAY_resources.c'):
                test_file = 'MAIN_DISPLAY_resources.c'
                
            elif file.endswith('SECONDARY_resources.c'):
                test_file = 'SECONDARY_resources.c'
                
            elif file.endswith('resources.c'):
                test_file = 'resources.c'
                
            elif file.endswith('MAIN_DISPLAY_resources.h'):
                test_file = 'MAIN_DISPLAY_resources.h'
                
            elif file.endswith('SECONDARY_resources.h'):
                test_file = 'SECONDARY_resources.h'
                
            elif file.endswith('resources.h'):
                test_file ='resources.h'

            elif file.endswith('_specifications.c'):
                test_file = 'specifications.c'

            elif file.endswith('_specifications.h'):
                test_file = 'specifications.h'

            elif file.endswith("resources.bin"):
                test_file = "resources.bin"
                
            elif file.endswith("Japanese.bin"):
                test_file = "japanese.bin"
                
            elif file.endswith("Japanese_big_endian.bin"):
                test_file = "japanese_big_endian.bin"
                
            elif file.endswith("resources_big_endian.bin"):
                test_file = "resources_big_endian.bin"

            if test_file:
                found = True
                if compare(project_path + test_file, project_path + file) == False:
                    cmp_result = False

    if found == False:
        logging.debug("No file been compared")
        return False
        
    return cmp_result

def compare_output_folders(project_path, test_output_path):  
    cmp_result = True
    file_list = os.listdir(test_output_path)
    logging.debug("comparing output folder: %s", test_output_path)
    for file in file_list:
        if compare(project_path + file, test_output_path + file) == False:
            cmp_result = False

    return cmp_result