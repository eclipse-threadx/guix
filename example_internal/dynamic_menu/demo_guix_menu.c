/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "dynamic_menu_resources.h"
#include "dynamic_menu_specifications.h"

/* Define the ThreadX demo thread control block and stack.  */

TX_THREAD          demo_thread;
UCHAR              demo_thread_stack[4096];

#define            BYTE_POOL_SIZE (4 * 1024)
UCHAR              byte_pool_mem[BYTE_POOL_SIZE];
TX_BYTE_POOL       byte_pool;

GX_WINDOW_ROOT    *root;


/* Define prototypes.   */
VOID  start_guix(VOID);
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);

int main(int argc, char ** argv)
{
  tx_kernel_enter();
  return(0);
}


VOID tx_application_define(void *first_unused_memory)
{
    /* Create the main demo thread.  */
    start_guix();
}

void *demo_allocator(ULONG size)
{
    VOID *memptr;

    if (tx_byte_allocate(&byte_pool, &memptr, size, TX_NO_WAIT) == TX_SUCCESS)
    {
        return memptr;
    }
    return NULL;
}

void demo_free(void *mem)
{
    tx_byte_release(mem);
}

VOID report_byte_pool_status(VOID)
{
    ULONG available_bytes;
    ULONG fragments;
    char mem_message[120];

    tx_byte_pool_info_get(&byte_pool, GX_NULL, &available_bytes, &fragments, GX_NULL, GX_NULL, GX_NULL);
    sprintf(mem_message, "Byte Pool: %d bytes available, %d fragments\n", (int)available_bytes, (int)fragments);
#if defined WIN_32
    OutputDebugString(mem_message);
#endif
}


VOID  start_guix(VOID)
{
    tx_byte_pool_create(&byte_pool, "scratchpad", byte_pool_mem, BYTE_POOL_SIZE);

    /* Initialize GUIX.  */
    gx_system_initialize();

    /* install our memory allocator and de-allocator */
    gx_system_memory_allocator_set(demo_allocator, demo_free);
    report_byte_pool_status();

    gx_studio_display_configure(MAIN_DISPLAY, win32_graphics_driver_setup_24xrgb, 
                                LANGUAGE_ENGLISH, MAIN_DISPLAY_THEME_1, &root);

    /* create the top menu screen */
    gx_studio_named_widget_create("top_menu", (GX_WIDGET *) root, GX_NULL);

    /* Show the root window to make it and child screen visible.  */
    gx_widget_show(root);

    /* let GUIX run */
    gx_system_start();
}

void ToggleScreen(GX_WINDOW *new_win, GX_WINDOW *old_win)
{
    if (!new_win->gx_widget_parent)
    {
        gx_widget_attach(root, (GX_WIDGET *) new_win);
    }
    else
    {
        gx_widget_show((GX_WIDGET *) new_win);
    }
    gx_widget_hide((GX_WIDGET *) old_win);
}

UINT top_menu_event(GX_WINDOW *top_menu, GX_EVENT *event_ptr)
{
    GX_WIDGET *new_win;

    switch(event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_ALBUMS, GX_EVENT_CLICKED):
        gx_studio_named_widget_create("albums_screen", (GX_WIDGET *) root, GX_NULL);
        break;

    case GX_SIGNAL(ID_ARTISTS, GX_EVENT_CLICKED):
        gx_studio_named_widget_create("artists_screen", (GX_WIDGET *) root, GX_NULL);
        break;

    case GX_SIGNAL(ID_GENRES, GX_EVENT_CLICKED):
        gx_studio_named_widget_create("genre_screen", (GX_WIDGET *) root, GX_NULL);
        break;

    case GX_SIGNAL(ID_PLAYLISTS, GX_EVENT_CLICKED):
        gx_studio_named_widget_create("playlist_screen", (GX_WIDGET *) root, GX_NULL);
        break;

    case GX_SIGNAL(ID_SONGS, GX_EVENT_CLICKED):
        gx_studio_named_widget_create("songs_screen", (GX_WIDGET *) root, GX_NULL);
        break;

    case GX_SIGNAL(ID_SETTINGS, GX_EVENT_CLICKED):
        gx_studio_named_widget_create("second_level_derived_screen", (GX_WIDGET *) GX_NULL, &new_win);
        gx_widget_attach(root, new_win);
        break;

    default:
        gx_window_event_process(top_menu, event_ptr);
    }
    return 0;
}


UINT sub_menu_event(GX_WINDOW *menu, GX_EVENT *event_ptr)
{
    switch(event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        gx_window_event_process(menu, event_ptr);
#if defined WIN_32
        OutputDebugString("Received Show Event\n");
#endif
        break;

    case GX_SIGNAL(IDB_BACK, GX_EVENT_CLICKED):
        gx_widget_delete(menu);
        report_byte_pool_status();
        break;

    default:
        gx_window_event_process(menu, event_ptr);
    }
    return 0;
}
