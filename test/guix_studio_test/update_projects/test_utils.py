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

user32 = ctypes.windll.user32

studio_exe_path = "../../../guix_studio/Release/guix_studio.exe"
top_windows = []
WM_CLOSE = 0x0010
reset_map_format = False

props_ctrl_ids = {}
widget_types = {}
color_formats = {}
font_formats = {}
group_ids = {}
folder_ids = {}
resource_item_types = {}
palette_types = {}
border_types = {}

#dictionary of studio window name : handle
studio_windows = {
    'main_frame':0,
    'properties_win':0,
    'project_view':0,
    'target_screen':0,
    'target_view':0,
    'resource_view':0,
    }

CREATE_NEW_PROJECT_DIALOG = 'create_new_project'
CONFIGURE_PROJECT_DIALOG = 'configure_project'
CONFIGURE_THEME_DIALOG = 'configure_themes'
CONFIGURE_LANGUAGE_DIALOG = 'configure_languages'
EDIT_FONT_DIALOG = 'edit_font'
EDIT_COLOR_DIALOG = 'edit_color'
EDIT_PIXELMAP_DIALOG = 'edit_pixelmap'
STRING_TABLE_EDITOR_DIALOG = 'string_table_editor'
XLIFF_EXPORT_CONTROL_DIALOG = 'xliff_export_control'
NOTIFICATION_DIALOG = 'message_dialog'
ERROR_DIALOG = 'Error'
RECORD_MACRO_DIALOG = 'record_macro'
PLAYBACK_MACRO_DIALOG = 'playback_macro'
FOLDER_NAME_DIALOG = 'edit_folder_name'
GRID_SNAP_SETTING_DIALOG = 'grid_snap_setting'
RESOURCE_EXPORT_DIALOG = 'resource_export_dlg'

#dictionary of studio dialog name: handle    
studio_dialogs = {
    CREATE_NEW_PROJECT_DIALOG:0,
    CONFIGURE_PROJECT_DIALOG:0,
    CONFIGURE_THEME_DIALOG:0,
    CONFIGURE_LANGUAGE_DIALOG:0,
    EDIT_FONT_DIALOG:0,
    EDIT_COLOR_DIALOG:0,
    EDIT_PIXELMAP_DIALOG:0,
    STRING_TABLE_EDITOR_DIALOG:0,
    XLIFF_EXPORT_CONTROL_DIALOG:0,
    NOTIFICATION_DIALOG:0,
    ERROR_DIALOG:0,
    RECORD_MACRO_DIALOG:0,
    PLAYBACK_MACRO_DIALOG:0,
    FOLDER_NAME_DIALOG:0,
    GRID_SNAP_SETTING_DIALOG:0,
    RESOURCE_EXPORT_DIALOG:0
}

STUDIO_TEST_MESG = 0x0402
studio_test_index = 0
verbose = True
DEFAULT_OUTPUT_FILE_PATH = "output_files/"
DEFAULT_GOLDEN_FILE_PATH = 'golden_files/'
golden_file_path = ""
test_name = ""
gen_golden_file = False
gen_screenshot = False
good_results = []  
result_count = [0, 0]
total_failures = 0
CLOSE_WAIT = 0
CLOSE_NO_WAIT = 1

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
        
    os.system(msbuild_exe_path + " " + project_sln_path + " /p:Configuration=Release" + " /p:Platform=\"Win32\"")
    
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
def compare_output_file(pathname_1, pathname_2):
    logging.info('Comparing: %s and %s', pathname_1, pathname_2)
    
    if not os.path.isfile(pathname_1):
        logging.error("** ERROR ** %s does not exist", pathname_1)
        return False
        
    if not os.path.isfile(pathname_2):
        logging.error("** ERROR ** %s does not exist", pathname_2)
        return False
        
    #open comparing files
    file_1 = open(pathname_1,'r')
    file_2 = open(pathname_2, 'r')    

    list_1 = file_1.readlines()
    list_2 = file_2.readlines()
    
    #find the line where "#include" is 
    start_row_1 = 0
    start_row_2 = 0
    
    for line in list_1:
        if "#include" in line:
            start_row_1 = list_1.index(line)
            break;
            
    for line in list_2:
        if "#include" in line:
            start_row_2 = list_1.index(line)
            break;

    #compare from "#include" line
    list_1 = list_1[start_row_1:]
    list_2 = list_2[start_row_2:]
    
    for line_1,line_2 in zip(list_1,list_2):
        if line_1 != line_2:
            logging.error("Does Not Match:\n%s\n%s" %(line_1, line_2))  
            return False
    
        
    logging.info("Match success.")
    
    return True
  
def compare_xliff_file(pathname_1, pathname_2):
    logging.info('Comparing: %s and %s', pathname_1, pathname_2)
    
    if not os.path.isfile(pathname_1):
        logging.error("** ERROR ** %s does not exist", pathname_1)
        return False
        
    if not os.path.isfile(pathname_2):
        logging.error("** ERROR ** %s does not exist", pathname_2)
        return False
        
    #open comparing files
    file_1 = open(pathname_1, encoding='utf-8')
    file_2 = open(pathname_2, encoding='utf-8')    

    list_1 = file_1.readlines()
    list_2 = file_2.readlines()
    
    #find the line where "#include" is 
    start_row_1 = 0
    start_row_2 = 0
    
    for line in list_1:
        if "<file" in line:
            start_row_1 = list_1.index(line) + 1
            break
            
    for line in list_2:
        if "<file" in line:
            start_row_2 = list_2.index(line) + 1
            break
            
    # compare after <file> tag
    list_1 = list_1[start_row_1:]
    list_2 = list_2[start_row_2:]
    
    for line_1,line_2 in zip(list_1,list_2):
        if line_1 != line_2:
            logging.error("Does Not Match:\n%s\n%s" %(line_1, line_2))  
            return False
        
    logging.info("Match success.")
    
    return True
    
def cmp_files(output_file_lists, compare_func):
    global studio_test_index
        
    if gen_golden_file == True:
        golden_file = open(golden_file_path, "a+")
        if studio_test_index == 0:
            golden_file.write("good_results = [\n")

        #save golden values
        golden_file.write("    0, #test %d, comparing files\n" %(studio_test_index))
            
    else:
        result = True
        for file in output_file_lists:
            golden_file = DEFAULT_GOLDEN_FILE_PATH + file
            output_file = DEFAULT_OUTPUT_FILE_PATH + file
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
        
def cmp_output_files(output_file_lists):
    cmp_files(output_file_lists, compare_output_file)
    
def cmp_normal_files(normal_file_lists):
    cmp_files(normal_file_lists, filecmp.cmp)
        
# run the Studio executable
def run_studio():
    absolute_path = os.path.abspath(studio_exe_path)
    if not os.path.isfile(absolute_path):
        raise Exception("Unable to locate Studio executable.")
    Popen(os.path.abspath(absolute_path))
    time.sleep(5)

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
    child = user32.GetWindow(hwnd, 5)

    while child:
        length = user32.GetWindowTextLengthW(child) + 1
        if length > 1:
            buffer = ctypes.create_unicode_buffer(length)
            user32.GetWindowTextW(child, buffer, length)
            
            if buffer.value in studio_windows:
                studio_windows[buffer.value] = child

        get_window_tree(child)
        child = user32.GetWindow(child, 2)
            
#send a test event to the requested Studio window
def send_to_studio(handle, cmd, param):
    result = 0
    
    if type(param) == str:
        key = winreg.CreateKey(winreg.HKEY_CURRENT_USER, 'Software\\ExpressLogic, Inc.\\GUIX\\Test\\param0')
        winreg.SetValue(key, None, winreg.REG_SZ, param)
        result = user32.SendMessageA(handle, STUDIO_TEST_MESG, cmd, None)
    else:
        result = user32.SendMessageA(handle, STUDIO_TEST_MESG, cmd, param)
    return result
    
def post_to_studio(handle, cmd, param):
    result = 0
    
    if type(param) == str:
        key = winreg.CreateKey(winreg.HKEY_CURRENT_USER, 'Software\\ExpressLogic, Inc.\\GUIX\\Test\\param1')
        winreg.SetValue(key, None, winreg.REG_SZ, param)
        result = user32.PostMessageA(handle, STUDIO_TEST_MESG, cmd, None)
    else:
        result = user32.PostMessageA(handle, STUDIO_TEST_MESG, cmd, param)
    return result

    
# locate the Studio windows and store handles
def find_studio_handles():
    a = ctypes.WINFUNCTYPE(ctypes.c_bool,
                       ctypes.POINTER(ctypes.c_int),
                       ctypes.POINTER(ctypes.c_int))(enum_callback)
    user32.EnumWindows(a, 0)

    found_studio = False

    for hwnd, name in top_windows:
        if 'GUIX Studio' in name:
            print("Found Studio")
            found_studio = True
            studio_windows['main_frame'] = ctypes.c_void_p.from_buffer(hwnd).value
            get_window_tree(hwnd)

    if found_studio:
        for name in studio_windows:
            if studio_windows[name] == 0:
                raise Exception('Unable to find Studio window named: %s' %name)
            else:
                if verbose:
                    print('Found Studio Window: %s' %name)
    return found_studio

# find dialog handle 
def find_dialog(name, owner = 0):
    if owner == 0:
        owner = studio_windows['main_frame']
        
    param = "#32770" + ','
    param += name + ','
    param += str(owner) 

    studio_dialogs[name] = send_to_main_frame('CMD_FIND_DIALOG', param)
    
    return studio_dialogs[name]
    
def close_dialog(name, owner = 0):
    wait_dialog_open(name, owner)
    user32.PostMessageA(studio_dialogs[name], WM_CLOSE, None, None)
    wait_dialog_close(name, owner)

def wait_dialog_open(name, owner = 0):
    time_start = time.time()
    while find_dialog(name, owner) == 0:
        time.sleep(1)
        interval = time.time() - time_start
        if interval > 300:
            raise Exception('Unable to open %s dialog' %name)
            break
            
def wait_dialog_close(name, owner = 0):
    #wait popup dialog close
    time_start = time.time()
    
    while find_dialog(name, owner) != 0:
        time.sleep(1)
        interval = time.time() - time_start
        if interval > 300:
            raise Exception('Unable to close %s dialog' %name)
            break
            
# compare canvas crc32 value with known good value, or
# print value if it's a new index
def compare_result():
    global studio_test_index
    result = get_result()
        
    if result == 0:
        return True    
    
    if gen_golden_file == True:
        golden_file = open(golden_file_path, "a+")
        if studio_test_index == 0:
            golden_file.write("good_results = [\n")
            
        #save golden values
        golden_file.write("    %#x, #test %d\n" %(result, studio_test_index))

        print('Generate CRC32 for test %d' %studio_test_index)
        logging.info("test %d. Golden value: %#x.\n" %(studio_test_index, result)) #print golden values
        studio_test_index += 1
    else: 
        if len(good_results) > studio_test_index and good_results[studio_test_index] != 0:
            #generate screen shot
            if gen_screenshot == True:
                path = os.path.realpath(DEFAULT_OUTPUT_FILE_PATH + "/" + test_name)
                if os.path.exists(path) == False:
                    os.makedirs(path)
                filename = path + "/test_" + str(studio_test_index) + ".bmp"
                generate_screenshot(filename)
        
            if good_results[studio_test_index] == result:            
                print('Test %d passed' %studio_test_index)
                logging.info("Test #%d, compute CRC32 on canvas memory- Passed.\n" %studio_test_index)
                result_count[0] += 1
                studio_test_index += 1
                return True
            else:
                #generate failure screen shots
                path = os.path.realpath("test_failures/")
                if os.path.exists(path) == False:
                    os.makedirs(path)
                filename = path + "/" + test_name +"_" + str(studio_test_index) + ".bmp"
                generate_screenshot(filename)
                
                print('** Test %d failed **' %studio_test_index)
                logging.error("CRC32 failure on test %d. Golden value: %#x  Result Value: %#x\n" %(studio_test_index, good_results[studio_test_index], result))
                result_count[1] += 1
                studio_test_index += 1       
                return False
        else:
            logging.info('Result for index %d is %#x' %(studio_test_index, result))
            studio_test_index += 1
            return True
            

def set_test_results(passed, test_type, error_msg):
    global studio_test_index
        
    if gen_golden_file == True:
        golden_file = open(golden_file_path, "a+")
        if studio_test_index == 0:
            golden_file.write("good_results = [\n")

        golden_file.write("    0, #test %d, %s\n" %(studio_test_index - 1, test_type))
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
    logging.info("*******************************************************")
    logging.info(header_notes)
    logging.info("*                                                     *")    
    logging.info("*******************************************************")
    logging.info("Test date: %s\n", time.asctime())

def setup(generate, screenshot, golden_file):
    test_utils.gen_golden_file = generate
    test_utils.gen_screenshot = screenshot
    
    global studio_test_index
    studio_test_index = 0;
    
    global test_name
    test_name = golden_file.replace("_golden_file", "")
    
    if generate:
        global golden_file_path

        if golden_file is not None:
            golden_file_path = DEFAULT_OUTPUT_FILE_PATH + golden_file + ".py"
        
        #delete old golden file    
        if os.path.exists(golden_file_path):
            os.remove(golden_file_path)

        print("golden file path: %s" %golden_file_path)
    else:
        module = __import__("golden_files.%s" %(golden_file), fromlist=["golden_files"])
        global good_results
        good_results = module.good_results

def write_end(test_name):
    global studio_test_index
    global total_failures
    
    if gen_golden_file == True:
        golden_file = open(golden_file_path, "a+")
        golden_file.write("]")
    else:
        logging.info("********************************************************************")
        logging.info("** Test %s completed." %test_name)
        logging.info('** Tests Passed: %d  Tests Failed %d' %(result_count[0], result_count[1]))
        logging.info("********************************************************************")
        logging.info("\n\n\n")
        print('\nTest %s completed. \nTests Passed: %d\nTests Failed: %d\n' %(test_name, result_count[0], result_count[1]))

        total_failures += result_count[1]
        print('Total Failurs: %d\n' %(total_failures))
        result_count[0] = 0
        result_count[1] = 0
        studio_test_index = 0
  
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
    'CMD_GENERATE_ALL',
    'CMD_GENERATE_RESOURCES',
    'CMD_GET_MENU_STATUS',
    'CMD_GRID_SNAP_SETTING',
    'CMD_FIND_DIALOG'
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
    
def toolbar_paste():
    logging.info('click toolbar button: paste')
    send_to_main_frame('CMD_TOOLBAR_PASTE', 0)
    compare_result()
    time.sleep(1)
    
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
    
def get_menu_status(string):
    return send_to_main_frame('CMD_GET_MENU_STATUS', string)  
    
def grid_snap_setting():
    logging.info('grid and snap setting')
    post_to_main_frame('CMD_GRID_SNAP_SETTING', 0)
    wait_dialog_open(GRID_SNAP_SETTING_DIALOG)
    
#================================================================#
#           Send Commands to Target Screen                       #
#================================================================#
target_screen_test_commands = [
'',
'CMD_GENERATE_CHECKSUM',
'CMD_GENERATE_SCREENSHOT',
'CMD_MOVE_WINDOW',
'CMD_MOVE_SELECTED',
'CMD_ADD_WIDGET',
'CMD_ADD_TEMPLATE',
'CMD_ZOOM_IN',
'CMD_LBUTTON_DOWN',
'CMD_LBUTTON_UP',
'CMD_MOUSEMOVE'
]

#send a test command to the target screen
def send_to_target_screen(cmd, param):
    handle = studio_windows['target_screen']
    command = target_screen_test_commands.index(cmd)
    return(send_to_studio(handle, command, param))
    
#ask the target screen for current canvas crc32 value
def get_result():
    result= send_to_target_screen('CMD_GENERATE_CHECKSUM', None)
    return result
    
def generate_screenshot(filename):
    logging.info('generate screen shot, image file name = %s', filename)
    send_to_target_screen('CMD_GENERATE_SCREENSHOT', filename);
    
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
def add_template(base_name):
    send_to_target_screen('CMD_ADD_TEMPLATE', base_name)
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
    
def mousemove(x, y):
    param = str(x) + ',' + str(y)
    send_to_target_screen('CMD_MOUSEMOVE', param)
    logging.info('simulate mousemove on (%d, %d)', x, y)

#================================================================#
#           Send Commands to Properties Screen                   #
#================================================================#
properties_win_test_commands = [
'',
'CMD_EDIT_WIDGET_PROPS',
'CMD_EDIT_NAME'
]

#send a test command to the properties view    
def send_to_properties_screen(cmd, param):
    handle = studio_windows['properties_win']
    command = properties_win_test_commands.index(cmd)
    return(send_to_studio(handle, command, param))
    
def post_to_properties_screen(cmd, param):
    handle = studio_windows['properties_win']
    command = properties_win_test_commands.index(cmd)
    return(post_to_studio(handle, command, param))
    
def edit_widget_props(id_str, prop_val):
    control_id = props_ctrl_ids[id_str];
    param = str(control_id) + "," + str(prop_val)
    prop_string = str(prop_val)
    logging.info("editing property: %s, control_id = %d, val = %s" %(id_str, control_id, prop_string.encode(sys.stdout.encoding, 'replace')))
    send_to_properties_screen('CMD_EDIT_WIDGET_PROPS', param)
    
def edit_widget_name(id_str, name):
    control_id = props_ctrl_ids[id_str];
    param = str(control_id) + "," + name
    logging.info("editing widget name, control_id = %d, name = %s" %(control_id, name))
    post_to_properties_screen('CMD_EDIT_NAME', param)
    time.sleep(1)

#================================================================#
#           Send Commands to Project View                        #
#================================================================#
project_view_test_commands = [
'',
'CMD_OPEN_PROJECT',
'CMD_CLOSE_PROJECT',
'CMD_SELECT_PROJECT_TREE_NODE',
'CMD_SELECT_MULTI_WIDGETS',
'CMD_DELETE_WIDGET',
'CMD_GET_WIDGET_LEFT',
'CMD_GET_WIDGET_TOP',
'CMD_GET_WIDGET_RIGHT',
'CMD_GET_WIDGET_BOTTOM'
]

#send a test command to the project view
def send_to_project_view(cmd, param):
    handle = studio_windows['project_view']
    command = project_view_test_commands.index(cmd)
    return(send_to_studio(handle, command, param))
    
def post_to_project_view(cmd, param):
    handle = studio_windows['project_view']
    command = project_view_test_commands.index(cmd)
    return(post_to_studio(handle, command, param))
    
# tell the Studio project view to open a project
def open_project(path):
    if not os.path.isfile(path):
        raise Exception('Test project is missing: %s' %path)
    abs_path = os.path.abspath(path)
    logging.info("open project: %s", abs_path)
    global default_map
    send_to_project_view('CMD_OPEN_PROJECT', abs_path)
    compare_result()

# tell the Studio project view to close a project
def close_project(save_change = 0):
    logging.info('close current project')
    send_to_project_view('CMD_CLOSE_PROJECT', save_change)
  
# command the project view to select a widget
def select_project_tree_node(name):  
    logging.info('select "%s"' %name)
    send_to_project_view('CMD_SELECT_PROJECT_TREE_NODE', name)
    time.sleep(1)
    
# command the project view to select multiple widgets
def select_multi_widgets(name):
    logging.info('select "%s" with Shift-Key down' %name)
    send_to_project_view('CMD_SELECT_MULTI_WIDGETS', name)
    time.sleep(1)

# command the project view to delete a widget    
def delete_widget():
    logging.info('delete current selected wigdet')
    send_to_project_view('CMD_DELETE_WIDGET', 0)
    compare_result()
    time.sleep(1)
    
def get_widget_left(widget_name):
    return send_to_project_view('CMD_GET_WIDGET_LEFT', widget_name)
    
def get_widget_top(widget_name):
    return send_to_project_view('CMD_GET_WIDGET_TOP', widget_name)
    
def get_widget_right(widget_name):
    return send_to_project_view('CMD_GET_WIDGET_RIGHT', widget_name)

def get_widget_bottom(widget_name):
    return send_to_project_view('CMD_GET_WIDGET_BOTTOM', widget_name)
    
def drag_left(widget_name, delta_x):
    logging.info('drag left of widget "%s"', widget_name)
    left = get_widget_left(widget_name) - 1
    top = get_widget_top(widget_name) - 1
    bottom = get_widget_bottom(widget_name) + 1
    
    y = (top + bottom) / 2
    left_button_down(left, y)
    mousemove(left + delta_x, y)
    left_button_up(left + delta_x, y)

def drag_right(widget_name, delta_x):
    logging.info('drag right of widget "%s"', widget_name)
    right = get_widget_right(widget_name) + 1
    top = get_widget_top(widget_name) - 1
    bottom = get_widget_bottom(widget_name) + 1
    
    y = (top + bottom) / 2
    left_button_down(right, y)
    mousemove(right + delta_x, y)
    left_button_up(right + delta_x, y)

def drag_top(widget_name, delta_y):
    logging.info('drag top of widget "%s"', widget_name)
    left = get_widget_left(widget_name) - 1
    right = get_widget_right(widget_name) + 1
    top = get_widget_top(widget_name) - 1
    
    x = (left + right) / 2
    left_button_down(x, top)
    mousemove(x, top + delta_y)
    left_button_up(x, top + delta_y)
    
def drag_bottom(widget_name, delta_y):
    logging.info('drag bottom of widget "%s"', widget_name)
    left = get_widget_left(widget_name) - 1
    right = get_widget_right(widget_name) + 1
    bottom = get_widget_bottom(widget_name) + 1
    
    x = (left + right) / 2
    left_button_down(x, bottom)
    mousemove(x, bottom + delta_y)
    left_button_up(x, bottom + delta_y)

def drag_left_top(widget_name, delta_x, delta_y):
    logging.info('drag left top of widget "%s"', widget_name)
    left = get_widget_left(widget_name) - 1
    top = get_widget_top(widget_name) - 1
    
    left_button_down(left, top)
    mousemove(left + delta_x, top + delta_y)
    left_button_up(left + delta_x, top + delta_y)
    
def drag_right_top(widget_name, delta_x, delta_y):
    logging.info('drag right top of widget "%s"', widget_name)
    right = get_widget_right(widget_name) + 1
    top = get_widget_top(widget_name) - 1
    
    left_button_down(right, top)
    mousemove(right + delta_x, top + delta_y)
    left_button_up(right + delta_x, top + delta_y)
    
def drag_left_bottom(widget_name, delta_x, delta_y):
    logging.info('drag left bottom of widget "%s"', widget_name)
    left = get_widget_left(widget_name) - 1
    bottom = get_widget_bottom(widget_name) + 1
    
    left_button_down(left, bottom)
    mousemove(left + delta_x, bottom + delta_y)
    left_button_up(left + delta_x, bottom + delta_y)
    
def drag_right_bottom(widget_name, delta_x, delta_y):
    logging.info('drag right bottom of widget "%s"', widget_name)
    right = get_widget_right(widget_name) + 1
    bottom = get_widget_bottom(widget_name) + 1
    
    left_button_down(right, bottom)
    mousemove(right + delta_x, bottom + delta_y)
    left_button_up(right + delta_x, bottom + delta_y)
    

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
'CMD_DELETE_PIXELMAP',
'CMD_ENABLE_PIXELMAP',
'CMD_DISABLE_PIXELMAP',
'CMD_ADD_PIXELMAP_FOLDER',
'CMD_REMOVE_PIXELMAP_FOLDER',
'CMD_DISABLE_PIXELMAP_FOLDER',
'CMD_SET_FOLDER_NAME',
'CMD_SAVE_FOLDER_NAME_EDIT',
'CMD_EDIT_STRING'
]

#send a test command to resource view    
def send_to_resource_view(cmd, param):
    handle = studio_windows['resource_view']
    command = resource_view_test_commands.index(cmd)
    return(send_to_studio(handle, command, param))
    
#post a test command to resource view
def post_to_resource_view(cmd, param):
    handle = studio_windows['resource_view']
    command = resource_view_test_commands.index(cmd)
    return(post_to_studio(handle, command, param))
    
def click_resource_group(name):
    logging.info('click resource group: %s', name)
    send_to_resource_view('CMD_CLICK_RESOURCE_GROUP', group_ids[name])
    
def click_pixelmap_folder(type, name):
    logging.info('click resource folder: %s', name)
    param = str(type) + ',' + name
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
def add_pixelmaps(folder_name, path, name_array):
    path = os.path.realpath(path) + '\\'
    logging.info("add pixelmaps, folder name = %s, path = %s, names = %s", folder_name, path, name_array)
    
    param = folder_name + ','
    param += path + ','
    param += name_array
    
    click_pixelmap_folder(folder_ids['CUSTOM_PIXELMAP_FOLDER'], folder_name)
    send_to_resource_view('CMD_ADD_PIXELMAPS', param) 
    click_pixelmap_folder(folder_ids['CUSTOM_PIXELMAP_FOLDER'], folder_name)
    
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

# command the resource view to show string edit dialog
def edit_string():
    logging.info("edit string table")
    post_to_resource_view('CMD_EDIT_STRING', 0)
    
    wait_dialog_open(STRING_TABLE_EDITOR_DIALOG)
    
def add_pixelmap_folder():
    logging.info('add a pixelmap folder')
    test_utils.click_resource_group('PIXELMAP_GROUP')
    post_to_resource_view('CMD_ADD_PIXELMAP_FOLDER', 0)
    wait_dialog_open(FOLDER_NAME_DIALOG)
    test_utils.click_resource_group('PIXELMAP_GROUP')
    
def remove_pixelmap_folder(name):
    click_pixelmap_folder(folder_ids['CUSTOM_PIXELMAP_FOLDER'], name)
    logging.info('remove pixelmap folder: %s', name)
    send_to_resource_view('CMD_REMOVE_PIXELMAP_FOLDER', 0)
    
def disable_pixelmap_folder(name):
    click_pixelmap_folder(folder_ids['CUSTOM_PIXELMAP_FOLDER'], name)
    logging.info('disable pixelmap folder: %s', name)
    send_to_resource_view('CMD_DISABLE_PIXELMAP_FOLDER', 0)
    click_pixelmap_folder(folder_ids['CUSTOM_PIXELMAP_FOLDER'], name)
   
    
#================================================================#
#           Send Commands to Language Configure Dialog           #
#================================================================#

language_configure_commands = [
'',
'CMD_ADD_LANGUAGE',
'CMD_DELETE_LANGUAGE',
'CMD_SELECT_LANGUAGE_INDEX',
'CMD_SELECT_LANGUAGE_ID',
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
'CMD_CONFIGURE_DISPLAY_NAME',
'CMD_CONFIGURE_DISPLAY_COLOR_FORMAT',
'CMD_CONFIGURE_MINOR_VERSION',
'CMD_CONFIGURE_SERVICE_PACK',
'CMD_CONFIGURE_CANVAS_ALLOCATE',
'CMD_CONFIGURE_CPU_TYPE',
'CMD_CONFIGURE_BIG_ENDIAN',
'CMD_SAVE_PROJECT_CONFIGURE',
'CMD_CANCEL_PROJECT_CONFIGURE',
'CMD_IS_DEFAULT_MAP_FORMAT',
'CMD_GET_CPU_TYPE',
'CMD_GET_DISPLAY_COLOR_FORMAT'
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
    
def configure_display_name(name):
    logging.info('set display name to "%s"' %name)
    send_to_project_configure_dialog('CMD_CONFIGURE_DISPLAY_NAME', name)
    
def configure_display_color_format(format):
    global reset_map_format
    if get_display_color_format() != color_formats[format]:
        reset_map_format = True
    else:
        reset_map_format = False
        
    logging.info('set display depth to "%s"' %format)
    send_to_project_configure_dialog('CMD_CONFIGURE_DISPLAY_COLOR_FORMAT', color_formats[format])
    
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
    send_to_dialog(CONFIGURE_PROJECT_DIALOG, 'CMD_OPEN_ADVANCED_SETTINGS', 0)
    
def save_project_configure(wait_close = CLOSE_WAIT):
    logging.info("save project configure")
    
    # check if all pixelmaps are default color format
    is_default_map = is_default_map_format()
    
    post_to_project_configure_dialog('CMD_SAVE_PROJECT_CONFIGURE', 0)
    
    global reset_map_format
    if is_default_map == False and reset_map_format == True:
        # a notification message is popup, close it
        close_dialog(NOTIFICATION_DIALOG)
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
'CMD_CLICK_GRADIENT_BOX',
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
    
def click_gradient_box(row, col):
    logging.info('click on gradient box, row = %d, col = %d', row, col)
    param = str(row) + ',' + str(col)
    send_to_color_edit_dialog('CMD_CLICK_GRADIENT_BOX', param)
    
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
'CMD_SET_FONT_FORMAT',
'CMD_SET_FONT_COMPRESSION',
'CMD_SET_PAGE_RANGE',
'CMD_CHECK_EXTENDED_UNICODE',
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
  
# command the font edit dialog to set font compression  
def set_font_compression(compress):
    logging.info('set font compression: %d', compress)
    send_to_font_edit_dialog('CMD_SET_FONT_COMPRESSION', compress)
    
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
    
def check_raw_format(check):
    logging.info('check raw format: %d', check)
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
'CMD_SAVE_FOLDER_NAME_EDIT'
]

def send_to_folder_name_dialog(cmd, param):
    handle = studio_dialogs[FOLDER_NAME_DIALOG]
    command = folder_name_commands.index(cmd)
    return(send_to_studio(handle, command, param))

def post_to_folder_name_dialog(cmd, param):
    handle = studio_dialogs[FOLDER_NAME_DIALOG]
    command = folder_name_commands.index(cmd)
    return(post_to_studio(handle, command, param))
    
def set_folder_name(name):
    logging.info('set folder name to "%s"', name)
    send_to_folder_name_dialog('CMD_SET_FOLDER_NAME', name)
    
def save_folder_name_edit():
    logging.info('save folder name edit')
    post_to_folder_name_dialog('CMD_SAVE_FOLDER_NAME_EDIT', 0)
    wait_dialog_close(FOLDER_NAME_DIALOG)
    
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
'CMD_IMPORT_XLIFF',
'CMD_EXPORT_XLIFF',
'CMD_TOGGLE_THREE_COLUMN_MODE',
'CMD_INCREMENT_TRANS_LANGUAGE',
'CMD_DECREMENT_TRANS_LANGUAGE',
'CMD_EDIT_TOP_STRING',
'CMD_EDIT_BOTTOM_STRING',
'CMD_EDIT_STRING_ID',
'CMD_SELECT_STRING',
'CMD_SAVE_STRING_EDIT',
'CMD_CANCEL_STRING_EDIT'
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
def import_xliff(xliff):
    xliff = os.path.realpath(xliff)
    logging.info('import xliff file: %s', xliff)
    post_to_string_edit_dialog('CMD_IMPORT_XLIFF', xliff)
    close_dialog(NOTIFICATION_DIALOG)
    
# command the string table edit dialog to export a xliff file
def export_xliff():
    logging.info('export to xliff file')
    post_to_string_edit_dialog('CMD_EXPORT_XLIFF', 0)
    wait_dialog_open(XLIFF_EXPORT_CONTROL_DIALOG, studio_dialogs[STRING_TABLE_EDITOR_DIALOG])
 
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
    
#================================================================#
#           Send Commands to Xliff Export Dialog                 #
#================================================================#   
xliff_export_commands = [
'',
'CMD_SET_XLIFF_SRC_LANGUAGE',
'CMD_SET_XLIFF_TARGET_LANGUAGE',
'CMD_SET_XLIFF_VERSION',
'CMD_SET_XLIFF_FILENAME',
'CMD_SET_XLIFF_PATH',
'CMD_SAVE_XLIFF_EXPORT',
'CMD_CANCEL_XLIFF_EXPORT'
]

def send_to_xliff_export_dialog(cmd, param):
    handle = studio_dialogs[XLIFF_EXPORT_CONTROL_DIALOG]
    command = xliff_export_commands.index(cmd)
    return(send_to_studio(handle, command, param))

def post_to_xliff_export_dialog(cmd, param):
    handle = studio_dialogs[XLIFF_EXPORT_CONTROL_DIALOG]
    command = xliff_export_commands.index(cmd)
    return(post_to_studio(handle, command, param))
    
# command the xliff export control dialog to set source language    
def set_xliff_src_language(language):
    logging.info('set source language: %s', language)
    send_to_xliff_export_dialog('CMD_SET_XLIFF_SRC_LANGUAGE', language)
    
# command the xliff export control dialog to set target language
def set_xliff_target_language(language):
    logging.info('set target language: %s', language)
    send_to_xliff_export_dialog('CMD_SET_XLIFF_TARGET_LANGUAGE', language)
    
# command the xliff export control dialog to set xliff version
def set_xliff_version(version):
    logging.info('set XLIFF version: %s', test_constants.xliff_versions[version])
    send_to_xliff_export_dialog('CMD_SET_XLIFF_VERSION', test_constants.xliff_versions[version])
   
# command the xliff export control dialog to set xliff filename   
def set_xliff_filename(filename):
    logging.info('set XLIFF filename: %s', filename)
    send_to_xliff_export_dialog('CMD_SET_XLIFF_FILENAME', filename)
    
# command the xliff export control dialog to set xliff filepath
def set_xliff_path(path):
    path = os.path.realpath(path)
    
    if os.path.exists(path) == False:
        os.makedirs(path)
        
    logging.info('set XLIFF path: %s', path)
    send_to_xliff_export_dialog('CMD_SET_XLIFF_PATH', path)
    
# command the xliff export control dialog to save xliff export
def save_xliff_export():
    logging.info('save xliff export')
    post_to_xliff_export_dialog('CMD_SAVE_XLIFF_EXPORT', 0)
    wait_dialog_close(XLIFF_EXPORT_CONTROL_DIALOG, studio_dialogs[STRING_TABLE_EDITOR_DIALOG])
    close_dialog(NOTIFICATION_DIALOG)
    
# command the xliff export control dialog to cancel xliff export
def cancel_xliff_export():
    logging.info('cancel xliff export')
    post_to_xliff_export_dialog('CMD_CANCEL_XLIFF_EXPORT', 0)
    wait_dialog_close(XLIFF_EXPORT_CONTROL_DIALOG, studio_dialogs[STRING_TABLE_EDITOR_DIALOG])
    

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

def click_generate_button():
    logging.info('click generate button')
    post_to_resource_export_dialog('CMD_GENERATE', 0)
    wait_dialog_close(RESOURCE_EXPORT_DIALOG)
    close_dialog(NOTIFICATION_DIALOG)
    
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
    
def set_theme_name(theme_name):
    logging.info("set theme name: %s", theme_name)
    send_to_configure_theme_dialog('CMD_SET_THEME_NAME', theme_name)
    
def cancel_theme_configure():
    logging.info("cancel theme configure")
    post_to_configure_theme_dialog('CMD_CANCEL_THEME_CONFIGURE', 0)
    wait_dialog_close(CONFIGURE_THEME_DIALOG)
    
def save_theme_configure(wait_close = CLOSE_WAIT):
    logging.info("save theme configure")
    post_to_configure_theme_dialog('CMD_SAVE_THEME_CONFIGURE', 0)
    
    if wait_close == CLOSE_WAIT:
        wait_dialog_close(CONFIGURE_THEME_DIALOG)

    
