/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation 
 * 
 * This program and the accompanying materials are made available under the
 * terms of the MIT License which is available at
 * https://opensource.org/licenses/MIT.
 * 
 * SPDX-License-Identifier: MIT
 **************************************************************************/


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

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_line_chart_y_scale_calculate                   PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in the line chart y scale calculate */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    chart                                 Line chart control block      */
/*    return_value                          location to pass return value */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_line_chart_y_scale_calculate      Actual line chart draw call   */
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
UINT  _gxe_line_chart_y_scale_calculate(GX_LINE_CHART *chart, INT *return_value)
{
UINT status;

    /* Check for invalid input pointers.  */
    if ((chart == GX_NULL) || (return_value == GX_NULL))
    {
        return(GX_PTR_ERROR);
    }

    /* Call the actual icon button create function.  */
    status = _gx_line_chart_y_scale_calculate(chart, return_value);

    /* Return completion status.  */
    return status;
}

