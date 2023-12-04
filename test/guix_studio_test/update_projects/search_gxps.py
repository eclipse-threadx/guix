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


def project_search(start_dir):
    gxps = files_find(['*.gxp'], start_dir + '\\examples')
    gxps += files_find(['*.gxp'], start_dir + '\\example_internal')
    gxps += files_find(['*.gxp'], start_dir + '\\test\\AutoTest_Studio')
#    gxps = files_find(['*.gxp'], start_dir + '\\test\\AutoTest_STUDIO\\test_view')
    return gxps
