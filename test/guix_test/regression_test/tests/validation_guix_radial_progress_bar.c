/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_radial_progress_bar", /* Test name */
    50, 100, 300, 350  /* Define the coordinates of the capture area.
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
#include "demo_guix_radial_progress_bar.c"

char test_comment[255];

const char *direction[]={
"clockwise",
"anti-clockwise"};

GX_POINT points_on_track[] = { { 232, 149 }, { 247, 290 }, { 101, 300 }, { 89, 177 } };
GX_POINT points_off_track[] = { { 238, 174 }, { 235, 258 }, { 113, 268 }, { 108, 189 } };
GX_CONST GX_CHAR *on_track_text[] = { "p1", "p2", "p3", "p4" };
GX_CONST GX_CHAR *off_track_text[] = { "pp1", "pp2", "pp3", "pp4" };

VOID test_radial_progress_bar_draw(GX_RADIAL_PROGRESS_BAR *progress_bar)
{
GX_BRUSH    *brush;
INT          index;
GX_RECTANGLE rect;

    gx_context_brush_get(&brush);
    brush->gx_brush_alpha = radial_progress_bar_alpha;

    gx_radial_progress_bar_draw(progress_bar);

    for (index = 0; index < (INT)(sizeof(points_on_track) / sizeof(GX_POINT)); index++)
    {
        rect.gx_rectangle_left = points_on_track[index].gx_point_x;
        rect.gx_rectangle_top = points_on_track[index].gx_point_y;
        rect.gx_rectangle_right = rect.gx_rectangle_left + 2;
        rect.gx_rectangle_bottom = rect.gx_rectangle_top + 2;

        gx_context_brush_width_set(1);
        gx_context_font_set(GX_FONT_ID_DEFAULT);

        gx_context_fill_color_set(GX_COLOR_ID_RED);
        gx_context_line_color_set(GX_COLOR_ID_RED);
        gx_canvas_rectangle_draw(&rect);

        gx_context_line_color_set(GX_COLOR_ID_GREEN);
        gx_canvas_text_draw(rect.gx_rectangle_right + 2, rect.gx_rectangle_top, on_track_text[index], GX_STRLEN(on_track_text[index]));

        rect.gx_rectangle_left = points_off_track[index].gx_point_x;
        rect.gx_rectangle_top = points_off_track[index].gx_point_y;
        rect.gx_rectangle_right = rect.gx_rectangle_left + 2;
        rect.gx_rectangle_bottom = rect.gx_rectangle_top + 2;

        gx_context_fill_color_set(GX_COLOR_ID_RED);
        gx_context_line_color_set(GX_COLOR_ID_RED);
        gx_canvas_rectangle_draw(&rect);

        gx_context_line_color_set(GX_COLOR_ID_BTN_BORDER);
        gx_context_font_set(GX_FONT_ID_DEFAULT);
        gx_canvas_text_draw(rect.gx_rectangle_right + 2, rect.gx_rectangle_top, off_track_text[index], GX_STRLEN(off_track_text[index]));
    }
}

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int      frame_id = 1;
int      anchor_value;
int      value;
int      normal_brush_width = 1;
int      selected_brush_width = 28;
GX_EVENT my_event;
int      enable_style;
int      index;
int      move_direction;

    gx_widget_draw_set((GX_WIDGET *)progress_bar, test_radial_progress_bar_draw);
    /* Test enbale style.  */
     
    /* Initialize event structure.  */
    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_display_handle = 1;

    for(enable_style = 0; enable_style < 2; enable_style ++)
    {
        if(enable_style)
        {
            gx_widget_style_add((GX_WIDGET *)progress_bar, GX_STYLE_ENABLED);
        }
        else
        {
            gx_widget_style_remove((GX_WIDGET *)progress_bar, GX_STYLE_ENABLED);
        }

        for(move_direction = 0; move_direction < 2; move_direction++)
        {
            if(move_direction == 0)
            {
                /* Radial progress bar move clockwise.  */
                gx_radial_progress_bar_value_set(progress_bar, -180);
            }
            else
            {
                /* Radial progress bar move anti-clockwise.  */
                gx_radial_progress_bar_value_set(progress_bar, 180);
            }
        
            for(index = 0; index < (int)(sizeof(points_on_track)/sizeof(GX_POINT)); index++)
            {
                /* Click on track.  */
                my_event.gx_event_type = GX_EVENT_PEN_DOWN;
                my_event.gx_event_payload.gx_event_pointdata.gx_point_x = points_on_track[index].gx_point_x;
                my_event.gx_event_payload.gx_event_pointdata.gx_point_y = points_on_track[index].gx_point_y;
                gx_system_event_send(&my_event);

                my_event.gx_event_type = GX_EVENT_PEN_UP;
                gx_system_event_send(&my_event);

                gx_validation_set_frame_id(frame_id++);
                sprintf(test_comment, "Enable style: %d, move %s, click at %s.",
                    enable_style, direction[move_direction], on_track_text[index]);
                gx_validation_set_frame_comment(test_comment);
                gx_validation_screen_refresh();
            }

            for(index = 0; index < (int)(sizeof(points_off_track)/sizeof(GX_POINT)); index++)
            {
                /* Click off track.  */
                my_event.gx_event_type = GX_EVENT_PEN_DOWN;
                my_event.gx_event_payload.gx_event_pointdata.gx_point_x = points_off_track[index].gx_point_x;
                my_event.gx_event_payload.gx_event_pointdata.gx_point_y = points_off_track[index].gx_point_y;
                gx_system_event_send(&my_event);

                my_event.gx_event_type = GX_EVENT_PEN_UP;
                gx_system_event_send(&my_event);

                gx_validation_set_frame_id(frame_id++);
                sprintf(test_comment, "Enable style: %d, move %s, click at %s.",
                    enable_style, direction[move_direction], off_track_text[index]);
                gx_validation_set_frame_comment(test_comment);
                gx_validation_screen_refresh();
            }

            for(index = 0; index < (int)(sizeof(points_off_track)/sizeof(GX_POINT)); index++)
            {
                /* Send drag event.  */
                my_event.gx_event_type = GX_EVENT_PEN_DOWN;
                my_event.gx_event_payload.gx_event_pointdata.gx_point_x = points_on_track[0].gx_point_x;
                my_event.gx_event_payload.gx_event_pointdata.gx_point_y = points_on_track[0].gx_point_y;
                gx_system_event_send(&my_event);
 
                
                my_event.gx_event_type = GX_EVENT_PEN_DRAG;
                my_event.gx_event_payload.gx_event_pointdata.gx_point_x = points_off_track[index].gx_point_x;
                my_event.gx_event_payload.gx_event_pointdata.gx_point_y = points_off_track[index].gx_point_y;
                gx_system_event_send(&my_event);

                my_event.gx_event_type = GX_EVENT_PEN_UP;
                gx_system_event_send(&my_event);

                gx_validation_set_frame_id(frame_id++);
                sprintf(test_comment, "Enable style: %d, move %s, drag from %s to %s.",
                    enable_style, direction[move_direction], on_track_text[0], off_track_text[index]);
                gx_validation_set_frame_comment(test_comment);
                gx_validation_screen_refresh();
           }
        }
    }

    /* Test properties. */

    for(index = 0; index < 8; index++)
    {
                
        switch(index)
        {
        case 0:
            /* Set text color.  */
            gx_radial_progress_bar_text_color_set(progress_bar, GX_COLOR_ID_SLIDER_NEEDLE_LINE2, GX_COLOR_ID_GREEN, GX_COLOR_ID_SLIDER_NEEDLE_LINE2);
            sprintf(test_comment, "Set text color");
            break;

        case 1:
            /* Set brush color.  */
            info->gx_radial_progress_bar_info_normal_brush_color = GX_COLOR_ID_WINDOW_BORDER;
            info->gx_radial_progress_bar_info_selected_brush_color = GX_COLOR_ID_BTN_BORDER;
            info->gx_radial_progress_bar_info_anchor_val = -1;
            gx_radial_progress_bar_info_set(progress_bar, info);
            sprintf(test_comment, "Set brush color, set anchor value to -1");
            break;
        case 2:
            info->gx_radial_progress_bar_info_anchor_val = 400;
            gx_radial_progress_bar_info_set(progress_bar, info);
            sprintf(test_comment, "Set anchor value to 400");
            break;

        case 3:
            /* Set text font.  */
            gx_radial_progress_bar_font_set(progress_bar, GX_FONT_ID_SYSTEM);
            sprintf(test_comment, "Set font");
            break;

        case 4:
            /* Hide text.  */
            gx_widget_style_remove((GX_WIDGET *)progress_bar, GX_STYLE_PROGRESS_TEXT_DRAW);
            sprintf(test_comment, "Hide text");
            break;

        case 5:
            /* Show text.  */
            gx_widget_style_add((GX_WIDGET *)progress_bar, GX_STYLE_PROGRESS_TEXT_DRAW);
            sprintf(test_comment, "Show text");
            break;

        case 6:
            /* Show text without percent flag.  */
            gx_widget_style_remove((GX_WIDGET *)progress_bar, GX_STYLE_PROGRESS_PERCENT);
            sprintf(test_comment, "Disable percent flag");
            break;

        case 7:
            /* Show text with percent flag.  */
            gx_widget_style_add((GX_WIDGET *)progress_bar, GX_STYLE_PROGRESS_PERCENT);
            sprintf(test_comment, "Enable percent flag");
            break;
        }
        
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh(); 
    }


    for(anchor_value = 0; anchor_value < 360; anchor_value += 20)
    {

        if(anchor_value > 180)
        {
            gx_widget_style_remove((GX_WIDGET *)progress_bar, GX_STYLE_RADIAL_PROGRESS_ALIAS);
            gx_widget_style_remove((GX_WIDGET *)progress_bar, GX_STYLE_RADIAL_PROGRESS_ROUND);
           
            gx_validation_set_frame_id(frame_id++);
            sprintf(test_comment, "Draw without aliasing, draw with round end");
            gx_validation_set_frame_comment(test_comment);
            gx_validation_screen_refresh();
        }

        for (value = anchor_value - 380; value <= anchor_value + 360; value += 20)
        {
            normal_brush_width ++;
            if(normal_brush_width > 30)
            {
                normal_brush_width = 1;
            }

            selected_brush_width --;
            if(selected_brush_width < 1)
            {
                selected_brush_width = 20;
            }

            /* Set anchor value. */
            gx_radial_progress_bar_anchor_set(progress_bar, anchor_value);

            /* Set radial progress bar value.  */
            gx_radial_progress_bar_value_set(progress_bar, value);

            /* Set brush width. */
            info->gx_radial_progress_bar_info_selected_brush_width = selected_brush_width;
            info->gx_radial_progress_bar_info_normal_brush_width = normal_brush_width;
            gx_radial_progress_bar_info_set(progress_bar, info);

            gx_validation_set_frame_id(frame_id++);
            sprintf(test_comment, "anchor value:%d, value:%d, normal brush width:%d, selected brush width:%d",
                    anchor_value, value, normal_brush_width, selected_brush_width);
            gx_validation_set_frame_comment(test_comment);
            gx_validation_screen_refresh();
        }
    }

    gx_widget_hide(progress_bar);
    gx_radial_progress_bar_text_color_set(progress_bar, 0, 0, 0);
    gx_widget_show(progress_bar);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Set text_color for a hidden progress bar.");
    gx_validation_screen_refresh();

    gx_widget_hide(progress_bar);
    gx_radial_progress_bar_font_set(progress_bar, 0);
    gx_widget_show(progress_bar);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Set font for a hidden progress bar.");
    gx_validation_screen_refresh();

    gx_widget_hide(progress_bar);
    gx_radial_progress_bar_anchor_set(progress_bar, 0);
    gx_widget_show(progress_bar);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Set anchor for a hidden progress bar.");
    gx_validation_screen_refresh();

    gx_widget_hide(progress_bar);
    gx_radial_progress_bar_value_set(progress_bar, 0);
    gx_widget_show(progress_bar);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Set value for a hidden progress bar.");
    gx_validation_screen_refresh();

    gx_widget_style_remove((GX_WIDGET *)progress_bar, GX_STYLE_ENABLED);
    memset(&my_event, 0, sizeof(my_event));
    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    my_event.gx_event_target = (GX_WIDGET *)progress_bar;
    gx_system_event_send(&my_event);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Set PEN_DOWN event to a progress bar without GX_STYLE_ENABLED.");
    gx_validation_screen_refresh();

    gx_widget_status_remove((GX_WIDGET *)progress_bar, GX_STATUS_OWNS_INPUT);
    memset(&my_event, 0, sizeof(my_event));
    my_event.gx_event_type = GX_EVENT_PEN_UP;
    my_event.gx_event_target = (GX_WIDGET *)progress_bar;
    gx_system_event_send(&my_event);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Set PEN_UP event to a progress bar without GX_STATUS_OWNS_INPUT.");
    gx_validation_screen_refresh();

    gx_widget_status_remove((GX_WIDGET *)progress_bar, GX_STATUS_OWNS_INPUT);
    memset(&my_event, 0, sizeof(my_event));
    my_event.gx_event_type = GX_EVENT_PEN_DRAG;
    my_event.gx_event_target = (GX_WIDGET *)progress_bar;
    gx_system_event_send(&my_event);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Set PEN_DRAG event to a progress bar without GX_STATUS_OWNS_INPUT.");
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
