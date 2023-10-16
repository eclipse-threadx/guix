/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_display_active_language_set_no_output", /* Test name */
    0, 0, 0, 0  /* Define the coordinates of the capture area.*/
};

int main(int argc, char ** argv)
{
    /* Start ThreadX system */
    tx_kernel_enter(); 
    return(0);
}

static VOID      control_thread_entry(ULONG);

VOID tx_application_define(void *first_unused_memory)
{
    gx_validation_application_define(first_unused_memory);
    
    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Create a dedicated thread to perform various operations
       on the line drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);
}


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"
#include "demo_guix_all_widgets.c"

UINT test_display_set_up(GX_DISPLAY *display)
{
    return GX_SUCCESS;
}

#define TEST_LANGUAGE_TABLE_SIZE 2
#define TEST_COLOR_TABLE_SIZE    2

extern GX_CONST GX_UBYTE *Primary_English_string_table[];
extern GX_THEME Primary_theme_1;

GX_CONST GX_UBYTE **test_language_table[TEST_LANGUAGE_TABLE_SIZE] =
{
Primary_English_string_table,
Primary_English_string_table
};

GX_COLOR test_color_table[TEST_COLOR_TABLE_SIZE]=
{
0xff0000,
0x00ff00
};

GX_BOOL language_changed_event_count = 0;
UINT test_root_event_process(GX_WINDOW_ROOT *root, GX_EVENT *event_ptr)
{
    switch(event_ptr->gx_event_type)
    {
    case GX_EVENT_LANGUAGE_CHANGE:
        language_changed_event_count++;
        break;
    default:
        return gx_window_root_event_process(root, event_ptr);
    }
    return 0;
}

VOID test_display_driver_cleanup(GX_DISPLAY *display)
{
}

VOID test_palette_set(GX_DISPLAY *display, GX_COLOR *palette, INT count)
{
    display -> gx_display_palette = palette;
    display -> gx_display_palette_size = count;
}

static VOID control_thread_entry(ULONG input)
{
GX_BOOL     test_successed = GX_TRUE;
GX_DISPLAY  test_display;
UINT        status;

    gx_widget_event_process_set(root, test_root_event_process);

    memset(&test_display, 0, sizeof(GX_DISPLAY));
    gx_display_create(&test_display, "test_display", test_display_set_up, 640, 480);
    gx_display_language_table_set(&test_display, (GX_CHAR ***)test_language_table, TEST_LANGUAGE_TABLE_SIZE, PRIMARY_STRING_TABLE_SIZE);
    gx_display_color_table_set(&test_display, (GX_COLOR *)test_color_table, TEST_COLOR_TABLE_SIZE);

    /* Test _gx_display_canvas_dirty. */
    gx_display_color_set(&test_display, 0, 0xff);
    if(root->gx_widget_first_child->gx_widget_status & GX_STATUS_DIRTY)
    {
        test_successed = GX_FALSE;
    }

    /* Test _gx_display_active_language_set. */
    status = gx_display_active_language_set(&test_display, 1);
    if(status != GX_SUCCESS || language_changed_event_count != 0 || test_display.gx_display_active_language != 1)
    {
        test_successed = GX_FALSE;
    }

    gx_widget_hide(root);

    /* Test _gx_display_delete. */
    gx_display_delete(root->gx_window_root_canvas->gx_canvas_display, test_display_driver_cleanup);
    if(_gx_system_display_created_count != 1)
    {
        test_successed = GX_FALSE;
    }

    root->gx_window_root_canvas = GX_NULL;
    status = gx_display_active_language_set(&test_display, 0);
   
    if(status != GX_SUCCESS || language_changed_event_count != 0 || test_display.gx_display_active_language != 0)
    {
        test_successed = GX_FALSE;
    }

    status = gx_display_language_table_set(&test_display, (GX_CHAR ***)test_language_table, 1, PRIMARY_STRING_TABLE_SIZE);
    if(status != GX_SUCCESS || language_changed_event_count != 0)
    {
        test_successed = GX_FALSE;
    }

    status = gx_display_string_table_get(&test_display, 0, GX_NULL, GX_NULL);
    if(status != GX_SUCCESS)
    {
        test_successed = GX_FALSE;
    }

    test_display.gx_display_driver_palette_set = test_palette_set;
    status = gx_display_theme_install(&test_display, &Primary_theme_1);
    if(status != GX_SUCCESS || test_display.gx_display_palette_size != 0)
    {
        test_successed = GX_FALSE;
    }

    if(test_successed)
    {
        gx_validation_print_test_result(TEST_SUCCESS);
        exit(0);
    }
    else 
    {
        gx_validation_print_test_result(TEST_FAIL);
        exit(1);
    }
}
