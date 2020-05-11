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
/**   Numeric Scroll Wheel Management (Scroll Wheel)                      */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_scroll_wheel.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_numeric_scroll_wheel_range_set                  PORTABLE C      */
/*                                                           6.0          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function sets range values for numeric scroll wheel widget.    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    wheel                                 Scroll wheel control block    */
/*    start_val                             Start value of numeric range  */
/*    end_val                               End value of numeric range    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_scroll_wheel_total_rows_set       Reset scroll wheel total rows */
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
/*                                                                        */
/**************************************************************************/
UINT  _gx_numeric_scroll_wheel_range_set(GX_NUMERIC_SCROLL_WHEEL *wheel, INT start_val, INT end_val)
{
UINT status;

    wheel -> gx_numeric_scroll_wheel_start_val = start_val;
    wheel -> gx_numeric_scroll_wheel_end_val = end_val;

    /* Reset total rows. */
    status = _gx_scroll_wheel_total_rows_set((GX_SCROLL_WHEEL *)wheel, GX_ABS(end_val - start_val) + 1);

    /* Return completion status.  */
    return status;
}

