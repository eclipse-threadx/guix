/* This is a demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "all_widgets_1555xrgb_resources.h"
#include "all_widgets_1555xrgb_specifications.h"
#include "gx_system.h"

extern VOID memory_free(VOID *mem);

GX_PIXELMAP pixelmap_memory[2];
GX_PIXELMAP *png_to_1555xrgb_map = GX_NULL;
GX_PIXELMAP *jpeg_to_1555xrgb_map = GX_NULL;

GX_IMAGE_READER  image_reader;
GX_UBYTE image_reader_mode = GX_IMAGE_READER_MODE_ALPHA | GX_IMAGE_READER_MODE_COMPRESS | GX_IMAGE_READER_MODE_DITHER;
GX_UBYTE output_color_format = GX_COLOR_FORMAT_1555XRGB;
GX_BOOL  do_color_convert = GX_TRUE;

/* Convert png/jpeg 4444argb format pixelmap. */
void png_jpeg_to_1555xrgb_convert()
{
GX_PIXELMAP *pixelmap;

    png_to_1555xrgb_map = &pixelmap_memory[0];
    jpeg_to_1555xrgb_map = &pixelmap_memory[1];

    gx_widget_pixelmap_get((GX_WIDGET *)&image_convert_screen.image_convert_screen_png_to_1555xrgb_win,
        GX_PIXELMAP_ID_RED_APPLE_PNG, &pixelmap);

    gx_image_reader_create(&image_reader,
        pixelmap->gx_pixelmap_data,
        pixelmap->gx_pixelmap_data_size,
        output_color_format, image_reader_mode);

    if (gx_image_reader_start(&image_reader, png_to_1555xrgb_map) != GX_SUCCESS)
    {
        png_to_1555xrgb_map = GX_NULL;
    }

    gx_widget_pixelmap_get((GX_WIDGET *)&image_convert_screen.image_convert_screen_jpeg_to_1555xrgb_win,
        GX_PIXELMAP_ID_ICON_FOOT_JPEG, &pixelmap);

    gx_image_reader_create(&image_reader,
        pixelmap->gx_pixelmap_data,
        pixelmap->gx_pixelmap_data_size,
        output_color_format, image_reader_mode);

    if (gx_image_reader_start(&image_reader, jpeg_to_1555xrgb_map) != GX_SUCCESS)
    {
        jpeg_to_1555xrgb_map = GX_NULL;
    }
}

/* Cleanup converted pixlemap. */
void cleanup_output_pixelmaps()
{
    if (png_to_1555xrgb_map)
    {
        if (png_to_1555xrgb_map->gx_pixelmap_data)
        {
            memory_free((VOID *)png_to_1555xrgb_map->gx_pixelmap_data);
            png_to_1555xrgb_map->gx_pixelmap_data = GX_NULL;
        }

        if (png_to_1555xrgb_map->gx_pixelmap_aux_data)
        {
            memory_free((VOID *)png_to_1555xrgb_map->gx_pixelmap_aux_data);
            png_to_1555xrgb_map->gx_pixelmap_aux_data = GX_NULL;
        }
        png_to_1555xrgb_map = GX_NULL;
    }

    if (jpeg_to_1555xrgb_map)
    {
        if (jpeg_to_1555xrgb_map->gx_pixelmap_data)
        {
            memory_free((VOID *)jpeg_to_1555xrgb_map->gx_pixelmap_data);
            jpeg_to_1555xrgb_map->gx_pixelmap_data = GX_NULL;
        }

        if (jpeg_to_1555xrgb_map->gx_pixelmap_aux_data)
        {
            memory_free((VOID *)jpeg_to_1555xrgb_map->gx_pixelmap_aux_data);
            jpeg_to_1555xrgb_map->gx_pixelmap_aux_data = GX_NULL;
        }
        jpeg_to_1555xrgb_map = GX_NULL;
    }
}


void mark_dirty()
{
    gx_system_dirty_mark((GX_WIDGET *)&image_convert_screen.image_convert_screen_png_to_1555xrgb_win);
    gx_system_dirty_mark((GX_WIDGET *)&image_convert_screen.image_convert_screen_jpeg_to_1555xrgb_win);
}

void convert_start()
{
    cleanup_output_pixelmaps();
    png_jpeg_to_1555xrgb_convert();

    mark_dirty();
}

UINT image_convert_screen_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        gx_window_event_process(window, event_ptr);
GX_ENTER_CRITICAL
        png_jpeg_to_1555xrgb_convert();
GX_EXIT_CRITICAL
        break;

    case GX_EVENT_HIDE:
        gx_window_event_process(window, event_ptr);
        cleanup_output_pixelmaps();
        break;

    case GX_SIGNAL(ID_COLOR_CONVERT, GX_EVENT_TOGGLE_ON):
        do_color_convert = GX_TRUE;
        mark_dirty();
        break; 

    case GX_SIGNAL(ID_COLOR_CONVERT, GX_EVENT_TOGGLE_OFF):
        do_color_convert = GX_FALSE;
        mark_dirty();
        break;

    case GX_SIGNAL(ID_ALPHA, GX_EVENT_TOGGLE_ON):
        image_reader_mode |= GX_IMAGE_READER_MODE_ALPHA;
        convert_start();
        break;

    case GX_SIGNAL(ID_ALPHA, GX_EVENT_TOGGLE_OFF):
        image_reader_mode &= ~GX_IMAGE_READER_MODE_ALPHA;
        convert_start();
        break;

    case GX_SIGNAL(ID_COMPRESSED, GX_EVENT_TOGGLE_ON):
        image_reader_mode |= GX_IMAGE_READER_MODE_COMPRESS;
        convert_start();
        break;

    case GX_SIGNAL(ID_COMPRESSED, GX_EVENT_TOGGLE_OFF):
        image_reader_mode &= ~GX_IMAGE_READER_MODE_COMPRESS;
        convert_start();
        break;

    case GX_SIGNAL(ID_DITHER, GX_EVENT_TOGGLE_ON):
        image_reader_mode |= GX_IMAGE_READER_MODE_DITHER;
        convert_start();
        break;

    case GX_SIGNAL(ID_DITHER, GX_EVENT_TOGGLE_OFF):
        image_reader_mode &= ~GX_IMAGE_READER_MODE_DITHER;
        convert_start();
        break;

    case GX_SIGNAL(ID_ALPHAMAP, GX_EVENT_TOGGLE_ON):
        output_color_format = GX_COLOR_FORMAT_8BIT_ALPHAMAP;
        convert_start();
        break;

    case GX_SIGNAL(ID_ALPHAMAP, GX_EVENT_TOGGLE_OFF):
        output_color_format = GX_COLOR_FORMAT_1555XRGB;
        convert_start();
        break;

    default:
        return next_button_handler(window, event_ptr);
    }
    return GX_SUCCESS;
}

VOID png_to_1555xrgb_win_draw(GX_WINDOW *window)
{
    int xpos;
    int ypos;

    if (do_color_convert)
    {
        gx_widget_background_draw(window);

        if (png_to_1555xrgb_map)
        {
            xpos = window->gx_window_client.gx_rectangle_left + 10;
            ypos = window->gx_widget_clip.gx_rectangle_top + 10;

            gx_context_fill_color_set(window->gx_widget_normal_fill_color);

            gx_canvas_pixelmap_draw(xpos, ypos, png_to_1555xrgb_map);
        }
    }
    else
    {
        gx_window_draw(window);
    }
}

VOID jpeg_to_1555xrgb_win_draw(GX_WINDOW *window)
{
    int xpos;
    int ypos;

    if (do_color_convert)
    {
        gx_widget_background_draw(window);

        if (jpeg_to_1555xrgb_map)
        {
            xpos = window->gx_window_client.gx_rectangle_left + 10;
            ypos = window->gx_widget_clip.gx_rectangle_top + 10;

            gx_context_fill_color_set(window->gx_widget_normal_fill_color);

            gx_canvas_pixelmap_draw(xpos, ypos, jpeg_to_1555xrgb_map);
        }
    }
    else
    {
        gx_window_draw(window);
    }
}
