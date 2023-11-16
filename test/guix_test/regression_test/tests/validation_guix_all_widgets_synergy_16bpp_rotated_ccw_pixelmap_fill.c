/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_synergy_16bpp_rotated_ccw_pixelmap_fill", /* Test name */
    51, 9, 442, 400  /* Define the coordinates of the capture area.
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
#define _gx_synergy_display_driver_setup gx_validation_dave2d_graphics_driver_setup_565rgb_rotated


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets_synergy_16bpp_rotated_ccw.c"

extern GX_RESOURCE_ID pixelmap_id[];
extern INT fill_pixelmap_index;
extern GX_BOOL pixelmap_fill;
extern INT draw_shape;
extern GX_UBYTE brush_alpha;

#define TEST_ALPHA_CASE 3
char test_comment[256];
int test_alpha_list[TEST_ALPHA_CASE] = {0, 100, 255};

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
INT frame_id = 1;
INT alpha_index;

    /* Toggle to shape screen.  */
    ToggleScreen((GX_WINDOW *)&shapes_screen, (GX_WINDOW *)&button_screen);

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Shape screen");
    gx_validation_screen_refresh();

    for(alpha_index = 0; alpha_index < TEST_ALPHA_CASE; alpha_index++ )
    {
        /* Set brush alpha. */
        brush_alpha = test_alpha_list[alpha_index];

        for(draw_shape = 0; draw_shape < 5; draw_shape ++)
        {
            for(fill_pixelmap_index = 0; fill_pixelmap_index < 5; fill_pixelmap_index++)
            {
                sprintf(test_comment, "brush alpha = %d, shape id = %d. ", brush_alpha, draw_shape);

                if(fill_pixelmap_index < 4)
                {
                    pixelmap_fill = GX_TRUE;

                    /* Test shape fill with different pixelmap format.  */
                    switch(pixelmap_id[fill_pixelmap_index])
                    {
                        case GX_PIXELMAP_ID_ID_RAW:
                            strcat(test_comment, "test shape pixelmap fill: without alpha, without compression");
                            break;
                        case GX_PIXELMAP_ID_ID_ALPHA:
                            strcat(test_comment, "test shape pixelmap fill: with alpha, without compression");
                            break;
                        case GX_PIXELMAP_ID_ID_COMPRESS:
                            strcat(test_comment, "test shape pixelmap fill: without alpha, with compression");
                            break;
                        case GX_PIXELMAP_ID_ID_COMPRESS_ALPHA:
                            strcat(test_comment, "test shape pixelmap fill: with alpha, with compression");
                            break;
                    }
                }
                else
                {
                    pixelmap_fill = GX_FALSE;
                }
                

                gx_validation_set_frame_id(frame_id++);
                gx_validation_set_frame_comment(test_comment);
                gx_validation_screen_refresh();
            }
        }
    }

    /* Signal the end of the test case, verify the outout.  */
    gx_validation_end();

    exit(0);
}
