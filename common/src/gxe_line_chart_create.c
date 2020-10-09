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
/**   Line Chart  (Chart)                                                 */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE

/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_line_chart.h"

/* Bring in externs for caller checking code.  */
GX_CALLER_CHECKING_EXTERNS

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_line_chart_create                              PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in the line chart create function.  */
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
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_line_chart_create                 Actual line chart create call */
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
UINT  _gxe_line_chart_create(GX_LINE_CHART *chart,
                             GX_CONST GX_CHAR *name,
                             GX_WIDGET *parent,
                             GX_CONST GX_LINE_CHART_INFO *info,
                             ULONG style,
                             USHORT chart_id,
                             GX_CONST GX_RECTANGLE *size,
                             UINT control_block_size)
{
UINT status;

    /* Check for appropriate caller.  */
    GX_INIT_AND_THREADS_CALLER_CHECKING

    /* Check for invalid input pointers.  */
    if ((chart == GX_NULL) || (size == GX_NULL))
    {
        return(GX_PTR_ERROR);
    }

    if (control_block_size != sizeof(GX_LINE_CHART))
    {
        return(GX_INVALID_SIZE);
    }

    /* Check for widget already created.  */
    if (chart -> gx_widget_type != 0)
    {
        return(GX_ALREADY_CREATED);
    }

    /* Call the actual icon button create function.  */
    status = _gx_line_chart_create(chart, name, parent, info, style, chart_id, size);

    /* Return completion status.  */
    return status;
}

