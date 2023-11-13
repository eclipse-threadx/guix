/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_widget_canvas_get_no_output", /* Test name */
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
       on the pixelmap drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);

}

#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"
#include "demo_guix_all_widgets.c"

static GX_CANVAS *canvas = GX_NULL;
void button_screen_draw(GX_WINDOW *window)
{
    gx_widget_canvas_get(pTextScreen, &canvas);
    if(canvas != GX_NULL)
    {
        gx_widget_canvas_get(pButtonScreen, &canvas);
    }
}

static VOID control_thread_entry(ULONG input)
{
UINT index = 0;
    gx_widget_draw_set(pButtonScreen, (void (*)(GX_WIDGET *))button_screen_draw);
    
    while(canvas == GX_NULL)
    {
        gx_system_dirty_mark((GX_WIDGET *)pButtonScreen);
        gx_system_canvas_refresh();
        while(index < 0xffffff) index++;
        
    }
    
    gx_validation_print_test_result(TEST_SUCCESS);
    exit(0);
}
