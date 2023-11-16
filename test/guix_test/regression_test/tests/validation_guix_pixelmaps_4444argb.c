/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_pixelmaps_4444argb", /* Test name */
    9, 11, 280, 290  /* Define the coordinates of the capture area.
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
#ifdef win32_graphics_driver_setup_4444argb
#undef win32_graphics_driver_setup_4444argb
#endif
#define win32_graphics_driver_setup_4444argb gx_validation_graphics_driver_setup_4444argb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_pixelmaps_4444argb.c"

char test_comment[256];

static INT brush_alpha_value[] = {0, 100, 200, 255};

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{

int brush_alpha_index;
int frame_id = 1;
GX_RECTANGLE size;
GX_CANVAS *canvas;
GX_DISPLAY *display;
    size = pic_window->gx_widget_size;
    
    size.gx_rectangle_left = pMainWin->gx_widget_size.gx_rectangle_left - 40;
    size.gx_rectangle_top = pMainWin->gx_widget_size.gx_rectangle_top - 20;
    
    gx_widget_resize(pic_window, &size);
    gx_validation_set_frame_id(frame_id++);

    sprintf(test_comment, "Resize pic window");

    gx_validation_set_frame_comment(test_comment);

    gx_window_wallpaper_set(pic_window, maps[style_index], GX_FALSE);

    /* Force a screen refresh. */
    gx_validation_screen_refresh();


    for(brush_alpha_index = 0; brush_alpha_index < (INT)(sizeof(brush_alpha_value)/sizeof(INT)); brush_alpha_index++)
    {
        brush_alpha = brush_alpha_value[brush_alpha_index];
        for(style_index = 0; style_index < 4; style_index++)
        {
            /* Inform the validation system 
            (1) Frame ID, which identifies a specific test configuration;
            (2) Start recording frame on the next toggle operation.
            */
            gx_validation_set_frame_id(frame_id);

            sprintf(test_comment, "brush alpha: %d, pixelmap_style_index: %d, ", brush_alpha, style_index);

            gx_validation_set_frame_comment(test_comment);

            gx_window_wallpaper_set(pic_window, maps[style_index], GX_FALSE);

            /* Force a screen refresh. */
            gx_validation_screen_refresh();

            /* Increment frame ID */
            frame_id ++;
        }
    }
    
    gx_widget_canvas_get(pic_window, &canvas);
    display = canvas->gx_canvas_display;
    
    display->gx_display_driver_pixel_blend = NULL;
    brush_alpha = 100;
    for(style_index = 0; style_index < 4; style_index++)
    {
        /* Inform the validation system 
        (1) Frame ID, which identifies a specific test configuration;
        (2) Start recording frame on the next toggle operation.
        */
        gx_validation_set_frame_id(frame_id);

        sprintf(test_comment, "brush alpha: %d, pixelmap_style_index: %d, ", brush_alpha, style_index);

        gx_validation_set_frame_comment(test_comment);

        gx_window_wallpaper_set(pic_window, maps[style_index], GX_FALSE);

        /* Force a screen refresh. */
        gx_validation_screen_refresh();
        /* Increment frame ID */
        frame_id ++;
    }
    
    
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





