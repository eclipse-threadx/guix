/* This is a small demo of the high-performance GUIX graphics framework. */


#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_display_driver_4444argb_pixelmap_rotate", /* Test name */
    48, 68, 600, 404  /* Define the coordinates of the capture area.
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
#define win32_graphics_driver_setup_4444argb  gx_validation_graphics_driver_setup_4444argb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets_4444argb.c"

/*NOTICE: Number of xpos_value and ypos_valus should be same.*/

GX_RESOURCE_ID rotate_pixelmap_id_list[]={
    GX_PIXELMAP_ID_RED_APPLE,
    GX_PIXELMAP_ID_ROTATE_FISH
};

char test_comment[256];
static int map_id;
static int angle;

VOID test_pixelmap_draw(GX_WINDOW *window)
{
    INT  xpos;
    INT  ypos;
    GX_PIXELMAP *map;
    GX_RECTANGLE size;

    gx_window_draw((GX_WINDOW*)window);

    gx_context_pixelmap_get(map_id, &map);

    size = window->gx_widget_size;

    xpos = (size.gx_rectangle_right + size.gx_rectangle_left - map->gx_pixelmap_width) >> 1;
    ypos = (size.gx_rectangle_top + size.gx_rectangle_bottom - map->gx_pixelmap_height) >> 1;
 
    gx_canvas_pixelmap_rotate(xpos, ypos, map, angle, map->gx_pixelmap_width - 1, map->gx_pixelmap_height - 1);
}

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
int index;

    gx_widget_draw_set(&rotate_screen.rotate_screen_pixelmap_window, test_pixelmap_draw);

    for(index = 0; index < (int)(sizeof(rotate_pixelmap_id_list) / sizeof(int)); index++)
    {
        map_id = rotate_pixelmap_id_list[index];

        for(angle = 0; angle < 360; angle ++)
        {
            /* Capture a frame. */
            gx_validation_set_frame_id(frame_id++);
            sprintf(test_comment, "pixelmap_id = %d, angle = %d", map_id, angle);
            gx_validation_set_frame_comment(test_comment);
            gx_validation_screen_refresh();
        }
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);

}
