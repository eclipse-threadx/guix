import os
import sys
import argparse
import logging
import time
import search_gxps
import shutil
import glob
from compare_two_files import compare_output_files
from compare_two_files import compare
from compare_two_files import compare_output_folders
from set_project_lib_version import set_project_lib_version
from os.path import basename

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)) + '\\..\\test_view')
import test_utils

#count[total, passed, failed]
count_test=[0,0,0]
count_generate=[0, 0, 0]

multi_theme_project_names = ["multi_themes_16bpp.gxp", "multi_themes_332rgb.gxp"]
back_compatible_project_names = [
"all_widgets_5_2_0",
"all_widgets_5_2_5",
"all_widgets_5_3_0",
"all_widgets_5_3_2",
"all_widgets_5_3_3",
"all_widgets_5_3_4",
"all_widgets_5_4_0",
"all_widgets_5_4_1",
"all_widgets_5_4_2",
"all_widgets_5_5_1",
"all_widgets_synergy_5_4_2",
"multi_themes_16bpp_5_5_1",
"all_widgets_synergy_8bpp_6_1_3"
]

binres_only_project_names = [
'demo_guix_binres.gxp',
'all_widgets_16bpp_rotated_cw_binary.gxp',
'all_widgets_16bpp_rotated_ccw_binary.gxp',
'all_widgets_synergy_16bpp_rotated_cw_binary.gxp',
'all_widgets_synergy_16bpp_rotated_ccw_binary.gxp',
'all_widgets_8bpp_rotated_cw_binary.gxp',
'all_widgets_8bpp_rotated_ccw_binary.gxp',
'all_widgets_synergy_8bpp_rotated_cw_binary.gxp',
'all_widgets_synergy_8bpp_rotated_ccw_binary.gxp',
'all_widgets_24xrgb_rotated_cw_binary.gxp',
'all_widgets_24xrgb_rotated_ccw_binary.gxp',
'all_widgets_synergy_24xrgb_rotated_cw_binary.gxp',
'all_widgets_synergy_24xrgb_rotated_ccw_binary.gxp'
]

msbuild_exe_path = "C:/Windows/Microsoft.NET/Framework64/v4.0.30319/MSBuild.exe"
guix_lib = "../../../build/win32/msvc_2010/guix.vcxproj"

# compiler command line using gx_api.h in src directory
std_compile = "cl /c /nologo /I..\\..\\..\\common\\inc /I..\\..\\..\\ports\\win32\\inc /I..\\..\\..\\ports\\win32\\lib\\vs_2019"
# Set compile level4 to detect compile warning.
# But ignore warning C4132: const object should be initialized. The reason is about GX_CONST GX_STUDIO_WIDGET xxxx parameters in each specification.c.
compile_option = "/W4 /wd4132 /wd4100 /wd4244 /WX-"

compile_result =[0,0,0]

#Compile studio
def studio_compile(project_sln_path):
    #Ensure solution file exists
    if not os.path.isfile(project_sln_path):
        raise Exception('studiox.sln not found. path=' + project_sln_path)

    logging.debug("Building Studio Executable");
    if os.system("msbuild " + project_sln_path + " /p:Configuration=Release") != 0:
        logging.error("Studio Build Failed")
        return(-1)
    else:
        logging.debug("Studio Build Successful")
        return(0)

#Compile guix
def guix_compile(project_sln_path, configuration): 
    #Ensure solution file exists
    if not os.path.isfile(project_sln_path):
        raise Exception('guix.sln not found. path=' + project_sln_path)

    logging.debug("Building Guix library, Configuration=%s", configuration);
    if os.system("msbuild " + project_sln_path + " /p:Configuration=" + configuration) != 0:
        logging.error("Guix Build Failed")
        return(-1)
    else:
        logging.debug("Guix Build Successful")
        return(0)
    
def test_one_project(studio_release, file):
        project_pathname = os.path.abspath(file)
        (project_path, project_name) = os.path.split(project_pathname)
        project_path += '\\'
            
        logging.debug("===============================================================================")
        logging.info("Testing output files for project: %s", project_pathname)
        
        cmp_result = False
        
        test_file_list = os.listdir(project_path)
        
        if project_name == 'folder_output_test.gxp':
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -d MAIN_DISPLAY -r MAIN_DISPLAY_resources -s specifications -l English,Japanese")
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -d SECONDARY -r SECONDARY_resources -s specifications -l English,Chinese")
                
        elif (project_name == 'two_displays.gxp' or
              project_name == 'demo_display_resolution.gxp'): 
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -d MAIN_DISPLAY -r MAIN_DISPLAY_resources -s specifications")
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -d SECONDARY -r SECONDARY_resources -s specifications")

        elif project_name == 'two_displays_binres.gxp': 
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -b -d MAIN_DISPLAY -r MAIN_DISPLAY_resources -s specifications")
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -b -d SECONDARY -r SECONDARY_resources -s specifications")

        elif project_name in multi_theme_project_names:
                base_name = os.path.splitext(project_name)[0]
                #Gen "japanese.bin"
                os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -b --no_res_header -r temp_Japanese -l Japanese -t")
                #Gen "resource.bin": Including theme4, theme5, Spanish
                os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -b --no_res_header -r resources -t theme_4, theme_5 -l Spanish")
                #Gen "japanese_big_endian.bin"
                os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -b --big_endian --no_res_header -r temp_Japanese_big_endian -l Japanese -t")
                #Gen "resource_big_endian.bin"
                os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -b --big_endian --no_res_header -r resources_big_endian -t theme_4, theme_5 -l Spanish")
                
                #Gen "resource.c/.h" and "specifications.c/.h"
                os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -r resources -s specifications -t theme_1, theme_2, theme_3 -l English, Chinese")
                
                if project_name == 'multi_themes_16bpp.gxp':
                    #gen "resource_big_endian.c" and "resource_big_endian.h"
                    os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n --big_endian -r resources_big_endian -t theme_1, theme_2, theme_3 -l English, Chinese")
                    
        elif project_name == 'multi_themes_32bpp.gxp':
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -r resources -s specifications -b")
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n --big_endian -r resources_big_endian")
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -b --big_endian --no_res_header -r resources_big_endian")
        elif project_name == 'multi_themes_8bpp_palette.gxp':
            #Gen "resource.bin": Including theme4, theme5
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -b --no_res_header -r resources -t theme_4, theme_5 -l")
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -r resources -s specifications -t theme_1, theme_2, theme_3")
        elif project_name == 'multi_themes_16bpp_synergy.gxp':
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -b --no_res_header -r resources -t theme_4, theme_5")
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -r resources -s specifications -t theme_1, theme_2, theme_3")
        elif project_name == 'arabic_glyph_shaping.gxp':
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -r resources -s specifications -l English")
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -b --no_res_header -r resources -l Arabic -t")
        elif project_name == 'thai_glyph_shaping.gxp':
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -r resources -s specifications -l English")
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -b --no_res_header -r resources -l Thai -t")
        elif project_name == 'multi_themes_16bpp_5_5_1.gxp':
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -r resources -s specifications -t theme_1, theme_2, theme_3 -l English, Chinese")
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -b --no_res_header -r resources -t theme_4, theme_5, -l Japanese, Spanish")
        elif project_name == 'kerning_glyph_draw_32bpp_screen.gxp' or  project_name == 'kerning_glyph_in_string_table.gxp':
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -r resources -s specifications -t theme_1 -l English")
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -b --no_res_header -r resources -t theme_2")
        elif project_name == 'binres_theme_static_language.gxp':
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -b --no_res_header -r resources -t theme_1, theme_2 -l")
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -r resources -s specifications -t -l English,Spanish")
        elif project_name == 'bidi_text_draw_32bpp.gxp':
            #Gen "bidi_text_draw_32bpp_resources.bin"
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -b --no_res_header -r resources")
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -r resources -s specifications")
        elif project_name == 'bidi_text_line_break_static.gxp':
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -b --no_res_header -r resources -t theme_2 -l")
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -r resources -s specifications -t theme_1")
        elif project_name in binres_only_project_names:
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -b -r resources -s specifications")
        elif 'big_endian' in project_name:
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -r resources -s specifications --big_endian")
        elif project_name == 'standalone_binres_load_16bpp.gxp':
            os.system("mkdir " + project_path + "project_output_folder")
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n --output_path project_output_folder")
            os.system("mkdir " + project_path + "xml_output_folder")
            os.system(os.path.abspath(studio_release) + " -x " + project_path + "font_1bpp.xml -b --output_path   \"xml_output_folder\"   -o log.txt")
            os.system(os.path.abspath(studio_release) + " -x " + project_path + "font_4bpp.xml -b --output_path xml_output_folder  -o log.txt")
            os.system(os.path.abspath(studio_release) + " -x " + project_path + "font_8bpp.xml -b --output_path xml_output_folder  -o log.txt")
            os.system(os.path.abspath(studio_release) + " -x " + project_path + "pixelmap_8bit_palette.xml -b --output_path xml_output_folder -o log.txt")
            os.system(os.path.abspath(studio_release) + " -x " + project_path + "pixelmap_565rgb.xml -b --output_path xml_output_folder  -o log.txt")
            os.system(os.path.abspath(studio_release) + " -x " + project_path + "pixelmap_compress_4444argb.xml -b --output_path xml_output_folder  -o log.txt")
            os.system(os.path.abspath(studio_release) + " -x " + project_path + "pixelmap_compress_alphamap.xml -b --output_path xml_output_folder  -o log.txt")
            os.system(os.path.abspath(studio_release) + " -x " + project_path + "pixelmap_raw_4444argb.xml -b --output_path xml_output_folder  -o log.txt")
            os.system(os.path.abspath(studio_release) + " -x " + project_path + "pixelmap_raw_alphamap.xml -b --output_path xml_output_folder  -o log.txt")
        elif project_name == 'standalone_binres_load_8bpp.gxp':
            os.system("mkdir " + project_path + "project_output_folder")
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n --output_path project_output_folder")
            os.system("mkdir " + project_path + "xml_output_folder")
            os.system(os.path.abspath(studio_release) + " -x " + project_path + "pixelmap_folder_1.xml -b --output_path " + project_path + "xml_output_folder -o log.txt")
            os.system(os.path.abspath(studio_release) + " -x " + project_path + "pixelmap_fish.xml -b --output_path " + project_path + "xml_output_folder  -o log.txt")
        elif project_name == 'standalone_binres_load_extended_unicode_32bpp.gxp':
            os.system("mkdir " + project_path + "project_output_folder")
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n --output_path project_output_folder")
            os.system("mkdir " + project_path + "xml_output_folder")
            os.system(os.path.abspath(studio_release) + " -x " + project_path + "font_pld_italic_1bpp.xml -b --output_path " + project_path + "xml_output_folder -o log.txt")
            os.system(os.path.abspath(studio_release) + " -x " + project_path + "font_pld_italic_4bpp.xml -b --output_path " + project_path + "xml_output_folder  -o log.txt")
            os.system(os.path.abspath(studio_release) + " -x " + project_path + "font_pld_italic_8bpp.xml -b --output_path " + project_path + "xml_output_folder  -o log.txt")
            os.system(os.path.abspath(studio_release) + " -x " + project_path + "pixelmaps.xml -b --output_path " + project_path + "xml_output_folder  -o log.txt")
        else:
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -r resources -s specifications")

        # remove the temporary output files
        if (project_name == 'standalone_binres_load_16bpp.gxp' or project_name == 'standalone_binres_load_8bpp.gxp' or project_name == 'standalone_binres_load_extended_unicode_32bpp.gxp'):
            # check if log file is generated
            if os.path.isfile(project_path + "xml_output_folder\log.txt") == False:
                cmp_result = False
                logging.info("Log file is not generated")
            else:
                cmp_result = True
                # remove the log file
                os.remove(project_path + "xml_output_folder\log.txt")

            if cmp_result == True:
                cmp_result = compare_output_folders(project_path, project_path + "project_output_folder\\")
            if cmp_result == True:
                cmp_result = compare_output_folders(project_path, project_path + "xml_output_folder\\")

            #shutil.rmtree(project_path + "project_output_folder")
            #shutil.rmtree(project_path + "xml_output_folder")
        else:
            cmp_result = compare_output_files(project_path, test_file_list)

            remove_list = []
            remove_list.append(project_path + 'resources.c')
            remove_list.append(project_path + 'resources.h')
            remove_list.append(project_path + 'specifications.c')
            remove_list.append(project_path + 'specifications.h')
            
            if project_name in multi_theme_project_names:
                remove_list.append(project_path + 'resources.c')
                remove_list.append(project_path + 'resources.h') 
                remove_list.append(project_path + 'resources_big_endian.bin')
                remove_list.append(project_path + 'resources.bin')
                remove_list.append(project_path + 'temp_Japanese.bin')
                remove_list.append(project_path + 'temp_Japanese_big_endian.bin')
                    
                if project_name == 'multi_themes_16bpp.gxp':
                    remove_list.append(project_path + 'resources_big_endian.c')
                    remove_list.append(project_path + 'resources_big_endian.h')
            
            elif project_name == 'multi_themes_32bpp.gxp':
                remove_list.append(project_path + 'resources.bin')
                remove_list.append(project_path + 'resources_big_endian.bin')
                remove_list.append(project_path + 'resources_big_endian.c')
                remove_list.append(project_path + 'resources_big_endian.h')
                
            elif (project_name == 'arabic_glyph_shaping.gxp' or
                project_name == 'thai_glyph_shaping.gxp'):
                remove_list.append(project_path + 'resources.bin')
                
            elif (project_name == 'multi_themes_8bpp_palette.gxp' or
                project_name == 'multi_themes_16bpp_synergy.gxp' or
                project_name == 'kerning_glyph_draw_32bpp_screen.gxp' or
                project_name == 'kerning_glyph_in_string_table.gxp' or
                project_name == 'bidi_text_draw_32bpp.gxp' or
                project_name == 'bidi_text_line_break_static.gxp' or
                project_name == 'multi_themes_16bpp_5_5_1.gxp' or
                project_name == 'binres_theme_static_language.gxp' or
                project_name in binres_only_project_names):
                remove_list.append(project_path + 'resources.bin')

            elif project_name == 'folder_output_test.gxp':
                remove_list.remove(project_path + 'resources.c')
                remove_list.remove(project_path + 'resources.h')
                remove_list.append(project_path + 'main_display_resources.c')
                remove_list.append(project_path + 'main_display_resources.h')
                remove_list.append(project_path + 'secondary_resources.c')
                remove_list.append(project_path + 'secondary_resources.h')
                remove_list.append(project_path + 'specifications.c')
                remove_list.append(project_path + 'specifications.h')
                remove_list.append(project_path + 'button_screen_specifications.c')
                remove_list.append(project_path + 'empty_folder_specifications.c')
                remove_list.append(project_path + 'indicator_screen_runtime_allocate_specifications.c')
                remove_list.append(project_path + 'popup_modal_specifications.c')
                remove_list.append(project_path + 'static_screens_specifications.c')
                
            elif (project_name == 'two_displays.gxp' or 
                project_name == 'demo_display_resolution.gxp'): 
                remove_list.append(project_path + 'MAIN_DISPLAY_resources.c')
                remove_list.append(project_path + 'MAIN_DISPLAY_resources.h')
                remove_list.append(project_path + 'SECONDARY_resources.c')
                remove_list.append(project_path + 'SECONDARY_resources.h')

            elif project_name == 'two_displays_binres.gxp': 
                remove_list.append(project_path + 'MAIN_DISPLAY_resources.bin')
                remove_list.append(project_path + 'MAIN_DISPLAY_resources.h')
                remove_list.append(project_path + 'SECONDARY_resources.bin')
                remove_list.append(project_path + 'SECONDARY_resources.h')

            for file in remove_list:
                if os.path.isfile(file):
                    os.remove(file)

        return cmp_result
    
# Generate test output file for every gxp projects and
# compare between default project and new output files.
def test(gxp_projects, studio_release, write_result = True):
    init_file = '__init__.py'
    path = sys.path[0]
    file = open(path+'\\'+init_file,'a+')
    file.close()
            
    if not os.path.isfile(studio_release):
        raise Exception('guix_studio.exe not found. path=' + studio_release)
        
    for file in gxp_projects:
        # count total projects
        count_test[0] += 1
        if test_one_project(studio_release, file) == True:
            # count passed projects
            count_test[1] += 1
        else:
            #count failed projects
            count_test[2] += 1
            print("File comparison failed")

    # Write the result.
    if write_result:
        write_test_result()

# Regenerate output files for every gxp projects.
def generate(gxp_projects, studio_release, version, gen_resources, gen_specifications, gen_failures):
    init_file = '__init__.py'
    path = sys.path[0]
    file = open(path+'\\'+init_file,'a+')
    file.close()
    
    if not os.path.isfile(studio_release):
        raise Exception('guix_studio.exe not found. path=' + studio_release)

    for file in gxp_projects:
        if gen_failures:
            templist = []
            templist.append(file)
            previous_failed_test_count = count_test[2]
            test(templist, studio_release, False)
            if count_test[2] == previous_failed_test_count:
                 continue
                 
        count_generate[0] += 1
        project_pathname = os.path.abspath(file)
        project_name = os.path.basename(file)
        root_name = project_pathname[:-4]
        dirname = os.path.dirname(project_pathname)
        base_name = os.path.splitext(project_name)[0]
            
        logging.info("Generating output files for project %s", project_pathname)
        
        output_file_lists = []

        gen_options = " "
        
        if gen_specifications is True:
            gen_options += "-s "
            output_file_lists.append(root_name + '_specifications.c')
            output_file_lists.append(root_name + '_specifications.h')

        if gen_resources is True:
            gen_options += "-r "
            if project_name == 'two_displays.gxp' or project_name == 'demo_display_resolution.gxp' or project_name == 'folder_output_test.gxp':
                output_file_lists.append(root_name + '_MAIN_DISPLAY_resources.c')
                output_file_lists.append(root_name + '_MAIN_DISPLAY_resources.h')
                output_file_lists.append(root_name + '_SECONDARY_resources.c')
                output_file_lists.append(root_name + '_SECONDARY_resources.h')
            elif project_name == 'two_displays_binres.gxp':
                output_file_lists.append(root_name + '_MAIN_DISPLAY_resources.bin')
                output_file_lists.append(root_name + '_MAIN_DISPLAY_resources.h')
                output_file_lists.append(root_name + '_SECONDARY_resources.bin')
                output_file_lists.append(root_name + '_SECONDARY_resources.h')
            elif project_name in multi_theme_project_names:
                output_file_lists.append(root_name + '_resources.c')
                output_file_lists.append(root_name + '_resources.h') 
                output_file_lists.append(root_name + '_resources_big_endian.bin')
                output_file_lists.append(root_name + '_resources.bin')
                output_file_lists.append(dirname + '\Japanese.bin')
                output_file_lists.append(dirname + '\Japanese_big_endian.bin')
                
                if project_name == 'multi_themes_16bpp.gxp':
                    output_file_lists.append(root_name + '_resources_big_endian.c')
                    output_file_lists.append(root_name + '_resources_big_endian.h')
            elif project_name == 'arabic_glyph_shaping.gxp':
                output_file_lists.append(dirname + '\\arabic_string_res.bin')
                output_file_lists.append(root_name + '_resources.c')
                output_file_lists.append(root_name + '_resources.h')
            elif project_name == 'thai_glyph_shaping.gxp':
                output_file_lists.append(dirname + '\\thai_string_res.bin')
                output_file_lists.append(root_name + '_resources.c')
                output_file_lists.append(root_name + '_resources.h')
            elif project_name == 'multi_themes_32bpp.gxp':
                output_file_lists.append(root_name + '_resources.h')
                output_file_lists.append(root_name + '_resources.bin')
                output_file_lists.append(root_name + '_resources_big_endian.bin')
                output_file_lists.append(root_name + '_resources_big_endian.c')
                output_file_lists.append(root_name + '_resources_big_endian.h')

            elif project_name == 'multi_themes_8bpp_palette.gxp':
                output_file_lists.append(root_name + '_resources.bin')
                output_file_lists.append(root_name + '_resources.c')
                output_file_lists.append(root_name + '_resources.h') 
                
            elif project_name == 'multi_themes_16bpp_synergy.gxp':
                output_file_lists.append(root_name + '_resources.bin')
                output_file_lists.append(root_name + '_resources.c')
                output_file_lists.append(root_name + '_resources.h') 
                
            elif (project_name == 'bidi_text_draw_32bpp.gxp' or
                  project_name == 'bidi_text_line_break_static.gxp' or
                  project_name == 'kerning_glyph_in_string_table.gxp' or
                  project_name == 'kerning_glyph_draw_32bpp_screen.gxp' or
                  project_name == 'multi_themes_16bpp_5_5_1.gxp' or
                  project_name == 'binres_theme_static_language.gxp'):
                output_file_lists.append(root_name + '_resources.bin')
                output_file_lists.append(root_name + '_resources.c')
                output_file_lists.append(root_name + '_resources.h') 
            elif project_name in binres_only_project_names:
                output_file_lists.append(root_name + '_resources.bin')
                output_file_lists.append(root_name + '_resources.h')
            elif project_name == 'standalone_binres_load_16bpp.gxp':
                output_file_lists.append(root_name + '_resources.c')
                output_file_lists.append(root_name + '_resources.h')
                output_file_lists.append(dirname + '\\font_1bpp.bin')
                output_file_lists.append(dirname + '\\font_4bpp.bin')
                output_file_lists.append(dirname + '\\font_8bpp.bin')
                output_file_lists.append(dirname + '\\pixelmap_8bit_palette.bin')
                output_file_lists.append(dirname + '\\pixelmap_565rgb.bin')
                output_file_lists.append(dirname + '\\pixelmap_compress_4444argb.bin')
                output_file_lists.append(dirname + '\\pixelmap_compress_alphamap.bin')
                output_file_lists.append(dirname + '\\pixelmap_raw_4444argb.bin')
                output_file_lists.append(dirname + '\\pixelmap_raw_alphamap.bin')
            elif project_name == 'standalone_binres_load_8bpp.gxp':
                output_file_lists.append(root_name + '_resources.c')
                output_file_lists.append(root_name + '_resources.h')
                output_file_lists.append(dirname + '\\pixelmap_folder_1.bin')
                output_file_lists.append(dirname + '\\pixelmap_fish.bin')
            elif project_name == 'standalone_binres_load_extended_unicode_32bpp':
                output_file_lists.append(root_name + '_resources.c')
                output_file_lists.append(root_name + '_resources.h')
                output_file_lists.append(dirname + '\\font_old_italic_1bpp.bin')
                output_file_lists.append(dirname + '\\font_old_italic_4bpp.bin')
                output_file_lists.append(dirname + '\\font_old_italic_8bpp.bin')
                output_file_lists.append(dirname + '\\pixelmaps.bin')
            else:
                output_file_lists.append(root_name + '_resources.c')
                output_file_lists.append(root_name + '_resources.h')
        
        #remove exist output files
        for file in output_file_lists:
            if os.path.isfile(file):
                os.remove(file)

        #regenerate output files
        if project_name == 'two_displays.gxp':       
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n" + gen_options + "-d MAIN_DISPLAY")
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n" + gen_options  + "-d SECONDARY")
        elif project_name == 'two_displays_binres.gxp':       
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -b" + gen_options + "-d MAIN_DISPLAY")
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -b" + gen_options  + "-d SECONDARY")
        elif project_name in multi_theme_project_names:
            #generate binary format resource files
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -b --no_res_header -r Japanese -l Japanese -t")
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -b --no_res_header -r -t theme_4, theme_5 -l Spanish")
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -b --big_endian --no_res_header -r Japanese_big_endian -l Japanese -t")
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -b --big_endian --no_res_header -r " + base_name + "_resources_big_endian -t theme_4, theme_5 -l Spanish")
            
            #generate source format resource and specification files
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n" + gen_options + "-t theme_1, theme_2, theme_3 -l English, Chinese")
            
            if project_name == 'multi_themes_16bpp.gxp':
                os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n --big_endian -r " + base_name + "_resources_big_endian -t theme_1, theme_2, theme_3 -l English, Chinese")
        elif project_name == 'arabic_glyph_shaping.gxp':
                os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -r -s -l English")
                os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -b --no_res_header -r arabic_string_res -l Arabic -t")
        elif project_name == 'thai_glyph_shaping.gxp':
                os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -r -s -l English")
                os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -b --no_res_header -r thai_string_res -l Thai -t")
        elif project_name == 'multi_themes_16bpp_5_5_1.gxp':
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -r -s -t theme_1, theme_2, theme_3 -l English, Chinese")
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -b --no_res_header -r -t theme_4, theme_5, -l Japanese, Spanish")
        elif project_name == 'multi_themes_32bpp.gxp':
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -r -s -b")
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n --big_endian -r " + base_name + "_resources_big_endian")
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -b --big_endian --no_res_header -r " + base_name + "_resources_big_endian")
            
        elif project_name == 'multi_themes_8bpp_palette.gxp':
            #Gen "resource.bin": Including theme4, theme5
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -b --no_res_header -t theme_4, theme_5 -l")
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -t theme_1, theme_2, theme_3")      
                
        elif project_name == 'multi_themes_16bpp_synergy.gxp':
            #Gen "resource.bin": Including theme4, theme5
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -b --no_res_header -t theme_4, theme_5")
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -t theme_1, theme_2, theme_3") 
            
        elif project_name == 'kerning_glyph_draw_32bpp_screen.gxp' or  project_name == 'kerning_glyph_in_string_table.gxp':
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -r -s -t theme_1 -l English")
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -b --no_res_header -r -t theme_2")
            
        elif project_name == 'binres_theme_static_language.gxp':
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -b --no_res_header -r -t theme_1, theme_2 -l")
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -t -l English,Spanish")
        
        elif project_name == 'bidi_text_draw_32bpp.gxp':
            #Gen "bidi_text_draw_32bpp_resources.bin"
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -b --no_res_header")
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n")
        elif project_name == 'bidi_text_line_break_static.gxp':
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -b --no_res_header -t theme_2 -l")
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -t theme_1")
        elif project_name in binres_only_project_names:
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -r -s -b")
        elif 'big_endian' in project_name:
                os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -r -s --big_endian")
        elif project_name == 'folder_output_test.gxp':
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -d MAIN_DISPLAY -l English,Japanese")
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n -d SECONDARY -l English,Chinese")
        else: 
            os.system(os.path.abspath(studio_release) + " -p " + project_pathname + " -n" + gen_options)

            
        #verify if new output files been created
     
        success = True
            
        for file in output_file_lists:      
            if not os.path.isfile(file):
                print("Failed to generate output file " + file)
                success = False
                if file.endswith('_resources.c'):
                    logging.error('Failed to create resource file ' + file)
                if file.endswith('_resources.h'):
                    logging.error('Failed to create resource header ' + file)
                if file.endswith('.bin'):
                    logging.error('Failed to create binary resource file' + file)
                if file.endswith('_specification.c'):
                    logging.error('Failed to create specification file ' + file)
                if file.endswith('_specification.h'):
                    logging.error('Failed to create specification header ' + file)

        if success:
            count_generate[1] += 1
        else:
            count_generate[2] += 1

    #remove the generated object files
    os.system('del *.obj')

    write_test_result()

def upgrade_projects():
    logging.debug("*****************************************************")
    logging.debug("*     Upgrade projects                              *")
    logging.debug("*                                                   *")
    logging.debug("*                                                   *")
    logging.debug("*****************************************************")
        
    sln_files = search_gxps.sln_file_search('..\\..\\..')
    
    for file in sln_files:
        logging.debug('Upgrade ' + file)
        os.system("devenv /upgrade " + file)

def update_gxp_project(project_files):
    logging.debug("*****************************************************")
    logging.debug("*     Update gxp projects                           *")
    logging.debug("*                                                   *")
    logging.debug("*                                                   *")
    logging.debug("*****************************************************")

    for project in project_files:
        print("Updating project %s\n" %(project))
        test_utils.open_project(project, 0)
        test_utils.toolbar_save()
        
def compile_project(project_files):
    logging.debug("*****************************************************")
    logging.debug("*     Compile projects                              *")
    logging.debug("*                                                   *")
    logging.debug("*                                                   *")
    logging.debug("*****************************************************")
    logging.debug("  Test date: %s", time.asctime())
    logging.debug("  Test directory: %s", os.getcwd())

    for file in project_files:
        if "internal_build" in file:
            logging.debug('Compiling ' + file)
            
            project_name = os.path.basename(os.path.realpath(file))
            project_name = os.path.splitext(project_name)[0]
            
            if (project_name in back_compatible_project_names):
                continue
            
            # count total projects 
            count_test[0] += 1
            if os.system("msbuild /p:Configuration=Debug /p:BuildProjectReferences=false " + file):
                #count failed projects
                count_test[2] += 1
                logging.error("**Error** Compilation failed for project " + os.path.realpath(file))                     
            else:
                # count passed projects
                count_test[1] += 1 
        
    write_test_result()
      
def output_file_header():
    logging.debug("*****************************************************")
    logging.debug("*           Studio File Generation Test             *")
    logging.debug("*                                                   *")
    logging.debug("*                                                   *")
    logging.debug("*****************************************************")
    logging.debug("  Test date: %s", time.asctime())
    logging.debug("  Test directory: %s", os.getcwd())

def write_test_result():
    logging.debug("********************************************")
    if count_test[0]:
       logging.info(" Total Test Projects : %d", count_test[0])
       logging.info(" Passed : %d", count_test[1])
       logging.info(" Failed : %d", count_test[2])
    
    if count_generate[0]:
       logging.info(" Total Generate Projects : %d", count_generate[0])
       logging.info(" Passed : %d", count_generate[1])
       logging.info(" Failed : %d", count_generate[2])
                
def __main__():
    #Change the current working directory to the path of this file.
    current_path = os.path.split(os.path.realpath(__file__))[0]
    os.chdir(current_path)
    studio_sln_path = "../../../guix_studio/build/vs_2019/studiox.sln"
    studio_exe_path = "../../../guix_studio/build/vs_2019/Release/guix_studio.exe"
    guix_project_path = "../../../ports/win32/build/vs_2019/guix.vcxproj"

    gxp_projects = []

    parser = argparse.ArgumentParser(description = "Test Studio generated output files.")
    parser.add_argument('-b', action='store_true', dest='build_studio', help='Build Studio')
    parser.add_argument('--build_guix', action='store_true', dest='build_guix', help='Build Guix Lib')
    parser.add_argument('-p', action = 'store', dest ='project', help='Specify Studio project') 
    parser.add_argument('-t', action = 'store_true', dest ='test', help='Compare generated output files with golden files')
    parser.add_argument('-g', action = 'store_true', dest ='generate', help="Generate all golden files and compile them.")
    parser.add_argument('-r', action = 'store_true', dest ='gen_resources', help="Generate resource golden files and compile them.")
    parser.add_argument('-s', action = 'store_true', dest ='gen_specifications', help="Generate specifications golden files and compile them.")
    parser.add_argument('-v', action='store', dest='version', help='Specify guix library version, such as \'5.2.8\'')
    parser.add_argument('--upgrade', action = 'store_true', dest = 'upgrade', help="Upgrade sln files.")
    parser.add_argument('--compile_project', action = 'store_true', dest = 'compile_project', help="Compile project.")
    parser.add_argument('--update_gxp_project', action = 'store_true', dest = 'update_gxp_project', help="Update gxp project.")
    parser.add_argument('--gen_failures', action = 'store_true', dest = 'gen_failures', help="Used with -g/-r/-s to generate golden files only for failed tests.")
    parser.add_argument('--debug', action = 'store_true', dest = 'debug', help="Print the debug logs to console.")
    parser.add_argument('--samples', action = 'store_true', dest ='samples', help='Specify Studio projects under \"samples\" folder')
    parser.add_argument('--tutorials', action = 'store_true', dest ='tutorials', help='Specify Studio projects under \"tutorials\" folder')
    parser.add_argument('--internal_examples', action = 'store_true', dest ='internal_examples', help='Specify Studio projects under \"examples_internal\" folder')
    
    args = parser.parse_args()

    Format = '%(message)s'
    logging.basicConfig(filename='output_files_test_log.txt', filemode = 'w', level=logging.DEBUG, format = Format)

    #If using '--debug', set logging level to DEBUG to print all the logs(debug, info, warning and error message).
    if args.debug:
        logging_level = logging.DEBUG
    else:
        #Default logging level is INFO, will not print debug message.
        logging_level = logging.INFO

    console = logging.StreamHandler()
    console.setLevel(logging_level)
    formater = logging.Formatter(Format)
    console.setFormatter(formater)
    logging.getLogger('').addHandler(console)

    # Upgrade sln files
    if args.upgrade is True:
        upgrade_projects()

    # Build studio
    if args.build_studio:        
        build_result = studio_compile(studio_sln_path)
        if build_result != 0:
            exit(-1)
    
    if args.project is None:
        if (args.samples is False and
            args.tutorials is False and
            args.internal_examples is False):
           args.samples = True
           args.tutorials = True
           args.internal_examples = True
        
        gxp_projects = search_gxps.project_search('../../../', args.samples, args.tutorials, args.internal_examples)
        print("\nFound %d projects\n" %(len(gxp_projects)))            
    else:
        gxp_projects.append(args.project)
     
    # Update GUIX library version in Studio project files
    if args.version:
        set_project_lib_version(gxp_projects, args.version)
        
    # Update gxp projects
    if args.update_gxp_project:
        if not test_utils.find_studio_handles():
            test_utils.run_studio()   
            if not test_utils.find_studio_handles():
                raise Exception("Unable to start and find Studio application")
        update_gxp_project(gxp_projects)
        
    if args.compile_project:
        project_files = []
        if args.project is None:
            project_files = search_gxps.sln_file_search('..\\..\\..')
        else:
            project_files.append(args.project)
            
        compile_project(project_files)
        if count_test[2] != 0:
            exit(-1)
        else:
            exit(0)
    else:
        output_file_header()
            
    if args.build_guix:
        build_result = guix_compile(guix_project_path, "Debug")
        build_result += guix_compile(guix_project_path, "Debug_GUIX_5_4_0_COMPATIBILITY")
        build_result += guix_compile(guix_project_path, "Debug_GX_DYNAMIC_BIDI_TEXT_SUPPORT") 
        build_result += guix_compile(guix_project_path, "Debug_GX_PALETTE_MODE_AA_TEXT_COLORS16")
        build_result += guix_compile(guix_project_path, "Debug_GX_MOUSE_SUPPORT")
        build_result += guix_compile(guix_project_path, "Debug_GX_FONT_KERNING_SUPPORT")
        build_result += guix_compile(guix_project_path, "Debug_GX_SYNERGY_FONT_FORMAT_SUPPORT")
        build_result += guix_compile(guix_project_path, "Debug_GX_DISABLE_BRUSH_ALPHA_SUPPORT")
        build_result += guix_compile(guix_project_path, "Debug_GX_EXTENDED_UNICODE_SUPPORT")
        if build_result != 0:
            exit(-1)
  
    if args.generate is True:
        args.gen_resources = True
        args.gen_specifications = True
    
    #compile the source files
    if args.gen_resources is True or args.gen_specifications is True:
        if args.gen_failures:
            print("Test projects and generate golden files for failed tests.")
        else:
            print("Generate new golden files and compile them.")
        generate(gxp_projects, studio_exe_path, None, args.gen_resources, args.gen_specifications, args.gen_failures)

    else:
        if args.test is True:
            test(gxp_projects, studio_exe_path)
            if count_test[2] != 0:
                exit(-1)
            else:
                exit(0)

       
__main__()

