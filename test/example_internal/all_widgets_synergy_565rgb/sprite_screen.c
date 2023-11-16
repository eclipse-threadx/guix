/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "all_widgets_synergy_565rgb_resources.h"
#include "all_widgets_synergy_565rgb_specifications.h"

GX_UBYTE          alpha_value = 255;

VOID rotate_memory_free(VOID *mem);

UINT sprite_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_ALPHA_SLIDER, GX_EVENT_SLIDER_VALUE):
        alpha_value = (GX_UBYTE)event_ptr->gx_event_payload.gx_event_longdata;
        gx_system_dirty_mark(&sprite_screen.sprite_screen_apple_window);
        break;

    case GX_SIGNAL(ID_BIRD_SPRITE, GX_EVENT_SPRITE_COMPLETE):
        gx_sprite_start(&sprite_screen.sprite_screen_sprite_1, 0);
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}

VOID apple_window_draw(GX_WINDOW *window)
{
GX_PIXELMAP *map;
GX_PIXELMAP scaled_map;
int         width;
int         height;
int         xpos;
int         ypos;

    gx_context_pixelmap_get(GX_PIXELMAP_ID_RED_APPLE, &map);

    width = (alpha_value * map->gx_pixelmap_width) >> 8;
    height = (alpha_value * map->gx_pixelmap_height) >> 8;

    if ((width == 0) || (height == 0))
    {
        return;
    }

    if (gx_utility_pixelmap_resize(map, &scaled_map, width, height) == GX_SUCCESS)
    {
        xpos = window->gx_widget_size.gx_rectangle_left;
        ypos = window->gx_widget_size.gx_rectangle_top;

        xpos += (map->gx_pixelmap_width - width) >> 1;
        ypos += (map->gx_pixelmap_height - height) >> 1;

        gx_canvas_pixelmap_blend((GX_VALUE)xpos, (GX_VALUE)ypos, &scaled_map, alpha_value);

        if (scaled_map.gx_pixelmap_data)
        {
            rotate_memory_free((VOID *)scaled_map.gx_pixelmap_data);
        }
    }
}
