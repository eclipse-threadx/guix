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
/**   Text Button Management (Button)                                     */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE

/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_button.h"
#include "gx_canvas.h"
#include "gx_system.h"
#include "gx_context.h"
#include "gx_utility.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_button_text_draw                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function draws the text of a multi-line text button.           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    button                                Button control block          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*   _gx_widget_font_get                    Get the font                  */
/*   _gx_widget_border_width_get            Get border width              */
/*   _gx_context_font_set                   Set font                      */
/*   _gx_context_line_color_set             Set line color                */
/*   _gx_system_string_width_get            Get the width of the string   */
/*   _gx_canvas_text_draw                   Draw text string              */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    System and Application Code                                         */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            removed dynamic bidi text   */
/*                                            processing logic,           */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
VOID  _gx_multi_line_text_button_text_draw(GX_MULTI_LINE_TEXT_BUTTON *button)
{
GX_WIDGET     *widget;
INT            xtextoffset = 0;
INT            ytextoffset = 0;
INT            xtextpos;
INT            ytextpos;
INT            text_height;
GX_VALUE       text_width;
INT            widget_height;
GX_VALUE       widget_width;
INT            line_index;
GX_FONT       *font;
GX_RESOURCE_ID color;
GX_VALUE       border_width;
GX_STRING     *line;

    /* Setup the button.  */
    widget = (GX_WIDGET *)button;

    /* draw the text lines */

    if (widget -> gx_widget_style & GX_STYLE_ENABLED)
    {
        if (widget -> gx_widget_style & GX_STYLE_BUTTON_PUSHED)
        {
            xtextoffset = ytextoffset = 1;
            color = button -> gx_text_button_selected_text_color;
        }
        else
        {
            color = button -> gx_text_button_normal_text_color;
        }
    }
    else
    {
        color = button -> gx_text_button_disabled_text_color;
    }

    _gx_context_font_get(button -> gx_text_button_font_id, &font);

    if (font)
    {
        text_height = button -> gx_multi_line_text_button_line_count * font -> gx_font_line_height;
        widget_height = button -> gx_widget_size.gx_rectangle_bottom - button -> gx_widget_size.gx_rectangle_top + 1;
        ytextpos = (widget_height - text_height) / 2;
        ytextpos += button -> gx_widget_size.gx_rectangle_top;

        _gx_widget_border_width_get(widget, &border_width);
        _gx_context_font_set(button -> gx_text_button_font_id);

        _gx_context_line_color_set(color);

        for (line_index = 0; line_index < button -> gx_multi_line_text_button_line_count; line_index++)
        {
            xtextpos = button -> gx_widget_size.gx_rectangle_left;
            line = &button -> gx_multi_line_text_button_lines[line_index];

            if (line -> gx_string_ptr != GX_NULL)
            {
                switch (widget -> gx_widget_style & GX_STYLE_TEXT_ALIGNMENT_MASK)
                {
                case GX_STYLE_TEXT_RIGHT:
                    _gx_system_string_width_get_ext(font, line, &text_width);
                    _gx_widget_width_get(widget, &widget_width);
                    xtextpos += widget_width - 1;
                    xtextpos -= text_width + border_width;
                    break;

                case GX_STYLE_TEXT_LEFT:
                    xtextpos += border_width;
                    break;

                case GX_STYLE_TEXT_CENTER:
                default:
                    _gx_system_string_width_get_ext(font, line, &text_width);
                    _gx_widget_width_get(widget, &widget_width);
                    xtextpos += ((widget_width - text_width) / 2);
                    break;
                }

                /* Draw the text.  */
                _gx_canvas_text_draw_ext((GX_VALUE)(xtextpos + xtextoffset), (GX_VALUE)(ytextpos + ytextoffset), line);
            }
            ytextpos += font -> gx_font_line_height;
        }
    }
}

