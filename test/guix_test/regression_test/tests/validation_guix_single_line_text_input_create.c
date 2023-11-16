/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_single_line_text_input_create", /* Test name */
    0, 0, 640, 480  /* Define the coordinates of the capture area.
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

char test_comment[255];

GX_RECTANGLE size[3] = {
    {200, 100, 450, 150}, //center
    {200, 170, 450, 220}, //left
    {200, 240, 450, 290}  //right
};

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
GX_SINGLE_LINE_TEXT_INPUT   sl_text_input_center;
GX_SINGLE_LINE_TEXT_INPUT   sl_text_input_left;
GX_SINGLE_LINE_TEXT_INPUT   sl_text_input_right;
int                        frame_id = 1;
GX_WIDGET *child;
static GX_CHAR buffer[20];
GX_BOOL status;

    child = pButtonScreen->gx_widget_first_child;
    
    while(pButtonScreen->gx_widget_first_child)
    {
        child = child -> gx_widget_next;
        gx_widget_delete(pButtonScreen->gx_widget_first_child);
        pButtonScreen->gx_widget_first_child = child;
    }
    
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "Delete all child of button screen.");
    gx_validation_set_frame_comment(test_comment);
    /* Force a screen refresh. */
    gx_validation_screen_refresh();
    
    memset(&sl_text_input_center, 0, sizeof(GX_SINGLE_LINE_TEXT_INPUT));
    memset(&sl_text_input_left, 0, sizeof(GX_SINGLE_LINE_TEXT_INPUT));
    memset(&sl_text_input_right, 0, sizeof(GX_SINGLE_LINE_TEXT_INPUT));

    memset(buffer, 0, sizeof(buffer));
    status = gx_single_line_text_input_create(&sl_text_input_center, "sl_text_input_center", 0, buffer, 0, GX_STYLE_TEXT_CENTER|GX_STYLE_CURSOR_ALWAYS|GX_STYLE_BORDER_RAISED, 0, &size[0]);
    status |= gx_single_line_text_input_create(&sl_text_input_left, "sl_text_input_left", pButtonScreen, buffer, 20, GX_STYLE_TEXT_LEFT|GX_STYLE_CURSOR_ALWAYS|GX_STYLE_BORDER_RAISED, 0, &size[1]);
    memset(buffer, 'x', sizeof(buffer));
    status |= gx_single_line_text_input_create(&sl_text_input_right, "sl_text_input_right", pButtonScreen, buffer, 20, GX_STYLE_TEXT_RIGHT|GX_STYLE_CURSOR_ALWAYS|GX_STYLE_BORDER_RAISED, 0, &size[2]);

    if(status == GX_SUCCESS)
    {
        gx_validation_set_frame_id(frame_id++);
        gx_widget_attach(pButtonScreen, &sl_text_input_center);
        sprintf(test_comment, "Create several sl_text_input and attach them to button screen.");
        gx_validation_set_frame_comment(test_comment);
        /* Force a screen refresh. */
        gx_validation_screen_refresh();
    }
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();
    exit(0);
}
