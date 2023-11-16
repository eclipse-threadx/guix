/* This is a small demo of the high-performance GUIX graphics framework. */


#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_32argb_rotated_cw_rotate_screen", /* Test name */
    7, 10, 471, 355  /* Define the coordinates of the capture area.
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
       on the pixelmap drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);

    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Call the actual line example routine. */
    gx_validation_application_define(first_unused_memory);

}


/* Replace the default graphics driver with the validation driver. */
#ifdef win32_graphics_driver_setup_32argb_rotated
#undef win32_graphics_driver_setup_32argb_rotated
#endif
#define win32_graphics_driver_setup_32argb_rotated  gx_validation_graphics_driver_setup_32argb_rotated


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets_32argb_rotated_cw.c"

/*NOTICE: Number of xpos_value and ypos_valus should be same.*/
GX_RESOURCE_ID pixelmap_id_list[]={
    GX_PIXELMAP_ID_ROTATE_APPLE,
    GX_PIXELMAP_ID_ROTATE_FISH
};

extern INT     angle;
extern GX_BOOL direct;
extern INT     rotate_pixelmap_id;

char test_comment[256];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
int id, index;

    ToggleScreen((GX_WINDOW *)&rotate_screen, pButtonScreen);
    for(id = 0; id < 2; id++)
    {
        rotate_pixelmap_id = pixelmap_id_list[id];

        for(angle = 1; angle < 360; angle++)
        {
            /* Inform the validation system 
            (1) Frame ID, which identifies a specific test configuration;
            (2) Start recording frame on the next toggle operation.
             */
            for(index = 0; index < 2; index++)
            {
                if(index)
                {
                    direct = GX_TRUE;
                }
                else
                {
                    direct = GX_FALSE;
                }
                gx_validation_set_frame_id(frame_id++);
                sprintf(test_comment, "canvas rotate: %d, angle: %d, pixelmap_id: %d. ", direct, angle, rotate_pixelmap_id);
                gx_validation_set_frame_comment(test_comment);

                gx_system_dirty_mark((GX_WIDGET *)&rotate_screen.rotate_screen_pixelmap_window);

                /* Force a screen refresh. */
                gx_validation_screen_refresh();
            }
        }
    }

    direct = GX_TRUE;

    angle = 90;
    root->gx_window_root_canvas->gx_canvas_display->gx_display_color_format = 0;
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "Set wrong display color format, angle: %d, map id: %d", angle, rotate_pixelmap_id);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    angle = 30;
    for(id = 0; id < 2; id++)
    {
        rotate_pixelmap_id = pixelmap_id_list[id];
        gx_validation_set_frame_id(frame_id++);
        sprintf(test_comment, "Set wrong display color format, angle: %d, map id: %d", angle, rotate_pixelmap_id);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }

    root->gx_window_root_canvas->gx_canvas_display->gx_display_color_format = GX_COLOR_FORMAT_32ARGB;

    for(index = 0; index < 3; index++)
    {
        switch(index)
        {
        case 0:
            rotate_pixelmap_id = GX_PIXELMAP_ID__ALPHAMAP_A;
            sprintf(test_comment, "set alphamap");
            break;

        case 1:
            root->gx_window_root_canvas->gx_canvas_display->gx_display_driver_pixel_blend = GX_NULL;
            rotate_pixelmap_id = pixelmap_id_list[0];
            sprintf(test_comment, "set pixelmap to apple, set driver pixel blend null");
            break;

        case 2:
            rotate_pixelmap_id = pixelmap_id_list[1];
            sprintf(test_comment, "set pixelmap to fish, set driver pixel blend null");
            break;
        }

        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }
   
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





