/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_canvas_pixelmap_get_no_output", /* Test name */
    0, 0, 0, 0  /* Define the coordinates of the capture area. */
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
    /* Create a dedicated thread to perform various operations
       on the pixelmap drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);

    gx_validation_application_define(first_unused_memory);
    
    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);
}

#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"
#include "demo_guix_all_widgets.c"

static VOID control_thread_entry(ULONG input)
{
int failed_tests = 0;
GX_CANVAS *canvas = GX_NULL;
GX_DISPLAY *display = GX_NULL;
GX_PIXELMAP return_map;
UINT status;
GX_RECTANGLE size = {0};
    
    canvas = root->gx_window_root_canvas;
    display = canvas->gx_canvas_display;
    
    status = gx_canvas_pixelmap_get(GX_NULL);
    EXPECT_EQ(GX_PTR_ERROR, status);
    
    status = gx_canvas_pixelmap_get(&return_map);
    EXPECT_EQ(GX_INVALID_CONTEXT, status);
    
    gx_canvas_drawing_initiate(canvas, root, &size);
    status = gx_canvas_pixelmap_get(&return_map);
    gx_canvas_drawing_complete(canvas, GX_FALSE);
    EXPECT_EQ(GX_SUCCESS, status);

    gx_canvas_drawing_initiate(canvas, root, &size);
    _gx_system_current_draw_context->gx_draw_context_display = GX_NULL;
    status = gx_canvas_pixelmap_get(&return_map);
    _gx_system_current_draw_context->gx_draw_context_display = display;
    gx_canvas_drawing_complete(canvas, GX_FALSE);
    EXPECT_EQ(GX_INVALID_DISPLAY, status);
    
    gx_canvas_drawing_initiate(canvas, root, &size);
    _gx_system_current_draw_context->gx_draw_context_canvas = GX_NULL;
    status = gx_canvas_pixelmap_get(&return_map);
    gx_canvas_drawing_complete(canvas, GX_FALSE);
    EXPECT_EQ(GX_INVALID_CANVAS, status);


    if(failed_tests == 0)
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
