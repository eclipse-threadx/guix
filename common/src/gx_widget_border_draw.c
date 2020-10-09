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


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_widget_border_draw                              PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function draws the specified widget.                           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    widget                                Pointer to widget             */
/*    border_color                          Color of border               */
/*    upper_color                           Color of upper fill           */
/*    lower_color                           Color of lower fill           */
/*    fill                                  flag to request fill          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_border_width_get           Get border width              */
/*    _gx_context_raw_line_color_set        Set line color in context     */
/*    _gx_context_brush_define              Define the brush of the       */
/*                                            context                     */
/*    _gx_context_brush_width_set           Set brush width in context    */
/*    _gx_canvas_line_draw                  Draw a line                   */
/*    _gx_widget_client_get                 Get widget client             */
/*    _gx_canvas_rectangle_draw             Draw rectangle                */

/*    _gx_context_fill_color_set            Set the fill color            */
/*    _gx_canvas_pixel_draw                 Draw a pixel                  */
/*    _gx_context_line_color_set            Set line color                */
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
VOID  _gx_widget_border_draw(GX_WIDGET *widget,
                             GX_RESOURCE_ID border_color,
                             GX_RESOURCE_ID upper_color,
                             GX_RESOURCE_ID lower_color, GX_BOOL fill)
{
GX_POINT     corner;
GX_RECTANGLE fillrect;
GX_VALUE     widget_height;
INT          left;
INT          right;
INT          top;
GX_VALUE     border_width;
GX_BOOL      rounded = GX_FALSE;

    _gx_widget_border_width_get(widget, &border_width);

    if (border_width > 0)
    {
        /* Pick up the current brush.  */

        _gx_context_line_color_set(border_color);
        top = widget -> gx_widget_size.gx_rectangle_top;

        if (widget -> gx_widget_style & GX_STYLE_BORDER_THICK)
        {
            _gx_context_brush_width_set((UINT)border_width);
            top += border_width / 2;
        }
        else
        {
            _gx_context_brush_width_set(1);
        }

        /* Draw line across the top.  */
        left = widget -> gx_widget_size.gx_rectangle_left;
        right = widget -> gx_widget_size.gx_rectangle_right;

        if (widget -> gx_widget_parent)
        {
            if (widget -> gx_widget_parent -> gx_widget_type != GX_TYPE_ROOT_WINDOW ||
                (widget -> gx_widget_status & GX_STATUS_TRANSPARENT))
            {
                rounded = GX_TRUE;
                left += 2;
                right -= 2;
            }
        }

        _gx_canvas_line_draw((GX_VALUE)left, (GX_VALUE)top, (GX_VALUE)right, (GX_VALUE)top);

        /* Draw line across the bottom.  */
        if (widget -> gx_widget_style & GX_STYLE_BORDER_THICK)
        {
            top = widget -> gx_widget_size.gx_rectangle_bottom - border_width + 1;
            top += border_width / 2;
            _gx_canvas_line_draw((GX_VALUE)left, (GX_VALUE)top, (GX_VALUE)right, (GX_VALUE)top);
        }
        else
        {
            _gx_canvas_line_draw((GX_VALUE)left,
                                 (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_bottom),
                                 (GX_VALUE)right,
                                 (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_bottom));
        }

        /* Draw line down left side.  */
        left = widget -> gx_widget_size.gx_rectangle_left;

        if (widget -> gx_widget_style & GX_STYLE_BORDER_THICK)
        {
            left += border_width / 2;
        }

        if (rounded)
        {
            _gx_canvas_line_draw((GX_VALUE)left, (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_top + 2),
                                 (GX_VALUE)left, (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_bottom - 2));
        }
        else
        {
            _gx_canvas_line_draw((GX_VALUE)left, (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_top + border_width - 1),
                                 (GX_VALUE)left, (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_bottom - border_width + 1));
        }

        /* Draw line down right side.  */
        right = widget -> gx_widget_size.gx_rectangle_right - border_width + 1;
        right += border_width / 2;

        if (widget -> gx_widget_style & GX_STYLE_BORDER_THICK)
        {
            _gx_canvas_line_draw((GX_VALUE)right, (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_top + border_width - 1),
                                 (GX_VALUE)right, (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_bottom - border_width + 1));

            /* draw highlight along top border */
            _gx_context_line_color_set(GX_COLOR_ID_SHINE);
            _gx_context_brush_width_set(1);

            _gx_canvas_line_draw((GX_VALUE)(widget -> gx_widget_size.gx_rectangle_left + 2),
                                 (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_top + 1),
                                 (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_right - 2),
                                 (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_top + 1));

            /* draw higtlight along left border */
            _gx_canvas_line_draw((GX_VALUE)(widget -> gx_widget_size.gx_rectangle_left + 1),
                                 (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_top + 2),
                                 (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_left + 1),
                                 (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_bottom - 2));


            /* draw shadow along right border */
            _gx_context_line_color_set(GX_COLOR_ID_SHADOW);

            _gx_canvas_line_draw((GX_VALUE)(widget -> gx_widget_size.gx_rectangle_right - 1),
                                 (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_top + 2),
                                 (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_right - 1),
                                 (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_bottom - 2));

            /* draw shadow along bottom border */
            _gx_canvas_line_draw((GX_VALUE)(widget -> gx_widget_size.gx_rectangle_left + 2),
                                 (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_bottom - 1),
                                 (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_right - 2),
                                 (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_bottom - 1));
        }
        else
        {
            if (rounded)
            {
                _gx_canvas_line_draw((GX_VALUE)(widget -> gx_widget_size.gx_rectangle_right),
                                     (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_top + 2),
                                     (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_right),
                                     (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_bottom - 2));
            }
            else
            {
                _gx_canvas_line_draw((GX_VALUE)(widget -> gx_widget_size.gx_rectangle_right),
                                     (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_top + border_width - 1),
                                     (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_right),
                                     (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_bottom - border_width + 1));
            }
        }
    }

    if (fill && !(widget -> gx_widget_style & GX_STYLE_TRANSPARENT))
    {
        _gx_widget_client_get(widget, border_width, &fillrect);

        /* Pickup widget width and height.  */
        _gx_widget_height_get(widget, &widget_height);

        /* fill my client area */
        /* first setup the brush color and style */
        _gx_context_brush_define(upper_color, upper_color, GX_BRUSH_SOLID_FILL);
        _gx_context_brush_width_set(0);

        if (upper_color != lower_color)
        {
            fillrect.gx_rectangle_bottom = (GX_VALUE)(fillrect.gx_rectangle_top + (widget_height / 2) - 1);
            _gx_canvas_rectangle_draw(&fillrect);

            _gx_context_fill_color_set(lower_color);
            fillrect.gx_rectangle_top = (GX_VALUE)(fillrect.gx_rectangle_bottom + 1);
            fillrect.gx_rectangle_bottom = (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_bottom - 1);
        }
        _gx_canvas_rectangle_draw(&fillrect);
    }

    if ((widget -> gx_widget_style & GX_STYLE_BORDER_MASK) == GX_STYLE_BORDER_NONE)
    {
        return;
    }

    if (rounded)
    {
        _gx_context_line_color_set(border_color);

        /* the top-left corner */
        corner.gx_point_x = (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_left + 2);
        corner.gx_point_y = (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_top + 1);
        _gx_canvas_pixel_draw(corner);
        corner.gx_point_x = (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_left + 1);
        _gx_canvas_pixel_draw(corner);
        corner.gx_point_y = (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_top + 2);
        _gx_canvas_pixel_draw(corner);

        /* the top-right corner */
        corner.gx_point_x = (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_right - 2);
        corner.gx_point_y = (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_top + 1);
        _gx_canvas_pixel_draw(corner);
        corner.gx_point_x = (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_right - 1);
        _gx_canvas_pixel_draw(corner);
        corner.gx_point_y = (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_top + 2);
        _gx_canvas_pixel_draw(corner);

        /* the bottom-right corner */
        corner.gx_point_x = (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_right - 2);
        corner.gx_point_y = (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_bottom - 1);
        _gx_canvas_pixel_draw(corner);
        corner.gx_point_x = (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_right - 1);
        _gx_canvas_pixel_draw(corner);
        corner.gx_point_y = (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_bottom - 2);
        _gx_canvas_pixel_draw(corner);

        /* the bottom-left corner */
        corner.gx_point_x = (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_left + 2);
        corner.gx_point_y = (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_bottom - 1);
        _gx_canvas_pixel_draw(corner);
        corner.gx_point_x = (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_left + 1);
        _gx_canvas_pixel_draw(corner);
        corner.gx_point_y = (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_bottom - 2);
        _gx_canvas_pixel_draw(corner);
    }

    if (border_width != 2)
    {
        return;
    }

    /* check for raised or recessed border style.  */

    _gx_context_brush_width_set(1);

    if (widget -> gx_widget_style & GX_STYLE_BORDER_RECESSED)
    {
        /* pick up the shadow color */
        _gx_context_line_color_set(GX_COLOR_ID_SHADOW);
    }
    else
    {
        _gx_context_line_color_set(GX_COLOR_ID_SHINE);
    }

    if (rounded)
    {
        /* line down left edge */
        _gx_canvas_line_draw((GX_VALUE)(widget -> gx_widget_size.gx_rectangle_left + 1),
                             (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_top + 3),
                             (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_left + 1),
                             (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_bottom - 3));

        /* line across the top */
        _gx_canvas_line_draw((GX_VALUE)(widget -> gx_widget_size.gx_rectangle_left + 3),
                             (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_top + 1),
                             (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_right - 3),
                             (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_top + 1));

        corner.gx_point_x = (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_left + 2);
        corner.gx_point_y = (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_top + 2);
        _gx_canvas_pixel_draw(corner);
        corner.gx_point_x = (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_right - 2);
        _gx_canvas_pixel_draw(corner);
    }
    else
    {
        /* line down left edge */
        _gx_canvas_line_draw((GX_VALUE)(widget -> gx_widget_size.gx_rectangle_left + 1),
                             (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_top + border_width),
                             (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_left + 1),
                             (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_bottom - 1));

        /* line across the top */
        _gx_canvas_line_draw((GX_VALUE)(widget -> gx_widget_size.gx_rectangle_left + 1),
                             (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_top + 1),
                             (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_right - 1),
                             (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_top + 1));
    }

    if (widget -> gx_widget_style & GX_STYLE_BORDER_RECESSED)
    {
        _gx_context_line_color_set(GX_COLOR_ID_SHINE);
    }
    else
    {
        /* pick up the shadow color */
        _gx_context_line_color_set(GX_COLOR_ID_SHADOW);
    }

    if (rounded)
    {
        /* line down right edge */
        _gx_canvas_line_draw((GX_VALUE)(widget -> gx_widget_size.gx_rectangle_right - 1),
                             (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_top + 3),
                             (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_right - 1),
                             (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_bottom - 3));

        /* line across the bottom */
        _gx_canvas_line_draw((GX_VALUE)(widget -> gx_widget_size.gx_rectangle_left + 3),
                             (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_bottom - 1),
                             (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_right - 3),
                             (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_bottom - 1));

        corner.gx_point_x = (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_right - 2);
        corner.gx_point_y = (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_bottom - 2);
        _gx_canvas_pixel_draw(corner);
        corner.gx_point_x = (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_left + 2);
        _gx_canvas_pixel_draw(corner);
    }
    else
    {
        /* line down right edge */
        _gx_canvas_line_draw((GX_VALUE)(widget -> gx_widget_size.gx_rectangle_right - 1),
                             (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_top + 2),
                             (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_right - 1),
                             (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_bottom - 1));

        /* line across the bottom */
        _gx_canvas_line_draw((GX_VALUE)(widget -> gx_widget_size.gx_rectangle_left + 2),
                             (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_bottom - 1),
                             (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_right - 2),
                             (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_bottom - 1));
    }
}

