/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_canvas_rotated_text_draw", /* Test name */
    73, 7, 572, 475  /* Define the coordinates of the capture area.
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
#ifdef win32_graphics_driver_setup_24xrgb  
#undef win32_graphics_driver_setup_24xrgb  
#endif
#define win32_graphics_driver_setup_24xrgb  gx_validation_graphics_driver_setup_24xrgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"
#include "demo_guix_all_widgets.c"

char                     test_comment[255];
int                      frame_id = 1;
extern GX_CONST GX_FONT *Primary_theme_1_font_table[];

#define TEST_MEMORY_SIZE 5000
GX_UBYTE test_memory[TEST_MEMORY_SIZE];

static void button_screen_draw_test(GX_WINDOW * window)
{
UINT       status = 0;
int xpos, ypos;

    xpos = window->gx_widget_size.gx_rectangle_left + 10;
    ypos = window->gx_widget_size.gx_rectangle_top + 10;
    
    gx_window_draw(window);
    
    switch(frame_id)
    {
    case 1:
        /* Do nothing to make it return GX_INVALID_FONT. */ 
        break;
        
    case 2:
    case 3:
        /*Set font to draw rotated text. */
        gx_context_font_set(GX_FONT_ID_NORMAL_FONT);
        gx_context_fill_color_set(GX_COLOR_ID_ORANGE);
        break;
    }
    
    status = gx_canvas_rotated_text_draw( "test_string", xpos, ypos, 30);
    
    if((frame_id == 1) && (status != GX_FAILURE))
    {
        printf("Guix Test:   guix_canvas_rotated_text_draw....................................................Failed!\n");
        exit(1);
    }
    
    if(selected_semaphore)
    {
        tx_semaphore_put(selected_semaphore);
    }
}

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
static TX_SEMAPHORE semaphore_memory;
UINT   status;
    selected_semaphore = &semaphore_memory;
    tx_semaphore_create(selected_semaphore, "test_semaphore", 0);

    status = gx_canvas_rotated_text_draw("test text", 0, 0, 10);
    if(status != GX_FAILURE)
    {
        printf("Guix Test:   guix_canvas_rotated_text_draw....................................................Failed!\n");
        exit(1);
    }
    
    gx_widget_draw_set(pButtonScreen, button_screen_draw_test);
    for(frame_id = 1; frame_id < 4; frame_id++)
    {
        switch(frame_id)
        {
            case 1:
                sprintf(test_comment, "Draw text without set font");
                break;
            
            case 2:
                sprintf(test_comment, "Draw rotated text with normal font.");
                break;

            case 3:
                tx_byte_pool_delete(&rotate_pool);
                tx_byte_pool_create(&rotate_pool, "test_memory_pool", test_memory, TEST_MEMORY_SIZE);
                sprintf(test_comment, "Set memory size = 5000(bytes)");
                break;
        }
        gx_validation_set_frame_id(frame_id);
        gx_validation_set_frame_comment(test_comment);
        gx_system_dirty_mark(pButtonScreen);
        gx_validation_screen_refresh();
        
        tx_semaphore_get(selected_semaphore, TX_WAIT_FOREVER);
    }
    
    tx_semaphore_delete(selected_semaphore);
    selected_semaphore = GX_NULL;
    
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
