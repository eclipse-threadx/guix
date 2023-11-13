/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_canvas_hardware_layer", /* Test name */
    24, 10, 579, 465  /* Define the coordinates of the capture area.
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
#ifdef win32_graphics_driver_setup_24xrgb  
#undef win32_graphics_driver_setup_24xrgb  
#endif
#define win32_graphics_driver_setup_24xrgb  gx_validation_graphics_driver_setup_24xrgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets.c"

/* Define functions, which do nothing. */
static UINT display_layer_initialize(INT layer, GX_CANVAS *canvas)
{
   GX_PARAMETER_NOT_USED(layer); 
   GX_PARAMETER_NOT_USED(canvas); 
   return GX_SUCCESS;
}

static VOID display_layer_show(INT layer){GX_PARAMETER_NOT_USED(layer); }
static VOID display_layer_hide(INT layer){GX_PARAMETER_NOT_USED(layer); }
static VOID display_layer_alpha_set(INT layer, GX_UBYTE alpha){
                                                               GX_PARAMETER_NOT_USED(layer); 
                                                               GX_PARAMETER_NOT_USED(alpha); }
static VOID display_layer_offset_set(INT layer, GX_VALUE xoffset, GX_VALUE yoffset){
                                                                                    GX_PARAMETER_NOT_USED(layer);
                                                                                    GX_PARAMETER_NOT_USED(xoffset);
                                                                                    GX_PARAMETER_NOT_USED(yoffset); }


char test_comment[255];
static GX_DISPLAY_LAYER_SERVICES display_layer_services = 
{
    display_layer_initialize,
    display_layer_show,
    display_layer_hide,
    display_layer_alpha_set,
    display_layer_offset_set
};

static GX_DISPLAY_LAYER_SERVICES display_layer_services_1 = 
{
0,
0,
0,
0,
0
};

static UINT display_layer_initialize_2(INT layer, GX_CANVAS *canvas)
{
   GX_PARAMETER_NOT_USED(layer); 
   GX_PARAMETER_NOT_USED(canvas); 
   return GX_FAILURE;
}


static GX_DISPLAY_LAYER_SERVICES display_layer_services_2 = 
{
display_layer_initialize_2,
0,
0,
0,
0
};

/* Note: function gx_canvas_hide isn't correct for now.
         so frame 2 isn't the expected output. 
         This test would fail if function gx_canvas_hide is fixed, 
         then it should be updated. */
static VOID control_thread_entry(ULONG input)
{
int       frame_id = 1;
GX_DISPLAY *display;

    ToggleScreen(pWindowScreen, pButtonScreen);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Original window screen. ");
    gx_validation_screen_refresh();   
    
    gx_canvas_hardware_layer_bind(&animation_canvas, 1); // should return GX_INVALID_DISPLAY
    
    display = root->gx_window_root_canvas->gx_canvas_display;
    display->gx_display_layer_services = &display_layer_services_1;
    gx_canvas_hardware_layer_bind(&animation_canvas, 1); // should return GX_INVALID_DISPLAY    
    
    display->gx_display_layer_services = &display_layer_services;
    
    gx_canvas_hardware_layer_bind(&animation_canvas, 1); // should return GX_SUCCESS
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Bound animation_canvas to layer 1.");
    gx_validation_screen_refresh();
    
    gx_canvas_hide(&animation_canvas);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Hide animation_canvas.");
    gx_validation_screen_refresh();
    
    gx_canvas_show(&animation_canvas);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Show animation_canvas.");
    gx_validation_screen_refresh();

    gx_canvas_alpha_set(&animation_canvas, 100);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Set canvas alpha 100.");
    gx_validation_screen_refresh();
    
    gx_canvas_offset_set(&animation_canvas, 10, 10);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Set canvas offset (10, 10).");
    gx_validation_screen_refresh();

    display->gx_display_layer_services = &display_layer_services_2;

    if(gx_canvas_hardware_layer_bind(&animation_canvas, 1) != GX_FAILURE)
    {
        exit(1);
    }
    gx_canvas_alpha_set(&animation_canvas, 100);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Set empty layer service, set canvas alpha 100");
    gx_validation_screen_refresh();
 
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
