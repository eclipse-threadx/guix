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
/**   Button Management (Button)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_canvas.h"
#include "gx_context.h"
#include "gx_widget.h"
#include "gx_button.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_checkbox_draw                                   PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function draws the specified button, which is a special type   */
/*    of widget.                                                          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    button                                Checkbox control block        */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_context_pixelmap_get              Retrieve pixelmap image       */
/*    _gx_widget_height_get                 Gets the height of the widget */
/*    _gx_canvas_pixelmap_draw              Draw the pixelmap image       */
/*    _gx_widget_text_id_draw               Draw the text based on ID     */
/*    _gx_widget_text_draw                  Draw the text string          */
/*    _gx_widget_children_draw              Draw children widgets         */
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
/*                                            improved logic,             */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
VOID  _gx_checkbox_draw(GX_CHECKBOX *checkbox)
{
GX_WIDGET     *widget = (GX_WIDGET *)checkbox;
GX_VALUE       xoffset = 0;
GX_VALUE       yoffset = 0;
GX_RESOURCE_ID fill_color;
GX_RESOURCE_ID text_color;
GX_RESOURCE_ID pixelmap_id;
GX_PIXELMAP   *pixelmap = GX_NULL;
GX_STRING      string;

    if (widget -> gx_widget_style & GX_STYLE_ENABLED)
    {
        if (widget -> gx_widget_style & GX_STYLE_BUTTON_PUSHED)
        {
            fill_color = widget -> gx_widget_selected_fill_color;
            text_color = checkbox -> gx_text_button_selected_text_color;
            pixelmap_id = checkbox -> gx_checkbox_checked_pixelmap_id;
        }
        else
        {
            fill_color = widget -> gx_widget_normal_fill_color;
            text_color = checkbox -> gx_text_button_normal_text_color;
            pixelmap_id = checkbox -> gx_checkbox_unchecked_pixelmap_id;
        }
    }
    else
    {
        fill_color = checkbox -> gx_widget_disabled_fill_color;
        text_color = checkbox -> gx_text_button_disabled_text_color;
        if (widget -> gx_widget_style & GX_STYLE_BUTTON_PUSHED)
        {
            pixelmap_id = checkbox -> gx_checkbox_checked_disabled_pixelmap_id;
        }
        else
        {
            pixelmap_id = checkbox -> gx_checkbox_unchecked_disabled_pixelmap_id;
        }
    }

    /* If I am not transparent, draw my border and background */

    if (!(widget -> gx_widget_status & GX_STATUS_TRANSPARENT))
    {
        _gx_widget_border_draw(widget, GX_COLOR_ID_DEFAULT_BORDER, fill_color, fill_color, GX_TRUE);
    }

    /* draw the pixelmap */

    if (pixelmap_id)
    {
        _gx_context_pixelmap_get(pixelmap_id, &pixelmap);
    }

    if (pixelmap)
    {
        _gx_widget_height_get((GX_WIDGET *)checkbox, &yoffset);
        yoffset = (GX_VALUE)((yoffset - pixelmap -> gx_pixelmap_height) / 2);

        switch (checkbox -> gx_widget_style & GX_STYLE_TEXT_ALIGNMENT_MASK)
        {
        case GX_STYLE_TEXT_RIGHT:
            _gx_widget_width_get((GX_WIDGET *)checkbox, &xoffset);
            xoffset = (GX_VALUE)(xoffset - pixelmap -> gx_pixelmap_width + 1);
            /* draw the pixelmap on the right */
            _gx_canvas_pixelmap_draw((GX_VALUE)(checkbox -> gx_widget_size.gx_rectangle_right -
                                                pixelmap -> gx_pixelmap_width + 1),
                                     (GX_VALUE)(checkbox -> gx_widget_size.gx_rectangle_top + yoffset), pixelmap);

            /* draw the text to the left of the bitmap */
            xoffset = (GX_VALUE)(-(pixelmap -> gx_pixelmap_width * 3 / 2));
            break;

        case GX_STYLE_TEXT_CENTER:
            /* draw the pixelmap and text centered         */
            _gx_widget_width_get((GX_WIDGET *)checkbox, &xoffset);
            xoffset = (GX_VALUE)((xoffset - pixelmap -> gx_pixelmap_width) / 2);

            /* draw the pixelmap centered */
            _gx_canvas_pixelmap_draw((GX_VALUE)(checkbox -> gx_widget_size.gx_rectangle_left + xoffset),
                                     (GX_VALUE)(checkbox -> gx_widget_size.gx_rectangle_top + yoffset), pixelmap);
            /* draw the text centered: */
            xoffset = 0;
            break;

        case GX_STYLE_TEXT_LEFT:
            /* draw the pixelmap on the left */
            _gx_canvas_pixelmap_draw(checkbox -> gx_widget_size.gx_rectangle_left,
                                     (GX_VALUE)(checkbox -> gx_widget_size.gx_rectangle_top + yoffset), pixelmap);

            /* draw the text to the right of the pixelmap */
            xoffset = (GX_VALUE)(pixelmap -> gx_pixelmap_width * 3 / 2);
            break;
        }
    }

    _gx_text_button_text_get_ext((GX_TEXT_BUTTON *)checkbox, &string);

    if (string.gx_string_ptr)
    {
        _gx_widget_text_draw_ext(widget, text_color,
                                 checkbox -> gx_text_button_font_id,
                                 &string, xoffset, 0);
    }

    /* Draw checkbox's children.  */
    _gx_widget_children_draw(widget);
}

