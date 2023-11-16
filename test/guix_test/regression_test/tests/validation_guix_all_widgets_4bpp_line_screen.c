/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_4bpp_line_screen", /* Test name */
    39, 71, 359, 391  /* Define the coordinates of the capture area.
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

/*NOTICE: Number of xpos_value and ypos_valus should be same.*/
extern ULONG palette_4bpp_grayscale[16];
char test_comment[256];
extern int line_width;
extern int line_angle;
extern GX_BOOL rounded;
extern INT line_pattern;
extern VOID calculate_line_ends(VOID);
extern GX_WINDOW *pLineWin;

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
int pattern_index;
int round_end;
GX_RECTANGLE size;
    gx_validation_write_palette((ULONG *)palette_4bpp_grayscale, 16);
    ToggleScreen(pLineScreen, pSpriteScreen);
    for(line_width = 1; line_width < 20 ; line_width += 6)
    {
       for(line_angle = 0; line_angle < 360; line_angle += 45)
       {
           for(round_end = 0; round_end <= 1; round_end ++)
           {
                rounded = round_end;
                for (pattern_index = 0; pattern_index <= 1; pattern_index++)
                {
                    if(pattern_index) line_pattern = 0x0f0f0f0f;
                    else              line_pattern = 0;
                    /* Inform the validation system 
                    (1) Frame ID, which identifies a specific test configuration;
                        (2) Start recording frame on the next toggle operation.
                        */
                        calculate_line_ends();

                    gx_system_dirty_mark(pLineWin);
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

    size.gx_rectangle_left = 39;
    size.gx_rectangle_top = 71;
    size.gx_rectangle_right = 39 + 149;
    size.gx_rectangle_bottom = 71 + 149;
    gx_widget_resize(pLineWin, &size); 
    for(line_width = 1; line_width < 20 ; line_width += 6)
    {
       for(line_angle = 0; line_angle < 360; line_angle += 30)
       {
           for(round_end = 0; round_end <= 1; round_end ++)
           {
                rounded = round_end;
                line_pattern = 0;
	            /* Inform the validation system 
	            (1) Frame ID, which identifies a specific test configuration;
                    (2) Start recording frame on the next toggle operation.
                    */
                calculate_line_ends();

	            gx_system_dirty_mark(pLineWin);
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

    size.gx_rectangle_left = 39;
    size.gx_rectangle_top = 71;
    size.gx_rectangle_right = 39 + 160;
    size.gx_rectangle_bottom = 71 + 160;
    gx_widget_resize(pLineWin, &size);

    for(line_width = 1; line_width < 15 ; line_width += 6)
    {
       for(line_angle = 0; line_angle < 360; line_angle += 30)
       {
           for(round_end = 0; round_end <= 1; round_end ++)
           {
                rounded = round_end;
                line_pattern = 0;
	            /* Inform the validation system 
	            (1) Frame ID, which identifies a specific test configuration;
                    (2) Start recording frame on the next toggle operation.
                    */
                calculate_line_ends();

	            gx_system_dirty_mark(pLineWin);
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





