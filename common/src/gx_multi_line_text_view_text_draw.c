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
/**   Multi Line Text View Management (Multi Line Text View)              */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_canvas.h"
#include "gx_context.h"
#include "gx_multi_line_text_view.h"
#include "gx_utility.h"
#include "gx_widget.h"
#include "gx_window.h"
#include "gx_scrollbar.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_view_draw                       PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function draws text for a multi-line-text-view widget.         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_view                              Multi-line_text_view widget  */
/*                                             control block              */
/*    text_color                             ID of text color             */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_context_line_color_set            Set the context color         */
/*    _gx_context_font_get                  Get font associated with the  */
/*                                            specified ID                */
/*    _gx_context_font_set                  Set the context font          */
/*    _gx_context_brush_width_set           Set the brush width           */
/*    _gx_multi_line_text_view_visible_rows_compute                       */
/*                                          Calculate visible rows        */
/*    _gx_multi_line_text_view_string_total_rows_compute                  */
/*                                          Calculate total rows for input*/
/*                                            string                      */
/*    _gx_utility_rectangle_resize          Offset rectangle by specified */
/*                                            value                       */
/*    _gx_utility_rectangle_overlap_detect  Detect rectangle overlaps     */
/*    _gx_utility_string_length_check       Test string length            */
/*    _gx_system_string_get                 Get string by specified id    */
/*    _gx_system_private_string_get         Get string pointer in         */
/*                                            dynamically copied string   */
/*                                            buffer                      */
/*    _gx_multi_line_text_view_paragraph_start_get                        */
/*                                          Get start index of a paragraph*/
/*    _gx_utility_bidi_paragraph_reorder    Reorder bidi text for display */
/*    _gx_system_string_width_get           Get string width              */
/*    _gx_canvas_text_draw                  Draw the text                 */
/*    _gx_canvas_drawing_initiate           Initiate a drawing context    */
/*    _gx_canvas_drawing_complete           Complete a drawing            */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            support runtime Arabic      */
/*                                            line breaking,              */
/*                                            modified logic of dynamic   */
/*                                            bidi text draw,             */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
VOID  _gx_multi_line_text_view_text_draw(GX_MULTI_LINE_TEXT_VIEW *text_view, GX_RESOURCE_ID text_color)
{
INT           index;
INT           line_height;
GX_STRING     string;
GX_STRING     line_string;
INT           x_pos;
INT           y_pos;
GX_RECTANGLE  client;
GX_RECTANGLE  draw_area;
GX_CANVAS    *canvas;
INT           first_visible_line;
INT           last_visible_line;
UINT          line_start_index;
UINT          line_end_index;
UINT          line_cache_start;
GX_VALUE      text_width;
GX_VALUE      space_width;
GX_VALUE      client_width;
GX_FONT      *font;
GX_SCROLLBAR *scroll;

#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
GX_BIDI_RESOLVED_TEXT_INFO *next = GX_NULL;
UINT                        bidi_text_line_index = 0;
#endif

    _gx_context_line_color_set(text_color);
    _gx_context_font_get(text_view -> gx_multi_line_text_view_font_id, &font);
    _gx_context_font_set(text_view -> gx_multi_line_text_view_font_id);
    _gx_context_brush_width_set(1);

    _gx_window_scrollbar_find((GX_WINDOW *)text_view, GX_TYPE_VERTICAL_SCROLL, &scroll);

    if (text_view -> gx_multi_line_text_view_line_index_old)
    {

        /* Get visible rows. */
        _gx_multi_line_text_view_visible_rows_compute(text_view);

        /* Calculate text total rows. */
        _gx_multi_line_text_view_string_total_rows_compute(text_view);

        if (scroll)
        {
            /* Reset scrollbar.  */
            _gx_scrollbar_reset(scroll, GX_NULL);
        }
        else
        {
            if (text_view -> gx_multi_line_text_view_text_total_rows >
                text_view -> gx_multi_line_text_view_cache_size)
            {
                /* Update line cache. */
                _gx_multi_line_text_view_line_cache_update(text_view);
            }
        }
    }

    /* Is there a string and font?  */
    if ((text_view -> gx_multi_line_text_view_text.gx_string_length <= 0)  ||
        font == GX_NULL)
    {
        return;
    }

    /* Pickup text height. */
    line_height = font -> gx_font_line_height + text_view -> gx_multi_line_text_view_line_space;

    if (!line_height)
    {
        return;
    }

    /* pick up current canvas */
    canvas = _gx_system_current_draw_context -> gx_draw_context_canvas;

    /* Pick up client area.  */
    client = text_view -> gx_window_client;

    /* Offset client area by the size of whitespace.  */
    _gx_utility_rectangle_resize(&client, (GX_VALUE)(-text_view -> gx_multi_line_text_view_whitespace));

    /* check for auto-scrolling vertically centered text */
    if (text_view -> gx_widget_type == GX_TYPE_MULTI_LINE_TEXT_VIEW &&
        scroll == GX_NULL &&
        (text_view -> gx_widget_style & GX_STYLE_VALIGN_CENTER) == GX_STYLE_VALIGN_CENTER)
    {
        space_width = (GX_VALUE)(client.gx_rectangle_bottom - client.gx_rectangle_top);
        space_width = (GX_VALUE)((INT)space_width - (INT)((INT)text_view -> gx_multi_line_text_view_text_total_rows * line_height));
        text_view -> gx_multi_line_text_view_text_scroll_shift = (space_width >> 1);
    }

    _gx_utility_rectangle_overlap_detect(&_gx_system_current_draw_context -> gx_draw_context_dirty, &client, &draw_area);
    _gx_canvas_drawing_initiate(canvas, (GX_WIDGET *)text_view, &draw_area);

    /* Compute the start displaying position of pixels in x direction and y direction. */
    y_pos = client.gx_rectangle_top;
    y_pos += text_view -> gx_multi_line_text_view_text_scroll_shift;
    y_pos += (text_view -> gx_multi_line_text_view_line_space >> 1);

    line_string.gx_string_ptr = " ";
    line_string.gx_string_length = 1;

    _gx_system_string_width_get_ext(font, &line_string, &space_width);

    first_visible_line = ((-text_view -> gx_multi_line_text_view_text_scroll_shift)) / line_height;

    if (first_visible_line < 0)
    {
        first_visible_line = 0;
    }

    last_visible_line = first_visible_line + (INT)(text_view -> gx_multi_line_text_view_text_visible_rows);

    if (last_visible_line > (INT)(text_view -> gx_multi_line_text_view_text_total_rows - 1))
    {
        last_visible_line = (INT)(text_view -> gx_multi_line_text_view_text_total_rows - 1);
    }

    if (text_view -> gx_multi_line_text_view_text_id)
    {
        _gx_widget_string_get_ext((GX_WIDGET *)text_view, text_view -> gx_multi_line_text_view_text_id, &string);
    }
    else
    {
        _gx_system_private_string_get(&text_view -> gx_multi_line_text_view_text, &string, text_view -> gx_widget_style);
    }

    y_pos += (INT)(first_visible_line * line_height);

    for (index = first_visible_line; index <= last_visible_line; index++)
    {
#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
        if (_gx_system_bidi_text_enabled)
        {
            line_string.gx_string_ptr = GX_NULL;
            line_string.gx_string_length = 0;

            if (!next)
            {
                next = text_view -> gx_multi_line_text_view_bidi_resolved_text_info;
            }

            while (next)
            {
                if (bidi_text_line_index + next -> gx_bidi_resolved_text_info_total_lines > (UINT)index)
                {
                    /* Get line string. */
                    if (next -> gx_bidi_resolved_text_info_text)
                    {
                        line_string = next -> gx_bidi_resolved_text_info_text[(UINT)index - bidi_text_line_index];
                    }
                    break;
                }

                bidi_text_line_index += next -> gx_bidi_resolved_text_info_total_lines;
                next = next -> gx_bidi_resolved_text_info_next;
            }
        }
        else
        {
#endif /* defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT) */
            line_cache_start = text_view -> gx_multi_line_text_view_first_cache_line;
            line_start_index = text_view -> gx_multi_line_text_view_line_index[index - (INT)line_cache_start];

            if ((INT)(index - (INT)line_cache_start) >= (INT)(text_view -> gx_multi_line_text_view_cache_size - 1))
            {
                line_end_index = text_view -> gx_multi_line_text_view_text.gx_string_length;
            }
            else
            {
                line_end_index = text_view -> gx_multi_line_text_view_line_index[index - (INT)(line_cache_start) + 1];
            }

            /* Get line string. */
            line_string.gx_string_ptr = string.gx_string_ptr + line_start_index;
            line_string.gx_string_length = line_end_index - line_start_index;
#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
        }
#endif /* defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT) */


        switch (text_view -> gx_widget_style & GX_STYLE_TEXT_ALIGNMENT_MASK)
        {
        case GX_STYLE_TEXT_RIGHT:
            _gx_system_string_width_get_ext(font, &line_string, &text_width);
            while (text_width > (client.gx_rectangle_right - client.gx_rectangle_left - 2))
            {
                text_width = (GX_VALUE)(text_width - space_width);
            }
            x_pos = client.gx_rectangle_right - 1;
            x_pos = (GX_VALUE)(x_pos - text_width);
            break;
        case GX_STYLE_TEXT_LEFT:
            x_pos = client.gx_rectangle_left + 1;
            break;
        case GX_STYLE_TEXT_CENTER:
        default:
            _gx_system_string_width_get_ext(font, &line_string, &text_width);
            client_width = (GX_VALUE)(client.gx_rectangle_right - client.gx_rectangle_left + 1);
            while (text_width > (client_width - 3))
            {
                text_width = (GX_VALUE)(text_width - space_width);
            }
            x_pos = (GX_VALUE)(client.gx_rectangle_left + ((client_width - text_width) / 2));
            break;
        }

        /* Draw the text. */
        _gx_canvas_text_draw_ext((GX_VALUE)x_pos, (GX_VALUE)y_pos, &line_string);
        y_pos += line_height;
    }

    _gx_canvas_drawing_complete(canvas, GX_FALSE);
}

