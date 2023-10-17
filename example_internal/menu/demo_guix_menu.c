/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "menu_resources.h"
#include "menu_specifications.h"

#define SCREEN_STACK_SIZE  20
/* Define the ThreadX demo thread control block and stack.  */

TX_THREAD          demo_thread;
UCHAR              demo_thread_stack[4096];

GX_WINDOW_ROOT            *root;
GX_SCREEN_STACK_CONTROL   screen_stack_ctrl;
GX_WIDGET                *screen_stack[SCREEN_STACK_SIZE];
GX_BOOL                   show_toolbar = GX_FALSE;

extern BOOKSHELF_SCREEN_CONTROL_BLOCK bookshelf_screen;
extern BOOK_SCREEN_CONTROL_BLOCK      book_screen;
extern TOOLBAR_SCREEN_CONTROL_BLOCK   toolbar_screen;
extern MAIN_SCREEN_CONTROL_BLOCK      main_screen;
/* Define prototypes.   */
VOID  start_guix(VOID);
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);

GX_CHAR text[] = {
    "\r\n2002 - 2015 by Express Logic, Inc.\r\n\r\n\
    All rights reserved.\
    Inc.Each contains proprietary information of Express Logic, Inc.\
    Any portion of this document without the prior ...\r\n\r\n\
    Express Logic, Inc.reserves the right to ... \
    without notice in order to improve design or reliability of GUIX.\
    carefully checked for accuracy; however, Express Logic, Inc."
};

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


VOID  start_guix(VOID)
{
    GX_STRING str;

    /* Initialize GUIX.  */
    gx_system_initialize();

    gx_studio_display_configure(MAIN_DISPLAY, win32_graphics_driver_setup_24xrgb, 
                                LANGUAGE_ENGLISH, MAIN_DISPLAY_THEME_1, &root);

    /* create the top menu screen */
    gx_studio_named_widget_create("main_screen", (GX_WIDGET *) root, (GX_WIDGET **)&main_screen);

    gx_studio_named_widget_create("bookshelf_screen", GX_NULL, (GX_WIDGET **)&bookshelf_screen);

    gx_studio_named_widget_create("book_screen", GX_NULL, (GX_WIDGET **)&book_screen);

    gx_studio_named_widget_create("toolbar_screen", GX_NULL, (GX_WIDGET **)&toolbar_screen);

    str.gx_string_ptr = text;
    str.gx_string_length = strnlen(text, sizeof(text));
    gx_multi_line_text_view_text_set_ext(&book_screen.book_screen_text_view_1, &str);

    gx_screen_stack_create(&screen_stack_ctrl, screen_stack, SCREEN_STACK_SIZE * sizeof(GX_WIDGET *));

    /* Show the root window to make it and child screen visible.  */
    gx_widget_show(root);

    /* let GUIX run */
    gx_system_start();
}

void toolbar_detach_attach()
{
    if (show_toolbar)
    {
        gx_widget_detach(&toolbar_screen);
        show_toolbar = GX_FALSE;
    }
    else
    {
        gx_widget_attach(&book_screen, &toolbar_screen);
        show_toolbar = GX_TRUE;
    }
}

UINT sub_menu_event(GX_WINDOW *window, GX_EVENT *my_event)
{
    switch (my_event->gx_event_type)
    {
    case GX_SIGNAL(ID_BTN_BOOKSHELF, GX_EVENT_CLICKED):
        gx_screen_stack_push(&screen_stack_ctrl, (GX_WIDGET *)window, (GX_WIDGET *)&bookshelf_screen);
        break;

    case GX_SIGNAL(ID_PROMPT_BOOK, GX_EVENT_CLICKED):
        gx_screen_stack_push(&screen_stack_ctrl, (GX_WIDGET *)window, (GX_WIDGET *)&book_screen);
        break;

    case GX_SIGNAL(ID_BACK, GX_EVENT_CLICKED):
        gx_screen_stack_pop(&screen_stack_ctrl);

        if (show_toolbar)
        {
            toolbar_detach_attach();
        }
        break;
    case GX_SIGNAL(ID_HOME_BUTTON,GX_EVENT_CLICKED):
        gx_widget_detach(window);
        gx_widget_attach(root, (GX_WIDGET *)&main_screen);
        gx_screen_stack_reset(&screen_stack_ctrl);

        if (show_toolbar)
        {
            toolbar_detach_attach();
        }
        break;

    case GX_EVENT_PEN_DOWN:
        if (window->gx_widget_id == ID_BOOK_SCREEN)
        {
            toolbar_detach_attach();
        }
        break;

    default:
        gx_window_event_process(window, my_event);
        break;
    }
    return GX_SUCCESS;
}
