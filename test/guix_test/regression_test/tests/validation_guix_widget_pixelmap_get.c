/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_widget_pixelmap_get", /* Test name */
    41, 31, 597, 471  /* Define the coordinates of the capture area.
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
    /* Call the actual line example routine. */
    gx_validation_application_define(first_unused_memory);

    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Create a dedicated thread to perform various operations
       on the line drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);

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

static GX_RESOURCE_ID blend_map = GX_PIXELMAP_ID_ROTATE_FOOT;
VOID sprite_screen_draw(GX_WINDOW *window)
{
    GX_PIXELMAP *map;

    gx_window_draw(window);
    
    gx_widget_pixelmap_get(window, blend_map, &map);
    if(map)
    {
        gx_canvas_pixelmap_blend(window->gx_widget_size.gx_rectangle_left, window->gx_widget_size.gx_rectangle_top, map, 200);
    }
}

static GX_CHAR test_comment[256];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int                        frame_id = 1;
GX_RECTANGLE size;
GX_PIXELMAP *map;
UINT        status;

    gx_widget_style_remove(&((SPRITE_SCREEN_CONTROL_BLOCK *)pSpriteScreen)->sprite_screen_sprite_1, GX_STYLE_SPRITE_AUTO);
    gx_widget_draw_set(pSpriteScreen, (void(*)(GX_WIDGET *))sprite_screen_draw);

    size = pButtonScreen->gx_widget_size;
    size.gx_rectangle_right = 400;
    size.gx_rectangle_bottom = 400;
    
    size = pSpriteScreen->gx_widget_size;
    size.gx_rectangle_left = 300;
    size.gx_rectangle_top = 200;
    gx_widget_resize(pSpriteScreen, &size);
    gx_widget_attach(root, pSpriteScreen);   
    gx_widget_back_move(pSpriteScreen, GX_NULL);
    /* blend pixelmap in different position. */

    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "Change sprite screen draw function and attach sprite screen to root window.");
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();
    
    blend_map = PRIMARY_PIXELMAP_TABLE_SIZE + 1;
    
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "Call function gx_widget_pixelmap_get with invalid pixelmap id.");
    gx_validation_set_frame_comment(test_comment);
    gx_system_dirty_mark(pSpriteScreen);
    gx_validation_screen_refresh();  
    
    gx_widget_hide(pSpriteScreen);
    status = gx_widget_pixelmap_get(pSpriteScreen, blend_map, &map);
    if((status != GX_INVALID_CANVAS) || (map != GX_NULL))
    {
        printf("Guix Test:   guix_canvas_pixelmap_blend.......................................................Failed!\n");
        exit(1);
    }
    
    text_screen.gx_widget_status |= GX_STATUS_VISIBLE;
    /* Call pixelmap get function here, which condition has no canvas and no context.
       And GX_INVALID_CANVAS will be returned. */
    status = gx_widget_pixelmap_get((GX_WIDGET *)&text_screen, blend_map, &map);
    if((status != GX_INVALID_CANVAS) || (map != GX_NULL))
    {
        printf("Guix Test:   guix_canvas_pixelmap_blend.......................................................Failed!\n");
        exit(1);
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
