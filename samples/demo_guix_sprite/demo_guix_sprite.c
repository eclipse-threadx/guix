/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"

#include "guix_sprite_resources.h"
#include "guix_sprite_specifications.h"

/* Define the ThreadX demo thread control block and stack.  */
MAIN_WINDOW_CONTROL_BLOCK *pMainWin;
GX_WINDOW *pAppleWin;

GX_WINDOW_ROOT    *root;
GX_PIXELMAP       scaled_map;
GX_UBYTE          alpha_value = 255;
int               scale = 100;
GX_BOOL           sprite_move_down = GX_TRUE;

TX_THREAD         demo_thread;
UCHAR             demo_thread_stack[4096];
TX_BYTE_POOL      resize_pool;

/* Memroy should be a little larger than the resize image. */
#define           SCRATCHPAD_PIXELS (DISPLAY_1_X_RESOLUTION * (DISPLAY_1_Y_RESOLUTION + 1))
GX_COLOR          scratchpad[SCRATCHPAD_PIXELS];

/* Define prototypes.   */
VOID  start_guix(VOID);
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);

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


VOID  start_guix(VOID)
{
    /* Initialize GUIX.  */
    gx_system_initialize();

    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_24xrgb, 
                                LANGUAGE_ENGLISH, DISPLAY_1_DEFAULT_THEME, &root);

    scaled_map.gx_pixelmap_data = GX_NULL;

    /* create the main screen */
    gx_studio_named_widget_create("main_window", (GX_WIDGET *) root, (GX_WIDGET **) &pMainWin);

    gx_widget_show(root);

    /* start GUIX thread */
    gx_system_start();
}

void MoveSprite(void)
{
    GX_RECTANGLE size;
    GX_SPRITE *sprite = &main_window.main_window_bird_sprite;

    size = sprite -> gx_widget_size;

    if (sprite_move_down)
    {
        gx_utility_rectangle_shift(&size, -120, 57);
    }
    else
    {
        gx_utility_rectangle_shift(&size, 80, -157);
    }

    if (size.gx_rectangle_bottom > pMainWin -> gx_widget_size.gx_rectangle_bottom)
    {
        sprite_move_down = GX_FALSE;
    }
    if (size.gx_rectangle_top < pMainWin -> gx_widget_size.gx_rectangle_top)
    {
        sprite_move_down = GX_TRUE;
    }
    
    if (size.gx_rectangle_right < pMainWin -> gx_widget_size.gx_rectangle_left)
    {
        size.gx_rectangle_right += pMainWin -> gx_widget_size.gx_rectangle_right;
        size.gx_rectangle_left += pMainWin -> gx_widget_size.gx_rectangle_right;
    }
    gx_widget_resize(sprite, &size);
}

void create_scaled_map()
{
    int width;
    int height;
    GX_PIXELMAP *map = GX_NULL;

    if (scaled_map.gx_pixelmap_data)
    {
        resize_memory_free((VOID *)scaled_map.gx_pixelmap_data);
        scaled_map.gx_pixelmap_data = GX_NULL;
    }

    gx_widget_pixelmap_get(pMainWin, GX_PIXELMAP_ID_SKY, &map);

    if (map)
    {
        width = (scale * DISPLAY_1_X_RESOLUTION) / 100;
        height = (scale * DISPLAY_1_Y_RESOLUTION) / 100;

        if ((width <= 0) || (height <= 0))
        {
            return;
        }
        gx_utility_pixelmap_resize(map, &scaled_map, width, height);
    }
    
}

UINT MainWindowEventProcess(GX_WINDOW *window, GX_EVENT *myevent)
{
    UINT status = 0;

    switch(myevent -> gx_event_type)
    {
    case GX_SIGNAL(ID_ALPHA_SLIDER, GX_EVENT_SLIDER_VALUE):
        alpha_value = (GX_UBYTE)  myevent -> gx_event_payload.gx_event_longdata;
        gx_system_dirty_mark(window);
        break;

    case GX_SIGNAL(ID_BACKGROUND_SIZE, GX_EVENT_SLIDER_VALUE):
        scale = (int)  myevent -> gx_event_payload.gx_event_longdata;
        gx_system_dirty_mark(window);
        break;

    case GX_SIGNAL(ID_BIRD_SPRITE, GX_EVENT_SPRITE_COMPLETE):
        MoveSprite();
        gx_sprite_start(&main_window.main_window_bird_sprite, 0);
        break;
    
    default:
        status = gx_window_event_process(window, myevent);
        break;
    }
    return status;
}

VOID MainWindowDraw(GX_WINDOW *window)
{
GX_PIXELMAP *map = GX_NULL;
int          win_width;
int          win_height;
int          xpos;
int          ypos;

    gx_window_draw(window);

    gx_widget_pixelmap_get(pMainWin, GX_PIXELMAP_ID_SKY, &map);

    if (map)
    {
        if (map -> gx_pixelmap_width != (scale * DISPLAY_1_X_RESOLUTION) / 100)
        {
            create_scaled_map();

            if (scaled_map.gx_pixelmap_data)
            {
                map = &scaled_map;
            }
        }

        if (map)
        {
            win_width = window->gx_widget_size.gx_rectangle_right - window->gx_widget_size.gx_rectangle_left + 1;
            win_height = window->gx_widget_size.gx_rectangle_bottom - window->gx_widget_size.gx_rectangle_top + 1;

            xpos = window->gx_widget_size.gx_rectangle_left;
            ypos = window->gx_widget_size.gx_rectangle_top;

            xpos += (win_width - map->gx_pixelmap_width) >> 1;
            ypos += (win_height - map->gx_pixelmap_height) >> 1;

            if (alpha_value < 255)
            {
                gx_canvas_pixelmap_blend(xpos, ypos, map, alpha_value);
            }
            else
            {
                gx_canvas_pixelmap_draw(xpos, ypos, map);
            }
        }
    }

    gx_widget_children_draw(window);
}


