import test_utils
import test_constants

display_name = "display_1"
folder_name = "default_folder"

test_copy_paste_output_files = [
"test_copy_paste_resources.c",
"test_copy_paste_resources.h",
"test_copy_paste_specifications.c",
"test_copy_paste_specifications.h"]

def get_test_header():
    notes =  "*          Studio Copy/Paste Widgets Test             *\n"
    notes += "*                                                     *\n"
    notes += "* Copy and Paste widgets to an empty project with     *\n"
    notes += "*   pixlemap folder been removed                      *\n"    
    return notes

def run_new_project_create(path, name):
    # create a new project
    test_utils.toolbar_create()
    test_utils.set_new_project_path(path)
    test_utils.set_new_project_name(name)
    test_utils.save_new_project_create()
    
    # configure project
    test_utils.configure_project()
    test_utils.configure_x_res(800)
    test_utils.configure_y_res(480)
    test_utils.configure_display_name('display_1')
    test_utils.configure_display_color_format('GX_COLOR_FORMAT_565RGB')
    test_utils.configure_source_path('output_files')
    test_utils.configure_header_path('output_files')
    test_utils.configure_resource_path('output_files')
    test_utils.save_project_configure()
    
    # close message dialog when create new and delete default window
    test_utils.close_message_dialog()
    test_utils.select_project_tree_node("window")
    test_utils.delete_widget(0)
    
    # configure languages
    test_utils.configure_languages()  
    test_utils.add_language()
    test_utils.add_language() 
    test_utils.select_language_index(2)
    test_utils.select_language_id('Latin')
    test_utils.select_language_index(3)
    test_utils.select_language_id('French')
    test_utils.save_language_configure()
    
    #edit string table
    test_utils.edit_string()
    
    test_utils.sort_string(test_constants.STRING_SORT_TYPE_ID)
    test_utils.save_string_edit()
    
    test_utils.toolbar_save()

def run_copy_paste_folder():
    print("test copy-paste with folder")
    base_screen = 'base_screen'
    folder_1 = 'default_folder_1'
    folder_2 = 'default_folder_2'
    folder_3 = 'default_folder_3'
    derived_1 = 'derived_1'
    derived_2 = 'derived_2'
    test_utils.select_project_tree_node(folder_name)
    test_utils.toolbar_copy()#copy default folder
    test_utils.toolbar_paste_no_wait()#paste to default folder
    test_utils.close_message_dialog()#notify that paste to folder is not allowed
    
    test_utils.select_project_tree_node(display_name)
    test_utils.toolbar_paste_no_wait()#paste to display
    
    test_utils.select_project_tree_node('window')
    test_utils.toolbar_copy()
    test_utils.select_project_tree_node(folder_1)
    test_utils.toolbar_paste()#paste window to default folder 1
    
    test_utils.select_project_tree_node('window_2')
    test_utils.edit_widget_props("ID_WIDGET_NAME", base_screen)
    test_utils.edit_widget_props("ID_TEMPLATE", 1)
    test_utils.select_project_tree_node(folder_1)
    test_utils.add_template(base_screen)
    test_utils.edit_widget_props("ID_WIDGET_NAME",derived_1)

    test_utils.select_project_tree_node(folder_1)
    test_utils.toolbar_copy()
    test_utils.select_project_tree_node(display_name)
    test_utils.toolbar_paste_no_wait()
    test_utils.close_message_dialog()#notify to skip paste template "base_screen"

    test_utils.select_project_tree_node(derived_1)
    test_utils.edit_widget_props("ID_TEMPLATE", 1)
    test_utils.select_project_tree_node(folder_1)
    test_utils.add_template(derived_1)
    test_utils.edit_widget_props("ID_WIDGET_NAME",derived_2)
    
    test_utils.select_project_tree_node(folder_1)
    test_utils.toolbar_copy()
    test_utils.select_project_tree_node(display_name)
    test_utils.toolbar_paste_no_wait()
    test_utils.close_message_dialog()#notify to skip paste template "derived_1"
    test_utils.close_message_dialog()#notify to skip paste template "base_screen"

    test_utils.select_project_tree_node(folder_3) 
    test_utils.delete_folder()
    test_utils.select_project_tree_node(folder_2)
    test_utils.delete_folder()
    test_utils.select_project_tree_node(folder_1)
    test_utils.delete_folder()

    test_utils.select_project_tree_node(display_name)
    test_utils.insert_folder()
    test_utils.rename_folder(folder_name)
    test_utils.close_message_dialog()#notify that folder name already exist
    test_utils.select_project_tree_node(folder_1)
    test_utils.toolbar_copy()
    test_utils.select_project_tree_node(display_name)
    test_utils.toolbar_paste_no_wait()
    
    test_utils.select_project_tree_node(display_name)
    test_utils.insert_folder()
    test_utils.rename_folder('a_folder')
    test_utils.select_project_tree_node(display_name)
    test_utils.insert_folder()
    test_utils.rename_folder('b_folder')
    test_utils.select_project_tree_node('b_folder')
    test_utils.delete_folder()
    test_utils.select_project_tree_node('a_folder')
    test_utils.delete_folder()

def run_copy_paste_template():
    test_utils.select_project_tree_node(folder_name)
    
    base_name = 'base'
    derived_name = 'derived'
    
    # add a template
    test_utils.add_widget('GX_TYPE_WINDOW');
    test_utils.edit_widget_props('ID_WIDGET_NAME', base_name)
    test_utils.edit_widget_props('ID_TEMPLATE', 1)
    
    # add a child widget to the template
    test_utils.select_project_tree_node(base_name)
    test_utils.add_widget('GX_TYPE_BUTTON');
    
    # copy/paste template as a top level screen
    test_utils.select_project_tree_node(base_name)
    test_utils.toolbar_copy()
    test_utils.select_project_tree_node(folder_name)
    test_utils.toolbar_paste_no_wait()
    test_utils.close_message_dialog()#notify template already exist.
    
    # copy/paste template to a top level screen
    test_utils.select_project_tree_node(folder_name)
    test_utils.add_widget('GX_TYPE_WINDOW')
    test_utils.edit_widget_props('ID_WIDGET_NAME', 'parent_screen')
    test_utils.toolbar_paste_no_wait();
    test_utils.close_message_dialog()#notify template can only be pasted to folder node
    
    # delete template
    test_utils.select_project_tree_node(base_name)
    test_utils.delete_widget()
    
    # paste template widget to default folder
    test_utils.select_project_tree_node(folder_name)
    test_utils.toolbar_paste();
    
    # add a derived widget from template
    test_utils.select_project_tree_node(folder_name)
    test_utils.add_template(base_name)
    test_utils.edit_widget_props("ID_WIDGET_NAME", derived_name)
    test_utils.add_widget('GX_TYPE_PROMPT')
    test_utils.edit_widget_props("ID_STRING_VAL", "derived")
    
    # copy derived widget
    test_utils.select_project_tree_node(derived_name)
    test_utils.toolbar_copy()
    
    # delete derived widget
    test_utils.select_project_tree_node(derived_name)
    test_utils.delete_widget()
    
    # delete base widget
    test_utils.select_project_tree_node(base_name)
    test_utils.delete_widget()
    
    # paste derived widget to default folder
    test_utils.select_project_tree_node(folder_name)
    test_utils.toolbar_paste()
    
    # delete derived widget
    test_utils.select_project_tree_node(derived_name)
    test_utils.delete_widget()
    
    # delete base wiget
    test_utils.select_project_tree_node(base_name)
    test_utils.delete_widget()
    
    # create a widget that duplicate template name
    test_utils.select_project_tree_node(folder_name)
    test_utils.add_widget('GX_TYPE_WINDOW')
    test_utils.edit_widget_props('ID_WIDGET_NAME', base_name)
    
    # paste derived widget to parent screen
    test_utils.select_project_tree_node('parent_screen')
    test_utils.toolbar_paste_no_wait()
    test_utils.close_message_dialog() # notify template name already exist
    
    # delete widget that duplicate template name
    test_utils.select_project_tree_node(base_name)
    test_utils.delete_widget()
    
    # paste derived widget to parent screen
    test_utils.select_project_tree_node('parent_screen')
    test_utils.toolbar_paste()
    
    # paste derived widget to parent screen
    test_utils.select_project_tree_node('parent_screen')
    test_utils.toolbar_paste()
    
def run_copy_paste_tests(generate, screenshot):
    test_utils.output_test_header(get_test_header())
    test_utils.setup(generate, screenshot, 'test_copy_paste_golden_file')
    
    test_project = 'test_copy_paste'
    print('create a new project: %s' %(test_project))
    run_new_project_create('./', test_project)
    
    #remove custom pixelmap folder
    test_utils.click_resource_group('PIXELMAP_GROUP')
    test_utils.remove_pixelmap_folder('Custom')
    test_utils.click_resource_group('PIXELMAP_GROUP')
    test_utils.toolbar_save()
    
    #copy widgets to empty project with pixelmap folder being removed
    print('copy from generic_16bpp.gxp to %s.gxp' %(test_project))
    test_utils.open_project('generic_16bpp.gxp')
    test_utils.select_project_tree_node('window')
    test_utils.toolbar_copy()
    test_utils.open_project(test_project+'.gxp')
    test_utils.select_project_tree_node(folder_name)
    test_utils.toolbar_paste()
    
    #copy "window"
    test_utils.select_project_tree_node("window")
    test_utils.toolbar_copy()
    
    #paste "window" to "display_1"
    test_utils.select_project_tree_node(folder_name)
    test_utils.toolbar_paste()
    
    #copy/paste multiple widgets
    test_utils.select_multi_widgets("window_1")
    test_utils.select_multi_widgets("prompt_1")
    test_utils.select_multi_widgets("text_input_2")
    test_utils.toolbar_copy()
    test_utils.select_project_tree_node("window")
    test_utils.toolbar_paste()
    
    #test copy/paste folders
    run_copy_paste_folder()
    
    #test copy/paste template
    run_copy_paste_template()
    
    #generate output files
    test_utils.generate_all()
    test_utils.click_generate_button(1)
    test_utils.cmp_output_files(test_copy_paste_output_files)
    
    test_utils.toolbar_save()
    
    test_utils.write_end('Copy/Paste Tests')
    