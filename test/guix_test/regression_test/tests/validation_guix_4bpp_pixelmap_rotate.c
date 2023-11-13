/* This is a small demo of the high-performance GUIX graphics framework. */


#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_4bpp_pixelmap_rotate", /* Test name */
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
#ifdef win32_graphics_driver_setup_4bpp_grayscale
#undef win32_graphics_driver_setup_4bpp_grayscale  
#endif
#define win32_graphics_driver_setup_4bpp_grayscale  gx_validation_graphics_driver_setup_4bpp_grayscale


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets_4bpp.c"

static int xpos = 230;
static int map_id = 0;
static int angle = 90;
static GX_BOOL test_invalid_map = GX_FALSE;

VOID test_pixelmap_draw(GX_WINDOW *window)
{
GX_PIXELMAP *pixelmap;
GX_PIXELMAP invalid_map;

    gx_window_draw((GX_WINDOW*)window);

    if(test_invalid_map)
    {
        memset(&invalid_map, 0, sizeof(GX_PIXELMAP));
        invalid_map.gx_pixelmap_width = 100;
        invalid_map.gx_pixelmap_height = 100;
        invalid_map.gx_pixelmap_flags = GX_PIXELMAP_TRANSPARENT;
        pixelmap = &invalid_map;
    }
    else
    {    
        gx_context_pixelmap_get(map_id, &pixelmap);
    }

    gx_canvas_pixelmap_rotate(xpos, 161, pixelmap, angle, -1, -1);
}

/*NOTICE: Number of xpos_value and ypos_valus should be same.*/

GX_RESOURCE_ID pixelmap_id_list[]={
    GX_PIXELMAP_ID_RED_APPLE,
    GX_PIXELMAP_ID_ROTATE_FISH
};

char test_comment[256];
/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
int index;

    ToggleScreen(pRotateScreen, pSpriteScreen);

    gx_widget_draw_set((GX_WIDGET *)&rotate_screen.rotate_screen_pixelmap_window, test_pixelmap_draw);

    for(index = 0; index < (int)(sizeof(pixelmap_id_list)/sizeof(GX_RESOURCE_ID)); index++)
    {
       map_id = pixelmap_id_list[index];
       for(xpos = 229; xpos <= 230; xpos++)
       {
          for(angle = 90; angle <= 270; angle += 90)
          {
             sprintf(test_comment, "map_id = %d, xpos = %d, angle = %d", map_id, xpos, angle);
             gx_validation_set_frame_id(frame_id++);
             gx_validation_set_frame_comment(test_comment);
             gx_validation_screen_refresh();
          }
       }
    }

    test_invalid_map = GX_TRUE;
    angle = 90;
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("set invalid rotate pixelmap");
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
