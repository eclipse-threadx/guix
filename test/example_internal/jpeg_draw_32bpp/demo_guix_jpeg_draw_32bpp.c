/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"

#include "jpeg_draw_32bpp_resources.h"
#include "jpeg_draw_32bpp_specifications.h"

MAIN_WINDOW_CONTROL_BLOCK *pMainWin;
GX_WINDOW                 *pic_window;

/* Define the ThreadX demo thread control block and stack.  */

TX_THREAD          demo_thread;
UCHAR              demo_thread_stack[4096];
TX_BYTE_POOL       decode_pool;

#define            SCRATCHPAD_PIXELS (DISPLAY_1_X_RESOLUTION * DISPLAY_1_Y_RESOLUTION)
GX_COLOR           scratchpad[SCRATCHPAD_PIXELS];

GX_WINDOW_ROOT    *root;
GX_BOOL            alpha = GX_FALSE;
GX_BOOL            compressed = GX_FALSE;
INT                xshift;
INT                yshift;
INT                horizontal_slider = 0;
INT                vertical_slider = 0;

/* Define prototypes.   */
VOID  demo_thread_entry(ULONG thread_input);
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);
VOID  guix_setup();

extern GX_CONST GX_PIXELMAP DISPLAY_1_DEFAULT_THEME_RESTART_INTERVAL_pixelmap;

int main(int argc, char ** argv)
{
  tx_kernel_enter();
  return(0);
}

VOID *decode_memory_allocate(ULONG size)
{
    VOID *memptr;
    if (tx_byte_allocate(&decode_pool, &memptr, size, TX_NO_WAIT) == TX_SUCCESS)
    {
        return memptr;
    }
    return NULL;
}

void decode_memory_free(VOID *mem)
{
    tx_byte_release(mem);
}

VOID tx_application_define(void *first_unused_memory)
{

    /* Create the main demo thread.  */
    tx_thread_create(&demo_thread, "GUIX Demo Thread", demo_thread_entry, 
                     0,  demo_thread_stack, sizeof(demo_thread_stack), 
                     1, 1, TX_NO_TIME_SLICE, TX_AUTO_START);
}


VOID  demo_thread_entry(ULONG thread_input)
{
    /* create byte pool for decode to use */
    tx_byte_pool_create(&decode_pool, "scratchpad", scratchpad,
                        SCRATCHPAD_PIXELS * sizeof(GX_COLOR));

    guix_setup();

    /* install our memory allocator and de-allocator */
    gx_system_memory_allocator_set(decode_memory_allocate, decode_memory_free);
}

VOID  guix_setup()
{
    /* Initialize GUIX.  */
    gx_system_initialize();


    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_24xrgb,
        LANGUAGE_ENGLISH, DISPLAY_1_DEFAULT_THEME, &root);

    /* create the main screen */
    gx_studio_named_widget_create("main_window", (GX_WIDGET *)root, (GX_WIDGET **)&pMainWin);
    pic_window = &pMainWin->main_window_pic_window;

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* let GUIX run */
    gx_system_start();
}

VOID main_window_draw(GX_WINDOW *window)
{
    int xpos;
    int ypos;

    gx_window_background_draw((GX_WINDOW*) window);

    xpos = window->gx_widget_size.gx_rectangle_left;
    ypos = window->gx_widget_size.gx_rectangle_top;

    gx_canvas_pixelmap_draw(xpos, ypos, (GX_PIXELMAP *)&DISPLAY_1_DEFAULT_THEME_RESTART_INTERVAL_pixelmap);

    gx_widget_children_draw(window);
}
