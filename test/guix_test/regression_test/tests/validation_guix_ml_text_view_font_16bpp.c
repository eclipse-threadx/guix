/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_ml_text_view_font_16bpp", /* Test name */
    30, 40, 196, 191  /* Define the coordinates of the capture area.
                      In this test, we only need to capture the 
                      multi_line_text_view window
                      drawing area.  */
};

static VOID      control_thread_entry(ULONG);
int main(int argc, char ** argv)
{
    /* Parse the command canvas_text_view argument. */
    gx_validation_setup(argc, argv);

    /* Start ThreadX system */
    tx_kernel_enter(); 
    return(0);
}

VOID tx_application_define(void *first_unused_memory)
{

    /* Create a dedicated thread to perform various operations
    on the canvas_text_view drawing example. These operations 
    simulate user input. */
    gx_validation_control_thread_create(control_thread_entry);

    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Call the actual canvas_text_view routine. */
    gx_validation_application_define(first_unused_memory);

}


/* Replace the default graphics driver with the validation driver. */
#ifdef win32_graphics_driver_setup_565rgb  
#undef win32_graphics_driver_setup_565rgb  
#endif
#define win32_graphics_driver_setup_565rgb  gx_validation_graphics_driver_setup_565rgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_multi_line_text_view_font_16bpp.c"

typedef struct ID_NAME_MAP_STRUCT{
GX_RESOURCE_ID font_id;
GX_CHAR        font_name[256];
}ID_NAME_MAP;

ID_NAME_MAP font[] ={
{GX_FONT_ID_MIDDLE_1BPP, "1bpp font"},
{GX_FONT_ID_LARGE_4BPP, "4bpp font"},
{GX_FONT_ID_SMALL_8BPP, "8bpp font"}
};

static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
UINT font_index;

    for(font_index = 0; font_index < sizeof(font)/sizeof(ID_NAME_MAP); font_index ++)
    {
        /* Set frame id. */         
        gx_validation_set_frame_id(frame_id);

        /* Set comments. */
        gx_validation_set_frame_comment(font[font_index].font_name);
          
        /* Set font. */
        gx_multi_line_text_view_font_set(&pMainWin -> main_window_text_view, font[font_index].font_id);

        /* Mark the window "dirty" */
        gx_system_dirty_mark(pMainWin);

        /* Force a screen refresh. */
        gx_validation_screen_refresh();

        /* Increment frame ID */
        frame_id ++;
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
