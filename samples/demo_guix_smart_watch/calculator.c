/* This is a small demo of the high-performance GUIX graphics framework. */

#include "demo_guix_smart_watch.h"

#define TYPE_OPERAND       1
#define TYPE_OPERATION     2
#define TYPE_DOT           3
#define TYPE_FUNCTION      4

GX_BOOL is_string_float(GX_CHAR *string);
GX_BOOL do_input_buffer_clear = GX_FALSE;
GX_BOOL is_input_value_float = GX_FALSE;

typedef struct
{
    GX_CHAR *text;
    USHORT   widget_id;
    USHORT   event_type;
    LONG     type;
} KEY_INFO_ENTRY;

KEY_INFO_ENTRY key_info[] = { 
    { "0", ID_CALCULATOR_0, GX_SIGNAL(ID_CALCULATOR_0, GX_EVENT_CLICKED), TYPE_OPERAND },
    { "1", ID_CALCULATOR_1, GX_SIGNAL(ID_CALCULATOR_1, GX_EVENT_CLICKED), TYPE_OPERAND },
    { "2", ID_CALCULATOR_2, GX_SIGNAL(ID_CALCULATOR_2, GX_EVENT_CLICKED), TYPE_OPERAND },
    { "3", ID_CALCULATOR_3, GX_SIGNAL(ID_CALCULATOR_3, GX_EVENT_CLICKED), TYPE_OPERAND },
    { "4", ID_CALCULATOR_4, GX_SIGNAL(ID_CALCULATOR_4, GX_EVENT_CLICKED), TYPE_OPERAND },
    { "5", ID_CALCULATOR_5, GX_SIGNAL(ID_CALCULATOR_5, GX_EVENT_CLICKED), TYPE_OPERAND },
    { "6", ID_CALCULATOR_6, GX_SIGNAL(ID_CALCULATOR_6, GX_EVENT_CLICKED), TYPE_OPERAND },
    { "7", ID_CALCULATOR_7, GX_SIGNAL(ID_CALCULATOR_7, GX_EVENT_CLICKED), TYPE_OPERAND },
    { "8", ID_CALCULATOR_8, GX_SIGNAL(ID_CALCULATOR_8, GX_EVENT_CLICKED), TYPE_OPERAND },
    { "9", ID_CALCULATOR_9, GX_SIGNAL(ID_CALCULATOR_9, GX_EVENT_CLICKED), TYPE_OPERAND },
    { "+", ID_CALCULATOR_PLUS, GX_SIGNAL(ID_CALCULATOR_PLUS, GX_EVENT_CLICKED), TYPE_OPERATION },
    { "-", ID_CALCULATOR_MINUS, GX_SIGNAL(ID_CALCULATOR_MINUS, GX_EVENT_CLICKED), TYPE_OPERATION },
    { "*", ID_CALCULATOR_MULTI, GX_SIGNAL(ID_CALCULATOR_MULTI, GX_EVENT_CLICKED), TYPE_OPERATION },
    { "/", ID_CALCULATOR_DIVIDE, GX_SIGNAL(ID_CALCULATOR_DIVIDE, GX_EVENT_CLICKED), TYPE_OPERATION },
    { ".", ID_CALCULATOR_DOT, GX_SIGNAL(ID_CALCULATOR_DOT, GX_EVENT_CLICKED), TYPE_DOT },
    { "C", ID_CALCULATOR_C, 0, TYPE_FUNCTION },
    { "+/-", ID_CALCULATOR_SIGN, 0, TYPE_FUNCTION },
    { "=", ID_CALCULATOR_RESULT, 0, TYPE_FUNCTION },
    { "%", ID_CALCULATOR_PERCENT, 0, TYPE_FUNCTION },
    { GX_NULL, 0, 0, 0 }
};


/*************************************************************************************/
VOID calculator_pixelmap_button_draw(GX_PIXELMAP_BUTTON *key)
{
    /* Custom pixelmap button draw. */
    GX_RESOURCE_ID  font_id = GX_FONT_ID_MIDDLE_30;
    KEY_INFO_ENTRY *pKeyInfo = key_info;
    INT             x_offset = 0;
    INT             y_offset = 0;
    GX_PIXELMAP    *map;
    GX_COLOR        fill_color;
    GX_STRING       string;

    gx_context_pixelmap_get(key->gx_pixelmap_button_normal_id, &map);

    while (pKeyInfo->text)
    {
        if (pKeyInfo->widget_id == key->gx_widget_id)
        {
            if (key->gx_widget_style & GX_STYLE_BUTTON_PUSHED)
            {
                fill_color = key->gx_widget_selected_fill_color;
                x_offset++;
                y_offset++;
            }
            else
            {
                fill_color = key->gx_widget_normal_fill_color;
            }

            if (map)
            {
                gx_context_fill_color_set(fill_color);
                gx_canvas_pixelmap_draw(key->gx_widget_size.gx_rectangle_left + x_offset,
                    key->gx_widget_size.gx_rectangle_top + y_offset, map);
            }
            else
            {
                string.gx_string_ptr = pKeyInfo->text;
                string.gx_string_length = string_length_get(string.gx_string_ptr, MAX_KEY_LENGTH);
                gx_widget_text_draw_ext(key, fill_color,
                    font_id, &string,
                    x_offset, y_offset);
            }
            break;
        }

        pKeyInfo++;
    }
}

/*************************************************************************************/
UINT calculator_screen_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    GX_SINGLE_LINE_TEXT_INPUT *text_input;
    KEY_INFO_ENTRY            *pKeyInfo = key_info;
    GX_CHAR                    pre_char = '\0';
    INT                        string_size;
    GX_CHAR                   *result;

    text_input = &calculator_screen.calculator_screen_text_input;

    if ((event_ptr->gx_event_type & GX_SIGNAL_EVENT_MASK) == GX_EVENT_CLICKED)
    {
        while (pKeyInfo->text)
        {
            if (pKeyInfo->event_type == event_ptr->gx_event_type)
            {
                if (do_input_buffer_clear)
                {
                    if ((pKeyInfo->text[0] >= '0') && (pKeyInfo->text[0] <= '9'))
                    {
                        /* Clear input buffer.  */
                        gx_single_line_text_input_buffer_clear(text_input);
                    }
                    else if (!strcmp(text_input->gx_single_line_text_input_buffer, "Error Format"))
                    {
                        return 0;
                    }

                    do_input_buffer_clear = GX_FALSE;
                }

                string_size = text_input->gx_single_line_text_input_string_size;

                if (string_size)
                {
                    /* Get previous character.  */
                    pre_char = text_input->gx_single_line_text_input_buffer[string_size - 1];
                }

                if (pKeyInfo->type == TYPE_OPERAND)
                {
                    if ((pre_char == '0') && (string_size == 1))
                    {
                        gx_single_line_text_input_backspace(text_input);
                    }
                    else if ((pre_char == '/') && (pKeyInfo->text[0] == '0'))
                    {
                        return 0;
                    }

                    gx_single_line_text_input_character_insert(text_input, (GX_UBYTE *)&pKeyInfo->text[0], 1);
                }
                else if (string_size)
                {
                    if ((pre_char >= '0') && (pre_char <= '9'))
                    {
                        if (pKeyInfo->type == TYPE_OPERATION)
                        {
                            gx_single_line_text_input_character_insert(text_input, (GX_UBYTE *)&pKeyInfo->text[0], 1);
                            is_input_value_float = GX_FALSE;
                        }
                        else if ((pKeyInfo->type == TYPE_DOT) && !is_input_value_float)
                        {
                            gx_single_line_text_input_character_insert(text_input, (GX_UBYTE *)&pKeyInfo->text[0], 1);
                            is_input_value_float = GX_TRUE;
                        }
                    }
                }

                return 0;
            }
            pKeyInfo++;
        }
    }

    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_CALCULATOR_C, GX_EVENT_CLICKED):
        /* Clear input buffer. */
        gx_single_line_text_input_buffer_clear(text_input);
        is_input_value_float = GX_FALSE;
        break;

    case GX_SIGNAL(ID_CALCULATOR_RESULT, GX_EVENT_CLICKED):
        if (string_length_get(text_input->gx_single_line_text_input_buffer, MAX_EXPRESSION_LENGTH) > 0)
        {
            /* Evaluate expression. */
            result = expression_evaluate(text_input->gx_single_line_text_input_buffer, GX_FALSE);
            is_input_value_float = is_string_float(result);

            /* Clear input buffer. */
            gx_single_line_text_input_buffer_clear(text_input);

            /* Show result. */
            gx_single_line_text_input_character_insert(text_input, (GX_UBYTE *)result, string_length_get(result, MAX_EXPRESSION_LENGTH));

            do_input_buffer_clear = GX_TRUE;
        }
        break;

    case GX_SIGNAL(ID_CALCULATOR_PERCENT, GX_EVENT_CLICKED):
        if (string_length_get(text_input->gx_single_line_text_input_buffer, MAX_EXPRESSION_LENGTH) > 0)
        {
            result = expression_evaluate(text_input->gx_single_line_text_input_buffer, GX_TRUE);
            is_input_value_float = is_string_float(result);

            /* Clear input buffer. */
            gx_single_line_text_input_buffer_clear(text_input);

            /* Show result. */
            gx_single_line_text_input_character_insert(text_input, (GX_UBYTE *)result, string_length_get(result, MAX_EXPRESSION_LENGTH));

            do_input_buffer_clear = GX_TRUE;
        }
        break;

    default:
        return template_main_event_handler(window, event_ptr);
    }

    return 0;
}

/* Check if a number string a float value. */
GX_BOOL is_string_float(GX_CHAR *string)
{
    INT index = 0;
    while (string[index])
    {
        if (string[index] == '.')
        {
            return GX_TRUE;
        }

        index++;
    }

    return GX_FALSE;
}
