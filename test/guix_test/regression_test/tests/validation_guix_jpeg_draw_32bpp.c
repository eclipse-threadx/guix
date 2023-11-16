/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_jpeg_draw_32bpp", /* Test name */
   0 , 0, 800, 480  /* Define the coordinates of the capture area.
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
#include "demo_guix_jpeg_draw_32bpp.c"

#define TEST_MEMORY_SIZE 100

GX_UBYTE test_memory[TEST_MEMORY_SIZE];
char     test_comment[255];

typedef struct MAP_INFO_STRUCT{
    GX_RESOURCE_ID map_id;
    char id_name[30];
}MAP_INFO;

static MAP_INFO test_map_info_list[] ={
    {GX_PIXELMAP_ID_RED_APPLE, "GX_PIXELMAP_ID_RED_APPLE"},
    {GX_PIXELMAP_ID_FISH_SMALL_411, "GX_PIXELMAP_ID_FISH_SMALL_411"},
    {GX_PIXELMAP_ID_FISH_SMALL_420, "GX_PIXELMAP_ID_FISH_SMALL_420"},
    {GX_PIXELMAP_ID_FISH_SMALL_422, "GX_PIXELMAP_ID_FISH_SMALL_422"},
    {GX_PIXELMAP_ID_FISH_SMALL_444, "GX_PIXELMAP_ID_FISH_SMALL_444"},
    {GX_NULL, ""}
};

VOID test_pic_win_draw(GX_WINDOW *window)
{
int xpos;
int ypos;
GX_PIXELMAP *map;

    xpos = window->gx_window_client.gx_rectangle_left - 10;
    ypos = window->gx_window_client.gx_rectangle_top - 10;

    gx_window_draw(window);

    gx_widget_pixelmap_get((GX_WIDGET *)window, window->gx_window_wallpaper, &map);
    gx_canvas_pixelmap_draw(xpos, ypos, map);
}
/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;   
int index = 0;

    gx_widget_draw_set((GX_WIDGET *)&main_window.main_window_pic_window, test_pic_win_draw);

    while(test_map_info_list[index].map_id != GX_NULL)
    {
        gx_validation_set_frame_id(frame_id++);
    
        /* Set the wallpaper of the window.  */
        gx_window_wallpaper_set(&main_window.main_window_pic_window, test_map_info_list[index].map_id, test_map_info_list[index].map_id);

        /* Set test comment.  */
        gx_validation_set_frame_comment(test_map_info_list[index].id_name);
        
        /* Force a screen refresh. */
        gx_validation_screen_refresh();

        index++;
    }

    for(index = 0; index < 3; index++)
    { 
        switch(index)
        {
        case 0:
            gx_system_memory_allocator_set(GX_NULL, decode_memory_free);
            sprintf(test_comment, "set memory allocate function to GX_NULL");
            break;

        case 1:
            gx_system_memory_allocator_set(decode_memory_allocate, GX_NULL);
            sprintf(test_comment, "set memory free function to GX_NULL");
            break;

        case 2:
            tx_byte_pool_delete(&decode_pool);
            tx_byte_pool_create(&decode_pool, "test_memory", test_memory, TEST_MEMORY_SIZE);
            gx_system_memory_allocator_set(decode_memory_allocate, decode_memory_free);
            sprintf(test_comment, "resize memory pool to not enought for decode");
            break;
        }

        gx_validation_set_frame_id(frame_id++);
   
        gx_validation_set_frame_comment(test_comment);
        
        /* Force a screen refresh. */
        gx_validation_screen_refresh();
    }
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





