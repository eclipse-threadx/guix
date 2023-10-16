/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_utility_1555xrgb_pixelmap_rotate", /* Test name */
    30, 68, 615, 402  /* Define the coordinates of the capture area.
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

int rotate_pixelmap_id_list[]={
    GX_PIXELMAP_ID_ROTATE_FOOT,
    GX_PIXELMAP_ID_ROTATE_FISH
};

char test_comment[256];

GX_PIXELMAP *rotated_map = GX_NULL;
#define TEST_MEMORY_SIZE_20000 20000
#define TEST_MEMORY_SIZE_38000 38000
   
GX_COLOR       test_memory_20000[TEST_MEMORY_SIZE_20000];
GX_COLOR       test_memory_38000[TEST_MEMORY_SIZE_38000];

VOID test_pixelmap_draw(GX_WINDOW *window)
{
INT  xpos;
INT  ypos;
GX_RECTANGLE win_size;

    gx_window_draw((GX_WINDOW*)window);

    win_size = pRotateWin->gx_widget_size;

    xpos = win_size.gx_rectangle_left;
    ypos = win_size.gx_rectangle_top;

    if (rotated_map)
    {
        /* Draw rotated pixelmap.  */
        gx_canvas_pixelmap_draw(xpos, ypos, rotated_map);
        
        /* free memory of the rotated pixelmap */
        if (rotated_map->gx_pixelmap_data)
        {
            memory_free((VOID *)rotated_map->gx_pixelmap_data);
        }
        if (rotated_map->gx_pixelmap_aux_data)
        {
            memory_free((VOID *)rotated_map->gx_pixelmap_aux_data);
        }
        rotated_map = GX_NULL;
    }
}
/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int          frame_id = 1;
int          pixelmap_index;
GX_PIXELMAP  destination;
GX_PIXELMAP *pixelmap;
int          map_id;
int          x_center;
int          int_memory;
int         *p_xcenter;
int         *p_ycenter;
int          index;

    gx_widget_draw_set(&rotate_screen.rotate_screen_pixelmap_window, test_pixelmap_draw);

    for(index = 0; index < 2; index++)
    {
        if(index)
        {
            /* Set memory size to 20000. */
            tx_byte_pool_delete(&rotate_pool);
            tx_byte_pool_create(&rotate_pool, "test_memory_pool_20000", test_memory_20000,
                        TEST_MEMORY_SIZE_20000 * sizeof(GX_COLOR));
        }
        else
        {
            /* Set memory size to 38000. */
            tx_byte_pool_delete(&rotate_pool);
            tx_byte_pool_create(&rotate_pool, "test_memory_pool_38000", test_memory_38000,
                        TEST_MEMORY_SIZE_38000 * sizeof(GX_COLOR));
           
        }

        for(pixelmap_index = 0; pixelmap_index < 2; pixelmap_index++)
        {
            map_id = rotate_pixelmap_id_list[pixelmap_index];
            gx_widget_pixelmap_get(&rotate_screen.rotate_screen_pixelmap_window, map_id, &pixelmap);

            for(x_center = 0; x_center < 2; x_center++)
            {
                if(x_center)
                {
                    p_xcenter = &int_memory;
                    p_ycenter = GX_NULL;
                }          
                else
                {
                    p_xcenter = GX_NULL;
                    p_ycenter = &int_memory;
                }

                if( gx_utility_pixelmap_rotate(pixelmap, 30, &destination, p_xcenter, p_ycenter) == GX_SUCCESS)
                {
                    rotated_map = &destination;
                }

                /* Capture a frame. */
                gx_validation_set_frame_id(frame_id++);
                sprintf(test_comment, "pixelmap_id = %d, specified x center: %d, specified y center: %d", map_id, x_center, !x_center);
                if(index)
                {
                    strcat(test_comment, ", memory pool size = 20000 * 4(bytes)");
                }
                else
                {
                    strcat(test_comment, ", memory pool size = 38000 * 4(bytes)");
                }
                gx_validation_set_frame_comment(test_comment);
                gx_validation_screen_refresh();
            }
        }
    }
 
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
