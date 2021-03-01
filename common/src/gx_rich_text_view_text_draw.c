/**************************************************************************/
/*                                                                        */
/*       Copyright (c) Microsoft Corporation. All rights reserved.        */
/*                                                                        */
/*       This software is licensed under the Microsoft Software License   */
/*       Terms for Microsoft Azure RTOS. Full text of the license can be  */
/*       found in the LICENSE file at https://aka.ms/AzureRTOS_EULA       */
/*       and in the root directory of this software.                      */
/*                                                                        */
/**************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** GUIX Component                                                        */
/**                                                                       */
/**   Rich Text View Management (Rich Text View)                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_canvas.h"
#include "gx_rich_text_view.h"
#include "gx_window.h"
#include "gx_utility.h"
#include "gx_widget.h"
#include "gx_context.h"
#include "gx_scrollbar.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_rich_text_view_single_line_draw                 PORTABLE C      */
/*                                                           6.1.5        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function draws a single line text for a rich text view widget. */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_view                              Rich text view control block */
/*    ypos                                   Draw start y position        */
/*    line_info                              Text information for drawing */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_rich_text_view_tag_enter          Detect rich text enter tag    */
/*    _gx_system_rich_text_format_pop       Pop rich text format          */
/*    _gx_system_rich_text_format_push      Push rich text format         */
/*    _gx_widget_font_get                   Retrieve font by font id      */
/*    _gx_system_string_width_get           Get string width              */
/*    _gx_canvas_text_draw                  Draw text                     */
/*    _gx_canvas_line_draw                  Draw line                     */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_rich_text_view_draw                                             */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  09-30-2020     Kenneth Maxwell          Initial Version 6.1           */
/*  03-02-2021     Ting Zhu                 Modified comment(s),          */
/*                                            removed unreachable code,   */
/*                                            resulting in version 6.1.5  */
/*                                                                        */
/**************************************************************************/
static VOID _gx_rich_text_view_single_line_draw(GX_RICH_TEXT_VIEW *text_view, GX_VALUE ypos, GX_RICH_TEXT_LINE_INFO *line_info)
{
GX_STRING           text = line_info -> gx_rich_text_line_info_text;
GX_STRING           draw_text;
GX_UBYTE            processed_count;
GX_FONT            *font;
GX_BOOL             draw_start = GX_FALSE;
GX_VALUE            text_width;
GX_VALUE            xpos;
GX_VALUE            y;
GX_RICH_TEXT_FORMAT start_format;
GX_RICH_TEXT_FORMAT format = line_info -> gx_rich_text_line_info_start_format;
GX_RECTANGLE        client = text_view -> gx_window_client;
GX_VALUE            client_width;
GX_RECTANGLE        draw_area;
GX_BOOL             escape = GX_FALSE;

    /* Offset client area by the size of whitespace.  */
    _gx_utility_rectangle_resize(&client, (GX_VALUE)(-text_view -> gx_multi_line_text_view_whitespace));

    /* Calculate client width. */
    client_width = (GX_VALUE)(client.gx_rectangle_right - client.gx_rectangle_left);

    /* Process proceding styles. */
    while (text.gx_string_ptr[0] == '<')
    {
        if ((_gx_rich_text_view_tag_enter(text_view, &text, &format, &processed_count) == GX_SUCCESS))
        {
            text.gx_string_ptr += processed_count;
            text.gx_string_length -= processed_count;
        }
        else
        {
            break;
        }
    }

    start_format = format;

    /* Calculate draw start x coordinate. */
    switch (format.gx_rich_text_flags & GX_RICH_TEXT_ALIGN_MASK)
    {
    case GX_RICH_TEXT_RIGHT:
        xpos = (GX_VALUE)(client.gx_rectangle_right - 1);
        xpos = (GX_VALUE)(xpos - (INT)line_info -> gx_rich_text_line_info_text_width);
        break;

    case GX_RICH_TEXT_CENTER:
        xpos = (GX_VALUE)((client_width - (INT)line_info -> gx_rich_text_line_info_text_width) / 2);
        xpos = (GX_VALUE)(xpos + client.gx_rectangle_left);
        break;

    case GX_RICH_TEXT_LEFT:
    default:
        xpos = (GX_VALUE)(client.gx_rectangle_left + 1);
        break;
    }

    draw_text.gx_string_ptr = text.gx_string_ptr;
    draw_text.gx_string_length = 0;

    /* Calculate the total rows of text view string. */
    while (text.gx_string_length > 0)
    {
        processed_count = 1;

        if ((!escape) && (text.gx_string_ptr[0] == '\\'))
        {
            escape = GX_TRUE;

            if (draw_text.gx_string_length == 0)
            {
                draw_text.gx_string_ptr++;
            }
            else
            {
                draw_start = GX_TRUE;
            }
        }
        else
        {
            /* Test tags. */
            if ((!escape) &&
                (text.gx_string_ptr[0] == '<') &&
                (_gx_rich_text_view_tag_enter(text_view, &text, &format, &processed_count) == GX_SUCCESS))
            {
                if (draw_text.gx_string_length == 0)
                {
                    /* Skip tags. */
                    draw_text.gx_string_ptr += processed_count;

                    /* Set text draw style to current draw style. */
                    start_format = format;
                }
                else
                {
                    /* Draw text before current tag. */
                    draw_start = GX_TRUE;
                }
            }
            else
            {
                draw_text.gx_string_length += processed_count;
            }

            escape = GX_FALSE;
        }

        text.gx_string_ptr += processed_count;
        text.gx_string_length -= processed_count;

        /* If we come the the last character, or draw_text falg is true, process drawing. */
        if ((text.gx_string_length == 0) || draw_start)
        {
            /* Pick up text font. */
            _gx_context_font_get(start_format.gx_rich_text_font_id, &font);

            /* Pict up text width. */
            _gx_system_string_width_get_ext(font, &draw_text, &text_width);

            /* Set text color. */
            _gx_context_line_color_set(start_format.gx_rich_text_color);

            /* Set font. */
            _gx_context_font_set(start_format.gx_rich_text_font_id);

            if (start_format.gx_rich_text_highlight_color != text_view -> gx_widget_normal_fill_color)
            {
                _gx_context_brush_width_set(0);

                /* Draw highlight background. */
                _gx_context_fill_color_set(start_format.gx_rich_text_highlight_color);

                draw_area.gx_rectangle_left = (GX_VALUE)xpos;
                draw_area.gx_rectangle_right = (GX_VALUE)(xpos + text_width - 1);
                draw_area.gx_rectangle_top = (GX_VALUE)(ypos - (text_view -> gx_multi_line_text_view_line_space >> 1));
                draw_area.gx_rectangle_bottom = (GX_VALUE)(draw_area.gx_rectangle_top + line_info -> gx_rich_text_line_info_line_height - 1);

                _gx_canvas_rectangle_draw(&draw_area);
            }

            y = (GX_VALUE)(ypos + line_info -> gx_rich_text_line_info_baseline - font -> gx_font_baseline);

            /* Draw text. */
            _gx_canvas_text_draw_ext(xpos, y, &draw_text);

            /* Test italic flag is valid. */
            if (start_format.gx_rich_text_flags & GX_RICH_TEXT_UNDERLINE)
            {
                y = (GX_VALUE)(ypos + line_info -> gx_rich_text_line_info_baseline + 1);

                _gx_context_brush_width_set(1);

                /* Draw underline. */
                _gx_canvas_line_draw(xpos, y, (GX_VALUE)(xpos + text_width - 1), y);
            }

            /* Advance draw start x position by text width. */
            xpos = (GX_VALUE)(xpos + text_width);

            draw_text.gx_string_ptr = text.gx_string_ptr;
            draw_text.gx_string_length = 0;

            /* Set draw text flag to false. */
            draw_start = GX_FALSE;
            start_format = format;
        }
    }

    line_info -> gx_rich_text_line_info_end_format = format;
}


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_rich_text_view_text_draw                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function draws text for a rich text view widget.               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_view                              Rich text view control block */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_window_scrollbar_find             Find scroll bar for specified */
/*                                          window.                       */
/*    _gx_rich_text_view_text_total_height_calculate                      */
/*                                          Calculate text total height   */
/*    _gx_scrollbar_reset                   Reset scrollbar               */
/*    _gx_utility_rectangle_resize          Resize rectangle              */
/*    _gx_widget_string_get_ext             Retrieve text by id           */
/*    _gx_widget_font_get                   Retireve font by id           */
/*    _gx_system_private_string_get         Retreive string pointer       */
/*    _gx_system_rich_text_format_stack_clear                             */
/*                                          Clear rich text format stack  */
/*    _gx_system_rich_text_format_stack_switch                            */
/*                                          Switch rich text format stack */
/*    _gx_rich_text_view_line_info_get      Get one line text for drawing */
/*    _gx_rich_text_view_single_line_text_draw                            */
/*                                          Draw a single line            */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
/*    Application Code                                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  09-30-2020     Kenneth Maxwell          Initial Version 6.1           */
/*                                                                        */
/**************************************************************************/
VOID  _gx_rich_text_view_text_draw(GX_RICH_TEXT_VIEW *text_view)
{
GX_STRING              text;
GX_FONT               *font;
GX_SCROLLBAR          *scroll;
GX_RICH_TEXT_LINE_INFO line_info;
GX_RICH_TEXT_FORMAT    format;
GX_VALUE               client_width;
GX_RECTANGLE           client;
GX_VALUE               ypos;
GX_CANVAS             *canvas;
GX_RECTANGLE           draw_area;

    if (!text_view -> gx_multi_line_text_view_text.gx_string_length)
    {
        /* Nothing to draw. */
        return;
    }

    if (text_view -> gx_multi_line_text_view_line_index_old)
    {

        /* Rich text layout changes, we might need to reset scrollbar. */
        _gx_window_scrollbar_find((GX_WINDOW *)text_view, GX_TYPE_VERTICAL_SCROLL, &scroll);

        if (scroll)
        {
            /* Update rich text view text total height.  */
            _gx_rich_text_view_text_total_height_calculate(text_view);

            /* Reset scrollbar.  */
            _gx_scrollbar_reset(scroll, GX_NULL);
        }
    }

    /* Pick up client retangle. */
    client = text_view -> gx_window_client;

    /* Offset client area by the size of whitespace.  */
    _gx_utility_rectangle_resize(&client, (GX_VALUE)(-text_view -> gx_multi_line_text_view_whitespace));

    /* Calculate text display width. */
    client_width = (GX_VALUE)(client.gx_rectangle_right - client.gx_rectangle_left - 1);

    /* Set default draw style. */
    format.gx_rich_text_color = text_view -> gx_multi_line_text_view_normal_text_color;
    format.gx_rich_text_highlight_color = text_view -> gx_widget_normal_fill_color;
    format.gx_rich_text_font_id = text_view -> gx_rich_text_view_fonts.gx_rich_text_fonts_normal_id;
    format.gx_rich_text_flags = 0;

    switch (text_view -> gx_widget_style & GX_STYLE_TEXT_ALIGNMENT_MASK)
    {
    case GX_STYLE_TEXT_CENTER:
        format.gx_rich_text_flags |= GX_RICH_TEXT_CENTER;
        break;

    case GX_STYLE_TEXT_RIGHT:
        format.gx_rich_text_flags |= GX_RICH_TEXT_RIGHT;
        break;

    default:
        /* Do nothing. */
        break;
    }

    /* Pick up text for drawing. */
    if (text_view -> gx_multi_line_text_view_text_id)
    {
        _gx_widget_string_get_ext((GX_WIDGET *)text_view, text_view -> gx_multi_line_text_view_text_id, &text);
    }
    else
    {
        _gx_system_private_string_get(&text_view -> gx_multi_line_text_view_text, &text, text_view -> gx_widget_style);
    }

    ypos = client.gx_rectangle_top;
    ypos = (GX_VALUE)(ypos + text_view -> gx_multi_line_text_view_text_scroll_shift);
    ypos = (GX_VALUE)(ypos + (text_view -> gx_multi_line_text_view_line_space >> 1));

    /* pick up current canvas */
    canvas = _gx_system_current_draw_context -> gx_draw_context_canvas;

    _gx_utility_rectangle_overlap_detect(&_gx_system_current_draw_context -> gx_draw_context_dirty, &client, &draw_area);
    _gx_canvas_drawing_initiate(canvas, (GX_WIDGET *)text_view, &draw_area);

    /* Calculate the total rows of text view string. */
    while (text.gx_string_length > 0)
    {
        /* Pickup draw font. */
        _gx_widget_font_get((GX_WIDGET *)text_view, format.gx_rich_text_font_id, &font);

        if (!font)
        {
            break;
        }

        line_info.gx_rich_text_line_info_text.gx_string_ptr = text.gx_string_ptr;
        line_info.gx_rich_text_line_info_text.gx_string_length = 0;
        line_info.gx_rich_text_line_info_start_format = format;
        line_info.gx_rich_text_line_info_end_format = format;
        line_info.gx_rich_text_line_info_line_height = font -> gx_font_line_height;
        line_info.gx_rich_text_line_info_baseline = font -> gx_font_baseline;
        line_info.gx_rich_text_line_info_text_width = 0;

        _gx_rich_text_view_context_save();

        if (_gx_rich_text_view_line_info_get(text_view, text, &line_info, client_width) != GX_SUCCESS)
        {
            break;
        }

        format = line_info.gx_rich_text_line_info_end_format;

        /* Draw text. */
        if ((GX_VALUE)(ypos + line_info.gx_rich_text_line_info_line_height) > client.gx_rectangle_top)
        {
            if ((GX_VALUE)(ypos) < client.gx_rectangle_bottom)
            {
                _gx_rich_text_view_context_restore();
                _gx_rich_text_view_single_line_draw(text_view, ypos, &line_info);
            }
            else
            {
                break;
            }
        }

        ypos = (GX_VALUE)(ypos + line_info.gx_rich_text_line_info_line_height);
        ypos = (GX_VALUE)(ypos + text_view -> gx_multi_line_text_view_line_space);

        text.gx_string_ptr += line_info.gx_rich_text_line_info_text.gx_string_length;
        text.gx_string_length -= line_info.gx_rich_text_line_info_text.gx_string_length;
    }
    _gx_canvas_drawing_complete(canvas, GX_FALSE);

    _gx_rich_text_view_context_reset();
}

