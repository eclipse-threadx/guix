# -*- coding: utf-8 -*-
import os
import sys
import time
import test_utils
import test_constants
import filecmp

test_resources_edit_16bpp_output_files = [
"test_resources_edit_16bpp_fish.c",
"test_resources_edit_16bpp_custom_maps.c",
"test_resources_edit_16bpp_resources.c",
"test_resources_edit_16bpp_resources.h"]

test_resources_edit_8bpp_output_files = [
"test_resources_edit_8bpp_fish.c",
"test_resources_edit_8bpp_custom_maps.c",
"test_resources_edit_8bpp_resources.c",
"test_resources_edit_8bpp_resources.h",]

test_resources_edit_24bpp_output_files = [
"test_resources_edit_24bpp_fish.c",
"test_resources_edit_24bpp_custom_maps.c",
"test_resources_edit_24bpp_resources.c",
"test_resources_edit_24bpp_resources.h",]

test_resources_edit_export_xliff_files = [
'EnglishtoJapanese_v1.2.xliff',
'EnglishtoJapanese_v2.0.xliff']

display_name = 'display_1'
folder_name = 'default_folder'
bad_name = 'bAd #1 name'                     # Name check test.

display_color_format = 'INVALID'

def run_font_edit():
    # open font folder
    test_utils.click_resource_group('FONT_GROUP')
    
    # test cancel button
    test_utils.add_font()
    test_utils.cancel_font_edit()
    
    # open font NotoSansHans-Regular.otf
    test_utils.add_font()   
    test_utils.set_font_path('..\\..\\fonts\\NotoSansHans-Regular.otf')
    test_utils.set_font_name(test_constants.FONT_NOTOSANSHANS)
    test_utils.set_font_format('GX_FONT_FORMAT_1BPP')
    test_utils.set_font_height(10)
    # parameters: page_index, page_enabled, range_start, range_end
    test_utils.set_page_range(0, 1, '0020', '007e')
    test_utils.save_font_edit()
    
    # add font Vera.ttf
    test_utils.add_font() 
    test_utils.set_font_path('..\\..\\fonts\\Vera.ttf')
    test_utils.set_font_name(test_constants.FONT_VERA)
    test_utils.set_font_format('GX_FONT_FORMAT_4BPP')
    test_utils.set_font_height(10)  
    test_utils.save_font_edit()
    
    # add font VeraBd.ttf
    test_utils.add_font()
    test_utils.set_font_path('..\\..\\fonts\\VeraBd.ttf')
    test_utils.set_font_name(test_constants.FONT_VERABD)
    test_utils.set_font_format('GX_FONT_FORMAT_8BPP')
    test_utils.set_font_height(100)
    test_utils.set_page_range(0, 1, '0030', '0030') 
    test_utils.save_font_edit()
    
    # add font VeraBI.ttf
    test_utils.add_font()  
    test_utils.set_font_path('..\\..\\fonts\\VeraBI.ttf')
    test_utils.set_font_name(test_constants.FONT_VERABI)
    test_utils.set_font_format('GX_FONT_FORMAT_1BPP')
    test_utils.set_font_height(50)
    test_utils.set_page_range(1, 1, '0100', '0110')
    test_utils.save_font_edit()
    
    #Name check and cancel check
    test_utils.edit_font(test_constants.FONT_VERABD)
    test_utils.set_font_path('..\\..\\fonts\\VeraBI.ttf')
    test_utils.cancel_font_edit()
    test_utils.edit_font(test_constants.FONT_VERABD)
    test_utils.set_font_name(test_constants.FONT_VERABI)
    test_utils.save_font_edit(test_utils.CLOSE_NO_WAIT)
    test_utils.close_error_dialog()
    test_utils.save_font_edit()
    
    #Test font path edit
    test_utils.add_font()
    test_utils.set_font_path('..\\..\\..\\..\\..\\..\\..\\..\\..\\..\\fonts\\VeraBI.ttf')
    test_utils.set_font_name("Test_font_path")
    test_utils.save_font_edit()
    print("Calling close_error_dialog second time")
    test_utils.close_error_dialog()
    print("back from close error dialog, calling delete font")
    test_utils.delete_font("Test_font_path")
    
    # cloce font folder
    test_utils.click_resource_group('FONT_GROUP')
 
def run_color_edit():
    # open color group folder
    test_utils.click_resource_group('COLOR_GROUP')
    
    # test cancel button
    test_utils.add_color()
    test_utils.cancel_color_edit()
    
    #---------------------------------------------------------#
    #          Add "green" color                              #
    #---------------------------------------------------------#
    
    # open color edit dialog
    test_utils.add_color()
    
    # edit color
    test_utils.set_color_name('green')
    test_utils.set_color_red(15)
    test_utils.set_color_green(235)
    test_utils.set_color_blue(16)
    test_utils.save_color_edit()
    
    #---------------------------------------------------------#
    #          Add "red" color                                #
    #---------------------------------------------------------#
    
    # open color edit dialog
    test_utils.add_color()
    
    # edit color
    test_utils.set_color_name('red')
    test_utils.set_color_hue(12)
    test_utils.set_color_sat(88)
    test_utils.set_color_lum(49)
    test_utils.save_color_edit()
    
    #---------------------------------------------------------#
    #         ID Name and cancel button check                 #
    #---------------------------------------------------------#
    test_utils.edit_color('green')
    test_utils.set_color_name(bad_name)
    test_utils.save_color_edit(test_utils.CLOSE_NO_WAIT)
    test_utils.close_message_dialog()
    test_utils.set_color_name('red')
    test_utils.save_color_edit(test_utils.CLOSE_NO_WAIT)
    test_utils.close_message_dialog()
    test_utils.save_color_edit()
    
    # close color group folder
    test_utils.click_resource_group('COLOR_GROUP')

def run_8bit_palette_color_edit():

    #open theme configure dialog
    test_utils.configure_themes()
    
    #open palette edit dialog
    test_utils.edit_palette()
    
    #change predefined palette entry from 45 to 47
    test_utils.set_predefined_palette_entry(47)
    
    #set green color to palette index 45
    test_utils.set_palette_color(45, 15, 235, 16)
    
    #set red color to palette index 46
    test_utils.set_palette_color(46, 235, 47, 15)
    
    test_utils.save_palette_edit()
    
    test_utils.save_theme_configure()
    
    # open color group folder
    test_utils.click_resource_group('COLOR_GROUP')
    
    # test cancel button
    test_utils.add_color()
    test_utils.cancel_color_edit()
    
    #---------------------------------------------------------#
    #          Add "green" color                              #
    #---------------------------------------------------------#
    
    # open color edit dialog
    test_utils.add_color()
    
    # edit color
    test_utils.set_color_name('green')
    test_utils.set_palette_color_index(45)
    test_utils.save_color_edit()
    
    #---------------------------------------------------------#
    #          Add "red" color                                #
    #---------------------------------------------------------#
    
    # open color edit dialog
    test_utils.add_color()
    
    # edit color
    test_utils.set_color_name('red')
    test_utils.set_palette_color_index(46)
    test_utils.save_color_edit()
    
    #---------------------------------------------------------#
    #         ID Name and cancel button check                 #
    #---------------------------------------------------------#
    test_utils.edit_color('green')
    test_utils.set_color_name(bad_name)
    test_utils.save_color_edit(test_utils.CLOSE_NO_WAIT)
    test_utils.close_message_dialog()
    test_utils.set_color_name('red')
    test_utils.save_color_edit(test_utils.CLOSE_NO_WAIT)
    test_utils.close_message_dialog()
    test_utils.save_color_edit()
    
    # close color group folder
    test_utils.click_resource_group('COLOR_GROUP')
    
#edit pixelmap add and edit
def run_pixelmap_edit():
    # add three pixelmap folder
    test_utils.add_pixelmap_folder()
    test_utils.save_folder_name_edit()
    
    test_utils.add_pixelmap_folder()
    test_utils.set_folder_name('folder_delete_test_1')
    test_utils.save_folder_name_edit()
    
    test_utils.add_pixelmap_folder()
    test_utils.set_folder_name('folder_test_1')
    test_utils.save_folder_name_edit()
    
    test_utils.add_pixelmap_folder()
    test_utils.save_folder_name_edit()
    
    test_utils.add_pixelmap_folder()
    test_utils.set_folder_name('folder_delete_test_2')
    test_utils.save_folder_name_edit()
    
    test_utils.add_pixelmap_folder()
    test_utils.set_folder_name('folder_test_2')
    test_utils.save_folder_name_edit()
    
    test_utils.add_pixelmap_folder()
    test_utils.set_folder_name('folder_test_3')
    test_utils.save_folder_name_edit()
    
    test_utils.add_pixelmaps('Custom', '..\\pixelmaps\\', 'fish.png')
    test_utils.add_pixelmaps('Custom', '..\\pixelmaps\\', 'Gaugemeter.png,i_emptyfill_middle.png')
    test_utils.add_pixelmaps('Custom', '..\\pixelmaps\\', 'i_emptyfill_middle_horizontal.png')
    test_utils.add_pixelmaps('folder_test_3', '..\\pixelmaps\\', 'i_history_lg.png,i_indicator.png')
    test_utils.add_pixelmaps('folder_test_3', '..\\pixelmaps\\', 'i_indicator_horizontal.png')
    test_utils.add_pixelmaps('folder_test_3', '..\\pixelmaps\\', 'i_orangefill_middle.png')
    test_utils.add_pixelmaps('folder_test_2', '..\\pixelmaps\\', 'i_orangefill_middle_horizontal.png')
    test_utils.add_pixelmaps('folder_test_2', '..\\pixelmaps\\', 'needle.png')
    test_utils.add_pixelmaps('folder_test_2', '..\\pixelmaps\\', 'orange_button.png')
    test_utils.add_pixelmaps('folder_test_2', '..\\pixelmaps\\', 'save_icon.png')
    test_utils.add_pixelmaps('folder_test_2', '..\\pixelmaps\\', 'tfield_fill_small.png')
    test_utils.add_pixelmaps('folder_test_1', '..\\pixelmaps\\', 'tfield_left_small.png')
    test_utils.add_pixelmaps('folder_test_1', '..\\pixelmaps\\', 'tfield_right_small.png')
    test_utils.add_pixelmaps('folder_test_1', '..\\pixelmaps\\', 'top_bar_background.png')
    test_utils.add_pixelmaps('folder_test_1', '..\\pixelmaps\\', 'radiobutton_on.png')
    test_utils.add_pixelmaps('folder_test_1', '..\\pixelmaps\\', 'i_orangefill_left.png')
    test_utils.add_pixelmaps('folder_test_1', '..\\pixelmaps\\', 'i_orangefill_bottom.png')
    test_utils.add_pixelmaps('folder_test_1', '..\\pixelmaps\\', 'i_medicationsred_lg.png')

    test_utils.add_pixelmaps('folder_delete_test_2', '..\\pixelmaps\\', 'i_emptyfill_top.png')
    test_utils.add_pixelmaps('folder_delete_test_2', '..\\pixelmaps\\', 'i_emptyfill_right.png')
    test_utils.add_pixelmaps('folder_delete_test_2', '..\\pixelmaps\\', 'horizontal_fill_bkgnd.png')
    test_utils.add_pixelmaps('folder_delete_test_1', '..\\pixelmaps\\', 'checkbox_on.png')
    test_utils.add_pixelmaps('folder_delete_test_1', '..\\pixelmaps\\', 'checkbox_off.png')
    test_utils.add_pixelmaps('folder_delete_test_1', '..\\pixelmaps\\', 'button_disabled.png')
    test_utils.add_pixelmaps('folder_delete_test_1', '..\\pixelmaps\\', 'black_pause.png')
    
    test_utils.click_resource_group('PIXELMAP_GROUP')
    
    test_utils.edit_pixelmap('FISH')
    test_utils.check_raw_format(0)
    test_utils.check_compress(0)
    test_utils.check_alpha(0)
    test_utils.check_dither(0)
    test_utils.check_custom_output(1)

    if display_color_format == 'GX_COLOR_FORMAT_565RGB':
        print('Setting output file to 16bpp_fish_maps.c')
        test_utils.set_custom_output_file('test_resources_edit_16bpp_fish.c')
        test_utils.set_output_format('GX_COLOR_FORMAT_565RGB')
    else:
        if display_color_format == 'GX_COLOR_FORMAT_24XRGB':
            print('Setting output file to 24bpp_fish_custom_maps.c')
            test_utils.set_custom_output_file('test_resources_edit_24bpp_fish.c')
            test_utils.set_output_format('GX_COLOR_FORMAT_565RGB')
        else:
            if display_color_format == 'GX_COLOR_FORMAT_8BIT_PALETTE':
                print('Setting output file to 8bpp_fish.c')
                test_utils.set_custom_output_file('test_resources_edit_8bpp_fish.c')

    test_utils.save_pixelmap_edit()
    
    test_utils.edit_pixelmap('GAUGEMETER')
    test_utils.check_raw_format(0)
    test_utils.check_compress(1)
    test_utils.check_alpha(1)
    test_utils.check_dither(0)
    test_utils.set_output_format('GX_COLOR_FORMAT_565RGB')
    test_utils.save_pixelmap_edit()
    
    test_utils.edit_pixelmap('I_EMPTYFILL_MIDDLE')
    test_utils.check_raw_format(0)
    test_utils.check_compress(0)
    test_utils.check_alpha(1)
    test_utils.check_dither(0)
    test_utils.set_output_format('GX_COLOR_FORMAT_565RGB')
    test_utils.save_pixelmap_edit()
    
    test_utils.edit_pixelmap('I_EMPTYFILL_MIDDLE_HORIZONTAL')
    test_utils.check_raw_format(0)
    test_utils.check_compress(0)
    test_utils.check_alpha(0)
    test_utils.check_dither(1)
    test_utils.set_output_format('GX_COLOR_FORMAT_8BIT_ALPHAMAP')
    test_utils.save_pixelmap_edit()
    
    test_utils.edit_pixelmap('I_HISTORY_LG')
    test_utils.check_raw_format(0)
    test_utils.check_compress(1)
    test_utils.check_alpha(1)
    test_utils.check_dither(0)
    test_utils.check_custom_output(1)

    if display_color_format == 'GX_COLOR_FORMAT_565RGB':
        print('Setting output file to 16bpp_custom_maps.c')
        test_utils.set_custom_output_file('test_resources_edit_16bpp_custom_maps.c')
    else:
        if display_color_format == 'GX_COLOR_FORMAT_24XRGB':
            print('Setting output file to 24bpp_custom_maps.c')
            test_utils.set_custom_output_file('test_resources_edit_24bpp_custom_maps.c')
        else:
            if display_color_format == 'GX_COLOR_FORMAT_8BIT_PALETTE':
                print('Setting output file to 8bpp_custom_maps.c')
                test_utils.set_custom_output_file('test_resources_edit_8bpp_custom_maps.c')

    test_utils.set_output_format('GX_COLOR_FORMAT_8BIT_PALETTE')
    test_utils.set_palette_type('PALETTE_TYPE_PRIVATE')
    test_utils.save_pixelmap_edit()
    
    test_utils.edit_pixelmap('I_INDICATOR')
    test_utils.check_raw_format(0)
    test_utils.check_compress(1)
    test_utils.check_alpha(1)
    test_utils.check_dither(1)
    test_utils.check_custom_output(1)

    if display_color_format == 'GX_COLOR_FORMAT_565RGB':
        print('Setting output file to 16bpp_custom_maps.c')
        test_utils.set_custom_output_file('test_resources_edit_16bpp_custom_maps.c')
    else:
        if display_color_format == 'GX_COLOR_FORMAT_24XRGB':
            print('Setting output file to 24bpp_custom_maps.c')
            test_utils.set_custom_output_file('test_resources_edit_24bpp_custom_maps.c')
        else:
            if display_color_format == 'GX_COLOR_FORMAT_8BIT_PALETTE':
                print('Setting output file to 8bpp_custom_maps.c')
                test_utils.set_custom_output_file('test_resources_edit_8bpp_custom_maps.c')

    test_utils.set_output_format('GX_COLOR_FORMAT_8BIT_PALETTE')
    test_utils.set_palette_type('PALETTE_TYPE_SHARED')
    test_utils.save_pixelmap_edit()
    
    test_utils.edit_pixelmap('I_INDICATOR_HORIZONTAL')
    test_utils.check_raw_format(0) 
    test_utils.check_compress(0)
    test_utils.check_alpha(1)
    test_utils.check_dither(1)
    test_utils.set_output_format('GX_COLOR_FORMAT_565RGB')
    test_utils.save_pixelmap_edit()
    
    test_utils.edit_pixelmap('I_ORANGEFILL_MIDDLE')
    test_utils.check_raw_format(0)
    test_utils.check_compress(1)
    test_utils.check_alpha(1)
    test_utils.check_dither(1)
    test_utils.set_output_format('GX_COLOR_FORMAT_565RGB')
    test_utils.save_pixelmap_edit()
    
    test_utils.edit_pixelmap('I_ORANGEFILL_MIDDLE_HORIZONTAL')
    test_utils.check_raw_format(1)
    test_utils.check_compress(1)
    test_utils.check_alpha(1)
    test_utils.check_dither(1)
    test_utils.set_output_format('GX_COLOR_FORMAT_565RGB')
    test_utils.save_pixelmap_edit()
    
    test_utils.edit_pixelmap('NEEDLE')
    test_utils.check_raw_format(1)
    test_utils.check_compress(1)
    test_utils.check_alpha(1)
    test_utils.check_dither(1)
    test_utils.set_output_format('GX_COLOR_FORMAT_565RGB')
    test_utils.save_pixelmap_edit()
    
    test_utils.edit_pixelmap('ORANGE_BUTTON')
    test_utils.check_raw_format(0)
    test_utils.check_compress(0)
    test_utils.check_alpha(1)
    test_utils.check_dither(0)
    test_utils.set_output_format('GX_COLOR_FORMAT_565RGB')
    test_utils.save_pixelmap_edit()
    
    test_utils.edit_pixelmap('SAVE_ICON')
    test_utils.check_raw_format(0)
    test_utils.check_compress(1)
    test_utils.check_alpha(0)
    test_utils.check_dither(0)
    test_utils.set_output_format('GX_COLOR_FORMAT_565RGB')
    test_utils.save_pixelmap_edit()
    
    test_utils.edit_pixelmap('TFIELD_FILL_SMALL')
    test_utils.check_raw_format(0)
    test_utils.check_compress(0)
    test_utils.check_alpha(1)
    test_utils.check_dither(1)
    test_utils.set_output_format('GX_COLOR_FORMAT_565RGB')
    test_utils.save_pixelmap_edit()
    
    test_utils.edit_pixelmap('TFIELD_LEFT_SMALL')
    test_utils.check_raw_format(0)
    test_utils.check_compress(1)
    test_utils.check_alpha(1)
    test_utils.check_dither(0)
    test_utils.set_output_format('GX_COLOR_FORMAT_565RGB')
    test_utils.save_pixelmap_edit()
    
    test_utils.edit_pixelmap('TFIELD_RIGHT_SMALL')
    test_utils.check_raw_format(0)
    test_utils.check_compress(1)
    test_utils.check_alpha(0)
    test_utils.check_dither(1)
    test_utils.set_output_format('GX_COLOR_FORMAT_565RGB')
    test_utils.save_pixelmap_edit()
    
    #Name check and cancel edit 
    test_utils.edit_pixelmap('FISH')
    test_utils.check_raw_format(0)
    test_utils.check_compress(1)
    test_utils.check_compress(1)
    test_utils.check_alpha(1)
    test_utils.check_dither(1)
    test_utils.set_output_format('GX_COLOR_FORMAT_32ARGB')
    test_utils.cancel_pixelmap_edit()
    test_utils.edit_pixelmap('FISH')
    test_utils.set_image_id_name(bad_name)
    test_utils.save_pixelmap_edit(test_utils.CLOSE_NO_WAIT)
    test_utils.close_message_dialog()
    test_utils.cancel_pixelmap_edit()
    test_utils.edit_pixelmap('FISH')
    test_utils.set_image_id_name('TFIELD_RIGHT_SMALL')
    test_utils.save_pixelmap_edit(test_utils.CLOSE_NO_WAIT)
    test_utils.close_error_dialog()
    test_utils.save_pixelmap_edit()

    
    test_utils.delete_pixelmap('BLACK_PAUSE')
    test_utils.delete_pixelmap('HORIZONTAL_FILL_BKGND')

    #delete a non-existent pixelmap:
    test_utils.delete_pixelmap('DOES_NOTT_EXIST');

    test_utils.disable_pixelmap('NEEDLE')
    test_utils.disable_pixelmap('SAVE_ICON')
    test_utils.enable_pixelmap('NEEDLE')
    
    test_utils.remove_pixelmap_folder('folder_delete_test_1')
    test_utils.remove_pixelmap_folder('folder_delete_test_2')

    test_utils.click_pixelmap_folder('CUSTOM_PIXELMAP_FOLDER', 'folder_test_1')
    test_utils.disable_pixelmap_folder()
    
    test_utils.click_resource_group('PIXELMAP_GROUP')
    
def run_string_edit():
    
    # test cancel button
    test_utils.edit_string()
    test_utils.cancel_string_edit()
    
    # add strings
    test_utils.edit_string()
    test_utils.sort_string(test_constants.STRING_SORT_TYPE_ENGLISH)
    test_utils.add_string()
    test_utils.edit_top_string('button')
    test_utils.toggle_three_column_mode()
    #按钮
    test_utils.edit_bottom_string('按钮')
    test_utils.increment_trans_language()
    #ボタン
    test_utils.edit_bottom_string('ボタン')
    test_utils.increment_trans_language()
    test_utils.toggle_three_column_mode()
    
    test_utils.add_string()
    test_utils.edit_top_string('text_button')
    
    test_utils.add_string()
    test_utils.edit_top_string('multi\rline\rbutton')
    
    test_utils.add_string()
    test_utils.edit_top_string('checkbox')
    test_utils.toggle_three_column_mode()
    #チェックボックス
    test_utils.edit_bottom_string('チェックボックス')
    test_utils.decrement_trans_language()
    #复选框
    test_utils.edit_bottom_string('复选框')
    test_utils.decrement_trans_language()
    test_utils.toggle_three_column_mode()
    
    test_utils.add_string()
    test_utils.edit_top_string('radio')
    
    test_utils.add_string()
    test_utils.edit_top_string('radio_button')
    
    test_utils.add_string()
    test_utils.edit_top_string('prompt')
    
    test_utils.add_string()
    test_utils.edit_top_string('transparent prompt with thin border')
    
    test_utils.add_string()
    test_utils.edit_top_string('transparent prompt with no border')
    
    test_utils.add_string()
    test_utils.edit_top_string('pixelmap prompt')
    
    test_utils.add_string()
    test_utils.edit_top_string('initial text')
    
    test_utils.add_string()
    test_utils.edit_top_string('text_view')
    
    test_utils.add_string()
    test_utils.edit_top_string('text view')
    
    test_utils.add_string()
    test_utils.edit_top_string('text input')
    
    test_utils.add_string()
    test_utils.edit_string_id('STRING_LEFT_ALIGNMENT')
    test_utils.edit_top_string('left\ralignment')
    
    test_utils.add_string()
    test_utils.edit_string_id('STRING_RIGHT_ALIGNMENT')
    test_utils.edit_top_string('right\ralignment')
    
    test_utils.save_string_edit()
    
    #test name exists
    test_utils.edit_string()
    test_utils.select_string(2)
    test_utils.edit_string_id('STRING_RIGHT_ALIGNMENT')
    test_utils.save_string_edit(test_utils.CLOSE_NO_WAIT)
    test_utils.close_error_dialog()
    test_utils.cancel_string_edit()
   
    #test bad name
    test_utils.edit_string()
    test_utils.select_string(4)
    test_utils.edit_string_id(bad_name)
    test_utils.save_string_edit(test_utils.CLOSE_NO_WAIT)
    test_utils.close_message_dialog()
    test_utils.cancel_string_edit()

    
    #test cancel settings
    test_utils.click_resource_group('STRING_GROUP')
    test_utils.edit_string()
    test_utils.select_string(4)
    test_utils.edit_string_id('test_cancel')
    test_utils.cancel_string_edit()
    
    # delete strings
    test_utils.edit_string()
    test_utils.select_string(2)
    test_utils.delete_string() 
    test_utils.select_string(4)
    test_utils.delete_string()
    
    # export xliff file
    test_utils.export_string()
    test_utils.cancel_string_export()
    
    test_utils.export_string()
    test_utils.set_string_export_src_language('English')
    test_utils.set_string_export_target_language('Japanese')
    test_utils.select_string_export_format('xliff')
    test_utils.set_xliff_version('version_1.2')
    test_utils.set_string_export_filename('EnglishtoJapanese_v1.2.xliff')
    test_utils.set_string_export_path(test_utils.DEFAULT_OUTPUT_FILE_PATH)
    test_utils.save_string_export()
    
    test_utils.export_string()
    test_utils.set_string_export_src_language('English')
    test_utils.set_string_export_target_language('Japanese')
    test_utils.select_string_export_format('xliff')
    test_utils.set_xliff_version('version_2.0')
    test_utils.set_string_export_filename('EnglishtoJapanese_v2.0.xliff')
    test_utils.set_string_export_path(test_utils.DEFAULT_OUTPUT_FILE_PATH)
    test_utils.save_string_export()
    
    test_utils.cmp_files(test_resources_edit_export_xliff_files, test_utils.compare_xliff_file)
    
    # import xliff file
    test_utils.import_string('EnglishtoChinese.xliff')
    
    test_utils.save_string_edit()

def run_new_project_create(project_name):

    # close current project
    # Menu '&Edit' and '&Insert' should be disabled when project is closed
    test_utils.close_project()
    passed = True
    test_type = 'Check menu status'
    error_msg = ''
    if test_utils.get_menu_status('&Edit') == test_constants.MENU_ENABLED:
        passed = False
        error_msg += 'menu "&Edit" should be disabled\n'
    if test_utils.get_menu_status('&Insert') == test_constants.MENU_ENABLED:
        passed = False
        error_msg += 'menu "&Insert" should be disabled\n'
    
    test_utils.set_test_results(passed, test_type, error_msg)
    
    # test cancel button
    test_utils.toolbar_create()
    test_utils.cancel_new_project_create()
    
    # create a new project
    test_utils.toolbar_create()
    test_utils.set_new_project_path(test_utils.DEFAULT_OUTPUT_FILE_PATH)
    test_utils.set_new_project_name(project_name)
    test_utils.save_new_project_create()
    
    # close message dialog when create new and delete default folder
    test_utils.close_message_dialog()
    test_utils.select_project_tree_node("default_folder")
    test_utils.delete_folder(0)
    
    # Menu '&Edit' and '&Insert' should be enabled when project is opened
    passed = True
    test_type = 'Check menu status'
    error_msg = ''
    if test_utils.get_menu_status('&Edit') == test_constants.MENU_DISABLED:
        passed = False
        error_msg += 'menu "&Edit" should be enabled\n'
    if test_utils.get_menu_status('&Insert') == test_constants.MENU_DISABLED:
        passed = False
        error_msg += 'menu "&Insert" should be enabled\n'
        
    test_utils.set_test_results(passed, test_type, error_msg)
    
def run_project_configure(display_format):

    # test cancel button
    test_utils.configure_project()
    test_utils.cancel_project_configure()
    
    # configure project
    test_utils.configure_project()
    test_utils.configure_x_res(640)
    test_utils.configure_y_res(480)
    test_utils.configure_display_name(display_name)
    test_utils.configure_display_color_format(display_format)
    test_utils.configure_minor_version(2)
    test_utils.configure_service_pack(5)
    test_utils.save_project_configure()
    
    test_utils.configure_project()
    test_utils.configure_display_name(bad_name)
    test_utils.save_project_configure(test_utils.CLOSE_NO_WAIT)
    test_utils.close_message_dialog()
    test_utils.cancel_project_configure()
    
    # calculate checksum
    test_utils.compare_result()
        
    #Create default folder for project
    test_utils.select_project_tree_node(display_name)
    test_utils.insert_folder()
    
# add "Chinese"
# add "Japanese"
def run_languges_configure():
    test_utils.configure_languages()

    #add three languages
    test_utils.add_language()
    test_utils.add_language()
    test_utils.add_language()
    
    #set the second language to "Chinese"
    test_utils.select_language_index(2)
    test_utils.select_language_id('Chinese')
    
    #set the third language to "Japanese"
    test_utils.select_language_index(3)
    test_utils.select_language_id('Japanese')
    
    #delete the fourth language
    test_utils.select_language_index(4)
    test_utils.delete_language()
    
    #save modifications
    test_utils.save_language_configure() 
    
def run_theme_configure():
    test_utils.configure_themes()
    test_utils.set_theme_name('theme_1')
    test_utils.save_theme_configure()
    
    test_utils.configure_themes()
    test_utils.set_theme_name(bad_name)
    test_utils.save_theme_configure(test_utils.CLOSE_NO_WAIT)
    test_utils.close_message_dialog()
    test_utils.cancel_theme_configure()   
    
def get_test_header():
    notes =  "*        Studio Add/Edit Resources Test               *\n"
    notes += "*                                                     *\n"
    notes += "* Add and Edit colors                                 *\n"
    notes += "* Add and Edit fonts                                  *\n"
    notes += "* Add and Edit pixelmaps                              *\n"
    notes += "* Add and Edit languages                              *\n"
    notes += "* Add and Edit strings                                *\n"
    notes += "* Verify output files match golden file content       *\n"
    return notes
    
def run_edit_resources(new_project_name, display_format, golden_files):

    global display_color_format
    
    print('create new project')
    run_new_project_create(new_project_name)
    
    print('configure project')
    run_project_configure(display_format)

    display_color_format = display_format
    
    print('configure theme')
    run_theme_configure()
    
    print('configure language')
    run_languges_configure()

    print('run color add and edit')
    
    if display_format == 'GX_COLOR_FORMAT_8BIT_PALETTE':
        run_8bit_palette_color_edit()
    else:
        run_color_edit()
    
    print('run font add and edit')
    run_font_edit()
    
    print('run pixelmap add and edit')
    run_pixelmap_edit()

    print('run string add and edit')
    run_string_edit()
    
    print('save project')
    test_utils.toolbar_save()
    
    test_utils.generate_all()
    test_utils.click_generate_button()
    test_utils.cmp_output_files(golden_files)
    
def run_resources_edit_tests(generate, screenshot):

    test_utils.output_test_header(get_test_header())
    test_utils.setup(generate, screenshot, 'test_resources_edit_golden_file')

    print('Running edit-resources at 16 565 format')
    run_edit_resources("test_resources_edit_16bpp", 'GX_COLOR_FORMAT_565RGB', test_resources_edit_16bpp_output_files)

    print('Running edit-resources at 8bpp palette format')
    run_edit_resources("test_resources_edit_8bpp", 'GX_COLOR_FORMAT_8BIT_PALETTE', test_resources_edit_8bpp_output_files)

    print('Running edit-resources at 24 bpp rgb format')
    run_edit_resources("test_resources_edit_24bpp", 'GX_COLOR_FORMAT_24XRGB', test_resources_edit_24bpp_output_files)
    
    test_utils.write_end('Edit Resources Tests')
