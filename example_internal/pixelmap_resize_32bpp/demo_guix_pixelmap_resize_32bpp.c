/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "gx_canvas.h"

#include "pixelmap_resize_32bpp_resources.h"
#include "pixelmap_resize_32bpp_specifications.h"

    
extern GX_CONST GX_PIXELMAP *display_1_theme_1_pixelmap_table[];

WINDOW_CONTROL_BLOCK *pMainWin;
GX_WINDOW      *pPixelmapWin;
float            x_ratio = 1;
float            y_ratio = 1;
GX_RESOURCE_ID pixelmap_id = GX_PIXELMAP_ID_ICON_FOOT;


/* Define the ThreadX demo thread control block and stack.  */

TX_THREAD          demo_thread;
UCHAR              demo_thread_stack[4096];
TX_BYTE_POOL       resize_pool;

#define            SCRATCHPAD_PIXELS (DISPLAY_1_X_RESOLUTION * DISPLAY_1_Y_RESOLUTION)
GX_COLOR           scratchpad[SCRATCHPAD_PIXELS];


GX_WINDOW_ROOT    *root;

/* Define prototypes.   */
VOID  guix_setup(void);
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);

int main(int argc, char ** argv)
{
  tx_kernel_enter();
  return(0);
}

VOID *resize_memory_allocate(ULONG size)
{
    VOID *memptr;

    if (tx_byte_allocate(&resize_pool, &memptr, size, TX_NO_WAIT) == TX_SUCCESS)
    {
        return memptr;
    }
    return NULL;
}

void resize_memory_free(VOID *mem)
{
    tx_byte_release(mem);
}

VOID tx_application_define(void *first_unused_memory)
{

    /* create byte pool for resize to use */
    tx_byte_pool_create(&resize_pool, "scratchpad", scratchpad, 
                        SCRATCHPAD_PIXELS * sizeof(GX_COLOR));

    guix_setup();

    /* install our memory allocator and de-allocator */
    gx_system_memory_allocator_set(resize_memory_allocate, resize_memory_free);
}


VOID  guix_setup()
{
    /* Initialize GUIX.  */
    gx_system_initialize();

    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_24xrgb, 
                                LANGUAGE_ENGLISH, DISPLAY_1_THEME_1, &root);

    /* create the main screen */
    gx_studio_named_widget_create("window", (GX_WIDGET *) root, (GX_WIDGET **) &pMainWin);

    /* get a pointer to polygon window */
    pPixelmapWin = &pMainWin -> window_pixelmap_window;

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* let GUIX run */
    gx_system_start();
}

VOID update_prompt_text(GX_RESOURCE_ID id, float value)
{
GX_PROMPT *pp;
static GX_CHAR text[10];
GX_STRING str;

    gx_widget_find(pMainWin, (USHORT)id, 0, &pp);

    if(pp)
    {
        /* Transfer float value to string. */
        text[0] = '0' + (int)(value);
        text[1] = '.';
        text[2] = '0' + (int)(value * 10) % 10;
        text[3] = '0' + (int)(value * 100) % 10;
        text[4] = '\0';

        /* Reset prompt text. */
        str.gx_string_ptr = text;
        str.gx_string_length = 4;
        gx_prompt_text_set_ext(pp, &str);
    }
}

UINT main_event_handler(GX_WINDOW *window, GX_EVENT *myevent)
{
UINT status = 0;
INT  value;

    switch(myevent -> gx_event_type)
    {
    case GX_SIGNAL(ID_SLIDER_WIDTH, GX_EVENT_SLIDER_VALUE):
        value = myevent ->gx_event_payload.gx_event_longdata;
        value -= 50;
        x_ratio = 1 + (float)value/100;
        update_prompt_text(ID_WIDTH_DISPLAY, x_ratio);
        gx_system_dirty_mark(pPixelmapWin);
        break;

    case GX_SIGNAL(ID_SLIDER_HEIGHT, GX_EVENT_SLIDER_VALUE):
        value = myevent ->gx_event_payload.gx_event_longdata;
        value -= 50;
        y_ratio = 1 + (float)value/100;
        update_prompt_text(ID_HEIGHT_DISPLAY, y_ratio);
        gx_system_dirty_mark(pPixelmapWin);
        break;

    case GX_SIGNAL(ID_RADIO_FOOT, GX_EVENT_RADIO_SELECT):
        pixelmap_id = GX_PIXELMAP_ID_ICON_FOOT;
        gx_system_dirty_mark(pPixelmapWin);
        break;

    case GX_SIGNAL(ID_RADIO_FOOT_ALPHA, GX_EVENT_RADIO_SELECT):
        pixelmap_id = GX_PIXELMAP_ID_RED_APPLE;
        gx_system_dirty_mark(pPixelmapWin);
        break;

    case GX_SIGNAL(ID_RADIO_FISH, GX_EVENT_RADIO_SELECT):
        pixelmap_id = GX_PIXELMAP_ID_FISH;
        gx_system_dirty_mark(pPixelmapWin);
        break;

    default:
        status = gx_window_event_process(window, myevent);
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
INT width, height;

    gx_window_draw((GX_WINDOW*) window);

    gx_context_pixelmap_get(pixelmap_id, &pixelmap);
    win_size = pPixelmapWin->gx_widget_size;

    xpos = ((win_size.gx_rectangle_right + win_size.gx_rectangle_left) >> 1) - pixelmap->gx_pixelmap_width - 30;
    ypos = (win_size.gx_rectangle_top + win_size.gx_rectangle_bottom - pixelmap->gx_pixelmap_height) >> 1;

    width = (int)(pixelmap->gx_pixelmap_width * x_ratio);
    height = (int)(pixelmap->gx_pixelmap_height * y_ratio);

    if(gx_utility_pixelmap_resize(pixelmap, &destination, width, height) == GX_SUCCESS)
    {
        /* Draw source pixelmap.  */
        gx_canvas_pixelmap_draw(xpos, ypos, pixelmap);

        xpos = ((win_size.gx_rectangle_right + win_size.gx_rectangle_left) >> 1) + 30;

        xpos += (pixelmap->gx_pixelmap_width - destination.gx_pixelmap_width) >> 1;
        ypos += (pixelmap->gx_pixelmap_height - destination.gx_pixelmap_height) >> 1;

        /* Draw resized pixelmap.  */
        gx_canvas_pixelmap_draw(xpos, ypos, &destination);

        if(destination.gx_pixelmap_data)
        {
            resize_memory_free((VOID *)destination.gx_pixelmap_data);
        }
    }
}
