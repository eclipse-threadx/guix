/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_8bpp_pixelmap_draw", /* Test name */
    27, 23, 231, 160  /* Define the coordinates of the capture area.
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
#ifdef win32_graphics_driver_setup_8bit_palette 
#undef win32_graphics_driver_setup_8bit_palette  
#endif
#define win32_graphics_driver_setup_8bit_palette  gx_validation_graphics_driver_setup_8bit_palette


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_pixelmap_8bpp.c"

char test_comment[256];

static int x_shift = -50;

#define TEST_INVALID_FORMAT  0x1
#define TEST_INVALID_FLAG    0x2

static int test_invalid_type = 0;

VOID test_pic_win_draw(GX_WINDOW *window)
{
GX_PIXELMAP *map;
GX_PIXELMAP invalid_map;
int xpos;
int ypos;

    xpos = window->gx_window_client.gx_rectangle_left + x_shift;
    ypos = window->gx_window_client.gx_rectangle_top - 10;

    if(test_invalid_type)
    {
        memset(&invalid_map, 0, sizeof(GX_PIXELMAP));
        invalid_map.gx_pixelmap_width = 100;
        invalid_map.gx_pixelmap_height = 100;
        switch(test_invalid_type)
        {
        case TEST_INVALID_FORMAT:
            invalid_map.gx_pixelmap_flags = GX_PIXELMAP_COMPRESSED;
            break;

        case TEST_INVALID_FLAG:
            invalid_map.gx_pixelmap_format = GX_COLOR_FORMAT_8BIT_PALETTE;
            invalid_map.gx_pixelmap_flags = GX_PIXELMAP_ALPHA;
            break;
        }
        map = &invalid_map;
    }
    else
    {
        gx_widget_pixelmap_get((GX_WIDGET *)window, GX_PIXELMAP_ID_APPLE_ALPHA_COMPRESS, &map);
    }
    gx_canvas_pixelmap_draw(xpos, ypos, map);
}

extern GX_STUDIO_DISPLAY_INFO pixelmap_8bpp_display_table[1];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
int index;
GX_CONST GX_THEME *theme_ptr;

    /*Save palette. */
    theme_ptr = pixelmap_8bpp_display_table[DISPLAY_1].theme_table[0];
    gx_validation_write_palette(theme_ptr -> theme_palette, theme_ptr->theme_palette_size);

    gx_widget_draw_set((GX_WIDGET *)&main_window.main_window_pic_window, test_pic_win_draw);
    
    for(x_shift = -50; x_shift <= 100; x_shift += 150)
    {
        sprintf(test_comment, "compress, alpha, x_shift = %d", x_shift);
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }

    x_shift = -10;
    for(index = 0; index < 2; index++)
    {
        switch(index)
        {
        case 0:
            test_invalid_type = TEST_INVALID_FORMAT;
            sprintf(test_comment, "test invalid pixelmap format");
            break;

        case 1:
            test_invalid_type = TEST_INVALID_FLAG;
            sprintf(test_comment, "test invalid pixelmap flag");
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





