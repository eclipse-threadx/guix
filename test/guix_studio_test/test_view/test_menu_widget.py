# -*- coding: utf-8 -*-
import os
import sys
import time
import test_utils
import test_constants
import filecmp

display_name = "main_display"
folder_name = "default_folder"
screen_name = "main_screen"

def get_test_header():
    notes =  "*          Menu Widget Test                   *\n"
    notes += "*                                             *\n"
    notes += "* Add and Edit Accordion Menu                 *\n"
    notes += "* Add and Edit Tree View                      *\n"
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
    
    # add normal font
    # open font folder
    test_utils.click_resource_group('FONT_GROUP')
    
    test_utils.add_font() 
    test_utils.set_font_path('..\\..\\fonts\\Vera.ttf')
    test_utils.set_font_name('NORMAL_FONT')
    test_utils.set_font_format('GX_FONT_FORMAT_8BPP')
    test_utils.set_font_height(28)  
    test_utils.save_font_edit()
    
    # add selected font
    test_utils.add_font() 
    test_utils.set_font_path('..\\..\\fonts\\Vera.ttf')
    test_utils.set_font_name('SELECTED_FONT')
    test_utils.set_font_format('GX_FONT_FORMAT_8BPP')
    test_utils.set_font_height(30)  
    test_utils.save_font_edit()
    
    # cloce font folder
    test_utils.click_resource_group('FONT_GROUP')
    
    test_utils.add_pixelmaps('Custom', '..\\pixelmaps\\', 'tfield_fill_small.png')
    test_utils.add_pixelmaps('Custom', '..\\pixelmaps\\', 'collapse.png')
    test_utils.add_pixelmaps('Custom', '..\\pixelmaps\\', 'expand.png')
    
    # close message dialog when create new project
    test_utils.close_message_dialog()

    
def add_main_screen():
    #---------------------------------------#
    #add main screen to display             #
    #---------------------------------------#
    
    test_utils.select_project_tree_node("window")

    test_utils.edit_widget_props("ID_XPOS", 0)
    test_utils.edit_widget_props("ID_YPOS", 0)
    test_utils.edit_widget_props("ID_WIDTH", 640)
    test_utils.edit_widget_props("ID_HEIGHT", 480)
    test_utils.edit_widget_props("ID_WIDGET_NAME", screen_name)
    

def add_edit_accordion_menu_widget():
    #---------------------------------------#
    #add and edit numeric scroll wheel      #
    #---------------------------------------#
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_ACCORDION_MENU')
    accordion_menu_name = "accordion"
    accordion_menu_1 = "accordion_menu_1"
    accordion_menu_2 = "accordion_menu_2"
    accordion_menu_3 = "accordion_menu_3"
    accordion_menu_4 = "accordion_menu_4"
    accordion_menu_5 = "accordion_menu_5"
    
    test_utils.edit_widget_props("ID_XPOS", 29)
    test_utils.edit_widget_props("ID_YPOS", 30)
    test_utils.edit_widget_props("ID_WIDTH", 176)
    test_utils.edit_widget_props("ID_HEIGHT", 138)
    test_utils.edit_widget_props("ID_WIDGET_NAME", accordion_menu_name)
    test_utils.compare_result()
    
    #add 5 menus
    test_utils.select_project_tree_node(accordion_menu_name)
    test_utils.add_widget('GX_TYPE_MENU')
    test_utils.edit_widget_props("ID_WIDGET_NAME", accordion_menu_1)
    test_utils.edit_widget_props("ID_STRING_VAL", "menu 1")
    test_utils.edit_widget_props("ID_MENU_TEXT_X_OFFSET", "15")
    test_utils.edit_widget_props("ID_MENU_TEXT_Y_OFFSET", "5")
    test_utils.compare_result()
    test_utils.undo()
    test_utils.undo()
    test_utils.compare_result()
    test_utils.select_project_tree_node(accordion_menu_name)
    test_utils.add_widget('GX_TYPE_MENU')
    test_utils.edit_widget_props("ID_WIDGET_NAME", accordion_menu_2)
    test_utils.edit_widget_props("ID_STRING_VAL", "menu 2")
    test_utils.select_project_tree_node(accordion_menu_name)
    test_utils.add_widget('GX_TYPE_MENU')
    test_utils.edit_widget_props("ID_WIDGET_NAME", accordion_menu_3)
    test_utils.edit_widget_props("ID_STRING_VAL", "menu 3")
    test_utils.select_project_tree_node(accordion_menu_name)
    test_utils.add_widget('GX_TYPE_TEXT_BUTTON')
    test_utils.edit_widget_props("ID_BORDER_COMBO", test_constants.BORDER_TYPE_RAISED)
    test_utils.edit_widget_props("ID_WIDGET_NAME", accordion_menu_4)
    test_utils.edit_widget_props("ID_STRING_VAL", "menu 4")
    test_utils.select_project_tree_node(accordion_menu_name)
    test_utils.add_widget('GX_TYPE_MENU')
    test_utils.edit_widget_props("ID_WIDGET_NAME", accordion_menu_5)
    test_utils.edit_widget_props("ID_STRING_VAL", "menu 5")
    test_utils.compare_result()
    
    #remove menu 3
    test_utils.select_project_tree_node(accordion_menu_3)
    test_utils.delete_widget()
    test_utils.compare_result()
    
    #set fill pixelmap for menu 1
    test_utils.select_project_tree_node(accordion_menu_1)
    test_utils.edit_widget_props("ID_PIXELMAP_1", test_constants.PIXELMAP_FILL)
    test_utils.toolbar_size_to_fit()
    test_utils.compare_result()
    
    #set new size for menu 2
    test_utils.select_project_tree_node(accordion_menu_2)
    test_utils.edit_widget_props_post("ID_XPOS", 20)#not allowed
    test_utils.close_message_dialog()
    test_utils.edit_widget_props_post("ID_YPOS", 20)#not allowed
    test_utils.close_message_dialog()
    test_utils.edit_widget_props_post("ID_WIDTH", 40)#not allowed
    test_utils.close_message_dialog()
    test_utils.edit_widget_props("ID_HEIGHT", 40)
    test_utils.compare_result()
    
    #insert menu item for menu 1
    test_utils.select_project_tree_node(accordion_menu_1)
    test_utils.add_widget("GX_TYPE_PROMPT")
    test_utils.edit_widget_props("ID_WIDGET_NAME", "accordion_menu_1_item_1")
    test_utils.edit_widget_props_post("ID_XPOS", 20)#not allowed
    test_utils.close_message_dialog()
    test_utils.edit_widget_props_post("ID_YPOS", 20)#not allowed
    test_utils.close_message_dialog()
    test_utils.edit_widget_props("ID_WIDTH", 174)
    test_utils.edit_widget_props("ID_HEIGHT", 46)
    test_utils.compare_result()
    
    test_utils.select_project_tree_node(accordion_menu_1)
    test_utils.add_widget("GX_TYPE_MENU")
    test_utils.edit_widget_props("ID_WIDTH", 174)
    test_utils.edit_widget_props("ID_HEIGHT", 34)
    test_utils.edit_widget_props("ID_WIDGET_NAME", "accordion_menu_1_item_2")
    test_utils.compare_result()
    
    test_utils.select_project_tree_node(accordion_menu_1)
    test_utils.edit_widget_props("ID_MENU_EXPANDED", 0)#remove menu expanded style
    test_utils.edit_widget_props("ID_MENU_INSERT_MODE", 0)
    test_utils.add_widget("GX_TYPE_PROMPT")
    test_utils.compare_result()
    
    #undo widget add
    test_utils.undo()
    #test_utils.undo()
    
    #undo insert mode change
    test_utils.undo()
    
    #undo expand style change
    test_utils.undo()
    
    test_utils.compare_result()
    
    test_utils.select_project_tree_node(accordion_menu_1)
    #remove menu expanded style
    test_utils.edit_widget_props("ID_MENU_EXPANDED", 0)
    test_utils.compare_result()
    
    #remove menu 1 item 2
    test_utils.select_project_tree_node("accordion_menu_1_item_2")
    test_utils.delete_widget()
    test_utils.compare_result()
    
    #undo delete
    test_utils.undo()
    
    #insert an accordion menu to menu 4
    test_utils.select_project_tree_node(accordion_menu_2)
    test_utils.add_widget('GX_TYPE_ACCORDION_MENU')
    test_utils.edit_widget_props("ID_WIDTH", 174)
    test_utils.edit_widget_props("ID_HEIGHT", 34)
    test_utils.edit_widget_props("ID_WIDGET_NAME", "accordion_2")
    
    #insert a menu to accordion 2
    test_utils.add_widget("GX_TYPE_MENU")
    
    #insert a text view to the menu
    test_utils.add_widget("GX_TYPE_MULTI_LINE_TEXT_VIEW")
    test_utils.edit_widget_props("ID_WIDTH", 174)
    test_utils.edit_widget_props("ID_HEIGHT", 69)
    test_utils.compare_result()
    
    #select accordion 2
    test_utils.select_project_tree_node("accordion_2")
    
    #insert a menu to accordion 2
    test_utils.add_widget("GX_TYPE_MENU")
    
    #insert a text view to the menu
    test_utils.add_widget("GX_TYPE_MULTI_LINE_TEXT_VIEW")
    test_utils.edit_widget_props("ID_WIDTH", 174)
    test_utils.edit_widget_props("ID_HEIGHT", 69)
    test_utils.compare_result()
    
    #test accordion menu copy paste
    test_utils.select_project_tree_node(accordion_menu_name)
    test_utils.toolbar_copy()
    test_utils.select_project_tree_node(screen_name)
    test_utils.toolbar_paste()
    test_utils.compare_result()
    
    test_utils.undo()
    
    test_utils.select_project_tree_node("accordion_menu_1_item_1")
    test_utils.compare_result()
    
    
def add_edit_tree_view_widget():
    #---------------------------------------#
    #add and edti tree view                 #
    #---------------------------------------#
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_TREE_VIEW')
    tree_view_name = "tree_view"
    
    #edit tree view properties
    test_utils.edit_widget_props("ID_XPOS", 260)
    test_utils.edit_widget_props("ID_YPOS", 33)
    test_utils.edit_widget_props("ID_WIDTH", 276)
    test_utils.edit_widget_props("ID_HEIGHT", 335)
    test_utils.edit_widget_props("ID_WIDGET_NAME", tree_view_name)
    test_utils.edit_widget_props("ID_PIXELMAP_1", "EXPAND")
    test_utils.edit_widget_props("ID_PIXELMAP_2", "COLLAPSE")
    
    # insert 5 menus
    test_utils.select_project_tree_node(tree_view_name)
    test_utils.add_widget("GX_TYPE_MENU")
    test_utils.edit_widget_props("ID_WIDGET_NAME", "tree_menu_1")
    test_utils.edit_widget_props("ID_STRING_VAL", "menu 1")
    test_utils.select_project_tree_node(tree_view_name)  
    test_utils.add_widget("GX_TYPE_MENU")
    test_utils.edit_widget_props("ID_WIDGET_NAME", "tree_menu_2")
    test_utils.edit_widget_props("ID_STRING_VAL", "menu 2")
    test_utils.select_project_tree_node(tree_view_name)
    test_utils.add_widget("GX_TYPE_MENU")
    test_utils.edit_widget_props("ID_WIDGET_NAME", "tree_menu_3")
    test_utils.edit_widget_props("ID_STRING_VAL", "menu 3")
    test_utils.select_project_tree_node(tree_view_name)
    test_utils.add_widget("GX_TYPE_TEXT_BUTTON")
    test_utils.edit_widget_props("ID_BORDER_COMBO", test_constants.BORDER_TYPE_RAISED)
    test_utils.edit_widget_props("ID_WIDGET_NAME", "tree_menu_4")
    test_utils.edit_widget_props("ID_STRING_VAL", "menu 4")
    test_utils.select_project_tree_node(tree_view_name)
    test_utils.add_widget("GX_TYPE_MENU")
    test_utils.edit_widget_props("ID_WIDGET_NAME", "tree_menu_5")
    test_utils.edit_widget_props("ID_STRING_VAL", "menu 5")
    test_utils.compare_result()
    
    test_utils.select_project_tree_node(tree_view_name)
    test_utils.edit_widget_props("ID_TREE_VIEW_INDENTATION", "10")
    test_utils.compare_result()
    test_utils.undo()
    
    test_utils.select_project_tree_node(tree_view_name)
    test_utils.edit_widget_props("ID_TREE_VIEW_SHOW_ROOT_LINES", 0)
    test_utils.compare_result()
    test_utils.undo()
    test_utils.select_project_tree_node(tree_view_name)
    test_utils.edit_widget_props("ID_TREE_VIEW_ROOT_LINE_COLOR", 'BTN_BORDER')
    test_utils.compare_result()
    test_utils.undo()
    
    # insert menu item to menu 1
    test_utils.select_project_tree_node("tree_menu_1")
    test_utils.add_widget("GX_TYPE_MENU")
    test_utils.edit_widget_props("ID_WIDGET_NAME", "tree_menu_1_item_1")
    test_utils.edit_widget_props("ID_STRING_VAL", "tree_menu_1_item_1")
    test_utils.select_project_tree_node("tree_menu_1")
    test_utils.add_widget("GX_TYPE_MENU")
    test_utils.edit_widget_props("ID_WIDGET_NAME", "tree_menu_1_item_2")
    test_utils.edit_widget_props("ID_STRING_VAL", "tree_menu_1_item_2")
    test_utils.add_widget("GX_TYPE_MENU")
    test_utils.edit_widget_props("ID_WIDGET_NAME", "tree_menu_1_item_2_1")
    test_utils.edit_widget_props("ID_STRING_VAL", "tree_menu_1_item_2_1")
    test_utils.add_widget("GX_TYPE_MENU")
    test_utils.edit_widget_props("ID_WIDGET_NAME", "tree_menu_1_item_2_1_1")
    test_utils.edit_widget_props("ID_STRING_VAL", "tree_menu_1_item_2_1_1")
    test_utils.compare_result()
    
    #insert menu item to menu 2
    test_utils.select_project_tree_node("tree_menu_2")
    test_utils.add_widget("GX_TYPE_PROMPT")
    test_utils.select_project_tree_node("tree_menu_2")
    test_utils.add_widget("GX_TYPE_BUTTON")
    test_utils.edit_widget_props("ID_BORDER_COMBO", test_constants.BORDER_TYPE_RAISED)
    test_utils.edit_widget_props_post("ID_XPOS", 250)#not allowed
    test_utils.close_message_dialog()
    test_utils.edit_widget_props_post("ID_YPOS", 150)#not allowed
    test_utils.close_message_dialog()
    test_utils.edit_widget_props("ID_WIDTH", 206)
    test_utils.edit_widget_props("ID_HEIGHT", 35)
    test_utils.compare_result()
    
    #insert menu item to menu 3
    test_utils.select_project_tree_node("tree_menu_3")
    test_utils.add_widget("GX_TYPE_MENU")
    test_utils.add_widget("GX_TYPE_PROMPT")
    test_utils.compare_result()
    
    #delete menu 3
    test_utils.select_project_tree_node("tree_menu_3")
    test_utils.delete_widget()
    test_utils.compare_result()
    
    #undo delete
    test_utils.undo()
    
    #undo add
    test_utils.undo()
    test_utils.compare_result()
    
    #resize tree view
    test_utils.select_project_tree_node(tree_view_name)
    test_utils.edit_widget_props("ID_WIDTH", 171)
    test_utils.edit_widget_props("ID_HEIGHT", 240)
    
    #add scroll bar
    test_utils.select_project_tree_node(tree_view_name)
    test_utils.add_widget("GX_TYPE_HORIZONTAL_SCROLL")
    test_utils.select_project_tree_node(tree_view_name)
    test_utils.add_widget("GX_TYPE_VERTICAL_SCROLL")
    test_utils.compare_result()
    
    #test copy paste
    test_utils.select_project_tree_node(tree_view_name)
    test_utils.toolbar_copy()
    test_utils.select_project_tree_node(screen_name)
    test_utils.toolbar_paste()
    test_utils.compare_result()
    test_utils.undo()
    
    #collapse menu 1
    test_utils.select_project_tree_node("tree_menu_1")
    test_utils.edit_widget_props("ID_MENU_EXPANDED", 0)
    test_utils.compare_result()
    
    #collapse menu 2
    test_utils.select_project_tree_node("tree_menu_2")
    test_utils.edit_widget_props("ID_MENU_EXPANDED", 0)
    test_utils.compare_result()
    
    
def select_menu_item_that_is_not_expanded():
    # open test project
    test_utils.open_project('all_widgets_execute.gxp')
    
    test_utils.select_project_tree_node('tree_menu_2')
    test_utils.edit_widget_props("ID_MENU_EXPANDED", 0)
    test_utils.compare_result()
    test_utils.select_project_tree_node('tree_menu_2_1')
    test_utils.compare_result()
    
    test_utils.select_project_tree_node('tree_menu_2')
    test_utils.edit_widget_props("ID_MENU_EXPANDED", 0)
    test_utils.compare_result()
    test_utils.select_project_tree_node('tree_menu_2_1_1')
    test_utils.compare_result()

def run_menu_position():
    # open test project
    test_utils.open_project('all_widgets_execute.gxp')
    
    #delete accordion_menu 1.1
    test_utils.select_project_tree_node('prompt_25_1')
    test_utils.delete_widget()
    
    test_utils.undo()

    #delete accordion_menu 1.2
    test_utils.select_project_tree_node('prompt_26_1')
    test_utils.delete_widget()

    #delete tree view menu 2.1.1
    test_utils.select_project_tree_node('tree_menu_2_1_1')
    test_utils.delete_widget()

    #delete tree view menu 2.1.2
    test_utils.select_project_tree_node('tree_menu_2_1_2')
    test_utils.delete_widget()

    #delete tree view menu 1.1
    test_utils.select_project_tree_node('tree_menu_1_1')
    test_utils.delete_widget()

    #undo delete
    test_utils.undo()
    
def run_menu_widget_tests(generate, screenshot):

    test_utils.output_test_header(get_test_header())
    test_utils.setup(generate, screenshot, 'test_menu_widget_golden_file')

    print('create new project')
    run_new_project_create("test_menu_widget")
    
    print('add main screen')
    add_main_screen()
    
    print('add and edit accordion menu widget')
    add_edit_accordion_menu_widget()
    
    print('add and edit tree view widget')
    add_edit_tree_view_widget()
    
    test_utils.toolbar_save()

    test_utils.toolbar_save()
    
    #close project with widget been selected
    test_utils.select_project_tree_node("accordion")
    test_utils.close_project()
    test_utils.open_project(test_utils.DEFAULT_OUTPUT_FILE_PATH + "\\test_menu_widget.gxp")
    test_utils.select_project_tree_node("tree_view")
    test_utils.close_project()
    
    print('test menu item select when the item is not expanded')
    select_menu_item_that_is_not_expanded()
    
    print('run menu position')
    run_menu_position()
    
    test_utils.write_end('Menu Widget Tests')