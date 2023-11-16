/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_graphics_arc_32bpp", /* Test name */
    24, 43, 370, 402  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the graphics
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
#ifdef win32_graphics_driver_setup_24xrgb  
#undef win32_graphics_driver_setup_24xrgb  
#endif
#define win32_graphics_driver_setup_24xrgb  gx_validation_graphics_driver_setup_24xrgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_graphics_32bpp.c"

typedef struct ARC_PROPS_STRUCT{
INT start_angle;
INT end_angle;
INT brush_width;
INT solid_fill;
INT pixelmap_fill;
INT alpha_mode;
INT compress_mode;
INT anti_aliased_mode;
}ARC_PROPS;

ARC_PROPS arc_props[]={
    {0, 90, 1, 0, 0, 0, 0, 1},
    {10, 30, 2, 1, 0, 0, 0, 1},
    {10, 65, 3, 0, 1, 0, 0, 1},
    {100, 115, 5, 0, 1, 0, 1, 1},
    {110, 175, 7, 0, 1, 2, 0, 1},
    {180, 190, 9, 0, 1, 2, 1, 1},
    {185, -115, 10, 1, 0, 0, 0, 1},
    {-10, -55, 12, 1, 0, 0, 0, 1},
    {15, 175, 13, 1, 0, 0, 0, 1},
    {16, 260, 14, 1, 0, 0, 0, 1},
    {25, 335, 15, 1, 0, 0, 0, 1},
    {115, 200, 16, 1, 0, 0, 0, 1},
    {160, 300, 17, 1, 0, 0, 0, 1},
    {210, 355, 18, 1, 0, 0, 0, 1},
    {0, 90, 1, 0, 0, 0, 0, 0},
    {185, -115, 10, 1, 0, 0, 0, 0},
    {-10, -55, 12, 1, 0, 0, 0, 0},
    {115, 200, 16, 1, 0, 0, 0, 0},
    {210, 355, 18, 1, 0, 0, 0, 0},
};

static INT brush_alpha_value[] = {0, 155, 255};

char test_comment[256];


/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
INT index, brush_alpha_index;
INT frame_id = 1;

    draw_shape = ARC;

    /* Loop radius from 5 to 200. */
    for(radius = 1; radius < 200; radius += 50)
    {
        for(brush_alpha_index = 0; brush_alpha_index < (INT)(sizeof(brush_alpha_value)/sizeof(INT)); brush_alpha_index++)
        {
           brush_alpha = brush_alpha_value[brush_alpha_index];
    
            for(index = 0; index < (INT)(sizeof(arc_props)/sizeof(ARC_PROPS)); index++)
            {
                start_angle = arc_props[index].start_angle;
                end_angle = arc_props[index].end_angle;

                if(radius < 30)
                {
                    brush_width = 1;
                }
                else
                {
                    brush_width = arc_props[index].brush_width;
                }

                solid_fill = arc_props[index].solid_fill;
                pixelmap_fill = arc_props[index].pixelmap_fill;
                alpha = arc_props[index].alpha_mode;
                compress = arc_props[index].compress_mode;
                pixelmap_index = alpha + compress;
                anti_aliased = arc_props[index].anti_aliased_mode;
                /* Set frame id. */
                gx_validation_set_frame_id(frame_id);

                frame_id++;

                /* Set test comments. */
                sprintf(test_comment, "radius:%d, start_angle:%d, end_angle:%d, brush_width %d, solid_fill:%d, pixelmap_fill:%d, alpha:%d, compress: %d, brush_alpha: %d, anti_aliased: %d", radius, start_angle, end_angle, brush_width, solid_fill, pixelmap_fill, alpha, compress, brush_alpha, anti_aliased);

                gx_validation_set_frame_comment(test_comment);

                /* Mark graphics window dirty. */
                gx_system_dirty_mark(pGraphicsWin);

                /* Force a screen refresh.  */
                gx_validation_screen_refresh();
            }
        }
    }

    anti_aliased = GX_TRUE;
    brush_alpha = 255;
    root->gx_window_root_canvas->gx_canvas_display->gx_display_driver_pixel_blend = GX_NULL;

    brush_width = 1;
    gx_validation_screen_refresh();

    brush_width = 2;
    gx_validation_screen_refresh();

    /* Signal the end of the test case, Verify the outout.  */
    gx_validation_end();

    exit(0);
}
