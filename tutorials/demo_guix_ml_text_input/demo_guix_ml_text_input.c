/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "demo_guix_ml_text_input_resources.h"
#include "demo_guix_ml_text_input_specifications.h"

#define MEMORY_POOL_SIZE (DISPLAY_1_X_RESOLUTION * DISPLAY_1_Y_RESOLUTION)

/* Define variables.  */
TX_BYTE_POOL       memory_pool;
GX_COLOR           memory_pool_buffer[MEMORY_POOL_SIZE];
GX_WINDOW_ROOT    *root;

/* Define prototypes.   */
VOID  start_guix();
extern UINT win32_graphics_driver_setup_565rgb(GX_DISPLAY *display);

GX_CONST GX_CHAR test_text[] = "It is helpful to have some knowledge of the basic organization of a GUIX Studio project "
                      "to understand how to use GUIX Studio effectively and to understand the information presented "
                      "in the Project View of the GUIX Studio IDE. The Project View is a summary visual representation "
                      "of all of the information contained in your project.\n"
                      "Before describing the project, it is necessary to define few terms. First, we use the term Display "
                      "to mean a physical display device. This is most often an LCD display device but it could be using other "
                      "technology. The next term is Screen, which mean a top-level GUIX object, usually a GUIX Window, and all of "
                      "its associated child elements. A Screen is a software construct that can be defined and modified at runtime."
                      "Finally, a Theme is a collection of resources. A theme includes a table of color definitions,"
                      "font definitions, and pixelmap definitions that are designed to work well together "
                      "and present your end user with a consistent look and feel.\n"
                      "The project first includes a set of global information such as the project name, "
                      "number of displays supported, the resolution and color format of each display, the number "
                      "of languages supported, the name of each supported language. The project name is the first node "
                      "displayed in the Project View.\n"
                      "The project next organizes all of the information required for up to 4 physical displays "
                      "and the screens and resources available to each display. The display names are the next level "
                      "nodes in the Project View tree.";


/******************************************************************************************/
/* Application entry.                                                                     */
/******************************************************************************************/
int main(int argc, char ** argv)
{
  tx_kernel_enter();
  return(0);
}

/******************************************************************************************/
/* Application defined memory allocate function.                                          */
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
/* Application defined memory free function.                                              */
/******************************************************************************************/
VOID memory_free(VOID *mem)
{
    tx_byte_release(mem);
}

/******************************************************************************************/
/* Define tx_application_define function.                                                 */
/******************************************************************************************/
VOID tx_application_define(void *first_unused_memory)
{
    /* create byte pool for rotate to use */
    tx_byte_pool_create(&memory_pool, "memory_pool", memory_pool_buffer, MEMORY_POOL_SIZE * sizeof(GX_COLOR));

    start_guix();
}

/******************************************************************************************/
/* Initiate and run GUIX.                                                                 */
/******************************************************************************************/
VOID  start_guix()
{
    GX_STRING string;

    /* Initialize GUIX.  */
    gx_system_initialize();

    /* install our memory allocator and de-allocator */
    gx_system_memory_allocator_set(memory_allocate, memory_free);

    /* Configure display. */
    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_565rgb, 
                                LANGUAGE_ENGLISH, DISPLAY_1_THEME_1, &root);

    /* create the MAIN screen */
    gx_studio_named_widget_create("main_screen", (GX_WIDGET *) root, GX_NULL);

    /* Set multi line text input text. */
    string.gx_string_ptr = test_text;
    string.gx_string_length = sizeof(test_text) - 1;
    gx_multi_line_text_input_text_set_ext(&main_screen.main_screen_ml_text_input, &string);

    /* Show the root window to make it visible.  */
    gx_widget_show(root);

    /* let GUIX run.  */
    gx_system_start();
}

/******************************************************************************************/
/* Convert string to integer value.                                                       */
/******************************************************************************************/
static void string_to_int(char *str, int len, int *value)
{
    int index;

    *value = 0;

    for (index = 0; index < len; index++)
    {
        *value = *value * 10;
        *value += str[index] - '0';
    }
}

/******************************************************************************************/
/* Set text alignment style.                                                              */
/******************************************************************************************/
VOID set_alignment(int alignment)
{
    gx_widget_style_remove((GX_WIDGET *)&main_screen.main_screen_ml_text_input, GX_STYLE_TEXT_ALIGNMENT_MASK);
    gx_widget_style_add((GX_WIDGET *)&main_screen.main_screen_ml_text_input, alignment);
}

/******************************************************************************************/
/* Set cursor width.                                                                      */
/******************************************************************************************/
VOID set_cursor_width()
{
    GX_MULTI_LINE_TEXT_INPUT *ml_input;
    GX_SINGLE_LINE_TEXT_INPUT *cursor_width_input;
    GX_CHAR  *input_buffer;
    UINT     string_size;
    INT      value;

    ml_input = &main_screen.main_screen_ml_text_input;

    cursor_width_input = &main_screen.main_screen_cursor_width_input;

    /* Pick up buffer address and input string size. */
    gx_single_line_text_input_buffer_get(cursor_width_input, &input_buffer, &string_size, GX_NULL);

    /* Convert string to integer. */
    string_to_int(input_buffer, string_size, &value);

    if (value > 0)
    {
        /* Set cursor width. */
        gx_text_input_cursor_width_set(&ml_input->gx_multi_line_text_input_cursor_instance, value);
    }
}

/******************************************************************************************/
/* Set cursor height.                                                                     */
/******************************************************************************************/
VOID set_cursor_height()
{
    GX_MULTI_LINE_TEXT_INPUT *ml_input;
    GX_SINGLE_LINE_TEXT_INPUT *cursor_height_input;
    GX_CHAR  *input_buffer;
    UINT     string_size;
    INT      value;

    ml_input = &main_screen.main_screen_ml_text_input;

    cursor_height_input = &main_screen.main_screen_cursor_height_input;

    /* Pick up buffer address and input string size. */
    gx_single_line_text_input_buffer_get(cursor_height_input, &input_buffer, &string_size, GX_NULL);

    /* Convert string to integer. */
    string_to_int(input_buffer, string_size, &value);

    if (value >= 0)
    {
        /* Set cursor height, 0 means use font height as cursor height. */
        gx_text_input_cursor_height_set(&ml_input->gx_multi_line_text_input_cursor_instance, value);
    }
}

/******************************************************************************************/
/* Set whitespace, the space from text draw area to the client boder.                     */
/******************************************************************************************/
VOID set_whitespace(int value)
{
    /* Set prompt value. */
    gx_numeric_prompt_value_set(&main_screen.main_screen_prompt_whitespace, value);

    /* Set multi line text view whitespace. */
    gx_multi_line_text_view_whitespace_set((GX_MULTI_LINE_TEXT_VIEW *)&main_screen.main_screen_ml_text_input, value);
}

/******************************************************************************************/
/* Set line space, the space between lines.                                               */
/******************************************************************************************/
VOID set_line_space(int value)
{
    /* Set prompt value. */
    gx_numeric_prompt_value_set(&main_screen.main_screen_prompt_line_space, value);

    /* Set multi line text view line space. */
    gx_multi_line_text_view_line_space_set((GX_MULTI_LINE_TEXT_VIEW *)&main_screen.main_screen_ml_text_input, value);
}

/******************************************************************************************/
/* Highlight whole text.                                                                    */
/******************************************************************************************/
VOID select_whole_text()
{
    UINT     string_size;

    /* Pick up string size. */
    gx_multi_line_text_input_buffer_get(&main_screen.main_screen_ml_text_input, GX_NULL, &string_size, GX_NULL);

    /* Select text[0: string_size - 1]. */
    gx_multi_line_text_input_text_select(&main_screen.main_screen_ml_text_input, 0, string_size - 1);
}

/******************************************************************************************/
/* Override the default event processing of "main_screen" to handle signals from my child */
/* widgets.                                                                               */
/******************************************************************************************/
UINT main_screen_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch(event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_FONT_SMALL, GX_EVENT_RADIO_SELECT):
        gx_multi_line_text_view_font_set((GX_MULTI_LINE_TEXT_VIEW *)&main_screen.main_screen_ml_text_input, GX_FONT_ID_SYSTEM);
        break;

    case GX_SIGNAL(ID_FONT_MEDIUM, GX_EVENT_RADIO_SELECT):
        gx_multi_line_text_view_font_set((GX_MULTI_LINE_TEXT_VIEW *)&main_screen.main_screen_ml_text_input, GX_FONT_ID_TITLE);
        break;

    case GX_SIGNAL(ID_ALIGN_LEFT, GX_EVENT_RADIO_SELECT):
        set_alignment(GX_STYLE_TEXT_LEFT);
        break;

    case GX_SIGNAL(ID_ALIGN_CENTER, GX_EVENT_RADIO_SELECT):
        set_alignment( GX_STYLE_TEXT_CENTER);
        break;

    case GX_SIGNAL(ID_ALIGN_RIGHT, GX_EVENT_RADIO_SELECT):
        set_alignment(GX_STYLE_TEXT_RIGHT);
        break;

    case GX_SIGNAL(ID_CURSOR_WIDTH_INPUT, GX_EVENT_TEXT_EDITED):
        set_cursor_width();
        break;

    case GX_SIGNAL(ID_CURSOR_HEIGHT_INPUT, GX_EVENT_TEXT_EDITED):
        set_cursor_height();
        break;

    case GX_SIGNAL(ID_SLIDER_WHITESPACE, GX_EVENT_SLIDER_VALUE):
        set_whitespace(event_ptr->gx_event_payload.gx_event_longdata);
        break;

    case GX_SIGNAL(ID_SLIDER_LINE_SPACE, GX_EVENT_SLIDER_VALUE):
        set_line_space(event_ptr->gx_event_payload.gx_event_longdata);
        break;

    case GX_SIGNAL(ID_SCROLLBAR, GX_EVENT_TOGGLE_ON):
        gx_widget_show((GX_WIDGET *)&main_screen.main_screen_input_scroll);
        break;

    case GX_SIGNAL(ID_SCROLLBAR, GX_EVENT_TOGGLE_OFF):
        gx_widget_hide((GX_WIDGET *)&main_screen.main_screen_input_scroll);
        break;

    case GX_SIGNAL(ID_SELECT_ALL, GX_EVENT_CLICKED):
        select_whole_text();
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }
    return GX_SUCCESS;
}

