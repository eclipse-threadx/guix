/* This is a small demo of the high-performance GUIX graphics framework. */

#include "demo_guix_smart_watch.h"

VOID map_decode();
VOID map_delete();
VOID map_resize();

#define MAX_SCALE 4
#define MIN_SCALE 1

#if defined(GX_TARGET_WIN32)
FILE   *p_file = GX_NULL;
#endif

GX_PIXELMAP decoded_map;
GX_PIXELMAP resized_map;
INT     resize_scale = 1;

extern GX_CONST GX_PIXELMAP *Main_Screen_theme_default_pixelmap_table[];

/*************************************************************************************/
UINT map_screen_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        memset(&decoded_map, 0, sizeof(GX_PIXELMAP));
        memset(&resized_map, 0, sizeof(GX_PIXELMAP));
        map_decode();
        gx_window_event_process(window, event_ptr);
        break;

    case GX_EVENT_HIDE:
        map_delete();
        gx_window_event_process(window, event_ptr);
        break;

    case GX_SIGNAL(ID_ZOOM_IN, GX_EVENT_CLICKED):
        if (resize_scale < MAX_SCALE)
        {
            resize_scale++;
            map_resize();
        }
        break;

    case GX_SIGNAL(ID_ZOOM_OUT, GX_EVENT_CLICKED):
        if (resize_scale > MIN_SCALE)
        {
            resize_scale--;
            map_resize();
        }
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}

#if defined(GX_PIXELMAP_ID_ROAD_MAP)
/* map image is stored in resource file */
#else
/* Map image is not stored in resource file */
/* Read the map image from file system     */

#if defined(GX_TARGET_WIN32)
/*************************************************************************************/
UINT file_open(CHAR *filename)
{
    fopen_s(&p_file, filename, "rb");

    if (p_file)
    {
        return GX_SUCCESS;
    }

    return GX_FAILURE;
}

/*************************************************************************************/
UINT file_read(UINT offset, INT read_size, GX_BYTE *put)
{
    INT result;

    if (p_file)
    {
        fseek(p_file, offset, SEEK_SET);

        result = fread(put, 1, read_size, p_file);
    }

    if (result != read_size)
    {
        return GX_FAILURE;
    }

    return GX_SUCCESS;
}

/*************************************************************************************/
UINT file_size_get()
{
    if (p_file)
    {
        fseek(p_file, 0, SEEK_END);
        return ftell(p_file);
    }
    return 0;
}

/*************************************************************************************/
UINT file_close()
{
    if (p_file)
    {
        fclose(p_file);
    }

    return GX_SUCCESS;
}

#else
/* Not running on Win32                 */
/* Insert target-specific file I/O here */
#endif
#endif

/*************************************************************************************/
VOID map_decode()
{
    GX_IMAGE_READER image_reader;
    GX_CONST GX_UBYTE *jpg_data = GX_NULL;
    GX_CHAR *file_data = GX_NULL;
    UINT jpg_data_size = 0;

#if defined(GX_PIXELMAP_ID_ROAD_MAP)
    /* map image is stored in resource file */
    jpg_data = Main_Screen_theme_default_pixelmap_table[GX_PIXELMAP_ID_ROAD_MAP]->gx_pixelmap_data;
    jpg_data_size = Main_Screen_theme_default_pixelmap_table[GX_PIXELMAP_ID_ROAD_MAP]->gx_pixelmap_data_size;
#else
    /* Map image is not stored in resource file */
    /* Read the map image from file system     */
#if defined(GX_TARGET_WIN32)
    if (file_open(ROAD_MAP_JPG_PATH) == GX_SUCCESS)
    {
        jpg_data_size = file_size_get();

        file_data = memory_allocate(jpg_data_size);
        file_read(0, jpg_data_size, file_data);

        jpg_data = file_data;
        file_close();
    }
#else
    /* Not running on Win32                    */
    /* Insert target-specific file I/O here    */
#endif
#endif
    if (jpg_data)
    {
        gx_image_reader_create(&image_reader, jpg_data, jpg_data_size, GX_COLOR_FORMAT_24XRGB, GX_IMAGE_READER_MODE_ALPHA);
        gx_image_reader_start(&image_reader, &decoded_map);
    }

    if (file_data)
    {
        memory_free((void *)file_data);
        file_data = GX_NULL;
    }
}

/*************************************************************************************/
VOID map_delete()
{
    if (decoded_map.gx_pixelmap_data)
    {
        memory_free((void *)decoded_map.gx_pixelmap_data);
        decoded_map.gx_pixelmap_data = GX_NULL;
    }

    if (decoded_map.gx_pixelmap_aux_data)
    {
        memory_free((void *)decoded_map.gx_pixelmap_aux_data);
        decoded_map.gx_pixelmap_aux_data = GX_NULL;
    }

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
}

/*************************************************************************************/
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

    newwidth = (decoded_map.gx_pixelmap_width * (4 + resize_scale)) >> 2;
    newheight = (decoded_map.gx_pixelmap_height * (4 + resize_scale)) >> 2;

    gx_utility_pixelmap_resize(&decoded_map, &resized_map, newwidth, newheight);
    gx_system_dirty_mark(&map_screen);
}

/*************************************************************************************/
VOID map_screen_draw(GX_WINDOW *window)
{
    INT xpos;
    INT ypos;
    GX_PIXELMAP *map;

    if (decoded_map.gx_pixelmap_data)
    {
        /* Draw map. */
        if (resize_scale == 1)
        {
            xpos = (275 - decoded_map.gx_pixelmap_width) >> 1;
            ypos = (313 - decoded_map.gx_pixelmap_height) >> 1;

            gx_canvas_pixelmap_draw(xpos + window->gx_widget_size.gx_rectangle_left,
                ypos + window->gx_widget_size.gx_rectangle_top, &decoded_map);
        }
        else
        {
            xpos = (275 - resized_map.gx_pixelmap_width) >> 1;
            ypos = (313 - resized_map.gx_pixelmap_height) >> 1;
            gx_canvas_pixelmap_draw(xpos + window->gx_widget_size.gx_rectangle_left,
                ypos + window->gx_widget_size.gx_rectangle_top, &resized_map);
        }
    }

    gx_context_pixelmap_get(GX_PIXELMAP_ID_COVER_TOP_LEFT, &map);

    if (map)
    {
        /* Draw top left cover pixelmap to make a round corner. */
        gx_canvas_pixelmap_draw(window->gx_widget_size.gx_rectangle_left,
            window->gx_widget_size.gx_rectangle_top, map);
    }

    gx_context_pixelmap_get(GX_PIXELMAP_ID_COVER_TOP_RIGHT, &map);

    if (map)
    {
        /* Draw top right cover pixelmap to make a round corner. */
        gx_canvas_pixelmap_draw(window->gx_widget_size.gx_rectangle_right - map->gx_pixelmap_width + 1,
            window->gx_widget_size.gx_rectangle_top, map);
    }

    gx_context_pixelmap_get(GX_PIXELMAP_ID_COVER_BOTTOM_LEFT, &map);

    if (map)
    {
        /* Draw bottom left cover pixelmap to make a round corner. */
        gx_canvas_pixelmap_draw(window->gx_widget_size.gx_rectangle_left,
            window->gx_widget_size.gx_rectangle_bottom - map->gx_pixelmap_height + 1, map);
    }

    gx_context_pixelmap_get(GX_PIXELMAP_ID_COVER_BOTTOM_RIGHT, &map);

    if (map)
    {
        /* Draw bottom right cover pixelmap to make a round corner. */
        gx_canvas_pixelmap_draw(window->gx_widget_size.gx_rectangle_right - map->gx_pixelmap_width + 1,
            window->gx_widget_size.gx_rectangle_bottom - map->gx_pixelmap_height + 1, map);
    }

    gx_widget_children_draw((GX_WIDGET *)window);
}