/* This is a small demo of the high-performance GUIX graphics framework. */


#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_utility_string_to_alphamap", /* Test name */
    31, 69, 613, 400  /* Define the coordinates of the capture area.
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

#include "demo_guix_all_widgets.c"

char test_comment[256];
GX_UBYTE test_string[]={'N', 'v', 'e', 'm', 'b', 'e', 'r', 0xe4, 0xb8, 0xad, '/', 0};
GX_UBYTE test_invalid_string[]={'a', 'b', 0xff, 0};

GX_PIXELMAP *map = GX_NULL;
void test_window_draw(GX_WINDOW *window)
{
    gx_window_draw(window);

    if(map)
    {
        gx_context_fill_color_set(GX_COLOR_ID_BLACK);
        gx_canvas_pixelmap_draw(window->gx_widget_size.gx_rectangle_left + 10, window->gx_widget_size.gx_rectangle_top + 10, map);

        if(map->gx_pixelmap_data)
        {
            rotate_memory_free((void *)map->gx_pixelmap_data);
        }    
        map = GX_NULL;
    }
}

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int         frame_id = 1;
GX_FONT    *font;
GX_FONT     font_page[2];
GX_PIXELMAP alphamap;
int         index;
GX_UBYTE   *string;

    ToggleScreen((GX_WINDOW *)&rotate_screen, pButtonScreen);
    gx_widget_draw_set(&rotate_screen.rotate_screen_pixelmap_window, test_window_draw);

    gx_widget_font_get(&rotate_screen, GX_FONT_ID_SELECTED_FONT, &font);

    memset(&font_page[0], 0, sizeof(GX_FONT));
    font_page[0].gx_font_format = GX_FONT_FORMAT_8BPP;
    font_page[0].gx_font_first_glyph = 0x00;
    font_page[0].gx_font_last_glyph = 0x04;
    font_page[0].gx_font_line_height = 31;
    font_page[0].gx_font_baseline = 24;
    font_page[0].gx_font_next_page = &font_page[1];

    memset(&font_page[1], 0, sizeof(GX_FONT));
    font_page[1].gx_font_format = GX_FONT_FORMAT_8BPP;
    font_page[1].gx_font_first_glyph = 0x80;
    font_page[1].gx_font_last_glyph = 0x81;
    font_page[1].gx_font_line_height = 31;
    font_page[1].gx_font_baseline = 24;
    font_page[1].gx_font_next_page = font;
   
    font = &font_page[0];

    for(index = 0; index < 4; index++)
    {
        switch(index)
        {
        case 0:
            string = test_string;
            sprintf(test_comment, "Convert string to alphamap");
            break;

        case 1:
            string = test_invalid_string;
            sprintf(test_comment, "Convert invalid string to alphamap");
            break;
       
        case 2:
            string = test_string;
            font->gx_font_format = GX_FONT_FORMAT_2BPP;
            sprintf(test_comment, "Set font format to 2BPP");
            break;

        case 3:
            string = test_string;
            font->gx_font_line_height = 0;
            sprintf(test_comment, "set font line height 0");
            break;
        }

        if(gx_utility_string_to_alphamap((char *)string, font, &alphamap) == GX_SUCCESS)
        {
            map = &alphamap;
        }
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





