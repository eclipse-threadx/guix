# -*- coding: utf-8 -*-
import os
import sys
import time
import test_utils
import test_constants
import filecmp

test_add_widget_16bpp_output_files = [
"test_add_widget_16bpp_resources.c",
"test_add_widget_16bpp_resources.h",
"test_add_widget_16bpp_specifications.c",
"test_add_widget_16bpp_specifications.h"]

test_add_widget_8bpp_output_files = [
"test_add_widget_8bpp_resources.c",
"test_add_widget_8bpp_resources.h",
"test_add_widget_8bpp_specifications.c",
"test_add_widget_8bpp_specifications.h"]

test_add_widget_24bpp_output_files = [
"test_add_widget_24bpp_resources.c",
"test_add_widget_24bpp_resources.h",
"test_add_widget_24bpp_specifications.c",
"test_add_widget_24bpp_specifications.h"]

display_name = 'display_1'
folder_name = 'default_folder'
ml_text_input_widget_name = 'ml_text_input_test'
text_screen_name = 'text_screen'
button_screen_name = 'button_screen'
window_screen_name = 'window_screen'
bad_name = 'bAd #1 name'                     # Name check test.

#Create a screen with a bunch of buttons on it:
def add_button_screen():
    #---------------------------------------#
    #add button screen window to display    #
    #---------------------------------------#
    test_utils.select_project_tree_node(folder_name)
    test_utils.add_widget('GX_TYPE_WINDOW')
    screen_name = "button_screen"
    
    test_utils.edit_widget_props("ID_XPOS", 0)
    test_utils.edit_widget_props("ID_YPOS", 0)
    test_utils.edit_widget_props("ID_WIDTH", 640)
    test_utils.edit_widget_props("ID_HEIGHT", 480)
    
    test_utils.edit_widget_props("ID_WIDGET_NAME", screen_name)
    test_utils.edit_widget_props("ID_BORDER_COMBO", test_constants.BORDER_TYPE_THICK)
    test_utils.edit_widget_props("ID_NORMAL_COLOR", test_constants.COLOR_WHITE) 
    
    #add button
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_BUTTON')
    test_utils.edit_widget_props("ID_XPOS", 41)
    test_utils.edit_widget_props("ID_YPOS", 35)
    test_utils.edit_widget_props("ID_WIDTH", 161)
    test_utils.edit_widget_props("ID_HEIGHT", 49)
    test_utils.edit_widget_props("ID_BORDER_COMBO", test_constants.BORDER_TYPE_RAISED)
    
    #add text button
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_TEXT_BUTTON')
    test_utils.edit_widget_props("ID_XPOS", 41)
    test_utils.edit_widget_props("ID_YPOS", 98)
    test_utils.edit_widget_props("ID_WIDTH", 161)
    test_utils.edit_widget_props("ID_HEIGHT", 49)
    test_utils.edit_widget_props("ID_STRING_VAL", "text_button")
    test_utils.edit_widget_props("ID_BORDER_COMBO", test_constants.BORDER_TYPE_RAISED)
    
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_TEXT_BUTTON')
    test_utils.edit_widget_props("ID_XPOS", 327)
    test_utils.edit_widget_props("ID_YPOS", 176)
    test_utils.edit_widget_props("ID_WIDTH", 161)
    test_utils.edit_widget_props("ID_HEIGHT", 49)
    test_utils.edit_widget_props("ID_TEXT_ALIGN", 'Left')
    test_utils.edit_widget_props("ID_STRING_VAL", "left_alignment")
    test_utils.edit_widget_props("ID_BORDER_COMBO", test_constants.BORDER_TYPE_RAISED)
    
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_TEXT_BUTTON')
    test_utils.edit_widget_props("ID_XPOS", 327)
    test_utils.edit_widget_props("ID_YPOS", 240)
    test_utils.edit_widget_props("ID_WIDTH", 161)
    test_utils.edit_widget_props("ID_HEIGHT", 49)
    test_utils.edit_widget_props("ID_TEXT_ALIGN", 'right')
    test_utils.edit_widget_props("ID_STRING_VAL", "right_alignment")
    test_utils.edit_widget_props("ID_BORDER_COMBO", test_constants.BORDER_TYPE_RAISED)
    
    #add multi line button
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_MULTI_LINE_TEXT_BUTTON')
    test_utils.edit_widget_props("ID_XPOS", 41)
    test_utils.edit_widget_props("ID_YPOS", 157)
    test_utils.edit_widget_props("ID_WIDTH", 161)
    test_utils.edit_widget_props("ID_HEIGHT", 64)
    test_utils.edit_widget_props("ID_TRANSPARENT", 1)
    test_utils.edit_widget_props("ID_BORDER_COMBO", test_constants.BORDER_TYPE_RAISED)
    
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_MULTI_LINE_TEXT_BUTTON')
    test_utils.edit_widget_props("ID_XPOS", 327)
    test_utils.edit_widget_props("ID_YPOS", 304)
    test_utils.edit_widget_props("ID_WIDTH", 161)
    test_utils.edit_widget_props("ID_HEIGHT", 64)
    test_utils.edit_widget_props("ID_TEXT_ALIGN", 'Left')
    test_utils.edit_widget_props("ID_STRING_ID", "STRING_LEFT_ALIGNMENT")
    test_utils.edit_widget_props("ID_BORDER_COMBO", test_constants.BORDER_TYPE_RAISED)
    
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_MULTI_LINE_TEXT_BUTTON')
    test_utils.edit_widget_props("ID_XPOS", 327)
    test_utils.edit_widget_props("ID_YPOS", 386)
    test_utils.edit_widget_props("ID_WIDTH", 161)
    test_utils.edit_widget_props("ID_HEIGHT", 64)
    test_utils.edit_widget_props("ID_TEXT_ALIGN", 'right')
    test_utils.edit_widget_props("ID_STRING_ID", "STRING_RIGHT_ALIGNMENT")
    test_utils.edit_widget_props("ID_BORDER_COMBO", test_constants.BORDER_TYPE_RAISED)
    
    #add pixelmap button
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_PIXELMAP_BUTTON')
    test_utils.edit_widget_props("ID_XPOS", 74)
    test_utils.edit_widget_props("ID_YPOS", 220)
    test_utils.edit_widget_props("ID_WIDTH", 141)
    test_utils.edit_widget_props("ID_HEIGHT", 69)
    test_utils.edit_widget_props("ID_PIXELMAP_0", test_constants.PIXELMAP_ORANGE_BUTTON)
    test_utils.edit_widget_props("ID_BORDER_COMBO", test_constants.BORDER_TYPE_RAISED)
    
    #add checkbox
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_CHECKBOX')
    test_utils.edit_widget_props("ID_XPOS", 101)
    test_utils.edit_widget_props("ID_YPOS", 302)
    test_utils.edit_widget_props("ID_WIDTH", 99)
    test_utils.edit_widget_props("ID_HEIGHT", 24)
    test_utils.edit_widget_props("ID_STRING_VAL", "checkbox")
    
    #add radio button
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_RADIO_BUTTON')
    test_utils.edit_widget_props("ID_XPOS", 74)
    test_utils.edit_widget_props("ID_YPOS", 337)
    test_utils.edit_widget_props("ID_WIDTH", 130)
    test_utils.edit_widget_props("ID_HEIGHT", 24)
    test_utils.edit_widget_props("ID_STRING_VAL", "radio_button")
    
    #add icon button
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_ICON_BUTTON')
    test_utils.edit_widget_props("ID_XPOS", 177)
    test_utils.edit_widget_props("ID_YPOS", 368)
    test_utils.edit_widget_props("ID_WIDTH", 23)
    test_utils.edit_widget_props("ID_HEIGHT", 24)
    test_utils.edit_widget_props("ID_PIXELMAP_0", test_constants.PIXELMAP_SAVE_ICON)
    test_utils.edit_widget_props("ID_BORDER_COMBO", test_constants.BORDER_TYPE_RAISED)

    #add icon
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_ICON')
    test_utils.edit_widget_props("ID_XPOS", 172)
    test_utils.edit_widget_props("ID_YPOS", 407)
    test_utils.edit_widget_props("ID_WIDTH", 38)
    test_utils.edit_widget_props("ID_HEIGHT", 38)
    test_utils.edit_widget_props("ID_PIXELMAP_0", test_constants.PIXELMAP_HISTORY)
    
    #add prompt
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_PROMPT')
    test_utils.edit_widget_props("ID_XPOS", 327)
    test_utils.edit_widget_props("ID_YPOS", 34)
    test_utils.edit_widget_props("ID_WIDTH", 287)
    test_utils.edit_widget_props("ID_HEIGHT", 34)
    test_utils.edit_widget_props("ID_BORDER_COMBO", test_constants.BORDER_TYPE_THIN)
    test_utils.edit_widget_props("ID_TRANSPARENT", 1)
    test_utils.edit_widget_props("ID_STRING_VAL", "transparent prompt with thin border")
    
    #add prompt
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_PROMPT')
    test_utils.edit_widget_props("ID_XPOS", 327)
    test_utils.edit_widget_props("ID_YPOS", 80)
    test_utils.edit_widget_props("ID_WIDTH", 287)
    test_utils.edit_widget_props("ID_HEIGHT", 34)
    test_utils.edit_widget_props("ID_BORDER_COMBO", test_constants.BORDER_TYPE_NONE)
    test_utils.edit_widget_props("ID_TRANSPARENT", 1)
    test_utils.edit_widget_props("ID_TEXT_ALIGN", 'Left')
    test_utils.edit_widget_props("ID_STRING_VAL", "transparent prompt with NO border")
    test_utils.edit_widget_props("ID_FONT_SELECT_0", test_constants.FONT_VERABI)

    #add text copy style and assign an empty string
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_PROMPT')
    test_utils.edit_widget_props("ID_XPOS", 327)
    test_utils.edit_widget_props("ID_YPOS", 7)
    test_utils.edit_widget_props("ID_WIDTH", 80)
    test_utils.edit_widget_props("ID_HEIGHT", 24)
    test_utils.edit_widget_props("ID_BORDER_COMBO", test_constants.BORDER_TYPE_NONE)
    test_utils.edit_widget_props("ID_TRANSPARENT", 1)
    test_utils.edit_widget_props("ID_TEXT_COPY", 1)
    test_utils.edit_widget_props("ID_STRING_VAL", "")

    #add pixelmap prompt
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_PIXELMAP_PROMPT')
    test_utils.edit_widget_props("ID_XPOS", 332)
    test_utils.edit_widget_props("ID_YPOS", 125)
    test_utils.edit_widget_props("ID_WIDTH", 279)
    test_utils.edit_widget_props("ID_HEIGHT", 36)
    test_utils.edit_widget_props("ID_BORDER_COMBO", test_constants.BORDER_TYPE_NONE)
    test_utils.edit_widget_props("ID_TEXT_ALIGN", 'right')
    test_utils.edit_widget_props("ID_STRING_VAL", "000")
    test_utils.edit_widget_props("ID_PIXELMAP_0", test_constants.PIXELMAP_LEFT)
    test_utils.edit_widget_props("ID_PIXELMAP_1", test_constants.PIXELMAP_FILL)
    test_utils.edit_widget_props("ID_PIXELMAP_2", test_constants.PIXELMAP_RIGHT)
    test_utils.edit_widget_props("ID_FONT_SELECT_0", test_constants.FONT_VERABD)
    test_utils.edit_widget_props("ID_TRANSPARENT", 1)
    test_utils.compare_result()

#Create a screen with a bunch of text widgets on it:
def add_text_screen():
    
    # Add text view / input widgets
    test_utils.select_project_tree_node(folder_name)    
    test_utils.add_widget('GX_TYPE_WINDOW')
    screen_name = "text_screen"
    
    test_utils.edit_widget_props("ID_XPOS", 10)
    test_utils.edit_widget_props("ID_YPOS", 10)
    test_utils.edit_widget_props("ID_WIDTH", 620)
    test_utils.edit_widget_props("ID_HEIGHT", 460)

    test_utils.edit_widget_props("ID_WIDGET_NAME", screen_name)
    test_utils.edit_widget_props("ID_BORDER_COMBO", test_constants.BORDER_TYPE_NONE)
    test_utils.edit_widget_props("ID_NORMAL_COLOR", test_constants.COLOR_WHITE)   
    
    #add single line text input
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_SINGLE_LINE_TEXT_INPUT')
    test_utils.edit_widget_props("ID_XPOS", 20)
    test_utils.edit_widget_props("ID_YPOS", 20)
    test_utils.edit_widget_props("ID_WIDTH", 283)
    test_utils.edit_widget_props("ID_HEIGHT", 33)
    test_utils.edit_widget_props("ID_BORDER_COMBO", test_constants.BORDER_TYPE_RECESSED)
    test_utils.edit_widget_props("ID_STRING_VAL", "left aligned text")
    test_utils.edit_widget_props("ID_NORMAL_COLOR", test_constants.COLOR_BLACK)
    test_utils.edit_widget_props("ID_TEXT_ALIGN", 'Left')
    test_utils.edit_widget_props("ID_FONT_SELECT_0", test_constants.FONT_VERA)

    #add single line text input
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_SINGLE_LINE_TEXT_INPUT')
    test_utils.edit_widget_props("ID_XPOS", 20)
    test_utils.edit_widget_props("ID_YPOS", 60)
    test_utils.edit_widget_props("ID_WIDTH", 283)
    test_utils.edit_widget_props("ID_HEIGHT", 33)
    test_utils.edit_widget_props("ID_BORDER_COMBO", test_constants.BORDER_TYPE_RECESSED)
    test_utils.edit_widget_props("ID_STRING_VAL", "left")
    test_utils.edit_widget_props("ID_NORMAL_COLOR", test_constants.COLOR_BLACK)
    test_utils.edit_widget_props("ID_TEXT_ALIGN", 'Left')
    test_utils.edit_widget_props("ID_FONT_SELECT_0", test_constants.FONT_VERABI)

    #add single line text input
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_SINGLE_LINE_TEXT_INPUT')
    test_utils.edit_widget_props("ID_XPOS", 20)
    test_utils.edit_widget_props("ID_YPOS", 100)
    test_utils.edit_widget_props("ID_WIDTH", 283)
    test_utils.edit_widget_props("ID_HEIGHT", 33)
    test_utils.edit_widget_props("ID_BORDER_COMBO", test_constants.BORDER_TYPE_RECESSED)
    test_utils.edit_widget_props("ID_STRING_VAL", "right aligned text")
    test_utils.edit_widget_props("ID_NORMAL_COLOR", test_constants.COLOR_BLACK)
    test_utils.edit_widget_props("ID_TEXT_ALIGN", 'Right')
    
    #add single line text input
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_SINGLE_LINE_TEXT_INPUT')
    test_utils.edit_widget_props("ID_XPOS", 20)
    test_utils.edit_widget_props("ID_YPOS", 138)
    test_utils.edit_widget_props("ID_WIDTH", 283)
    test_utils.edit_widget_props("ID_HEIGHT", 33)
    test_utils.edit_widget_props("ID_BORDER_COMBO", test_constants.BORDER_TYPE_RECESSED)
    test_utils.edit_widget_props("ID_STRING_VAL", "center aligned text")
    test_utils.edit_widget_props("ID_NORMAL_COLOR", test_constants.COLOR_BLACK)
    test_utils.edit_widget_props("ID_TEXT_ALIGN", 'Center')
    
    #add multi line text view
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_MULTI_LINE_TEXT_VIEW')
    test_utils.edit_widget_props("ID_XPOS", 20)
    test_utils.edit_widget_props("ID_YPOS", 178)
    test_utils.edit_widget_props("ID_WIDTH", 180)
    test_utils.edit_widget_props("ID_HEIGHT", 74)
    test_utils.edit_widget_props("ID_STRING_VAL", "Left Align")
    test_utils.edit_widget_props("ID_NORMAL_COLOR", test_constants.COLOR_BLACK)
    test_utils.edit_widget_props("ID_TEXT_ALIGN", 'Left')

    #add multi line text view
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_MULTI_LINE_TEXT_VIEW')
    test_utils.edit_widget_props("ID_XPOS", 20)
    test_utils.edit_widget_props("ID_YPOS", 259)
    test_utils.edit_widget_props("ID_WIDTH", 180)
    test_utils.edit_widget_props("ID_HEIGHT", 74)
    test_utils.edit_widget_props("ID_STRING_VAL", "Center Align")
    test_utils.edit_widget_props("ID_NORMAL_COLOR", test_constants.COLOR_BLACK)
    test_utils.edit_widget_props("ID_TEXT_ALIGN", 'Center')

    #add multi line text view
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_MULTI_LINE_TEXT_VIEW')
    test_utils.edit_widget_props("ID_XPOS", 20)
    test_utils.edit_widget_props("ID_YPOS", 340)
    test_utils.edit_widget_props("ID_WIDTH", 180)
    test_utils.edit_widget_props("ID_HEIGHT", 74)
    test_utils.edit_widget_props("ID_STRING_VAL", "Right Align")
    test_utils.edit_widget_props("ID_NORMAL_COLOR", test_constants.COLOR_BLACK)
    test_utils.edit_widget_props("ID_TEXT_ALIGN", 'Right')       
    
    #add multi line text input
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_MULTI_LINE_TEXT_INPUT')
    test_utils.edit_widget_props("ID_WIDGET_NAME", ml_text_input_widget_name)
    test_utils.edit_widget_props("ID_XPOS", 332)
    test_utils.edit_widget_props("ID_YPOS", 20)
    test_utils.edit_widget_props("ID_WIDTH", 180)
    test_utils.edit_widget_props("ID_HEIGHT", 74)
    test_utils.edit_widget_props("ID_STRING_VAL", "Left Align Input")
    test_utils.edit_widget_props("ID_NORMAL_COLOR", test_constants.COLOR_BLACK)
    test_utils.edit_widget_props("ID_TEXT_ALIGN", 'Left')
    test_utils.add_widget('GX_TYPE_VERTICAL_SCROLL')


    #add multi line text input
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_MULTI_LINE_TEXT_INPUT')
    test_utils.edit_widget_props("ID_WIDGET_NAME", "multi_line_text_input_c")
    test_utils.edit_widget_props("ID_XPOS", 332)
    test_utils.edit_widget_props("ID_YPOS", 120)
    test_utils.edit_widget_props("ID_WIDTH", 180)
    test_utils.edit_widget_props("ID_HEIGHT", 74)
    test_utils.edit_widget_props("ID_STRING_VAL", "Center Align")
    test_utils.edit_widget_props("ID_NORMAL_COLOR", test_constants.COLOR_BLACK)
    test_utils.edit_widget_props("ID_TEXT_ALIGN", 'Center')
    test_utils.add_widget('GX_TYPE_VERTICAL_SCROLL')

    #add multi line text input
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_MULTI_LINE_TEXT_INPUT')
    test_utils.edit_widget_props("ID_WIDGET_NAME", "multi_line_text_input_r")
    test_utils.edit_widget_props("ID_XPOS", 332)
    test_utils.edit_widget_props("ID_YPOS", 220)
    test_utils.edit_widget_props("ID_WIDTH", 180)
    test_utils.edit_widget_props("ID_HEIGHT", 74)
    test_utils.edit_widget_props("ID_STRING_VAL", "Right Align")
    test_utils.edit_widget_props("ID_NORMAL_COLOR", test_constants.COLOR_BLACK)
    test_utils.edit_widget_props("ID_TEXT_ALIGN", 'Right')
    test_utils.add_widget('GX_TYPE_VERTICAL_SCROLL')
    
    #add numeric prompt
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_NUMERIC_PROMPT')
    test_utils.edit_widget_props("ID_XPOS", 333)
    test_utils.edit_widget_props("ID_YPOS", 309)
    test_utils.edit_widget_props("ID_WIDTH", 180)
    test_utils.edit_widget_props("ID_HEIGHT", 33)
    test_utils.edit_widget_props("ID_WIDGET_NAME", "numeric_prompt")
    test_utils.edit_widget_props("ID_BORDER_COMBO", test_constants.BORDER_TYPE_NONE)
    test_utils.edit_widget_props("ID_STRING_ID", "STRING_RIGHT_ALIGNMENT")
    test_utils.compare_result()
    
    test_utils.select_project_tree_node("numeric_prompt")
    test_utils.edit_widget_props("ID_NUMERIC_PROMPT_VALUE", 1000)
    test_utils.edit_widget_props("ID_NUMERIC_PROMPT_VALUE", 2000)
    test_utils.compare_result()
    
    test_utils.undo()
    
    #add numericpixelmap prompt
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_NUMERIC_PIXELMAP_PROMPT')
    test_utils.edit_widget_props("ID_WIDGET_NAME", "numeric_pixelmap_prompt")
    test_utils.edit_widget_props("ID_XPOS", 334)
    test_utils.edit_widget_props("ID_YPOS", 356)
    test_utils.edit_widget_props("ID_WIDTH", 179)
    test_utils.edit_widget_props("ID_HEIGHT", 48)
    test_utils.edit_widget_props("ID_BORDER_COMBO", test_constants.BORDER_TYPE_NONE)
    test_utils.edit_widget_props("ID_STRING_VAL", "000")
    test_utils.edit_widget_props("ID_PIXELMAP_0", test_constants.PIXELMAP_LEFT)
    test_utils.edit_widget_props("ID_PIXELMAP_1", test_constants.PIXELMAP_FILL)
    test_utils.edit_widget_props("ID_PIXELMAP_2", test_constants.PIXELMAP_RIGHT)
    test_utils.compare_result()
    
    test_utils.select_project_tree_node("numeric_pixelmap_prompt")
    test_utils.edit_widget_props("ID_NUMERIC_PROMPT_VALUE", 1000)
    test_utils.edit_widget_props("ID_NUMERIC_PROMPT_VALUE", 2000)
    test_utils.compare_result()
    
    test_utils.undo()
    
def add_window_screen():
    #---------------------------------------#
    #add window screen to display           #
    #---------------------------------------#
    
    test_utils.select_project_tree_node(folder_name)
    test_utils.add_widget('GX_TYPE_WINDOW')
    screen_name = "window_screen"
    
    test_utils.edit_widget_props("ID_XPOS", 0)
    test_utils.edit_widget_props("ID_YPOS", 0)
    test_utils.edit_widget_props("ID_WIDTH", 640)
    test_utils.edit_widget_props("ID_HEIGHT", 480)
    
    test_utils.edit_widget_props("ID_WIDGET_NAME", screen_name)
    test_utils.edit_widget_props("ID_BORDER_COMBO", test_constants.BORDER_TYPE_THICK)
    
    #test Name check of properties window
    test_utils.edit_widget_props_post("ID_WIDGET_NAME", bad_name)
    test_utils.select_project_tree_node(screen_name)
    test_utils.close_message_dialog()
    test_utils.edit_widget_props_post("ID_WIDGET_ID", bad_name)
    test_utils.select_project_tree_node(screen_name)
    test_utils.close_message_dialog()
    test_utils.edit_widget_props_post("ID_EVENT_FUNCTION", bad_name)
    test_utils.select_project_tree_node(screen_name)
    test_utils.close_message_dialog()
    test_utils.edit_widget_props_post("ID_DRAW_FUNCTION", bad_name)
    test_utils.select_project_tree_node(screen_name)
    test_utils.close_message_dialog()
    
    #add vertical list
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_VERTICAL_LIST')
    test_utils.edit_widget_props("ID_WIDGET_NAME", "vertical_list")
    test_utils.edit_widget_props("ID_XPOS", 34)
    test_utils.edit_widget_props("ID_YPOS", 215)
    test_utils.edit_widget_props("ID_WIDTH", 102)
    test_utils.edit_widget_props("ID_HEIGHT", 144)
    
    test_utils.add_widget('GX_TYPE_TEXT_BUTTON')
    test_utils.edit_widget_props("ID_BORDER_COMBO", test_constants.BORDER_TYPE_RAISED)
    test_utils.select_project_tree_node("vertical_list")
    test_utils.add_widget('GX_TYPE_TEXT_BUTTON')
    test_utils.edit_widget_props("ID_BORDER_COMBO", test_constants.BORDER_TYPE_RAISED)
    test_utils.select_project_tree_node("vertical_list")
    test_utils.add_widget('GX_TYPE_TEXT_BUTTON')
    test_utils.edit_widget_props("ID_BORDER_COMBO", test_constants.BORDER_TYPE_RAISED)
    test_utils.select_project_tree_node("vertical_list")
    test_utils.add_widget('GX_TYPE_TEXT_BUTTON')
    test_utils.edit_widget_props("ID_BORDER_COMBO", test_constants.BORDER_TYPE_RAISED)
    
    #add horizontal list
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_HORIZONTAL_LIST')
    test_utils.edit_widget_props("ID_WIDGET_NAME", "horizontal_list")
    test_utils.edit_widget_props("ID_XPOS", 157)
    test_utils.edit_widget_props("ID_YPOS", 213)
    test_utils.edit_widget_props("ID_WIDTH", 172)
    test_utils.edit_widget_props("ID_HEIGHT", 64)
    
    test_utils.add_widget('GX_TYPE_ICON_BUTTON')
    test_utils.edit_widget_props("ID_BORDER_COMBO", test_constants.BORDER_TYPE_RAISED)
    test_utils.edit_widget_props("ID_PIXELMAP_0", test_constants.PIXELMAP_HISTORY)
    test_utils.select_project_tree_node("horizontal_list")
    test_utils.add_widget('GX_TYPE_ICON_BUTTON')
    test_utils.edit_widget_props("ID_BORDER_COMBO", test_constants.BORDER_TYPE_RAISED)
    test_utils.edit_widget_props("ID_PIXELMAP_0", test_constants.PIXELMAP_SAVE_ICON)
    test_utils.select_project_tree_node("horizontal_list")
    test_utils.add_widget('GX_TYPE_ICON_BUTTON')
    test_utils.edit_widget_props("ID_BORDER_COMBO", test_constants.BORDER_TYPE_RAISED)
    test_utils.edit_widget_props("ID_PIXELMAP_0", test_constants.PIXELMAP_HISTORY)
    test_utils.select_project_tree_node("horizontal_list")
    test_utils.add_widget('GX_TYPE_ICON_BUTTON')
    test_utils.edit_widget_props("ID_BORDER_COMBO", test_constants.BORDER_TYPE_RAISED)
    test_utils.edit_widget_props("ID_PIXELMAP_0", test_constants.PIXELMAP_SAVE_ICON)
        
    #add drop list
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_DROP_LIST')
    test_utils.edit_widget_props("ID_WIDGET_NAME", "drop_list")
    test_utils.edit_widget_props("ID_WIDGET_ID", "ID_DROP_LIST")
    test_utils.edit_widget_props("ID_XPOS", 154)
    test_utils.edit_widget_props("ID_YPOS", 304)
    test_utils.edit_widget_props("ID_WIDTH", 207)
    test_utils.edit_widget_props("ID_HEIGHT", 29)
    test_utils.edit_widget_props("ID_LIST_ROWS", 50)
    test_utils.edit_widget_props("ID_OPEN_HEIGHT", 100)
    test_utils.edit_widget_props("ID_CALLBACK_NAME", "drop_list_row_create")
    test_utils.edit_widget_props("ID_PIXELMAP_1", test_constants.PIXELMAP_ORANGE_MIDDLE_HORIZONTAL)
    test_utils.edit_widget_props("ID_TILE_DROP_LIST_BACKGROUND", 1)
    test_utils.compare_result()
    test_utils.edit_widget_props("ID_PIXELMAP_0", test_constants.PIXELMAP_GAUGEMETER)
    test_utils.edit_widget_props("ID_TILE_DROP_LIST_POPLIST_WALLPAPER", 0)
    test_utils.compare_result()
    
    #add child widget to drop list
    test_utils.select_project_tree_node(screen_name)
    test_utils.select_project_tree_node("drop_list")
    test_utils.add_widget('GX_TYPE_PIXELMAP_BUTTON')
    test_utils.edit_widget_props("ID_WIDGET_NAME", "drop_list_button")
    test_utils.edit_widget_props("ID_XPOS", 340)
    test_utils.edit_widget_props("ID_YPOS", 305)
    test_utils.edit_widget_props("ID_WIDTH", 20)
    test_utils.edit_widget_props("ID_HEIGHT", 28)
    test_utils.edit_widget_props("ID_PIXELMAP_0", test_constants.PIXELMAP_SAVE_ICON)
    
    #add scrollable frame
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_WINDOW')
    test_utils.edit_widget_props("ID_WIDGET_NAME", "scroll_frame")
    test_utils.edit_widget_props("ID_XPOS", 33)
    test_utils.edit_widget_props("ID_YPOS", 17)
    test_utils.edit_widget_props("ID_WIDTH", 212)
    test_utils.edit_widget_props("ID_HEIGHT", 136)
    
    test_utils.add_widget('GX_TYPE_WINDOW')
    test_utils.edit_widget_props("ID_WIDGET_NAME", "client_window")
    test_utils.edit_widget_props("ID_XPOS", 33)
    test_utils.edit_widget_props("ID_YPOS", 17)
    test_utils.edit_widget_props("ID_WIDTH", 400)
    test_utils.edit_widget_props("ID_HEIGHT", 500)
    test_utils.edit_widget_props("ID_PIXELMAP_0", test_constants.PIXELMAP_FISH)
    
    test_utils.select_project_tree_node("scroll_frame")
    test_utils.add_widget('GX_TYPE_VERTICAL_SCROLL')
    test_utils.edit_widget_props("ID_WIDGET_NAME", "v_scroll")
    
    test_utils.select_project_tree_node("scroll_frame")
    test_utils.add_widget('GX_TYPE_HORIZONTAL_SCROLL')
    test_utils.edit_widget_props("ID_WIDGET_NAME", "h_scroll")
    
    test_utils.compare_result()
    
    widgets = ["h_scroll", "v_scroll"]
    fill_pixelmaps = {
        widgets[0]: test_constants.PIXELMAP_FILL,
        widgets[1]: test_constants.PIXELMAP_ORANGE_MIDDLE
    }
    
    thumb_pixelmaps = {
        widgets[0]: test_constants.PIXELMAP_INDICATOR_HORIZONTAL,
        widgets[1]: test_constants.PIXELMAP_INDICATOR
    }
    
    left_pixelmaps = {
        widgets[0]: test_constants.PIXELMAP_LEFT,
        widgets[1]: test_constants.PIXELMAP_EMPTY_MIDDLE
    }
    
    right_pixelmaps = {
        widgets[0]: test_constants.PIXELMAP_RIGHT,
        widgets[1]: test_constants.PIXELMAP_EMPTY_MIDDLE
    }

    #edit scrollbar properties
    for widget in widgets:
        test_utils.select_project_tree_node(widget)
        test_utils.edit_widget_props("ID_NORMAL_COLOR", test_constants.COLOR_WHITE)
        test_utils.compare_result()
        
        #set background pixelmap
        test_utils.select_project_tree_node(widget)
        test_utils.edit_widget_props("ID_PIXELMAP_0", fill_pixelmaps[widget])
        test_utils.compare_result()
        
        #check on "Tile Background"
        test_utils.select_project_tree_node(widget)
        test_utils.edit_widget_props("ID_SCROLL_TILE_BKGND", 1)
        test_utils.compare_result()
        
        #set thumb pixelmap
        test_utils.select_project_tree_node(widget)
        test_utils.edit_widget_props("ID_PIXELMAP_1", thumb_pixelmaps[widget])
        test_utils.compare_result()
        
        #set left pixelmap
        test_utils.select_project_tree_node(widget)
        test_utils.edit_widget_props("ID_PIXELMAP_2", left_pixelmaps[widget])
        test_utils.compare_result()
        
        #set right pixelmap
        test_utils.select_project_tree_node(widget)
        test_utils.edit_widget_props("ID_PIXELMAP_3", right_pixelmaps[widget])
        test_utils.compare_result()
        
        #check off "End Buttons"
        test_utils.select_project_tree_node(widget)
        test_utils.edit_widget_props("ID_SCROLL_END_BUTTONS", 0)
        test_utils.compare_result()
        
        #check off "Relative Thumb Size"
        test_utils.select_project_tree_node(widget)
        test_utils.edit_widget_props("ID_SCROLL_RELATIVE_THUMB", 0)
        test_utils.compare_result()

        #check on "Transparent"
        #test_utils.edit_widget_props("ID_TRANSPARENT", 1)
        #test_utils.compare_result()
        
    #add vertical pixelmap slider
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_PIXELMAP_SLIDER')
    test_utils.edit_widget_props("ID_XPOS", 291)
    test_utils.edit_widget_props("ID_YPOS", 24)
    test_utils.edit_widget_props("ID_WIDTH", 29)
    test_utils.edit_widget_props("ID_HEIGHT", 121)
    test_utils.edit_widget_props("ID_SLIDER_VERTICAL", 1)
    test_utils.edit_widget_props("ID_TRANSPARENT", 1)
    test_utils.edit_widget_props("ID_SLIDER_MINVAL", 0)
    test_utils.edit_widget_props("ID_SLIDER_MAXVAL", 100)
    test_utils.edit_widget_props("ID_SLIDER_CURRENTVAL", 30)
    test_utils.edit_widget_props("ID_SLIDER_INCREMENT", 10)
    test_utils.edit_widget_props("ID_SLIDER_MINTRAVEL", 12)
    test_utils.edit_widget_props("ID_SLIDER_MAXTRAVEL", 13)
    test_utils.edit_widget_props("ID_SLIDER_NEEDLE_INSET", -2)
    test_utils.edit_widget_props("ID_NEEDLE_HOTSPOT", 10)
    test_utils.edit_widget_props("ID_SLIDER_TILE_BKGND", 1)
    test_utils.edit_widget_props("ID_PIXELMAP_0", test_constants.PIXELMAP_ORANGE_MIDDLE)
    test_utils.edit_widget_props("ID_PIXELMAP_1", test_constants.PIXELMAP_EMPTY_MIDDLE)
    test_utils.edit_widget_props("ID_PIXELMAP_2", test_constants.PIXELMAP_INDICATOR)
    
    #add vertical slider
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_SLIDER')
    test_utils.edit_widget_props("ID_XPOS", 346)
    test_utils.edit_widget_props("ID_YPOS", 22)
    test_utils.edit_widget_props("ID_WIDTH", 34)
    test_utils.edit_widget_props("ID_HEIGHT", 140)
    test_utils.edit_widget_props("ID_SLIDER_VERTICAL", 1)
    test_utils.edit_widget_props("ID_SHOW_TICKMARKS", 1)
    test_utils.edit_widget_props("ID_TICKMARK_COUNT", 9)
    test_utils.edit_widget_props("ID_SLIDER_MINVAL", 0)
    test_utils.edit_widget_props("ID_SLIDER_MAXVAL", 100)
    test_utils.edit_widget_props("ID_SLIDER_CURRENTVAL", 30)
    test_utils.edit_widget_props("ID_SLIDER_INCREMENT", 10)
    test_utils.edit_widget_props("ID_SLIDER_MINTRAVEL", 12)
    test_utils.edit_widget_props("ID_SLIDER_MAXTRAVEL", 13)
    test_utils.edit_widget_props("ID_SLIDER_NEEDLE_WIDTH", 20)
    test_utils.edit_widget_props("ID_SLIDER_NEEDLE_HEIGHT", 5)
    test_utils.edit_widget_props("ID_SLIDER_NEEDLE_INSET", 5)
    test_utils.edit_widget_props("ID_NEEDLE_HOTSPOT", 2)
    
    #add horizontal slider
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_SLIDER')
    test_utils.edit_widget_props("ID_XPOS", 418)
    test_utils.edit_widget_props("ID_YPOS", 34)
    test_utils.edit_widget_props("ID_WIDTH", 184)
    test_utils.edit_widget_props("ID_HEIGHT", 37)
    test_utils.edit_widget_props("ID_SLIDER_VERTICAL", 0)
    test_utils.edit_widget_props("ID_TICKMARK_COUNT", 9)
    test_utils.edit_widget_props("ID_SLIDER_MINVAL", 0)
    test_utils.edit_widget_props("ID_SLIDER_MAXVAL", 100)
    test_utils.edit_widget_props("ID_SLIDER_CURRENTVAL", 30)
    test_utils.edit_widget_props("ID_SLIDER_INCREMENT", 10)
    test_utils.edit_widget_props("ID_SLIDER_MINTRAVEL", 12)
    test_utils.edit_widget_props("ID_SLIDER_MAXTRAVEL", 13)
    test_utils.edit_widget_props("ID_SLIDER_NEEDLE_WIDTH", 5)
    test_utils.edit_widget_props("ID_SLIDER_NEEDLE_HEIGHT", 20)
    test_utils.edit_widget_props("ID_SLIDER_NEEDLE_INSET", 5)
    test_utils.edit_widget_props("ID_NEEDLE_HOTSPOT", 2)
    
    #add horizontal pixelmap slider
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_PIXELMAP_SLIDER')
    test_utils.edit_widget_props("ID_XPOS", 424)
    test_utils.edit_widget_props("ID_YPOS", 96)
    test_utils.edit_widget_props("ID_WIDTH", 161)
    test_utils.edit_widget_props("ID_HEIGHT", 38)
    test_utils.edit_widget_props("ID_SLIDER_VERTICAL", 0)
    test_utils.edit_widget_props("ID_TRANSPARENT", 1)
    test_utils.edit_widget_props("ID_SLIDER_MINVAL", 0)
    test_utils.edit_widget_props("ID_SLIDER_MAXVAL", 100)
    test_utils.edit_widget_props("ID_SLIDER_CURRENTVAL", 80)
    test_utils.edit_widget_props("ID_SLIDER_INCREMENT", 10)
    test_utils.edit_widget_props("ID_SLIDER_MINTRAVEL", 12)
    test_utils.edit_widget_props("ID_SLIDER_MAXTRAVEL", 13)
    test_utils.edit_widget_props("ID_SLIDER_NEEDLE_INSET", 4)
    test_utils.edit_widget_props("ID_NEEDLE_HOTSPOT", 10)
    test_utils.edit_widget_props("ID_SLIDER_TILE_BKGND", 1)
    test_utils.edit_widget_props("ID_PIXELMAP_0", test_constants.PIXELMAP_ORANGE_MIDDLE_HORIZONTAL)
    test_utils.edit_widget_props("ID_PIXELMAP_1", test_constants.PIXELMAP_EMPTY_MIDDLE_HORIZONTAL)
    test_utils.edit_widget_props("ID_PIXELMAP_2", test_constants.PIXELMAP_INDICATOR_HORIZONTAL)
    
    #add progress bar
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_PROGRESS_BAR')
    test_utils.edit_widget_props("ID_XPOS", 422)
    test_utils.edit_widget_props("ID_YPOS",151)
    test_utils.edit_widget_props("ID_WIDTH", 166)
    test_utils.edit_widget_props("ID_HEIGHT", 39)
    test_utils.edit_widget_props("ID_PROGRESS_SHOW_TEXT", 1)
    test_utils.edit_widget_props("ID_PROGRESS_PERCENT",1)
    test_utils.edit_widget_props("ID_PROGRESS_MINVAL", 0)
    test_utils.edit_widget_props("ID_PROGRESS_MAXVAL", 100)
    test_utils.edit_widget_props("ID_PROGRESS_CURRENTVAL", 50)
    
    #add vertical progress bar
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_PROGRESS_BAR')
    test_utils.edit_widget_props("ID_XPOS", 424)
    test_utils.edit_widget_props("ID_YPOS",204)
    test_utils.edit_widget_props("ID_WIDTH", 36)
    test_utils.edit_widget_props("ID_HEIGHT", 95)
    test_utils.edit_widget_props("ID_PROGRESS_VERTICAL", 1)
    test_utils.edit_widget_props("ID_PROGRESS_SHOW_TEXT", 1)
    test_utils.edit_widget_props("ID_PROGRESS_PERCENT",0)
    test_utils.edit_widget_props("ID_PROGRESS_MINVAL", 0)
    test_utils.edit_widget_props("ID_PROGRESS_MAXVAL", 100)
    test_utils.edit_widget_props("ID_PROGRESS_CURRENTVAL", 30)
    
    #add gauge
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_CIRCULAR_GAUGE')
    test_utils.edit_widget_props("ID_XPOS", 408)
    test_utils.edit_widget_props("ID_YPOS",277)
    test_utils.edit_widget_props("ID_WIDTH", 280)
    test_utils.edit_widget_props("ID_HEIGHT", 280)
    test_utils.edit_widget_props("ID_TRANSPARENT", 1)
    test_utils.edit_widget_props("ID_GAUGE_START_ANGLE", 0)
    test_utils.edit_widget_props("ID_GAUGE_ANIMATION_STEPS", 30)
    test_utils.edit_widget_props("ID_GAUGE_ANIMATION_DELAY", 1)
    test_utils.edit_widget_props("ID_GAUGE_NEEDLE_XPOS", 140)
    test_utils.edit_widget_props("ID_GAUGE_NEEDLE_YPOS", 140)
    test_utils.edit_widget_props("ID_GAUGE_NEEDLE_XCOR", 20)
    test_utils.edit_widget_props("ID_GAUGE_NEEDLE_YCOR", 99)
    test_utils.edit_widget_props("ID_PIXELMAP_0", test_constants.PIXELMAP_GAUGEMETER)
    test_utils.edit_widget_props("ID_PIXELMAP_1", test_constants.PIXELMAP_GAUGEMETER)
    test_utils.edit_widget_props("ID_PIXELMAP_2", test_constants.PIXELMAP_NEEDLE)
    
    #add sprite
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_SPRITE')
    test_utils.edit_widget_props("ID_XPOS", 252)
    test_utils.edit_widget_props("ID_YPOS", 367)
    test_utils.edit_widget_props("ID_WIDTH", 117)
    test_utils.edit_widget_props("ID_HEIGHT", 101)
    
    #add radial progress bar
    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_RADIAL_PROGRESS_BAR')
    test_utils.edit_widget_props("ID_RADIAL_PROGRESS_XCENTER", 94)
    test_utils.edit_widget_props("ID_RADIAL_PROGRESS_YCENTER", 397)
    test_utils.edit_widget_props("ID_RADIAL_PROGRESS_RADIUS", 50)
    test_utils.edit_widget_props("ID_RADIAL_PROGRESS_ANCHORVAL", 60)
    test_utils.edit_widget_props("ID_RADIAL_PROGRESS_CURRENTVAL", -200)
    test_utils.edit_widget_props("ID_NORMAL_TEXT_COLOR", "red")
    test_utils.edit_widget_props("ID_RADIAL_PROGRESS_NORMAL_BRUSH_COLOR", "green")
    test_utils.edit_widget_props("ID_RADIAL_PROGRESS_SELECTED_BRUSH_COLOR", "red")
    test_utils.edit_widget_props("ID_RADIAL_PROGRESS_NORMAL_BRUSH_WIDTH", 18)
    test_utils.edit_widget_props("ID_RADIAL_PROGRESS_SELECTED_BRUSH_WIDTH", 16)
    
    test_utils.compare_result()
    
    test_utils.edit_widget_props("ID_PROGRESS_PERCENT", 0)
    test_utils.compare_result()
    
    test_utils.edit_widget_props("ID_PROGRESS_SHOW_TEXT", 0)
    test_utils.edit_widget_props("ID_RADIAL_PROGRESS_ALIAS", 0)
    test_utils.edit_widget_props("ID_RADIAL_PROGRESS_ROUND", 0)
    test_utils.compare_result()
    
    test_utils.edit_widget_props("ID_RADIAL_PROGRESS_ALIAS", 1)
    test_utils.edit_widget_props("ID_RADIAL_PROGRESS_ROUND", 1)
    test_utils.compare_result()

    test_utils.select_project_tree_node(screen_name)
    test_utils.add_widget('GX_TYPE_LINE_CHART')
    test_utils.edit_widget_props('ID_XPOS', 133)
    test_utils.edit_widget_props('ID_YPOS', 288)
    test_utils.edit_widget_props('ID_WIDTH', 252)
    test_utils.edit_widget_props('ID_HEIGHT', 74)
    test_utils.edit_widget_props("ID_TRANSPARENT", 1)
    test_utils.edit_widget_props('ID_CHART_MIN_VAL', 10)
    test_utils.edit_widget_props('ID_CHART_MAX_VAL', 90)
    test_utils.edit_widget_props('ID_CHART_LEFT_MARGIN', 10)
    test_utils.edit_widget_props('ID_CHART_TOP_MARGIN', 2)
    test_utils.edit_widget_props('ID_CHART_RIGHT_MARGIN', 30)
    test_utils.edit_widget_props('ID_CHART_BOTTOM_MARGIN', 15)
    test_utils.edit_widget_props('ID_CHART_MAX_COUNT', 200)
    test_utils.edit_widget_props('ID_CHART_AXIS_WIDTH', 5)
    test_utils.edit_widget_props('ID_CHART_LINE_WIDTH', 2)
    test_utils.edit_widget_props('ID_CHART_AXIS_COLOR', 'red')
    test_utils.edit_widget_props('ID_CHART_LINE_COLOR', 'green')
    
    test_utils.compare_result()

def run_string_table_sorting():

    #test sort by string id
    test_utils.edit_string()
    test_utils.sort_string(test_constants.STRING_SORT_TYPE_ID)
    test_utils.select_string(7)#STRING_16
    test_utils.edit_top_string("a right alignment")#change from "right_alignment" to "a right alignment"
    test_utils.select_string(7)
    test_utils.toggle_three_column_mode()
    test_utils.edit_bottom_string("zzz")
    test_utils.select_string(7)
    test_utils.edit_string_id('STRING_300')#change from "STRING_16" to "STRING_300"
    test_utils.select_string(7)
    test_utils.add_string()
    
    test_utils.save_string_edit()
    test_utils.select_project_tree_node("button_screen")
    test_utils.compare_result()

    #test sort by English
    test_utils.edit_string()
   
    test_utils.sort_string(test_constants.STRING_SORT_TYPE_ENGLISH)
    test_utils.select_string(44)#STRING_18
    test_utils.edit_string_id('CHECKBOX')#change from "STRING_18" to "CHECKBOX"
    test_utils.select_string(44)#CHECKBOX
    test_utils.toggle_three_column_mode()
    test_utils.edit_bottom_string('aaa')
    test_utils.select_string(44)#CHECKBOX
    test_utils.toggle_three_column_mode()
    test_utils.edit_top_string('a checkbox')
    test_utils.select_string(44)#CHECKBOX
    test_utils.add_string()
    
    test_utils.save_string_edit()
    test_utils.compare_result()
    
    #test sort by Chinese
    test_utils.edit_string()
    
    test_utils.sort_string(test_constants.STRING_SORT_TYPE_CHINESE)
    test_utils.select_string(31)#STRING_6
    test_utils.edit_string_id('RADIO_BUTTON')#changed from "STRING_6" to "RADIO_BUTTON"
    test_utils.select_string(31)#STRING_6
    test_utils.edit_top_string('A radio_button')#change from "radio_button" to "A radio_button"
    test_utils.select_string(31)#RADIO_BUTTON
    test_utils.toggle_three_column_mode()
    test_utils.edit_bottom_string('bbb')
    test_utils.select_string(31)#RADIO_BUTON
    test_utils.select_string(4)
    test_utils.delete_string()
    test_utils.save_string_edit()
    test_utils.compare_result()

    #test sort by reference count
    test_utils.edit_string()
    test_utils.sort_string(test_constants.STRING_SORT_TYPE_REFERENCE_COUNT)
    test_utils.select_string(51)#STRING_2
    test_utils.edit_string_id('TEXT_BUTTON')#change from "STRING_2" to "TEXT_BUTTON"
    test_utils.select_string(51)#TEXT_BUTTON
    test_utils.edit_top_string('a_text_button')#change from "text_button" to "a_text_button"
    test_utils.select_string(51)#TEXT_BUTTON
    test_utils.toggle_three_column_mode()
    test_utils.edit_bottom_string('ccc')
    test_utils.select_string(51)#TEXT_BUTTON
    test_utils.save_string_edit()
    test_utils.compare_result()
    
    test_utils.select_project_tree_node("prompt_1")
    test_utils.edit_widget_props("ID_STRING_ID", "TEXT_BUTTON")
    test_utils.compare_result()
    test_utils.edit_string()
    test_utils.select_string(60)#TEXT_BUTTON
    test_utils.edit_top_string('text_button')#change from "a_text_button" to "text_button"
    test_utils.save_string_edit()
    test_utils.compare_result()

#step 1: set a new text to a multi line text input widget
#step 2: edit table string to delete last string  
def run_ml_text_input_test():
    test_utils.select_project_tree_node(text_screen_name)
    test_utils.select_project_tree_node(ml_text_input_widget_name)
    
    test_utils.edit_widget_props("ID_STRING_VAL", "Test_string_for_testing_multi_line_text_input")
    
    #Open string editor
    test_utils.edit_string()
        
    #get string count
    #string_count = test_utils.get_string_count()
    
    #delete last string
    test_utils.select_string(38)
    test_utils.delete_string()
    
    #save changes
    test_utils.save_string_edit()

def get_test_header():
    notes =  "*          Studio Add/Edit Widget Test                *\n"
    notes += "*                                                     *\n"
    notes += "* Add and Edit all widget types                       *\n"    
    notes += "* Verify output files match golden file content       *\n"
    return notes
    
def run_add_widget(project_name, golden_files):

    test_utils.open_project(project_name)

    print('run widget add')
    add_button_screen()
    add_text_screen()
    add_window_screen()
    
    print('run multi-line text input test')
    run_ml_text_input_test()
    
    print("run string table sorting")
    run_string_table_sorting()
 
    test_utils.generate_all()
    test_utils.click_generate_button(1)
    test_utils.cmp_output_files(golden_files)
    
def run_add_widget_tests(generate, screenshot):

    test_utils.output_test_header(get_test_header())
    test_utils.setup(generate, screenshot, 'test_add_widget_golden_file')

    print('Running add-widgets at 16 565 format')
    run_add_widget("test_add_widget_16bpp.gxp", test_add_widget_16bpp_output_files)

    print('Running add-widgets at 8bpp palette format')
    run_add_widget("test_add_widget_8bpp.gxp", test_add_widget_8bpp_output_files)

    print('Running add-widgets at 24 bpp rgb format')
    run_add_widget("test_add_widget_24bpp.gxp", test_add_widget_24bpp_output_files)
    
    test_utils.write_end('Add Widgets Tests')
