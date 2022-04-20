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
/**   Canvas Management (Canvas)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_utility.h"
#include "gx_canvas.h"
#include "gx_context.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_canvas_text_draw                                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function prepares to draw text.                                */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    x_start                               X-coordinate, text left edge  */
/*    y_start                               Y-coordinate, text baseline   */
/*    string                                String to draw                */
/*    length                                Length of string to draw      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_string_length_check       Verify string length          */
/*    _gx_canvas_text_draw_ext              Actual text draw function    */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_multi_line_text_view_draw                                       */
/*    _gx_single_line_text_input_draw                                     */
/*    _gx_widget_text_draw                                                */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT  _gx_canvas_text_draw(GX_VALUE x_start, GX_VALUE y_start, GX_CONST GX_CHAR *text, INT length)
{
GX_STRING string;
UINT      count;
UINT      status;

    string.gx_string_ptr = text;

    if (length >= 0)
    {
        string.gx_string_length = (UINT)length;
    }
    else
    {
        status = _gx_utility_string_length_check(text, &count, GX_MAX_STRING_LENGTH);
        if (status != GX_SUCCESS)
        {
            return status;
        }
        string.gx_string_length = count;
    }
    status = _gx_canvas_text_draw_ext(x_start, y_start, &string);
    return status;
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_canvas_text_draw_ext                            PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function prepares to draw text.                                */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    x_start                               X-coordinate, text left edge  */
/*    y_start                               Y-coordinate, text baseline   */
/*    string                                String to draw                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_rectangle_define          Initialize a rectangle        */
/*    _gx_system_string_width_get           Get width of the string in    */
/*                                            pixels                      */
/*    _gx_utility_rectangle_overlap_detect  Determine if two rectangles   */
/*                                            overlap                     */
/*    _gx_canvas_glyphs_draw                Draw glyphs on canvas         */
/*    [gx_display_driver_horizontal_line_draw]                            */
/*                                       The display driver horizontal    */
/*                                         line drawing function          */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_multi_line_text_view_draw                                       */
/*    _gx_single_line_text_input_draw                                     */
/*    _gx_widget_text_draw                                                */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  12-31-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            replaced font format value  */
/*                                            with macro defines,         */
/*                                            resulting in version 6.1.3  */
/*                                                                        */
/**************************************************************************/
UINT  _gx_canvas_text_draw_ext(GX_VALUE x_start, GX_VALUE y_start, GX_CONST GX_STRING *string)
{
GX_RECTANGLE bound;
GX_RECTANGLE clip;
GX_VIEW     *view;
GX_VALUE     width;
GX_POINT     draw_position;
GX_DISPLAY  *display;
VOID         (*glyph_draw)(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area,
                           GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph);
UINT         status = GX_FAILURE;

/* pickup pointer to current context */
GX_DRAW_CONTEXT *context = _gx_system_current_draw_context;

/* get pointer to current font */
GX_FONT *font = context -> gx_draw_context_brush.gx_brush_font;

    width = 0;

    if ((font == GX_NULL) || (font -> gx_font_glyphs.gx_font_normal_glyphs == GX_NULL))
    {
        return GX_INVALID_FONT;
    }
    display = context -> gx_draw_context_display;

    _gx_system_string_width_get_ext(font, string, &width);

    /* calculate bounding rectangle */
    _gx_utility_rectangle_define(&bound, x_start, y_start,
                                 (GX_VALUE)(x_start + width - 1),
                                 (GX_VALUE)(y_start + font -> gx_font_line_height));

    /* check to see if the text is in the dirty area */

    if (!_gx_utility_rectangle_overlap_detect(&bound, &context -> gx_draw_context_dirty, &bound))
    {
        return(GX_SUCCESS);
    }

    /*  loop through viewports, painting text into each that overlaps
        text bound
     */
    view = context -> gx_draw_context_view_head;

    draw_position.gx_point_x = x_start;
    draw_position.gx_point_y = y_start;

    /* use this for underline position, if needed */
    y_start = (GX_VALUE)(y_start + font -> gx_font_baseline + 1);

    switch (font -> gx_font_format & GX_FONT_FORMAT_BPP_MASK)
    {
    case GX_FONT_FORMAT_1BPP:
        if (font -> gx_font_format & GX_FONT_FORMAT_COMPRESSED)
        {
            glyph_draw = display -> gx_display_driver_1bit_compressed_glyph_draw;
        }
        else
        {
            glyph_draw = display -> gx_display_driver_1bit_glyph_draw;
        }
        break;

    case GX_FONT_FORMAT_4BPP:
        if (font -> gx_font_format & GX_FONT_FORMAT_COMPRESSED)
        {
            glyph_draw = display -> gx_display_driver_4bit_compressed_glyph_draw;
        }
        else
        {
            glyph_draw = display -> gx_display_driver_4bit_glyph_draw;
        }
        break;

    case GX_FONT_FORMAT_8BPP:
        if (font -> gx_font_format & GX_FONT_FORMAT_COMPRESSED)
        {
            glyph_draw = display -> gx_display_driver_8bit_compressed_glyph_draw;
        }
        else
        {
            glyph_draw = display -> gx_display_driver_8bit_glyph_draw;
        }
        break;

    default:
        glyph_draw = GX_NULL;
        break;
    }

    if (glyph_draw)
    {
        while (view)
        {
            if (_gx_utility_rectangle_overlap_detect(&view -> gx_view_rectangle, &bound, &clip))
            {
                _gx_canvas_glyphs_draw(context, &draw_position, string, &clip, glyph_draw);

                if (context -> gx_draw_context_brush.gx_brush_style & GX_BRUSH_UNDERLINE)
                {
                    /* underline the text.  */

                    if (clip.gx_rectangle_top <= y_start &&
                        clip.gx_rectangle_bottom >= y_start)
                    {
                        width = context -> gx_draw_context_brush.gx_brush_width;
                        if (width <= 0)
                        {
                            width = 1;
                        }
                        if (y_start + width - 1 > clip.gx_rectangle_bottom)
                        {
                            width = (GX_VALUE)(clip.gx_rectangle_bottom - y_start + 1);
                        }
                        display -> gx_display_driver_horizontal_line_draw(context,
                                                                          clip.gx_rectangle_left,
                                                                          clip.gx_rectangle_right,
                                                                          y_start, width,
                                                                          context -> gx_draw_context_brush.gx_brush_line_color);
                    }
                }
            }

            view = view -> gx_view_next;
        }
        status = GX_SUCCESS;
    }

    return(status);
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_canvas_aligned_text_draw                        PORTABLE C      */
/*                                                           6.1.11       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function draws text to canvas with specified alignment style.  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    string                                String to draw                */
/*    rectangle                             Drawing area                  */
/*    alignment                             Alignment style               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_context_brush_get                 Get context brush             */
/*    _gx_system_string_width_get_ext       Get string width              */
/*    _gx_canvas_text_draw_ext              Actual text draw function     */
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
/*  04-25-2022     Ting Zhu                 Initial Version 6.1.11        */
/*                                                                        */
/**************************************************************************/
UINT  _gx_canvas_aligned_text_draw(GX_CONST GX_STRING *string, GX_RECTANGLE *rectangle, ULONG alignment)
{
GX_VALUE  text_width;
GX_VALUE  text_height;
GX_VALUE  x_pos;
GX_VALUE  y_pos;
GX_BRUSH *brush;
GX_VALUE  rect_width;
GX_VALUE  rect_height;

    _gx_context_brush_get(&brush);

    if (!brush -> gx_brush_font)
    {
        return GX_INVALID_FONT;
    }

    text_height = brush -> gx_brush_font -> gx_font_line_height;

    rect_width = (GX_VALUE)(rectangle -> gx_rectangle_right - rectangle -> gx_rectangle_left + 1);
    rect_height = (GX_VALUE)(rectangle -> gx_rectangle_bottom - rectangle -> gx_rectangle_top + 1);

    x_pos = rectangle -> gx_rectangle_left;
    y_pos = rectangle -> gx_rectangle_top;
    y_pos = (GX_VALUE)(y_pos + (rect_height - text_height) / 2);

    switch (alignment & GX_STYLE_TEXT_ALIGNMENT_MASK)
    {
    case GX_STYLE_TEXT_RIGHT:
        _gx_system_string_width_get_ext(brush -> gx_brush_font, string, &text_width);
        x_pos = (GX_VALUE)(x_pos + rect_width - 1);
        x_pos = (GX_VALUE)(x_pos - text_width);
        break;

    case GX_STYLE_TEXT_LEFT:
        break;

    default:
        _gx_system_string_width_get_ext(brush -> gx_brush_font, string, &text_width);
        x_pos = (GX_VALUE)(x_pos + ((rect_width - text_width) / 2));
        break;
    }

    /* Draw the text.  */
    return _gx_canvas_text_draw_ext(x_pos, y_pos, string);
}

