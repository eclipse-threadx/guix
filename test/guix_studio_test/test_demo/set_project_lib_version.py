import os
import argparse
import codecs
from search_gxps import project_search

#define a exception table that contain projects whose lib version should not change
exception_projects=[
"all_widgets_5_2_0.gxp",
"all_widgets_5_2_5.gxp",
"all_widgets_5_3_0.gxp",
"all_widgets_5_3_2.gxp",
"all_widgets_5_3_3.gxp",
"all_widgets_5_3_4.gxp",
"all_widgets_5_4_0.gxp",
"all_widgets_5_4_1.gxp",
"all_widgets_5_4_2.gxp",
"all_widgets_synergy_5_4_2.gxp",
"all_widgets_5_5_1.gxp",
"multi_themes_16bpp_5_5_1.gxp",
"old_language_name_test_5_6.gxp"
]

def set_project_lib_version(gxp_projects, version):

    if version is None:
        print('You must specify the GUIX library version')
        parser.print_help()
        exit()
        
    updated = 0
    
    major, minor, patch = version.split('.')
    int_major = int(major)
    int_minor = int(minor)
    int_patch = int(patch)
    new_version = str.format('%d%02d%02d' %(int_major, int_minor, int_patch))

    for project in gxp_projects:          
        absolute_path = os.path.abspath(project)

        if not os.path.isfile(absolute_path):
            raise Exception("project was deleted")

        (project_path, project_name) = os.path.split(absolute_path)
        
        if project_name in exception_projects:
            continue
            
        # read project into memory
        file = open(absolute_path, 'rb', -1)
        src_lines = file.readlines()
        file.close()
        update = True

        for line in src_lines:
            if b'<guix_version>' in line:
                start = line.find(b'>')
                end = line.find(b'</')
                if line[start+1:end] == bytearray(new_version, 'utf8'):
                    update = False
                    break

        if not update:
            continue

        # now open project in write mode
        file = open(absolute_path, 'wb', -1)
        
        for line in src_lines:
            if b'<guix_version>' in line:
                start = line.find(b'>')
                end = line.find(b'</')
                line = line[:start+1] + bytearray(new_version, 'utf8') + line[end:]
            file.write(line)
        file.close()
        updated += 1

    print('Projects searched: %d  Projects updated: %d' %(len(gxp_projects), updated))

