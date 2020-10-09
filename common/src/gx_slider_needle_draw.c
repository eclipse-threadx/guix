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
/**   Slider Management (Slider)                                          */
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
#include "gx_slider.h"



/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_slider_needle_draw                              PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service draws a slider needle.                                 */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    slider                                Slider widget control block   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_slider_needle_position_get        Get the position of the       */
/*                                            slider needle               */
/*    _gx_context_line_color_set            Set the line color for the    */
/*                                            context                     */
/*    _gx_context_brush_width_set           Set the brush width of the    */
/*                                            context                     */
/*    _gx_canvas_line_draw                  Draw a line on the canvas     */
/*    _gx_context_brush_define              Define the brush of the       */
/*                                            context                     */
/*    _gx_canvas_rectangle_draw             Draw a rectangle              */
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
VOID  _gx_slider_needle_draw(GX_SLIDER *slider)
{
GX_VALUE        NeedleSpace;
GX_VALUE        center;
GX_RECTANGLE    NeedleSize;
GX_SLIDER_INFO *info;

    /* pick up the needle width */
    info = &slider -> gx_slider_info;
    if (slider -> gx_widget_style & GX_STYLE_SLIDER_VERTICAL)
    {
        NeedleSpace = info -> gx_slider_info_needle_height;
    }
    else
    {
        NeedleSpace = info -> gx_slider_info_needle_width;
    }

    /* get needle dimensions based on current value */
    _gx_slider_needle_position_get(slider, info, &NeedleSize);

    /* pick up brush pointer */
    _gx_context_line_color_set(GX_COLOR_ID_SLIDER_GROOVE_TOP);
    _gx_context_brush_width_set(1);

    /* draw the line on which the needle slides */
    if (slider -> gx_widget_style & GX_STYLE_SLIDER_VERTICAL)
    {
        center = (GX_VALUE)(slider -> gx_widget_size.gx_rectangle_left +
                            slider -> gx_widget_size.gx_rectangle_right) / 2;

        _gx_canvas_line_draw(center,
                             (GX_VALUE)(slider -> gx_widget_size.gx_rectangle_top + NeedleSpace),
                             center,
                             (GX_VALUE)(slider -> gx_widget_size.gx_rectangle_bottom - NeedleSpace));
        _gx_context_line_color_set(GX_COLOR_ID_SLIDER_GROOVE_BOTTOM);
        _gx_canvas_line_draw((GX_VALUE)(center + 1),
                             (GX_VALUE)(slider -> gx_widget_size.gx_rectangle_top + NeedleSpace),
                             (GX_VALUE)(center + 1),
                             (GX_VALUE)(slider -> gx_widget_size.gx_rectangle_bottom - NeedleSpace));
    }
    else
    {
        center = (GX_VALUE)(slider -> gx_widget_size.gx_rectangle_top +
                            slider -> gx_widget_size.gx_rectangle_bottom) / 2;

        _gx_canvas_line_draw((GX_VALUE)(slider -> gx_widget_size.gx_rectangle_left + NeedleSpace),
                             center, (GX_VALUE)(slider -> gx_widget_size.gx_rectangle_right - NeedleSpace),
                             center);
        _gx_context_line_color_set(GX_COLOR_ID_SLIDER_GROOVE_BOTTOM);
        _gx_canvas_line_draw((GX_VALUE)(slider -> gx_widget_size.gx_rectangle_left + NeedleSpace),
                             (GX_VALUE)(center + 1), (GX_VALUE)(slider -> gx_widget_size.gx_rectangle_right - NeedleSpace),
                             (GX_VALUE)(center + 1));
    }


    /* draw the needle itself */
    _gx_context_brush_define(GX_COLOR_ID_SLIDER_NEEDLE_OUTLINE,
                             GX_COLOR_ID_SLIDER_NEEDLE_FILL,
                             GX_BRUSH_SOLID_FILL);
    _gx_canvas_rectangle_draw(&NeedleSize);

    _gx_context_line_color_set(GX_COLOR_ID_SLIDER_NEEDLE_LINE1);

    if (slider -> gx_widget_style & GX_STYLE_SLIDER_VERTICAL)
    {
        _gx_canvas_line_draw((GX_VALUE)(NeedleSize.gx_rectangle_left + 1),
                             (GX_VALUE)(NeedleSize.gx_rectangle_top + 2),
                             (GX_VALUE)(NeedleSize.gx_rectangle_right - 1),
                             (GX_VALUE)(NeedleSize.gx_rectangle_top + 2));
        _gx_context_line_color_set(GX_COLOR_ID_SLIDER_NEEDLE_LINE2);

        _gx_canvas_line_draw((GX_VALUE)(NeedleSize.gx_rectangle_left + 1),
                             (GX_VALUE)(NeedleSize.gx_rectangle_top + 3),
                             (GX_VALUE)(NeedleSize.gx_rectangle_right - 1),
                             (GX_VALUE)(NeedleSize.gx_rectangle_top + 3));
    }
    else
    {
        _gx_canvas_line_draw((GX_VALUE)(NeedleSize.gx_rectangle_left + 2),
                             (GX_VALUE)(NeedleSize.gx_rectangle_top + 1),
                             (GX_VALUE)(NeedleSize.gx_rectangle_left + 2),
                             (GX_VALUE)(NeedleSize.gx_rectangle_bottom - 1));
        _gx_context_line_color_set(GX_COLOR_ID_SLIDER_NEEDLE_LINE2);

        _gx_canvas_line_draw((GX_VALUE)(NeedleSize.gx_rectangle_left + 3),
                             (GX_VALUE)(NeedleSize.gx_rectangle_top + 1),
                             (GX_VALUE)(NeedleSize.gx_rectangle_left + 3),
                             (GX_VALUE)(NeedleSize.gx_rectangle_bottom - 1));
    }
}

