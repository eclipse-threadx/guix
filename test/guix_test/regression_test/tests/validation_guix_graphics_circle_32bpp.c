/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_graphics_circle_32bpp", /* Test name */
    28, 24, 400, 425  /* Define the coordinates of the capture area.
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
#ifdef win32_graphics_driver_setup_24xrgb  
#undef win32_graphics_driver_setup_24xrgb  
#endif
#define win32_graphics_driver_setup_24xrgb  gx_validation_graphics_driver_setup_24xrgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_graphics_32bpp.c"



static int radius_value[]={1, 100, 200, 210};

static int brush_value[]={0, 1, 10};


static int fill_color_value[]={GX_COLOR_ID_CANVAS, GX_COLOR_ID_SCROLL_BUTTON};

static int wallpaper_value[] = {GX_NULL, GX_PIXELMAP_ID_CHECKBOX_OFF};

static int brush_alpha_value[] = {0, 155, 255};

static int pixelmap_fill_value[] = {GX_FALSE, GX_TRUE};

char test_comment[256];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int b_index,r_index, wallpaper_index,  fill_color_index, brush_alpha_index, pixelmap_fill_index, pixelmap_id_index, a_index;
int frame_id = 1;
GX_SLIDER *slider_b;
GX_SLIDER *slider_r;
int index;

    gx_widget_find(pMainWin, ID_RADIUS_SLIDER, 0, &slider_r);
    gx_widget_find(pMainWin, ID_BRUSH_WIDTH, 0, &slider_b);

    draw_shape = CIRCLE;

    for(wallpaper_index = 0; wallpaper_index < (INT)(sizeof(wallpaper_value)/sizeof(INT)); wallpaper_index++)
    {
        for(brush_alpha_index = 0; brush_alpha_index < (INT)(sizeof(brush_alpha_value)/sizeof(INT)); brush_alpha_index++)
        {
            brush_alpha = brush_alpha_value[brush_alpha_index];

            for(fill_color_index = 0; fill_color_index < (INT)(sizeof(fill_color_value)/sizeof(INT)); fill_color_index++ )
            {  	
	        fill_color = fill_color_value[fill_color_index];
                for(b_index = 0; b_index < (INT)(sizeof(brush_value) / sizeof(int)); b_index++)
	            {
         		    gx_slider_value_set(slider_b, &slider_b -> gx_slider_info, brush_value[b_index]);

		            for(r_index = 0; r_index < (INT)(sizeof(radius_value) / sizeof(int)); r_index++)
		            {
                        for(pixelmap_fill_index = 0; pixelmap_fill_index < (INT)(sizeof(pixelmap_fill_value)/sizeof(INT)); pixelmap_fill_index++)
                        {
                            pixelmap_fill = pixelmap_fill_value[pixelmap_fill_index];
                            for(a_index = 0; a_index < 2; a_index++)
                            {
                                anti_aliased = (GX_BOOL)a_index;
                                if(pixelmap_fill)
                                {
                                   for(pixelmap_id_index = 0; pixelmap_id_index < (INT)(sizeof(pixelmap_id)/sizeof(INT)); pixelmap_id_index++)
                                   {
                                        pixelmap_index = pixelmap_id_index;

                                        /* Inform the validation system 
                                       (1) Frame ID, which identifies a specific test configuration;
                                       (2) Start recording frame on the next toggle operation.
                                         */
                                        gx_validation_set_frame_id(frame_id);
    
                                        /* Set the vertical slider value */
                                        gx_slider_value_set(slider_r, &slider_r -> gx_slider_info, radius_value[r_index]);

                                        sprintf(test_comment, "brush width %d, brush_alpha:%d, radius %d, pixelmap_index: %d", brush_value[b_index], brush_alpha, radius_value[r_index], pixelmap_index);

                                        gx_validation_set_frame_comment(test_comment);

                                        /* Mark the window "dirty" */
                                        gx_system_dirty_mark(pGraphicsWin);

                                        /* Force a screen refresh. */
                                        gx_validation_screen_refresh();

                                        /* Increment frame ID */
                                        frame_id ++;


                                   }
                                }
                                else
                                {
                                    /* Inform the validation system 
                                    (1) Frame ID, which identifies a specific test configuration;
                                    (2) Start recording frame on the next toggle operation.
                                     */
                                     gx_validation_set_frame_id(frame_id);

                                     /* Set the vertical slider value */
                                     gx_slider_value_set(slider_r, &slider_r -> gx_slider_info, radius_value[r_index]);

                                     sprintf(test_comment, "brush width %d, brush_alpha:%d, radius %d, ", brush_value[b_index], brush_alpha, radius_value[r_index]);

                                     gx_validation_set_frame_comment(test_comment);

                                     /* Mark the window "dirty" */
                                     gx_system_dirty_mark(pGraphicsWin);
 
                                     /* Force a screen refresh. */
                                     gx_validation_screen_refresh();

                                     /* Increment frame ID */
                                     frame_id ++;

                                }	
                            }
                        }
		            }
	            }
            }
	    }
	
    } 
   
    brush_alpha = 255;
    anti_aliased = GX_TRUE;

    for(index = 0; index < 4; index++)
    {
        switch(index)
        {
        case 0:
             brush_width = 1;
             radius = 300;
             sprintf(test_comment, "brush_width = 1, radius = 300");
             break;

        case 1:
             brush_width = 2;
             sprintf(test_comment, "brush_width = 2, radius = 300");
             break;

        case 2:
             brush_width = 1;
             root->gx_window_root_canvas->gx_canvas_display->gx_display_driver_pixel_blend = GX_NULL;
             sprintf(test_comment, "brush_width = 1, pixel_blend = null");
             break;

        case 3:
             brush_width = 2;
             sprintf(test_comment, "brush_width = 2, pixel_blend = null");
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





