/* This is a small demo of the high-performance GUIX graphics framework. */

#include "demo_guix_car_infotainment.h"

#define MAX_SCALE 290
#define MIN_SCALE 200

/* Define variables. */
INT         resize_scale = 255;/* Resizse_scale is larged by 255 for precision needs. */

GX_RECTANGLE map_area_list[] = {
    { 0, 54, 319, 239 },/* Left top area. */
    { 320, 54, 639, 239 },/* Right top area. */
    { 0, 240, 319, 425 },/* Left bottom area. */
    { 320, 240, 639, 425 }/* Right bottom area. */
};

INT map_offset_x = 0;
INT map_offset_y = 0;
INT min_offset_x = 0;
INT min_offset_y = 0;

/* Define prototypes. */
VOID map_decode(GX_RESOURCE_ID map_id);
VOID map_resize();
VOID map_move(GX_POINT *point);

/******************************************************************************************/
/* Override default drawing of "navigation_screen" to do custom drawing.                  */
/******************************************************************************************/
VOID navigation_screen_draw(GX_WINDOW *window)
{
INT           xpos;
INT           ypos;
GX_RECTANGLE *size = &window->gx_widget_size;
GX_PIXELMAP  *pixelmap;

    if (decoded_map.gx_pixelmap_data)
    {
        if (resize_scale == 255)
        {
            /* Draw decoded jpg image. */
            gx_canvas_pixelmap_draw(size->gx_rectangle_left + map_offset_x,
                size->gx_rectangle_top + map_offset_y, &decoded_map);
        }
        else if (resized_map.gx_pixelmap_data)
        {
            /* Draw resized imgae. */
            gx_canvas_pixelmap_draw(size->gx_rectangle_left + map_offset_x,
                size->gx_rectangle_top + map_offset_y, &resized_map);
        }
    }

    /* Pick window filling pixelmap. */
    gx_context_pixelmap_get(GX_PIXELMAP_ID_TRANSPARENT_DOT_50, &pixelmap);

    if (pixelmap)
    {
        /* Tile filling pixelmap. */
        for (ypos = size->gx_rectangle_top; ypos < 57; ypos+=pixelmap->gx_pixelmap_height)
        {
            for (xpos = size->gx_rectangle_left; xpos < size->gx_rectangle_right; xpos+=pixelmap->gx_pixelmap_width)
            {
                gx_canvas_pixelmap_draw(xpos, ypos, pixelmap);
            }
        }
    }

    /* Call default window draw. */
    gx_window_draw(window);
}

/******************************************************************************************/
/* Override default event processing of "navigation_screen" to handle signal from my child*/
/* widgets.                                                                               */
/******************************************************************************************/
UINT navigation_screen_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        /* Call base screen event process. */
        base_screen_event_process(window, event_ptr);

        resize_scale = 255;
        gx_icon_pixelmap_set(&navigation_screen.navigation_screen_globe_active_icon, GX_NULL, GX_NULL);
        gx_icon_pixelmap_set(&navigation_screen.navigation_screen_traffic_active_icon, GX_NULL, GX_NULL);

        /* Decode map jpg. */
        map_decode(GX_PIXELMAP_ID_MAP_NORMAL);

        /* Calcualte min/max draw offset value. */
        min_offset_x = MAIN_DISPLAY_X_RESOLUTION - decoded_map.gx_pixelmap_width;
        min_offset_y = MAIN_DISPLAY_Y_RESOLUTION - decoded_map.gx_pixelmap_height;

        /* Set draw offset value to center map image. */
        map_offset_x = min_offset_x >> 1;
        map_offset_y = min_offset_y >> 1;
        break;

    case GX_EVENT_HIDE:
        /* Call base screen event process. */
        base_screen_event_process(window, event_ptr);
        break;

    case GX_SIGNAL(ID_BUTTON_PLUS, GX_EVENT_CLICKED):
        if (resize_scale + 10 <= MAX_SCALE)
        {
            /* Increase resize scale. */
            resize_scale += 10;

            /* Resize current map. */
            map_resize();
        }
        break;

    case GX_SIGNAL(ID_BUTTON_MINUS, GX_EVENT_CLICKED):
        if (resize_scale - 10 > MIN_SCALE)
        {
            /* Decrease resize scale. */
            resize_scale -= 10;

            /* Resize current map. */
            map_resize();
        }
        break;

    case GX_EVENT_PEN_DOWN:
        /* Move current map according to click area. */
        map_move(&event_ptr->gx_event_payload.gx_event_pointdata);

        /* Call base screen event process. */
        base_screen_event_process(window, event_ptr);
        break;

    case GX_SIGNAL(ID_BUTTON_LOCATION, GX_EVENT_CLICKED):
        /* Reset draw offset to center the map. */
        map_offset_x = min_offset_x >> 1;
        map_offset_y = min_offset_y >> 1;

        /* Mark map window as dirty. */
        gx_system_dirty_mark(&navigation_screen);
        break;

    case GX_SIGNAL(ID_BUTTON_GLOBE, GX_EVENT_CLICKED):
        if (navigation_screen.navigation_screen_globe_active_icon.gx_icon_normal_pixelmap == 0)
        {
            gx_icon_pixelmap_set(&navigation_screen.navigation_screen_globe_active_icon, GX_PIXELMAP_ID_ARROW_ACTIVE, GX_NULL);

            if (navigation_screen.navigation_screen_traffic_active_icon.gx_icon_normal_pixelmap == 0)
            {
                /* Decode globe map. */
                map_decode(GX_PIXELMAP_ID_MAP_GLOBE);
            }
            else
            {
                /* Decode globe traffic map. */
                map_decode(GX_PIXELMAP_ID_MAP_GLOBE_TRAFFIC);
            }
        }
        else
        {
            gx_icon_pixelmap_set(&navigation_screen.navigation_screen_globe_active_icon, GX_NULL, GX_NULL);

            if (navigation_screen.navigation_screen_traffic_active_icon.gx_icon_normal_pixelmap == 0)
            {
                /* Decode normal map. */
                map_decode(GX_PIXELMAP_ID_MAP_NORMAL);
            }
            else
            {
                /* Decode traffic map. */
                map_decode(GX_PIXELMAP_ID_MAP_TRAFFIC);
            }
        }
        gx_system_dirty_mark(&navigation_screen);
        break;

    case GX_SIGNAL(ID_BUTTON_TRAFFIC, GX_EVENT_CLICKED):
        if (navigation_screen.navigation_screen_traffic_active_icon.gx_icon_normal_pixelmap == 0)
        {
            gx_icon_pixelmap_set(&navigation_screen.navigation_screen_traffic_active_icon, GX_PIXELMAP_ID_ARROW_ACTIVE, GX_NULL);

            if (navigation_screen.navigation_screen_globe_active_icon.gx_icon_normal_pixelmap == 0)
            {
                /* Decode traffic map. */
                map_decode(GX_PIXELMAP_ID_MAP_TRAFFIC);
            }
            else
            {
                /* Decode globe traffic map. */
                map_decode(GX_PIXELMAP_ID_MAP_GLOBE_TRAFFIC);
            }
        }
        else
        {
            gx_icon_pixelmap_set(&navigation_screen.navigation_screen_traffic_active_icon, GX_NULL, GX_NULL);

            if (navigation_screen.navigation_screen_globe_active_icon.gx_icon_normal_pixelmap == 0)
            {
                /* Decode normal map. */
                map_decode(GX_PIXELMAP_ID_MAP_NORMAL);
            }
            else
            {
                /* Decode globe map. */
                map_decode(GX_PIXELMAP_ID_MAP_GLOBE);
            }
        }
        gx_system_dirty_mark(&navigation_screen);
        break;

    default:
        return base_screen_event_process(window, event_ptr);
    }

    return 0;
}

/******************************************************************************************/
/* Resize decoded pixelmap with current resize scale.                                     */
/******************************************************************************************/
VOID map_resize()
{
INT newwidth;
INT newheight;

    if (resized_map.gx_pixelmap_data)
    {
        memory_free((void *)resized_map.gx_pixelmap_data);
        resized_map.gx_pixelmap_data = GX_NULL;
    }

    if (resized_map.gx_pixelmap_aux_data)
    {
        memory_free((void *)resized_map.gx_pixelmap_aux_data);
        resized_map.gx_pixelmap_aux_data = GX_NULL;
    }

    if (resize_scale != 255)
    {
        /* Calculate width and height of resized pixelmap. */
        newwidth = (decoded_map.gx_pixelmap_width * resize_scale) >> 8;
        newheight = (decoded_map.gx_pixelmap_height * resize_scale) >> 8;

        /* Resize pixelmap. */
        gx_utility_pixelmap_resize(&decoded_map, &resized_map, newwidth, newheight);

        /* Calcualte min/max draw offset value. */
        min_offset_x = MAIN_DISPLAY_X_RESOLUTION - resized_map.gx_pixelmap_width;
        min_offset_y = MAIN_DISPLAY_Y_RESOLUTION - resized_map.gx_pixelmap_height;
    }
    else
    {
        min_offset_x = MAIN_DISPLAY_X_RESOLUTION - decoded_map.gx_pixelmap_width;
        min_offset_y = MAIN_DISPLAY_Y_RESOLUTION - decoded_map.gx_pixelmap_height;
    }

    /* Reset draw offset value of center the current map. */
    map_offset_x = min_offset_x >> 1;
    map_offset_y = min_offset_y >> 1;

    gx_system_dirty_mark(&navigation_screen);
}

/*******************************************************************************************/
/* Decode a map.                                                                           */
/*******************************************************************************************/
VOID map_decode(GX_RESOURCE_ID map_id)
{
    map_delete(&resized_map);

    jpg_decode(map_id, &decoded_map);

    if (resize_scale != 255)
    {
        map_resize();
    }
}

/******************************************************************************************/
/* Move the current map by setting its draw offset value.                                 */
/******************************************************************************************/
VOID map_move(GX_POINT *point)
{
INT index;

    /* Check which area the click point is belong to. */
    for (index = 0; index < 4; index++)
    {
        if (gx_utility_rectangle_point_detect(&map_area_list[index], *point))
        {
            break;
        }
    }

    switch (index)
    {
    case 0:
        /* The click point is in left top area, move map right down. */
        map_offset_x += 5;
        map_offset_y += 5;
        break;
    case 1:
        /* The click point is in right top area, move map left down. */
        map_offset_x -= 5;
        map_offset_y += 5;
        break;
    case 2:
        /* The click point is in left bottom area, move map right top. */
        map_offset_x += 5;
        map_offset_y -= 5;
        break;
    case 3:
        /* The click point is in right bottom area, move map left top. */
        map_offset_x -= 5;
        map_offset_y -= 5;
        break;
    }

    /* Check if the draw offset value is between valid range. */
    if (map_offset_x > 0)
    {
        map_offset_x = 0;
    }
    else if (map_offset_x < min_offset_x)
    {
        map_offset_x = min_offset_x;
    }

    if (map_offset_y > 0)
    {
        map_offset_y = 0;
    }
    else if (map_offset_y < min_offset_y)
    {
        map_offset_y = min_offset_y;
    }
}