/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_line_1bpp", /* Test name */
   20, 20, 400, 460  /* Define the coordinates of the capture area.
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
#ifdef win32_graphics_driver_setup_monochrome  
#undef win32_graphics_driver_setup_monochrome  
#endif
#define win32_graphics_driver_setup_monochrome  gx_validation_graphics_driver_setup_monochrome


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_line_1bpp.c"

char test_comment[256];
extern INT line_width;
extern GX_WINDOW line_window;
extern INT line_angle;
extern INT line_pattern;
extern GX_BOOL is_round;

static VOID test_line_win_draw(GX_WIDGET *widget)
{
ULONG brush_style = 0;
GX_BRUSH *brush = GX_NULL;

    gx_window_draw((GX_WINDOW*)widget);

    if (is_round)
    {
        brush_style |= GX_BRUSH_ROUND;
    }

    gx_context_brush_width_set(line_width);
    gx_context_raw_brush_define(0xffffff, 0x0, brush_style);

    gx_context_brush_get(&brush);

    if (brush)
    {
        brush -> gx_brush_line_pattern = line_pattern;
        gx_context_brush_set(brush);

        /* calculate the line end points */
        gx_canvas_line_draw(line_x1, line_y1, line_x2, line_y2);
    }

}

static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
int pattern_index;
int round_end;
GX_RECTANGLE size;

    for(line_width = 1; line_width < 20 ; line_width += 6)
    {
       for(line_angle = 0; line_angle < 360; line_angle += 45)
       {
           for(round_end = 0; round_end <= 1; round_end ++)
           {
                is_round = round_end;
                for (pattern_index = 0; pattern_index <= 1; pattern_index++)
                {
                    if(pattern_index) line_pattern = 0x0f0f0f0f;
                    else              line_pattern = 0;
                    /* Inform the validation system 
                    (1) Frame ID, which identifies a specific test configuration;
                    (2) Start recording frame on the next toggle operation.
                    */
                    calculate_line_ends();

                    gx_system_dirty_mark(&line_window);
                    gx_validation_set_frame_id(frame_id);
                    sprintf(test_comment, "id: %d, line_width: %d, line_angle: %d, round_end: %d, line_pattern: %d", frame_id, line_width, line_angle, round_end, line_pattern);
                    gx_validation_set_frame_comment(test_comment);
                        /* Force a screen refresh. */
                    gx_validation_screen_refresh();

                        /* Increment frame ID */
                    frame_id ++;
               }
           }
       }
    }
    size.gx_rectangle_left = 20;
    size.gx_rectangle_top = 20;
    size.gx_rectangle_right = 260;
    size.gx_rectangle_bottom = 260;  
    gx_widget_resize(&line_window, &size);

    for(line_width = 1; line_width < 20 ; line_width += 6)
    {
       for(line_angle = 0; line_angle < 360; line_angle += 30)
       {
           for(round_end = 0; round_end <= 1; round_end ++)
           {
                is_round = round_end;
                line_pattern = 0;
	            /* Inform the validation system 
	            (1) Frame ID, which identifies a specific test configuration;
                (2) Start recording frame on the next toggle operation.
                */
                calculate_line_ends();

	            gx_system_dirty_mark(&line_window);
                gx_validation_set_frame_id(frame_id);
	            sprintf(test_comment, "id: %d, line_width: %d, line_angle: %d, round_end: %d ", frame_id, line_width, line_angle, round_end);
	            gx_validation_set_frame_comment(test_comment);
                    /* Force a screen refresh. */
	            gx_validation_screen_refresh();

                    /* Increment frame ID */
	            frame_id ++;
           }
       }
    }
    size.gx_rectangle_left = 20;
    size.gx_rectangle_top = 20;
    size.gx_rectangle_right = 179;
    size.gx_rectangle_bottom = 179;  
    gx_widget_resize(&line_window, &size);
    gx_context_raw_brush_define(0x0, 0xffffff, 0);

    for(line_width = 1; line_width < 15 ; line_width += 6)
    {
       for(line_angle = 0; line_angle < 360; line_angle += 30)
       {
           for(round_end = 0; round_end <= 1; round_end ++)
           {
                is_round = round_end;
                line_pattern = 0;
	            /* Inform the validation system 
	            (1) Frame ID, which identifies a specific test configuration;
                (2) Start recording frame on the next toggle operation.
                */
                calculate_line_ends();

	            gx_system_dirty_mark(&line_window);
                gx_validation_set_frame_id(frame_id);
	            sprintf(test_comment, "id: %d, line_width: %d, line_angle: %d, round_end: %d ", frame_id, line_width, line_angle, round_end);
	            gx_validation_set_frame_comment(test_comment);
                    /* Force a screen refresh. */
	            gx_validation_screen_refresh();

                    /* Increment frame ID */
	            frame_id ++;
           }
       }
    }
    
    /******************************************************/ 
    /* set window color to black and line color to white.
       then do operation as above again. */
    gx_widget_fill_color_set((GX_WIDGET *)&line_window, GX_1BPP_BLACK, GX_1BPP_BLACK, GX_1BPP_BLACK);
    line_color = GX_1BPP_WHITE;
    size.gx_rectangle_left = 20;
    size.gx_rectangle_top = 20;
    size.gx_rectangle_right = 400;
    size.gx_rectangle_bottom = 460;  
    gx_widget_resize(&line_window, &size);
    gx_context_raw_brush_define(0xffffff, 0, 0);
    /* Reset line wind draw function because line color is hard-coded in original draw function in demo file,
       Rewrite this draw function to change line color. */
    gx_widget_draw_set((GX_WIDGET *)&line_window, test_line_win_draw);
    
    for(line_width = 1; line_width < 20 ; line_width += 6)
    {
       for(line_angle = 0; line_angle < 360; line_angle += 45)
       {
           for(round_end = 0; round_end <= 1; round_end ++)
           {
                is_round = round_end;
                for (pattern_index = 0; pattern_index <= 1; pattern_index++)
                {
                    if(pattern_index) line_pattern = 0x0f0f0f0f;
                    else              line_pattern = 0;
                    /* Inform the validation system 
                    (1) Frame ID, which identifies a specific test configuration;
                    (2) Start recording frame on the next toggle operation.
                    */
                    calculate_line_ends();

                    gx_system_dirty_mark(&line_window);
                    gx_validation_set_frame_id(frame_id);
                    sprintf(test_comment, "id: %d, line_width: %d, line_angle: %d, round_end: %d, line_pattern: %d", frame_id, line_width, line_angle, round_end, line_pattern);
                    gx_validation_set_frame_comment(test_comment);
                        /* Force a screen refresh. */
                    gx_validation_screen_refresh();

                        /* Increment frame ID */
                    frame_id ++;
               }
           }
       }
    }
    size.gx_rectangle_left = 20;
    size.gx_rectangle_top = 20;
    size.gx_rectangle_right = 260;
    size.gx_rectangle_bottom = 260;  
    gx_widget_resize(&line_window, &size);
    gx_context_raw_brush_define(0xffffff, 0, 0);
    for(line_width = 1; line_width < 20 ; line_width += 6)
    {
       for(line_angle = 0; line_angle < 360; line_angle += 30)
       {
           for(round_end = 0; round_end <= 1; round_end ++)
           {
                is_round = round_end;
                line_pattern = 0;
	            /* Inform the validation system 
	            (1) Frame ID, which identifies a specific test configuration;
                (2) Start recording frame on the next toggle operation.
                */
                calculate_line_ends();

	            gx_system_dirty_mark(&line_window);
                gx_validation_set_frame_id(frame_id);
	            sprintf(test_comment, "id: %d, line_width: %d, line_angle: %d, round_end: %d ", frame_id, line_width, line_angle, round_end);
	            gx_validation_set_frame_comment(test_comment);
                    /* Force a screen refresh. */
	            gx_validation_screen_refresh();

                    /* Increment frame ID */
	            frame_id ++;
           }
       }
    }
    size.gx_rectangle_left = 20;
    size.gx_rectangle_top = 20;
    size.gx_rectangle_right = 179;
    size.gx_rectangle_bottom = 179;  
    gx_widget_resize(&line_window, &size);
    gx_context_raw_brush_define(0xffffff, 0, 0);

    for(line_width = 1; line_width < 15 ; line_width += 6)
    {
       for(line_angle = 0; line_angle < 360; line_angle += 30)
       {
           for(round_end = 0; round_end <= 1; round_end ++)
           {
                is_round = round_end;
                line_pattern = 0;
	            /* Inform the validation system 
	            (1) Frame ID, which identifies a specific test configuration;
                (2) Start recording frame on the next toggle operation.
                */
                calculate_line_ends();

	            gx_system_dirty_mark(&line_window);
                gx_validation_set_frame_id(frame_id);
	            sprintf(test_comment, "id: %d, line_width: %d, line_angle: %d, round_end: %d ", frame_id, line_width, line_angle, round_end);
	            gx_validation_set_frame_comment(test_comment);
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









