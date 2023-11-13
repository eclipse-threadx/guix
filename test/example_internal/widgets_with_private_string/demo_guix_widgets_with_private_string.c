/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "widgets_with_private_string_resources.h"
#include "widgets_with_private_string_specifications.h"


GX_WINDOW *pScrollWheelScreen;


int          scroll_wheel_selected = 0;/* Used for testing*/
TX_SEMAPHORE *selected_semaphore = GX_NULL;

/* Define the ThreadX demo thread control block and stack.  */

GX_WINDOW_ROOT    *root;

TX_BYTE_POOL       rotate_pool;

#define            SCRATCHPAD_PIXELS (PRIMARY_X_RESOLUTION * PRIMARY_X_RESOLUTION)
GX_COLOR           scratchpad[SCRATCHPAD_PIXELS];

#define ID_SCROLL_WHEEL_MONTH 0xf1

VOID animation_speed_set();

/* Define prototypes.   */
VOID start_guix(VOID);
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);

INT numeric_prompt_value = 10000;
INT button_click_count = 0;


int main(int argc, char ** argv)
{
  tx_kernel_enter();
  return(0);
}

VOID *rotate_memory_allocate(ULONG size)
{
    VOID *memptr;

    if (tx_byte_allocate(&rotate_pool, &memptr, size, TX_NO_WAIT) == TX_SUCCESS)
    {
        return memptr;
    }

    return NULL;
}

VOID rotate_memory_free(VOID *mem)
{
    tx_byte_release(mem);
}

VOID tx_application_define(void *first_unused_memory)
{
    /* Create the main demo thread.  */

    /* create byte pool for rotate to use */
    tx_byte_pool_create(&rotate_pool, "scratchpad", scratchpad,
                        SCRATCHPAD_PIXELS * sizeof(GX_COLOR));

    start_guix();
}

VOID start_guix(void)
{
    /* Initialize GUIX.  */
    gx_system_initialize();

    /* install our memory allocator and de-allocator */
    gx_system_memory_allocator_set(rotate_memory_allocate, rotate_memory_free);

    gx_studio_display_configure(PRIMARY, win32_graphics_driver_setup_24xrgb, 
                                LANGUAGE_ENGLISH, PRIMARY_THEME_1, &root);
    /* create the scroll_wheel_screen */
    gx_studio_named_widget_create("scroll_wheel_screen", (GX_WIDGET *) root, (GX_WIDGET **) &pScrollWheelScreen);
    gx_studio_named_widget_create("main_screen", GX_NULL, GX_NULL);

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* start the GUIX thread */
    gx_system_start();
}

void scroll_wheel_value_set(GX_PROMPT *prompt, INT selected_row)
{
    GX_CHAR text_buffer[10];
    GX_STRING text;

    gx_utility_ltoa(selected_row, text_buffer, 10);
    text.gx_string_ptr = text_buffer;
    text.gx_string_length = strnlen(text_buffer, sizeof(text_buffer));
    gx_prompt_text_set_ext(prompt, &text);
}


/* Override default event processing of scroll wheel screen. */
UINT scroll_wheel_screen_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{

    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        pScrollWheelScreen = window;
        gx_window_event_process(window, event_ptr);
        animation_speed_set();
        break;


    case GX_SIGNAL(ID_MONTH_WHEEL, GX_EVENT_LIST_SELECT):
        if (selected_semaphore)
        {
            tx_semaphore_put(selected_semaphore);
        }
        break;


    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}

VOID animation_speed_set()
{
    GX_STRING_SCROLL_WHEEL *month_wheel;

    gx_widget_find(pScrollWheelScreen, ID_MONTH_WHEEL, GX_SEARCH_DEPTH_INFINITE, &month_wheel);

    gx_scroll_wheel_speed_set(month_wheel, (GX_FIXED_VAL_MAKE(1) / 2), 100, 10, 2);
}


