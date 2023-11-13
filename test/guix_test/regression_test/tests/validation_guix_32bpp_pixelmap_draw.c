/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_32bpp_pixelmap_draw", /* Test name */
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
#ifdef win32_graphics_driver_setup_24xrgb  
#undef win32_graphics_driver_setup_24xrgb  
#endif
#define win32_graphics_driver_setup_24xrgb  gx_validation_graphics_driver_setup_24xrgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_pixelmaps_32bpp.c"

char test_comment[256];

static int test_map_list[]={
GX_PIXELMAP_ID_APPLE_ALPHA_565RGB,
GX_PIXELMAP_ID_COMPRESS_ALPHA_565RGB,
GX_PIXELMAP_ID_APPLE_COMPRESS_TRANSPARENT_PALETTE,
GX_PIXELMAP_ID_COMPRESS_PALETTE
};

static int x_shift = -50;
static int test_map_id = 0;
static int blend_alpha = 255;

#define TEST_INVALID_FORMAT  0x1
#define TEST_INVALID_PALETTE 0x2
#define TEST_INVALID_TRANSPARENT_PALETTE 0x3
#define TEST_INVALID_BLEND 0x04
#define TEST_INVALID_PIXELMAP_DATA 0x05

static int test_invalid_type = 0;
static GX_UBYTE pixelmap_data_1[] = {0x00, 0x00, 0x00};
static GX_UBYTE pixelmap_data_2[] = {0xff, 0x00, 0x00};
static GX_UBYTE *pixelmap_data = GX_NULL;

VOID test_pic_win_draw(GX_WINDOW *window)
{
GX_PIXELMAP *map;
GX_PIXELMAP invalid_map;
int xpos;
int ypos;
GX_BRUSH *brush;

    gx_window_background_draw(window);

    gx_context_brush_get(&brush);
    brush->gx_brush_alpha = blend_alpha;

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

        case TEST_INVALID_PALETTE:
            invalid_map.gx_pixelmap_format = GX_COLOR_FORMAT_8BIT_PALETTE;
            break;

        case TEST_INVALID_TRANSPARENT_PALETTE:
            invalid_map.gx_pixelmap_format = GX_COLOR_FORMAT_8BIT_PALETTE;
            invalid_map.gx_pixelmap_flags = GX_PIXELMAP_TRANSPARENT;
            break;

        case TEST_INVALID_BLEND:
            break;

        case TEST_INVALID_PIXELMAP_DATA:
            invalid_map.gx_pixelmap_flags = GX_PIXELMAP_RAW_FORMAT;
            invalid_map.gx_pixelmap_data = pixelmap_data;
            invalid_map.gx_pixelmap_data_size = 30;
            break;
        }
        map = &invalid_map;
    }
    else
    {
        gx_widget_pixelmap_get((GX_WIDGET *)window, test_map_id, &map);
    }
    gx_canvas_pixelmap_draw(xpos, ypos, map);
}
/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
int map_index;
int index;
GX_CANVAS *canvas;

    gx_widget_draw_set((GX_WIDGET *)&main_window.main_window_pic_window, test_pic_win_draw);
    
    for(map_index = 0; map_index < (int)(sizeof(test_map_list) / sizeof(int)); map_index++)
    {
        test_map_id = test_map_list[map_index];

        for(blend_alpha = 128; blend_alpha < 256; blend_alpha += 127)
        {
            for(x_shift = -50; x_shift <= 100; x_shift += 150)
            {
                sprintf(test_comment, "map_id = %d, blend_alpha = %d, x_shift = %d", test_map_list[map_index], blend_alpha, x_shift);
                gx_validation_set_frame_id(frame_id++);
                gx_validation_set_frame_comment(test_comment);
                gx_validation_screen_refresh();
            }
        }
    }    

    for(index = 0; index < 8; index++)
    {
        switch(index)
        {
        case 0:
            test_invalid_type = TEST_INVALID_FORMAT;
            blend_alpha = 128;
            sprintf(test_comment, "test draw with invalid pixelmap format, blend alpha = 128");
            break;

        case 1:
            test_invalid_type = TEST_INVALID_FORMAT;
            blend_alpha = 255;
            sprintf(test_comment, "test draw with invalid pixelmap format, blend alpha = 255");
            break;

        case 2:
            test_invalid_type = TEST_INVALID_PALETTE;
            blend_alpha = 255;
            sprintf(test_comment, "test invalid palette pixelmap without aux data");
            break;

        case 3:
            test_invalid_type = TEST_INVALID_TRANSPARENT_PALETTE;
            blend_alpha = 255;
            sprintf(test_comment, "test invalid transparent palette pixelmap withou aux data");
            break;

         case 4:
            test_invalid_type = TEST_INVALID_BLEND;
            blend_alpha = 128;
            sprintf(test_comment, "test blend with invalid pixelmap format");
            break;

         case 5:
            test_invalid_type = TEST_INVALID_PIXELMAP_DATA;
            blend_alpha = 255;
            pixelmap_data = pixelmap_data_1;
            sprintf(test_comment, "test raw format pixelmap draw with invalid pixelmap data");
            break;

         case 6:
            pixelmap_data = pixelmap_data_2;
            break;

         case 7:
            test_invalid_type = 0;
            x_shift = 10;
            gx_widget_canvas_get(&main_window, &canvas);
            canvas->gx_canvas_display->gx_display_driver_pixel_write = GX_NULL;
            sprintf(test_comment, "set gx_dispaly_driver_pixel_write = GX_NULL");
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





