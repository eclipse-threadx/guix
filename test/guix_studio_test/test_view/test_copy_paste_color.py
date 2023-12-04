import test_utils

def get_test_header():
    notes =  "*             Studio Copy/Paste Color Test            *\n"
    notes += "*                                                     *\n"
    notes += "* Copy and Paste widget with custom color between     *\n"
    notes += "*   8bpp and 16bpp project                            *\n"
    return notes
   
 
def run_new_project_create(name, format):
    # create a new project
    test_utils.toolbar_create()
    test_utils.set_new_project_path(test_utils.DEFAULT_OUTPUT_FILE_PATH)
    test_utils.set_new_project_name(name)
    test_utils.save_new_project_create()
    
    # configure project
    test_utils.configure_project()
    test_utils.configure_x_res(320)
    test_utils.configure_y_res(240)
    test_utils.configure_display_name('display_1')
    test_utils.configure_display_color_format(format)
    test_utils.configure_source_path('output_files')
    test_utils.configure_header_path('output_files')
    test_utils.configure_resource_path('output_files')
    test_utils.save_project_configure()
    
    #create default folder for new project
    test_utils.close_message_dialog()
    test_utils.select_project_tree_node("window")
    test_utils.delete_widget(0)
    
    test_utils.toolbar_save()

def insert_gray_color():
    # open color group folder
    test_utils.click_resource_group('COLOR_GROUP')
    
    #---------------------------------------------------------#
    #          Add "GRAY" color                               #
    #---------------------------------------------------------#
    
    # open color edit dialog
    test_utils.add_color()
    
    # edit color
    test_utils.set_color_name('GRAY')
    test_utils.set_color_red(100)
    test_utils.set_color_green(111)
    test_utils.set_color_blue(112)
    test_utils.save_color_edit()

    # close color group folder
    test_utils.click_resource_group('COLOR_GROUP')
    
def run_copy_paste_color_tests(generate, screenshot):
    test_utils.output_test_header(get_test_header())
    test_utils.setup(generate, screenshot, 'test_copy_paste_color_golden_file')

    #create a 16bpp project
    test_project = 'test_copy_paste_color'
    print('create a 16bpp project: %s' %(test_project))
    run_new_project_create(test_project, 'GX_COLOR_FORMAT_565RGB')
    
    #insert gray color
    insert_gray_color()
    
    #insert a window and set fill color to "GRAY"
    test_utils.select_project_tree_node("default_folder")
    test_utils.add_widget("GX_TYPE_WINDOW")
    test_utils.edit_widget_props("ID_NORMAL_COLOR", "GRAY") 
    test_utils.edit_widget_props("ID_XPOS", 80)
    test_utils.edit_widget_props("ID_YPOS", 60)
    test_utils.edit_widget_props("ID_WIDTH", 160)
    test_utils.edit_widget_props("ID_HEIGHT", 120)
    
    #copy the inserted window
    test_utils.toolbar_copy()
    
    #create a 8bpp project
    print('create a 8bpp project: %s' %(test_project))
    run_new_project_create(test_project, 'GX_COLOR_FORMAT_8BIT_PALETTE')
    
    #paste widget from 16bpp to 8bpp empty project
    test_utils.select_project_tree_node("default_folder")
    test_utils.toolbar_paste()
    test_utils.select_project_tree_node("default_folder")
    test_utils.toolbar_paste()

    #copy "window"
    test_utils.select_project_tree_node("window")
    test_utils.toolbar_copy()
    
    #create a 16bpp project
    print('create a 16bpp project: %s' %(test_project))
    run_new_project_create(test_project, 'GX_COLOR_FORMAT_565RGB')
    
    #paste widget from 8bpp project to 16bpp empty project
    test_utils.select_project_tree_node("default_folder")
    test_utils.toolbar_paste()
    test_utils.select_project_tree_node("default_folder")
    test_utils.toolbar_paste()

    test_utils.toolbar_save()
    
    test_utils.write_end('Copy/Paste Color Tests')
    