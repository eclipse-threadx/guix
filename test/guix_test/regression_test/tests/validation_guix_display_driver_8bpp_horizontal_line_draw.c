/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_display_driver_8bpp_horizontal_line_draw", /* Test name */
    25, 17, 344, 336  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the line
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

#include "demo_guix_line_8bpp.c"
static INT brush_alpha = 255;
static VOID test_line_wind_draw(GX_WINDOW * window)
{
GX_BRUSH *brush;
    gx_window_draw((GX_WINDOW*) window);
    
    gx_context_brush_get(&brush);
    brush -> gx_brush_alpha = brush_alpha;
    gx_context_brush_width_set(line_width);

    calculate_line_ends();
    gx_canvas_line_draw(line_x1, line_y1, line_x2, line_y2);
}

/* Define the width values for the line.*/
static int width_value[] = {1, 3, 10};

char test_comment[256];
extern GX_STUDIO_DISPLAY_INFO lines_8bpp_display_table[];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{

int  width_index;
int frame_id = 1;
GX_CONST GX_THEME *theme_ptr;

    /*Save palette. */
    theme_ptr = lines_8bpp_display_table[DISPLAY_1].theme_table[0];
    gx_validation_write_palette(theme_ptr -> theme_palette, theme_ptr->theme_palette_size);
               
    /* Set the line angle value */
    line_angle = 0;
    
    for(width_index = 0; width_index < (INT)(sizeof(width_value) / sizeof(int)); width_index++)
    {
        line_width = width_value[width_index];
                                 
        /* Inform the validation system 
        (1) Frame ID, which identifies a specific test configuration;
        (2) Start recording frame on the next toggle operation.
        */
        gx_validation_set_frame_id(frame_id);

                                 
        sprintf(test_comment, "line width %d ", line_width);
          
        gx_validation_set_frame_comment(test_comment);
          
        /* Mark the window "dirty" */
        gx_system_dirty_mark(pLineWin);
          
        /* Force a screen refresh. */
        gx_validation_screen_refresh();

        /* Increment frame ID */
        frame_id ++;
    }
    
    brush_alpha = 100;
    gx_widget_draw_set((GX_WIDGET *)&main_window.main_window_line_window, (VOID (*)(GX_WIDGET *))test_line_wind_draw);
    
    for(width_index = 0; width_index < (INT)(sizeof(width_value) / sizeof(int)); width_index++)
    {
        line_width = width_value[width_index];
                                 
        /* Inform the validation system 
        (1) Frame ID, which identifies a specific test configuration;
        (2) Start recording frame on the next toggle operation.
        */
        gx_validation_set_frame_id(frame_id);

                                 
        sprintf(test_comment, "Line width %d, but set brush alpha to 100 here, so no line should be drawn here.", line_width);
          
        gx_validation_set_frame_comment(test_comment);
          
        /* Mark the window "dirty" */
        gx_system_dirty_mark(pLineWin);
          
        /* Force a screen refresh. */
        gx_validation_screen_refresh();

        /* Increment frame ID */
        frame_id ++;
    }
    
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





