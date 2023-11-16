/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_string_scroll_wheel_text_get", /* Test name */
    200, 100, 380, 360  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the pixelmap
                         drawing area.  */
};

static VOID      control_thread_entry(ULONG);
int main(int argc, char ** argv)
{
    /* Parse the command line argument. */
    gx_validation_setup(argc, argv);

    /* Start ThreadX system */
    tx_kernel_enter(); 
    return(0);
}

VOID tx_application_define(void *first_unused_memory)
{

    /* Create a dedicated thread to perform various operations
       on the line drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);

    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Call the actual line example routine. */
    gx_validation_application_define(first_unused_memory);

}


/* Replace the default graphics driver with the validation driver. */
#ifdef win32_graphics_driver_setup_24xrgb  
#undef win32_graphics_driver_setup_24xrgb  
#endif
#define win32_graphics_driver_setup_24xrgb  gx_validation_graphics_driver_setup_24xrgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"
#include "demo_guix_all_widgets.c"
#include "gx_scroll_wheel.h"

#ifdef GX_ENABLE_DEPRECATED_STRING_API
GX_CHAR test_string[GX_MAX_STRING_LENGTH + 1];
GX_CHAR *month_wheel_value_format_1(GX_STRING_SCROLL_WHEEL *wheel, INT row)
{
    memset(test_string, 0, sizeof(test_string));
    memset(test_string, '0', 2);

    return test_string;
}

GX_CHAR *month_wheel_value_format_2(GX_STRING_SCROLL_WHEEL *wheel, INT row)
{
    memset(test_string, '0', sizeof(test_string));

    return test_string;

}

GX_CHAR *month_wheel_value_format_3(GX_STRING_SCROLL_WHEEL *wheel, INT row)
{ 
    return GX_NULL;
}

GX_CHAR *month_wheel_value_format_4(GX_STRING_SCROLL_WHEEL *wheel, INT row)
{
    if(row < wheel->gx_scroll_wheel_selected_row)
    {
         memset(test_string, '0', sizeof(test_string));    
    }
    else
    {
         memset(test_string, 0, sizeof(test_string));
         memset(test_string, '0', 2);
    }

    return test_string;
}
#endif

char test_comment[255];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
GX_STRING_SCROLL_WHEEL  *month_wheel = GX_NULL;
int                      frame_id = 1;
UINT                     status;
GX_STRING                string;
INT                      index;
INT                      loop;

    /* Create scroll wheel screen. */
    gx_studio_named_widget_create("scroll_wheel_screen", GX_NULL, (GX_WIDGET **)&pScrollWheelScreen);

    /* Toggle to scroll wheel screen. */
    ToggleScreen(pScrollWheelScreen, pButtonScreen);
    
    /* Get month wheel widget pointer for testing. */
    gx_widget_find(pScrollWheelScreen, ID_MONTH_WHEEL, GX_SEARCH_DEPTH_INFINITE, &month_wheel);

    gx_widget_status_add(month_wheel, GX_STATUS_OWNS_INPUT);
    
    sprintf(test_comment, "Original scroll wheel screen.");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_system_dirty_mark(month_wheel);
    /* Force a screen refresh. */
    gx_validation_screen_refresh();
 
    _gx_string_scroll_wheel_text_get(month_wheel, month_wheel->gx_scroll_wheel_total_rows, &string);
    if(string.gx_string_ptr != GX_NULL)
    {
        exit(1);
    }
   
    gx_widget_hide(month_wheel); 
    gx_string_scroll_wheel_string_list_set(month_wheel, 0, 0);
    gx_widget_show(month_wheel);

    gx_scroll_wheel_total_rows_set(month_wheel, 10);
    sprintf(test_comment, "Set string list to null and refresh.");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    /* Force a screen refresh. */
    gx_validation_screen_refresh();
   
    gx_system_memory_allocator_set(GX_NULL, GX_NULL);
    gx_widget_style_add(month_wheel, GX_STYLE_TEXT_COPY);

    /* If text copy style is enabled, memory allocator/free should be availlable,
       return status should be failure. */
    gx_widget_hide(month_wheel);
    status = gx_string_scroll_wheel_string_list_set(month_wheel, 0, 0);
    gx_scroll_wheel_selected_set(month_wheel, 4);
    gx_widget_show(month_wheel);
    if(status == GX_SUCCESS)
    {
        exit(1);
    }

    gx_system_memory_allocator_set(rotate_memory_allocate, rotate_memory_free);
 
#ifdef GX_ENABLE_DEPRECATED_STRING_API
    for(loop = 0; loop < 2; loop++)
    {
        if(loop)
        {
            gx_widget_style_remove(month_wheel, GX_STYLE_TEXT_SCROLL_WHEEL_ROUND);
        }
        else
        {
            gx_widget_style_add(month_wheel, GX_STYLE_TEXT_SCROLL_WHEEL_ROUND);
        }

        for(index = 0; index < 4; index++)
        {
            if(loop)
            {
                sprintf(test_comment, "Flat draw, ");
            }
            else
            {
                sprintf(test_comment, "Round draw, ");
            }


            switch(index)
            {
            case 0:
                gx_text_scroll_wheel_callback_set(month_wheel, month_wheel_value_format_1);
                strcat(test_comment, "set correct text get callback");
                break;
    
            case 1:
                gx_text_scroll_wheel_callback_set(month_wheel, month_wheel_value_format_2);
                strcat(test_comment, "set text get callback that returns invalid string");
                break;
    
            case 2:
                gx_text_scroll_wheel_callback_set(month_wheel, month_wheel_value_format_3);
                strcat(test_comment, "set text get callback that returns null string");
                break;

            case 3:
                gx_text_scroll_wheel_callback_set(month_wheel, month_wheel_value_format_4);
                strcat(test_comment, "set text get callback with partial rows return invalid string");
                break;
            }
            gx_validation_set_frame_id(frame_id++);
            gx_validation_set_frame_comment(test_comment);
            gx_validation_screen_refresh();
        }
    }
#endif

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
