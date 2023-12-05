import test_utils
import os

def get_test_header():
    notes =  "*         Studio Guix Lib Version Set Test            *\n"
    notes += "*                                                     *\n"
    notes += "* Test Studio Giox Lib Version Set                    *\n"
    return notes
   
 
def verify_guix_lib_version(test_project):
    if not os.path.isfile(test_project):
        logging.error("** ERROR ** %s does not exist", test_project)
        return False
        
    file = open(test_project,'r')
    lines = file.readlines()
    
    for line in lines:
        if '<guix_version>' in line:
            if '50600' in line:
                test_utils.set_test_results(True, 'check guix lib version', '')
            else:
                test_utils.set_test_results(False, 'check guix lib verison', 'guix lib version should be 50600')
    
def run_guix_lib_version_set_tests(generate, screenshot):
    test_utils.output_test_header(get_test_header())
    
    # Open test project
    test_project = 'simple_5_6_0.gxp'
    test_utils.open_project(test_project, 0)

    # Open project configure dialog
    test_utils.configure_project()

    # Save project configure dialog
    test_utils.save_project_configure()
    
    # Save project
    test_utils.close_project(1)
    test_utils.click_yes_to_please_confirm_dialog()
    
    # verify guix lib version
    verify_guix_lib_version(test_project)
    
    test_utils.write_end('Studio Guix Lib Version Set Tests')
    