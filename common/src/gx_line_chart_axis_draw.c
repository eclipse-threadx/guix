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
/**   Line Chart Management (Line Chart)                                  */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_context.h"
#include "gx_canvas.h"
#include "gx_window.h"
#include "gx_line_chart.h"



/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_line_chart_axis_draw                            PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function draws the x,y chart axis of a line chart              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    chart                                 Line chart                    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_context_brush_define                                            */
/*    _gx_context_brush_width_set                                         */
/*    _gx_canvas_line_draw                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
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
VOID _gx_line_chart_axis_draw(GX_LINE_CHART *chart)
{
GX_VALUE            center;
GX_LINE_CHART_INFO *chart_info;

    chart_info = &chart -> gx_line_chart_info;

    _gx_context_brush_define(chart_info -> gx_line_chart_axis_color, chart_info -> gx_line_chart_axis_color, 0);
    _gx_context_brush_width_set((UINT)(chart_info -> gx_line_chart_axis_line_width));

    /* draw the Y axis on the left side */
    center = (GX_VALUE)(chart -> gx_widget_size.gx_rectangle_left + chart_info -> gx_line_chart_left_margin + (chart_info -> gx_line_chart_axis_line_width / 2));
    _gx_canvas_line_draw(center,
                         (GX_VALUE)(chart -> gx_widget_size.gx_rectangle_top + chart_info -> gx_line_chart_top_margin),
                         center,
                         (GX_VALUE)(chart -> gx_widget_size.gx_rectangle_bottom - chart_info -> gx_line_chart_bottom_margin - chart_info -> gx_line_chart_axis_line_width + (GX_VALUE)1));

    /* draw the X axis along the bottom */
    center = (GX_VALUE)(chart -> gx_widget_size.gx_rectangle_bottom - chart_info -> gx_line_chart_bottom_margin - (chart_info -> gx_line_chart_axis_line_width / 2));
    _gx_canvas_line_draw((GX_VALUE)(chart -> gx_widget_size.gx_rectangle_left + chart_info -> gx_line_chart_left_margin),
                         center,
                         (GX_VALUE)(chart -> gx_widget_size.gx_rectangle_right - chart_info -> gx_line_chart_right_margin),
                         center);
}

