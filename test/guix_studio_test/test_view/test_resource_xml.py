import os
import sys
import time
import test_utils
import test_constants
import logging

standalone_binres_load_16bpp_xml_files = [
"font_1bpp.xml",
"font_4bpp.xml",
"font_8bpp.xml",
"pixelmap_565rgb.xml",
"pixelmap_8bit_palette.xml",
"pixelmap_compress_4444argb.xml",
"pixelmap_raw_4444argb.xml",
"pixelmap_compress_alphamap.xml",
"pixelmap_raw_alphamap.xml"
]

standalone_binres_load_8bpp_xml_files = [
"pixelmap_folder_1.xml",
"pixelmap_fish.xml"
]

standalone_binres_load_extended_unicode_32bpp_xml_files = [
"font_old_italic_1bpp.xml",
"font_old_italic_4bpp.xml",
"font_old_italic_8bpp.xml",
"pixelmaps.xml"
]

def get_test_header():

    notes =  "*            Studio Resource XML Tests                *\n"
    notes += "*                                                     *\n"
    notes += "* Test Resource XML generation                        *\n"
    notes += "* Test Resource XML content                           *\n"
    return notes

def run_resource_xml_tests(generate, screenshot):

    test_utils.output_test_header(get_test_header()) 
    test_utils.setup(generate, screenshot, 'test_resource_xml')
    
    # Test Resource XML generation for project standalone_binres_load_16bpp
    test_utils.open_project('..\..\example_internal\standalone_binres_load_16bpp\standalone_binres_load_16bpp.gxp')
    test_utils.click_resource_item("RES_TYPE_FONT", "FONT_1BPP")
    test_utils.generate_xml(test_utils.DEFAULT_OUTPUT_FILE_PATH + "font_1bpp.xml")
    test_utils.click_resource_item("RES_TYPE_FONT", "FONT_4BPP")
    test_utils.generate_xml(test_utils.DEFAULT_OUTPUT_FILE_PATH + "font_4bpp.xml")
    test_utils.click_resource_item("RES_TYPE_FONT", "FONT_8BPP")
    test_utils.generate_xml(test_utils.DEFAULT_OUTPUT_FILE_PATH + "font_8bpp.xml")
    test_utils.click_pixelmap_folder("CUSTOM_PIXELMAP_FOLDER", "565rgb")
    test_utils.generate_xml(test_utils.DEFAULT_OUTPUT_FILE_PATH + "pixelmap_565rgb.xml")
    test_utils.click_pixelmap_folder("CUSTOM_PIXELMAP_FOLDER", "8bit palette")
    test_utils.generate_xml(test_utils.DEFAULT_OUTPUT_FILE_PATH + "pixelmap_8bit_palette.xml")
    test_utils.click_resource_item("RES_TYPE_PIXELMAP", "COMPRESS_4444ARGB")
    test_utils.generate_xml(test_utils.DEFAULT_OUTPUT_FILE_PATH + "pixelmap_compress_4444argb.xml")
    test_utils.click_resource_item("RES_TYPE_PIXELMAP", "RAW_4444ARGB")
    test_utils.generate_xml(test_utils.DEFAULT_OUTPUT_FILE_PATH + "pixelmap_raw_4444argb.xml")
    test_utils.click_resource_item("RES_TYPE_PIXELMAP", "COMPRESS_ALPHAMAP")
    test_utils.generate_xml(test_utils.DEFAULT_OUTPUT_FILE_PATH + "pixelmap_compress_alphamap.xml")
    test_utils.click_resource_item("RES_TYPE_PIXELMAP", "RAW_ALPHAMAP")
    test_utils.generate_xml(test_utils.DEFAULT_OUTPUT_FILE_PATH + "pixelmap_raw_alphamap.xml")

    test_utils.cmp_xml_files(standalone_binres_load_16bpp_xml_files, test_utils.DEFAULT_OUTPUT_FILE_PATH, '..\..\example_internal\standalone_binres_load_16bpp\\')

    # Test Resource XML generation for project standalone_binres_load_8bpp
    test_utils.open_project('..\..\example_internal\standalone_binres_load_8bpp\standalone_binres_load_8bpp.gxp')
    test_utils.click_pixelmap_folder("CUSTOM_PIXELMAP_FOLDER", "Folder_1")
    test_utils.generate_xml(test_utils.DEFAULT_OUTPUT_FILE_PATH + "pixelmap_folder_1.xml")
    test_utils.click_resource_item("RES_TYPE_PIXELMAP", "FISH")
    test_utils.generate_xml(test_utils.DEFAULT_OUTPUT_FILE_PATH + "pixelmap_fish.xml")

    test_utils.cmp_xml_files(standalone_binres_load_8bpp_xml_files, test_utils.DEFAULT_OUTPUT_FILE_PATH, '..\..\example_internal\standalone_binres_load_8bpp\\')

    # Test Resource XML generation for project standalone_binres_load_extended_unicode_32bpp
    test_utils.open_project('..\..\example_internal\standalone_binres_load_extended_unicode_32bpp\standalone_binres_load_extended_unicode_32bpp.gxp')
    test_utils.click_resource_item("RES_TYPE_FONT", "OLD_ITALIC_1BPP")
    test_utils.generate_xml(test_utils.DEFAULT_OUTPUT_FILE_PATH + "font_old_italic_1bpp.xml")
    test_utils.click_resource_item("RES_TYPE_FONT", "OLD_ITALIC_4BPP")
    test_utils.generate_xml(test_utils.DEFAULT_OUTPUT_FILE_PATH + "font_old_italic_4bpp.xml")
    test_utils.click_resource_item("RES_TYPE_FONT", "OLD_ITALIC_8BPP")
    test_utils.generate_xml(test_utils.DEFAULT_OUTPUT_FILE_PATH + "font_old_italic_8bpp.xml")
    test_utils.click_pixelmap_folder("CUSTOM_PIXELMAP_FOLDER", "Custom")
    test_utils.generate_xml(test_utils.DEFAULT_OUTPUT_FILE_PATH + "pixelmaps.xml")

    test_utils.cmp_xml_files(standalone_binres_load_extended_unicode_32bpp_xml_files, test_utils.DEFAULT_OUTPUT_FILE_PATH, '..\..\example_internal\standalone_binres_load_extended_unicode_32bpp\\')

    test_utils.write_end("Test Resource XML Generation")

