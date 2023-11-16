/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"

#include "color_convert_16bpp_resources.h"
#include "color_convert_16bpp_specifications.h"

WINDOW_CONTROL_BLOCK *pMainWin;
GX_WINDOW *pixelmap_window;
GX_WINDOW *alphamap_window;

TX_BYTE_POOL       decode_pool;

#define            SCRATCHPAD_PIXELS (DISPLAY_1_X_RESOLUTION * DISPLAY_1_Y_RESOLUTION)
GX_COLOR           scratchpad[SCRATCHPAD_PIXELS];

GX_WINDOW_ROOT    *root;
GX_IMAGE_READER    image_reader;
GX_PIXELMAP        out_pixelmap;
GX_PIXELMAP        out_alphamap;

GX_UBYTE           image_reader_mode = GX_IMAGE_READER_MODE_COMPRESS|GX_IMAGE_READER_MODE_ALPHA|GX_IMAGE_READER_MODE_DITHER;

GX_RESOURCE_ID     map_id = GX_PIXELMAP_ID_RED_APPLE_PNG;

/* Define prototypes.   */
VOID  guix_setup(void);
extern UINT win32_graphics_driver_setup_565rgb(GX_DISPLAY *display);

int main(int argc, char ** argv)
{
    tx_kernel_enter();
    return(0);
}

VOID *convert_memory_allocate(ULONG size)
{
    VOID *memptr;

    if (tx_byte_allocate(&decode_pool, &memptr, size, TX_NO_WAIT) == TX_SUCCESS)
    {
        return memptr;
    }
    return NULL;
}

void convert_memory_free(VOID *mem)
{
    tx_byte_release(mem);
}

VOID tx_application_define(void *first_unused_memory)
{

    /* create byte pool for decode to use */
    tx_byte_pool_create(&decode_pool, "scratchpad", scratchpad,
        SCRATCHPAD_PIXELS * sizeof(GX_COLOR));

    guix_setup();

    /* install our memory allocator and de-allocator */
    gx_system_memory_allocator_set(convert_memory_allocate, convert_memory_free);
}


VOID  guix_setup()
{
    /* Initialize GUIX.  */
    gx_system_initialize();

    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_565rgb,
        LANGUAGE_ENGLISH, DISPLAY_1_THEME_1, &root);

    /* create the main screen */
    gx_studio_named_widget_create("window", (GX_WIDGET *)root, (GX_WIDGET **)&pMainWin);
    pixelmap_window = (GX_WINDOW *)&pMainWin->window_pixelmap_window;
    alphamap_window = (GX_WINDOW *)&pMainWin->window_alphamap_window;

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* let GUIX run */
    gx_system_start();
}


VOID pixelmap_draw(GX_WINDOW *widget)
{
GX_PIXELMAP *pixelmap;

    gx_window_draw(pixelmap_window);

    gx_widget_pixelmap_get((GX_WIDGET *)widget, map_id, &pixelmap);

    gx_image_reader_create(&image_reader,
                            pixelmap->gx_pixelmap_data,
                            pixelmap->gx_pixelmap_data_size,
                            GX_COLOR_FORMAT_565RGB,
                            image_reader_mode);

    if (gx_image_reader_start(&image_reader, &out_pixelmap) == GX_SUCCESS)
    {
        gx_canvas_pixelmap_draw(widget->gx_widget_size.gx_rectangle_left + 10,
            widget->gx_widget_size.gx_rectangle_top + 10,
            &out_pixelmap);

        if (out_pixelmap.gx_pixelmap_data)
        {
            convert_memory_free((VOID *)out_pixelmap.gx_pixelmap_data);
            out_pixelmap.gx_pixelmap_data = GX_NULL;
        }

        if (out_pixelmap.gx_pixelmap_aux_data)
        {
            convert_memory_free((VOID *)out_pixelmap.gx_pixelmap_aux_data);
            out_pixelmap.gx_pixelmap_aux_data = GX_NULL;
        }
    }
}

VOID alphamap_draw(GX_WINDOW *widget)
{
GX_PIXELMAP *pixelmap;
GX_BRUSH    *brush;

    gx_window_draw(alphamap_window);
    if (map_id == GX_PIXELMAP_ID_ICON_FOOT_JPG)
    {
        return;
    }
    gx_context_brush_get(&brush);
    brush->gx_brush_fill_color = 0;

    gx_widget_pixelmap_get((GX_WIDGET *)widget, map_id, &pixelmap);

    gx_image_reader_create(&image_reader,
                           pixelmap->gx_pixelmap_data,
                           pixelmap->gx_pixelmap_data_size,
                           GX_COLOR_FORMAT_8BIT_ALPHAMAP,
                           image_reader_mode);
 
    if (gx_image_reader_start(&image_reader, &out_alphamap) == GX_SUCCESS)
    {
        gx_canvas_pixelmap_draw(widget->gx_widget_size.gx_rectangle_left + 10,
            widget->gx_widget_size.gx_rectangle_top + 10,
            &out_alphamap);

        if (out_alphamap.gx_pixelmap_data)
        {
            convert_memory_free((VOID *)out_alphamap.gx_pixelmap_data);
            out_alphamap.gx_pixelmap_data = GX_NULL;
        }

        if (out_alphamap.gx_pixelmap_aux_data)
        {
            convert_memory_free((VOID *)out_alphamap.gx_pixelmap_aux_data);
            out_alphamap.gx_pixelmap_aux_data = GX_NULL;
        }
    }
}


UINT main_event_handler(GX_WINDOW *window, GX_EVENT *myevent)
{
    UINT status = 0;

    switch (myevent->gx_event_type)
    {
    case GX_SIGNAL(ID_COMPRESS_CHECK, GX_EVENT_TOGGLE_ON):
        image_reader_mode |= GX_IMAGE_READER_MODE_COMPRESS;
        gx_system_dirty_mark(pixelmap_window);
        gx_system_dirty_mark(alphamap_window);
        break;

    case GX_SIGNAL(ID_COMPRESS_CHECK, GX_EVENT_TOGGLE_OFF):
        image_reader_mode &= ~GX_IMAGE_READER_MODE_COMPRESS;
        gx_system_dirty_mark(pixelmap_window);
        gx_system_dirty_mark(alphamap_window);
        break;

    case GX_SIGNAL(ID_ALPHA_CHECK, GX_EVENT_TOGGLE_ON):
        image_reader_mode |= GX_IMAGE_READER_MODE_ALPHA;
        gx_system_dirty_mark(pixelmap_window);
        gx_system_dirty_mark(alphamap_window);
        break;

    case GX_SIGNAL(ID_ALPHA_CHECK, GX_EVENT_TOGGLE_OFF):
        image_reader_mode &= ~GX_IMAGE_READER_MODE_ALPHA;
        gx_system_dirty_mark(pixelmap_window);
        gx_system_dirty_mark(alphamap_window);
        break;

    case GX_SIGNAL(ID_DITHER_CHECK, GX_EVENT_TOGGLE_ON):
        image_reader_mode |= GX_IMAGE_READER_MODE_DITHER;
        gx_system_dirty_mark(pixelmap_window);
        gx_system_dirty_mark(alphamap_window);
        break;

    case GX_SIGNAL(ID_DITHER_CHECK, GX_EVENT_TOGGLE_OFF):
        image_reader_mode &= ~GX_IMAGE_READER_MODE_DITHER;
        gx_system_dirty_mark(pixelmap_window);
        gx_system_dirty_mark(alphamap_window);
        break;

    case GX_SIGNAL(ID_JPG_RADIO_BTN, GX_EVENT_RADIO_SELECT):
        map_id = GX_PIXELMAP_ID_ICON_FOOT_JPG;
        gx_system_dirty_mark(pixelmap_window);
        gx_system_dirty_mark(alphamap_window);
        break;

    case GX_SIGNAL(ID_PNG_RADIO_BTN, GX_EVENT_RADIO_SELECT):
        map_id = GX_PIXELMAP_ID_RED_APPLE_PNG;
        gx_system_dirty_mark(pixelmap_window);
        gx_system_dirty_mark(alphamap_window);
        break;

    default:
        status = gx_window_event_process(window, myevent);
        break;
    }
    return status;
}
