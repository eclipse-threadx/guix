/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_utility_8bpp_pixelmap_rotate", /* Test name */
    31, 69, 613, 400     /* Define the coordinates of the capture area.
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
       on the line drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);

    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Call the actual line example routine. */
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
#include "demo_guix_all_widgets_8bpp.c"

GX_RESOURCE_ID rotate_pixelmap_id_list[]={
    GX_PIXELMAP_ID_ROTATE_APPLE,
    GX_PIXELMAP_ID_ROTATE_FISH
};

int angle_list[]={30, 90, -180, 270};

char test_comment[255];
extern GX_STUDIO_DISPLAY_INFO all_widgets_8bpp_display_table[];

#define TEST_MEMORY_SIZE 500
GX_COLOR       test_memory[TEST_MEMORY_SIZE];
GX_PIXELMAP   *rotated_map = GX_NULL;

VOID test_pixelmap_draw(GX_WINDOW *window)
{
INT  xpos;
INT  ypos;
GX_RECTANGLE win_size;

    gx_window_draw((GX_WINDOW*)window);

    win_size = window->gx_widget_size;

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
int          angle_index;
int          angle;
int          memory_index;
GX_CONST GX_THEME *theme_ptr;

    /* Save palette info. */
    theme_ptr = all_widgets_8bpp_display_table[MAIN_DISPLAY].theme_table[MAIN_DISPLAY_THEME_1];
    gx_validation_write_palette(theme_ptr -> theme_palette, theme_ptr->theme_palette_size);

    /* Toggle to rotate screen. */
    ToggleScreen((GX_WINDOW *)&rotate_screen, (GX_WINDOW *)&button_screen);

    gx_widget_draw_set(&rotate_screen.rotate_screen_pixelmap_window, test_pixelmap_draw);

    for(memory_index = 0; memory_index < 2; memory_index++)
    {
        if(memory_index)
        {
            /* Set memory size to 2000. */
            tx_byte_pool_delete(&mem_pool);
            tx_byte_pool_create(&mem_pool, "test_memory_pool", test_memory,
                        TEST_MEMORY_SIZE * sizeof(GX_COLOR));
        }

        for(pixelmap_index = 0; pixelmap_index < 2; pixelmap_index++)
        {
            map_id = rotate_pixelmap_id_list[pixelmap_index];
            gx_widget_pixelmap_get(&rotate_screen.rotate_screen_pixelmap_window, map_id, &pixelmap);

            for(angle_index = 0; angle_index < (int)(sizeof(angle_list)/sizeof(int)); angle_index++)
            {
                angle = angle_list[angle_index];
           
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

                    if(angle % 90)
                    {
                        if( gx_utility_pixelmap_rotate(pixelmap, angle, &destination, p_xcenter, p_ycenter) == GX_SUCCESS)
                        {
                            rotated_map = &destination;
                        }
                    }
                    else
                    {
                        if( gx_utility_pixelmap_simple_rotate(pixelmap, angle, &destination, p_xcenter, p_ycenter) == GX_SUCCESS)
                        {
                            rotated_map = &destination;
                        }
                    }

                    /* Capture a frame. */
                    gx_validation_set_frame_id(frame_id++);
                    sprintf(test_comment, "pixelmap_id = %d, angle = %d, specified x center: %d, specified y center: %d", map_id, angle, x_center, !x_center);
                    if(memory_index)
                    {
                        strcat(test_comment, ", memory pool size = 500 * 4(bytes)");
                    }
                    gx_validation_set_frame_comment(test_comment);
                    gx_validation_screen_refresh();
                }
            }
        }
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
