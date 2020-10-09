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
/*    _gx_line_chart_data_draw                            PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function draws the chart data line                             */
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
/*    _gx_line_chart_y_scale_calculate                                    */
/*    _gx_context_brush_width_set                                         */
/*    _gx_context_brush_define                                            */
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
VOID _gx_line_chart_data_draw(GX_LINE_CHART *chart)
{
INT                 x_pos;
INT                 last_x_pos;
INT                 x_step;
INT                 y_scale;
INT                 y_pos;
INT                 index;
INT                 last_y_pos;
GX_RECTANGLE        chart_bound;
GX_LINE_CHART_INFO *chart_info;

    chart_info = &chart -> gx_line_chart_info;

    if (chart_info -> gx_line_chart_active_data_count <= 0 ||
        chart_info -> gx_line_chart_data == GX_NULL)
    {
        return;
    }

    chart_bound = chart -> gx_widget_size;
    chart_bound.gx_rectangle_left = (GX_VALUE)(chart_bound.gx_rectangle_left + chart_info -> gx_line_chart_left_margin);
    chart_bound.gx_rectangle_top = (GX_VALUE)(chart_bound.gx_rectangle_top + chart_info -> gx_line_chart_top_margin);
    chart_bound.gx_rectangle_right = (GX_VALUE)(chart_bound.gx_rectangle_right - chart_info -> gx_line_chart_right_margin);
    chart_bound.gx_rectangle_bottom = (GX_VALUE)(chart_bound.gx_rectangle_bottom - chart_info -> gx_line_chart_bottom_margin);

    x_step = chart_bound.gx_rectangle_right - chart_bound.gx_rectangle_left - chart_info -> gx_line_chart_axis_line_width;
    x_step = GX_FIXED_VAL_MAKE(x_step);

    if (chart_info -> gx_line_chart_max_data_count > 0)
    {
        x_step /= chart_info -> gx_line_chart_max_data_count;
    }
    last_x_pos = GX_FIXED_VAL_MAKE(chart_bound.gx_rectangle_left + chart_info -> gx_line_chart_axis_line_width + 1);

    _gx_line_chart_y_scale_calculate(chart, &y_scale);

    last_y_pos = chart_info -> gx_line_chart_data[0] - chart_info -> gx_line_chart_min_val;
    last_y_pos *= y_scale;
    last_y_pos = (INT)(chart_bound.gx_rectangle_bottom) - GX_FIXED_VAL_TO_INT(last_y_pos);

    _gx_context_brush_width_set((UINT)(chart_info -> gx_line_chart_data_line_width));
    _gx_context_brush_define(chart_info -> gx_line_chart_line_color, chart_info -> gx_line_chart_line_color, GX_BRUSH_ALIAS | GX_BRUSH_ROUND);

    for (index = 1; index < chart_info -> gx_line_chart_active_data_count; index++)
    {
        y_pos = chart_info -> gx_line_chart_data[index] - chart_info -> gx_line_chart_min_val;
        y_pos *= y_scale;
        y_pos = chart_bound.gx_rectangle_bottom - GX_FIXED_VAL_TO_INT(y_pos);

        x_pos = last_x_pos + x_step;
        _gx_canvas_line_draw((GX_VALUE)(GX_FIXED_VAL_TO_INT(last_x_pos)), (GX_VALUE)last_y_pos,
                             (GX_VALUE)(GX_FIXED_VAL_TO_INT(x_pos)), (GX_VALUE)y_pos);
        last_x_pos = x_pos;
        last_y_pos = y_pos;
    }
}

