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
/**   Numeric Scroll Wheel Management (Scroll Wheel)                      */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_scroll_wheel.h"

GX_CALLER_CHECKING_EXTERNS

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_numeric_scroll_wheel_range_set                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in numeric scroll wheel range set   */
/*    call.                                                               */
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
/*    _gx_numeric_scroll_wheel_create       Actual numeric scroll wheel   */
/*                                            create call                 */
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
UINT  _gxe_numeric_scroll_wheel_range_set(GX_NUMERIC_SCROLL_WHEEL *wheel, INT start_val, INT end_val)
{
UINT status;

    /* Check for appropriate caller.  */
    GX_INIT_AND_THREADS_CALLER_CHECKING

    /* Check for invalid pointer. */
    if (wheel == GX_NULL)
    {
        return GX_PTR_ERROR;
    }

    status = _gx_numeric_scroll_wheel_range_set(wheel, start_val, end_val);

    return status;
}

