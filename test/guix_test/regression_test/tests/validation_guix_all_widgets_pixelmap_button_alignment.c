/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_pixelmap_button_alignment", /* Test name */
    105, 250, 275, 336  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the line
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

typedef struct TEST_STRUCT{
    INT remove_style;
    INT add_style;
    GX_CHAR *comments;
}TEST;

TEST test_list[]={
    {GX_PIXELMAP_HALIGN_MASK, GX_STYLE_HALIGN_RIGHT,     "pixelmap horizontal right alignment"},
    {GX_PIXELMAP_HALIGN_MASK, GX_STYLE_HALIGN_CENTER,    "pixelmap horizontal center alignment"},
    {GX_PIXELMAP_HALIGN_MASK, GX_STYLE_HALIGN_LEFT,      "pixelmap horizontal left alignment"},
    {GX_PIXELMAP_VALIGN_MASK, GX_STYLE_VALIGN_BOTTOM,    "pixlemap vertical bottom alignment"},
    {GX_PIXELMAP_VALIGN_MASK, GX_STYLE_VALIGN_CENTER,    "pixelmap vertical center alignment"},
    {GX_PIXELMAP_VALIGN_MASK, GX_STYLE_VALIGN_TOP,       "pixelmap vertical top alignment"},
    {0, 0, 0}
};

static VOID control_thread_entry(ULONG input)
{
int              frame_id = 1;
GX_PIXELMAP_BUTTON  *button;
TEST            *test;
GX_RECTANGLE     size;

    gx_widget_find(pButtonScreen, ID_PIXELMAP_BUTTON, 0, &button);
    gx_utility_rectangle_define(&size, 105, 250, 274, 335);
    gx_widget_resize(button, &size);
   
    test = test_list;
    while(test->comments)
    {
        gx_widget_style_remove((GX_WIDGET *)button, test->remove_style);
        gx_widget_style_add((GX_WIDGET *)button, test->add_style);
        
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test->comments);
        gx_validation_screen_refresh();
        test++;
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
