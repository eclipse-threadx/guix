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
#include "gx_widget.h"
#include "gx_window.h"
#include "gx_line_chart.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_line_chart_create                               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates a GX_LINE_CHART widget                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    chart                                 GX_LINE_CHART control block   */
/*    name                                  Logical name of icon widget   */
/*    parent                                Pointer to the parent widget  */
/*    info                                  chart drawing parameters      */
/*    style                                 chart style flags             */
/*    chart_id                              chart ID                      */
/*    size                                  chart size                    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion Status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_window_create                                                   */
/*    _gx_widget_link                                                     */
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
UINT _gx_line_chart_create(GX_LINE_CHART *chart,
                           GX_CONST GX_CHAR *name,
                           GX_WIDGET *parent,
                           GX_CONST GX_LINE_CHART_INFO *info,
                           ULONG style,
                           USHORT chart_id,
                           GX_CONST GX_RECTANGLE *size)
{

    _gx_window_create((GX_WINDOW *) chart, name, parent, style, chart_id, size);

    chart -> gx_widget_draw_function =           (VOID (*)(GX_WIDGET *))_gx_line_chart_draw;

    chart -> gx_line_chart_info = *info;
    chart -> gx_widget_type = GX_TYPE_LINE_CHART;

    if (parent)
    {
        _gx_widget_link(parent, (GX_WIDGET *) chart);
    }

    return(GX_SUCCESS);
}
