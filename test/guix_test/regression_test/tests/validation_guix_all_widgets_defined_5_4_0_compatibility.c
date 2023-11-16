/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_defined_5_4_0_compatibility", /* Test name */
    0, 0, 640, 480  /* Define the coordinates of the capture area.
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

#include "demo_guix_all_widgets_defined_5_4_0_compatibility.c"

GX_WINDOW *screen_list[]={
    (GX_WINDOW *)&button_screen,
    (GX_WINDOW *)&math_screen,
    (GX_WINDOW *)&scroll_wheel_screen,
    (GX_WINDOW *)&sprite_screen,
    (GX_WINDOW *)&gauge_screen,
    (GX_WINDOW *)&popup_modal,
    (GX_WINDOW *)&indicator_screen,
    (GX_WINDOW *)&text_screen,
    (GX_WINDOW *)&window_screen,
};

INT curve_id_list[]={
    ID_SIN,
    ID_COS,
    ID_ASIN,
    ID_ACOS
};

char test_comment[256];

extern void draw_sin();
extern void draw_cos();
extern void draw_asin();
extern void draw_acos();

static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
GX_WINDOW *window;
GX_WINDOW *prewin;
int win_index;
int index;

    for(win_index = 0; win_index < (int)(sizeof(screen_list)/sizeof(GX_WINDOW *)); win_index++)
    {
        window = screen_list[win_index];

        if(window->gx_widget_id != ID_BUTTON_SCREEN)
        {
            ToggleScreen(window, prewin);
        }

        if(window->gx_widget_id == ID_MATH_SCREEN)
        {
            for(index = 0; index < (int)(sizeof(curve_id_list) / sizeof(INT)); index++)
            {
                switch(curve_id_list[index])
                {
                    case ID_SIN:
                        draw_sin();
                        sprintf(test_comment, "draw sin");
                        break;

                    case ID_COS:
                        draw_cos();
                        sprintf(test_comment, "draw cos");
                        break;

                    case ID_ASIN:
                        draw_asin();
                        sprintf(test_comment, "draw asin");
                        break;

                    case ID_ACOS:
                        draw_acos();
                        sprintf(test_comment, "draw acos");
                        break;
                }
                
                gx_validation_set_frame_id(frame_id++);
                gx_validation_set_frame_comment(test_comment);
                gx_validation_screen_refresh();
            }
        }
        else
        {
            gx_validation_set_frame_id(frame_id++);
            sprintf(test_comment, "%s", window->gx_widget_name);
            gx_validation_set_frame_comment(test_comment);
            gx_validation_screen_refresh();
        }

        prewin = window;
        window++;
    }
    
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
