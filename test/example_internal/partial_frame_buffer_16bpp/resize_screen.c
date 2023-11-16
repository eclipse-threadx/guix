/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "gx_canvas.h"

#include "partial_frame_buffer_16bpp_resources.h"
#include "partial_frame_buffer_16bpp_specifications.h"

extern VOID memory_free(VOID *mem);

#define RATIO_ONE 100

INT x_ratio = RATIO_ONE;
INT y_ratio = RATIO_ONE;

GX_RESOURCE_ID   resize_pixelmap_id = GX_PIXELMAP_ID_ROTATE_FOOT;

VOID resize_ratio_format(GX_NUMERIC_PROMPT *prompt, INT value)
{
    INT index = 0;

    /* Transfer float value to string. */
    prompt->gx_numeric_prompt_buffer[index++] = '0' + (value / RATIO_ONE);
    prompt->gx_numeric_prompt_buffer[index++] = '.';
    prompt->gx_numeric_prompt_buffer[index++] = '0' + (value % RATIO_ONE) / 10;
    prompt->gx_numeric_prompt_buffer[index++] = '0' + (value % RATIO_ONE) % 10;
    prompt->gx_numeric_prompt_buffer[index] = '\0';
}

UINT resize_screen_event_handler(GX_WINDOW *window, GX_EVENT *myevent)
{
    switch (myevent->gx_event_type)
    {
    case GX_SIGNAL(ID_SLIDER_WIDTH, GX_EVENT_SLIDER_VALUE):
        x_ratio = myevent->gx_event_payload.gx_event_longdata;
        gx_numeric_prompt_value_set(&resize_screen.resize_screen_width, x_ratio);
        gx_system_dirty_mark((GX_WIDGET *)&resize_screen.resize_screen_resize_window);
        break;

    case GX_SIGNAL(ID_SLIDER_HEIGHT, GX_EVENT_SLIDER_VALUE):
        y_ratio = myevent->gx_event_payload.gx_event_longdata;
        gx_numeric_prompt_value_set(&resize_screen.resize_screen_height, y_ratio);
        gx_system_dirty_mark((GX_WIDGET *)&resize_screen.resize_screen_resize_window);
        break;

    case GX_SIGNAL(ID_RADIO_FOOT, GX_EVENT_RADIO_SELECT):
        resize_pixelmap_id = GX_PIXELMAP_ID_ROTATE_FOOT;
        gx_system_dirty_mark((GX_WIDGET *)&resize_screen.resize_screen_resize_window);
        break;

    case GX_SIGNAL(ID_RADIO_FOOT_ALPHA, GX_EVENT_RADIO_SELECT):
        resize_pixelmap_id = GX_PIXELMAP_ID_ROTATE_APPLE;
        gx_system_dirty_mark((GX_WIDGET *)&resize_screen.resize_screen_resize_window);
        break;

    case GX_SIGNAL(ID_RADIO_FISH, GX_EVENT_RADIO_SELECT):
        resize_pixelmap_id = GX_PIXELMAP_ID_ROTATE_FISH;
        gx_system_dirty_mark((GX_WIDGET *)&resize_screen.resize_screen_resize_window);
        break;

    default:
        return gx_window_event_process(window, myevent);
    }
    return 0;
}

VOID resize_window_draw(GX_WINDOW *window)
{
    INT  xpos;
    INT  ypos;
    GX_PIXELMAP *pixelmap;
    GX_RECTANGLE win_size;
    GX_PIXELMAP destination;
    INT width, height;

    gx_window_draw((GX_WINDOW*)window);

    gx_context_pixelmap_get(resize_pixelmap_id, &pixelmap);
    win_size = resize_screen.resize_screen_resize_window.gx_widget_size;

    xpos = (win_size.gx_rectangle_right + win_size.gx_rectangle_left -pixelmap->gx_pixelmap_width) >> 1;
    ypos = (win_size.gx_rectangle_top + win_size.gx_rectangle_bottom - pixelmap->gx_pixelmap_height) >> 1;

    width = (pixelmap->gx_pixelmap_width * x_ratio / RATIO_ONE);
    height = (pixelmap->gx_pixelmap_height * y_ratio / RATIO_ONE);

    if (x_ratio == RATIO_ONE && y_ratio == RATIO_ONE)
    {
        /* Draw source pixelmap.  */
        gx_canvas_pixelmap_draw(xpos, ypos, pixelmap);
    }
    else if (gx_utility_pixelmap_resize(pixelmap, &destination, width, height) == GX_SUCCESS)
    {
        xpos += (pixelmap->gx_pixelmap_width - destination.gx_pixelmap_width) >> 1;
        ypos += (pixelmap->gx_pixelmap_height - destination.gx_pixelmap_height) >> 1;

        /* Draw resized pixelmap.  */
        gx_canvas_pixelmap_draw(xpos, ypos, &destination);

        if (destination.gx_pixelmap_data)
        {
            memory_free((VOID *)destination.gx_pixelmap_data);
        }

        if (destination.gx_pixelmap_aux_data)
        {
            memory_free((VOID *)destination.gx_pixelmap_aux_data);
        }
    }
}
