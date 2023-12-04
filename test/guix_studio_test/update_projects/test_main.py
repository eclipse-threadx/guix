import os
import sys
import subprocess
import argparse
import test_utils
import search_gxps

#define a exception table that contain projects whose lib version should not change
excluded_projects=[
"old_language_name_test_5_6.gxp"
]
            
def __main__():

    proj_list = []   
    msbuild_exe_path = "C:/Windows/Microsoft.NET/Framework64/v4.0.30319/MSBuild.exe"
    project_sln_path = "../../../guix_studio/studiox.sln"
        
    if not test_utils.find_studio_handles():
        test_utils.run_studio()   
        if not test_utils.find_studio_handles():
            raise Exception("Unable to start and find Studio application") 
          
    test_utils.read_constants()
    proj_list = search_gxps.project_search('..\\..\\..')
    print("Found %d projects\n" %(len(proj_list)))

    for project in proj_list:
    
        if project_name in excluded_projects:
            continue
        
        print("Updating project %s\n" %(project))
        test_utils.open_project(project)
        test_utils.toolbar_save()

    exit(0)

__main__()        

