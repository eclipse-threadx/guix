import os
import sys
import time
import test_utils
import test_constants
import logging

all_widgets_execute_output_files = [
"all_widgets_execute_resources.c",
"all_widgets_execute_resources.h",
"all_widgets_execute_specifications.c",
"all_widgets_execute_specifications.h"
]

def get_test_header():

    notes =  "*          Studio Trigger Edit Tests                  *\n"
    notes += "*                                                     *\n"
    notes += "* Test trigger add/edit/edit                          *\n"
    notes += "* Test empty screen flow check when flow count is 0   *\n"
    return notes

def run_trigger_merge():
    # open screen flow edit dialog
    test_utils.configure_screen_flow()
    
    # open trigger list edit dialog
    test_utils.edit_trigger_list('popup_modal')

    #-----------------------------------------------------#
    # Add trigger                                         #
    #-----------------------------------------------------#
    test_utils.add_trigger()
    test_utils.set_trigger_type('TRIGGER_TYPE_CHILD_SIGNAL')
    event_type = '(IDB_OK, GX_EVENT_FOCUS_GAINED)'
    test_utils.set_event_type(event_type)
    test_utils.save_trigger_edit_dialog()
    
    #-----------------------------------------------------#
    # Edit action                                         #
    #-----------------------------------------------------#
    test_utils.select_trigger_list_row(2)
    test_utils.edit_action('gx_signal'+ event_type.lower())
    test_utils.add_action('Window Execute Stop')
    test_utils.select_target('popup_modal')
    test_utils.save_trigger_action_edit_dialog()
    
    #-----------------------------------------------------#
    # Edit trigger                                        #
    #-----------------------------------------------------#
    test_utils.edit_trigger()
    test_utils.set_event_type('(IDB_OK, GX_EVENT_CLICKED)')
    test_utils.save_trigger_edit_dialog(test_utils.CLOSE_NO_WAIT)# duplicate trigger exist, ask user if they want to do merging
    test_utils.click_yes_to_please_confirm_dialog()# click yes to merge dupliate triggers

    # wait trigger edit dialog close
    test_utils.wait_dialog_close(test_utils.TRIGGER_EDIT_DIALOG)
    
    test_utils.select_trigger_list_row(0)
    test_utils.edit_trigger()
    test_utils.set_event_type('(IDB_CANCEL, GX_EVENT_CLICKED)')
    test_utils.save_trigger_edit_dialog(test_utils.CLOSE_NO_WAIT)# duplicate trigger exist, ask user if they want to do merging
    test_utils.click_yes_to_please_confirm_dialog()# click yes to merge dupliate triggers
    
    # wait trigger edit dialog close
    test_utils.wait_dialog_close(test_utils.TRIGGER_EDIT_DIALOG)
    
    # save trigger list edit dialog
    test_utils.save_trigger_list_edit_dialog()
    
    # save screen flow edit dialog
    test_utils.save_screen_flow_edit_dialog()

def run_trigger_delete():
    # open screen flow edit dialog
    test_utils.configure_screen_flow()
    
    # open trigger list edit dialog
    test_utils.edit_trigger_list('menu_screen')
    
    test_utils.delete_trigger()
    
    test_utils.save_trigger_list_edit_dialog()
    
    test_utils.save_screen_flow_edit_dialog()
    
def run_trigger_edit():
    # open screen flow edit dialog
    test_utils.configure_screen_flow()
    
    # open trigger list edit dialog
    test_utils.edit_trigger_list('menu_screen')

    #-----------------------------------------------------#
    # Add trigger                                         #
    #-----------------------------------------------------#
    test_utils.add_trigger()
    test_utils.set_trigger_type('TRIGGER_TYPE_CHILD_SIGNAL')
    event_type = '(IDB_NEXT, GX_EVENT_CLICKED)'
    test_utils.set_event_type(event_type)
    test_utils.save_trigger_edit_dialog()
    
    #-----------------------------------------------------#
    # Edit action                                         #
    #-----------------------------------------------------#
    test_utils.edit_action('gx_signal' + event_type.lower())
    test_utils.add_action('Animation')
    test_utils.select_target('menu_screen')
    test_utils.check_target_show_child_widgets(1)
    test_utils.select_target('text_view')
    
    # set animation parameters
    test_utils.edit_action_name("menu_screen_fade_out")
    test_utils.edit_animation_info('ANIMATION_ID', 'ANIMATION_ID_MENU_SCREEN_FADE_OUT')
    test_utils.edit_animation_info('ANIMATION_START_X', '-5')
    test_utils.edit_animation_info('ANIMATION_START_X', '5')
    test_utils.edit_animation_info('ANIMATION_START_Y', '-5')
    test_utils.edit_animation_info('ANIMATION_START_Y', '5')
    test_utils.edit_animation_info('ANIMATION_END_X', '-200')
    test_utils.edit_animation_info('ANIMATION_END_X', '200')
    test_utils.edit_animation_info('ANIMATION_END_Y', '-50')
    test_utils.edit_animation_info('ANIMATION_END_Y', '50')
    test_utils.edit_animation_info('ANIMATION_STEPS', '20')
    test_utils.edit_animation_info('ANIMATION_TICK_RATE', '2')
    test_utils.edit_animation_info('ANIMATION_DELAY_BEFORE', '1')
    test_utils.edit_animation_info('ANIMATION_START_ALPHA', '200')
    test_utils.edit_animation_info('ANIMATION_END_ALPHA', '10')
    test_utils.check_detach_target(1)

    # set easing function
    test_utils.configure_easing_function()
    test_utils.select_easing_function('sine ease in')
    test_utils.save_easing_function_select_dialog()
    
    test_utils.add_action('Animation')
    test_utils.delete_action()
    test_utils.save_trigger_action_edit_dialog()
    
    #-----------------------------------------------------#
    # Add trigger                                         #
    #-----------------------------------------------------#
    test_utils.add_trigger()
    test_utils.set_trigger_type('TRIGGER_TYPE_SYSTEM_EVENT')
    event_type = 'GX_EVENT_PEN_DOWN'
    test_utils.set_event_type(event_type)
    test_utils.save_trigger_edit_dialog()

    #-----------------------------------------------------#
    # Edit action                                         #
    #-----------------------------------------------------#
    test_utils.select_trigger_list_row(1)
    test_utils.edit_action(event_type.lower())
    test_utils.add_action('Detach')
    test_utils.check_target_show_child_widgets(1)
    test_utils.select_target('title_4')
    test_utils.save_trigger_action_edit_dialog()

    #-----------------------------------------------------#
    # Add trigger                                         #
    #-----------------------------------------------------#
    test_utils.add_trigger()
    test_utils.set_trigger_type('TRIGGER_TYPE_USER_EVENT')
    event_type = 'USER_EVENT_ID'
    test_utils.set_event_type(event_type)
    test_utils.save_trigger_edit_dialog()
    
    #-----------------------------------------------------#
    # Edit action                                         #
    #-----------------------------------------------------#
    test_utils.select_trigger_list_row(2)
    test_utils.edit_action(event_type.lower())
    test_utils.add_action('Detach')
    test_utils.check_target_show_child_widgets(1)
    test_utils.select_target('title_4')
    test_utils.save_trigger_action_edit_dialog()
    
    #edit system event trigger
    test_utils.select_trigger_list_row(1)
    test_utils.edit_trigger()
    test_utils.save_trigger_edit_dialog()
    
    #edit user event trigger
    test_utils.select_trigger_list_row(2)
    test_utils.edit_trigger()
    test_utils.save_trigger_edit_dialog()
    
    # save trigger list edit dialog
    test_utils.save_trigger_list_edit_dialog()
    
    # save screen flow edit dialog
    test_utils.save_screen_flow_edit_dialog()

def run_notify_message_test():

    test_utils.select_project_tree_node('test_button_window')
    test_utils.delete_widget()
    
    #delete a widget whose id name is referenced by a trigger
    test_utils.select_project_tree_node('next_button')
    test_utils.post_delete_widget()
      #a confirm dialog will show up, id name IDB_NEXT is referenced by a trigger,
      #remove the id name will invalid the trigger, continue?
      
    #cancel delete
    test_utils.click_no_to_please_confirm_dialog()
    test_utils.compare_result()
    
    #delete next button
    test_utils.select_project_tree_node('next_button')
    test_utils.post_delete_widget()
    test_utils.click_yes_to_please_confirm_dialog()
    
    #wait next button been deleted
    while test_utils.select_project_tree_node('next_button'):
        time.sleep(1)

    test_utils.compare_result()
    
    #change widget id name that is referenced by a trigger
    test_utils.select_project_tree_node('text_button_1')
    test_utils.edit_widget_props('ID_WIDGET_ID', 'ID_TEXT_BUTTON_test')
    test_utils.edit_widget_props_post('ID_WIDGET_ID', '')
    test_utils.click_no_to_please_confirm_dialog()
    
    #delete a top level widget
    test_utils.select_project_tree_node('gauge_screen')
    test_utils.post_delete_widget()
    test_utils.click_no_to_please_confirm_dialog()
    
    #delete a folder
    test_utils.select_project_tree_node('default_folder')
    test_utils.post_delete_folder()
    test_utils.click_no_to_please_confirm_dialog()

def run_animation_id_add_remove():
    #--------------------------------------------------------------------------------------------#
    # Test if animation ID is added to dictionay when cancel screen flow edit dialog             #
    #--------------------------------------------------------------------------------------------#
    test_utils.configure_screen_flow()
    test_utils.edit_trigger_list('sprite_screen')
    
    #-----------------------------------------------------#
    # Add trigger                                         #
    #-----------------------------------------------------#
    test_utils.add_trigger()
    test_utils.set_trigger_type('TRIGGER_TYPE_SYSTEM_EVENT')
    event_type = 'GX_EVENT_PEN_DOWN'
    test_utils.set_event_type(event_type)
    test_utils.save_trigger_edit_dialog()
    
    #-----------------------------------------------------#
    # Edit action                                         #
    #-----------------------------------------------------#
    # add 2 animation
    test_utils.select_trigger_list_row(1)
    test_utils.edit_action(event_type.lower())
    test_utils.add_action('Animation')
    test_utils.edit_action_name("animation_0")
    test_utils.edit_animation_info('ANIMATION_ID', 'TEST_ANIMATION_ID_0')
    test_utils.save_trigger_action_edit_dialog()
    
    test_utils.save_trigger_list_edit_dialog()
    test_utils.cancel_screen_flow_edit_dialog()

    #--------------------------------------------------------------------------------------------#
    # Test if animation ID is removed from dictionary when animation is delete                   #
    # Test if animation ID is removed from dictionary when trigger is delete                     #
    #--------------------------------------------------------------------------------------------#
    
    # open screen flow edit dialog
    test_utils.configure_screen_flow()
    
    # open trigger list edit dialog
    test_utils.edit_trigger_list('text_screen')

    #-----------------------------------------------------#
    # Add trigger                                         #
    #-----------------------------------------------------#
    test_utils.add_trigger()
    test_utils.set_trigger_type('TRIGGER_TYPE_SYSTEM_EVENT')
    event_type = 'GX_EVENT_PEN_DOWN'
    test_utils.set_event_type(event_type)
    test_utils.save_trigger_edit_dialog()
    
    #-----------------------------------------------------#
    # Edit action                                         #
    #-----------------------------------------------------#
    # add 2 animation
    test_utils.select_trigger_list_row(1)
    test_utils.edit_action(event_type)
    test_utils.add_action('Animation')
    test_utils.edit_action_name("animation_1")
    test_utils.edit_animation_info('ANIMATION_ID', 'TEST_ANIMATION_ID_1')
    test_utils.add_action('Animation')
    test_utils.edit_action_name("animation_2")
    test_utils.edit_animation_info('ANIMATION_ID', 'TEST_ANIMATION_ID_2')

    # delete aniamtion_1
    test_utils.select_action("animation_1")
    test_utils.delete_action()
    test_utils.save_trigger_action_edit_dialog()
    
    #-----------------------------------------------------#
    # Add trigger                                         #
    #-----------------------------------------------------#
    test_utils.add_trigger()
    test_utils.set_trigger_type('TRIGGER_TYPE_SYSTEM_EVENT')
    event_type = 'GX_EVENT_PEN_UP'
    test_utils.set_event_type(event_type)
    test_utils.save_trigger_edit_dialog()
    
    #-----------------------------------------------------#
    # Edit action                                         #
    #-----------------------------------------------------#
    # add 1 animation
    test_utils.select_trigger_list_row(2)
    test_utils.edit_action(event_type.lower())
    test_utils.add_action('Animation')
    test_utils.edit_action_name("animation_3")
    test_utils.edit_animation_info('ANIMATION_ID', 'TEST_ANIMATION_ID_3')
    test_utils.save_trigger_action_edit_dialog()
    
    # delete the added trigger
    test_utils.select_trigger_list_row(2)
    test_utils.delete_trigger()

    test_utils.save_trigger_list_edit_dialog()
    
    #--------------------------------------------------------------------------------------------#
    # Test if animation ID is removed from dictionay when flow item is delete                    #
    #--------------------------------------------------------------------------------------------#
    test_utils.edit_trigger_list('gauge_screen')

    #-----------------------------------------------------#
    # Add trigger                                         #
    #-----------------------------------------------------#
    test_utils.add_trigger()
    test_utils.set_trigger_type('TRIGGER_TYPE_SYSTEM_EVENT')
    event_type = 'GX_EVENT_PEN_DOWN'
    test_utils.set_event_type(event_type)
    test_utils.save_trigger_edit_dialog()
    
    #-----------------------------------------------------#
    # Edit action                                         #
    #-----------------------------------------------------#
    # add 1 animation
    test_utils.select_trigger_list_row(1)
    test_utils.edit_action(event_type.lower())
    test_utils.add_action('Animation')
    test_utils.edit_action_name("animation_4")
    test_utils.edit_animation_info('ANIMATION_ID', 'TEST_ANIMATION_ID_4')
    
    test_utils.save_trigger_action_edit_dialog()
    test_utils.save_trigger_list_edit_dialog()
    test_utils.save_screen_flow_edit_dialog()
    
    test_utils.select_project_tree_node('gauge_screen')
    test_utils.post_delete_widget()
    test_utils.click_yes_to_please_confirm_dialog()

    #--------------------------------------------------------------------------------------------#
    # Test if animation ID is added to dictionay when cancel trigger edit dialog                 #
    #--------------------------------------------------------------------------------------------#
    test_utils.configure_screen_flow()
    test_utils.edit_trigger_list('text_screen')
    
    #-----------------------------------------------------#
    # Add trigger                                         #
    #-----------------------------------------------------#
    test_utils.add_trigger()
    test_utils.set_trigger_type('TRIGGER_TYPE_SYSTEM_EVENT')
    event_type = 'GX_EVENT_PEN_UP'
    test_utils.set_event_type(event_type)
    test_utils.save_trigger_edit_dialog()
    
    #-----------------------------------------------------#
    # Edit action                                         #
    #-----------------------------------------------------#    
    # add 2 animation
    test_utils.select_trigger_list_row(1)
    test_utils.edit_action(event_type)
    test_utils.add_action('Animation')
    test_utils.edit_action_name("animation_5")
    test_utils.edit_animation_info('ANIMATION_ID', 'TEST_ANIMATION_ID_5')
    
    test_utils.add_action('Animation')
    test_utils.edit_action_name("animation_6")
    test_utils.edit_animation_info('ANIMATION_ID', 'TEST_ANIMATION_ID_6')
    
    test_utils.cancel_trigger_action_edit_dialog()
    test_utils.select_trigger_list_row(1)
    test_utils.delete_trigger()
    test_utils.save_trigger_list_edit_dialog()
    test_utils.save_screen_flow_edit_dialog()
    

    #--------------------------------------------------------------------------------------------#
    # Test if animation ID is added to dictionay when cancel trigger list edit dialog            #
    #--------------------------------------------------------------------------------------------#
    test_utils.configure_screen_flow()
    test_utils.edit_trigger_list('text_screen')

    #-----------------------------------------------------#
    # Add trigger                                         #
    #-----------------------------------------------------#
    test_utils.add_trigger()
    test_utils.set_trigger_type('TRIGGER_TYPE_SYSTEM_EVENT')
    event_type = 'GX_EVENT_HIDE'
    test_utils.set_event_type(event_type)
    test_utils.save_trigger_edit_dialog()

    #-----------------------------------------------------#
    # Edit action                                         #
    #-----------------------------------------------------#
    # add 2 animation
    test_utils.select_trigger_list_row(1)
    test_utils.edit_action(event_type.lower())
    test_utils.add_action('Animation')
    test_utils.edit_action_name("animation_7")
    test_utils.edit_animation_info('ANIMATION_ID', 'TEST_ANIMATION_ID_7')

    test_utils.save_trigger_action_edit_dialog()
    test_utils.cancel_trigger_list_edit_dialog()
    test_utils.save_screen_flow_edit_dialog()

    #--------------------------------------------------------------------------------------------#
    # Test if animation ID is removed from dictionay when rename animation id name               #
    #--------------------------------------------------------------------------------------------#
    test_utils.configure_screen_flow()
    test_utils.edit_trigger_list('sprite_screen')

    #-----------------------------------------------------#
    # Add trigger                                         #
    #-----------------------------------------------------#
    test_utils.add_trigger()
    test_utils.set_trigger_type('TRIGGER_TYPE_SYSTEM_EVENT')
    event_type = 'GX_EVENT_PEN_DOWN'
    test_utils.set_event_type(event_type)
    test_utils.save_trigger_edit_dialog()
    
    #-----------------------------------------------------#
    # Edit action                                         #
    #-----------------------------------------------------#    
    # add 2 animation
    test_utils.select_trigger_list_row(1)
    test_utils.edit_action(event_type.lower())
    test_utils.add_action('Animation')
    test_utils.edit_action_name("animation_8")
    test_utils.edit_animation_info('ANIMATION_ID', 'TEST_ANIMATION_ID_8')
    
    test_utils.add_action('Animation')
    test_utils.edit_action_name("animation_9")
    test_utils.edit_animation_info('ANIMATION_ID', 'TEST_ANIMATION_ID_9')

    test_utils.select_action('animation_8')
    test_utils.edit_animation_info('ANIMATION_ID', 'TEST_ANIMATION_ID_10')
    
    test_utils.save_trigger_action_edit_dialog()
    test_utils.save_trigger_list_edit_dialog()
    test_utils.save_screen_flow_edit_dialog()
    
    #--------------------------------------------------------------------------------------------#
    # Test remove animation id name                                                              #
    #--------------------------------------------------------------------------------------------#
    test_utils.configure_screen_flow()
    test_utils.edit_trigger_list('sprite_screen')
    
    #-----------------------------------------------------#
    # Add trigger                                         #
    #-----------------------------------------------------#
    test_utils.add_trigger()
    test_utils.set_trigger_type('TRIGGER_TYPE_SYSTEM_EVENT')
    event_type = 'GX_EVENT_KEY_DOWN'
    test_utils.set_event_type(event_type)
    test_utils.save_trigger_edit_dialog()
    
    #-----------------------------------------------------#
    # Edit action                                         #
    #-----------------------------------------------------#
    # add 2 animation
    test_utils.select_trigger_list_row(2)
    test_utils.edit_action(event_type.lower())
    test_utils.add_action('Animation')
    test_utils.edit_action_name("animation_11")
    test_utils.edit_animation_info('ANIMATION_ID', 'TEST_ANIMATION_ID_11')
    
    test_utils.add_action('Animation')
    test_utils.edit_action_name("animation_12")
    test_utils.edit_animation_info('ANIMATION_ID', 'TEST_ANIMATION_ID_12')

    test_utils.select_action('animation_11')
    test_utils.edit_animation_info('ANIMATION_ID', '')
    
    test_utils.save_trigger_action_edit_dialog()
    test_utils.save_trigger_list_edit_dialog()
    test_utils.save_screen_flow_edit_dialog()

def run_check_empty_screen_flow_test():
    test_utils.open_project('all_widgets_execute.gxp')
 
    # delete all top level widget
    test_utils.select_project_tree_node('button_screen')
    test_utils.post_delete_widget()
    test_utils.click_yes_to_please_confirm_dialog()
    
    test_utils.select_project_tree_node('gauge_screen')
    test_utils.post_delete_widget()
    test_utils.click_yes_to_please_confirm_dialog()
    
    test_utils.select_project_tree_node('indicator_screen')
    test_utils.post_delete_widget()
    test_utils.click_yes_to_please_confirm_dialog()
    
    test_utils.select_project_tree_node('menu_screen')
    test_utils.post_delete_widget()
    test_utils.click_yes_to_please_confirm_dialog()
    
    test_utils.select_project_tree_node('popup_modal')
    test_utils.post_delete_widget()
    test_utils.click_yes_to_please_confirm_dialog()
    
    test_utils.select_project_tree_node('scroll_wheel_screen')
    test_utils.post_delete_widget()
    test_utils.click_yes_to_please_confirm_dialog()
    
    test_utils.select_project_tree_node('sprite_screen')
    test_utils.post_delete_widget()
    test_utils.click_yes_to_please_confirm_dialog()
    
    test_utils.select_project_tree_node('text_screen')
    test_utils.post_delete_widget()
    test_utils.click_yes_to_please_confirm_dialog()
    
    test_utils.select_project_tree_node('window_screen')
    test_utils.post_delete_widget()
    test_utils.click_yes_to_please_confirm_dialog()
    test_utils.compare_result()
    
    # generate all resources
    test_utils.generate_all()
    test_utils.click_generate_button()

def run_trigger_edit_tests(generate, screenshot):

    test_utils.output_test_header(get_test_header()) 
    test_utils.setup(generate, screenshot, 'test_trigger_edit_golden_file')
    
    # open test project
    test_utils.open_project('all_widgets_execute.gxp')
    
    print("run trigger merge")
    run_trigger_merge()
    
    print("run trigger delete")
    run_trigger_delete()

    print("run trigger edit")
    run_trigger_edit()

    print("run notify message test")
    run_notify_message_test()
    
    print("run animation id add remove test")
    run_animation_id_add_remove()
    
    # test output files
    test_utils.generate_all()
    test_utils.click_generate_button()
    test_utils.cmp_output_files(all_widgets_execute_output_files)

    print("run check empty screen flow test")
    run_check_empty_screen_flow_test()
    
    test_utils.write_end("Test Trigger Edit")


