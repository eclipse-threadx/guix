/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include <string.h>
#include "tx_api.h"
#include "gx_api.h"

#include "demo_guix_runtime_bidi_text_resources.h"
#include "demo_guix_runtime_bidi_text_specifications.h"

/* Define the ThreadX demo thread control block and stack.  */

TX_THREAD          demo_thread;
UCHAR              demo_thread_stack[4096];

GX_WINDOW_ROOT    *root;
TX_BYTE_POOL       memory_pool;

#define            GUIX_POOL_SIZE (DISPLAY_1_X_RESOLUTION * DISPLAY_1_Y_RESOLUTION * 2)
GX_COLOR           guix_pool_memory[GUIX_POOL_SIZE];

/* Define prototypes.   */
VOID  demo_thread_entry(ULONG thread_input);
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);

/************************************************************************************/
/*  Program entry.                                                                  */
/************************************************************************************/
int main(int argc, char ** argv)
{
    tx_kernel_enter();
    return(0);
}

/************************************************************************************/
/*  User defined memory allocate function.                                          */
/************************************************************************************/
VOID *memory_allocate(ULONG size)
{
    VOID *memptr;

    if (tx_byte_allocate(&memory_pool, &memptr, size, TX_NO_WAIT) == TX_SUCCESS)
    {
        return memptr;
    }

    return NULL;
}

/************************************************************************************/
/*  User defined memory free function.                                              */
/************************************************************************************/
VOID memory_free(VOID *mem)
{
    tx_byte_release(mem);
}

/************************************************************************************/
/*  Define system initialization.                                                   */
/************************************************************************************/
VOID tx_application_define(void *first_unused_memory)
{

    /* Create memory pool. */
    tx_byte_pool_create(&memory_pool, "guix pool memory", guix_pool_memory,
        GUIX_POOL_SIZE * sizeof(GX_COLOR));

    /* Create the main demo thread.  */
    tx_thread_create(&demo_thread, "GUIX Demo Thread", demo_thread_entry, 
                     0,  demo_thread_stack, sizeof(demo_thread_stack), 
                     1, 1, TX_NO_TIME_SLICE, TX_AUTO_START);
}

/******************************************************************************************/
/* Demo thread entry.                                                                     */
/******************************************************************************************/
VOID  demo_thread_entry(ULONG thread_input)
{
    /* Initialize GUIX.  */
    gx_system_initialize();

    /* Install our memory allocator and de-allocator */
    gx_system_memory_allocator_set(memory_allocate, memory_free);

    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_24xrgb, 
                                0, 0, &root);

    /* Create the main screen */
    gx_studio_named_widget_create("main_screen", (GX_WIDGET *) root, GX_NULL);

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* let GUIX run */
    gx_system_start();
}

/******************************************************************************************/
/* Override the default event processing of "main_screen" to handle signals from my child */
/* widgets.                                                                               */
/******************************************************************************************/
UINT main_screen_event_process(GX_WINDOW* window, GX_EVENT* event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_BUTTON_ENGLISH, GX_EVENT_RADIO_SELECT):
        /* Switch to English. */
        gx_display_active_language_set(root->gx_window_root_canvas->gx_canvas_display, LANGUAGE_ENGLISH);

#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
        /* Disable runtime bidi text support. */
        gx_system_bidi_text_disable();
#endif
        break;

    case GX_SIGNAL(ID_BUTTON_ARABIC, GX_EVENT_RADIO_SELECT):
        /* Switch to Arabic. */
        gx_display_active_language_set(root->gx_window_root_canvas->gx_canvas_display, LANGUAGE_ARABIC);

#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
        /* Enable runtime bidi text support. */
        gx_system_bidi_text_enable();
#endif
        break;
    }

    /* Call default window event process. */
    return gx_window_event_process(window, event_ptr);
}

/******************************************************************************************/
/* Override the default drawing of "window" to do custom drawing.                         */
/******************************************************************************************/
VOID window_draw(GX_WINDOW *window)
{
#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
    /* This is an example to draw bidi text to canvas.
    
       If you are using a text widget for displaying bidi text,
       the following process is not needed, the text widget handles everything for you. */

    GX_BIDI_TEXT_INFO info;
    GX_BIDI_RESOLVED_TEXT_INFO *resolved_head;
    GX_BIDI_RESOLVED_TEXT_INFO* resolved;
    INT line;
    INT xpos;
    INT ypos;

    /* Call default window draw. */
    gx_window_draw(window);

    /* Set bidi text. */
    gx_context_string_get_ext(GX_STRING_ID_WINDOW_TEXT, &info.gx_bidi_text_info_text);

    /* Set text draw font. */
    gx_context_font_get(GX_FONT_ID_ARABIC, &info.gx_bidi_text_info_font);

    /* Set text display width for line breaking, if there is no need for line breaking, set display width to -1. */
    gx_window_client_width_get(window, &info.gx_bidi_text_info_display_width);

    xpos = window->gx_window_client.gx_rectangle_left;
    ypos = window->gx_window_client.gx_rectangle_top;

    gx_context_line_color_set(GX_COLOR_ID_TEXT);
    gx_context_font_set(GX_FONT_ID_ARABIC);

    /* Process bidi text reordering for display. */
    if (gx_utility_bidi_paragraph_reorder(&info, &resolved_head) == GX_SUCCESS)
    {
        resolved = resolved_head;

        do
        {
            for (line = 0; line < resolved->gx_bidi_resolved_text_info_total_lines; line++)
            {
                gx_canvas_text_draw_ext(xpos, ypos, &resolved->gx_bidi_resolved_text_info_text[line]);

                ypos += info.gx_bidi_text_info_font->gx_font_line_height;
            }

            resolved = resolved->gx_bidi_resolved_text_info_next;
        } while (resolved);

        /* Delete reordered bidi text. */
        gx_utility_bidi_resolved_text_info_delete(&resolved_head);
    }
#endif
}