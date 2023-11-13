/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_system.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_h_pixelmap_slider_16bpp", /* Test name */
    266, 173, 434, 222  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the pixelmap slider
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
int needle_insert[] = {-5, 5, 0};
int increment[] = {5, 10};

int new_pos;

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{

int pos, min_travel, max_travel, increment_index, needle_index, backgd_index = 1, border_index = 1;
int frame_id = 1;
GX_PIXELMAP_SLIDER *slider;
ULONG event_memory[GX_EVENT_SIZE];
GX_EVENT *event_ptr;
int slider_left, slider_right, slider_width;
int mintravel, maxtravel;
GX_SLIDER_INFO *slider_info;
GX_PIXELMAP_SLIDER_INFO *pixinfo;
GX_RECTANGLE new_size;

    ToggleScreen(pIndicatorScreen, pButtonScreen);
    gx_widget_find((GX_WIDGET *) pIndicatorScreen, ID_PIXELMAP_SLIDER_H, GX_SEARCH_DEPTH_INFINITE, (GX_WIDGET **) &slider);
    slider_info = &slider -> gx_slider_info;
    pixinfo = &slider -> gx_pixelmap_slider_pixelmap_info;
    gx_slider_travel_get((GX_SLIDER *)slider, slider_info, &mintravel, &maxtravel);
    event_ptr =  (GX_EVENT *)((VOID *)&event_memory[0]);
    memset(event_ptr, 0, sizeof(GX_EVENT));
    slider_left = slider -> gx_widget_size.gx_rectangle_left;
    slider_right = slider -> gx_widget_size.gx_rectangle_right;
    slider_width = slider_right - slider_left + 1;
    
    /* Test needle insert.  */
    for (needle_index = 0; needle_index < 3; needle_index ++)
    {
        slider_info->gx_slider_info_needle_inset = needle_insert[needle_index];

        gx_slider_info_set((GX_SLIDER *)slider, slider_info);

        gx_validation_set_frame_id(frame_id);

        sprintf(test_comment, "needle_insert: %d", needle_insert[needle_index]);

        gx_validation_set_frame_comment(test_comment);

        /* Force a screen refresh. */
        gx_validation_screen_refresh();

        /* Increment frame ID */
        frame_id ++;
    }

    for (increment_index = 1; increment_index >= 0; increment_index --)
    {
        slider_info -> gx_slider_info_increment = increment[increment_index];

        if (increment_index)
        {
            gx_slider_value_set((GX_SLIDER *)slider, slider_info, slider_info->gx_slider_info_min_val);

            /* Shift the slider up by 5 pixels. */
            gx_widget_shift((GX_WIDGET *)slider, 0, -5, GX_TRUE);
        }
        else
        {
            gx_slider_value_set((GX_SLIDER *)slider, slider_info, slider_info->gx_slider_info_max_travel);

            /* Shift the slider down by 5 pixels.  */
            gx_widget_shift((GX_WIDGET *)slider, 0, 5, GX_TRUE);

            /* Resize slider widget.  */
            new_size = slider -> gx_widget_size;
            gx_utility_rectangle_resize(&new_size, 5);
            gx_widget_resize((GX_WIDGET *)slider, &new_size);
        }

        for (min_travel = 10; min_travel < slider_width; min_travel += 40)
        {
            for (max_travel = 10; max_travel < (slider_width - min_travel); max_travel +=40)
            {
                for(pos = slider_left; pos <= slider_right; pos += 20)
                {
                    /* Inform the validation system 
                    (1) Frame ID, which identifies a specific test configuration;
                    (2) Start recording frame on the next toggle operation.
                    */
                    GX_ENTER_CRITICAL

                    slider_info -> gx_slider_info_min_travel = min_travel;
                    slider_info -> gx_slider_info_max_travel = max_travel; 
                    gx_slider_info_set((GX_SLIDER *)slider, slider_info);

                    sprintf(test_comment, "min_travel:%d, max_travel:%d, increment:%d", min_travel, max_travel, increment[increment_index]);

                    switch(backgd_index)
                    {
                    case 1:
                        pixinfo -> gx_pixelmap_slider_info_lower_background_pixelmap = GX_PIXELMAP_ID_HORIZONTAL_FILL_BKGND;
                        gx_pixelmap_slider_pixelmap_set(slider, pixinfo);
                        gx_widget_style_add((GX_WIDGET *)slider, GX_STYLE_TILE_BACKGROUND);
                        gx_widget_style_remove((GX_WIDGET *)slider, GX_STYLE_TRANSPARENT);
                        strcat(test_comment, ", tiled, with lowermap, transparent");
                        backgd_index ++;
                        break;
                    case 2:
                        pixinfo -> gx_pixelmap_slider_info_needle_pixelmap = GX_NULL;
                        gx_pixelmap_slider_pixelmap_set(slider, pixinfo);
                        strcat(test_comment, ", tiled, with lowermap, without needlemap");
                        backgd_index ++;
                        break;
                    case 3:
                        pixinfo -> gx_pixelmap_slider_info_needle_pixelmap = GX_PIXELMAP_ID_I_INDICATOR_HORIZONTAL;
                        gx_pixelmap_slider_pixelmap_set(slider, pixinfo);
                        gx_widget_style_remove((GX_WIDGET *)slider, GX_STYLE_TILE_BACKGROUND);
                        strcat(test_comment, ", with lowermap");
                        backgd_index ++;
                        break;
                    case 4:
                        pixinfo -> gx_pixelmap_slider_info_lower_background_pixelmap = GX_NULL;
                        gx_pixelmap_slider_pixelmap_set(slider, pixinfo);
                        gx_widget_fill_color_set((GX_WIDGET *)slider, GX_COLOR_ID_ORANGE, GX_COLOR_ID_WHITE, GX_COLOR_ID_ORANGE);
                        gx_widget_style_remove((GX_WIDGET *)slider, GX_STYLE_TRANSPARENT);
                        strcat(test_comment, ", widget background");
                        switch(border_index)
                        {
                        case 1:
                            gx_widget_border_style_set(slider, GX_STYLE_BORDER_NONE);
                            strcat(test_comment, ", border: none");
                            border_index ++;
                            break;
                        case 2:
                            gx_widget_border_style_set(slider, GX_STYLE_BORDER_THICK);
                            strcat(test_comment, ", border: thick");
                            border_index ++;
                            break;
                        case 3:
                            gx_widget_border_style_set(slider, GX_STYLE_BORDER_THIN);
                            strcat(test_comment, ", border: thin");
                            border_index ++;
                            break;
                        case 4:
                            gx_widget_border_style_set(slider, GX_STYLE_BORDER_RAISED);
                            strcat(test_comment, ", border: raised");
                            border_index ++;
                            break;
                        case 5:
                            gx_widget_border_style_set(slider, GX_STYLE_BORDER_RECESSED);
                            strcat(test_comment, ", border: recessed");
                            border_index = 1;
                            break;
                        }
                        backgd_index ++;
                        break;
                    case 5:
                        gx_widget_style_add((GX_WIDGET *)slider, GX_STYLE_TRANSPARENT);
                        backgd_index = 1;
                        strcat(test_comment, ", transparent");
                        break;
                    }

                    gx_validation_set_frame_id(frame_id);
                    gx_validation_set_frame_comment(test_comment);

                    event_ptr->gx_event_payload.gx_event_pointdata.gx_point_y = 200;
                    event_ptr->gx_event_payload.gx_event_pointdata.gx_point_x = pos;
                    event_ptr -> gx_event_type = GX_EVENT_PEN_DOWN;
                    event_ptr -> gx_event_display_handle = 1;
                    gx_system_event_send(event_ptr);
                    GX_EXIT_CRITICAL
                    event_ptr->gx_event_payload.gx_event_pointdata.gx_point_x = pos + 10;
                    event_ptr -> gx_event_type = GX_EVENT_PEN_DRAG;
                    event_ptr -> gx_event_display_handle = 1;
                    gx_system_event_send(event_ptr);

                    /* Increment frame ID */
                    frame_id ++;
                 }
            }
        }
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





