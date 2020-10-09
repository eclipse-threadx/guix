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
/**   Line Chart  (Charting)                                              */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/
/*                                                                        */
/*  COMPONENT DEFINITION                                   RELEASE        */
/*                                                                        */
/*    gx_line_chart.h                                     PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file defines the GUIX line chart component, including all data */
/*    types and external references.  It is assumed that gx_api.h and     */
/*    gx_port.h have already been included.                               */
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

#ifndef GX_LINE_CHART_H
#define GX_LINE_CHART_H

/* APIs defined for line chart widget */

VOID _gx_line_chart_axis_draw(GX_LINE_CHART *chart);
UINT _gx_line_chart_create(GX_LINE_CHART *chart,
                           GX_CONST GX_CHAR *name,
                           GX_WIDGET *parent,
                           GX_CONST GX_LINE_CHART_INFO *info,
                           ULONG style,
                           USHORT chart_id,
                           GX_CONST GX_RECTANGLE *size);
VOID _gx_line_chart_data_draw(GX_LINE_CHART *chart);
VOID _gx_line_chart_draw(GX_LINE_CHART *chart);
UINT _gx_line_chart_update(GX_LINE_CHART *chart, INT *data, INT data_count);
UINT _gx_line_chart_y_scale_calculate(GX_LINE_CHART *chart_info, INT *return_val);

/* Define error checking shells for API services.  These are only referenced by the
   application.  */

UINT _gxe_line_chart_create(GX_LINE_CHART *chart,
                            GX_CONST GX_CHAR *name,
                            GX_WIDGET *parent,
                            GX_CONST GX_LINE_CHART_INFO *info,
                            ULONG style,
                            USHORT chart_id,
                            GX_CONST GX_RECTANGLE *size,
                            UINT control_block_size);
UINT _gxe_line_chart_update(GX_LINE_CHART *chart, INT *data, INT data_count);
UINT _gxe_line_chart_y_scale_calculate(GX_LINE_CHART *chart_info, INT *return_val);


#endif

