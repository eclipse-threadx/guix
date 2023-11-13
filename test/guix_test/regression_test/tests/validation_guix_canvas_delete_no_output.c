/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_canvas.h"

TEST_PARAM test_parameter = {
    "guix_canvas_delete_no_output", /* Test name */
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
GX_CANVAS *canvas = GX_NULL;
GX_DISPLAY *display = GX_NULL;
static GX_CANVAS canvas1;
static GX_CANVAS canvas2;
static GX_COLOR  canvas1_memory[4];
static GX_COLOR  canvas2_memory[4];

    gx_widget_canvas_get((GX_WIDGET *)root, &canvas);
    display = canvas->gx_canvas_display;
    if(!canvas || !display) 
    {
        gx_validation_print_test_result(TEST_FAIL);
        exit(1);
    }
    
    if (gx_canvas_create(&canvas1, "canvas1", display, GX_CANVAS_SIMPLE, 1, 1, canvas1_memory, 4) != GX_SUCCESS)
    {
        gx_validation_print_test_result(TEST_FAIL);
        exit(1);
    }
    
    if (gx_canvas_create(&canvas2, "canvas2", display, GX_CANVAS_SIMPLE, 1, 1, canvas2_memory, 4) != GX_SUCCESS)
    {
        gx_validation_print_test_result(TEST_FAIL);
        exit(1);
    }    
    
    if(gx_canvas_delete(&canvas1) != GX_SUCCESS)
    {
        gx_validation_print_test_result(TEST_FAIL);
        exit(1);
    }   
    
    if(gx_canvas_delete(&canvas2) != GX_SUCCESS)
    {
        gx_validation_print_test_result(TEST_FAIL);
        exit(1);
    }   
    
    if(gx_canvas_delete(canvas) != GX_SUCCESS)
    {
        gx_validation_print_test_result(TEST_FAIL);
        exit(1);
    }   
    gx_canvas_delete(&composite_canvas);
    composite_canvas.gx_canvas_id = GX_CANVAS_ID;
    gx_canvas_delete(&composite_canvas);

    gx_validation_print_test_result(TEST_SUCCESS);
    exit(0);

}
