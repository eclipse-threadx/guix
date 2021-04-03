/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"

#include "demo_guix_circular_gauge_resources.h"
#include "demo_guix_circular_gauge_specifications.h"

/* Define the ThreadX demo thread control block and stack.  */

TX_THREAD          demo_thread;
UCHAR              demo_thread_stack[4096];
TX_BYTE_POOL       memory_pool;

#define            SCRATCHPAD_PIXELS (DISPLAY_1_X_RESOLUTION * DISPLAY_1_Y_RESOLUTION)
GX_COLOR           scratchpad[SCRATCHPAD_PIXELS];

GX_WINDOW_ROOT    *root;


/* Define prototypes.   */
VOID  guix_setup(void);
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);
UINT string_length_get(GX_CONST GX_CHAR* input_string, UINT max_string_length);

/******************************************************************************************/
int main(int argc, char ** argv)
{
  tx_kernel_enter();
  return(0);
}

/******************************************************************************************/
VOID *memory_allocate(ULONG size)
{
    VOID *memptr;

    if (tx_byte_allocate(&memory_pool, &memptr, size, TX_NO_WAIT) == TX_SUCCESS)
    {
        return memptr;
    }
    return NULL;
}

/******************************************************************************************/
void memory_free(VOID *mem)
{
    tx_byte_release(mem);
}

/******************************************************************************************/
VOID tx_application_define(void *first_unused_memory)
{

    /* create byte pool for needle rotation to use */
    tx_byte_pool_create(&memory_pool, "scratchpad", scratchpad, 
                        SCRATCHPAD_PIXELS * sizeof(GX_COLOR));

    guix_setup();

    /* install our memory allocator and de-allocator */
    gx_system_memory_allocator_set(memory_allocate, memory_free);
}

/******************************************************************************************/
VOID  guix_setup()
{

    /* Initialize GUIX.  */
    gx_system_initialize();

    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_24xrgb, 
                                LANGUAGE_ENGLISH, DISPLAY_1_THEME_1, &root);

    /* create the gauge_window */
    gx_studio_named_widget_create("gauge_window", (GX_WIDGET *)root, GX_NULL);

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* let GUIX run */
    gx_system_start();
}


/******************************************************************************************/
void set_speed_value(int id, int value)
{
GX_PROMPT *prompt;
static GX_CHAR str_value1[10];
static GX_CHAR str_value2[10];
GX_STRING string;

    if (id == 1)
    {
        prompt = &gauge_window.gauge_window_speed_value_1;
        string.gx_string_ptr = str_value1;
    }
    else
    {
        prompt = &gauge_window.gauge_window_speed_value_2;
        string.gx_string_ptr = str_value2;
    }

    gx_utility_ltoa(value, (GX_CHAR *)string.gx_string_ptr, 10);
    string.gx_string_length = string_length_get(string.gx_string_ptr, sizeof(str_value1) - 1);
    gx_prompt_text_set_ext(prompt, &string);
}


/******************************************************************************************/
UINT window_event_handle(GX_WINDOW *win, GX_EVENT *event_ptr)
{
int pos;
GX_CIRCULAR_GAUGE *gauge;

    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_POS_1, GX_EVENT_SLIDER_VALUE):
        /* Change needle position. */
        pos = event_ptr->gx_event_payload.gx_event_longdata;
        set_speed_value(1, pos);
        gx_widget_find(win, ID_GAUGE_1, GX_SEARCH_DEPTH_INFINITE, &gauge);
        gx_circular_gauge_angle_set(gauge, pos);
        break;

    case GX_SIGNAL(ID_POS_2, GX_EVENT_SLIDER_VALUE):
        /* Change needle position of gauge 1*/
        pos = event_ptr->gx_event_payload.gx_event_longdata;
        set_speed_value(2, pos);
        gx_widget_find(win, ID_GAUGE_2, GX_SEARCH_DEPTH_INFINITE, &gauge);
        gx_circular_gauge_angle_set(gauge, pos);
        break;
    }

    return gx_window_event_process(win, event_ptr);
}

/******************************************************************************************/
UINT string_length_get(GX_CONST GX_CHAR* input_string, UINT max_string_length)
{
    UINT length = 0;

    if (input_string)
    {
        /* Traverse the string.  */
        for (length = 0; input_string[length]; length++)
        {
            /* Check if the string length is bigger than the max string length.  */
            if (length >= max_string_length)
            {
                break;
            }
        }
    }

    return length;
}
