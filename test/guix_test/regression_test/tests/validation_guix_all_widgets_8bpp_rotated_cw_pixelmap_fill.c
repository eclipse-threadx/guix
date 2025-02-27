/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_8bpp_rotated_cw_pixelmap_fill", /* Test name */
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
#ifdef win32_graphics_driver_setup_8bit_palette_rotated
#undef win32_graphics_driver_setup_8bit_palette_rotated
#endif
#define win32_graphics_driver_setup_8bit_palette_rotated  gx_validation_graphics_driver_setup_8bit_palette_rotated


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets_8bpp_rotated_cw.c"

extern GX_STUDIO_DISPLAY_INFO all_widgets_8bpp_rotated_cw_display_table[];
extern GX_RESOURCE_ID pixelmap_id[];
extern INT fill_pixelmap_index;
extern GX_BOOL pixelmap_fill;
extern INT draw_shape;
extern INT start_angle;
extern INT end_angle;

char test_comment[256];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
INT frame_id = 1;
GX_CONST GX_THEME *theme_ptr;
GX_DRAW_CONTEXT context;
GX_FILL_PIXELMAP_INFO info;

    /* Save palette info. */
    theme_ptr = all_widgets_8bpp_rotated_cw_display_table[MAIN_DISPLAY].theme_table[MAIN_DISPLAY_THEME_1];
    gx_validation_write_palette(theme_ptr -> theme_palette, theme_ptr->theme_palette_size);

    /* Toggle to shape screen.  */
    ToggleScreen((GX_WINDOW *)&shapes_screen, (GX_WINDOW *)&button_screen);

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Shape screen");
    gx_validation_screen_refresh();

    start_angle = 30;
    end_angle = 200;
    
    for(draw_shape = 0; draw_shape < 5; draw_shape ++)
    {
        for(fill_pixelmap_index = 0; fill_pixelmap_index < 5; fill_pixelmap_index++)
        {
            sprintf(test_comment, "shape id = %d. ", draw_shape);

            if(fill_pixelmap_index < 4)
            {
                pixelmap_fill = GX_TRUE;

                /* Test shape fill with different pixelmap format.  */
                switch(pixelmap_id[fill_pixelmap_index])
                {
                    case GX_PIXELMAP_ID__RAW:
                        strcat(test_comment, "test shape pixelmap fill: without alpha, without compression");
                        break;
                    case GX_PIXELMAP_ID__TRANS:
                        strcat(test_comment, "test shape pixelmap fill: with alpha, without compression");
                        break;
                    case GX_PIXELMAP_ID__COMPRESS:
                        strcat(test_comment, "test shape pixelmap fill: without alpha, with compression");
                        break;
                    case GX_PIXELMAP_ID__COMPRESS_TRANS:
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

    draw_shape = 0;
    fill_pixelmap_index = 0;

    memset(&context, 0, sizeof(GX_DRAW_CONTEXT));
    memset(&info, 0, sizeof(GX_FILL_PIXELMAP_INFO));
    context.gx_draw_context_display = root->gx_window_root_canvas->gx_canvas_display;
    context.gx_draw_context_canvas = root->gx_window_root_canvas;
    root->gx_window_root_canvas->gx_canvas_display->gx_display_driver_horizontal_pixelmap_line_draw(&context, 40, 43, 1, &info);

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("call horizontal pixelmap line draw with info->pixelmap = GX_NULL");   
    gx_validation_screen_refresh();

    /* Signal the end of the test case, verify the outout.  */
    gx_validation_end();

    exit(0);
}
