import os
import fnmatch

#find all files under a specified directory that matches specified pattern
def files_find(pattern, directory):
    gxps = []
    for root, dirnames, filenames in os.walk(directory):
        for extensions in pattern:
            for filename in fnmatch.filter(filenames, extensions):
                gxps.append(os.path.join(root, filename))
    return gxps


def project_search(start_dir, search_samples = True, search_tutorials = True, search_internal_examples = True):
    gxps = []
    if search_samples is True:
        gxps = files_find(['*.gxp'], start_dir + '\\samples')
       
    if search_internal_examples is True:
        gxps += files_find(['*.gxp'], start_dir + '\\test\example_internal')
        
    if search_tutorials is True:
        gxps += files_find(['*.gxp'], start_dir + '\\tutorials')
        
    return gxps

def source_file_search(start_dir):
    solutions = files_find(['*.c'], start_dir + '\\samples')
    solutions += files_find(['*.c'], start_dir + '\\test\example_internal')
    solutions += files_find(['*.c'], start_dir + '\\tutorials')
    return solutions
    
def sln_file_search(start_dir):
    solutions = files_find(['*.vcxproj'], start_dir + '\\samples')
    solutions += files_find(['*.vcxproj'], start_dir + '\\test\example_internal')
    solutions += files_find(['*.vcxproj'], start_dir + '\\tutorials')
    return solutions