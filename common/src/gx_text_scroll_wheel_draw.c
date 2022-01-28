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
/**   String Scroll Wheel Management (Scroll Wheel)                       */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_window.h"
#include "gx_widget.h"
#include "gx_scroll_wheel.h"
#include "gx_context.h"
#include "gx_canvas.h"
#include "gx_utility.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_text_scroll_wheel_round_text_draw               PORTABLE C      */
/*                                                           6.1.10       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to draw text to specified area in rounded  */
/*    style.                                                              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    wheel                                 Scroll wheel control block    */
/*    tColor                                Text color id                 */
/*    font_id                               Font id                       */
/*    string                                Text to be draw               */
/*    x_pos                                 Draw start xpos               */
/*    y_pos                                 Draw start ypos               */
/*    width                                 Draw width                    */
/*    height                                Draw height                   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_context_line_color_set            Set line color                */
/*    _gx_context_fill_color_set            Set fill color                */
/*    _gx_context_font_set                  Set font                      */
/*    _gx_context_brush_get                 Get brush                     */
/*    _gx_system_string_width_get           Get string width              */
/*    _gx_utility_string_to_alphamap        Convert string to alphamap    */
/*    _gx_utility_pixelmap_resize           Resize a pixelmap             */
/*    _gx_canvas_pixelmap_draw              Draw pixelmap to canvas       */
/*    _gx_system_memory_free                Application defined memory    */
/*                                            free function               */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_text_scroll_wheel_row_draw                                      */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  06-02-2021     Kenneth Maxwell          Modified comment(s),          */
/*                                            rename RENESAS_DAVE2D       */
/*                                            support conditional,        */
/*                                            resulting in version 6.1.7  */
/*  01-31-2022     Ting Zhu                 Modified comment(s),          */
/*                                            removed alpha set,          */
/*                                            resulting in version 6.1.10 */
/*                                                                        */
/**************************************************************************/
static UINT  _gx_text_scroll_wheel_round_text_draw(GX_TEXT_SCROLL_WHEEL *wheel, GX_RESOURCE_ID tColor, GX_RESOURCE_ID font_id,
                                                   GX_CONST GX_STRING *string, GX_VALUE x_pos, GX_VALUE y_pos, GX_VALUE width, GX_VALUE height)
{
UINT        status = GX_SUCCESS;
GX_VALUE    text_width = 0;
GX_VALUE    text_height;
GX_BRUSH   *brush;
GX_PIXELMAP textmap;
GX_PIXELMAP resized_map;
GX_COLOR    old_fill_color;

    _gx_context_line_color_set(tColor);
    _gx_context_font_set(font_id);
    _gx_context_brush_get(&brush);

    if (!brush -> gx_brush_font)
    {
        return(GX_SUCCESS);
    }

    text_height = brush -> gx_brush_font -> gx_font_line_height;

    if (wheel -> gx_scroll_wheel_row_height <= 0)
    {
        return GX_FAILURE;
    }

    text_height = (GX_VALUE)(text_height * height / wheel -> gx_scroll_wheel_row_height);

    _gx_system_string_width_get_ext(brush -> gx_brush_font, string, &text_width);

    if (!text_height || !text_width)
    {
        return(GX_SUCCESS);
    }

    y_pos = (GX_VALUE)(y_pos + (height - text_height) / 2);

    switch (wheel -> gx_widget_style & GX_STYLE_TEXT_ALIGNMENT_MASK)
    {
    case GX_STYLE_TEXT_RIGHT:
        x_pos = (GX_VALUE)(x_pos + width - 1);
        x_pos = (GX_VALUE)(x_pos - text_width);
        break;

    case GX_STYLE_TEXT_LEFT:
        break;

    case GX_STYLE_TEXT_CENTER:
    default:
        x_pos = (GX_VALUE)(x_pos + ((width - text_width) / 2));
        break;
    }

    status = _gx_utility_string_to_alphamap_ext(string, brush -> gx_brush_font, &textmap);

    if (status == GX_SUCCESS)
    {
        status = _gx_utility_pixelmap_resize(&textmap, &resized_map, text_width, text_height);

        if (status == GX_SUCCESS)
        {
            old_fill_color = brush -> gx_brush_fill_color;
            _gx_context_fill_color_set(tColor);
#if defined(GX_RENESAS_DAVE2D_DRAW)
            resized_map.gx_pixelmap_flags |= GX_PIXELMAP_DYNAMICALLY_ALLOCATED;
#endif
            _gx_canvas_pixelmap_draw(x_pos, y_pos, &resized_map);
            brush -> gx_brush_fill_color = old_fill_color;

            /* free the resized alphamap memory */
            _gx_system_memory_free((void *)resized_map.gx_pixelmap_data);
        }

        /* free the temporary canvas memory */
        _gx_system_memory_free((void *)(textmap.gx_pixelmap_data));
    }

    return status;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_text_scroll_wheel_flat_text_draw                PORTABLE C      */
/*                                                           6.1.10       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to draw text to specified area in normal   */
/*    style.                                                              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    wheel                                 Scroll wheel control block    */
/*    tColor                                Text color id                 */
/*    font_id                               Font id                       */
/*    string                                Text to be draw               */
/*    x_pos                                 Draw start xpos               */
/*    y_pos                                 Draw start ypos               */
/*    width                                 Draw width                    */
/*    height                                Draw height                   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_context_line_color_set            Set line color                */
/*    _gx_context_font_set                  Set font                      */
/*    _gx_context_brush_get                 Get brush                     */
/*    _gx_system_string_width_get           Get string width              */
/*    _gx_canvas_text_draw                  Draw text to canvas           */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_text_scroll_wheel_row_draw                                      */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  01-31-2022     Ting Zhu                 Modified comment(s),          */
/*                                            removed alpha set,          */
/*                                            resulting in version 6.1.10 */
/*                                                                        */
/**************************************************************************/
static UINT  _gx_text_scroll_wheel_flat_text_draw(GX_TEXT_SCROLL_WHEEL *wheel, GX_RESOURCE_ID tColor, GX_RESOURCE_ID font_id,
                                                  GX_CONST GX_STRING *string, GX_VALUE x_pos, GX_VALUE y_pos, GX_VALUE width, GX_VALUE height)
{
GX_VALUE  text_width = 0;
GX_VALUE  text_height;
GX_BRUSH *brush;

    _gx_context_line_color_set(tColor);
    _gx_context_font_set(font_id);
    _gx_context_brush_get(&brush);

    if (!brush -> gx_brush_font)
    {
        return(GX_SUCCESS);
    }

    text_height = brush -> gx_brush_font -> gx_font_line_height;

    y_pos = (GX_VALUE)(y_pos + (height - text_height) / 2);

    switch (wheel -> gx_widget_style & GX_STYLE_TEXT_ALIGNMENT_MASK)
    {
    case GX_STYLE_TEXT_RIGHT:
        _gx_system_string_width_get_ext(brush -> gx_brush_font, string, &text_width);
        x_pos = (GX_VALUE)(x_pos + width - 1);
        x_pos = (GX_VALUE)(x_pos - text_width);
        break;

    case GX_STYLE_TEXT_LEFT:
        break;
    case GX_STYLE_TEXT_CENTER:
    default:
        _gx_system_string_width_get_ext(brush -> gx_brush_font, string, &text_width);
        x_pos = (GX_VALUE)(x_pos + ((width - text_width) / 2));
        break;
    }

    /* Draw the text.  */
    _gx_canvas_text_draw_ext(x_pos, y_pos, string);

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_text_scroll_wheel_row_draw                      PORTABLE C      */
/*                                                           6.1.4        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to draw text to specified area with        */
/*    relevant fonts.                                                     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    wheel                                 Scroll wheel control block    */
/*    selected_area                         The area of selected item     */
/*    draw_draw                             The area for drawing          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_rectangle_compare         Test if rectangles are equal  */
/*    _gx_utility_string_length_check       Test string length            */
/*    _gx_text_scroll_wheel_round_text_draw Draw text in rounded style    */
/*    _gx_text_scroll_wheel_flat_text_draw  Draw text in normal style     */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_text_scroll_wheel_round_draw                                    */
/*    _gx_text_scroll_wheel_flat_draw                                     */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  02-02-2021     Kenneth Maxwell          Modified comment(s),          */
/*                                            renamed                     */
/*                                            GX_STYLE_SCROLL_WHEEL_DRAG  */
/*                                            to GX_STATUS_TRACKING_PEN,  */
/*                                            resulting in version 6.1.4  */
/*                                                                        */
/**************************************************************************/
static UINT _gx_text_scroll_wheel_row_draw(GX_TEXT_SCROLL_WHEEL *wheel, GX_RECTANGLE *selected_area, GX_RECTANGLE *draw_area, GX_CONST GX_STRING *string)
{
UINT           status = GX_SUCCESS;
GX_RESOURCE_ID text_color;
GX_RESOURCE_ID font;
INT            width;
INT            height;
UINT           (*text_draw)(GX_TEXT_SCROLL_WHEEL *wheel, GX_RESOURCE_ID tColor, GX_RESOURCE_ID font_id,
                            GX_CONST GX_STRING *string, GX_VALUE x_pos, GX_VALUE y_pos, GX_VALUE width, GX_VALUE height);


    if (string -> gx_string_length == 0)
    {
        return status;
    }

    width = draw_area -> gx_rectangle_right - draw_area -> gx_rectangle_left + 1;
    height = draw_area -> gx_rectangle_bottom - draw_area -> gx_rectangle_top + 1;

    if (wheel -> gx_widget_style & GX_STYLE_TEXT_SCROLL_WHEEL_ROUND)
    {
        text_draw = _gx_text_scroll_wheel_round_text_draw;
    }
    else
    {
        text_draw = _gx_text_scroll_wheel_flat_text_draw;
    }

    if ((wheel -> gx_widget_status & GX_STATUS_TRACKING_PEN) ||
        (wheel -> gx_scroll_wheel_animation_steps != 0) ||
        (!_gx_utility_rectangle_compare(selected_area, draw_area)))
    {
        if (wheel -> gx_widget_style & GX_STYLE_ENABLED)
        {
            text_color = wheel -> gx_text_scroll_wheel_normal_text_color;
        }
        else
        {
            text_color = wheel -> gx_text_scroll_wheel_disabled_text_color;
        }

        font = wheel -> gx_text_scroll_wheel_normal_font;
    }
    else
    {
        if (wheel -> gx_widget_style & GX_STYLE_ENABLED)
        {
            text_color = wheel -> gx_text_scroll_wheel_selected_text_color;
        }
        else
        {
            text_color = wheel -> gx_text_scroll_wheel_disabled_text_color;
        }
        font = wheel -> gx_text_scroll_wheel_selected_font;
    }

    /* Draw text. */
    status = text_draw(wheel, text_color, font, string, draw_area -> gx_rectangle_left,
                       draw_area -> gx_rectangle_top, (GX_VALUE)width, (GX_VALUE)height);

    return status;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_text_scroll_wheel_text_get                      PORTABLE C      */
/*                                                           6.1.10       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to retrieve text of sepecified row.        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    wheel                                 Scroll wheel control block    */
/*    row                                   Row index                     */
/*    string                                String pointer to be return   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    [gx_text_scroll_wheel_text_get]      Retrieve row text              */
/*    [_gx_system_memory_allocator]        Memory allocator               */
/*    _gx_utility_bidi_paragraph_reorder   Reorder bidi text              */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_text_scroll_wheel_flat_draw                                     */
/*    _gx_text_scroll_wheel_round_draw                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  09-30-2020     Kenneth Maxwell          Initial Version 6.1           */
/*  01-31-2022     Ting Zhu                 Modified comment(s),          */
/*                                            updated with new bidi text  */
/*                                            reorder function call,      */
/*                                            resulting in version 6.1.10 */
/*                                                                        */
/**************************************************************************/
static UINT _gx_text_scroll_wheel_text_get(GX_TEXT_SCROLL_WHEEL *wheel, INT row, GX_STRING *string)
{
#ifdef GX_DYNAMIC_BIDI_TEXT_SUPPORT
GX_BIDI_TEXT_INFO           text_info;
GX_BIDI_RESOLVED_TEXT_INFO *resolved_info;
GX_CANVAS                  *canvas;
GX_DISPLAY                 *display;
#endif

#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT status;

    if (wheel -> gx_text_scroll_wheel_text_get_deprecated)
    {
        string -> gx_string_ptr = wheel -> gx_text_scroll_wheel_text_get_deprecated(wheel, row);

        if (string -> gx_string_ptr)
        {
            status = _gx_utility_string_length_check(string -> gx_string_ptr, &string -> gx_string_length, GX_MAX_STRING_LENGTH);

            if (status != GX_SUCCESS)
            {
                return status;
            }
        }
    }
    else
    {
#endif
        wheel -> gx_text_scroll_wheel_text_get(wheel, row, string);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
    }
#endif

#ifdef GX_DYNAMIC_BIDI_TEXT_SUPPORT
    if (_gx_system_bidi_text_enabled)
    {
        if (!_gx_system_memory_allocator)
        {
            return GX_SYSTEM_MEMORY_ERROR;
        }

        if (!wheel -> gx_text_scroll_wheel_bidi_resolved_text_info)
        {
            wheel -> gx_text_scroll_wheel_bidi_resolved_text_info = (GX_BIDI_RESOLVED_TEXT_INFO **)_gx_system_memory_allocator(sizeof(GX_BIDI_RESOLVED_TEXT_INFO *) * (UINT)wheel -> gx_scroll_wheel_total_rows);

            if (!wheel -> gx_text_scroll_wheel_bidi_resolved_text_info)
            {
                return GX_SYSTEM_MEMORY_ERROR;
            }

            memset(wheel -> gx_text_scroll_wheel_bidi_resolved_text_info, 0, sizeof(GX_BIDI_RESOLVED_TEXT_INFO *) * (UINT)wheel -> gx_scroll_wheel_total_rows);
        }

        if (!wheel -> gx_text_scroll_wheel_bidi_resolved_text_info[row])
        {
            text_info.gx_bidi_text_info_text = *string;
            text_info.gx_bidi_text_info_font = GX_NULL;
            text_info.gx_bidi_text_info_display_width = 0;
            GX_UTILITY_TEXT_DIRECTION_GET(text_info.gx_bidi_text_info_direction, wheel, canvas, display);

            if (_gx_utility_bidi_paragraph_reorder_ext(&text_info, &resolved_info) == GX_SUCCESS)
            {
                wheel -> gx_text_scroll_wheel_bidi_resolved_text_info[row] = resolved_info;
            }
        }

        if (wheel -> gx_text_scroll_wheel_bidi_resolved_text_info[row])
        {
            *string = *wheel -> gx_text_scroll_wheel_bidi_resolved_text_info[row] -> gx_bidi_resolved_text_info_text;
        }
    }
#endif

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_text_scroll_wheel_round_draw                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to draw a scroll wheel, which contain some */
/*    text transformating process.                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    wheel                                 Scroll wheel control block    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_context_pixelmap_get             Get pixelmap by resource ID    */
/*    _gx_window_client_height_get         Get window client height       */
/*    _gx_window_client_width_get          Get window clietn width        */
/*    _gx_widget_border_width_get          Get widget border width        */
/*    _gx_utility_rectangle_define         Define a rectangle             */
/*    _gx_canvas_pixelmap_tile             Title a pixelmap               */
/*    _gx_text_scroll_wheel_row_draw       Draw text to specified area    */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_text_scroll_wheel_draw                                          */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            improved logic,             */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
static UINT _gx_text_scroll_wheel_round_draw(GX_TEXT_SCROLL_WHEEL *wheel)
{
UINT         status = GX_SUCCESS;
GX_PIXELMAP *map;
GX_VALUE     width;
GX_VALUE     height;
GX_VALUE     row_height;
INT          trans_height;
GX_VALUE     xpos;
GX_VALUE     ypos;
GX_VALUE     ycenter;
GX_VALUE     border_width;
GX_RECTANGLE selected_area;
GX_RECTANGLE draw_area;
GX_STRING    string;
INT          row;

    /* Pickup selected background. */
    _gx_context_pixelmap_get(wheel -> gx_scroll_wheel_selected_background, &map);

    /* Pickup client height. */
    _gx_window_client_height_get((GX_WINDOW *)wheel, &height);
    _gx_window_client_width_get((GX_WINDOW *)wheel, &width);
    _gx_widget_border_width_get((GX_WIDGET *)wheel, &border_width);

    if (width <= 0 || height <= 0)
    {
        return GX_FALSE;
    }

    row_height = wheel -> gx_scroll_wheel_row_height;
    xpos = (GX_VALUE)(wheel -> gx_widget_size.gx_rectangle_left + border_width);

    ypos = (GX_VALUE)(wheel -> gx_widget_size.gx_rectangle_top + border_width);
    ypos = (GX_VALUE)(ypos + (height >> 1));
    ypos = (GX_VALUE)(ypos - (row_height >> 1));

    /* Draw selected background. */
    _gx_utility_rectangle_define(&selected_area, xpos, ypos, (GX_VALUE)(xpos + width - 1), (GX_VALUE)(ypos + row_height - 1));

    if (map)
    {
        _gx_canvas_pixelmap_tile(&selected_area, map);
    }

    /* Draw scroll wheel rows. */

    ycenter = (GX_VALUE)(ypos + row_height / 2);
    ypos = (GX_VALUE)(ypos + wheel -> gx_scroll_wheel_selected_yshift);
    row = wheel -> gx_scroll_wheel_selected_row;

    while (status == GX_SUCCESS &&
           ypos < wheel -> gx_widget_size.gx_rectangle_bottom - border_width)
    {
        if (row > (INT)(wheel -> gx_scroll_wheel_total_rows - 1))
        {
            if (wheel -> gx_widget_style & GX_STYLE_WRAP)
            {
                row -= wheel -> gx_scroll_wheel_total_rows;
            }
            else
            {
                break;
            }
        }

        status = _gx_text_scroll_wheel_text_get(wheel, row, &string);

        if (status != GX_SUCCESS)
        {
            return status;
        }

        trans_height = GX_ABS(ypos + row_height / 2 - ycenter);

        if (trans_height == 0)
        {
            trans_height = row_height;
        }
        else
        {
            trans_height = GX_FIXED_VAL_MAKE(GX_ABS(height * 3 / 4 - trans_height));
            trans_height = 120 * trans_height / height;

            trans_height = GX_FIXED_VAL_TO_INT(_gx_utility_math_sin(trans_height) * row_height);

            if (trans_height < row_height / 5)
            {
                break;
            }
        }

        _gx_utility_rectangle_define(&draw_area, xpos, ypos, (GX_VALUE)(xpos + width - 1), (GX_VALUE)(ypos + trans_height - 1));
        status = _gx_text_scroll_wheel_row_draw((GX_TEXT_SCROLL_WHEEL *)wheel, &selected_area, &draw_area, &string);
        ypos = (GX_VALUE)(ypos + trans_height);
        row++;
    }

    ypos = (GX_VALUE)(ycenter - row_height / 2 + wheel -> gx_scroll_wheel_selected_yshift);
    row = wheel -> gx_scroll_wheel_selected_row - 1;

    while (status == GX_SUCCESS &&
           ypos > wheel -> gx_widget_size.gx_rectangle_top + border_width)
    {
        if (row < 0)
        {
            if (wheel -> gx_widget_style & GX_STYLE_WRAP)
            {
                row += wheel -> gx_scroll_wheel_total_rows;
            }
            else
            {
                break;
            }
        }

        status = _gx_text_scroll_wheel_text_get(wheel, row, &string);

        if (status != GX_SUCCESS)
        {
            return status;
        }

        trans_height = ycenter + (row_height / 2) - ypos;

        trans_height = GX_FIXED_VAL_MAKE(GX_ABS(height * 3 / 4 - trans_height));
        trans_height = 120 * trans_height / height;

        trans_height = GX_FIXED_VAL_TO_INT(_gx_utility_math_sin(trans_height) * row_height);

        if (trans_height < row_height / 5)
        {
            break;
        }

        _gx_utility_rectangle_define(&draw_area, xpos, (GX_VALUE)(ypos - trans_height), (GX_VALUE)(xpos + width - 1), (GX_VALUE)(ypos - 1));
        status = _gx_text_scroll_wheel_row_draw((GX_TEXT_SCROLL_WHEEL *)wheel, &selected_area, &draw_area, &string);
        ypos = (GX_VALUE)(ypos - trans_height);
        row--;
    }

    /* Draw the overlay pixelmap, if there is one: */

    if (wheel -> gx_scroll_wheel_gradient.gx_gradient_pixelmap.gx_pixelmap_data)
    {
        _gx_canvas_pixelmap_tile(&wheel -> gx_widget_size, &wheel -> gx_scroll_wheel_gradient.gx_gradient_pixelmap);
    }

    return status;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_text_scroll_wheel_flat_draw                     PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to draw a scroll wheel in normal case.     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    wheel                                 Scroll wheel control block    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_context_pixelmap_get              Get pixelmap by resource ID   */
/*    _gx_window_client_height_get          Get window client height      */
/*    _gx_window_client_width_get           Get window client width       */
/*    _gx_widget_border_width_get           Get widget border width       */
/*    _gx_utility_rectangle_define          Define a rectangle            */
/*    _gx_canvas_pixelmap_tile              Tile a pixelmap               */
/*    _gx_text_scroll_wheel_row_draw        Draw text to specified area   */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_text_scroll_wheel_draw                                          */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            improved logic,             */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
static UINT _gx_text_scroll_wheel_flat_draw(GX_TEXT_SCROLL_WHEEL *wheel)
{
GX_PIXELMAP *map;
GX_VALUE     width;
GX_VALUE     height;
GX_VALUE     row_height;
GX_VALUE     xpos;
GX_VALUE     ypos;
GX_VALUE     border_width;
GX_RECTANGLE selected_area;
GX_RECTANGLE draw_area;
GX_STRING    string;
INT          row;
INT          top_rows;
UINT         status;

    /* Pickup selected background. */
    _gx_context_pixelmap_get(wheel -> gx_scroll_wheel_selected_background, &map);

    /* Pickup client height. */
    _gx_window_client_height_get((GX_WINDOW *)wheel, &height);
    _gx_window_client_width_get((GX_WINDOW *)wheel, &width);
    _gx_widget_border_width_get((GX_WIDGET *)wheel, &border_width);

    row_height = wheel -> gx_scroll_wheel_row_height;

    if (row_height <= 0)
    {
        return GX_FALSE;
    }
    xpos = (GX_VALUE)(wheel -> gx_widget_size.gx_rectangle_left + border_width);

    ypos = (GX_VALUE)(wheel -> gx_widget_size.gx_rectangle_top + border_width);
    ypos = (GX_VALUE)(ypos + (height >> 1));
    ypos = (GX_VALUE)(ypos - (row_height >> 1));

    /* Draw selected background. */
    _gx_utility_rectangle_define(&selected_area, xpos, ypos, (GX_VALUE)(xpos + width - 1), (GX_VALUE)(ypos + row_height - 1));

    if (map)
    {
        _gx_canvas_pixelmap_tile(&selected_area, map);
    }

    ypos = (GX_VALUE)(ypos + wheel -> gx_scroll_wheel_selected_yshift);

    top_rows = (ypos - wheel -> gx_widget_size.gx_rectangle_top + row_height) / row_height;
    ypos = (GX_VALUE)(ypos - (top_rows * row_height));

    row = wheel -> gx_scroll_wheel_selected_row - top_rows;

    while (row < 0)
    {
        if (wheel -> gx_widget_style & GX_STYLE_WRAP)
        {
            row += wheel -> gx_scroll_wheel_total_rows;
        }
        else
        {
            ypos = (GX_VALUE)(ypos - row * row_height);
            row = 0;
        }
    }

    while (ypos < wheel -> gx_widget_size.gx_rectangle_bottom - border_width)
    {
        if (row > (INT)(wheel -> gx_scroll_wheel_total_rows - 1))
        {
            if (wheel -> gx_widget_style & GX_STYLE_WRAP)
            {
                row -= wheel -> gx_scroll_wheel_total_rows;
            }
            else
            {
                break;
            }
        }

        status = _gx_text_scroll_wheel_text_get(wheel, row, &string);
        if (status != GX_SUCCESS)
        {
            return status;
        }

        _gx_utility_rectangle_define(&draw_area, xpos, ypos, (GX_VALUE)(xpos + width - 1), (GX_VALUE)(ypos + row_height - 1));
        _gx_text_scroll_wheel_row_draw((GX_TEXT_SCROLL_WHEEL *)wheel, &selected_area, &draw_area, &string);
        ypos = (GX_VALUE)(ypos + row_height);
        row++;
    }

    /* Draw the overlay pixelmap, if there is one: */

    if (wheel -> gx_scroll_wheel_gradient.gx_gradient_pixelmap.gx_pixelmap_data)
    {
        _gx_canvas_pixelmap_tile(&wheel -> gx_widget_size, &wheel -> gx_scroll_wheel_gradient.gx_gradient_pixelmap);
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_text_scroll_wheel_draw                          PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function draws a text scroll wheel widget.                     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    wheel                                 Text scroll wheel control     */
/*                                            block                       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_window_draw                       Draw a window                 */
/*    _gx_text_scroll_wheel_round_draw      Draw round style scroll wheel */
/*    _gx_text_scroll_wheel_flat_draw       Draw flat style scroll wheel  */
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
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
VOID _gx_text_scroll_wheel_draw(GX_TEXT_SCROLL_WHEEL *wheel)
{
    _gx_window_draw((GX_WINDOW *)wheel);

    if (!wheel -> gx_scroll_wheel_total_rows)
    {
        return;
    }

#if defined(GX_ENABLE_DEPRECATED_STRING_API)
    if ((wheel -> gx_text_scroll_wheel_text_get_deprecated == GX_NULL) &&
        (wheel -> gx_text_scroll_wheel_text_get == GX_NULL))
    {
        return;
    }
#else
    if (wheel -> gx_text_scroll_wheel_text_get == GX_NULL)
    {
        return;
    }
#endif

    if (wheel -> gx_widget_style & GX_STYLE_TEXT_SCROLL_WHEEL_ROUND)
    {
        _gx_text_scroll_wheel_round_draw((GX_TEXT_SCROLL_WHEEL *)wheel);
    }
    else
    {
        _gx_text_scroll_wheel_flat_draw((GX_TEXT_SCROLL_WHEEL *)wheel);
    }
}

