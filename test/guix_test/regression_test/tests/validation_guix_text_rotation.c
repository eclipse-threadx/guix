/* This is a small demo of the high-performance GUIX graphics framework. */


#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_text_rotation", /* Test name */
    0, 0, 639, 479  /* Define the coordinates of the capture area.
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

#include "demo_text_rotation.c"

/*NOTICE: Number of xpos_value and ypos_valus should be same.*/
extern INT            text_angle;
extern GX_RESOURCE_ID text_color;

char test_comment[256];
GX_CONST GX_UBYTE utf8_text[10] = {0xe5, 0xa4, 0x9a, 0xe8, 0xa1, 0x8c, 0 };

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
int angle;
int xcenter, ycenter;
GX_WINDOW_ROOT *window_root = NULL;
    
    gx_window_root_find((GX_WIDGET *)&screen_base.screen_base_text_window, &window_root);
    if(window_root)
    {
        for(angle = 0; angle < 360; angle++)
        {
            if(angle < 180)
            {
                text_color = GX_COLOR_ID_RED;
                if(angle == 0)
                {
                    text_angle = 360;
                }
                else
                {
                    text_angle = angle - 720;
                }
            }
            else
            {
                text_color = GX_COLOR_ID_GREEN;
                text_angle = angle + 360;
            }

            /* Inform the validation system 
            (1) Frame ID, which identifies a specific test configuration;
            (2) Start recording frame on the next toggle operation.
            */
            gx_validation_set_frame_id(frame_id++);
            sprintf(test_comment, "color id = %d, text angle = %d", (int)text_color, text_angle);
            gx_validation_set_frame_comment(test_comment);

            gx_system_dirty_mark((GX_WIDGET *)&screen_base.screen_base_text_window);
            gx_system_dirty_mark((GX_WIDGET *)&screen_base.screen_base_text_window_4bpp);
            gx_system_dirty_mark((GX_WIDGET *)&screen_base.screen_base_text_window_1bpp);
            gx_system_dirty_mark((GX_WIDGET *)&screen_base.screen_base_dynamic_text_window);

            /* Force a screen refresh. */
            gx_validation_screen_refresh();
        }

        /* Test text rotation with text not availlable in font */
        gx_canvas_drawing_initiate(window_root->gx_window_root_canvas, window_root->gx_widget_first_child, &window_root->gx_widget_size);
        gx_context_line_color_set(text_color);
        gx_context_fill_color_set(text_color);
        gx_context_font_set(GX_FONT_ID_SMALL_BOLD);
        xcenter = (window_root->gx_widget_size.gx_rectangle_left + window_root->gx_widget_size.gx_rectangle_right) >> 1;
        ycenter = (window_root->gx_widget_size.gx_rectangle_top + window_root->gx_widget_size.gx_rectangle_bottom) >> 1;
        gx_canvas_rotated_text_draw((GX_CHAR *)utf8_text, xcenter, ycenter, 30);
        gx_canvas_drawing_complete(window_root->gx_window_root_canvas, GX_TRUE);
    }
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
