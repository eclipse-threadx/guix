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

#if defined(GX_FONT_KERNING_SUPPORT)
/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_canvas_kerning_glyphs_draw                      PORTABLE C      */
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
/*    context                               Drawing context               */
/*    font                                  Font used by the string       */
/*    draw_position                         Coord of draw position        */
/*    string                                Pointer to string which need  */
/*                                            to draw                     */
/*    length                                Number of string about to draw*/
/*    view                                  Pointer to view size          */
/*    draw_glyph                            Callback pointer to display   */
/*                                            driver text draw function   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_utf8_string_character_get                               */
/*                                    Get characters of this string       */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_canvas_glyph_draw                                               */
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
static VOID _gx_canvas_kerning_glyphs_draw(GX_DRAW_CONTEXT *context, GX_FONT *font, GX_POINT *draw_position, GX_CONST GX_STRING *string, GX_RECTANGLE *view,
                                           VOID (*draw_glyph)(GX_DRAW_CONTEXT *, GX_RECTANGLE *, GX_POINT *, GX_CONST GX_GLYPH *))
{
GX_CONST GX_KERNING_GLYPH *glyph;
GX_CHAR_CODE               char_val;
GX_VALUE                   x_offset;
GX_VALUE                   y_offset;
GX_VALUE                   xstart;
GX_VALUE                   ystart;
GX_POINT                   map_offset;
GX_RECTANGLE               draw_area;
GX_CONST GX_FONT          *font_link;

/* Used for kerning glyph. */
GX_CHAR_CODE       pre_char_val = 0;
GX_BYTE            kerning_offset = 0;
GX_CONST GX_UBYTE *kerning_table;
INT                kerning_counts;
INT                index;
GX_UBYTE          *left_glyph_ptr;
GX_STRING          string_copy;

#if defined(GX_UTF8_SUPPORT)
UINT ret;
#endif /* GX_UTF8_SUPPORT */

    /* Setup local variables.  */

    xstart = draw_position -> gx_point_x;
    ystart = draw_position -> gx_point_y;
    string_copy = *string;

    /* for each character in the string */
    do
    {
#ifdef GX_UTF8_SUPPORT
        ret = _gx_utility_utf8_string_character_get(&string_copy, &char_val, GX_NULL);

        if ((ret != GX_SUCCESS) || (char_val == 0))
#else
        char_val = (GX_CHAR_CODE)(*string_copy.gx_string_ptr++);
        string_copy.gx_string_length--;

        if (char_val == 0)
#endif /* GX_UTF8_SUPPORT */
        {
            break;
        }

        font_link = font;
        while (font_link)
        {
            if (char_val >= font_link -> gx_font_first_glyph &&
                char_val <= font_link -> gx_font_last_glyph)
            {
                break;
            }
            font_link = font_link -> gx_font_next_page;
        }

        if (font_link)
        {
            char_val = (GX_CHAR_CODE)(char_val - font_link -> gx_font_first_glyph);

            glyph = &((GX_CONST GX_KERNING_GLYPH *)font_link -> gx_font_glyphs.gx_font_kerning_glyphs)[char_val];
            kerning_table = ((GX_KERNING_GLYPH *)glyph) -> gx_kerning_table;
            if (kerning_table && (pre_char_val != 0))
            {
                /* Search the kerning table for the kerning value. */
                kerning_counts = *kerning_table;
                left_glyph_ptr = (GX_UBYTE *)(kerning_table + 1);

                for (index = 0; index < kerning_counts; index++)
                {
                    if ((*left_glyph_ptr) == (pre_char_val + font_link -> gx_font_first_glyph))
                    {
                        kerning_offset = (GX_CHAR)(*(left_glyph_ptr + 1));
                        break;
                    }
                    left_glyph_ptr += 2;
                }
            }

            if (glyph -> gx_glyph_map)
            {
                x_offset = (GX_VALUE)(xstart + glyph -> gx_glyph_leading);
                x_offset = (GX_VALUE)(x_offset + kerning_offset);
                y_offset = (GX_VALUE)(ystart + font_link -> gx_font_baseline - glyph -> gx_glyph_ascent);

                draw_area.gx_rectangle_left = x_offset;
                draw_area.gx_rectangle_top = y_offset;
                draw_area.gx_rectangle_right = (GX_VALUE)(draw_area.gx_rectangle_left + glyph -> gx_glyph_width - 1);
                draw_area.gx_rectangle_bottom = (GX_VALUE)(draw_area.gx_rectangle_top + glyph -> gx_glyph_height - 1);

                if (draw_area.gx_rectangle_bottom >= view -> gx_rectangle_top &&
                    draw_area.gx_rectangle_top <= view -> gx_rectangle_bottom &&
                    draw_area.gx_rectangle_right >= view -> gx_rectangle_left &&
                    draw_area.gx_rectangle_left <= view -> gx_rectangle_right)
                {
                    map_offset.gx_point_x = 0;
                    map_offset.gx_point_y = 0;
                    /* Calculate the y_start value, which is the offset into the row of
                       the glyph where we start to the draw. */
                    if (draw_area.gx_rectangle_top < view -> gx_rectangle_top)
                    {
                        map_offset.gx_point_y = (GX_VALUE)(view -> gx_rectangle_top - draw_area.gx_rectangle_top);
                        draw_area.gx_rectangle_top = view -> gx_rectangle_top;
                    }

                    if (draw_area.gx_rectangle_left < view -> gx_rectangle_left)
                    {
                        map_offset.gx_point_x = (GX_VALUE)(view -> gx_rectangle_left - x_offset);
                        draw_area.gx_rectangle_left = view -> gx_rectangle_left;
                    }

                    if (draw_area.gx_rectangle_bottom > view -> gx_rectangle_bottom)
                    {
                        draw_area.gx_rectangle_bottom = view -> gx_rectangle_bottom;
                    }
                    if (draw_area.gx_rectangle_right > view -> gx_rectangle_right)
                    {
                        draw_area.gx_rectangle_right = view -> gx_rectangle_right;
                    }

                    draw_glyph(context, &draw_area, &map_offset, (const GX_GLYPH *)glyph);
                }
            }
            xstart = (GX_VALUE)(xstart + glyph -> gx_glyph_advance);
            xstart = (GX_VALUE)(xstart + kerning_offset);
        }

        pre_char_val = char_val;
        kerning_offset = 0;
    } while (string_copy.gx_string_length > 0);
}
#endif


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_canvas_compressed_glyphs_draw                   PORTABLE C      */
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
/*    context                               Drawing context               */
/*    font                                  Font used by the string       */
/*    draw_position                         Coord of draw position        */
/*    string                                Pointer to string which need  */
/*                                            to draw                     */
/*    length                                Number of string about to draw*/
/*    view                                  Pointer to view size          */
/*    draw_glyph                            Callback pointer to display   */
/*                                            driver text draw function   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_utf8_string_character_get                               */
/*                                    Get characters of this string       */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_canvas_glyph_draw                                               */
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
static VOID _gx_canvas_compressed_glyphs_draw(GX_DRAW_CONTEXT *context, GX_FONT *font, GX_POINT *draw_position, GX_CONST GX_STRING *string, GX_RECTANGLE *view,
                                              VOID (*draw_glyph)(GX_DRAW_CONTEXT *, GX_RECTANGLE *, GX_POINT *, GX_CONST GX_GLYPH *))
{
GX_CONST GX_COMPRESSED_GLYPH *glyph;
GX_CHAR_CODE                  char_val;
GX_VALUE                      x_offset;
GX_VALUE                      y_offset;
GX_VALUE                      xstart;
GX_VALUE                      ystart;
GX_POINT                      map_offset;
GX_RECTANGLE                  draw_area;
GX_CONST GX_FONT             *font_link;
GX_STRING                     string_copy;

#ifdef GX_UTF8_SUPPORT
UINT ret;

#ifdef GX_THAI_GLYPH_SHAPING_SUPPORT
GX_CHAR_CODE *code_list = GX_NULL;
UINT          code_count;
UINT          index = 0;
    if (_gx_system_text_render_style & GX_TEXT_RENDER_THAI_GLYPH_SHAPING)
    {
        ret = _gx_utility_thai_glyph_shaping(string, &code_list, &code_count);
    }
#endif

#endif /* GX_UTF8_SUPPORT */

    /* Setup local variables.  */
    xstart = draw_position -> gx_point_x;
    ystart = draw_position -> gx_point_y;
    string_copy = *string;

    /* for each character in the string */
    do
    {
#ifdef GX_UTF8_SUPPORT
#if defined(GX_THAI_GLYPH_SHAPING_SUPPORT)
        if (code_list)
        {
            if (index < code_count)
            {
                char_val = code_list[index++];
            }
            else
            {
                char_val = 0;
            }
        }
        else
        {
#endif
            ret = _gx_utility_utf8_string_character_get(&string_copy, &char_val, GX_NULL);
#if defined(GX_THAI_GLYPH_SHAPING_SUPPORT)
        }
#endif
        if ((ret != GX_SUCCESS) || (char_val == 0))
#else
        char_val = (GX_CHAR_CODE)(*string_copy.gx_string_ptr++);
        string_copy.gx_string_length--;

        if (char_val == 0)
#endif /* GX_UTF8_SUPPORT */
        {
            break;
        }

        font_link = font;
        while (font_link)
        {
            if (char_val >= font_link -> gx_font_first_glyph &&
                char_val <= font_link -> gx_font_last_glyph)
            {
                break;
            }
            font_link = font_link -> gx_font_next_page;
        }

        if (font_link)
        {
            char_val = (GX_CHAR_CODE)(char_val - font_link -> gx_font_first_glyph);
            glyph = &((GX_CONST GX_COMPRESSED_GLYPH *)font_link -> gx_font_glyphs.gx_font_compressed_glyphs)[char_val];

            /* Skip this glyph if glyph map is availlable. */
            if (glyph -> gx_glyph_map)
            {
                x_offset = (GX_VALUE)(xstart + glyph -> gx_glyph_leading);
                y_offset = (GX_VALUE)(ystart + font_link -> gx_font_baseline - glyph -> gx_glyph_ascent);

                draw_area.gx_rectangle_left = x_offset;
                draw_area.gx_rectangle_top = y_offset;
                draw_area.gx_rectangle_right = (GX_VALUE)(draw_area.gx_rectangle_left + glyph -> gx_glyph_width - 1);
                draw_area.gx_rectangle_bottom = (GX_VALUE)(draw_area.gx_rectangle_top + glyph -> gx_glyph_height - 1);

                if (draw_area.gx_rectangle_bottom >= view -> gx_rectangle_top &&
                    draw_area.gx_rectangle_top <= view -> gx_rectangle_bottom &&
                    draw_area.gx_rectangle_right >= view -> gx_rectangle_left &&
                    draw_area.gx_rectangle_left <= view -> gx_rectangle_right)
                {

                    map_offset.gx_point_x = 0;
                    map_offset.gx_point_y = 0;
                    /* Calculate the y_start value, which is the offset into the row of
                       the glyph where we start to the draw. */
                    if (draw_area.gx_rectangle_top < view -> gx_rectangle_top)
                    {
                        map_offset.gx_point_y = (GX_VALUE)(view -> gx_rectangle_top - draw_area.gx_rectangle_top);
                        draw_area.gx_rectangle_top = view -> gx_rectangle_top;
                    }

                    if (draw_area.gx_rectangle_left < view -> gx_rectangle_left)
                    {
                        map_offset.gx_point_x = (GX_VALUE)(view -> gx_rectangle_left - x_offset);
                        draw_area.gx_rectangle_left = view -> gx_rectangle_left;
                    }

                    if (draw_area.gx_rectangle_bottom > view -> gx_rectangle_bottom)
                    {
                        draw_area.gx_rectangle_bottom = view -> gx_rectangle_bottom;
                    }
                    if (draw_area.gx_rectangle_right > view -> gx_rectangle_right)
                    {
                        draw_area.gx_rectangle_right = view -> gx_rectangle_right;
                    }

                    draw_glyph(context, &draw_area, &map_offset, (const GX_GLYPH *)glyph);
                }
            }
            xstart = (GX_VALUE)(xstart + glyph -> gx_glyph_advance);
        }
    } while (string_copy.gx_string_length > 0);
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_canvas_generic_glyphs_draw                      PORTABLE C      */
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
/*    context                               Drawing context               */
/*    font                                  Font used by the string       */
/*    draw_position                         Coord of draw position        */
/*    string                                Pointer to string which need  */
/*                                            to draw                     */
/*    length                                Number of string about to draw*/
/*    view                                  Pointer to view size          */
/*    draw_glyph                            Callback pointer to display   */
/*                                            driver text draw function   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_utf8_string_character_get                               */
/*                                    Get characters of this string       */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_canvas_glyph_draw                                               */
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
static VOID _gx_canvas_generic_glyphs_draw(GX_DRAW_CONTEXT *context, GX_FONT *font, GX_POINT *draw_position, GX_CONST GX_STRING *string, GX_RECTANGLE *view,
                                           VOID (*draw_glyph)(GX_DRAW_CONTEXT *, GX_RECTANGLE *, GX_POINT *, GX_CONST GX_GLYPH *))
{
GX_CONST GX_GLYPH *glyph;
GX_CHAR_CODE       char_val;
GX_VALUE           x_offset;
GX_VALUE           y_offset;
GX_VALUE           xstart;
GX_VALUE           ystart;
GX_POINT           map_offset;
GX_RECTANGLE       draw_area;
GX_CONST GX_FONT  *font_link;
GX_STRING          string_copy;

#ifdef GX_UTF8_SUPPORT
UINT          ret;
#ifdef GX_THAI_GLYPH_SHAPING_SUPPORT
GX_CHAR_CODE *code_list = GX_NULL;
UINT          code_count;
UINT          index = 0;
    if (_gx_system_text_render_style & GX_TEXT_RENDER_THAI_GLYPH_SHAPING)
    {
        ret = _gx_utility_thai_glyph_shaping(string, &code_list, &code_count);
    }
#endif
#endif /* GX_UTF8_SUPPORT */

    /* Setup local variables.  */
    xstart = draw_position -> gx_point_x;
    ystart = draw_position -> gx_point_y;
    string_copy = *string;

    /* for each character in the string */
    do
    {
#ifdef GX_UTF8_SUPPORT
#if defined(GX_THAI_GLYPH_SHAPING_SUPPORT)
        if (code_list)
        {
            if (index < code_count)
            {
                char_val = code_list[index++];
            }
            else
            {
                char_val = 0;
            }
        }
        else
        {
#endif
            ret = _gx_utility_utf8_string_character_get(&string_copy, &char_val, GX_NULL);
#if defined(GX_THAI_GLYPH_SHAPING_SUPPORT)
        }
#endif
        if ((ret != GX_SUCCESS) || (char_val == 0))
#else
        char_val = (GX_CHAR_CODE)(*string_copy.gx_string_ptr++);
        string_copy.gx_string_length--;

        if (char_val == 0)
#endif /* GX_UTF8_SUPPORT */
        {
            break;
        }

        font_link = font;
        while (font_link)
        {
            if (char_val >= font_link -> gx_font_first_glyph &&
                char_val <= font_link -> gx_font_last_glyph)
            {
                break;
            }
            font_link = font_link -> gx_font_next_page;
        }

        if (font_link)
        {
            char_val = (GX_CHAR_CODE)(char_val - font_link -> gx_font_first_glyph);
            glyph = &font_link -> gx_font_glyphs.gx_font_normal_glyphs[char_val];

            if (glyph -> gx_glyph_map)
            {
                x_offset = (GX_VALUE)(xstart + glyph -> gx_glyph_leading);
                y_offset = (GX_VALUE)(ystart + font_link -> gx_font_baseline - glyph -> gx_glyph_ascent);

                draw_area.gx_rectangle_left = x_offset;
                draw_area.gx_rectangle_top = y_offset;
                draw_area.gx_rectangle_right = (GX_VALUE)(draw_area.gx_rectangle_left + glyph -> gx_glyph_width - 1);
                draw_area.gx_rectangle_bottom = (GX_VALUE)(draw_area.gx_rectangle_top + glyph -> gx_glyph_height - 1);

                if (draw_area.gx_rectangle_bottom >= view -> gx_rectangle_top &&
                    draw_area.gx_rectangle_top <= view -> gx_rectangle_bottom &&
                    draw_area.gx_rectangle_right >= view -> gx_rectangle_left &&
                    draw_area.gx_rectangle_left <= view -> gx_rectangle_right)
                {
                    map_offset.gx_point_x = 0;
                    map_offset.gx_point_y = 0;

                    /* Calculate the y_start value, which is the offset into the row of
                       the glyph where we start to the draw. */
                    if (draw_area.gx_rectangle_top < view -> gx_rectangle_top)
                    {
                        map_offset.gx_point_y = (GX_VALUE)(view -> gx_rectangle_top - draw_area.gx_rectangle_top);
                        draw_area.gx_rectangle_top = view -> gx_rectangle_top;
                    }

                    if (draw_area.gx_rectangle_left < view -> gx_rectangle_left)
                    {
                        map_offset.gx_point_x = (GX_VALUE)(view -> gx_rectangle_left - x_offset);
                        draw_area.gx_rectangle_left = view -> gx_rectangle_left;
                    }

                    if (draw_area.gx_rectangle_bottom > view -> gx_rectangle_bottom)
                    {
                        draw_area.gx_rectangle_bottom = view -> gx_rectangle_bottom;
                    }
                    if (draw_area.gx_rectangle_right > view -> gx_rectangle_right)
                    {
                        draw_area.gx_rectangle_right = view -> gx_rectangle_right;
                    }

                    draw_glyph(context, &draw_area, &map_offset, glyph);
                }
            }
            xstart = (GX_VALUE)(xstart + glyph -> gx_glyph_advance);
        }
    } while (string_copy.gx_string_length > 0);

#ifdef GX_UTF8_SUPPORT
#if defined(GX_THAI_GLYPH_SHAPING_SUPPORT)
    if (code_list)
    {
        _gx_system_memory_free((void *)code_list);
    }
#endif
#endif
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_canvas_glyphs_draw                              PORTABLE C      */
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
/*    context                               Drawing context               */
/*    draw_position                         Coord of draw position        */
/*    string                                Pointer to string which need  */
/*                                            to draw                     */
/*    length                                Number of string about to draw*/
/*    view                                  Pointer to view size          */
/*    draw_glyph                            Callback pointer to display   */
/*                                            driver text draw function   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_utf8_string_character_get                               */
/*                                    Get characters of this string       */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_canvas_compressed_glyphs_draw                                   */
/*    _gx_canvas_kerning_glyphs_draw                                      */
/*    _gx_canvas_generic_glyphs_draw                                      */
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
VOID _gx_canvas_glyphs_draw(GX_DRAW_CONTEXT *context, GX_POINT *draw_position, GX_CONST GX_STRING *string,
                            GX_RECTANGLE *view,
                            VOID (*draw_glyph)(GX_DRAW_CONTEXT *, GX_RECTANGLE *, GX_POINT *, GX_CONST GX_GLYPH *))
{
GX_BRUSH *brush;
GX_FONT  *font;

    /* pickup pointer to current drawing context */
    context =   _gx_system_current_draw_context;

    /* get the current brush */
    brush =     &context -> gx_draw_context_brush;

    /* get the current font and color */
    font =      brush -> gx_brush_font;

    if (!string || !font)
    {
        /* Return error.  */
        return;
    }

    if (font -> gx_font_format & GX_FONT_FORMAT_COMPRESSED)
    {
        _gx_canvas_compressed_glyphs_draw(context, font, draw_position, string, view, draw_glyph);
    }
#if defined(GX_FONT_KERNING_SUPPORT)
    else if (font -> gx_font_format & GX_FONT_FORMAT_KERNING)
    {
        _gx_canvas_kerning_glyphs_draw(context, font, draw_position, string, view, draw_glyph);
    }
#endif
    else
    {
        _gx_canvas_generic_glyphs_draw(context, font, draw_position, string, view, draw_glyph);
    }
}

