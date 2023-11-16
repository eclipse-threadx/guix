/*This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_display.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_1bpp_pixelmap_line_draw", /* Test name */
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
#ifdef win32_graphics_driver_setup_monochrome  
#undef win32_graphics_driver_setup_monochrome
#endif
#define win32_graphics_driver_setup_monochrome  gx_validation_graphics_driver_setup_monochrome


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets_1bpp.c"

char test_comment[256];
static int map_id = GX_PIXELMAP_ID_BLACKBOX_COMRESSED;

VOID test_graphics_draw(GX_WINDOW *window)
{
    INT      xcenter = 213;
    INT      ycenter = 230;

    gx_window_draw((GX_WINDOW*)window);

    gx_context_brush_define(GX_COLOR_ID_BLACK, GX_COLOR_ID_WHITE, GX_BRUSH_PIXELMAP_FILL);
    gx_context_pixelmap_set(map_id);
    gx_context_brush_width_set(1);


    gx_canvas_circle_draw(xcenter, ycenter, 160);
}

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
int index;
GX_FILL_PIXELMAP_INFO info;

    ToggleScreen(pShapesScreen, pSpriteScreen);

    gx_widget_draw_set((GX_WIDGET *)&shapes_screen.shapes_screen_graphics_window, test_graphics_draw);

    for(index = 0; index < 3; index++)
    {
        switch(index)
        {
        case 0:
            map_id = GX_PIXELMAP_ID_BLACKBOX_COMRESSED;
            sprintf(test_comment, "set fill map: compressed");
            break;

        case 1:
            map_id = GX_PIXELMAP_ID_SAVE_ICON_RAW;
            sprintf(test_comment, "set fill map: raw");
            break;

        case 2:
            memset(&info, 0, sizeof(GX_FILL_PIXELMAP_INFO));
            root->gx_window_root_canvas->gx_canvas_display->gx_display_driver_horizontal_pixelmap_line_draw(GX_NULL, 0, 0, 0, &info);
            sprintf(test_comment, "call 1bpp horizontal pixelmap line draw with info->pixelmap = GX_NULL");
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






