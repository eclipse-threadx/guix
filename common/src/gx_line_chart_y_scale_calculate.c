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
#include "gx_window.h"
#include "gx_line_chart.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_line_chart_y_scale_calculate                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function calculates chart y axis scale value                   */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    chart                                 Line chart                    */
/*    return_value                          location to pass return value */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion Status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    none                                                                */
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
UINT _gx_line_chart_y_scale_calculate(GX_LINE_CHART *chart, INT *return_value)
{
INT y_scale;
GX_LINE_CHART_INFO *chart_info;

    chart_info = &chart -> gx_line_chart_info;

    y_scale = chart -> gx_widget_size.gx_rectangle_bottom - chart -> gx_widget_size.gx_rectangle_top + 1;
    y_scale -= chart_info -> gx_line_chart_top_margin + chart_info -> gx_line_chart_bottom_margin;
    y_scale -= chart_info -> gx_line_chart_axis_line_width;

    y_scale = GX_FIXED_VAL_MAKE(y_scale);

    if (chart_info -> gx_line_chart_max_val != chart_info -> gx_line_chart_min_val)
    {
        y_scale /= (chart_info -> gx_line_chart_max_val - chart_info -> gx_line_chart_min_val);
    }
    *return_value = y_scale;
    return GX_SUCCESS;
}





