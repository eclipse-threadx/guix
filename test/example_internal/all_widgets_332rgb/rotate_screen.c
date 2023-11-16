/****************************************************************/
/* This file tests gx_utility_332rgb_roate and                  */
/*                 _gx_display_driver_332rgb_pixelmap_rotate    */
/*                                                              */
/****************************************************************/
#include <stdio.h>
#include "gx_api.h"
#include "all_widgets_332rgb_resources.h"
#include "all_widgets_332rgb_specifications.h"

extern GX_CONST GX_PIXELMAP *display_1_theme_1_pixelmap_table[];
extern VOID                  memory_free(VOID *mem);

extern GX_WINDOW     *pRotateScreen;
extern GX_WINDOW     *pRotateWin;
INT                   angle = 30;
GX_RESOURCE_ID        pixelmap_id = GX_PIXELMAP_ID_ROTATE_FOOT;
GX_BOOL               direct = GX_FALSE;

VOID update_angle_prompt()
{
    GX_PROMPT *pp;
    static GX_CHAR angle_text_buffer[10];
    GX_STRING angle_text;

    gx_widget_find(pRotateScreen, ID_ANGLE_DISPLAY, 0, &pp);

    if (pp)
    {
        gx_utility_ltoa(angle, angle_text_buffer, 10);
        angle_text.gx_string_ptr = angle_text_buffer;
        angle_text.gx_string_length = strnlen(angle_text_buffer, sizeof(angle_text_buffer));
        gx_prompt_text_set_ext(pp, &angle_text);
    }
}

UINT rotate_event_handler(GX_WINDOW *window, GX_EVENT *myevent)
{
    UINT status = 0;

    switch (myevent->gx_event_type)
    {
    case GX_SIGNAL(ID_SLIDER_ANGLE, GX_EVENT_SLIDER_VALUE):
        angle = myevent->gx_event_payload.gx_event_longdata;
        update_angle_prompt();
        gx_system_dirty_mark(pRotateScreen);
        break;

    case GX_SIGNAL(ID_RADIO_FOOT, GX_EVENT_RADIO_SELECT):
        pixelmap_id = GX_PIXELMAP_ID_ROTATE_FOOT;
        gx_system_dirty_mark(pRotateScreen);
        break;

    case GX_SIGNAL(ID_RADIO_APPLE, GX_EVENT_RADIO_SELECT):
        pixelmap_id = GX_PIXELMAP_ID_ROTATE_APPLE;
        gx_system_dirty_mark(pRotateScreen);
        break;

    case GX_SIGNAL(ID_RADIO_FISH, GX_EVENT_RADIO_SELECT):
        pixelmap_id = GX_PIXELMAP_ID_ROTATE_FISH;
        gx_system_dirty_mark(pRotateScreen);
        break;

    case GX_SIGNAL(ID_DIRECT_ROTATE, GX_EVENT_TOGGLE_ON):
        direct = GX_TRUE;
        gx_system_dirty_mark(pRotateScreen);
        break;

    case GX_SIGNAL(ID_DIRECT_ROTATE, GX_EVENT_TOGGLE_OFF):
        direct = GX_FALSE;
        gx_system_dirty_mark(pRotateScreen);
        break;

    default:
        return next_button_handler(window, myevent);
        break;
    }
    return status;
}

VOID pixelmap_draw(GX_WINDOW *window)
{
INT              xpos;
INT              ypos;
GX_PIXELMAP     *pixelmap;
GX_RECTANGLE     win_size;
GX_PIXELMAP      destination;
INT             x_axis_old;
INT             y_axis_old;
INT             x_axis;
INT             y_axis;
UINT             status = GX_FAILURE;

    gx_window_draw((GX_WINDOW*)window);

    gx_context_pixelmap_get(pixelmap_id, &pixelmap);

    win_size = pRotateWin->gx_widget_size;

    xpos = (win_size.gx_rectangle_right + win_size.gx_rectangle_left - pixelmap->gx_pixelmap_width) >> 1;
    ypos = (win_size.gx_rectangle_top + win_size.gx_rectangle_bottom - pixelmap->gx_pixelmap_height) >> 1;

    if (direct)
    {
        gx_canvas_pixelmap_rotate(xpos, ypos, pixelmap, angle, -1, -1);
    }
    else
    {
        x_axis_old = (pixelmap->gx_pixelmap_width >> 1);
        y_axis_old = (pixelmap->gx_pixelmap_height >> 1);

        x_axis = x_axis_old;
        y_axis = y_axis_old;
        if (angle == 0)
        {
            xpos += x_axis_old - x_axis;
            ypos += y_axis_old - y_axis;
            gx_canvas_pixelmap_draw(xpos, ypos, pixelmap);
            return;
        }
        else if (angle % 90 == 0)
        {
            status = gx_utility_pixelmap_simple_rotate(pixelmap, angle, &destination, &x_axis, &y_axis);
        }
        else
        {
            status = gx_utility_pixelmap_rotate(pixelmap, angle, &destination, &x_axis, &y_axis);
        }

        if (status == GX_SUCCESS)
        {
            xpos += x_axis_old - x_axis;
            ypos += y_axis_old - y_axis;

            /* Draw rotated pixelmap.  */
            gx_canvas_pixelmap_draw(xpos, ypos, &destination);

            /* free memory of the rotated pixelmap */
            if (destination.gx_pixelmap_data)
            {
                memory_free((VOID *)destination.gx_pixelmap_data);
            }
            /* free memory of the rotated pixelmap */
            if (destination.gx_pixelmap_aux_data)
            {
                memory_free((VOID *)destination.gx_pixelmap_aux_data);
            }
        }
    }
}
