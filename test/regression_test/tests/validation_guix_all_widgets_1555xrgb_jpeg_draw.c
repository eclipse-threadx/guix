/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_1555xrgb_jpeg_draw", /* Test name */
    367, 75, 590, 365  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the pixelmap
                         drawing area.  */
};

extern void mark_dirty();
extern void convert_start();

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
#ifdef win32_graphics_driver_setup_1555xrgb
#undef win32_graphics_driver_setup_1555xrgb
#endif
#define win32_graphics_driver_setup_1555xrgb  gx_validation_graphics_driver_setup_1555xrgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets_1555xrgb.c"

char test_comment[256];

typedef struct MAP_INFO_STRUCT{
    GX_RESOURCE_ID map_id;
    char id_name[30];
}MAP_INFO;

static MAP_INFO test_map_info_list[] ={
    {GX_PIXELMAP_ID_FISH_JPEG, "GX_PIXELMAP_ID_FISH_JPEG"},
    {GX_PIXELMAP_ID_FISH_SMALL_411, "GX_PIXELMAP_ID_FISH_SMALL_411"},
    {GX_PIXELMAP_ID_FISH_SMALL_420, "GX_PIXELMAP_ID_FISH_SMALL_420"},
    {GX_PIXELMAP_ID_FISH_SMALL_422, "GX_PIXELMAP_ID_FISH_SMALL_422"},
    {GX_PIXELMAP_ID_FISH_SMALL_444, "GX_PIXELMAP_ID_FISH_SMALL_444"},
    {GX_NULL, ""}
};

VOID jpeg_window_draw(GX_WINDOW *window)
{
int xpos;
int ypos;
GX_PIXELMAP *map;

    xpos = window->gx_window_client.gx_rectangle_left - 10;
    ypos = window->gx_window_client.gx_rectangle_top - 10;

    gx_widget_pixelmap_get(window, window->gx_window_wallpaper, &map);
    gx_canvas_pixelmap_draw(xpos, ypos, map);
}

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
INT frame_id = 1;
INT index;
GX_WINDOW *jpeg_win = &image_convert_screen.image_convert_screen_jpeg_to_1555xrgb_win;

    /* Toggle to image convert screen. */
    ToggleScreen((GX_WINDOW *)&image_convert_screen, (GX_WINDOW *)&rotate_screen);

    /* Reset draw function. */
    gx_widget_draw_set(jpeg_win, jpeg_window_draw);

    while(test_map_info_list[index].map_id != GX_NULL)
    {
        gx_validation_set_frame_id(frame_id++);
    
        /* Set the wallpaper of the window.  */
        gx_window_wallpaper_set(jpeg_win, test_map_info_list[index].map_id, test_map_info_list[index].map_id);

        /* Set test comment.  */
        gx_validation_set_frame_comment(test_map_info_list[index].id_name);
        
        /* Force a screen refresh. */
        gx_validation_screen_refresh();

        index++;
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
