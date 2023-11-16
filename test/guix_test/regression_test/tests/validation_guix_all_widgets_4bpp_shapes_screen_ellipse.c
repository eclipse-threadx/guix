/*This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_4bpp_shapes_screen_ellipse", /* Test name */
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
#ifdef win32_graphics_driver_setup_4bpp_grayscale  
#undef win32_graphics_driver_setup_4bpp_grayscale  
#endif
#define win32_graphics_driver_setup_4bpp_grayscale  gx_validation_graphics_driver_setup_4bpp_grayscale


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets_4bpp.c"

char test_comment[256];
extern ULONG palette_4bpp_grayscale[256];
extern GX_BOOL solid_fill;
extern GX_BOOL pixelmap_fill;
extern INT     compressed;
extern INT     transparent;
extern INT     radius;
extern INT     brush_width;
extern INT     draw_shape;

static INT value[2] = {GX_TRUE,GX_FALSE};
static INT brush_value[] = {0, 3, 10, 20};
static INT radius_value[] = {1, 10, 100, 200};

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int a,b,c = 0, d = 0, i,j;
int frame_id = 1;

    gx_validation_write_palette((ULONG *)palette_4bpp_grayscale, 256);
    ToggleScreen(pShapesScreen, pSpriteScreen);
    /*draw shape is ellipse.*/
    draw_shape = 4;
    /*radius width*/
    for( j= 0; j < (int)(sizeof(radius_value)/sizeof(int)); j++)
    {
        radius = radius_value[j];
        /*Brush width*/
        for(i = 0; i < (int)(sizeof(brush_value)/sizeof(int)); i++)
        {
            brush_width = brush_value[i];
            c = 0;
            d = 0;
            /*pixelmap_fill is true or false*/
            for(a = 0; a < (int)(sizeof(value)/sizeof(int)); a++)
            {
                pixelmap_fill = value[a];
                /*solid_fill is true or false*/
                for(b = 0; b < (int)(sizeof(value)/sizeof(int)); b++)
                {
                    solid_fill = value[b];
                    /*solid_fill is true or false*/
                    
                        /* Inform the validation system 
                        (1) Frame ID, which identifies a specific test configuration;
                        (2) Start recording frame on the next toggle operation.
                        */
                    
                    if(pixelmap_fill)
                    {

                       for(c = 0; c <=1; c++)
                        {
                            for(d=0; d<=1; d++)
                            {
                                gx_validation_set_frame_id(frame_id);
                                sprintf(test_comment, "%d, brush_width %d, radius %d, ", frame_id, brush_width, radius);
                                strcat(test_comment, "pixelmap_fill, ");
         
                                compressed = c;
                                transparent = d;
                                if(compressed){strcat(test_comment, "with compressed, ");}
                                else{strcat(test_comment, " without compress, ");}
                                if(transparent){strcat(test_comment, "with transparent, ");}
                                else{strcat(test_comment, " without transparent, ");}


                                gx_validation_set_frame_comment(test_comment);
                                /* Mark the window "dirty" */
                                gx_system_dirty_mark(pShapesScreen);

                                /* Force a screen refresh. */
                                gx_validation_screen_refresh();

                                /* Increment frame ID */
                                frame_id ++;
                            }
                        }

                    }
                    else
                    {
                        gx_validation_set_frame_id(frame_id);
                        sprintf(test_comment, "%d, brush_width %d, radius %d, ", frame_id, brush_width, radius);
                        if(pixelmap_fill){strcat(test_comment, "pixelmap_fill, ");}
                        else{strcat(test_comment, " without pixelmap_fill, ");}
                        if(solid_fill){strcat(test_comment, "solid_fill, ");}
                        else{strcat(test_comment, " without solid_fill, ");}
         
                        gx_validation_set_frame_comment(test_comment);
                        /* Mark the window "dirty" */
                        gx_system_dirty_mark(pShapesScreen);

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






