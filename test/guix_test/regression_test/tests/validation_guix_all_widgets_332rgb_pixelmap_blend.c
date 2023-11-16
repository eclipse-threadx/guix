/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_332rgb_pixelmap_blend", /* Test name */
    0, 0, 100, 480  /* Define the coordinates of the capture area.
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
#ifdef win32_graphics_driver_setup_332rgb  
#undef win32_graphics_driver_setup_332rgb  
#endif
#define win32_graphics_driver_setup_332rgb  gx_validation_graphics_driver_setup_332rgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_all_widgets_332rgb.c"

char test_comment[256];
extern GX_RESOURCE_ID     map_id;
GX_RESOURCE_ID maps[] = {GX_PIXELMAP_ID_FISH, GX_PIXELMAP_ID_ROTATE_APPLE, GX_PIXELMAP_ID_FOOT, GX_PIXELMAP_ID_APPLE }; //raw, alpha, compress, compressed-alpha
static int alpha_index;

static GX_PIXELMAP test_map =
{
    0x00000001,                              /* major version                  */
    0x00000000,                              /* minor version                  */
    0,                                       /* flags                 */
    GX_COLOR_FORMAT_32BGRA,                  /* Format                       */
    0,
    0,                                       /* the size of pixelmap_data*/
    NULL,
    0,                                       /* auxiliary data size            */
    0x00,                                    /* used for transparent iamges    */
    1,                                       /* width in pixel                 */
    1                                        /* height in pixel                */
};

/* pixelmap with invalid format */
static GX_COLOR invalid_format_map_data[4] = {0};

static GX_PIXELMAP test_invalid_format_map =
{
    0x00000001,                              /* major version                  */
    0x00000000,                              /* minor version                  */
    0,                                       /* flags                          */
    GX_COLOR_FORMAT_32BGRA,                  /* Format                         */
    (GX_UBYTE *) invalid_format_map_data,
    sizeof(invalid_format_map_data),         /* the size of pixelmap_data      */
    NULL,
    0,                                       /* auxiliary data size            */
    0x00,                                    /* used for transparent iamges    */
    2,                                       /* width in pixel                 */
    2                                        /* height in pixel                */
};

/* pixelmap with invalid style. */


void window_draw_func(GX_WINDOW *win)
{
    GX_BRUSH *brush;
    
    gx_context_brush_get(&brush);
    brush -> gx_brush_alpha = alpha_index;
    gx_window_draw(win);
    

    /* Test map with no map data and invalid size.*/
    gx_canvas_pixelmap_draw(win->gx_widget_size.gx_rectangle_left + 10, win->gx_widget_size.gx_rectangle_top + 10, &test_map);
    
    /* Test map with invalid format. */
    gx_canvas_pixelmap_draw(win->gx_widget_size.gx_rectangle_left + 10, win->gx_widget_size.gx_rectangle_top + 10, &test_invalid_format_map);
    
    
}

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
int has_alpha, compressed;
GX_WINDOW *apple_window;
GX_RECTANGLE size;

    gx_widget_style_remove(&((SPRITE_SCREEN_CONTROL_BLOCK *)pSpriteScreen)->sprite_screen_sprite, GX_STYLE_SPRITE_AUTO);
    apple_window = &sprite_screen.sprite_screen_apple_window;
    gx_widget_draw_set((GX_WIDGET *)apple_window, (void(*)(GX_WIDGET *))window_draw_func);
    size.gx_rectangle_left = pSpriteScreen->gx_widget_size.gx_rectangle_left - 10;
    size.gx_rectangle_right = pSpriteScreen->gx_widget_size.gx_rectangle_left + 100;
    size.gx_rectangle_top = pSpriteScreen->gx_widget_size.gx_rectangle_top - 10;
    size.gx_rectangle_bottom = pSpriteScreen->gx_widget_size.gx_rectangle_bottom;
    
    gx_widget_resize((GX_WIDGET *)apple_window, &size);
    
    ToggleScreen(pSpriteScreen, pShapesScreen);
    
    for(has_alpha = 0; has_alpha <= 1; has_alpha++)
    {
        for(compressed = 0; compressed <= 2; compressed+=2)
        {
            gx_window_wallpaper_set(apple_window, maps[compressed + has_alpha], GX_TRUE);

            for(alpha_index = 0; alpha_index <= 255; alpha_index+=51)
            {
                /* Inform the validation system 
                (1) Frame ID, which identifies a specific test configuration;
                (2) Start recording frame on the next toggle operation.
                */
                gx_validation_set_frame_id(frame_id);
                sprintf(test_comment, "alpha value %d, pixelmap with alpha-channel: %d, pixelmap compressed: %d ", alpha_index, has_alpha, compressed);
                                
                gx_validation_set_frame_comment(test_comment);

                /* Mark the window "dirty" */
                gx_system_dirty_mark(apple_window);

                /* Force a screen refresh. */
                gx_validation_screen_refresh();

                /* Increment frame ID */
                frame_id ++;
            
            }
        }
    }
    
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





