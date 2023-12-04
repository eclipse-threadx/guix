# -*- coding: utf-8 -*-
import os
import sys
import argparse
import ctypes
import time
import test_utils
from   subprocess import Popen
import winreg
import logging
import test_constants
import filecmp
import shutil
from io import StringIO
try:
    from junit_xml import TestCase
except ImportError:
    print("junit_xml module not found. Please install it by running 'pip install junit_xml' if you want to generate JUnit XML report.")

user32 = ctypes.windll.user32

studio_exe_path = "../../../guix_studio/build/vs_2019/Release/guix_studio.exe"
top_windows = []
WM_CLOSE = 0x0010
reset_map_format = False
change_color_table = False
Debug = False

props_ctrl_ids = {}
widget_types = {}
color_formats = {}
font_formats = {}
group_ids = {}
folder_ids = {}
resource_item_types = {}
palette_types = {}
border_types = {}
trigger_types = {}
animation_param_ids = {}

#dictionary of studio window name : handle
studio_windows = {
    'main_frame':0,
    'Properties Win':0,
    'Project View':0,
    'Target Screen':0,
    'Target View':0,
    'Resource View':0,
    }

CREATE_NEW_PROJECT_DIALOG = 'Create New Project'
CONFIGURE_PROJECT_DIALOG = 'Configure Project'
SYNERGY_SETTINGS_DIALOG = 'Renesas Graphics Advanced Settings'
CONFIGURE_THEME_DIALOG = 'Configure Themes'
CONFIGURE_LANGUAGE_DIALOG = 'Configure Languages'
EDIT_FONT_DIALOG = 'Edit Font'
EDIT_COLOR_DIALOG = 'Edit Color'
EDIT_PIXELMAP_DIALOG = 'Edit Pixelmap(s)'
STRING_TABLE_EDITOR_DIALOG = 'String Table Editor'
STRING_EXPORT_CONTROL_DIALOG = 'XLIFF/CSV Export Control'
NOTIFICATION_DIALOG = 'Notification'
ERROR_DIALOG = 'Error'
PLEASE_CONFIRM_DIALOG = 'Please Confirm'
RECORD_MACRO_DIALOG = 'Record Macro'
PLAYBACK_MACRO_DIALOG = 'Playback Macro'
EDIT_FOLDER_NAME_DIALOG = 'Edit Folder Name'
GRID_SNAP_SETTING_DIALOG = 'Grid and Snap Settings'
RESOURCE_EXPORT_DIALOG = 'Select Export Resources'
STRING_SCROLL_WHEEL_EDIT_DIALOG = 'String Scroll Wheel Edit'
IMPORT_PROJECT_DIALOG = 'Import Project'
OPTION_DIALOG='Warning'
WAIT_DIALOG='Please Wait...'
EDIT_PALETTE_DIALOG='Edit Palette'
SCREEN_FLOW_EDIT_DIALOG='Edit Screen Flow'
TRIGGER_LIST_EDIT_DIALOG='Edit Trigger List'
TRIGGER_EDIT_DIALOG='trigger edit dialog'
TRIGGER_ACTION_EDIT_DIALOG='trigger action edit dialog'
TRIGGER_ACTION_SELECT_DIALOG='Select Action'
EASING_FUNCTION_SELECT_DIALOG='Select Easing Function'
RICH_TEXT_EDIT_DIALOG='Edit Rich Text'
SPRITE_FRAME_EDIT_DIALOG='Edit Sprite Frames'
SPRITE_FRAME_IMPORT_DIALOG='Import Sprite Frames'
APP_EXECUTION_WINDOW='GUIX'

GW_CHILD = 5
GW_HWNDNEXT = 2

active_dialog_stack = []

#dictionary of studio dialog name: handle    
studio_dialogs = {
    CREATE_NEW_PROJECT_DIALOG:0,
    CONFIGURE_PROJECT_DIALOG:0,
    SYNERGY_SETTINGS_DIALOG:0,
    CONFIGURE_THEME_DIALOG:0,
    CONFIGURE_LANGUAGE_DIALOG:0,
    EDIT_FONT_DIALOG:0,
    EDIT_COLOR_DIALOG:0,
    EDIT_PIXELMAP_DIALOG:0,
    STRING_TABLE_EDITOR_DIALOG:0,
    STRING_EXPORT_CONTROL_DIALOG:0,
    NOTIFICATION_DIALOG:0,
    ERROR_DIALOG:0,
    PLEASE_CONFIRM_DIALOG:0,
    RECORD_MACRO_DIALOG:0,
    PLAYBACK_MACRO_DIALOG:0,
    EDIT_FOLDER_NAME_DIALOG:0,
    GRID_SNAP_SETTING_DIALOG:0,
    RESOURCE_EXPORT_DIALOG:0,
    STRING_SCROLL_WHEEL_EDIT_DIALOG:0,
    IMPORT_PROJECT_DIALOG:0,
    OPTION_DIALOG:0,
    WAIT_DIALOG:0,
    EDIT_PALETTE_DIALOG:0,
    SCREEN_FLOW_EDIT_DIALOG:0,
    TRIGGER_LIST_EDIT_DIALOG:0,
    TRIGGER_EDIT_DIALOG:0,
    TRIGGER_ACTION_EDIT_DIALOG:0,
    TRIGGER_ACTION_SELECT_DIALOG:0,
    EASING_FUNCTION_SELECT_DIALOG:0,
    RICH_TEXT_EDIT_DIALOG:0,
    SPRITE_FRAME_EDIT_DIALOG:0,
    SPRITE_FRAME_IMPORT_DIALOG:0,
    APP_EXECUTION_WINDOW:0
}

STUDIO_TEST_MESG = 0x0402
studio_test_index = 0
DEFAULT_TEST_FAILURE_PATH = 'test_failures/'
DEFAULT_OUTPUT_FILE_PATH = "output_files/"
DEFAULT_GOLDEN_FILE_PATH = 'golden_files/'
gen_golden_file_pathname = ""
test_name = ""
gen_golden_file = False
gen_screenshot = False
good_results = []  
result_count = [0, 0]
total_failures = 0
CLOSE_WAIT = 0
CLOSE_NO_WAIT = 1
CLOSE_NOTIFICATION_MSG = True
CLOSE_ERROR_MSG = True
selected_screen_name = ""
test_start_time = 0
test_cases = []
test_log_stream = StringIO()
test_log_txt = None
test_log_xml = None

def get_total_failures():
    global total_failures
    return total_failures

#Compile studio
def studio_compile(msbuild_exe_path, project_sln_path):
    #Ensure msbuild exists
    if not os.path.isfile(msbuild_exe_path):
        raise Exception('Msbuild.exe not found. path=' + msbuild_exe_path)
        
    #Ensure solution file exists
    if not os.path.isfile(msbuild_exe_path):
        raise Exception('studiox.sln not found. path=' + project_sln_path)
  
    os.system("msbuild " + project_sln_path + " /p:Configuration=Release")
    
#generate resource files
def generate(gxp_project, studio_release):
    path = os.path.realpath(DEFAULT_OUTPUT_FILE_PATH)
    
    if os.path.exists(path) == False:
        os.makedirs(path)
        
    if not os.path.isfile(gxp_project):
        raise Exception('gxp project not found. path=' + studio_release)
        
    if not os.path.isfile(studio_release):
        raise Exception('guix_studio.exe not found. path=' + studio_release)
        
    absolute_path = os.path.abspath(gxp_project)
    os.system(os.path.abspath(studio_release) + " -p " + absolute_path + " -n")

#compare two resource files
def compare_file(pathname_1, pathname_2, encoding = "", compare_start_string = "", skip_line = ""):
    logging.info('Comparing: %s and %s', pathname_1, pathname_2)
    
    # check if files exist
    if not os.path.isfile(pathname_1):
        logging.error("** ERROR ** %s does not exist", pathname_1)
        return False
        
    if not os.path.isfile(pathname_2):
        logging.error("** ERROR ** %s does not exist", pathname_2)
        return False
        
    # open comparing files
    if encoding != "":
        file_1 = open(pathname_1,'r', encoding=encoding)
        file_2 = open(pathname_2, 'r', encoding=encoding)
    else:
        file_1 = open(pathname_1,'r')
        file_2 = open(pathname_2, 'r')

    # read files into lists
    list_1 = file_1.readlines()
    list_2 = file_2.readlines()
    
    start_row_1 = 0
    start_row_2 = 0
    
    if compare_start_string != "":
        # find the line where comparing starts 
        for line in list_1:
            if compare_start_string in line:
                start_row_1 = list_1.index(line)
                break;
                
        for line in list_2:
            if compare_start_string in line:
                start_row_2 = list_1.index(line)
                break;

    # compare from the start line
    list_1 = list_1[start_row_1:]
    list_2 = list_2[start_row_2:]
    
    for line_1,line_2 in zip(list_1,list_2):
        # If the skip line string is not empty, skip the line if it contains the skip_line string
        if skip_line != "" and skip_line in line_1:
            continue
        
        if line_1 != line_2:
            logging.error("** ERROR ** Does Not Match:\n%s\n%s" %(line_1, line_2))  
            return False
        
    logging.info("Match success.")
    return True

def compare_output_file(pathname_1, pathname_2):
    return compare_file(pathname_1, pathname_2, '', '#include')

def compare_xliff_file(pathname_1, pathname_2):
    return compare_file(pathname_1, pathname_2, 'utf-8', '', '<file')

def compare_xml_file(pathname_1, pathname_2):
    return compare_file(pathname_1, pathname_2, 'utf-8', '', '<studio_version>')

def compare_normal_file(pathname_1, pathname_2):
    return compare_file(pathname_1, pathname_2, 'utf-8')
   
def cmp_files(output_file_lists, compare_func, output_file_path = DEFAULT_OUTPUT_FILE_PATH, golden_file_path = DEFAULT_GOLDEN_FILE_PATH):
    global studio_test_index

    if gen_golden_file == True:
        golden_file = open(gen_golden_file_pathname, "a+")
        if studio_test_index == 0:
            golden_file.write("good_results = [\n")

        #save golden values
        golden_file.write("    0, #test %d, comparing files\n" %(studio_test_index))
            
    else:
        result = True
        for file in output_file_lists:
            golden_file = golden_file_path + file
            output_file = output_file_path + file
            if compare_func(output_file, golden_file) == False:              
                result = False
                
        if result == True:
            print('Test %d passed' %studio_test_index)
            logging.info("Test #%d, compare output files- Passed." %studio_test_index)
            result_count[0] += 1
        else:
            print('** Test %d failed **' %studio_test_index)
            logging.info("Test #%d, compare output files- Failed." %studio_test_index)  
            result_count[1] += 1
           
    studio_test_index += 1  
        
def cmp_output_files(output_file_lists, output_file_path = DEFAULT_OUTPUT_FILE_PATH, golden_file_path = DEFAULT_GOLDEN_FILE_PATH):
    cmp_files(output_file_lists, compare_output_file, output_file_path, golden_file_path)
    
def cmp_normal_files(normal_file_lists, output_file_path = DEFAULT_OUTPUT_FILE_PATH, golden_file_path = DEFAULT_GOLDEN_FILE_PATH):
    cmp_files(normal_file_lists, compare_normal_file, output_file_path, golden_file_path)

def cmp_xml_files(xml_file_lists, output_file_path = DEFAULT_OUTPUT_FILE_PATH, golden_file_path = DEFAULT_GOLDEN_FILE_PATH):
    cmp_files(xml_file_lists, compare_xml_file, output_file_path, golden_file_path)
        
# run the Studio executable
def run_studio():
    absolute_path = os.path.abspath(studio_exe_path)
    if not os.path.isfile(absolute_path):
        raise Exception("Unable to locate Studio executable.")
    Popen(os.path.abspath(absolute_path))
    time.sleep(5)

def wait_for_key(prompt = ''):
     if Debug:
        if prompt:
             print('%s' %prompt)
 
        input("\nPress Enter to continue")
 

def enum_callback(hwnd, lParam):
    if user32.IsWindowVisible(hwnd):
        length = user32.GetWindowTextLengthW(hwnd) + 1

        if length > 1:
            buffer = ctypes.create_unicode_buffer(length)
            user32.GetWindowTextW(hwnd, buffer, length)
            top_windows.append((hwnd, buffer.value))
    return True

# search for named child windows, save handles for those that we want to send
# test events

def get_window_tree(hwnd):
    # get the first child window
    child = user32.GetWindow(hwnd, GW_CHILD)

    while child:
        length = user32.GetWindowTextLengthW(child) + 1
        if length > 1:
            buffer = ctypes.create_unicode_buffer(length)
            user32.GetWindowTextW(child, buffer, length)
            
            if buffer.value in studio_windows:
                studio_windows[buffer.value] = child

        get_window_tree(child)

        # get the next child window
        child = user32.GetWindow(child, GW_HWNDNEXT)
            
#send a test event to the requested Studio window
def send_to_studio(handle, cmd, param):
    result = 0
    
    if type(param) == str:
        with open('c:\\temp\\guix_param0.txt', 'w', encoding="utf-8") as fparam:
            fparam.write(param)
            fparam.close()

        result = user32.SendMessageA(handle, STUDIO_TEST_MESG, cmd, None)
    else:
        result = user32.SendMessageA(handle, STUDIO_TEST_MESG, cmd, param)
    return result
    
def post_to_studio(handle, cmd, param):
    result = 0
    
    if type(param) == str:
        with open('c:\\temp\\guix_param1.txt', 'w', encoding="utf-8") as fparam:
            fparam.write(param)
            fparam.close()

        result = user32.PostMessageA(handle, STUDIO_TEST_MESG, cmd, None)
    else:
        result = user32.PostMessageA(handle, STUDIO_TEST_MESG, cmd, param)
    return result

def delete_temp_files():
    if (os.path.exists("c:\\temp\\guix_param1.txt")):
        os.remove("c:\\temp\\guix_param1.txt")

    if (os.path.exists("c:\\temp\\guix_param0.txt")):
        os.remove("c:\\temp\\guix_param0.txt")        

# locate the Studio windows and store handles
def find_studio_handles():
    a = ctypes.WINFUNCTYPE(ctypes.c_bool,
                       ctypes.POINTER(ctypes.c_int),
                       ctypes.POINTER(ctypes.c_int))(enum_callback)
    user32.EnumWindows(a, 0)

    found_studio = False

    for hwnd, name in top_windows:
        if 'GUIX Studio' in name:
            found_studio = True
            studio_windows['main_frame'] = ctypes.c_void_p.from_buffer(hwnd).value
            get_window_tree(hwnd)

    if found_studio:
        for name in studio_windows:
            if studio_windows[name] == 0:
                print('Unable to find Studio window named: %s' %name)
                return False
            else:
                print('Found Studio Window: %s' %name)
    return found_studio

# find dialog handle 
def find_dialog(name, owner = 0, class_name = '#32770'):
    if class_name == 'GUIX':
        owner = 0
    elif owner == 0:
        owner = studio_windows['main_frame']
        
    param = class_name + ';'
    param += name + ';'
    param += str(owner) 

    studio_dialogs[name] = send_to_main_frame('CMD_FIND_DIALOG', param)
    
    return studio_dialogs[name]
    
def close_dialog(name, owner = 0):
    retries = 0
    wait_dialog_open(name, owner)

    while retries < 3:
        user32.PostMessageA(studio_dialogs[name], WM_CLOSE, None, None)
        if wait_dialog_close(name, owner) is True:
            return
        retries += 1

    raise Exception('Unable to close %s dialog' %name)

def close_window(name, owner = 0):
    user32.PostMessageA(studio_windows[name], WM_CLOSE, None, None)
    
def wait_dialog_open(name, owner = 0, class_name = '#32770'):
    time_start = time.time()
    
    logging.debug("wait dialog open: %s", name)
    if owner == 0 and len(active_dialog_stack) > 0:
        logging.debug("dialog parent: %s", active_dialog_stack[-1])
        owner = studio_dialogs[active_dialog_stack[-1]]
        
    while find_dialog(name, owner, class_name) == 0:
        time.sleep(1)
        interval = time.time() - time_start
        if interval > 300:
            raise Exception('Unable to open %s dialog' %name)
            break
    active_dialog_stack.append(name)
    logging.debug("push active dialog: %s", name)
    logging.debug("active dialog list: %s", active_dialog_stack)
    time.sleep(1)
            
def wait_dialog_close(name, owner = 0, class_name = '#32770'):
    #wait popup dialog close
    time_start = time.time()
    
    if owner == 0 and len(active_dialog_stack) > 1:
        owner = studio_dialogs[active_dialog_stack[-2]]
        
    while find_dialog(name, owner, class_name) != studio_dialogs[name]:
        time.sleep(1)
        interval = time.time() - time_start
        if interval > 300:
            return False
    time.sleep(1)
    active_dialog_stack.pop()
    logging.debug("pop active dialog: %s", name)
    logging.debug("active dialog list: %s", active_dialog_stack)
    return True
            
# compare canvas crc32 value with known good value, or
# print value if it's a new index
def compare_result():
    global studio_test_index
    result = get_result()
        
    if result == 0:
        return True    
        
    while is_wait_dialog_running() == True:
        time.sleep(5)
        
    #generate screen shot
    if gen_screenshot == True:
        path = os.path.realpath(DEFAULT_OUTPUT_FILE_PATH + "/" + test_name)
        if os.path.exists(path) == False:
            os.makedirs(path)
        filename = path + "/test_" + str(studio_test_index) + ".bmp"
        generate_screenshot(filename)
    
    if gen_golden_file == True:
        golden_file = open(gen_golden_file_pathname, "a+")
        if studio_test_index == 0:
            golden_file.write("good_results = [\n")
            
        #save golden values
        golden_file.write("    %#x, #test %d\n" %(result, studio_test_index))

        print('Generate CRC32 for test %d' %studio_test_index)
        logging.info("test %d. Golden value: %#x.\n" %(studio_test_index, result)) #print golden values
        studio_test_index += 1
    else: 
        if len(good_results) > studio_test_index and good_results[studio_test_index] != 0:
            if good_results[studio_test_index] == result:            
                print('Test %d passed' %studio_test_index)
                logging.info("Test #%d, compute CRC32 on canvas memory- Passed.\n" %studio_test_index)
                result_count[0] += 1
                studio_test_index += 1
                return True
            else:
                #generate failure screen shots
                path = os.path.realpath(DEFAULT_TEST_FAILURE_PATH)
                if os.path.exists(path) == False:
                    os.makedirs(path)
                filename = path + "/" + test_name +"_" + str(studio_test_index) + ".bmp"
                generate_screenshot(filename)
                
                print('** Test %d failed **' %studio_test_index)
                logging.error("CRC32 failure on test %d. Golden value: %#x  Result Value: %#x\n" %(studio_test_index, good_results[studio_test_index], result))
                result_count[1] += 1
                studio_test_index += 1       
                
                #this will make the script wait here until the user inputs a CR key
                #input()
                return False
        else:
            logging.info('Result for index %d is %#x' %(studio_test_index, result))
            studio_test_index += 1
            return True
            

def set_test_results(passed, test_type, error_msg):
    global studio_test_index
        
    if gen_golden_file == True:
        golden_file = open(gen_golden_file_pathname, "a+")
        if studio_test_index == 0:
            golden_file.write("good_results = [\n")

        golden_file.write("    0, #test %d, %s\n" %(studio_test_index, test_type))
    else:
        if passed == True:
            print('Test %d passed' %studio_test_index)
            logging.info("Test #%d, %s- Passed.\n", studio_test_index, test_type)
            result_count[0] += 1
        else:
            print('** Test %d failed **' %studio_test_index)
            logging.info("Test #%d, %s- Failed.\n", studio_test_index, test_type) 
            logging.info(error_msg)            
            result_count[1] += 1
           
    studio_test_index += 1  
    
def output_test_header(header_notes):
    global test_start_time
    test_start_time = time.time()
    global test_log_stream
    test_log_stream.truncate(0)
    test_log_stream.seek(0)

    logging.info("*******************************************************")
    logging.info(header_notes)
    logging.info("*                                                     *")    
    logging.info("*******************************************************")
    logging.info("Test date: %s\n", time.asctime())

def clean_up():
    path = os.path.realpath(DEFAULT_TEST_FAILURE_PATH)
    print(path)
    if os.path.exists(path):
        shutil.rmtree(path)
        
    path = os.path.realpath(DEFAULT_OUTPUT_FILE_PATH)
    print(path)
    if os.path.exists(path):
        shutil.rmtree(path)
    
def setup(generate, screenshot, golden_file):
    test_utils.gen_golden_file = generate
    test_utils.gen_screenshot = screenshot
    
    global studio_test_index
    studio_test_index = 0;
    
    global test_name
    test_name = golden_file.replace("_golden_file", "")
    
    if os.path.exists(DEFAULT_OUTPUT_FILE_PATH) == False:
        os.makedirs(DEFAULT_OUTPUT_FILE_PATH)
                    
    if generate:
        global gen_golden_file_pathname

        if golden_file is not None:
            gen_golden_file_pathname = DEFAULT_OUTPUT_FILE_PATH + golden_file + ".py"
        
        #delete old golden file    
        if os.path.exists(gen_golden_file_pathname):
            os.remove(gen_golden_file_pathname)

        print("golden file path: %s" %gen_golden_file_pathname)
    else:
        module = __import__("golden_files.%s" %(golden_file), fromlist=["golden_files"])
        global good_results
        good_results = module.good_results
    global active_dialog_stack
    active_dialog_stack = []
    
def write_end(test_name):
    global good_results
    global total_failures
    global test_log_xml
    global test_cases

    if gen_golden_file == True:
        golden_file = open(gen_golden_file_pathname, "a+")
        golden_file.write("]")
    else:
        if len(good_results):
            failures = len(good_results) - result_count[0]
        else:
            failures = result_count[1]
            
        total_failures += failures
        logging.info("********************************************************************")
        logging.info("** Test %s completed." %test_name)
        logging.info('** Tests Passed: %d  Tests Failed %d' %(result_count[0], failures))
        logging.info("********************************************************************")
        logging.info("Total Faiures: %d\n", total_failures)
        logging.info("\n\n\n")
        
        print('\nTest %s completed. \nTests Passed: %d\nTests Failed: %d\n' %(test_name, result_count[0], failures))
        print('Total Failures: %d\n' %(total_failures))

        result_count[0] = 0
        result_count[1] = 0
        studio_test_index = 0
        good_results = []

    test_time = time.time() - test_start_time
    test_log_txt.write(test_log_stream.getvalue())

    if test_log_xml is not None:
        test = TestCase(test_name, "", test_time, test_log_stream.getvalue(), None)
        if failures:
            test.add_failure_info("Failed.")
        test_cases.append(test)
  
# read c++ type defines into a python dictionary
def read_defines(file_path, prefix):
    if os.path.exists(file_path) is False:
        print("** ERROR ** %s does not exist" %file_path)
        return False
        
    logging.info("read '%s' defines from %s", prefix, file_path)
    
    file = open(file_path, 'r')
    lines = file.readlines()
    
    # read defines into string
    defines = {}
    for line in lines:
        if '#define' in line and prefix in line:
            line = ' '.join(line.split())
            list = line.split()
            defines[list[1]] = int(list[2], 0)
                
    return defines
    
# read c++ type enum structure into a python dictionary  
def read_enum(file_path, variable_name):
    if os.path.exists(file_path) is False:
        print("** ERROR ** %s does not exist" %file_path)
        return False
        
    logging.info("read '%s' enum structure from %s", variable_name, file_path)
    
    file = open(file_path, 'r')
    lines = file.readlines()
    
    start_read = False
    
    # read enum structure into string
    enum_str = ''
    for line in lines:
        if 'enum' in line and variable_name in line:
            start_read = True
            
        if start_read:  
            enum_str += line
                
            index = line.find('}')
            if index >= 0:
                break

    # remove braces
    index = enum_str.find('{')
    if index >= 0:
        enum_str = enum_str[index + 1:]
        
    index = enum_str.find('}')
    if index >= 0:
        enum_str = enum_str[:index - 1]
      
    # remove line breaks
    enum_str = enum_str.replace('\n', '')
    enum_str = enum_str.replace('\r', '')

    # read enums into a dictionary of {"ID", value} pair
    enum = {}
    enum_index = 0
    index = enum_str.find(',')
    while len(enum_str) > 0:
    
        if index == -1:
            temp = enum_str
            enum_str =  ''
        else:
            temp = enum_str[0:index]
            enum_str = enum_str[index + 1:]
        
        index = temp.find('=')
        if index >= 0:
            if index == 0:
                temp = ''
            else:
                enum_index = int(temp[index + 1:])
                temp = temp[0:index - 1]
        
        temp = temp.replace(' ', '')
     
        if len(temp) > 0:
            enum[temp] = enum_index
            enum_index += 1
    
        index = enum_str.find(',')
        
    return enum

def read_constants():
    logging.info("*******************************************************")
    logging.info("*                Reading Constants                    *")
    logging.info("*                                                     *")
    logging.info("*******************************************************")
    print('Reading Constants')
    
    gx_api_h = '..//..//..//common//inc//gx_api.h'
    studioxproject_h = '..//..//..//guix_studio//StudioXProject.h'
    properties_win_cpp = '..//..//..//guix_studio//properties_win.cpp'
    trigger_edit_dlg_h = '..//..//..//guix_studio//trigger_edit_dlg.h'
    trigger_action_edit_dlg_h = '..//..//..//guix_studio//trigger_action_edit_dlg.h'

    # read property control ids
    global props_ctrl_ids
    props_ctrl_ids = read_enum(properties_win_cpp, 'CONTROL_IDS')

    # read widget types
    global widget_types
    widget_types = read_defines(gx_api_h, 'GX_TYPE')
    
    # read color formats
    global color_formats
    color_formats = read_defines(gx_api_h, 'GX_COLOR_FORMAT')
    
    # read font formats
    global font_formats
    font_formats = read_defines(gx_api_h, 'GX_FONT_FORMAT')
    
    global resource_item_types
    resource_item_types = read_enum(studioxproject_h, 'resource_item_types')
    
    global group_ids
    group_ids = read_enum(studioxproject_h, 'GROUP_IDS')
    
    global folder_ids
    folder_ids = read_enum(studioxproject_h, 'FolderIds')
    
    global palette_types
    palette_types = read_enum(studioxproject_h, 'PALETTE_TYPES')
    
    global trigger_types
    trigger_types = read_enum(trigger_edit_dlg_h, 'TRIGGER_TYPE')
    
    global animation_param_ids
    animation_param_ids = read_enum(trigger_action_edit_dlg_h, 'TRIGGER_ACTION_EDIT_CONTROL_IDS')
    
    logging.info("\n")
    
#================================================================#
#           Send Commands to Main Frame                          #
#================================================================#
main_frame_test_commands=[
    '',
    'CMD_ZOOM_IN',
    'CMD_TOOLBAR_CREATE',
    'CMD_TOOLBAR_OPEN',
    'CMD_TOOLBAR_SAVE',
    'CMD_TOOLBAR_CUT',
    'CMD_TOOLBAR_COPY',
    'CMD_TOOLBAR_PASTE',
    'CMD_TOOLBAR_ALIGN_LEFT',
    'CMD_TOOLBAR_ALIGN_RIGHT',
    'CMD_TOOLBAR_ALIGN_TOP',
    'CMD_TOOLBAR_ALIGN_BOTTOM',
    'CMD_TOOLBAR_VSPACE_EQUALLY',
    'CMD_TOOLBAR_HSPACE_EQUALLY',
    'CMD_TOOLBAR_EQUAL_WIDTH',
    'CMD_TOOLBAR_EQUAL_HEIGHT',
    'CMD_TOOLBAR_MOVE_TO_FRONT',
    'CMD_TOOLBAR_MOVE_TO_BACK',
    'CMD_TOOLBAR_SIZE_TO_FIT',
    'CMD_TOOLBAR_ZOOM_IN',
    'CMD_TOOLBAR_ZOOM_OUT',
    'CMD_TOOLBAR_RECORD_MACRO',
    'CMD_TOOLBAR_PLAYBACK_MACRO',
    'CMD_TOOLBAR_ABOUT',
    'CMD_UNDO',
    'CMD_CONFIGURE_LANGUAGES',
    'CMD_CONFIGURE_PROJECT',
    'CMD_CONFIGURE_THEMES',
    'CMD_CONFIGURE_SCREEN_FLOW',
    'CMD_GENERATE_ALL',
    'CMD_GENERATE_RESOURCES',
    'CMD_GET_MENU_STATUS',
    'CMD_GRID_SNAP_SETTING',
    'CMD_FIND_DIALOG',
    'CMD_LOCK_UNLOCK_WIDGET_POSITIONS',
    'CMD_RUN_APPLICATION',
    'CMD_IS_WAIT_DIALOG_RUNNING'
]
    
#send a test command to the main frame
def send_to_main_frame(cmd, param):
    handle = studio_windows['main_frame']
    command = main_frame_test_commands.index(cmd)
    return(send_to_studio(handle,command, param))
    
#post a test command to the main frame
def post_to_main_frame(cmd, param):
    handle = studio_windows['main_frame']
    command = main_frame_test_commands.index(cmd)
    return(post_to_studio(handle, command, param))
    
# command the main frame to click toolbar button
def toolbar_create():
    logging.info('create new project')
    post_to_main_frame('CMD_TOOLBAR_CREATE', 0)
    wait_dialog_open(CREATE_NEW_PROJECT_DIALOG)
    
def toolbar_save():
    logging.info('save project')
    send_to_main_frame('CMD_TOOLBAR_SAVE', 0)
    
def toolbar_cut():
    logging.info('click toolbar button: cut')
    send_to_main_frame('CMD_TOOLBAR_CUT', 0)
    compare_result()
    time.sleep(1)
 
def toolbar_copy():
    logging.info('click toolbar button: copy')
    send_to_main_frame('CMD_TOOLBAR_COPY', 0)
    compare_result()
    time.sleep(1)
    
def toolbar_paste(do_deselect = 1):
    logging.info('click toolbar button: paste')
    send_to_main_frame('CMD_TOOLBAR_PASTE', 0)
   
    if do_deselect:
        deselect()
        dirty_root_window()
        compare_result()
       
    
def toolbar_paste_no_wait():
    logging.info('click toolbar button: paste')
    post_to_main_frame('CMD_TOOLBAR_PASTE', 0)
    
def toolbar_align_left():
    logging.info('click toolbar button: align left')
    send_to_main_frame('CMD_TOOLBAR_ALIGN_LEFT', 0)
    compare_result()
    time.sleep(1)
    
def toolbar_align_right():
    logging.info('click toolbar button: align right')
    send_to_main_frame('CMD_TOOLBAR_ALIGN_RIGHT', 0)
    compare_result()
    time.sleep(1)
    
def toolbar_align_top():
    logging.info('click toolbar button: align top')
    send_to_main_frame('CMD_TOOLBAR_ALIGN_TOP', 0)
    compare_result()
    time.sleep(1)
    
def toolbar_align_bottom():
    logging.info('click toolbar button: align bottom')
    send_to_main_frame('CMD_TOOLBAR_ALIGN_BOTTOM', 0)
    compare_result()
    time.sleep(1)
    
def toolbar_vspace_equally():
    logging.info('click toolbar button: vertical space equally')
    send_to_main_frame('CMD_TOOLBAR_VSPACE_EQUALLY', 0)
    compare_result()
    time.sleep(1)
    
def toolbar_hspace_equally():
    logging.info('click toolbar button: horizontal space equally')
    send_to_main_frame('CMD_TOOLBAR_HSPACE_EQUALLY', 0)
    compare_result()
    time.sleep(1)
    
def toolbar_equal_width():
    logging.info('click toolbar button: equal width')
    send_to_main_frame('CMD_TOOLBAR_EQUAL_WIDTH', 0)
    compare_result()
    time.sleep(1)
    
def toolbar_equal_height():
    logging.info('click toolbar button: equal height')
    send_to_main_frame('CMD_TOOLBAR_EQUAL_HEIGHT', 0)
    compare_result()
    time.sleep(1)
    
def toolbar_move_to_front():
    logging.info('click toolbar button: move to front')
    send_to_main_frame('CMD_TOOLBAR_MOVE_TO_FRONT', 0)
    compare_result()
    time.sleep(1)
    
def toolbar_move_to_back():
    logging.info('click toolbar button: move to back')
    send_to_main_frame('CMD_TOOLBAR_MOVE_TO_BACK', 0)
    compare_result()
    time.sleep(1)
    
def toolbar_size_to_fit():
    logging.info('click toolbar button: size to fit')
    send_to_main_frame('CMD_TOOLBAR_SIZE_TO_FIT', 0)
    compare_result()
    time.sleep(1)
    
def toolbar_zoom_in():
    logging.info('click toolbar button: zoom in')
    send_to_main_frame('CMD_TOOLBAR_ZOOM_IN', 0)
    compare_result()
    time.sleep(1)
    
def toolbar_zoom_out():
    logging.info('click toolbar button: zoom out')
    send_to_main_frame('CMD_TOOLBAR_ZOOM_OUT', 0)
    compare_result()
    time.sleep(1)
    
def toolbar_record_macro():
    logging.info('click toolbar button: record macro')
    post_to_main_frame('CMD_TOOLBAR_RECORD_MACRO', 0)
    
def toolbar_playback_macro():
    logging.info('click toolbar button: playback macro')
    post_to_main_frame('CMD_TOOLBAR_PLAYBACK_MACRO', 0)
    
def toolbar_about():
    logging.info('click toolbar button: about')
    post_to_main_frame('CMD_TOOLBAR_ABOUT', 0)

def undo():
    logging.info('click edit menu: undo')
    send_to_main_frame('CMD_UNDO', 0)
    time.sleep(1)
    compare_result() 
    
# command to main frame to generate all output files    
def generate_all():
    logging.info("generate all output files.")
    post_to_main_frame('CMD_GENERATE_ALL', 0)
    wait_dialog_open(RESOURCE_EXPORT_DIALOG)
    
def generate_resources():
    logging.info("generate resources")
    post_to_main_frame('CMD_GENERATE_RESOURCES', 0)
    wait_dialog_open(RESOURCE_EXPORT_DIALOG)
 
def configure_project():
    logging.info('configure project')
    post_to_main_frame('CMD_CONFIGURE_PROJECT', 0)
    wait_dialog_open(CONFIGURE_PROJECT_DIALOG)
    
def configure_themes():
    logging.info('configure themes')
    post_to_main_frame('CMD_CONFIGURE_THEMES', 0)
    wait_dialog_open(CONFIGURE_THEME_DIALOG)
    
# command the main frame to popup language configure dialog
def configure_languages():
    logging.info('configure languages')
    post_to_main_frame('CMD_CONFIGURE_LANGUAGES', 0)   
    wait_dialog_open(CONFIGURE_LANGUAGE_DIALOG)

def configure_screen_flow():
    logging.info('configure screen flow')
    post_to_main_frame('CMD_CONFIGURE_SCREEN_FLOW', 0)
    wait_dialog_open(SCREEN_FLOW_EDIT_DIALOG)
    
def get_menu_status(string):
    return send_to_main_frame('CMD_GET_MENU_STATUS', string)  
    
def grid_snap_setting():
    logging.info('grid and snap setting')
    post_to_main_frame('CMD_GRID_SNAP_SETTING', 0)
    wait_dialog_open(GRID_SNAP_SETTING_DIALOG)
    
def lock_unlock_widget_positions(lock):
    if lock:
        logging.info('lock widget positions')
    else:
        logging.info('unlock widget positions')
    
    send_to_main_frame('CMD_LOCK_UNLOCK_WIDGET_POSITIONS', lock)

def run_application():
    logging.info('run application')
    send_to_main_frame('CMD_RUN_APPLICATION', 0)
    wait_dialog_open('GUIX', 0, 'GUIX')

def is_wait_dialog_running():
    logging.info('check if wait dialog is running')
    return send_to_main_frame('CMD_IS_WAIT_DIALOG_RUNNING', 0)
    
#================================================================#
#           Send Commands to Target Screen                       #
#================================================================#
target_screen_test_commands = [
'',
'CMD_GENERATE_CHECKSUM',
'CMD_GENERATE_SCREENSHOT',
'CMD_DESELECT',
'CMD_DIRTY_ROOT_WINDOW',
'CMD_MOVE_WINDOW',
'CMD_MOVE_SELECTED',
'CMD_ADD_WIDGET',
'CMD_ADD_TEMPLATE',
'CMD_ZOOM_IN',
'CMD_LBUTTON_DOWN',
'CMD_LBUTTON_UP',
'CMD_MOUSEMOVE',
'CMD_MOUSEMOVE_POST'
]

#send a test command to the target screen
def send_to_target_screen(cmd, param):
    handle = studio_windows['Target Screen']
    command = target_screen_test_commands.index(cmd)
    return(send_to_studio(handle, command, param))

def post_to_target_screen(cmd, param):
    handle = studio_windows['Target Screen']
    command = target_screen_test_commands.index(cmd)
    return(post_to_studio(handle, command, param))
    
#ask the target screen for current canvas crc32 value
def get_result():
    result= send_to_target_screen('CMD_GENERATE_CHECKSUM', None)
    return result
    
def generate_screenshot(filename):
    logging.info('generate screen shot, image file name = %s', filename)
    send_to_target_screen('CMD_GENERATE_SCREENSHOT', filename);
    
def deselect():
    logging.info('Deselect')
    send_to_target_screen('CMD_DESELECT', 0)

def dirty_root_window():
    logging.info('dirty root window')
    send_to_target_screen('CMD_DIRTY_ROOT_WINDOW', 0)
    
# command the target screen to shift the main window
def move_window(shift):
    logging.info('move top-level window to force redraw')
    send_to_target_screen('CMD_MOVE_WINDOW', shift)
    compare_result()    
    time.sleep(1)
    
# command the target screen to shift the selected widget
def move_selected(shift):
    logging.info('move the selected widget')
    send_to_target_screen('CMD_MOVE_SELECTED', shift)
    compare_result()
    time.sleep(1)
    
# command the target screen to add widget
def add_widget(typename):
    if typename in widget_types:
        logging.info('add a widget, type = %s' %typename)        
        send_to_target_screen('CMD_ADD_WIDGET', widget_types[typename])
    else:
        raise Exception("Invalid widget type: %s" %typename)

    time.sleep(1)
    
# command to target screen to add template
def add_template(base_name, display = 0):
    param = str(base_name) + ',' + str(display)
    send_to_target_screen('CMD_ADD_TEMPLATE', param)
    logging.info('add a widget using template %s' %base_name)
    time.sleep(1)
    
def zoom_in(scale):
    send_to_target_screen('CMD_ZOOM_IN', scale)
    logging.info('zoom in target view by %d percent' %scale)
    compare_result()
    
def left_button_down(x, y):
    param = str(x) + ',' + str(y)
    send_to_target_screen('CMD_LBUTTON_DOWN', param)
    logging.info('simulate left button down on (%d, %d)', x, y)
 
def left_button_up(x, y):
    param = str(x) + ',' + str(y)
    send_to_target_screen('CMD_LBUTTON_UP', param)
    logging.info('simulate left button up on (%d, %d)', x, y)
    
def mousemove(x, y, close_notification = False):
    param = str(x) + ',' + str(y)
    
    if close_notification == False:
        send_to_target_screen('CMD_MOUSEMOVE', param)
        logging.info('simulate mousemove on (%d, %d)', x, y)
    else:
        post_to_target_screen('CMD_MOUSEMOVE_POST', param)
        logging.info('simulate mousemove on (%d, %d)', x, y)
        close_message_dialog()
    
#================================================================#
#           Send Commands to Properties Screen                   #
#================================================================#
properties_win_test_commands = [
'',
'CMD_EDIT_WIDGET_PROPS',
'CMD_EDIT_WIDGET_PROPS_POST'
]

#send a test command to the properties view    
def send_to_properties_screen(cmd, param):
    handle = studio_windows['Properties Win']
    command = properties_win_test_commands.index(cmd)
    return(send_to_studio(handle, command, param))
    
def post_to_properties_screen(cmd, param):
    handle = studio_windows['Properties Win']
    command = properties_win_test_commands.index(cmd)
    return(post_to_studio(handle, command, param))
    
def edit_widget_props(id_str, prop_val):
    control_id = props_ctrl_ids[id_str];
    param = str(control_id) + "," + str(prop_val)
    prop_string = str(prop_val)
    logging.info("editing property: %s, control_id = %d, val = %s" %(id_str, control_id, prop_string.encode(sys.stdout.encoding, 'replace')))
    send_to_properties_screen('CMD_EDIT_WIDGET_PROPS', param)
    
def edit_widget_props_post(id_str, prop_val):
    control_id = props_ctrl_ids[id_str];
    param = str(control_id) + "," + str(prop_val)
    prop_string = str(prop_val)
    logging.info("editing property: %s, control_id = %d, val = %s" %(id_str, control_id, prop_string.encode(sys.stdout.encoding, 'replace')))
    post_to_properties_screen('CMD_EDIT_WIDGET_PROPS_POST', param)

#================================================================#
#           Send Commands to Project View                        #
#================================================================#
project_view_test_commands = [
'',
'CMD_OPEN_PROJECT',
'CMD_CLOSE_PROJECT',
'CMD_IMPORT_PROJECT',
'CMD_SELECT_PROJECT_TREE_NODE',
'CMD_SELECT_PROJECT_TREE_FOLDER',
'CMD_SELECT_CHILD_WIDGET',
'CMD_SELECT_MULTI_WIDGETS',
'CMD_DELETE_WIDGET',
'CMD_GET_WIDGET_LEFT',
'CMD_GET_WIDGET_TOP',
'CMD_GET_WIDGET_RIGHT',
'CMD_GET_WIDGET_BOTTOM',
'CMD_INSERT_FOLDER',
'CMD_EDIT_FOLDER_PROPERTIES',
'CMD_DELETE_FOLDER',
'CMD_TERMINATE_APP_EXECUTION'
]

#send a test command to the project view
def send_to_project_view(cmd, param):
    handle = studio_windows['Project View']
    command = project_view_test_commands.index(cmd)
    return(send_to_studio(handle, command, param))
    
def post_to_project_view(cmd, param):
    handle = studio_windows['Project View']
    command = project_view_test_commands.index(cmd)
    return(post_to_studio(handle, command, param))
    
# tell the Studio project view to open a project
def open_project(path, compare = 1):
    if not os.path.isfile(path):
        raise Exception('Test project is missing: %s' %path)
    abs_path = os.path.abspath(path)
    logging.info("open project: %s", abs_path)
    global default_map
    send_to_project_view('CMD_OPEN_PROJECT', abs_path)
    
    if compare == 1:
        compare_result()

def import_project(pathname):
    logging.info("import project:%s", pathname)
    post_to_project_view('CMD_IMPORT_PROJECT', pathname)
    wait_dialog_open(IMPORT_PROJECT_DIALOG)
    
# tell the Studio project view to close a project
def close_project(save_change = 0):
    logging.info('close current project')
    if save_change == 1:
        post_to_project_view('CMD_CLOSE_PROJECT', save_change)
    else:
        send_to_project_view('CMD_CLOSE_PROJECT', save_change)
  
# command the project view to select a widget
def select_project_tree_node(name):
    while is_wait_dialog_running() == True:
        time.sleep(5)
    
    logging.info('select "%s"' %name)
    result = send_to_project_view('CMD_SELECT_PROJECT_TREE_NODE', name)
    time.sleep(1)
    return result

# command the project view to select a folder
def select_project_tree_folder(display_name, folder_name):
    logging.info('select display, folder "%s", "%s"' %(display_name, folder_name))
    param = display_name + ',' + folder_name
    result = send_to_project_view('CMD_SELECT_PROJECT_TREE_FOLDER', param)

def select_project_tree_child():
    logging.info('select project tree child widget')
    result = send_to_project_view('CMD_SELECT_CHILD_WIDGET')
    
# command the project view to select multiple widgets
def select_multi_widgets(name):
    logging.info('select "%s" with Shift-Key down' %name)
    send_to_project_view('CMD_SELECT_MULTI_WIDGETS', name)
    time.sleep(1)
 
# command the project view to delete a widget    
def delete_widget(do_compare = 1):
    logging.info('delete current selected widget')
    send_to_project_view('CMD_DELETE_WIDGET', 0)
    
    if do_compare:
        compare_result()
        
    time.sleep(1)

def post_delete_widget():
    logging.info('post delete widget message')
    post_to_project_view('CMD_DELETE_WIDGET', 0)

def get_widget_left(widget_name):
    return send_to_project_view('CMD_GET_WIDGET_LEFT', widget_name)
    
def get_widget_top(widget_name):
    return send_to_project_view('CMD_GET_WIDGET_TOP', widget_name)
    
def get_widget_right(widget_name):
    return send_to_project_view('CMD_GET_WIDGET_RIGHT', widget_name)

def get_widget_bottom(widget_name):
    return send_to_project_view('CMD_GET_WIDGET_BOTTOM', widget_name)
    
def drag_left(widget_name, delta_x, close_notification = False):
    logging.info('drag left of widget "%s"', widget_name)
    left = get_widget_left(widget_name) - 1
    top = get_widget_top(widget_name) - 1
    bottom = get_widget_bottom(widget_name) + 1
    
    y = (top + bottom) / 2
    left_button_down(left, y)
    mousemove(left + delta_x, y, close_notification)
    left_button_up(left + delta_x, y)

def drag_right(widget_name, delta_x, close_notification = False):
    logging.info('drag right of widget "%s"', widget_name)
    right = get_widget_right(widget_name) + 1
    top = get_widget_top(widget_name) - 1
    bottom = get_widget_bottom(widget_name) + 1
    
    y = (top + bottom) / 2
    left_button_down(right, y)
    mousemove(right + delta_x, y, close_notification)
    left_button_up(right + delta_x, y)

def drag_top(widget_name, delta_y, close_notification = False):
    logging.info('drag top of widget "%s"', widget_name)
    left = get_widget_left(widget_name) - 1
    right = get_widget_right(widget_name) + 1
    top = get_widget_top(widget_name) - 1
    
    x = (left + right) / 2
    left_button_down(x, top)
    mousemove(x, top + delta_y, close_notification)
    left_button_up(x, top + delta_y)
    
def drag_bottom(widget_name, delta_y, close_notification = False):
    logging.info('drag bottom of widget "%s"', widget_name)
    left = get_widget_left(widget_name) - 1
    right = get_widget_right(widget_name) + 1
    bottom = get_widget_bottom(widget_name) + 1
    
    x = (left + right) / 2
    left_button_down(x, bottom)
    mousemove(x, bottom + delta_y, close_notification)
    left_button_up(x, bottom + delta_y)

def drag_left_top(widget_name, delta_x, delta_y, close_notification = False):
    logging.info('drag left top of widget "%s"', widget_name)
    left = get_widget_left(widget_name) - 1
    top = get_widget_top(widget_name) - 1
    
    left_button_down(left, top)
    mousemove(left + delta_x, top + delta_y, close_notification)
    left_button_up(left + delta_x, top + delta_y)
    
def drag_right_top(widget_name, delta_x, delta_y, close_notification = False):
    logging.info('drag right top of widget "%s"', widget_name)
    right = get_widget_right(widget_name) + 1
    top = get_widget_top(widget_name) - 1
    
    left_button_down(right, top)
    mousemove(right + delta_x, top + delta_y, close_notification)
    left_button_up(right + delta_x, top + delta_y)
    
def drag_left_bottom(widget_name, delta_x, delta_y, close_notification = False):
    logging.info('drag left bottom of widget "%s"', widget_name)
    left = get_widget_left(widget_name) - 1
    bottom = get_widget_bottom(widget_name) + 1
    
    left_button_down(left, bottom)
    mousemove(left + delta_x, bottom + delta_y, close_notification)
    left_button_up(left + delta_x, bottom + delta_y)
    
def drag_right_bottom(widget_name, delta_x, delta_y, close_notification = False):
    logging.info('drag right bottom of widget "%s"', widget_name)
    right = get_widget_right(widget_name) + 1
    bottom = get_widget_bottom(widget_name) + 1
    
    left_button_down(right, bottom)
    mousemove(right + delta_x, bottom + delta_y, close_notification)
    left_button_up(right + delta_x, bottom + delta_y)
    
def drag_move(widget_name, delta_x, delta_y, close_notification = False):
    logging.info('move widget "%s"', widget_name)
    left = get_widget_left(widget_name)
    right = get_widget_right(widget_name)
    top = get_widget_top(widget_name)
    bottom = get_widget_bottom(widget_name)
    
    x = (left + right) / 2
    y = (top + bottom) / 2
    
    left_button_down(x, y)
    mousemove(x + delta_x, y + delta_y, close_notification)
    left_button_up(x + delta_x, y + delta_y)
    
def insert_folder():
    send_to_project_view('CMD_INSERT_FOLDER',0)
    
def edit_folder_properties():
    post_to_project_view('CMD_EDIT_FOLDER_PROPERTIES', 0)
    wait_dialog_open(EDIT_FOLDER_NAME_DIALOG)
    
def rename_folder(new_name):
    edit_folder_properties()
    set_folder_name(new_name)
    save_folder_name_edit()
    
def delete_folder(do_compare = 1):
    logging.info('delete selected folder')
    send_to_project_view('CMD_DELETE_FOLDER', 0)
        
    if do_compare:
        compare_result()

def post_delete_folder():
    logging.info('post delete folder message')
    post_to_project_view('CMD_DELETE_FOLDER', 0)
    
    
def terminate_app_execution():
    logging.info('terminate app execution')
    send_to_project_view('CMD_TERMINATE_APP_EXECUTION', 0)
    active_dialog_stack.pop()
    logging.debug("pop active dialog: %s", APP_EXECUTION_WINDOW)
    
#================================================================#
#           Send Commands to Resource View                       #
#================================================================#
resource_view_test_commands = [
'',
'CMD_CLICK_RESOURCE_GROUP',
'CMD_CLICK_PIXELMAP_FOLDER',
'CMD_CLICK_RESOURCE_ITEM',
'CMD_ADD_COLOR',
'CMD_EDIT_COLOR',
'CMD_DELETE_COLOR',
'CMD_ADD_FONT',
'CMD_EDIT_FONT',
'CMD_DELETE_FONT',
'CMD_ADD_PIXELMAPS',
'CMD_EDIT_PIXELMAP',
'CMD_EDIT_PIXELMAPS',
'CMD_DELETE_PIXELMAP',
'CMD_ENABLE_PIXELMAP',
'CMD_DISABLE_PIXELMAP',
'CMD_ADD_PIXELMAP_FOLDER',
'CMD_REMOVE_PIXELMAP_FOLDER',
'CMD_ENABLE_PIXELMAP_FOLDER',
'CMD_DISABLE_PIXELMAP_FOLDER',
'CMD_SET_FOLDER_NAME',
'CMD_SAVE_FOLDER_NAME_EDIT',
'CMD_EDIT_STRING',
'CMD_INCREMENT_ACTIVE_LANGUAGE_INDEX',
'CMD_DECREMENT_ACTIVE_LANGUAGE_INDEX',
'CMD_GENERATE_XML'
]

#send a test command to resource view    
def send_to_resource_view(cmd, param):
    handle = studio_windows['Resource View']
    command = resource_view_test_commands.index(cmd)
    return(send_to_studio(handle, command, param))
    
#post a test command to resource view
def post_to_resource_view(cmd, param):
    handle = studio_windows['Resource View']
    command = resource_view_test_commands.index(cmd)
    return(post_to_studio(handle, command, param))
    
def click_resource_group(name):
    logging.info('click resource group: %s', name)
    send_to_resource_view('CMD_CLICK_RESOURCE_GROUP', group_ids[name])
    
def click_pixelmap_folder(folder_id, name):
    logging.info('click resource folder: %s', name)
    param = str(folder_ids[folder_id]) + ',' + name
    send_to_resource_view('CMD_CLICK_PIXELMAP_FOLDER', param)
    
# command the resource view to open color resource folder
def click_resource_item(type, name):
    param = str(resource_item_types[type]) + ',' + name
    send_to_resource_view('CMD_CLICK_RESOURCE_ITEM', param)
    
# command the resource view to open font edit dialog  
def add_font():
    logging.info('add a font')
    post_to_resource_view('CMD_ADD_FONT', 0)  
    wait_dialog_open(EDIT_FONT_DIALOG)
    
def edit_font(id_name):
    click_resource_item('RES_TYPE_FONT', id_name)
    
    logging.info('edit font: %s', id_name)
    post_to_resource_view('CMD_EDIT_FONT', 0)
    wait_dialog_open(EDIT_FONT_DIALOG)   
    
def delete_font(id_name):
    click_resource_item('RES_TYPE_FONT', id_name)
    
    logging.info('delete font: %s', id_name)
    send_to_resource_view('CMD_DELETE_FONT', 0)
  
# command the resource view to open color edit dialog
def add_color():
    logging.info('add a color')
    post_to_resource_view('CMD_ADD_COLOR', 0)
    wait_dialog_open(EDIT_COLOR_DIALOG)
    
def edit_color(id_name):
    click_resource_item('RES_TYPE_COLOR', id_name)
    
    logging.info('edit color: %s', id_name)
    post_to_resource_view('CMD_EDIT_COLOR', 0)
    wait_dialog_open(EDIT_COLOR_DIALOG)
    
def delete_color(id_name):
    click_resource_item('RES_TYPE_COLOR', id_name)
 
    logging.info('delete color: %s', id_name)
    send_to_resource_view('CMD_DELETE_COLOR', 0)
    
# command the resource view to add images
def add_pixelmaps(folder_name, path, name_array, wait = CLOSE_WAIT):
    while is_wait_dialog_running() == True:
        time.sleep(5)
        
    path = os.path.realpath(path) + '\\'
    logging.info("add pixelmaps, folder name = %s, path = %s, names = %s", folder_name, path, name_array)
    
    param = folder_name + ','
    param += path + ','
    param += name_array
    
    post_to_resource_view('CMD_ADD_PIXELMAPS', param)
    
    time.sleep(1)
    if wait == CLOSE_WAIT and find_dialog(WAIT_DIALOG):
        wait_dialog_close(WAIT_DIALOG)
    
    click_pixelmap_folder('CUSTOM_PIXELMAP_FOLDER', folder_name)

def delete_pixelmap(id_name):
    click_resource_item('RES_TYPE_PIXELMAP', id_name)
    logging.info('delete pixelmap: %s', id_name)
    send_to_resource_view('CMD_DELETE_PIXELMAP', 0)
    
def enable_pixelmap(id_name):
    click_resource_item('RES_TYPE_PIXELMAP', id_name)
    logging.info('enable pixelmap: %s', id_name)
    send_to_resource_view('CMD_ENABLE_PIXELMAP', 0)
    
def disable_pixelmap(id_name):
    click_resource_item('RES_TYPE_PIXELMAP', id_name)
    logging.info('disable pixelmap: %s', id_name)
    send_to_resource_view('CMD_DISABLE_PIXELMAP', 0)
    
# command the resoruce view to edit a pixelmap
def edit_pixelmap(id_name):
    click_resource_item('RES_TYPE_PIXELMAP', id_name)
    
    logging.info('edit pixelmap: %s', id_name)
    post_to_resource_view('CMD_EDIT_PIXELMAP', 0)
    wait_dialog_open(EDIT_PIXELMAP_DIALOG)
    
# command the resoruce view to edit pixelmaps under pixelmap group/folder
def edit_pixelmaps():
    logging.info('edit pixelmaps')
    post_to_resource_view('CMD_EDIT_PIXELMAPS', 0)
    wait_dialog_open(EDIT_PIXELMAP_DIALOG)


# command the resource view to show string edit dialog
def edit_string():
    logging.info("edit string table")
    post_to_resource_view('CMD_EDIT_STRING', 0)
    
    wait_dialog_open(STRING_TABLE_EDITOR_DIALOG)
    
def add_pixelmap_folder():
    logging.info('add a pixelmap folder')
    test_utils.click_resource_group('PIXELMAP_GROUP')
    post_to_resource_view('CMD_ADD_PIXELMAP_FOLDER', 0)
    wait_dialog_open(EDIT_FOLDER_NAME_DIALOG)
    test_utils.click_resource_group('PIXELMAP_GROUP')
    
def remove_pixelmap_folder(name):
    click_pixelmap_folder('CUSTOM_PIXELMAP_FOLDER', name)
    logging.info('remove pixelmap folder: %s', name)
    send_to_resource_view('CMD_REMOVE_PIXELMAP_FOLDER', 0)

def enable_pixelmap_folder():
    logging.info('enable pixelmap folder')
    send_to_resource_view('CMD_ENABLE_PIXELMAP_FOLDER', 0)
    
def disable_pixelmap_folder():
    logging.info('disable pixelmap folder')
    send_to_resource_view('CMD_DISABLE_PIXELMAP_FOLDER', 0)
   
def increment_active_language_index():
    logging.info('increase active language index')
    send_to_resource_view('CMD_INCREMENT_ACTIVE_LANGUAGE_INDEX', 0)
    
def decrement_active_language_index():
    logging.info('decrease active language index')
    send_to_resource_view('CMD_DECREMENT_ACTIVE_LANGUAGE_INDEX', 0)

def generate_xml(name):
    logging.info('generate xml file: %s', name)
    name = os.path.abspath(name)
    send_to_resource_view('CMD_GENERATE_XML', name)

#================================================================#
#           Send Commands to Message Dialog                      #
#================================================================#
message_dialog_commands = [
'',
'CMD_YES_TO_MESSAGE_DIALOG',
'CMD_NO_TO_MESSAGE_DIALOG',
'CMD_CLOSE_MESSAGE_DIALOG'
]

def post_to_message_dialog(cmd, param):
    handle = studio_dialogs[NOTIFICATION_DIALOG]
    command = message_dialog_commands.index(cmd)
    return(post_to_studio(handle, command, param))
    
def click_yes_to_message_dialog():
    wait_dialog_open(NOTIFICATION_DIALOG)
    logging.info('click yes button of message dialog.')
    post_to_message_dialog('CMD_YES_TO_MESSAGE_DIALOG', 0)
    wait_dialog_close(NOTIFICATION_DIALOG)

def click_no_to_message_dialog():
    wait_dialog_open(NOTIFICATION_DIALOG)
    logging.info('click no button of message dialog.')
    post_to_message_dialog('CMD_NO_TO_MESSAGE_DIALOG', 0)
    wait_dialog_close(NOTIFICATION_DIALOG)
    
def close_message_dialog(owner = 0):
    retries = 0

    wait_dialog_open(NOTIFICATION_DIALOG, owner)
    logging.info('close notification dialog.')

    while retries < 3:
        post_to_message_dialog('CMD_CLOSE_MESSAGE_DIALOG', 0)
        if wait_dialog_close(NOTIFICATION_DIALOG) is True:
            return;
        retries += 1

def post_to_error_dialog(cmd, param):
    handle = studio_dialogs[ERROR_DIALOG]
    command = message_dialog_commands.index(cmd)
    return(post_to_studio(handle, command, param))


def close_error_dialog(owner = 0):
    retries = 0

    logging.info('close error dialog.')
    wait_dialog_open(ERROR_DIALOG, owner)

    while retries < 3:
        post_to_error_dialog('CMD_CLOSE_MESSAGE_DIALOG', 0)
        if wait_dialog_close(ERROR_DIALOG) is True:
            return;
        retries += 1   
        
def post_to_please_confirm_dialog(cmd, param):
    handle = studio_dialogs[PLEASE_CONFIRM_DIALOG]
    command = message_dialog_commands.index(cmd)
    return(post_to_studio(handle, command, param))


def click_yes_to_please_confirm_dialog():
    wait_dialog_open(PLEASE_CONFIRM_DIALOG)
    logging.info('click yes button of please confirm dialog.')
    post_to_please_confirm_dialog('CMD_YES_TO_MESSAGE_DIALOG', 0)
    wait_dialog_close(PLEASE_CONFIRM_DIALOG)

def click_no_to_please_confirm_dialog():
    wait_dialog_open(PLEASE_CONFIRM_DIALOG)
    logging.info('click no button of please confirm dialog.')
    post_to_please_confirm_dialog('CMD_NO_TO_MESSAGE_DIALOG', 0)
    wait_dialog_close(PLEASE_CONFIRM_DIALOG)
    
#================================================================#
#           Send Commands to Language Configure Dialog           #
#================================================================#

language_configure_commands = [
'',
'CMD_ADD_LANGUAGE',
'CMD_DELETE_LANGUAGE',
'CMD_SELECT_LANGUAGE_INDEX',
'CMD_SELECT_LANGUAGE_ID',
'CMD_CHECK_SUPPORT_BIDI_TEXT',
'CMD_CHECK_REORDER_BIDI_TEXT',
'CMD_CHECK_SUPPORT_THAI_SHAPING',
'CMD_CHECK_GEN_ADJUSTED_THAI',
'CMD_SAVE_LANGUAGE_CONFIGURE',
'CMD_CANCEL_LANGUAGE_CONFIGURE'
]

def send_to_language_configure_dialog(cmd, param):
    handle = studio_dialogs[CONFIGURE_LANGUAGE_DIALOG]
    command = language_configure_commands.index(cmd)
    return(send_to_studio(handle, command, param))

def post_to_language_configure_dialog(cmd, param):
    handle = studio_dialogs[CONFIGURE_LANGUAGE_DIALOG]
    command = language_configure_commands.index(cmd)
    return(post_to_studio(handle, command, param))
    
# command the language configure dialog to add a language 
def add_language():
    logging.info('add a language')
    send_to_language_configure_dialog('CMD_ADD_LANGUAGE', 0)
    
# command the language configure dialog to delete a language
def delete_language():
    logging.info('delete a language')
    send_to_language_configure_dialog('CMD_DELETE_LANGUAGE', 0)
 
# command the languge configure dialog to select a language index 
def select_language_index(language_index):
    logging.info('select language index: %d', language_index)
    send_to_language_configure_dialog('CMD_SELECT_LANGUAGE_INDEX', language_index)
 
# command the language configure dialog to select a language id 
def select_language_id(language):
    logging.info('select language id: %s', language)
    send_to_language_configure_dialog('CMD_SELECT_LANGUAGE_ID', language)

def check_support_bidi_text(check):
    if check:
        logging.info('check on support bidi text box')
    else:
        logging.info('check off support bidi text box')
    send_to_language_configure_dialog('CMD_CHECK_SUPPORT_BIDI_TEXT', check)

def check_reorder_bidi_text(check):
    if check:
        logging.info('check on reorder bidi text box')
    else:
        logging.info('check off reorder bidi text box')
    send_to_language_configure_dialog('CMD_CHECK_REORDER_BIDI_TEXT', check)

def check_support_thai_glyph_shapping(check):
    if check:
        logging.debug('check on support support thai glyph shaping box')
    else:
        logging.debug('check off support support thai glyph shaping box')
    send_to_language_configure_dialog('CMD_CHECK_SUPPORT_THAI_SHAPING', check)

def check_gen_adjusted_thai_string(check):
    if check:
        logging.debug('check on generate adjusted thai string box')
    else:
        logging.debug('check off generate adjusted thai string box')
    send_to_language_configure_dialog('CMD_CHECK_GEN_ADJUSTED_THAI', check)
    
# command the languge configure dialog to save the changes  
def save_language_configure():
    logging.info('save language configuration')
    post_to_language_configure_dialog('CMD_SAVE_LANGUAGE_CONFIGURE', 0)
    wait_dialog_close(CONFIGURE_LANGUAGE_DIALOG)
 
# command the language configure dialog to cancel the changes 
def cancel_language_configure():
    logging.info('cancel language configuration')
    post_to_language_configure_dialog('CMD_CANCEL_LANGUAGE_CONFIGURE', 0)
    wait_dialog_close(CONFIGURE_LANGUAGE_DIALOG)
  
#================================================================#
#           Send Commands to Project Configure Dialog            #
#================================================================#
  
project_configure_commands  = [
'',
'CMD_CONFIGURE_SOURCE_PATH',
'CMD_CONFIGURE_HEADER_PATH',
'CMD_CONFIGURE_RESOURCE_PATH',
'CMD_CONFIGURE_X_RES',
'CMD_CONFIGURE_Y_RES',
'CMD_CONFIGURE_DISPLAY_NUM',
'CMD_SELECT_DISPLAY_INDEX',
'CMD_CONFIGURE_DISPLAY_NAME',
'CMD_CONFIGURE_DISPLAY_COLOR_FORMAT',
'CMD_CONFIGURE_DISPLAY_COLOR_DEPTH',
'CMD_CONFIGURE_DISPLAY_RGB_BITS',
'CMD_CONFIGURE_MINOR_VERSION',
'CMD_CONFIGURE_SERVICE_PACK',
'CMD_CONFIGURE_CANVAS_ALLOCATE',
'CMD_CONFIGURE_CPU_TYPE',
'CMD_CONFIGURE_BIG_ENDIAN',
'CMD_SAVE_PROJECT_CONFIGURE',
'CMD_CANCEL_PROJECT_CONFIGURE',
'CMD_IS_DEFAULT_MAP_FORMAT',
'CMD_GET_CPU_TYPE',
'CMD_GET_DISPLAY_COLOR_FORMAT',
'CMD_GET_DISPLAY_COLOR_DEPTH',
'CMD_IS_1555_FORMAT',
'CMD_IS_4444_FORAMT',
'CMD_IS_332_FORMAT',
'CMD_IS_NEW_PROJECT',
'CMD_OPEN_ADVANCED_SETTINGS',
'CMD_SET_AA_TEXT_COLORS'
]

def send_to_project_configure_dialog(cmd, param):
    handle = studio_dialogs[CONFIGURE_PROJECT_DIALOG]
    command = project_configure_commands.index(cmd)
    return(send_to_studio(handle, command, param))

def post_to_project_configure_dialog(cmd, param):
    handle = studio_dialogs[CONFIGURE_PROJECT_DIALOG]
    command = project_configure_commands.index(cmd)
    return(post_to_studio(handle, command, param))

def is_default_map_format():
    return send_to_project_configure_dialog('CMD_IS_DEFAULT_MAP_FORMAT', 0)
    
def get_cpu_type():
    return send_to_project_configure_dialog('CMD_GET_CPU_TYPE', 0)
    
def get_display_color_format():
    return send_to_project_configure_dialog('CMD_GET_DISPLAY_COLOR_FORMAT', 0)
    
def get_display_color_depth():
    return send_to_project_configure_dialog('CMD_GET_DISPLAY_COLOR_DEPTH', 0)

def is_1555_format():
    return  send_to_project_configure_dialog('CMD_IS_1555_FORMAT', 0)
    
def is_4444_format():
    return  send_to_project_configure_dialog('CMD_IS_4444_FORAMT', 0)
    
def is_332_format():
    return  send_to_project_configure_dialog('CMD_IS_332_FORMAT', 0)
    
def is_new_project():
    return send_to_project_configure_dialog('CMD_IS_NEW_PROJECT', 0)

def set_aa_text_colors(count):
    param = str(count)
    logging.info('set palette mode aa text colors = %d', count)
    send_to_project_configure_dialog('CMD_SET_AA_TEXT_COLORS', param)
    
def configure_source_path(path):
    path = os.path.realpath(path)
    
    if os.path.exists(path) == False:
        os.makedirs(path)
        
    logging.info('set source files path: %s', path)
    send_to_project_configure_dialog('CMD_CONFIGURE_SOURCE_PATH', path)
    
def configure_header_path(path):
    path = os.path.realpath(path)
    
    if os.path.exists(path) == False:
        os.makedirs(path)
        
    logging.info('set header files path: %s', path)
    send_to_project_configure_dialog('CMD_CONFIGURE_HEADER_PATH', path)
    
def configure_resource_path(path):
    path = os.path.realpath(path)
    
    if os.path.exists(path) == False:
        os.makedirs(path)
    
    logging.info('set resource files path: %s', path)
    send_to_project_configure_dialog('CMD_CONFIGURE_RESOURCE_PATH', path)
    
def configure_x_res(x_res):
    logging.info('set x resolution to "%d"' %x_res)
    send_to_project_configure_dialog('CMD_CONFIGURE_X_RES', str(x_res))
    
def configure_y_res(y_res):
    logging.info('set y resolution to "%d"' %y_res)
    send_to_project_configure_dialog('CMD_CONFIGURE_Y_RES', str(y_res))
    
def configure_display_num(display_num):
    logging.info('set display num: %d', display_num)
    send_to_project_configure_dialog('CMD_CONFIGURE_DISPLAY_NUM', display_num)
    
def select_display_index(display_index):
    logging.info('select display index: %d', display_index)
    send_to_project_configure_dialog('CMD_SELECT_DISPLAY_INDEX', display_index)
    
def configure_display_name(name):
    logging.info('set display name to "%s"' %name)
    send_to_project_configure_dialog('CMD_CONFIGURE_DISPLAY_NAME', name)
    
def configure_display_color_format(format):
    global reset_map_format
    global change_color_table

    if (get_display_color_format() != color_formats[format]) and (not is_new_project()):
        reset_map_format = True
        change_color_table = True
    else:
        reset_map_format = False
        change_color_table = False
        
    logging.info('set display color format to "%s"' %format)
    send_to_project_configure_dialog('CMD_CONFIGURE_DISPLAY_COLOR_FORMAT', color_formats[format])
    
def configure_display_color_depth(color_depth):
    logging.info('set display color depth to "%s"' %color_depth)
    status = send_to_project_configure_dialog('CMD_CONFIGURE_DISPLAY_COLOR_DEPTH', color_depth)

    if (status == 0) and (not is_new_project()): #means set color depth success
        logging.info('set display color depth to "%s" successfully.' %color_depth)
        global reset_map_format
        global change_color_table
        reset_map_format = True
        change_color_table = True
    
def configure_display_rgb_bits(rgb_bits):
    logging.info('set checkbox rgb bits to "%s"' %rgb_bits)
    send_to_project_configure_dialog('CMD_CONFIGURE_DISPLAY_RGB_BITS', rgb_bits)
    
def configure_minor_version(minor_version):
    logging.info('set minor version to "%d"' %minor_version)
    send_to_project_configure_dialog('CMD_CONFIGURE_MINOR_VERSION', minor_version)
    
def configure_service_pack(service_pack):
    logging.info('set service pack to "%d"' %service_pack)
    send_to_project_configure_dialog('CMD_CONFIGURE_SERVICE_PACK', service_pack)
 
def configure_canvas_allocate(check):
    if check:
        logging.info('check on "Allocate Canvas"')
    else:
        logging.info('check off "Allocate Canvas"')
        
    send_to_project_configure_dialog('CMD_CONFIGURE_CANVAS_ALLOCATE', check)
    
def configure_cpu_type(type):
    global reset_map_format
    if get_cpu_type() != test_constants.cpu_types[type]:
        reset_map_format = True
    else: 
        reset_map_format = False
        
    logging.info('set cpu type: %s', type)
    send_to_project_configure_dialog('CMD_CONFIGURE_CPU_TYPE', test_constants.cpu_types[type])
    
def configure_big_endian(check):
    if check:
        logging.info('check on "big endian"')
    else:
        logging.info('check off "big endian"')
        
    send_to_project_configure_dialog('CMD_CONFIGURE_BIG_ENDIAN', check)

def open_advanced_settings():
    logging.info('open synergy advanced settings dialog')
    post_to_project_configure_dialog('CMD_OPEN_ADVANCED_SETTINGS', 0)
    wait_dialog_open(SYNERGY_SETTINGS_DIALOG, studio_dialogs[CONFIGURE_PROJECT_DIALOG])
    
def save_project_configure(wait_close = CLOSE_WAIT):
    logging.info("save project configure")
    
    # check if all pixelmaps are default color format
    is_default_map = is_default_map_format()
    post_to_project_configure_dialog('CMD_SAVE_PROJECT_CONFIGURE', 0)
    
    global reset_map_format
    global change_color_table

    if change_color_table == True:
        logging.info('Close warning dialog about reset color table')
        close_message_dialog(studio_dialogs[CONFIGURE_PROJECT_DIALOG])
    change_color_table = False

    if is_default_map == False and reset_map_format == True:
        # a notification message is popup, close it
        logging.info('Close warning dialog about reset pixelmap format')
        close_message_dialog(studio_dialogs[CONFIGURE_PROJECT_DIALOG])
    reset_map_format = False
    
    if wait_close == CLOSE_WAIT:
        wait_dialog_close(CONFIGURE_PROJECT_DIALOG)
    
def cancel_project_configure():
    logging.info("cance project configure")
    post_to_project_configure_dialog('CMD_CANCEL_PROJECT_CONFIGURE', 0)
    wait_dialog_close(CONFIGURE_PROJECT_DIALOG)
   
#================================================================#
#           Send Commands to Color Edit Dialog                   #
#================================================================#   
color_edit_commands = [
'',
'CMD_SET_COLOR_NAME',
'CMD_SET_COLOR_RED',
'CMD_SET_COLOR_GREEN',
'CMD_SET_COLOR_BLUE',
'CMD_SET_COLOR_HUE',
'CMD_SET_COLOR_SAT',
'CMD_SET_COLOR_LUM',
'CMD_PALETTE_COLOR_INDEX',
'CMD_SAVE_COLOR_EDIT',
'CMD_CANCEL_COLOR_EDIT'
]

def send_to_color_edit_dialog(cmd, param):
    handle = studio_dialogs[EDIT_COLOR_DIALOG]
    command = color_edit_commands.index(cmd)
    return(send_to_studio(handle, command, param))

def post_to_color_edit_dialog(cmd, param):
    handle = studio_dialogs[EDIT_COLOR_DIALOG]
    command = color_edit_commands.index(cmd)
    return(post_to_studio(handle, command, param))

# command the color edit dialog to set color name
def set_color_name(name):
    logging.info('set color name: %s', name)
    send_to_color_edit_dialog('CMD_SET_COLOR_NAME', name)
    
# command the color edit dialog to set value of red channel
def set_color_red(red):
    logging.info('set red value: %d', red)
    send_to_color_edit_dialog('CMD_SET_COLOR_RED', str(red))
    
# command the color edit dialog to set value of green channel
def set_color_green(green):
    logging.info('set green value: %d', green)
    send_to_color_edit_dialog('CMD_SET_COLOR_GREEN', str(green))
    
# command the color edit dialog to set value of blue channel
def set_color_blue(blue):
    logging.info('set blue value: %d', blue)
    send_to_color_edit_dialog('CMD_SET_COLOR_BLUE', str(blue))
    
# command the color edit dialog to set value of hue channel
def set_color_hue(hue):
    logging.info('set hue value: %d', hue)
    send_to_color_edit_dialog('CMD_SET_COLOR_HUE', str(hue))
    
# command the color edit dialog to set value of saturation channel
def set_color_sat(sat):
    logging.info('set sat value: %d', sat)
    send_to_color_edit_dialog('CMD_SET_COLOR_SAT', str(sat))
    
# command the color edit dialog to set value of luminance channel
def set_color_lum(lum):
    logging.info('set lum value: %d', lum)
    send_to_color_edit_dialog('CMD_SET_COLOR_LUM', str(lum))
    
def set_palette_color_index(index):
    logging.info('set palette color index: %d', index)
    send_to_color_edit_dialog('CMD_PALETTE_COLOR_INDEX', index)
    
# command the color edit dialog to save color edit
def save_color_edit(wait_close = CLOSE_WAIT):
    logging.info('save color edit')
    post_to_color_edit_dialog('CMD_SAVE_COLOR_EDIT', 0)
    if wait_close == CLOSE_WAIT:
        wait_dialog_close(EDIT_COLOR_DIALOG)
    
# command the color edit dialog to cancel color edit
def cancel_color_edit():
    logging.info('cancel color edit')
    post_to_color_edit_dialog('CMD_CANCEL_COLOR_EDIT', 0)
    wait_dialog_close(EDIT_COLOR_DIALOG)
    
#================================================================#
#           Send Commands to Font Edit Dialog                    #
#================================================================# 
font_edit_commands = [
'',
'CMD_SET_FONT_PATH',
'CMD_SET_FONT_NAME',
'CMD_SET_FONT_HEIGHT',
'CMD_SET_FONT_HEIGHT_POST',
'CMD_SET_FONT_FORMAT',
'CMD_SET_FONT_COMPRESSION',
'CMD_SET_FONT_GENERATE_KERNING',
'CMD_SET_PAGE_RANGE',
'CMD_CHECK_EXTENDED_UNICODE',
'CMD_CHECK_CUSTOM_OUTPUT',
'CMD_SET_CUSTOM_OUTPUT_FILE',
'CMD_SAVE_FONT_EDIT',
'CMD_CANCEL_FONT_EDIT'
]

def send_to_font_edit_dialog(cmd, param):
    handle = studio_dialogs[EDIT_FONT_DIALOG]
    command = font_edit_commands.index(cmd)
    return(send_to_studio(handle, command, param))

def post_to_font_edit_dialog(cmd, param):
    handle = studio_dialogs[EDIT_FONT_DIALOG]
    command = font_edit_commands.index(cmd)
    return(post_to_studio(handle, command, param))
    
# command the font edit dialog to set font path
def set_font_path(font_path):
    logging.info('set truetype path: %s', font_path)
    send_to_font_edit_dialog('CMD_SET_FONT_PATH', font_path)
   
# command the font edit dialog to set font name   
def set_font_name(font_name):
    logging.info('set font name: %s', font_name)
    send_to_font_edit_dialog('CMD_SET_FONT_NAME', font_name)
  
# command the font edit dialog to set font format  
def set_font_format(format):
    logging.info('set font format: %d', font_formats[format])
    send_to_font_edit_dialog('CMD_SET_FONT_FORMAT', font_formats[format])
   
# command the font edit dialog to set font height   
def set_font_height(height):
    logging.info('set font height: %d', height)
    send_to_font_edit_dialog('CMD_SET_FONT_HEIGHT', str(height))

def set_font_height_post(height, close_notification = False):
    logging.info('set font height: %d', height)  
    post_to_font_edit_dialog('CMD_SET_FONT_HEIGHT_POST', str(height))
    
    if close_notification == True:        
        close_message_dialog(studio_dialogs[EDIT_FONT_DIALOG])
        
# command the font edit dialog to set font compression  
def set_font_compression(compress):
    logging.info('set font compression: %d', compress)
    send_to_font_edit_dialog('CMD_SET_FONT_COMPRESSION', compress)
    
def set_font_generate_kerning(kerning):
    logging.info('set font generate kerning info: %d', kerning)
    send_to_font_edit_dialog('CMD_SET_FONT_GENERATE_KERNING', kerning)
    
# command the font edit dialog to set page range information
def set_page_range(range_index, range_enabled, range_start, range_end):
    logging.info('set page range, range_index %d, range_enabled, %d range_start %s, range_end %s',
                 range_index, range_enabled, range_start, range_end)
    param = str(range_index) + ','
    param += str(range_enabled) + ','
    param += str(range_start) + ','
    param += str(range_end)
    
    send_to_font_edit_dialog('CMD_SET_PAGE_RANGE', param)
    
# command the font edit dialog to check on/off extended unicode support  
def check_extended_unicode(check):
    logging.info('check extended unicode support: %d', check)
    send_to_font_edit_dialog('CMD_CHECK_EXTENDED_UNICODE', check)
    
def check_font_custom_output(check):
    logging.info('check font custom output: %d', check)
    send_to_font_edit_dialog('CMD_CHECK_CUSTOM_OUTPUT', check)
    
def set_font_custom_output_file(filename):
    logging.info('set font custom output file: %s', filename)
    send_to_font_edit_dialog('CMD_SET_CUSTOM_OUTPUT_FILE', filename)
    
# command the font edit dialog to save modifications  
def save_font_edit(wait_close = CLOSE_WAIT):
    logging.info('save font edit')
    post_to_font_edit_dialog('CMD_SAVE_FONT_EDIT', 0)
    if wait_close == CLOSE_WAIT:
        wait_dialog_close(EDIT_FONT_DIALOG)
   
# command the font edit dialog to cancel modifications   
def cancel_font_edit():
    logging.info('cancel font edit')
    post_to_font_edit_dialog('CMD_CANCEL_FONT_EDIT', 0)
    wait_dialog_close(EDIT_FONT_DIALOG)
    
    
#================================================================#
#           Send Commands to Pixelmap Edit Dialog                #
#================================================================#   
pixelmap_edit_commands = [
"",
'CMD_SET_IMAGE_PATH',
'CMD_SET_IMAGE_NAME',
'CMD_CHECK_CUSTOM_OUTPUT',
'CMD_SET_CUSTOM_OUTPUT_FILE',
'CMD_CHECK_RAW_FORMAT',
'CMD_CHECK_COMPRESS',
'CMD_CHECK_ALPHA',
'CMD_CHECK_DITHER',
'CMD_SET_OUTPUT_FORMAT',
'CMD_SET_PALETTE_TYPE',
'CMD_SAVE_PIXELMAP_EDIT',
'CMD_CANCEL_PIXELMAP_EDIT'
]

def send_to_pixelmap_edit_dialog(cmd, param):
    handle = studio_dialogs[EDIT_PIXELMAP_DIALOG]
    command = pixelmap_edit_commands.index(cmd)
    return(send_to_studio(handle, command, param))

def post_to_pixelmap_edit_dialog(cmd, param):
    handle = studio_dialogs[EDIT_PIXELMAP_DIALOG]
    command = pixelmap_edit_commands.index(cmd)
    return(post_to_studio(handle, command, param))
    
def set_image_path(path):
    logging.info('set image path: %s', path)
    send_to_pixelmap_edit_dialog('CMD_SET_IMAGE_PATH', path)
    
def set_image_id_name(id_name):
    logging.info('set image id name: %s', id_name)
    send_to_pixelmap_edit_dialog('CMD_SET_IMAGE_NAME', id_name)
  
def check_custom_output(check):
    logging.info('check custom output: %d', check)
    send_to_pixelmap_edit_dialog('CMD_CHECK_CUSTOM_OUTPUT', check)
    
def set_custom_output_file(filename):
    logging.info('set custom output file: %s', filename)
    send_to_pixelmap_edit_dialog('CMD_SET_CUSTOM_OUTPUT_FILE', filename)
    
def check_raw_format(check, close_edit_dialog = False):
    logging.info('check raw format: %d', check)

    if(close_edit_dialog == True):
        post_to_pixelmap_edit_dialog('CMD_CHECK_RAW_FORMAT', check)
        close_error_dialog(studio_dialogs[EDIT_PIXELMAP_DIALOG])
    else:
        send_to_pixelmap_edit_dialog('CMD_CHECK_RAW_FORMAT', check)
    
def check_compress(check):
    logging.info('check compress: %d', check)
    send_to_pixelmap_edit_dialog('CMD_CHECK_COMPRESS', check)
    
def check_alpha(check):
    logging.info('check compress: %d', check)
    send_to_pixelmap_edit_dialog('CMD_CHECK_ALPHA', check)
    
def check_dither(check):
    logging.info('check dither: %d', check)
    send_to_pixelmap_edit_dialog('CMD_CHECK_DITHER', check)
    
def set_output_format(format):
    logging.info('set output format: %s', format)
    send_to_pixelmap_edit_dialog('CMD_SET_OUTPUT_FORMAT', color_formats[format])
 
def set_palette_type(type):
    logging.info('set palette type: %s', format)
    send_to_pixelmap_edit_dialog('CMD_SET_PALETTE_TYPE', palette_types[type])
    
def save_pixelmap_edit(wait_close = CLOSE_WAIT):
    logging.info('save pixelmap edit')
    post_to_pixelmap_edit_dialog('CMD_SAVE_PIXELMAP_EDIT', 0)
    if wait_close == CLOSE_WAIT:
        wait_dialog_close(EDIT_PIXELMAP_DIALOG)
    
def cancel_pixelmap_edit():
    logging.info('cancel pixelmap edit')
    post_to_pixelmap_edit_dialog('CMD_CANCEL_PIXELMAP_EDIT', 0)
    wait_dialog_close(EDIT_PIXELMAP_DIALOG)   
    
    
#================================================================#
#           Send Commands to Folder Name Dialog                  #
#================================================================#   
folder_name_commands=[
'',
'CMD_SET_FOLDER_NAME',
'CMD_SAVE_FOLDER_NAME_EDIT',
'CMD_CHECK_SPECIFY_OUTPUT_FILE',
'CMD_SET_FOLDER_OUTPUT_FILE_NAME',
]

def send_to_folder_name_dialog(cmd, param):
    handle = studio_dialogs[EDIT_FOLDER_NAME_DIALOG]
    command = folder_name_commands.index(cmd)
    return(send_to_studio(handle, command, param))

def post_to_folder_name_dialog(cmd, param):
    handle = studio_dialogs[EDIT_FOLDER_NAME_DIALOG]
    command = folder_name_commands.index(cmd)
    return(post_to_studio(handle, command, param))
    
def set_folder_name(name):
    logging.info('set folder name to "%s"', name)
    send_to_folder_name_dialog('CMD_SET_FOLDER_NAME', name)
    
def save_folder_name_edit():
    logging.info('save folder name edit')
    post_to_folder_name_dialog('CMD_SAVE_FOLDER_NAME_EDIT', 0)
    wait_dialog_close(EDIT_FOLDER_NAME_DIALOG)
    
def check_specify_output_file(check):
    logging.info('set folder name to "%d"', check)
    send_to_folder_name_dialog('CMD_CHECK_SPECIFY_OUTPUT_FILE', check)
    
def set_folder_output_file_name(name):
    logging.info('set folder output file name to "%s"', name)
    send_to_folder_name_dialog('CMD_SET_FOLDER_OUTPUT_FILE_NAME', name)
        
#================================================================#
#           Send Commands to New Project Dialog                  #
#================================================================#   
new_project_commands = [
'',
'CMD_SET_NEW_PROJECT_PATH',
'CMD_SET_NEW_PROJECT_NAME',
'CMD_SAVE_NEW_PROJECT_CREATE',
'CMD_CANCEL_NEW_PROJECT_CREATE'
]

def send_to_new_project_dialog(cmd, param):
    handle = studio_dialogs[CREATE_NEW_PROJECT_DIALOG]
    command = new_project_commands.index(cmd)
    return(send_to_studio(handle, command, param))

def post_to_new_project_dialog(cmd, param):
    handle = studio_dialogs[CREATE_NEW_PROJECT_DIALOG]
    command = new_project_commands.index(cmd)
    return(post_to_studio(handle, command, param))
    
def set_new_project_path(path):
    path = os.path.realpath(path)
    
    if os.path.exists(path) == False:
        os.makedirs(path)
        
    logging.info('set project path to "%s"', path)
    send_to_new_project_dialog('CMD_SET_NEW_PROJECT_PATH', path)
    
def set_new_project_name(name):
    logging.info('set project name to "%s"', name)
    send_to_new_project_dialog('CMD_SET_NEW_PROJECT_NAME', name)
    
def save_new_project_create():
    logging.info('save new project create')
    post_to_new_project_dialog('CMD_SAVE_NEW_PROJECT_CREATE', 0)
    wait_dialog_close(CREATE_NEW_PROJECT_DIALOG)
    
    # wait for project configure dialog show up
    wait_dialog_open(test_utils.CONFIGURE_PROJECT_DIALOG)
    
    # save configure
    save_project_configure()
    
def cancel_new_project_create():
    logging.info('cancel new project create')
    post_to_new_project_dialog('CMD_CANCEL_NEW_PROJECT_CREATE', 0)
    wait_dialog_close(CREATE_NEW_PROJECT_DIALOG)
    
#================================================================#
#           Send Commands to String Table Dialog                 #
#================================================================#   
string_edit_commands = [
'',
'CMD_GET_STRING_COUNT',
'CMD_ADD_STRING',
'CMD_DELETE_STRING',
'CMD_IMPORT_STRING',
'CMD_EXPORT_STRING',
'CMD_TOGGLE_THREE_COLUMN_MODE',
'CMD_INCREMENT_TRANS_LANGUAGE',
'CMD_DECREMENT_TRANS_LANGUAGE',
'CMD_EDIT_TOP_STRING',
'CMD_EDIT_BOTTOM_STRING',
'CMD_EDIT_STRING_ID',
'CMD_SELECT_STRING',
'CMD_SAVE_STRING_EDIT',
'CMD_CANCEL_STRING_EDIT',
'CMD_SORT_STRING'
]

def send_to_string_edit_dialog(cmd, param):
    handle = studio_dialogs[STRING_TABLE_EDITOR_DIALOG]
    command = string_edit_commands.index(cmd)
    return(send_to_studio(handle, command, param))

def post_to_string_edit_dialog(cmd, param):
    handle = studio_dialogs[STRING_TABLE_EDITOR_DIALOG]
    command = string_edit_commands.index(cmd)
    return(post_to_studio(handle, command, param))
    
# command the string table edit dialog to get string count
def get_string_count():
    return send_to_string_edit_dialog('CMD_GET_STRING_COUNT', 0)
    
# command the string table edit dialog to add a string
def add_string():
    logging.info('add a string')
    send_to_string_edit_dialog('CMD_ADD_STRING', 0)
   
# command the string table edit dialog to delete a string
def delete_string():
    logging.info('delete a string')
    send_to_string_edit_dialog('CMD_DELETE_STRING', 0)
    
# command the string table edit dialog to import a xliff file
def import_string(filename):
    filename = os.path.realpath(filename)
    logging.info('import string file: %s', filename)
    post_to_string_edit_dialog('CMD_IMPORT_STRING', filename)
    close_message_dialog(studio_dialogs[STRING_TABLE_EDITOR_DIALOG])
    
# command the string table edit dialog to export a xliff file
def export_string():
    logging.info('export to string file')
    post_to_string_edit_dialog('CMD_EXPORT_STRING', 0)
    wait_dialog_open(STRING_EXPORT_CONTROL_DIALOG, studio_dialogs[STRING_TABLE_EDITOR_DIALOG])
 
# command the string table edit dialog to toggole three column mode
def toggle_three_column_mode():
    logging.info('toggle three column mode')
    send_to_string_edit_dialog('CMD_TOGGLE_THREE_COLUMN_MODE', 0)
    
# command the string table edit dialog to increment transfer language
def increment_trans_language():
    logging.info('increment transfer language')
    send_to_string_edit_dialog('CMD_INCREMENT_TRANS_LANGUAGE', 0)
    
# command the string table edit dialog to decrement transfer language
def decrement_trans_language():
    logging.info('decrement transfer language')
    send_to_string_edit_dialog('CMD_DECREMENT_TRANS_LANGUAGE', 0)
    
# command the string table edit dialog to edit top string
def edit_top_string(string):
    logging.info('edit top string: %s', string.encode(sys.stdout.encoding, 'replace'))
    send_to_string_edit_dialog('CMD_EDIT_TOP_STRING', string)

# command the string table edit dialog to edit bottom string
def edit_bottom_string(string):
    logging.info('edit bottom string: %s', string.encode(sys.stdout.encoding, 'replace'))
    send_to_string_edit_dialog('CMD_EDIT_BOTTOM_STRING', string)
    
# command the string table edit dialog to edit string id
def edit_string_id(id_name):
    logging.info('edit string id: %s', id_name)
    send_to_string_edit_dialog('CMD_EDIT_STRING_ID', id_name)
    
# command the string table edit dialog to select a string    
def select_string(row_index):
    logging.info('select string row: %d', row_index)
    send_to_string_edit_dialog('CMD_SELECT_STRING', row_index)
 
# command the string table edit dialog to save the string table changes 
def save_string_edit(wait_close = CLOSE_WAIT):
    logging.info('save string edit')
    post_to_string_edit_dialog('CMD_SAVE_STRING_EDIT', 0)
    if wait_close == CLOSE_WAIT:
        wait_dialog_close(STRING_TABLE_EDITOR_DIALOG)
    
# command the string table edit dialog to close dialog
def cancel_string_edit():
    logging.info('cancel string edit')
    post_to_string_edit_dialog('CMD_CANCEL_STRING_EDIT', 0)
    wait_dialog_close(STRING_TABLE_EDITOR_DIALOG)
    
def sort_string(type):
    logging.info('sort string by: %s', type)
    send_to_string_edit_dialog('CMD_SORT_STRING', type)
    
#================================================================#
#           Send Commands to String Export Dialog                #
#================================================================#
string_export_commands = [
'',
'CMD_SET_STRING_EXPORT_SRC_LANGUAGE',
'CMD_SET_STRING_EXPORT_TARGET_LANGUAGE',
'CMD_SET_XLIFF_VERSION',
'CMD_SET_STRING_EXPORT_FILENAME',
'CMD_SET_STRING_EXPORT_PATH',
'CMD_SELECT_STRING_EXPORT_FORMAT',
'CMD_SAVE_STRING_EXPORT',
'CMD_CANCEL_STRING_EXPORT'
]

def send_to_string_export_dialog(cmd, param):
    handle = studio_dialogs[STRING_EXPORT_CONTROL_DIALOG]
    command = string_export_commands.index(cmd)
    return(send_to_studio(handle, command, param))

def post_to_string_export_dialog(cmd, param):
    handle = studio_dialogs[STRING_EXPORT_CONTROL_DIALOG]
    command = string_export_commands.index(cmd)
    return(post_to_studio(handle, command, param))
    
# command the string export control dialog to set source language    
def set_string_export_src_language(language):
    logging.info('set source language: %s', language)
    send_to_string_export_dialog('CMD_SET_STRING_EXPORT_SRC_LANGUAGE', language)
    
# command the string export control dialog to set target language
def set_string_export_target_language(language):
    logging.info('set target language: %s', language)
    send_to_string_export_dialog('CMD_SET_STRING_EXPORT_TARGET_LANGUAGE', language)
    
# command the string export control dialog to set xliff version
def set_xliff_version(version):
    logging.info('set XLIFF version: %s', test_constants.xliff_versions[version])
    send_to_string_export_dialog('CMD_SET_XLIFF_VERSION', test_constants.xliff_versions[version])
   
# command the string export control dialog to set string export filename   
def set_string_export_filename(filename):
    logging.info('set string export filename: %s', filename)
    send_to_string_export_dialog('CMD_SET_STRING_EXPORT_FILENAME', filename)
    
# command the string export control dialog to set string export filepath
def set_string_export_path(path):
    path = os.path.realpath(path)
    
    if os.path.exists(path) == False:
        os.makedirs(path)
        
    logging.info('set string export file path: %s', path)
    send_to_string_export_dialog('CMD_SET_STRING_EXPORT_PATH', path)
    
def select_string_export_format(format_type):
    logging.info('set string export format to: %s', test_constants.string_export_types[format_type])
    send_to_string_export_dialog('CMD_SELECT_STRING_EXPORT_FORMAT', test_constants.string_export_types[format_type])
    
# command the string export control dialog to save string export
def save_string_export():
    logging.info('save string export')
    post_to_string_export_dialog('CMD_SAVE_STRING_EXPORT', 0)
    wait_dialog_close(STRING_EXPORT_CONTROL_DIALOG, studio_dialogs[STRING_TABLE_EDITOR_DIALOG])
    close_message_dialog(studio_dialogs[STRING_TABLE_EDITOR_DIALOG])
    
# command the string export control dialog to cancel string export
def cancel_string_export():
    logging.info('cancel string export')
    post_to_string_export_dialog('CMD_CANCEL_STRING_EXPORT', 0)
    wait_dialog_close(STRING_EXPORT_CONTROL_DIALOG, studio_dialogs[STRING_TABLE_EDITOR_DIALOG])
    

#================================================================#
#          Send Commands to Grid Snap Setting Dialog             #
#================================================================#   
grid_snap_setting_commands = [
    '',
    'CMD_CHECK_SHOW_GRID',
    'CMD_CHECK_SNAP',
    'CMD_SET_GRID_SPACING',
    'CMD_SET_SNAP_SPACING',
    'CMD_SAVE',
    'CMD_CANCEL'
]

def send_to_grid_snap_setting_dialog(cmd, param):
    handle = studio_dialogs[GRID_SNAP_SETTING_DIALOG]
    command = grid_snap_setting_commands.index(cmd)
    return(send_to_studio(handle, command, param))

def post_to_grid_snap_setting_dialog(cmd, param):
    handle = studio_dialogs[GRID_SNAP_SETTING_DIALOG]
    command = grid_snap_setting_commands.index(cmd)
    return(post_to_studio(handle, command, param))
    
def check_show_grid(check):
    if check:
        logging.info('check on show grid')
    else:
        logging.info('check off show grid')
        
    send_to_grid_snap_setting_dialog('CMD_CHECK_SHOW_GRID', check)

def check_snap(check):
    if check:
        logging.info('check on snap to grid')
    else:
        logging.info('check off snap to grid')
        
    send_to_grid_snap_setting_dialog('CMD_CHECK_SNAP', check)
    
def set_grid_spacing(spacing):
    logging.info('set grid spacing to: %d', spacing)
    send_to_grid_snap_setting_dialog('CMD_SET_GRID_SPACING', spacing)
    
def set_snap_spacing(spacing):
    logging.info('set snap spacing to: %d', spacing)
    send_to_grid_snap_setting_dialog('CMD_SET_SNAP_SPACING', spacing)
    
def save_grid_snap_setting():
    logging.info('save grid snap setting')
    post_to_grid_snap_setting_dialog('CMD_SAVE', 0)
    wait_dialog_close(GRID_SNAP_SETTING_DIALOG)
    
def cancel_grid_snap_setting():
    logging.info('save grid snap setting')
    post_to_grid_snap_setting_dialog('CMD_CANCEL', 0)
    wait_dialog_close(GRID_SNAP_SETTING_DIALOG)
    
#================================================================#
#          Send Commands to Resource Export Dialog               #
#================================================================#   
resource_export_dialog_commands = [
    '',
    'CMD_CHECK_BINARY_MODE',
    'CMD_CHECK_RES_HEADER_GEN',
    'CMD_CHECK_RESOURCE',
    'CMD_SET_CUSTOM_RESOURCE_NAME',
    'CMD_GENERATE',
    'CMD_CANCEL'
]

def send_to_resource_export_dialog(cmd, param):
    handle = studio_dialogs[RESOURCE_EXPORT_DIALOG]
    command = resource_export_dialog_commands.index(cmd)
    return(send_to_studio(handle, command, param))
    
def post_to_resource_export_dialog(cmd, param):
    handle = studio_dialogs[RESOURCE_EXPORT_DIALOG]
    command = resource_export_dialog_commands.index(cmd)
    return(post_to_studio(handle, command, param))
    
def check_binary_mode(check):
    if check:
        logging.info('check on binary mode')
    else:
        logging.info('check off binary mode')
        
    send_to_resource_export_dialog('CMD_CHECK_BINARY_MODE', check)
    
def check_res_header_gen(check):
    if check:
        logging.info('check on resource header generation')
    else:
        logging.info('check off resource header genearation')
        
    send_to_resource_export_dialog('CMD_CHECK_RES_HEADER_GEN', check)
    
def check_resource(parent_name, resource_name, check):
    if check:
        logging.info('check on %s/%s', parent_name, resource_name)
    else:
        logging.info('check off %s/%s', parent_name, resource_name)
        
    param = parent_name + ','
    param += resource_name + ','
    param += str(check)
    
    send_to_resource_export_dialog('CMD_CHECK_RESOURCE', param)
    
def set_custom_resource_name(custom_name):
    logging.info('set ')
    send_to_resource_export_dialog('CMD_SET_CUSTOM_RESOURCE_NAME', custom_name)

def click_generate_button(close_notification = 1):
    logging.info('click generate button')
    post_to_resource_export_dialog('CMD_GENERATE', 0)
    wait_dialog_close(RESOURCE_EXPORT_DIALOG)
 
    if close_notification:
        close_message_dialog()
    
def cancel_resource_generation():
    logging.info('cance resource generation')
    post_to_resource_export_dialog('CMD_CANCEL', 0)
    wait_dialog_close(RESOURCE_EXPORT_DIALOG)
    
#================================================================#
#          Send Commands to Configure Theme Dialog               #
#================================================================#   
configure_theme_dialog_commands = [
    '',
    'CMD_SELECT_DISPLAY_NAME',
    'CMD_ADD_THEME',
    'CMD_DELETE_THEME',
    'CMD_SET_ACTIVE_THEME',
    'CMD_SELECT_THEME_INDEX',
    'CMD_SET_THEME_NAME',
    'CMD_EDIT_PALETTE',
    'CMD_CANCEL_THEME_CONFIGURE',
    'CMD_SAVE_THEME_CONFIGURE'
]

def send_to_configure_theme_dialog(cmd, param):
    handle = studio_dialogs[CONFIGURE_THEME_DIALOG]
    command = configure_theme_dialog_commands.index(cmd)
    return(send_to_studio(handle, command, param))
    
def post_to_configure_theme_dialog(cmd, param):
    handle = studio_dialogs[CONFIGURE_THEME_DIALOG]
    command = configure_theme_dialog_commands.index(cmd)
    return(post_to_studio(handle, command, param))
    
def select_display_name(display_name):
    logging.info("select display name: %s", display_name)
    send_to_configure_theme_dialog('CMD_SELECT_DISPLAY_NAME', display_name)
    
def add_theme():
    logging.info("increate theme number")
    send_to_configure_theme_dialog('CMD_ADD_THEME', 0)
    
def delete_theme():
    logging.info("decrease theme number")
    send_to_configure_theme_dialog('CMD_DELETE_THEME', 0)
    
def set_active_theme(theme_name):
    logging.info("set active theme: %s", theme_name)
    send_to_configure_theme_dialog('CMD_SET_ACTIVE_THEME', theme_name)
    
def select_theme_index(index):
    logging.info("select theme index: %d", index)
    send_to_configure_theme_dialog('CMD_SELECT_THEME_INDEX', index)

def post_select_theme_index(index):
    logging.info("post select theme index: %d", index)
    post_to_configure_theme_dialog('CMD_SELECT_THEME_INDEX', index)
    
def set_theme_name(theme_name):
    logging.info("set theme name: %s", theme_name)
    send_to_configure_theme_dialog('CMD_SET_THEME_NAME', theme_name)
    
def edit_palette():
    logging.info("edit palette")
    post_to_configure_theme_dialog('CMD_EDIT_PALETTE', 0)
    wait_dialog_open(EDIT_PALETTE_DIALOG, studio_dialogs[CONFIGURE_THEME_DIALOG])
    
def cancel_theme_configure():
    logging.info("cancel theme configure")
    post_to_configure_theme_dialog('CMD_CANCEL_THEME_CONFIGURE', 0)
    wait_dialog_close(CONFIGURE_THEME_DIALOG)
    
def save_theme_configure(wait_close = CLOSE_WAIT):
    logging.info("save theme configure")
    post_to_configure_theme_dialog('CMD_SAVE_THEME_CONFIGURE', 0)
    
    if wait_close == CLOSE_WAIT:
        wait_dialog_close(CONFIGURE_THEME_DIALOG)

#================================================================#
#          Send Commands to Edit palette                         #
#================================================================#  
palette_layout_dialog_commands = [
    '',
    'CMD_SET_PREDEFINED_PALETTE_ENTRY',
    'CMD_SET_PALETTE_COLOR',
    'CMD_SAVE_PALETTE_EDIT',
    'CMD_CANCEL_PALETTE_EDIT'
]

def send_to_palette_layout_dialog(cmd, param):
    handle = studio_dialogs[EDIT_PALETTE_DIALOG]
    command = palette_layout_dialog_commands.index(cmd)
    return(send_to_studio(handle, command, param))
    
def post_to_palette_layout_dialog(cmd, param):
    handle = studio_dialogs[EDIT_PALETTE_DIALOG]
    command = palette_layout_dialog_commands.index(cmd)
    return(post_to_studio(handle, command, param))

def set_predefined_palette_entry(count):
    logging.info("set predefined palette entry to %d", count)
    send_to_palette_layout_dialog('CMD_SET_PREDEFINED_PALETTE_ENTRY', count)
    
def set_palette_color(index, red, green, blue):
    color = (red << 16) | (green << 8) | blue
    logging.info("set color[%d] to %x", index, color)
    param = str(index) + ',' + str(color)
    send_to_palette_layout_dialog('CMD_SET_PALETTE_COLOR', param)
    
def save_palette_edit():
    logging.info("save palette edit")
    post_to_palette_layout_dialog('CMD_SAVE_PALETTE_EDIT', 0)
    wait_dialog_close(EDIT_PALETTE_DIALOG, studio_dialogs[CONFIGURE_THEME_DIALOG])
    
def cancel_palette_edit():
    logging.info("cancel palette edit")
    post_to_palette_layout_dialog('CMD_CANCEL_PALETTE_EDIT', 0)
    wait_dialog_close(EDIT_PALETTE_DIALOG, studio_dialogs[CONFIGURE_THEME_DIALOG])
    
#================================================================#
#          Send Commands to String Scroll Wheel Edit Dialog      #
#================================================================#  
string_scroll_wheel_edit_dialog_commands = [
    '',
    'CMD_SET_STRING',
    'CMD_SAVE',
    'CMD_CANCEL'
]

def send_to_string_scroll_wheel_edit_dialog(cmd, param):
    handle = studio_dialogs[STRING_SCROLL_WHEEL_EDIT_DIALOG]
    command = string_scroll_wheel_edit_dialog_commands.index(cmd)
    return(send_to_studio(handle, command, param))
    
def post_to_string_scroll_wheel_edit_dialog(cmd, param):
    handle = studio_dialogs[STRING_SCROLL_WHEEL_EDIT_DIALOG]
    command = string_scroll_wheel_edit_dialog_commands.index(cmd)
    return(post_to_studio(handle, command, param))
    
def edit_string_scroll_wheel_string(id, string):
    logging.info("edit string list: row = %d, string = %s", id, string)
    param = str(id) + ',' + string
    send_to_string_scroll_wheel_edit_dialog('CMD_SET_STRING', param)
    
def cancel_string_scroll_wheel_edit():
    logging.info("cancel string scroll wheel edit")
    post_to_string_scroll_wheel_edit_dialog('CMD_CANCEL', 0)
    wait_dialog_close(STRING_SCROLL_WHEEL_EDIT_DIALOG)
    
def save_string_scroll_wheel_edit():
    logging.info("save string scroll wheel edit")
    post_to_string_scroll_wheel_edit_dialog('CMD_SAVE', 0)
    wait_dialog_close(STRING_SCROLL_WHEEL_EDIT_DIALOG)

#================================================================#
#          Send Commands to String Scroll Wheel Edit Dialog      #
#================================================================#  
string_scroll_wheel_edit_dialog_commands = [
    '',
    'CMD_SET_STRING',
    'CMD_SAVE',
    'CMD_CANCEL'
]

def send_to_string_scroll_wheel_edit_dialog(cmd, param):
    handle = studio_dialogs[STRING_SCROLL_WHEEL_EDIT_DIALOG]
    command = string_scroll_wheel_edit_dialog_commands.index(cmd)
    return(send_to_studio(handle, command, param))
    
def post_to_string_scroll_wheel_edit_dialog(cmd, param):
    handle = studio_dialogs[STRING_SCROLL_WHEEL_EDIT_DIALOG]
    command = string_scroll_wheel_edit_dialog_commands.index(cmd)
    return(post_to_studio(handle, command, param))
    
def edit_string_scroll_wheel_string(id, string):
    logging.info("edit string list: row = %d, string = %s", id, string)
    param = str(id) + ',' + string
    send_to_string_scroll_wheel_edit_dialog('CMD_SET_STRING', param)
    
def cancel_string_scroll_wheel_edit():
    logging.info("cancel string scroll wheel edit")
    post_to_string_scroll_wheel_edit_dialog('CMD_CANCEL', 0)
    wait_dialog_close(STRING_SCROLL_WHEEL_EDIT_DIALOG)
    
def save_string_scroll_wheel_edit():
    logging.info("save string scroll wheel edit")
    post_to_string_scroll_wheel_edit_dialog('CMD_SAVE', 0)
    wait_dialog_close(STRING_SCROLL_WHEEL_EDIT_DIALOG)

#================================================================#
#          Send Commands to String Scroll Wheel Edit Dialog      #
#================================================================#  
string_scroll_wheel_edit_dialog_commands = [
    '',
    'CMD_SET_STRING',
    'CMD_SAVE',
    'CMD_CANCEL'
]

def send_to_string_scroll_wheel_edit_dialog(cmd, param):
    handle = studio_dialogs[STRING_SCROLL_WHEEL_EDIT_DIALOG]
    command = string_scroll_wheel_edit_dialog_commands.index(cmd)
    return(send_to_studio(handle, command, param))
    
def post_to_string_scroll_wheel_edit_dialog(cmd, param):
    handle = studio_dialogs[STRING_SCROLL_WHEEL_EDIT_DIALOG]
    command = string_scroll_wheel_edit_dialog_commands.index(cmd)
    return(post_to_studio(handle, command, param))
    
def edit_string_scroll_wheel_string(id, string):
    logging.info("edit string list: row = %d, string = %s", id, string)
    param = str(id) + ',' + string
    send_to_string_scroll_wheel_edit_dialog('CMD_SET_STRING', param)
    
def cancel_string_scroll_wheel_edit():
    logging.info("cancel string scroll wheel edit")
    post_to_string_scroll_wheel_edit_dialog('CMD_CANCEL', 0)
    wait_dialog_close(STRING_SCROLL_WHEEL_EDIT_DIALOG)
    
def save_string_scroll_wheel_edit():
    logging.info("save string scroll wheel edit")
    post_to_string_scroll_wheel_edit_dialog('CMD_SAVE', 0)
    wait_dialog_close(STRING_SCROLL_WHEEL_EDIT_DIALOG)

#================================================================#
#          Send Commands to Project Import Dialog                #
#================================================================#
project_import_dialog_commands = [
    '',
    'CMD_IMPORT',
    'CMD_CANCEL',
    'CMD_CHECK_SCREEN'
]

def send_to_project_import_dialog(cmd, param):
    handle = studio_dialogs[IMPORT_PROJECT_DIALOG]
    command = project_import_dialog_commands.index(cmd)
    return(send_to_studio(handle, command, param))
    
def post_to_project_import_dialog(cmd, param):
    handle = studio_dialogs[IMPORT_PROJECT_DIALOG]
    command = project_import_dialog_commands.index(cmd)
    return(post_to_studio(handle, command, param))
    
def check_import_screen(parent_name, screen_name, check):
    if check:
        logging.info('check on %s/%s', parent_name, screen_name)
    else:
        logging.info('check off %s/%s', parent_name, screen_name)
        
    param = parent_name + ','
    param += screen_name + ','
    param += str(check)
    
    send_to_project_import_dialog('CMD_CHECK_SCREEN', param)
    
def cancel_project_import_dialog(wait_close = CLOSE_WAIT):
    logging.info("cancel project import")
    post_to_project_import_dialog('CMD_CANCEL', 0)
    if wait_close == CLOSE_WAIT:
        wait_dialog_close(IMPORT_PROJECT_DIALOG)
    
def save_project_import_dialog(wait_close = CLOSE_WAIT):
    logging.info("save project import")
    post_to_project_import_dialog('CMD_IMPORT', 0)
    if wait_close == CLOSE_WAIT:
        wait_dialog_close(IMPORT_PROJECT_DIALOG)
    
#================================================================#
#                Send Commands to Option Dialog                  #
#================================================================#
option_dialog_commands = [
    '',
    'CMD_SELECT_OPTION'
]

def post_to_option_dialog(cmd, param):
    handle = studio_dialogs[OPTION_DIALOG]
    command = option_dialog_commands.index(cmd)
    return(post_to_studio(handle, command, param))
    
def select_option(option_index):
    wait_dialog_open(OPTION_DIALOG, studio_windows['main_frame'])
    logging.info("select option %d", option_index)
    post_to_option_dialog('CMD_SELECT_OPTION', option_index)
    wait_dialog_close(OPTION_DIALOG, studio_windows['main_frame'])
    

#================================================================#
#          Send Commands to Screen Flow Edit Dialog              #
#================================================================#
screen_flow_edit_dialog_commands = [
    '',
    'CMD_EDIT_TRIGGER_LIST',
    'CMD_SAVE_SCREEN_FLOW_EDIT',
    'CMD_CANCEL_SCREEN_FLOW_EDIT'
]

def send_to_screen_flow_edit_dialog(cmd, param):
    handle = studio_dialogs[SCREEN_FLOW_EDIT_DIALOG]
    command = screen_flow_edit_dialog_commands.index(cmd)
    return(send_to_studio(handle, command, param))
    
def post_to_screen_flow_edit_dialog(cmd, param):
    handle = studio_dialogs[SCREEN_FLOW_EDIT_DIALOG]
    command = screen_flow_edit_dialog_commands.index(cmd)
    return(post_to_studio(handle, command, param))
    
def edit_trigger_list(screen_name):
    global selected_screen_name;
    selected_screen_name = screen_name;
    logging.info('edit trigger list: %s', screen_name)
    post_to_screen_flow_edit_dialog('CMD_EDIT_TRIGGER_LIST', screen_name)
    wait_dialog_open(TRIGGER_LIST_EDIT_DIALOG, studio_dialogs[SCREEN_FLOW_EDIT_DIALOG])
    
def cancel_screen_flow_edit_dialog():
    logging.info("cancel screen flow edit")
    post_to_screen_flow_edit_dialog('CMD_CANCEL_SCREEN_FLOW_EDIT', 0)
    wait_dialog_close(SCREEN_FLOW_EDIT_DIALOG)
    
def save_screen_flow_edit_dialog():
    logging.info("save screen flow edit")
    post_to_screen_flow_edit_dialog('CMD_SAVE_SCREEN_FLOW_EDIT', 0)
    wait_dialog_close(SCREEN_FLOW_EDIT_DIALOG)

#================================================================#
#          Send Commands to Trigger List Edit Dialog             #
#================================================================#
trigger_list_edit_dialog_commands = [
    '',
    'CMD_ADD_TRIGGER',
    'CMD_DELETE_TRIGGER',
    'CMD_EDIT_TRIGGER',
    'CMD_EDIT_ACTION',
    'CMD_SELETE_ROW',
    'CMD_SAVE_TRIGGER_LIST_EDIT',
    'CMD_CANCEL_TRIGGER_LIST_EDIT'
]

def send_to_trigger_list_edit_dialog(cmd, param):
    handle = studio_dialogs[TRIGGER_LIST_EDIT_DIALOG]
    command = trigger_list_edit_dialog_commands.index(cmd)
    return(send_to_studio(handle, command, param))
    
def post_to_trigger_list_edit_dialog(cmd, param):
    handle = studio_dialogs[TRIGGER_LIST_EDIT_DIALOG]
    command = trigger_list_edit_dialog_commands.index(cmd)
    return(post_to_studio(handle, command, param))
    
def add_trigger():
    logging.info('add trigger')
    post_to_trigger_list_edit_dialog('CMD_ADD_TRIGGER', 0)
    global TRIGGER_EDIT_DIALOG
    TRIGGER_EDIT_DIALOG = 'Add Trigger for [' + selected_screen_name +']'
    wait_dialog_open(TRIGGER_EDIT_DIALOG, studio_dialogs[TRIGGER_LIST_EDIT_DIALOG])
    
def delete_trigger():
    logging.info('delete trigger')
    send_to_trigger_list_edit_dialog('CMD_DELETE_TRIGGER', 0)

def edit_trigger():
    logging.info('edit trigger')
    post_to_trigger_list_edit_dialog('CMD_EDIT_TRIGGER', 0)
    global TRIGGER_EDIT_DIALOG
    TRIGGER_EDIT_DIALOG = 'Edit Trigger for [' + selected_screen_name + ']'
    wait_dialog_open(TRIGGER_EDIT_DIALOG, studio_dialogs[TRIGGER_LIST_EDIT_DIALOG])

def edit_action(trigger_name):
    logging.info('edit action')
    post_to_trigger_list_edit_dialog('CMD_EDIT_ACTION', 0)
    global TRIGGER_ACTION_EDIT_DIALOG
    TRIGGER_ACTION_EDIT_DIALOG = 'Edit Actions for Trigger [on_' + trigger_name + ']'
    wait_dialog_open(TRIGGER_ACTION_EDIT_DIALOG, studio_dialogs[TRIGGER_LIST_EDIT_DIALOG])
    
def select_trigger_list_row(row):
    logging.info('selete trigger list row: %d', row)
    send_to_trigger_list_edit_dialog('CMD_SELETE_ROW', row)
    
def cancel_trigger_list_edit_dialog():
    logging.info("cancel trigger list edit")
    post_to_trigger_list_edit_dialog('CMD_CANCEL_TRIGGER_LIST_EDIT', 0)
    wait_dialog_close(TRIGGER_LIST_EDIT_DIALOG)
    
def save_trigger_list_edit_dialog():
    logging.info("save trigger list edit")
    post_to_trigger_list_edit_dialog('CMD_SAVE_TRIGGER_LIST_EDIT', 0)
    wait_dialog_close(TRIGGER_LIST_EDIT_DIALOG)

#================================================================#
#          Send Commands to Trigger Edit Dialog                  #
#================================================================#
trigger_edit_dialog_commands = [
    '',
    'CMD_SET_TRIGGER_TYPE',
    'CMD_SET_EVENT_TYPE',
    'CMD_SAVE_TRIGGER_EDIT',
    'CMD_CANCEL_TRIGGER_EDIT'
]

def send_to_trigger_edit_dialog(cmd, param):
    handle = studio_dialogs[TRIGGER_EDIT_DIALOG]
    command = trigger_edit_dialog_commands.index(cmd)
    return(send_to_studio(handle, command, param))
    
def post_to_trigger_edit_dialog(cmd, param):
    handle = studio_dialogs[TRIGGER_EDIT_DIALOG]
    command = trigger_edit_dialog_commands.index(cmd)
    return(post_to_studio(handle, command, param))
    
def set_trigger_type(type_string):
    logging.info('set trigger type to %s', type_string)
    send_to_trigger_edit_dialog('CMD_SET_TRIGGER_TYPE', trigger_types[type_string])
    
def set_event_type(type_string):
    logging.info('set event type to \"%s\"', type_string)
    send_to_trigger_edit_dialog('CMD_SET_EVENT_TYPE', type_string)

def cancel_trigger_edit_dialog():
    logging.info("cancel trigger edit dialog")
    post_to_trigger_edit_dialog('CMD_CANCEL_TRIGGER_EDIT', 0)
    wait_dialog_close(TRIGGER_EDIT_DIALOG)
    
def save_trigger_edit_dialog(wait = CLOSE_WAIT):
    logging.info("save trigger edit dialog")
    post_to_trigger_edit_dialog('CMD_SAVE_TRIGGER_EDIT', 0)
    
    if wait == CLOSE_WAIT:
        wait_dialog_close(TRIGGER_EDIT_DIALOG)
        
#================================================================#
#      Send Commands to Trigger Action Edit Dialog               #
#================================================================#
trigger_action_edit_dialog_commands = [
    '',
    'CMD_ADD_ACTION',
    'CMD_SELECT_ACTION',
    'CMD_DELETE_ACTION',
    'CMD_SELECT_TARGET',
    'CMD_SELECT_PARENT',
    'CMD_TARGET_SHOW_CHILD_WIDGETS',
    'CMD_PARENT_SHOW_CHILD_WIDGETS',
    'CMD_EDIT_ACTION_NAME',
    'CMD_EDIT_ANIMATION_INFO',
    'CMD_CHECK_DETACH_TARGET',
    'CMD_CONFIGURE_EASING_FUNCTION',
    'CMD_SAVE_TRIGGER_ACTION_EDIT',
    'CMD_CANCEL_TRIGGER_ACTION_EDIT'
]

def send_to_trigger_action_edit_dialog(cmd, param):
    handle = studio_dialogs[TRIGGER_ACTION_EDIT_DIALOG]
    command = trigger_action_edit_dialog_commands.index(cmd)
    return(send_to_studio(handle, command, param))
    
def post_to_trigger_action_edit_dialog(cmd, param):
    handle = studio_dialogs[TRIGGER_ACTION_EDIT_DIALOG]
    command = trigger_action_edit_dialog_commands.index(cmd)
    return(post_to_studio(handle, command, param))

def add_action(action_type):
    logging.info('set action type to %s', action_type)
    post_to_trigger_action_edit_dialog('CMD_ADD_ACTION', 0)
    wait_dialog_open(TRIGGER_ACTION_SELECT_DIALOG, studio_dialogs[TRIGGER_ACTION_EDIT_DIALOG])
    action_select_dlg_select_action(action_type)
    save_action_select_dialog()
    
def select_action(action_name):
    logging.info('select action: %s', action_name)
    send_to_trigger_action_edit_dialog('CMD_SELECT_ACTION', action_name)
    
#def select_action()
def delete_action():
    logging.info('delete current selected action')
    send_to_trigger_action_edit_dialog('CMD_DELETE_ACTION', 0)
    
def select_target(target_name):
    logging.info('select target \"%s\"', target_name)
    send_to_trigger_action_edit_dialog('CMD_SELECT_TARGET', target_name)
    
def select_parent(parent_name):
    logging.info('select parent \"%s\"', parent_name)
    send_to_trigger_action_edit_dialog('CMD_SELECT_PARENT', parent_name)

def check_target_show_child_widgets(check):
    if check:
        logging.info('check on target show child widgets')
    else:
        logging.info('check off target show child widgets')
        
    send_to_trigger_action_edit_dialog('CMD_TARGET_SHOW_CHILD_WIDGETS', check)

def check_parent_show_child_widgets(check):
    if check:
        logging.info('check on parent show child widgets')
    else:
        logging.info('check off parent show child widgets')
        
    send_to_trigger_action_edit_dialog('CMD_PARENT_SHOW_CHILD_WIDGETS', check)

def edit_action_name(name):
    logging.info('set action name to \"%s\"', name)
    send_to_trigger_action_edit_dialog('CMD_EDIT_ACTION_NAME', name)
    
def edit_animation_info(control_id, string):
    control_id = 'ID_' + control_id
    logging.info('set %s = %s', control_id, string)
    param = str(animation_param_ids[control_id]) + ',' + string
    send_to_trigger_action_edit_dialog('CMD_EDIT_ANIMATION_INFO', param)
    
def check_detach_target(check):
    if check:
        logging.info('check on detach target')
    else:
        logging.info('check off detach target')
        
    send_to_trigger_action_edit_dialog('CMD_CHECK_DETACH_TARGET', check)

def configure_easing_function():
    logging.info('click on easing function button')
    post_to_trigger_action_edit_dialog('CMD_CONFIGURE_EASING_FUNCTION', 0)
    wait_dialog_open(EASING_FUNCTION_SELECT_DIALOG, studio_dialogs[TRIGGER_ACTION_EDIT_DIALOG])
    
def cancel_trigger_action_edit_dialog():
    logging.info("cancel trigger action edit dialog")
    post_to_trigger_action_edit_dialog('CMD_CANCEL_TRIGGER_ACTION_EDIT', 0)
    wait_dialog_close(TRIGGER_ACTION_EDIT_DIALOG)
    
def save_trigger_action_edit_dialog(wait = CLOSE_WAIT):
    logging.info("save trigger action edit dialog")
    post_to_trigger_action_edit_dialog('CMD_SAVE_TRIGGER_ACTION_EDIT', 0)
    
    if wait == CLOSE_WAIT:
        wait_dialog_close(TRIGGER_ACTION_EDIT_DIALOG)

#================================================================#
#      Send Commands to Trigger Action Select Dialog             #
#================================================================#
action_select_dialog_commands = [
    '',
    'CMD_SELECT_ACTION',
    'CMD_SAVE_ACTION_SELECT',
    'CMD_CANCEL_ACTION_SELECT'
]

def send_to_action_select_dialog(cmd, param):
    handle = studio_dialogs[TRIGGER_ACTION_SELECT_DIALOG]
    command = action_select_dialog_commands.index(cmd)
    return(send_to_studio(handle, command, param))
    
def post_to_action_select_dialog(cmd, param):
    handle = studio_dialogs[TRIGGER_ACTION_SELECT_DIALOG]
    command = action_select_dialog_commands.index(cmd)
    return(post_to_studio(handle, command, param))

def action_select_dlg_select_action(type):
    logging.info('select action type: %s', type)
    send_to_action_select_dialog('CMD_SELECT_ACTION', type)
    
def cancel_action_select_dialog():
    logging.info("cancel action select dialog")
    post_to_action_select_dialog('CMD_CANCEL_ACTION_SELECT', 0)
    wait_dialog_close(TRIGGER_ACTION_SELECT_DIALOG)
    
def save_action_select_dialog():
    logging.info("save action select dialog")
    post_to_action_select_dialog('CMD_SAVE_ACTION_SELECT', 0)
    wait_dialog_close(TRIGGER_ACTION_SELECT_DIALOG)
    
#================================================================#
#      Send Commands to Easing Function Select Dialog            #
#================================================================#
easing_function_select_dialog_commands = [
    '',
    'CMD_SELECT_EASING_FUNCTION',
    'CMD_SAVE_EASING_FUNCTION_SELECT',
    'CMD_CANCEL_EASING_FUNCTION_SELECT'
]

def send_to_easing_function_select_dialog(cmd, param):
    handle = studio_dialogs[EASING_FUNCTION_SELECT_DIALOG]
    command = easing_function_select_dialog_commands.index(cmd)
    return(send_to_studio(handle, command, param))
    
def post_to_easing_function_select_dialog(cmd, param):
    handle = studio_dialogs[EASING_FUNCTION_SELECT_DIALOG]
    command = easing_function_select_dialog_commands.index(cmd)
    return(post_to_studio(handle, command, param))

def select_easing_function(name):
    logging.info('select easing function: %s', name)
    send_to_easing_function_select_dialog('CMD_SELECT_EASING_FUNCTION', name)
    
def cancel_easing_function_select_dialog():
    logging.info("cancel easing function select dialog")
    post_to_easing_function_select_dialog('CMD_CANCEL_EASING_FUNCTION_SELECT', 0)
    wait_dialog_close(EASING_FUNCTION_SELECT_DIALOG)
    
def save_easing_function_select_dialog():
    logging.info("save easing function select dialog")
    post_to_easing_function_select_dialog('CMD_SAVE_EASING_FUNCTION_SELECT', 0)
    wait_dialog_close(EASING_FUNCTION_SELECT_DIALOG)
    
#================================================================#
#      Send Commands to Synergy Settings Dialog                  #
#================================================================#
synergy_settings_dialog_commands = [
    '',
    'CMD_CHECK_2D_DRAWING_ENGINE',
    'CMD_SAVE_SYNERGY_SETTING',
    'CMD_CANCEL_SYNERGY_SETTING'
]

def send_to_synergy_settings_dialog(cmd, param):
    handle = studio_dialogs[SYNERGY_SETTINGS_DIALOG]
    command = synergy_settings_dialog_commands.index(cmd)
    return(send_to_studio(handle, command, param))
    
def post_to_synergy_settings_dialog(cmd, param):
    handle = studio_dialogs[SYNERGY_SETTINGS_DIALOG]
    command = synergy_settings_dialog_commands.index(cmd)
    return(post_to_studio(handle, command, param))

def check_2d_drawing_engine(enabled):
    logging.info('check 2d drawing engine: %d', enabled)
    send_to_synergy_settings_dialog('CMD_CHECK_2D_DRAWING_ENGINE', enabled)
    
def cancel_synergy_settings_dialog():
    logging.info("cancel synergy settings dialog")
    post_to_synergy_settings_dialog('CMD_CANCEL_SYNERGY_SETTING', 0)
    wait_dialog_close(EASING_FUNCTION_SELECT_DIALOG, studio_dialogs[CONFIGURE_PROJECT_DIALOG])
    
def save_synergy_settings_dialog():
    logging.info("save synergy settings dialog")
    post_to_synergy_settings_dialog('CMD_SAVE_SYNERGY_SETTING', 0)
    wait_dialog_close(EASING_FUNCTION_SELECT_DIALOG, studio_dialogs[CONFIGURE_PROJECT_DIALOG])
    
#================================================================#
#              Send Commands to Rich Text Edit Dialog            #
#================================================================#  
rich_text_edit_dialog_commands = [
    '',
    'CMD_SET_STRING',
    'CMD_SAVE',
    'CMD_CANCEL'
]

def send_to_rich_text_edit_dialog(cmd, param):
    handle = studio_dialogs[RICH_TEXT_EDIT_DIALOG]
    command = rich_text_edit_dialog_commands.index(cmd)
    return(send_to_studio(handle, command, param))
    
def post_to_rich_text_edit_dialog(cmd, param):
    handle = studio_dialogs[RICH_TEXT_EDIT_DIALOG]
    command = rich_text_edit_dialog_commands.index(cmd)
    return(post_to_studio(handle, command, param))
    
def edit_rich_text_view_string(string):
    logging.info("edit rich text view string: %s", string)
    send_to_rich_text_edit_dialog('CMD_SET_STRING', string)
    
def cancel_rich_text_edit():
    logging.info("cancel rich text edit")
    post_to_rich_text_edit_dialog('CMD_CANCEL', 0)
    wait_dialog_close(RICH_TEXT_EDIT_DIALOG)
    
def save_rich_text_edit():
    logging.info("save rich text edit")
    post_to_rich_text_edit_dialog('CMD_SAVE', 0)
    wait_dialog_close(RICH_TEXT_EDIT_DIALOG)

#================================================================#
#          Send Commands to Edit Sprite Frames Dialog            #
#================================================================#  
sprite_frame_edit_dialog_commands = [
    '',
    'CMD_EDIT_FRAME',
    'CMD_EDIT_NUM_FRAMES',
    'CMD_SET_ALPHA',
    'CMD_SET_XOFFSET',
    'CMD_SET_YOFFSET',
    'CMD_SET_DELAY',
    'CMD_CHECK_APPLY_TO_ALL_FRAMES',
    'CMD_IMPORT_FRAMES',
    'CMD_SAVE',
    'CMD_CANCEL'
]

def send_to_sprite_frame_edit_dialog(cmd, param):
    handle = studio_dialogs[SPRITE_FRAME_EDIT_DIALOG]
    command = sprite_frame_edit_dialog_commands.index(cmd)
    return(send_to_studio(handle, command, param))
    
def post_to_sprite_frame_edit_dialog(cmd, param):
    handle = studio_dialogs[SPRITE_FRAME_EDIT_DIALOG]
    command = sprite_frame_edit_dialog_commands.index(cmd)
    return(post_to_studio(handle, command, param))
    
def edit_sprite_frame(frame_id, pixelmap_string):
    logging.info("edit sprite frame: (%d, %s)", frame_id, pixelmap_string)
    string = str(frame_id) + ',' + pixelmap_string
    send_to_sprite_frame_edit_dialog('CMD_EDIT_FRAME', string)

def edit_sprite_total_frames(frame_count):
    logging.info("edit sprite total frames: %d", frame_count)
    send_to_sprite_frame_edit_dialog('CMD_EDIT_NUM_FRAMES', frame_count)

def edit_sprite_frame_alpha(value):
    logging.info("edit sprite frame alpha: %d", value)
    send_to_sprite_frame_edit_dialog('CMD_SET_ALPHA', value)
    
def edit_sprite_frame_xoffset(xoffset):
    logging.info("edit sprite frame x offset: %d", xoffset)
    send_to_sprite_frame_edit_dialog('CMD_SET_XOFFSET', xoffset)
    
def edit_sprite_frame_yoffset(yoffset):
    logging.info("edit sprite frame y offset: %d", yoffset)
    send_to_sprite_frame_edit_dialog('CMD_SET_YOFFSET', yoffset)
    
def edit_sprite_frame_delay(delay):
    logging.info("edit sprite frame delay: %d", delay)
    send_to_sprite_frame_edit_dialog('CMD_SET_DELAY', delay)

def check_apply_to_all_frames(check):
    logging.info("check apply to all frames: %d", check)
    send_to_sprite_frame_edit_dialog('CMD_CHECK_APPLY_TO_ALL_FRAMES', check)
    
def import_sprite_frames():
    logging.info("import sprite frames")
    post_to_sprite_frame_edit_dialog('CMD_IMPORT_FRAMES', 0)
    wait_dialog_open(SPRITE_FRAME_IMPORT_DIALOG)
    
def cancel_sprite_frame_edit():
    logging.info("cancel sprite frame edit")
    post_to_sprite_frame_edit_dialog('CMD_CANCEL', 0)
    wait_dialog_close(SPRITE_FRAME_EDIT_DIALOG)
    
def save_sprite_frame_edit():
    logging.info("save sprite frame edit")
    post_to_sprite_frame_edit_dialog('CMD_SAVE', 0)
    wait_dialog_close(SPRITE_FRAME_EDIT_DIALOG)

#================================================================#
#          Send Commands to Import Sprite Frames Dialog          #
#================================================================#  
sprite_frame_import_dialog_commands = [
    '',
    'CMD_SAVE',
    'CMD_CANCEL'
]

def send_to_sprite_frame_import_dialog(cmd, param):
    handle = studio_dialogs[SPRITE_FRAME_IMPORT_DIALOG]
    command = sprite_frame_import_dialog_commands.index(cmd)
    return(send_to_studio(handle, command, param))
    
def post_to_sprite_frame_import_dialog(cmd, param):
    handle = studio_dialogs[SPRITE_FRAME_IMPORT_DIALOG]
    command = sprite_frame_import_dialog_commands.index(cmd)
    return(post_to_studio(handle, command, param))
    
def cancel_sprite_frame_import_edit():
    logging.info("cancel sprite frame import")
    post_to_sprite_frame_import_dialog('CMD_CANCEL', 0)
    wait_dialog_close(SPRITE_FRAME_IMPORT_DIALOG)
    
def save_sprite_frame_import_edit():
    logging.info("save sprite frame import")
    post_to_sprite_frame_import_dialog('CMD_SAVE', 0)
    wait_dialog_close(SPRITE_FRAME_IMPORT_DIALOG)