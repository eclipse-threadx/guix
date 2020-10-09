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
/**   Widget Management (Widget)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_canvas.h"
#include "gx_context.h"
#include "gx_widget.h"
#include "gx_utility.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_widget_text_blend                               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION (deprecated)                                              */
/*                                                                        */
/*    This function blends the specified text using current brush and     */
/*    text alignment.                                                     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    widget                                Widget control block          */
/*    tColor                                Text Color                    */
/*    font_id                               Font Id                       */
/*    string                                Drawing string                */
/*    x_offset                              Drawing position adjustment   */
/*    y_offset                              Drawing position adjustment   */
/*    alpha                                 Blending value 0-255          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_string_length_check       Validate string length        */
/*    _gx_widget_text_blend_ext             New version of this function  */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
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
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT  _gx_widget_text_blend(GX_WIDGET *widget,
                            UINT tColor, UINT font_id,
                            GX_CONST GX_CHAR *string, INT x_offset, INT y_offset, UCHAR alpha)
{
UINT      status;
GX_STRING new_string;

    new_string.gx_string_ptr = string;

    /* Calcualte text length. */
    status = _gx_utility_string_length_check(string, &new_string.gx_string_length, GX_MAX_STRING_LENGTH);
    if (status != GX_SUCCESS)
    {
        return status;
    }

    status = _gx_widget_text_blend_ext(widget, tColor, font_id, &new_string, x_offset, y_offset, alpha);

    return status;
}
#endif


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_widget_text_blend_ext                           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function blends the specified text using current brush and     */
/*    text alignment.                                                     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    widget                                Widget control block          */
/*    tColor                                Text Color                    */
/*    font_id                               Font Id                       */
/*    string                                Drawing string                */
/*    x_offset                              Drawing position adjustment   */
/*    y_offset                              Drawing position adjustment   */
/*    alpha                                 Blending value 0-255          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_context_line_color_set            Set the line color            */
/*    _gx_context_font_set                  Set the font in the context   */
/*    _gx_context_brush_get                 Get the context brush         */
/*    _gx_widget_height_get                 Get widget height             */
/*    _gx_widget_width_get                  Get widget width              */
/*    _gx_widget_border_width_get           Get widget border width       */
/*    _gx_system_string_width_get           Get string width              */
/*    _gx_canvas_text_draw                  Draw text on the canvas       */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
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
UINT  _gx_widget_text_blend_ext(GX_WIDGET *widget,
                                UINT tColor, UINT font_id,
                                GX_CONST GX_STRING *string, INT x_offset, INT y_offset, UCHAR alpha)
{
GX_VALUE  text_width;
GX_VALUE  text_height;
GX_VALUE  widget_width;
GX_VALUE  widget_height;
GX_VALUE  x_pos;
GX_VALUE  y_pos;
GX_VALUE  border_width;

GX_BRUSH *brush;

    /* Is there a string?  */
    if (string && string -> gx_string_ptr)
    {
        _gx_context_line_color_set(tColor);
        _gx_context_font_set(font_id);
        _gx_context_brush_get(&brush);

        if (!brush -> gx_brush_font)
        {
            return(GX_SUCCESS);
        }
        brush -> gx_brush_alpha = alpha;

        text_height = brush -> gx_brush_font -> gx_font_line_height;
        _gx_widget_height_get(widget, &widget_height);

        _gx_widget_border_width_get(widget, &border_width);

        x_pos = widget -> gx_widget_size.gx_rectangle_left;
        y_pos = widget -> gx_widget_size.gx_rectangle_top;
        y_pos = (GX_VALUE)(y_pos + (widget_height - text_height) / 2);

        switch (widget -> gx_widget_style & GX_STYLE_TEXT_ALIGNMENT_MASK)
        {
        case GX_STYLE_TEXT_RIGHT:
            _gx_system_string_width_get_ext(brush -> gx_brush_font, string, &text_width);
            _gx_widget_width_get(widget, &widget_width);
            x_pos = (GX_VALUE)(x_pos + widget_width - 1);
            x_pos = (GX_VALUE)(x_pos - text_width - border_width);
            break;

        case GX_STYLE_TEXT_LEFT:
            x_pos = (GX_VALUE)(x_pos + border_width);
            break;

        case GX_STYLE_TEXT_CENTER:
        default:
            _gx_system_string_width_get_ext(brush -> gx_brush_font, string, &text_width);
            _gx_widget_width_get(widget, &widget_width);
            x_pos = (GX_VALUE)(x_pos + ((widget_width - text_width) / 2));
            break;
        }

        /* Draw the text.  */
        _gx_canvas_text_draw_ext((GX_VALUE)(x_pos + x_offset), (GX_VALUE)(y_pos + y_offset), string);
    }

    return(GX_SUCCESS);
}

