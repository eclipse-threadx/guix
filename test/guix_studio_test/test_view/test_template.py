import os
import sys
import time
import test_utils
import test_constants
import logging

test_template_output_files = [
"template_resources.c",
"template_resources.h",
"template_specifications.c",
"template_specifications.h"]

def get_test_header():
    notes =  "*            Studio Template Tests                    *\n"
    notes += "*                                                     *\n"
    notes += "* Test add template                                   *\n"
    notes += "* Test delete template                                *\n"
    notes += "* Test edit template                                  *\n"
    return notes

def run_template_delete():
    #delete template that is referenced that other widgets
    test_utils.select_project_tree_node('base')
    test_utils.post_delete_widget()
    test_utils.close_message_dialog()# This template cannot be deleted until all references are first removed.
    
    #delete template that is not reference
    test_utils.select_project_tree_node('base_0')
    test_utils.delete_widget(0)
    
    test_utils.select_project_tree_node('child_temlate_screen')
    test_utils.compare_result()
    
    test_utils.select_project_tree_node('template_screen')
    test_utils.compare_result()

def run_template_add():

    #add widget "test_base" and set it as template
    test_utils.select_project_tree_node("default_folder")
    test_utils.add_widget('GX_TYPE_WINDOW')
    test_utils.edit_widget_props("ID_XPOS", 0)
    test_utils.edit_widget_props("ID_YPOS", 0)
    test_utils.edit_widget_props("ID_WIDTH", 640)
    test_utils.edit_widget_props("ID_HEIGHT", 480)
    test_utils.edit_widget_props("ID_WIDGET_NAME","test_base")
    test_utils.edit_widget_props("ID_TEMPLATE", 1)
    
    #add widget "test_base_derived_0" based on "test_base"
    test_utils.select_project_tree_node("default_folder")
    test_utils.add_template("test_base")
    test_utils.edit_widget_props("ID_WIDGET_NAME","test_base_derived_0")
    test_utils.compare_result()
    
    #add a child to "test_base"
    test_utils.select_project_tree_node("test_base")
    test_utils.add_widget('GX_TYPE_WINDOW')
    test_utils.edit_widget_props("ID_WIDGET_NAME","test_base_child")
    test_utils.compare_result()
    
    #set "test_base_derived_0" as template
    #add a child to it
    test_utils.select_project_tree_node("test_base_derived_0")
    test_utils.edit_widget_props("ID_TEMPLATE", 1)
    test_utils.add_widget('GX_TYPE_WINDOW')
    test_utils.edit_widget_props("ID_WIDGET_NAME","test_base_derived_0_child")
    test_utils.compare_result()
    
    #add widget "test_base_derived_1" based on "test_base_derived_0"
    #set it as template
    #add a child
    test_utils.select_project_tree_node("default_folder")
    test_utils.add_template("test_base_derived_0")
    test_utils.edit_widget_props("ID_WIDGET_NAME","test_base_derived_1")
    test_utils.edit_widget_props("ID_TEMPLATE", 1)
    test_utils.add_widget('GX_TYPE_WINDOW')
    test_utils.edit_widget_props("ID_WIDGET_NAME","test_base_derived_1_child")
    test_utils.compare_result()
    
    #delete "test_base_derived_1"
    test_utils.select_project_tree_node("test_base_derived_1")
    test_utils.delete_widget()
    
    #delete "test_base_derived_0"
    test_utils.select_project_tree_node("test_base_derived_0")
    test_utils.delete_widget()
    
    test_utils.select_project_tree_node("test_base")
    test_utils.compare_result()
    
    #undo delete "test_base_derived_0"
    test_utils.undo()
    
    #add widget "test_base_derived_3" based on "test_base"
    test_utils.select_project_tree_node("default_folder")
    test_utils.add_template("test_base")
    test_utils.edit_widget_props("ID_WIDGET_NAME","test_base_derived_3")
    
    #add a child widget to template "test_base"
    test_utils.select_project_tree_node("test_base")
    test_utils.select_project_tree_node("test_base_child")
    test_utils.move_selected(20)
    test_utils.select_project_tree_node("test_base_derived_3")
    test_utils.compare_result()

    #add "radial_progress_bar" template
    test_utils.select_project_tree_node("default_folder")
    test_utils.add_widget('GX_TYPE_RADIAL_PROGRESS_BAR')
    test_utils.edit_widget_props("ID_WIDGET_NAME", 'base_radial_progress_bar')
    test_utils.edit_widget_props("ID_RADIAL_PROGRESS_XCENTER", 50)
    test_utils.edit_widget_props("ID_RADIAL_PROGRESS_YCENTER", 50)
    test_utils.edit_widget_props("ID_RADIAL_PROGRESS_RADIUS", 50)
    test_utils.edit_widget_props("ID_RADIAL_PROGRESS_ANCHORVAL", 60)
    test_utils.edit_widget_props("ID_RADIAL_PROGRESS_CURRENTVAL", -200)
    test_utils.edit_widget_props("ID_NORMAL_TEXT_COLOR", "red")
    test_utils.edit_widget_props("ID_RADIAL_PROGRESS_NORMAL_BRUSH_COLOR", "green")
    test_utils.edit_widget_props("ID_RADIAL_PROGRESS_SELECTED_BRUSH_COLOR", "red")
    test_utils.edit_widget_props("ID_RADIAL_PROGRESS_NORMAL_BRUSH_WIDTH", 18)
    test_utils.edit_widget_props("ID_RADIAL_PROGRESS_SELECTED_BRUSH_WIDTH", 16)
    test_utils.edit_widget_props("ID_TEMPLATE", 1)
    
    # add "icon" template
    test_utils.select_project_tree_node("default_folder")
    test_utils.add_widget('GX_TYPE_ICON')
    test_utils.edit_widget_props("ID_WIDGET_NAME", 'base_icon')
    test_utils.edit_widget_props("ID_PIXELMAP_0", "CHECKBOX_OFF")
    test_utils.edit_widget_props("ID_TEMPLATE", 1)
        
    # add "pixelmap_button" template
    test_utils.select_project_tree_node("default_folder")
    test_utils.add_widget('GX_TYPE_PIXELMAP_BUTTON')
    test_utils.edit_widget_props("ID_PIXELMAP_0", "ORANGE_BUTTON")
    test_utils.edit_widget_props("ID_WIDGET_NAME", 'base_pixelmap_button')
    test_utils.edit_widget_props("ID_TEMPLATE", 1)
    test_utils.toolbar_size_to_fit()
    
    # add "prompt" template
    test_utils.select_project_tree_node("default_folder")
    test_utils.add_widget('GX_TYPE_PROMPT')
    test_utils.edit_widget_props("ID_WIDGET_NAME", 'base_prompt')
    test_utils.edit_widget_props("ID_TEMPLATE", 1)
    
    # add "numeric_pixelmap_prompt" template
    test_utils.select_project_tree_node("default_folder")
    test_utils.add_widget('GX_TYPE_NUMERIC_PIXELMAP_PROMPT')
    test_utils.edit_widget_props("ID_PIXELMAP_0", "ORANGE_BUTTON")
    test_utils.edit_widget_props("ID_WIDGET_NAME", 'base_numeric_pixelmap_prompt')
    test_utils.edit_widget_props("ID_TEMPLATE", 1)
    test_utils.toolbar_size_to_fit()
    
    # add "ml_text_view" template
    test_utils.select_project_tree_node("default_folder")
    test_utils.add_widget('GX_TYPE_MULTI_LINE_TEXT_VIEW')
    test_utils.edit_widget_props("ID_WIDGET_NAME", 'base_ml_text_view')
    test_utils.edit_widget_props("ID_TEMPLATE", 1)
    
    # add "rich_text_view" template
    test_utils.select_project_tree_node("default_folder")
    test_utils.add_widget('GX_TYPE_RICH_TEXT_VIEW')
    test_utils.edit_widget_props("ID_WIDGET_NAME", 'base_rich_text_view')
    test_utils.edit_widget_props("ID_TEMPLATE", 1)
    
    # add "pixelmap_slider" template
    test_utils.select_project_tree_node("default_folder")
    test_utils.add_widget('GX_TYPE_PIXELMAP_SLIDER')
    test_utils.edit_widget_props("ID_WIDTH", 195)
    test_utils.edit_widget_props("ID_HEIGHT", 46)
    test_utils.edit_widget_props("ID_PIXELMAP_0", "I_ORANGEFILL_MIDDLE_HORIZONTAL")
    test_utils.edit_widget_props("ID_PIXELMAP_1", "I_EMPTYFILL_MIDDLE_HORIZONTAL")
    test_utils.edit_widget_props("ID_PIXELMAP_2", "I_INDICATOR_HORIZONTAL")
    test_utils.edit_widget_props("ID_WIDGET_NAME", 'base_pixelmap_slider')
    test_utils.edit_widget_props("ID_SLIDER_TILE_BKGND", 1)
    test_utils.edit_widget_props("ID_TEMPLATE", 1)
    
    # add "radial_slider" template
    test_utils.select_project_tree_node("default_folder")
    test_utils.add_widget('GX_TYPE_RADIAL_SLIDER')
    test_utils.edit_widget_props("ID_PIXELMAP_0", "BG_WASHER_ON")
    test_utils.edit_widget_props("ID_PIXELMAP_1", "I_INDICATOR_HORIZONTAL")
    test_utils.edit_widget_props("ID_WIDGET_NAME", 'base_radial_slider')
    test_utils.edit_widget_props("ID_TEMPLATE", 1)
    test_utils.toolbar_size_to_fit()
    
    # add "sprite" template
    test_utils.select_project_tree_node("default_folder")
    test_utils.add_widget('GX_TYPE_SPRITE')
    test_utils.edit_widget_props("ID_PIXELMAP_0", "SPRITE")
    test_utils.edit_widget_props("ID_WIDGET_NAME", 'base_sprite')
    test_utils.edit_widget_props("ID_TEMPLATE", 1)
    test_utils.toolbar_size_to_fit()
    
    # add "accordion" template
    test_utils.select_project_tree_node("default_folder")
    test_utils.add_widget('GX_TYPE_ACCORDION_MENU')
    test_utils.edit_widget_props("ID_WIDGET_NAME", 'base_accordion')
    test_utils.edit_widget_props("ID_TEMPLATE", 1)
    test_utils.add_widget('GX_TYPE_MENU')
    test_utils.select_project_tree_node('base_accordion')
    test_utils.add_widget('GX_TYPE_MENU')
    test_utils.select_project_tree_node('base_accordion')
    test_utils.add_widget('GX_TYPE_MENU')
    
    # add "tree_view" template
    test_utils.select_project_tree_node("default_folder")
    test_utils.add_widget('GX_TYPE_TREE_VIEW')
    test_utils.edit_widget_props("ID_WIDGET_NAME", 'base_tree_view')
    test_utils.edit_widget_props("ID_TEMPLATE", 1)
    test_utils.add_widget('GX_TYPE_MENU')
    test_utils.edit_widget_props("ID_WIDGET_NAME", 'base_tree_view_menu_1')
    test_utils.select_project_tree_node('base_tree_view')
    test_utils.add_widget('GX_TYPE_MENU')
    test_utils.select_project_tree_node('base_tree_view')
    test_utils.add_widget('GX_TYPE_MENU')
    test_utils.select_project_tree_node('base_tree_view_menu_1')
    test_utils.add_widget('GX_TYPE_MENU')  
    
    test_utils.select_project_tree_node("default_folder")
    test_utils.add_widget("GX_TYPE_WINDOW")
    test_utils.edit_widget_props("ID_WIDGET_NAME", 'widget_template_container')

    # derive widget based on "icon" template
    test_utils.add_template('base_icon')
    test_utils.edit_widget_props("ID_XPOS", 28)
    test_utils.edit_widget_props("ID_YPOS", 15)
    
    # derive widget based on "pixelmap_button" template
    test_utils.select_project_tree_node('widget_template_container')
    test_utils.add_template('base_pixelmap_button')
    test_utils.edit_widget_props("ID_XPOS", 80)
    test_utils.edit_widget_props("ID_YPOS", 8)
    
   # derive widget based on "pixelmap_slider" template
    test_utils.select_project_tree_node('widget_template_container')
    test_utils.add_template('base_pixelmap_slider')
    test_utils.edit_widget_props("ID_XPOS", 260)
    test_utils.edit_widget_props("ID_YPOS", 14) 
    
    # derive widget based on "prompt" template
    test_utils.select_project_tree_node('widget_template_container')
    test_utils.add_template('base_prompt')
    test_utils.edit_widget_props("ID_XPOS", 502)
    test_utils.edit_widget_props("ID_YPOS", 31)
    
    # derive widget based on "numeric_pixelmap_prompt" template
    test_utils.select_project_tree_node('widget_template_container')
    test_utils.add_template('base_numeric_pixelmap_prompt')
    test_utils.edit_widget_props("ID_XPOS", 7)
    test_utils.edit_widget_props("ID_YPOS", 38)
    
    # derive widget based on "radial_slider" template
    test_utils.select_project_tree_node('widget_template_container')
    test_utils.add_template('base_radial_slider')
    test_utils.edit_widget_props("ID_XPOS", 18)
    test_utils.edit_widget_props("ID_YPOS", 98)
    
    # derive widget based on "rich_text_view" template
    test_utils.select_project_tree_node('widget_template_container')
    test_utils.add_template('base_rich_text_view')
    test_utils.edit_widget_props("ID_XPOS", 316)
    test_utils.edit_widget_props("ID_YPOS", 65)
    
   # derive widget based on "sprite" template
    test_utils.select_project_tree_node('widget_template_container')
    test_utils.add_template('base_sprite')
    test_utils.edit_widget_props("ID_XPOS", 18)
    test_utils.edit_widget_props("ID_YPOS", 382)
    
    
    # derive widget based on "text_view" template
    test_utils.select_project_tree_node('widget_template_container')
    test_utils.add_template('base_ml_text_view')
    test_utils.edit_widget_props("ID_XPOS", 419)
    test_utils.edit_widget_props("ID_YPOS", 149)
    
    # derive widget based on "tree_view" template
    test_utils.select_project_tree_node('widget_template_container')
    test_utils.add_template('base_tree_view')
    test_utils.edit_widget_props("ID_XPOS", 142)
    test_utils.edit_widget_props("ID_YPOS", 397)
    
    # derive widget based on "accordion" template
    test_utils.select_project_tree_node('widget_template_container')
    test_utils.add_template('base_accordion')
    test_utils.edit_widget_props("ID_XPOS", 455)
    test_utils.edit_widget_props("ID_YPOS", 263)
  
    # derive widget based on "radial_progress_bar" template
    test_utils.select_project_tree_node('widget_template_container')
    test_utils.add_template('base_radial_progress_bar')
    test_utils.edit_widget_props("ID_XPOS", 209)
    test_utils.edit_widget_props("ID_YPOS", 129)
    
    test_utils.compare_result()
    
def run_template_edit():
    #Test template when color depth changed
    test_utils.select_project_tree_node("base")
    test_utils.add_widget('GX_TYPE_PROMPT')
    test_utils.edit_widget_props("ID_WIDGET_NAME","child_prompt")
    test_utils.edit_widget_props("ID_XPOS", 327)
    test_utils.edit_widget_props("ID_YPOS", 207)
    test_utils.edit_widget_props("ID_WIDTH", 308)
    test_utils.edit_widget_props("ID_HEIGHT", 34)
    test_utils.edit_widget_props("ID_BORDER_COMBO", test_constants.BORDER_TYPE_THIN)
    test_utils.edit_widget_props("ID_TRANSPARENT", 1)
    test_utils.edit_widget_props("ID_STRING_VAL", "transparent prompt with thin border")
    test_utils.configure_project()
    test_utils.configure_display_color_format('GX_COLOR_FORMAT_8BIT_PALETTE')
    test_utils.save_project_configure()
    test_utils.move_window(10)
    test_utils.move_window(-10)
    
    #test resize template
    #test transparent template
    test_utils.select_project_tree_node("base")
    test_utils.edit_widget_props("ID_TRANSPARENT", 1)
    test_utils.edit_widget_props("ID_WIDTH", 446)
    test_utils.compare_result()
    
    test_utils.select_project_tree_node("child_template_screen")
    test_utils.compare_result()

def run_derived_widget_update():
    test_utils.open_project('template.gxp')
    
    test_utils.add_pixelmaps('Custom', '..\\pixelmaps\\', 'black_pause.png')
    test_utils.add_pixelmaps('Custom', '..\\pixelmaps\\', 'save_icon.png')
    
    # open font folder
    test_utils.click_resource_group('FONT_GROUP')

    # add font Vera.ttf
    test_utils.add_font() 
    test_utils.set_font_path('..\\fonts\\Vera.ttf')
    test_utils.set_font_name('TEST_FONT_1')
    test_utils.save_font_edit()
    
    # add font VeraBd.ttf
    test_utils.add_font()
    test_utils.set_font_path('..\\fonts\\VeraBd.ttf')
    test_utils.set_font_name('TEST_FONT_2')
    test_utils.save_font_edit()
    
    test_utils.click_resource_group('FONT_GROUP')
    
    test_utils.edit_string()
    test_utils.sort_string(test_constants.STRING_SORT_TYPE_ID)
    test_utils.add_string()
    test_utils.edit_string_id('STRING_1')
    test_utils.edit_top_string('string 1')
    
    test_utils.add_string()
    test_utils.edit_string_id('STRING_2')
    test_utils.edit_top_string('string 2')
    
    test_utils.save_string_edit()
    
    # open color group folder
    test_utils.click_resource_group('COLOR_GROUP')
    
    # add a new color
    test_utils.add_color()
    test_utils.set_color_name('COLOR_1')
    test_utils.set_color_red(255)
    test_utils.save_color_edit()
    
    # add a new color
    test_utils.add_color()
    test_utils.set_color_name('COLOR_2')
    test_utils.set_color_green(255)
    test_utils.save_color_edit()
    
    test_utils.click_resource_group('COLOR_GROUP')
    
    test_utils.select_project_tree_node("base")
    test_utils.edit_widget_props("ID_PIXELMAP_0", "BLACK_PAUSE")
    test_utils.edit_widget_props("ID_NORMAL_COLOR", "COLOR_1")
    
    test_utils.add_widget('GX_TYPE_PROMPT')
    test_utils.edit_widget_props("ID_FONT_SELECT_0", "TEST_FONT_1")
    
    test_utils.select_project_tree_node('base')
    test_utils.add_widget("GX_TYPE_PROMPT")
    test_utils.edit_widget_props("ID_XPOS", 116)
    test_utils.edit_widget_props("ID_STRING_ID", 'STRING_1')
    
    test_utils.select_project_tree_node("child_template_screen");
    test_utils.compare_result()
    
    test_utils.delete_pixelmap("BLACK_PAUSE")
    test_utils.compare_result()
    
    test_utils.click_resource_group('FONT_GROUP')
    test_utils.delete_font("TEST_FONT_1")
    test_utils.click_resource_group('FONT_GROUP')
    test_utils.compare_result()
    
    test_utils.click_resource_group('COLOR_GROUP')
    test_utils.delete_color('COLOR_1')
    test_utils.click_resource_group('COLOR_GROUP')
    test_utils.compare_result()
    
    test_utils.edit_string()
    test_utils.select_string(1)
    test_utils.delete_string()
    test_utils.save_string_edit()
    test_utils.compare_result()
    
    test_utils.edit_string()
    test_utils.select_string(0)
    test_utils.delete_string()
    test_utils.save_string_edit()
    test_utils.compare_result()
    
def run_template_name_test():
    test_utils.select_project_tree_node("folder_1_window")
    test_utils.compare_result()
    
def run_template_tests(generate, screenshot):

    test_utils.output_test_header(get_test_header()) 
    test_utils.setup(generate, screenshot, 'test_template_golden_file')
    
    # open test project
    test_utils.open_project('template.gxp')
    
    print("run template delete")
    run_template_delete()
    
    print("run_template_add")
    run_template_add()

    print("run template edit")
    run_template_edit()

    print("run template name test")
    run_template_name_test()
    
    test_utils.generate_all()
    test_utils.click_generate_button(1)
    test_utils.cmp_output_files(test_template_output_files)
    
    print("run derived widget update")
    run_derived_widget_update()
    
    test_utils.write_end("Test Tempalte")
    

