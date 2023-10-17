/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "all_widgets_4bpp_resources.h"
#include "all_widgets_4bpp_specifications.h"

extern VOID memory_free(VOID *mem);

GX_PIXELMAP pixelmap_memory[2];
GX_PIXELMAP *png_to_4bit_gray_map = GX_NULL;
GX_PIXELMAP *jpeg_to_4bit_gray_map = GX_NULL;

GX_IMAGE_READER  image_reader;
GX_UBYTE image_reader_mode = GX_IMAGE_READER_MODE_ALPHA | GX_IMAGE_READER_MODE_COMPRESS | GX_IMAGE_READER_MODE_DITHER;

/* Convert png/jpeg 4444argb format pixelmap. */
void png_jpeg_to_4bit_gray_convert()
{
GX_PIXELMAP *pixelmap;

    png_to_4bit_gray_map = &pixelmap_memory[0];
    jpeg_to_4bit_gray_map = &pixelmap_memory[1];

    gx_widget_pixelmap_get((GX_WIDGET *)&image_convert_screen.image_convert_screen_png_to_4bit_gray_win,
        GX_PIXELMAP_ID_RED_APPLE_PNG, &pixelmap);

    gx_image_reader_create(&image_reader,
        pixelmap->gx_pixelmap_data,
        pixelmap->gx_pixelmap_data_size,
        GX_COLOR_FORMAT_4BIT_GRAY, image_reader_mode);

    if (gx_image_reader_start(&image_reader, png_to_4bit_gray_map) != GX_SUCCESS)
    {
        png_to_4bit_gray_map = GX_NULL;
    }

    gx_widget_pixelmap_get((GX_WIDGET *)&image_convert_screen.image_convert_screen_jpeg_to_4bit_gray_win,
        GX_PIXELMAP_ID_ICON_FOOT_JPEG, &pixelmap);

    gx_image_reader_create(&image_reader,
        pixelmap->gx_pixelmap_data,
        pixelmap->gx_pixelmap_data_size,
        GX_COLOR_FORMAT_4BIT_GRAY, image_reader_mode);

    if (gx_image_reader_start(&image_reader, jpeg_to_4bit_gray_map) != GX_SUCCESS)
    {
        jpeg_to_4bit_gray_map = GX_NULL;
    }
}

/* Cleanup converted pixlemap. */
void cleanup_output_pixelmaps()
{
    if (png_to_4bit_gray_map)
    {
        if (png_to_4bit_gray_map->gx_pixelmap_data)
        {
            memory_free((VOID *)png_to_4bit_gray_map->gx_pixelmap_data);
            png_to_4bit_gray_map->gx_pixelmap_data = GX_NULL;
        }

        if (png_to_4bit_gray_map->gx_pixelmap_aux_data)
        {
            memory_free((VOID *)png_to_4bit_gray_map->gx_pixelmap_aux_data);
            png_to_4bit_gray_map->gx_pixelmap_aux_data = GX_NULL;
        }
        png_to_4bit_gray_map = GX_NULL;
    }

    if (jpeg_to_4bit_gray_map)
    {
        if (jpeg_to_4bit_gray_map->gx_pixelmap_data)
        {
            memory_free((VOID *)jpeg_to_4bit_gray_map->gx_pixelmap_data);
            jpeg_to_4bit_gray_map->gx_pixelmap_data = GX_NULL;
        }

        if (jpeg_to_4bit_gray_map->gx_pixelmap_aux_data)
        {
            memory_free((VOID *)jpeg_to_4bit_gray_map->gx_pixelmap_aux_data);
            jpeg_to_4bit_gray_map->gx_pixelmap_aux_data = GX_NULL;
        }
        jpeg_to_4bit_gray_map = GX_NULL;
    }
}

void convert_start()
{
    cleanup_output_pixelmaps();
    png_jpeg_to_4bit_gray_convert();
    gx_system_dirty_mark((GX_WIDGET *)&image_convert_screen.image_convert_screen_png_to_4bit_gray_win);
    gx_system_dirty_mark((GX_WIDGET *)&image_convert_screen.image_convert_screen_jpeg_to_4bit_gray_win);
}

UINT image_convert_screen_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        gx_window_event_process(window, event_ptr);
        png_jpeg_to_4bit_gray_convert();
        break;

    case GX_EVENT_HIDE:
        gx_window_event_process(window, event_ptr);
        cleanup_output_pixelmaps();
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

    default:
        return next_button_handler(window, event_ptr);
    }

    return GX_SUCCESS;
}

VOID png_to_4bit_gray_win_draw(GX_WINDOW *window)
{
    int xpos;
    int ypos;

    gx_widget_background_draw(window);

    if (png_to_4bit_gray_map)
    {
        xpos = window->gx_window_client.gx_rectangle_left + 10;
        ypos = window->gx_widget_clip.gx_rectangle_top + 10;

        gx_canvas_pixelmap_draw(xpos, ypos, png_to_4bit_gray_map);
    }
}

VOID jpeg_to_4bit_gray_win_draw(GX_WINDOW *window)
{
    int xpos;
    int ypos;

    gx_widget_background_draw(window);

    if (jpeg_to_4bit_gray_map)
    {
        xpos = window->gx_window_client.gx_rectangle_left + 10;
        ypos = window->gx_widget_clip.gx_rectangle_top + 10;

        gx_canvas_pixelmap_draw(xpos, ypos, jpeg_to_4bit_gray_map);
    }
}
