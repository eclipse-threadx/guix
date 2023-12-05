#include <stdio.h>
#include "gx_api.h"
#include "all_widgets_5_5_1_resources.h"
#include "all_widgets_5_5_1_specifications.h"

extern VOID           rotate_memory_free(VOID *mem);

INT                   rotate_angle = 30;
INT                   rotate_pixelmap_id = GX_PIXELMAP_ID_ROTATE_FOOT;
GX_BOOL               canvas_rotate = GX_FALSE;

VOID update_angle_prompt()
{
    GX_PROMPT *pp;
    static GX_CHAR text[10];

    gx_widget_find((GX_WIDGET *)&rotate_screen, ID_ANGLE_DISPLAY, 0, &pp);

    if (pp)
    {
        gx_utility_ltoa(rotate_angle, text, 10);
        gx_prompt_text_set(pp, text);
    }
}

UINT rotate_event_handler(GX_WINDOW *window, GX_EVENT *myevent)
{
UINT status = 0;

    switch (myevent->gx_event_type)
    {
    case GX_SIGNAL(ID_SLIDER_ANGLE, GX_EVENT_SLIDER_VALUE):
        rotate_angle = myevent->gx_event_payload.gx_event_longdata;
        update_angle_prompt();
        gx_system_dirty_mark((GX_WIDGET *)&rotate_screen.rotate_screen_pixelmap_window);
        break;

    case GX_SIGNAL(ID_RADIO_FOOT, GX_EVENT_RADIO_SELECT):
        rotate_pixelmap_id = GX_PIXELMAP_ID_ROTATE_FOOT;
        gx_system_dirty_mark((GX_WIDGET *)&rotate_screen.rotate_screen_pixelmap_window);
        break;

    case GX_SIGNAL(ID_RADIO_APPLE, GX_EVENT_RADIO_SELECT):
        rotate_pixelmap_id = GX_PIXELMAP_ID_ROTATE_APPLE;
        gx_system_dirty_mark((GX_WIDGET *)&rotate_screen.rotate_screen_pixelmap_window);
        break;

    case GX_SIGNAL(ID_RADIO_FISH, GX_EVENT_RADIO_SELECT):
        rotate_pixelmap_id = GX_PIXELMAP_ID_ROTATE_FISH;
        gx_system_dirty_mark((GX_WIDGET *)&rotate_screen.rotate_screen_pixelmap_window);
        break;
        
    case GX_SIGNAL(ID_DIRECT_ROTATE, GX_EVENT_TOGGLE_ON):
        canvas_rotate = GX_TRUE;
        gx_system_dirty_mark((GX_WIDGET *)&rotate_screen.rotate_screen_pixelmap_window);
        break;

    case GX_SIGNAL(ID_DIRECT_ROTATE, GX_EVENT_TOGGLE_OFF):
        canvas_rotate = GX_FALSE;
        gx_system_dirty_mark((GX_WIDGET *)&rotate_screen.rotate_screen_pixelmap_window);
        break;

    default:
        return gx_window_event_process(window, myevent);
        break;
    }
    return status;
}

VOID pixelmap_draw(GX_WINDOW *window)
{
INT  xpos;
INT  ypos;
GX_PIXELMAP *pixelmap;
GX_RECTANGLE win_size;
GX_PIXELMAP destination;
INT x_axis_old;
INT y_axis_old;
INT x_axis;
INT y_axis;

    gx_window_draw((GX_WINDOW*)window);

    gx_context_pixelmap_get(rotate_pixelmap_id, &pixelmap);

    win_size = rotate_screen.rotate_screen_pixelmap_window.gx_widget_size;

    xpos = (win_size.gx_rectangle_right + win_size.gx_rectangle_left - pixelmap->gx_pixelmap_width) >> 1;
    ypos = (win_size.gx_rectangle_top + win_size.gx_rectangle_bottom - pixelmap->gx_pixelmap_height) >> 1;

    if (canvas_rotate)
    {
        if ((rotate_angle % 360) > 180)
        {
            x_axis = (pixelmap->gx_pixelmap_width >> 1);
            y_axis = (pixelmap->gx_pixelmap_height >> 1);
        }
        else
        {
            x_axis = -1;
            y_axis = -1;
        }
        gx_canvas_pixelmap_rotate(xpos, ypos, pixelmap, rotate_angle, x_axis, y_axis);
    }
    else
    {
        x_axis_old = (pixelmap->gx_pixelmap_width >> 1);
        y_axis_old = (pixelmap->gx_pixelmap_height >> 1);

        x_axis = x_axis_old;
        y_axis = y_axis_old;

        if (gx_utility_pixelmap_rotate(pixelmap, rotate_angle, &destination, &x_axis, &y_axis) == GX_SUCCESS)
        {
            xpos += x_axis_old - x_axis;
            ypos += y_axis_old - y_axis;

            /* Draw rotated pixelmap.  */
            gx_canvas_pixelmap_draw(xpos, ypos, &destination);

            /* free memory of the rotated pixelmap */
            if (destination.gx_pixelmap_data)
            {
                rotate_memory_free((VOID *)destination.gx_pixelmap_data);
            }
            if (destination.gx_pixelmap_aux_data)
            {
                rotate_memory_free((VOID *)destination.gx_pixelmap_aux_data);
            }
        }
        else
        {
            gx_canvas_pixelmap_draw(xpos, ypos, pixelmap);
        }
    }
}
