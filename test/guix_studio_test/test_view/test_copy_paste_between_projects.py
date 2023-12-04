import test_utils
import test_constants

folder_name = "default_folder"

project_list = [
'generic_1bpp.gxp',
'generic_4bpp.gxp',
'generic_8bpp.gxp'
]

def get_test_header():
    notes =  "*              Studio Copy/Paste Widgets Test         *\n"
    notes += "*                                                     *\n"
    notes += "* Copy and Paste widgets with custom colors           *\n"
    notes += "* Copy and Paste widgets with custom fonts            *\n"
    notes += "* Copy and Paste widgets with unique strings          *\n"
    notes += "* Copy and Paste to widgets without children          *\n"
    notes += "* Copy and Paste to widgets with children             *\n"
    notes += "* Copy and Paste widgets to display node              *\n"
    notes += "* Copy and Paste widgets to child widgets             *\n"
    return notes
   
 
def run_copy_paste(copied, src_project, target_project):
    test_utils.open_project(src_project)
    test_utils.select_project_tree_node(copied)
    test_utils.toolbar_copy()
    test_utils.open_project(target_project)
    test_utils.select_project_tree_node(folder_name)
    #copy and paste to display node
    test_utils.toolbar_paste()
    test_utils.select_project_tree_node('window')
    #copy and paste to widget with children
    test_utils.toolbar_paste()
    test_utils.select_project_tree_node('window_1')
    #copy and paste to widget without children
    if (target_project in project_list) and (copied != 'window'):
        test_utils.toolbar_paste_no_wait()
        test_utils.close_message_dialog()
        test_utils.deselect()
        test_utils.dirty_root_window()
        test_utils.compare_result()
    else:
        test_utils.toolbar_paste()

def run_copy_paste_between_projects_tests(generate, screenshot):
    test_utils.output_test_header(get_test_header())
    test_utils.setup(generate, screenshot, 'test_copy_paste_between_projects_golden_file')
    
    #Copy from 1bpp to other formats
    print('copy from generic_1bpp.gxp to generic_16bpp.gxp')
    run_copy_paste('prompt_1', 'generic_1bpp.gxp', 'generic_16bpp.gxp')
    run_copy_paste('horizontal_list_1', 'generic_1bpp.gxp', 'generic_16bpp.gxp')
    run_copy_paste('window', 'generic_1bpp.gxp', 'generic_16bpp.gxp')
    
    print('copy from generic_1bpp.gxp to generic_8bpp.gxp')
    run_copy_paste('prompt_1', 'generic_1bpp.gxp', 'generic_8bpp.gxp')
    run_copy_paste('horizontal_list_1', 'generic_1bpp.gxp', 'generic_8bpp.gxp')
    run_copy_paste('window', 'generic_1bpp.gxp', 'generic_8bpp.gxp')

    #Copy from  other formats to 1bpp
    print('copy from generic_16bpp.gxp to generic_1bpp.gxp')
    run_copy_paste('prompt_1', 'generic_16bpp.gxp', 'generic_1bpp.gxp')
    run_copy_paste('horizontal_list_1', 'generic_16bpp.gxp', 'generic_1bpp.gxp')
    run_copy_paste('window', 'generic_16bpp.gxp', 'generic_1bpp.gxp')
    
    print('copy from generic_8bpp.gxp')
    run_copy_paste('prompt_1', 'generic_8bpp.gxp', 'generic_1bpp.gxp')
    run_copy_paste('horizontal_list_1', 'generic_8bpp.gxp', 'generic_1bpp.gxp')
    run_copy_paste('window', 'generic_8bpp.gxp', 'generic_1bpp.gxp')
    
    #Copy from 4bpp to other formats
    print('copy from generic_4bpp.gxp to generic_16bpp.gxp')
    run_copy_paste('prompt_1', 'generic_4bpp.gxp', 'generic_16bpp.gxp')
    run_copy_paste('horizontal_list_1', 'generic_4bpp.gxp', 'generic_16bpp.gxp')
    run_copy_paste('window', 'generic_4bpp.gxp', 'generic_16bpp.gxp')
    
    print('copy from generic_4bpp.gxp to generic_8bpp.gxp')
    run_copy_paste('prompt_1', 'generic_4bpp.gxp', 'generic_8bpp.gxp')
    run_copy_paste('horizontal_list_1', 'generic_4bpp.gxp', 'generic_8bpp.gxp')
    run_copy_paste('window', 'generic_4bpp.gxp', 'generic_8bpp.gxp')
    
    #Copy from  other formats to 4bpp
    print('copy from generic_16bpp.gxp to generic_4bpp.gxp')
    run_copy_paste('prompt_1', 'generic_16bpp.gxp', 'generic_4bpp.gxp')
    run_copy_paste('horizontal_list_1', 'generic_16bpp.gxp', 'generic_4bpp.gxp')
    run_copy_paste('window', 'generic_16bpp.gxp', 'generic_4bpp.gxp')
    
    print('copy from generic_8bpp.gxp to generic_4bpp.gxp')
    run_copy_paste('prompt_1', 'generic_8bpp.gxp', 'generic_4bpp.gxp')
    run_copy_paste('horizontal_list_1', 'generic_8bpp.gxp', 'generic_4bpp.gxp')
    run_copy_paste('window', 'generic_8bpp.gxp', 'generic_4bpp.gxp')
    
    print('copy from generic_8bpp.gxp to generic_16bpp.gxp')
    run_copy_paste('prompt_1', 'generic_8bpp.gxp', 'generic_16bpp.gxp')
    run_copy_paste('horizontal_list_1', 'generic_8bpp.gxp', 'generic_16bpp.gxp')
    run_copy_paste('window', 'generic_8bpp.gxp', 'generic_16bpp.gxp')
    
    print('copy from generic_8bpp.gxp to genric_32bpp.gxp')
    run_copy_paste('prompt_1', 'generic_8bpp.gxp', 'generic_32bpp.gxp')
    run_copy_paste('horizontal_list_1', 'generic_8bpp.gxp', 'generic_32bpp.gxp')
    run_copy_paste('window', 'generic_8bpp.gxp', 'generic_32bpp.gxp')
    
    print('copy from generic_8bpp.gxp to generic_332rgb.gxp')
    run_copy_paste('prompt_1', 'generic_8bpp.gxp', 'generic_332rgb.gxp')
    run_copy_paste('horizontal_list_1', 'generic_8bpp.gxp', 'generic_332rgb.gxp')
    run_copy_paste('window', 'generic_8bpp.gxp', 'generic_332rgb.gxp')
    
    print('copy from generic_8bpp.gxp to synergy_16bpp.gxp')
    run_copy_paste('prompt_1', 'generic_8bpp.gxp', 'synergy_16bpp.gxp')
    run_copy_paste('horizontal_list_1', 'generic_8bpp.gxp', 'synergy_16bpp.gxp')
    run_copy_paste('window', 'generic_8bpp.gxp', 'synergy_16bpp.gxp')
    
    print('copy from generic_16bpp.gxp to generic_8bpp.gxp')
    run_copy_paste('window', 'generic_16bpp.gxp', 'generic_8bpp.gxp')
    
    print('copy from generic_16bpp.gxp to generic_332rgb.gxp')
    run_copy_paste('window', 'generic_16bpp.gxp', 'generic_332rgb.gxp')
    
    print('copy from generic_16bpp.gxp to generic_32bpp.gxp')
    run_copy_paste('window', 'generic_16bpp.gxp', 'generic_32bpp.gxp')
    
    print('copy from generic_16bpp.gxp to synergy_16bpp.gxp')
    run_copy_paste('window', 'generic_16bpp.gxp', 'synergy_16bpp.gxp')
    
    print('copy from generic_16bpp.gxp to synergy_32bpp.gxp')
    run_copy_paste('window', 'generic_16bpp.gxp', 'synergy_32bpp.gxp')

    test_utils.write_end('Copy/Paste Between Projects Tests')
    