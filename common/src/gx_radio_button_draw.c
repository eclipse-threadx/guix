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
/*    _gx_radio_button_draw                               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service draws a radio button widget.                           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    button                                Pointer to radio button       */
/*                                            widget control block        */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_context_pixelmap_get              Retrieve pixelmap image       */
/*    _gx_canvas_pixelmap_draw              Draw the pixelmap             */
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
VOID  _gx_radio_button_draw(GX_RADIO_BUTTON *button)
{

GX_VALUE       xoffset = 0;
GX_VALUE       yoffset = 0;
GX_RESOURCE_ID text_color;
GX_RESOURCE_ID fill_color;
GX_RESOURCE_ID pixelmap_id;
GX_PIXELMAP   *pixelmap = GX_NULL;
GX_STRING      string;

    if (button -> gx_widget_style & GX_STYLE_ENABLED)
    {

        if (button -> gx_widget_style & GX_STYLE_BUTTON_PUSHED)
        {
            fill_color = button -> gx_widget_selected_fill_color;
            text_color = button -> gx_text_button_selected_text_color;
            pixelmap_id = button -> gx_radio_button_on_pixelmap_id;
        }
        else
        {
            fill_color = button -> gx_widget_normal_fill_color;
            text_color = button -> gx_text_button_normal_text_color;
            pixelmap_id = button -> gx_radio_button_off_pixelmap_id;
        }
    }
    else
    {
        fill_color = button -> gx_widget_disabled_fill_color;
        text_color = button -> gx_text_button_disabled_text_color;

        if (button -> gx_widget_style & GX_STYLE_BUTTON_PUSHED)
        {
            pixelmap_id = button -> gx_radio_button_on_disabled_pixelmap_id;
        }
        else
        {
            pixelmap_id = button -> gx_radio_button_off_disabled_pixelmap_id;
        }
    }

    if (!(button -> gx_widget_status & GX_STATUS_TRANSPARENT))
    {
        _gx_widget_border_draw((GX_WIDGET *)button, GX_COLOR_ID_DEFAULT_BORDER, fill_color, fill_color, GX_TRUE);
    }

    if (pixelmap_id)
    {
        _gx_context_pixelmap_get(pixelmap_id, &pixelmap);
    }
    if (pixelmap)
    {
        _gx_widget_height_get((GX_WIDGET *)button, &yoffset);
        yoffset = (GX_VALUE)((yoffset - pixelmap -> gx_pixelmap_height) / 2);

        switch (button -> gx_widget_style & GX_STYLE_TEXT_ALIGNMENT_MASK)
        {
        case GX_STYLE_TEXT_RIGHT:
            _gx_widget_width_get((GX_WIDGET *)button, &xoffset);
            xoffset = (GX_VALUE)(xoffset - pixelmap -> gx_pixelmap_width + 1);
            /* draw the pixelmap on the right */
            _gx_canvas_pixelmap_draw((GX_VALUE)(button -> gx_widget_size.gx_rectangle_right -
                                                pixelmap -> gx_pixelmap_width + 1),
                                     (GX_VALUE)(button -> gx_widget_size.gx_rectangle_top + yoffset), pixelmap);

            /* draw the text to the left of the bitmap */
            xoffset = (GX_VALUE)(-(pixelmap -> gx_pixelmap_width * 3 / 2));
            break;

        case GX_STYLE_TEXT_CENTER:
            /* draw the pixelmap and text centered         */
            _gx_widget_width_get((GX_WIDGET *)button, &xoffset);
            xoffset = (GX_VALUE)((xoffset - pixelmap -> gx_pixelmap_width) / 2);

            /* draw the pixelmap centered */
            _gx_canvas_pixelmap_draw((GX_VALUE)(button -> gx_widget_size.gx_rectangle_left + xoffset),
                                     (GX_VALUE)(button -> gx_widget_size.gx_rectangle_top + yoffset), pixelmap);
            /* draw the text centered: */
            xoffset = 0;
            break;

        case GX_STYLE_TEXT_LEFT:
        default:
            /* draw the pixelmap on the left */
            _gx_canvas_pixelmap_draw(button -> gx_widget_size.gx_rectangle_left,
                                     (GX_VALUE)(button -> gx_widget_size.gx_rectangle_top + yoffset), pixelmap);

            /* draw the text to the right of the pixelmap */
            xoffset = (GX_VALUE)(pixelmap -> gx_pixelmap_width * 3 / 2);
            break;
        }
    }


    _gx_text_button_text_get_ext((GX_TEXT_BUTTON *)button, &string);

    if (string.gx_string_ptr)
    {
        _gx_widget_text_draw_ext((GX_WIDGET *)button, text_color,
                                 button -> gx_text_button_font_id,
                                 &string, xoffset, 0);
    }

    /* Draw button's children.  */
    _gx_widget_children_draw((GX_WIDGET *)button);
}

