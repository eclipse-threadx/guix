/* This is a small demo of the high-performance GUIX graphics framework. */

#include "demo_guix_smart_watch.h"

VOID map_decode();
VOID map_delete();
VOID map_resize();

#define MAX_SCALE 4
#define MIN_SCALE 1
FILE   *p_file = GX_NULL;
GX_PIXELMAP decoded_map;
GX_PIXELMAP resized_map;
INT     resize_scale = 1;

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

/*************************************************************************************/
VOID map_decode()
{
    UINT filesize;
    GX_IMAGE_READER image_reader;
    GX_CHAR *jpg_data;
    

    if (file_open("..\\graphics\\map\\map.jpg") == GX_SUCCESS)
    {
        filesize = file_size_get();

        jpg_data = memory_allocate(filesize);
        file_read(0, filesize, jpg_data);
        gx_image_reader_create(&image_reader, jpg_data, filesize, GX_COLOR_FORMAT_24XRGB, GX_IMAGE_READER_MODE_ALPHA);
        gx_image_reader_start(&image_reader, &decoded_map);

        if (jpg_data)
        {
            memory_free(jpg_data);
            jpg_data = GX_NULL;
        }
        file_close();
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

    if (decoded_map.gx_pixelmap_data)
    {
        /* Draw map. */
        if (resize_scale == 1)
        {
            xpos = (240 - decoded_map.gx_pixelmap_width) >> 1;
            ypos = (320 - decoded_map.gx_pixelmap_height) >> 1;

            gx_canvas_pixelmap_draw(xpos + window->gx_widget_size.gx_rectangle_left,
                ypos + window->gx_widget_size.gx_rectangle_top, &decoded_map);
        }
        else
        {
            xpos = (240 - resized_map.gx_pixelmap_width) >> 1;
            ypos = (320 - resized_map.gx_pixelmap_height) >> 1;
            gx_canvas_pixelmap_draw(xpos + window->gx_widget_size.gx_rectangle_left,
                ypos + window->gx_widget_size.gx_rectangle_top, &resized_map);
        }
    }

    gx_widget_children_draw((GX_WIDGET *)window);
}