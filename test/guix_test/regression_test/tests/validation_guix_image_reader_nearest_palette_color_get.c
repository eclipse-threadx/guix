/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_image_reader_nearest_palette_color_get", /* Test name */
    0, 0, 640, 480  /* Define the coordinates of the capture area.
                    In this test, we only need to capture the prompt
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
    on the prompt drawing example. These operations simulate
    user input. */
    gx_validation_control_thread_create(control_thread_entry);

    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Call the actual prompt routine. */
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

#include "demo_guix_color_convert_8bpp.c"

static VOID pixelmap_screen_draw(GX_WINDOW *widget)
{
    GX_PIXELMAP *pixelmap;

    gx_window_draw(pixelmap_window);

    gx_widget_pixelmap_get((GX_WIDGET *)widget, map_id, &pixelmap);

    gx_image_reader_create(&image_reader,
        pixelmap->gx_pixelmap_data,
        pixelmap->gx_pixelmap_data_size,
        GX_COLOR_FORMAT_8BIT_PALETTE,
        image_reader_mode);

    if (gx_image_reader_start(&image_reader, &out_pixelmap) == GX_SUCCESS)
    {
        gx_canvas_pixelmap_draw(widget->gx_widget_size.gx_rectangle_left + 10,
            widget->gx_widget_size.gx_rectangle_top + 10,
            &out_pixelmap);

        if (out_pixelmap.gx_pixelmap_data)
        {
            convert_memory_free((VOID *)out_pixelmap.gx_pixelmap_data);
            out_pixelmap.gx_pixelmap_data = GX_NULL;
        }

        if (out_pixelmap.gx_pixelmap_aux_data)
        {
            convert_memory_free((VOID *)out_pixelmap.gx_pixelmap_aux_data);
            out_pixelmap.gx_pixelmap_aux_data = GX_NULL;
        }
    }
}

extern GX_STUDIO_DISPLAY_INFO color_convert_8bpp_display_table[];

static VOID control_thread_entry(ULONG input)
{
INT frame_id = 1;
GX_CONST GX_THEME *theme_ptr;

    /* Save palette info. */
    theme_ptr = color_convert_8bpp_display_table[DISPLAY_1].theme_table[DISPLAY_1_THEME_1];

    gx_validation_write_palette(theme_ptr -> theme_palette, theme_ptr->theme_palette_size);

    gx_validation_set_frame_id(frame_id);

    frame_id++;

    gx_validation_set_frame_comment("Original screen.");
    /* Mark the window "dirty". */
    gx_system_dirty_mark(pMainWin);
    /* Force a screen refresh. */
    gx_validation_screen_refresh();
    
    gx_widget_draw_set((GX_WIDGET *)&window.window_pixelmap_window ,(void (*)(GX_WIDGET *))pixelmap_screen_draw);

    gx_validation_set_frame_id(frame_id);

    frame_id++;

    gx_validation_set_frame_comment("Call color space convert but don't set palette.");
    /* Mark the window "dirty". */
    gx_system_dirty_mark(pMainWin);
    /* Force a screen refresh. */
    gx_validation_screen_refresh();
    

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
