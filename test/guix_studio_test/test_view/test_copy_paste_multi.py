import test_utils

def get_test_header():
    notes =  "*     Studio Copy/Paste Multiple Widgets Test         *\n"
    notes += "*                                                     *\n"
    notes += "* Copy and Paste multiple widgets                     *\n"
    return notes
   
 
def run_new_project_create(name, format):
    # create a new project
    test_utils.toolbar_create()
    test_utils.set_new_project_path(test_utils.DEFAULT_OUTPUT_FILE_PATH)
    test_utils.set_new_project_name(name)
    test_utils.save_new_project_create()
    
    # configure project
    test_utils.configure_project()
    test_utils.configure_x_res(640)
    test_utils.configure_y_res(480)
    test_utils.configure_display_name('display_1')
    test_utils.configure_display_color_format(format)
    test_utils.configure_source_path('output_files')
    test_utils.configure_header_path('output_files')
    test_utils.configure_resource_path('output_files')
    test_utils.save_project_configure()
    
    #create default folder for new project
    test_utils.close_message_dialog()

    test_utils.toolbar_save()
    
def run_copy_paste_multi_tests(generate, screenshot):
    test_utils.output_test_header(get_test_header())
    test_utils.setup(generate, screenshot, 'test_copy_paste_multi_golden_file')

    #create a 16bpp project
    test_project = 'test_copy_paste_multi'
    print('create a 16bpp project: %s' %(test_project))
    run_new_project_create(test_project, 'GX_COLOR_FORMAT_565RGB')
    
    #insert a window
    test_utils.select_project_tree_node("window")
    test_utils.edit_widget_props("ID_XPOS", 0)
    test_utils.edit_widget_props("ID_YPOS", 0)
    test_utils.edit_widget_props("ID_WIDTH", 640)
    test_utils.edit_widget_props("ID_HEIGHT", 480)
    
    #insert a prompt to window
    test_utils.select_project_tree_node("window")
    test_utils.add_widget("GX_TYPE_PROMPT")
    
    #insert a button to window
    test_utils.select_project_tree_node("window")
    test_utils.add_widget("GX_TYPE_BUTTON")
    test_utils.edit_widget_props("ID_XPOS", 10)
    test_utils.edit_widget_props("ID_YPOS", 10)
    test_utils.edit_widget_props("ID_WIDTH", 100)
    test_utils.edit_widget_props("ID_HEIGHT", 50)
    
    #select button and prompt
    test_utils.select_multi_widgets("prompt")

    #copy selected widgets
    test_utils.toolbar_copy()

    #paste selected widgets to window
    test_utils.select_project_tree_node("window")
    test_utils.toolbar_paste(0)#0:do not deselect after paste

    #delete selected widgets(not allowed)
    test_utils.delete_widget(0)#0:do not compare after delete
    
    #move selected widget
    test_utils.move_selected(50)

    test_utils.undo()
    test_utils.undo()
    
    test_utils.toolbar_save()
    
    test_utils.write_end('Copy/Paste Multi Tests')
    