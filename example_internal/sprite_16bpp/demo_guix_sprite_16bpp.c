/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"

#include "sprite_16bpp_resources.h"
#include "sprite_16bpp_specifications.h"

/* Define the ThreadX demo thread control block and stack.  */
MAIN_WINDOW_CONTROL_BLOCK *pMainWin;
GX_WINDOW *pAppleWin;

TX_THREAD          demo_thread;
UCHAR              demo_thread_stack[4096];

GX_WINDOW_ROOT    *root;
GX_UBYTE          alpha_value = 255;

TX_THREAD         demo_thread;
UCHAR             demo_thread_stack[4096];
TX_BYTE_POOL      resize_pool;

#define           SCRATCHPAD_PIXELS (DISPLAY_1_X_RESOLUTION * DISPLAY_1_Y_RESOLUTION)
GX_COLOR          scratchpad[SCRATCHPAD_PIXELS];

/* Define prototypes.   */
VOID  start_guix();

#define USE_16BPP

#ifdef USE_16BPP
extern UINT win32_graphics_driver_setup_565rgb(GX_DISPLAY *display);
#else
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);
#endif

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

int main(int argc, char ** argv)
{
    tx_kernel_enter();
    return(0);
}

VOID tx_application_define(void *first_unused_memory)
{

    /* create byte pool for resize to use */
    tx_byte_pool_create(&resize_pool, "scratchpad", scratchpad,
        SCRATCHPAD_PIXELS * sizeof(GX_COLOR));

    start_guix();

    /* install our memory allocator and de-allocator */
    gx_system_memory_allocator_set(resize_memory_allocate, resize_memory_free);
}


VOID  start_guix()
{
    /* Initialize GUIX.  */
    gx_system_initialize();

    #ifdef USE_16BPP
    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_565rgb, 
                                LANGUAGE_ENGLISH, DISPLAY_1_DEFAULT_THEME, &root);
    #else
    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_24xrgb, 
                                LANGUAGE_ENGLISH, DISPLAY_1_DEFAULT_THEME, &root);
    #endif

    /* create the main screen */
    gx_studio_named_widget_create("main_window", (GX_WIDGET *) root, (GX_WIDGET **) &pMainWin);

    pAppleWin = &pMainWin -> main_window_AppleWindow;

    gx_widget_show(root);

    /* let GUIX run */
    gx_system_start();
}

void DisplayNewAlphaValue(GX_WINDOW *parent)
{
    GX_PROMPT *prompt;
    static GX_CHAR number_val[10];
    GX_STRING string;

    gx_widget_find(parent, ID_ALPHA_DISPLAY, 1, &prompt);
    gx_utility_ltoa(alpha_value, number_val, 10);
    string.gx_string_ptr = number_val;
    string.gx_string_length = strnlen(number_val, sizeof(number_val));
    gx_prompt_text_set_ext(prompt, &string);
}

UINT MainWindowEventProcess(GX_WINDOW *window, GX_EVENT *myevent)
{
    UINT status = 0;

    switch(myevent->gx_event_type)
    {
    case GX_SIGNAL(ID_ALPHA_SLIDER, GX_EVENT_SLIDER_VALUE):
        alpha_value = (GX_UBYTE)  myevent->gx_event_payload.gx_event_longdata;
        DisplayNewAlphaValue(window);
        gx_system_dirty_mark(&main_window.main_window_AppleWindow);
        break;
    
    default:
        status = gx_window_event_process(window, myevent);
        break;
    }
    return status;
}

VOID AppleWindowDraw(GX_WINDOW *window)
{
    GX_PIXELMAP *map;
    GX_PIXELMAP scaled_map;
    int width;
    int height;
    int xpos;
    int ypos;

    gx_context_pixelmap_get(GX_PIXELMAP_ID_RED_APPLE, &map);

    width = (alpha_value * map->gx_pixelmap_width) >> 8;
    height = (alpha_value * map->gx_pixelmap_height) >> 8;

	if((width == 0) || (height == 0))
	{
	    return;
	}
	
    if (gx_utility_pixelmap_resize(map, &scaled_map, width, height) == GX_SUCCESS)
    {
        xpos = window->gx_widget_size.gx_rectangle_left;
        ypos = window->gx_widget_size.gx_rectangle_top;

        xpos += (map->gx_pixelmap_width - width) >> 1;
        ypos += (map->gx_pixelmap_height - height) >> 1;

        gx_canvas_pixelmap_blend(xpos, ypos, &scaled_map, alpha_value);

        if (scaled_map.gx_pixelmap_data)
        {
            resize_memory_free((VOID *)scaled_map.gx_pixelmap_data);
        }

        if (scaled_map.gx_pixelmap_aux_data)
        {
            resize_memory_free((VOID *)scaled_map.gx_pixelmap_aux_data);
        }
    }
                            
}


