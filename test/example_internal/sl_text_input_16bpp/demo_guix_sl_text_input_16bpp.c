/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "sl_text_input_16bpp_resources.h"
#include "sl_text_input_16bpp_specifications.h"

WINDOW_CONTROL_BLOCK *pMainScreen;

/* Define the ThreadX demo thread control block and stack.  */

TX_THREAD          demo_thread;
UCHAR              demo_thread_stack[4096];
extern             GX_CONST GX_STRING *display_1_language_table[];

GX_WINDOW_ROOT    *root;
GX_RESOURCE_ID font[4][2] = {{GX_FONT_ID_TEXT_INPUT_1BPP_18PIX, GX_FONT_ID_TEXT_INPUT_1BPP_24PIX},
                             {GX_FONT_ID_TEXT_INPUT_4BPP_18PIX, GX_FONT_ID_TEXT_INPUT_4BPP_24PIX},
                             {GX_FONT_ID_TEXT_INPUT_8BPP_18PIX, GX_FONT_ID_TEXT_INPUT_8BPP_24PIX},
                             { GX_FONT_ID_CHINESE_18PIX, GX_FONT_ID_CHINESE_24PIX }};

UINT    font_depth = 2;
UINT    font_size = 0;

GX_BOOL APP_SHIFT_FLAG = GX_FALSE;
GX_BOOL APP_CTRL_FLAG = GX_FALSE;

TX_BYTE_POOL       memory_pool;

#define SCRATCHPAD_PIXELS (DISPLAY_1_X_RESOLUTION * DISPLAY_1_Y_RESOLUTION)
GX_COLOR           scratchpad[SCRATCHPAD_PIXELS];

/* Define prototypes.   */
VOID  demo_thread_entry(ULONG thread_input);
extern UINT win32_graphics_driver_setup_565rgb(GX_DISPLAY *display);

int main(int argc, char ** argv)
{
  tx_kernel_enter();
  return(0);
}

/*****************************************************************************/
/* Function called by GUIX internals to allocate memory block. This is used  */
/* to allocate memory for rotated gauge needle image                         */
/*****************************************************************************/
VOID *memory_allocate(ULONG size)
{
    VOID *memptr;

    if (tx_byte_allocate(&memory_pool, &memptr, size, TX_NO_WAIT) == TX_SUCCESS)
    {
        return memptr;
    }

    return NULL;
}

/*****************************************************************************/
/* Function called by GUIX internals to free memory dynamically allocated    */
/*****************************************************************************/
VOID memory_free(VOID *mem)
{
    tx_byte_release(mem);
}

VOID tx_application_define(void *first_unused_memory)
{
    /* create byte pool for rotate to use */
    tx_byte_pool_create(&memory_pool, "scratchpad", scratchpad,
        SCRATCHPAD_PIXELS * sizeof(GX_COLOR));

    /* Create the main demo thread.  */
    tx_thread_create(&demo_thread, "GUIX Demo Thread", demo_thread_entry, 
                     0,  demo_thread_stack, sizeof(demo_thread_stack), 
                     1, 1, TX_NO_TIME_SLICE, TX_AUTO_START);
}


GX_STUDIO_DISPLAY_INFO display_info;

VOID  demo_thread_entry(ULONG thread_input)
{
    /* Initialize GUIX.  */
    gx_system_initialize();

    /* install our memory allocator and de-allocator */
    gx_system_memory_allocator_set(memory_allocate, memory_free);

    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_565rgb, 
                                LANGUAGE_ENGLISH, DISPLAY_1_THEME_1, &root);

    /* Create the button screen */
    gx_studio_named_widget_create("window", (GX_WIDGET *) root, (GX_WIDGET **) &pMainScreen);
    gx_studio_named_widget_create("input_focus_release_test", GX_NULL, GX_NULL);

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* Let GUIX run */
    gx_system_start();
}

/* Convert string to integer. */
void string_to_int(char *str, int len, int *value)
{
int i;
    
    *value = 0;

    for(i = 0; i < len; i++)
    {
        *value = *value * 10;
        *value += str[i] - '0';
    }
}

UINT sl_input_event_handler(GX_SINGLE_LINE_TEXT_INPUT *input, GX_EVENT *event_ptr)
{
    GX_EVENT my_event;

    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_KEY_DOWN:
        memset(&my_event, 0, sizeof(GX_EVENT));
        my_event.gx_event_target = (GX_WIDGET *)input;
        switch (event_ptr->gx_event_payload.gx_event_ushortdata[0])
        {
        case GX_KEY_SHIFT:
            APP_SHIFT_FLAG = GX_TRUE;
            break;

        case GX_KEY_CONTROL:
            APP_CTRL_FLAG = GX_TRUE;
            break;

        case GX_KEY_LEFT_ARROW:
            if (APP_SHIFT_FLAG)
            {
                my_event.gx_event_type = GX_EVENT_MARK_PREVIOUS;
                return gx_system_event_send(&my_event);
            }
            break;

        case GX_KEY_RIGHT_ARROW:
            if (APP_SHIFT_FLAG)
            {
                my_event.gx_event_type = GX_EVENT_MARK_NEXT;
                return gx_system_event_send(&my_event);
            }
            break;

        case GX_KEY_HOME:
            if (APP_SHIFT_FLAG)
            {
                my_event.gx_event_type = GX_EVENT_MARK_HOME;
                return gx_system_event_send(&my_event);
            }
            break;

        case GX_KEY_END:
            if (APP_SHIFT_FLAG)
            {
                my_event.gx_event_type = GX_EVENT_MARK_END;
                return gx_system_event_send(&my_event);
            }
            break;

        case 'c':
        case 'C':
            if (APP_CTRL_FLAG)
            {
                /* Copy. */
                my_event.gx_event_type = GX_EVENT_COPY;
                
                return gx_system_event_send(&my_event);
            }
            break;

        case 'x':
        case 'X':
            if (APP_CTRL_FLAG)
            {
                /* Cut. */
                my_event.gx_event_type = GX_EVENT_CUT;
                return gx_system_event_send(&my_event);
            }
            break;

        case 'v':
        case 'V':
            if (APP_CTRL_FLAG)
            {
                /* Paste. */
                my_event.gx_event_type = GX_EVENT_PASTE;
                return gx_system_event_send(&my_event);
            }
            break;
        }
        return gx_single_line_text_input_event_process(input, event_ptr);

    case GX_EVENT_KEY_UP:
        switch (event_ptr->gx_event_payload.gx_event_ushortdata[0])
        {
        case GX_KEY_SHIFT:
            APP_SHIFT_FLAG = GX_FALSE;
            break;

        case GX_KEY_CONTROL:
            APP_CTRL_FLAG = GX_FALSE;
            break;
        }
        break;

    default:
        return gx_single_line_text_input_event_process(input, event_ptr);
    }

    return GX_SUCCESS;
}

UINT main_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
GX_SINGLE_LINE_TEXT_INPUT *input;
GX_SINGLE_LINE_TEXT_INPUT *cursor_width_input;
GX_SINGLE_LINE_TEXT_INPUT *cursor_height_input;
GX_CHAR  *input_buffer;
UINT     string_size;
INT      value;

    input = &pMainScreen->window_sl_text_input;

    switch(event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_CURSOR_BLINK, GX_EVENT_TOGGLE_ON):
        gx_single_line_text_input_style_add(input, GX_STYLE_CURSOR_BLINK);
        break;
    case GX_SIGNAL(ID_CURSOR_BLINK, GX_EVENT_TOGGLE_OFF):
        gx_single_line_text_input_style_remove(input, GX_STYLE_CURSOR_BLINK);
        break;
    case GX_SIGNAL(ID_CURSOR_ALWAYS, GX_EVENT_TOGGLE_ON):
        gx_single_line_text_input_style_add(input, GX_STYLE_CURSOR_ALWAYS);
        break;
    case GX_SIGNAL(ID_CURSOR_ALWAYS, GX_EVENT_TOGGLE_OFF):
        gx_single_line_text_input_style_remove(input, GX_STYLE_CURSOR_ALWAYS);
        break;
    case GX_SIGNAL(ID_FONT_1BPP, GX_EVENT_RADIO_SELECT):
        font_depth = 0;
        gx_prompt_font_set((GX_PROMPT *)input, font[font_depth][font_size]);
        break;
    case GX_SIGNAL(ID_FONT_4BPP, GX_EVENT_RADIO_SELECT):
        font_depth = 1;
        gx_prompt_font_set((GX_PROMPT *)input, font[font_depth][font_size]);
        break;
    case GX_SIGNAL(ID_FONT_8BPP, GX_EVENT_RADIO_SELECT):
        font_depth = 2;
        gx_prompt_font_set((GX_PROMPT *)input, font[font_depth][font_size]);
        break;
    case GX_SIGNAL(ID_SIZE_18PIX, GX_EVENT_RADIO_SELECT):
        font_size = 0;
        gx_prompt_font_set((GX_PROMPT *)input, font[font_depth][font_size]);
        break;
    case GX_SIGNAL(ID_SIZE_24PIX, GX_EVENT_RADIO_SELECT):
        font_size = 1;
        gx_prompt_font_set((GX_PROMPT *)input, font[font_depth][font_size]);
        break;
    case GX_SIGNAL(ID_NO_BORDER, GX_EVENT_RADIO_SELECT):
        gx_widget_border_style_set((GX_WIDGET *)input, GX_STYLE_BORDER_NONE);
        gx_system_dirty_mark((GX_WIDGET *)input);
        break;
    case GX_SIGNAL(ID_THIN_BORDER, GX_EVENT_RADIO_SELECT):
        gx_widget_border_style_set((GX_WIDGET *)input, GX_STYLE_BORDER_THIN);
        gx_system_dirty_mark((GX_WIDGET *)input);
        break;
    case GX_SIGNAL(ID_RECESSED_BORDER, GX_EVENT_RADIO_SELECT):
        gx_widget_border_style_set((GX_WIDGET *)input, GX_STYLE_BORDER_RECESSED);
        gx_system_dirty_mark((GX_WIDGET *)input);
        break;
    case GX_SIGNAL(ID_RAISED_BORDER, GX_EVENT_RADIO_SELECT):
        gx_widget_border_style_set((GX_WIDGET *)input, GX_STYLE_BORDER_RAISED);
        gx_system_dirty_mark((GX_WIDGET *)input);
        break;
    case GX_SIGNAL(ID_THICK_BORDER, GX_EVENT_RADIO_SELECT):
        gx_widget_border_style_set((GX_WIDGET *)input, GX_STYLE_BORDER_THICK);
        gx_system_dirty_mark((GX_WIDGET *)input);
        break;

    case GX_SIGNAL(ID_ALIGN_LEFT, GX_EVENT_RADIO_SELECT):
        gx_widget_style_remove((GX_WIDGET *)input, GX_STYLE_TEXT_ALIGNMENT_MASK);
        gx_widget_style_add((GX_WIDGET *)input, GX_STYLE_TEXT_LEFT);
        break;

    case GX_SIGNAL(ID_ALIGN_RIGHT, GX_EVENT_RADIO_SELECT):
        gx_widget_style_remove((GX_WIDGET *)input, GX_STYLE_TEXT_ALIGNMENT_MASK);
        gx_widget_style_add((GX_WIDGET *)input, GX_STYLE_TEXT_RIGHT);
        break;

    case GX_SIGNAL(ID_ALIGN_CENTER, GX_EVENT_RADIO_SELECT):
        gx_widget_style_remove((GX_WIDGET *)input, GX_STYLE_TEXT_ALIGNMENT_MASK);
        gx_widget_style_add((GX_WIDGET *)input, GX_STYLE_TEXT_CENTER);
        break;

    case GX_SIGNAL(ID_CURSOR_WIDTH_INPUT, GX_EVENT_TEXT_EDITED):
        cursor_width_input = &pMainScreen->window_cursor_width_input;

        /* Pick up buffer address and input string size. */
        gx_single_line_text_input_buffer_get(cursor_width_input, &input_buffer, &string_size, GX_NULL);

        /* Convert string to integer. */
        string_to_int(input_buffer, string_size, &value);

        if(value > 0)
        {
            gx_text_input_cursor_width_set(&input->gx_single_line_text_input_cursor_instance, value);
            gx_system_dirty_mark((GX_WIDGET *)input);
        }
        break;
    case GX_SIGNAL(ID_CURSOR_HEIGHT_INPUT, GX_EVENT_TEXT_EDITED):
        cursor_height_input = &pMainScreen->window_cursor_height_input;
       
        /* Pick up buffer address and input string size. */
        gx_single_line_text_input_buffer_get(cursor_height_input, &input_buffer, &string_size, GX_NULL);

        /* Convert string to integer. */
        string_to_int(input_buffer, string_size, &value);

        if(value > 0)
        {
            gx_text_input_cursor_height_set(&input->gx_single_line_text_input_cursor_instance, value);
            gx_system_dirty_mark((GX_WIDGET *)input);
        }
        break;

    case GX_SIGNAL(ID_FONT_CHINESE, GX_EVENT_RADIO_DESELECT):
        gx_display_language_table_set_ext(root->gx_window_root_canvas->gx_canvas_display, display_1_language_table, DISPLAY_1_LANGUAGE_TABLE_SIZE, DISPLAY_1_STRING_TABLE_SIZE);
        gx_system_active_language_set(LANGUAGE_ENGLISH);
        break;

    case GX_SIGNAL(ID_FONT_CHINESE, GX_EVENT_RADIO_SELECT):
        gx_display_language_table_set_ext(root->gx_window_root_canvas->gx_canvas_display, display_1_language_table, DISPLAY_1_LANGUAGE_TABLE_SIZE, DISPLAY_1_STRING_TABLE_SIZE);
        gx_system_active_language_set(LANGUAGE_CHINESE);
        font_depth = 3;
        gx_prompt_font_set((GX_PROMPT *)input, font[font_depth][font_size]);
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }
    return GX_SUCCESS;
}

