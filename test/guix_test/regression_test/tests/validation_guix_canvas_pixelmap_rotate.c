/* This is a small demo of the high-performance GUIX graphics framework. */


#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_canvas_pixelmap_rotate", /* Test name */
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
#ifdef win32_graphics_driver_setup_565rgb
#undef win32_graphics_driver_setup_565rgb
#endif
#define win32_graphics_driver_setup_565rgb  gx_validation_graphics_driver_setup_565rgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets_16bpp.c"

char test_comment[256];
int frame_id = 1;
static GX_RESOURCE_ID pixelmap_id = GX_PIXELMAP_ID_ID_RAW;
TX_SEMAPHORE *selected_semaphore;
static VOID test_pixelmap_draw(GX_WINDOW *window)
{
GX_PIXELMAP *pixelmap;
INT  xpos;
INT  ypos;
INT  angle;
UINT status;
GX_RECTANGLE win_size;
GX_DISPLAY *display;
VOID             (*rotate)(GX_DRAW_CONTEXT *, INT, INT, GX_PIXELMAP *, INT, INT, INT);
VOID             (*draw)(GX_DRAW_CONTEXT *,INT , INT , GX_PIXELMAP *);
    display = root->gx_window_root_canvas->gx_canvas_display;
    gx_window_draw((GX_WINDOW*)window);
    
    gx_widget_pixelmap_get(window, pixelmap_id, &pixelmap);
    win_size = pRotateWin->gx_widget_size;

    xpos = (win_size.gx_rectangle_right + win_size.gx_rectangle_left - pixelmap->gx_pixelmap_width) >> 1;
    ypos = (win_size.gx_rectangle_top + win_size.gx_rectangle_bottom - pixelmap->gx_pixelmap_height) >> 1;    
    if(pixelmap)
    {
        switch(frame_id)
        {
            case 5:
            case 1:
            /* do nothing to make it draw the original roatet map.*/
                angle = 30;
                gx_canvas_pixelmap_rotate(xpos, ypos, pixelmap, angle, -1, -1);
                break;
            
            case 2:
                /* Try to rotate compress pixelmap. */
                status = gx_canvas_pixelmap_rotate(xpos, ypos, pixelmap, angle, -1, -1);
                if(status != GX_NOT_SUPPORTED) 
                {
                    printf("Guix Test:   guix_canvas_rotated_text_draw....................................................Failed!\n");
                    exit(1);
                }
                break;
                
            case 3:
                angle = 0;
                draw = display->gx_display_driver_pixelmap_draw;
                display->gx_display_driver_pixelmap_draw = GX_NULL;
                
                status = gx_canvas_pixelmap_rotate(xpos, ypos, pixelmap, angle, -1, -1);
                if(status != GX_FAILURE) 
                {
                    printf("Guix Test:   guix_canvas_rotated_text_draw....................................................Failed!\n");
                    exit(1);
                }          
                display->gx_display_driver_pixelmap_draw  = draw;                
                break;
                
            case 4:
                angle = 30;
                rotate = display->gx_display_driver_pixelmap_rotate;
                display->gx_display_driver_pixelmap_rotate = GX_NULL;
                status = gx_canvas_pixelmap_rotate(xpos, ypos, pixelmap, angle, -1, -1);
                if(status != GX_FAILURE) 
                {
                    printf("Guix Test:   guix_canvas_rotated_text_draw....................................................Failed!\n");
                    exit(1);
                }  
                display->gx_display_driver_pixelmap_rotate = rotate;
                break;
                
        }
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
static GX_WINDOW cover_window;
GX_RECTANGLE size;
    selected_semaphore = &semaphore_memory;
    tx_semaphore_create(selected_semaphore, "test_semaphore", 0);
    
    size = pRotateScreen->gx_widget_size;
    size.gx_rectangle_left += 10;
    size.gx_rectangle_top += 10;
    size.gx_rectangle_right -= 10;
    size.gx_rectangle_bottom -= 10;
    gx_widget_resize(pRotateScreen, &size);
    
    gx_widget_draw_set(&rotate_screen.rotate_screen_pixelmap_window, (VOID (*)(GX_WIDGET *))test_pixelmap_draw);
    gx_widget_detach(pButtonScreen);
    gx_widget_attach(root, pRotateScreen);
    
    /* Inform the validation system 
    (1) Frame ID, which identifies a specific test configuration;
    (2) Start recording frame on the next toggle operation.
    */
    while(frame_id <= 5)
    {  
        pixelmap_id = GX_PIXELMAP_ID_ID_RAW;
        gx_validation_set_frame_id(frame_id);
        switch(frame_id)
        {
            case 1:
                sprintf(test_comment, "Just draw rotate raw pixelmap.");
            break;
            case 2:
                pixelmap_id = GX_PIXELMAP_ID_ID_COMPRESS;
                sprintf(test_comment, "Try to rotate compressed map, So no rotated map should be drawn.");
            break;
            case 3:
                sprintf(test_comment, "Set display pixelmap draw function to NULL, set rotate angle to 0, then call gx_canvas_pixelmap_rotate.");
            break;
            case 4:
                sprintf(test_comment, "Set display pixelmap rotate function to NULL, set rotate angle to 30, then call gx_canvas_pixelmap_rotate.");
            break;
            case 5:
                memset(&cover_window, 0, sizeof(GX_WINDOW));
                size = pRotateScreen->gx_widget_size;
                size.gx_rectangle_left += 50;
                size.gx_rectangle_top += 50;                
                gx_window_create(&cover_window, "cover_window", 0, 0, 0, &size);
                gx_widget_attach(root, &cover_window);
                sprintf(test_comment, "Create window to cover the rotate map.");
            break;
        }
        gx_validation_set_frame_comment(test_comment);

        gx_system_dirty_mark(pRotateScreen);

        /* Force a screen refresh. */
        gx_validation_screen_refresh();
        
        tx_semaphore_get(selected_semaphore, TX_WAIT_FOREVER);
        frame_id++;
    }
    
    tx_semaphore_delete(selected_semaphore);
    selected_semaphore = GX_NULL;
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





