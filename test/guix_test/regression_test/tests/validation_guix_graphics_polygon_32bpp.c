/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_graphics_polygon_32bpp", /* Test name */
    28, 24, 400, 430  /* Define the coordinates of the capture area.
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
#include "demo_guix_graphics_32bpp.c"

char test_comment[255];
static INT brush_alpha_value[] = {0, 155, 255};

GX_BOOL style_status[2] = {GX_TRUE, GX_FALSE};
/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
int brush_alpha_index, pixelmap_id_index, index1, index2;

    draw_shape = POLYGON;
    for (brush_alpha_index = 0; brush_alpha_index < (INT)(sizeof(brush_alpha_value)/sizeof(INT)); brush_alpha_index ++)
    {
	brush_alpha = brush_alpha_value[brush_alpha_index];

	for(index1 = 0; index1 < 2; index1 ++)
	{
	    anti_aliased = style_status[index1];
	    
	    for(index2 = 0; index2 < 2; index2++)
            {
		solid_fill = style_status[index2];
                pixelmap_fill = !solid_fill;
               
                for(brush_width = 0; brush_width <= 20; brush_width+= 9)
                {

                    if(pixelmap_fill)
                    {
                        for(pixelmap_id_index = 0; pixelmap_id_index < (INT)(sizeof(pixelmap_id)/sizeof(INT)); pixelmap_id_index++)
                        {
                            pixelmap_index = pixelmap_id_index;
                            gx_validation_set_frame_id(frame_id);
                            sprintf(test_comment, "brush alpha: %d anti-aliased: %d solide fill: %d pixelmap fill: %d pixelmap_id_index: %d brush width: %d", brush_alpha, anti_aliased, solid_fill, pixelmap_fill, pixelmap_index, brush_width);

                            gx_validation_set_frame_comment(test_comment);
            
                            /* Mark the window "dirty" */
                            gx_system_dirty_mark(pMainWin);
            
                            /* Force a screen refresh. */
                            gx_validation_screen_refresh();
            
                            /* Increment frame ID */
                            frame_id ++;
   
                        }
                    }
                    else
                    {
                         gx_validation_set_frame_id(frame_id);
                         sprintf(test_comment, "brush alpha: %d anti-aliased: %d solide fill: %d pixelmap fill: %d brush width: %d", brush_alpha, anti_aliased, solid_fill, pixelmap_fill, brush_width);

                        gx_validation_set_frame_comment(test_comment);
            
                        /* Mark the window "dirty" */
                        gx_system_dirty_mark(pMainWin);
            
                        /* Force a screen refresh. */
                        gx_validation_screen_refresh();
            
                        /* Increment frame ID */
                        frame_id ++;

                    }

                                   }
 	    }
	}
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
