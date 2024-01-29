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
/**   Circular Gauge Management (Circular Gauge)                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_circular_gauge.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_circular_gauge_angle_get                       PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in circular gauge angle range get   */
/*      call.                                                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    circular_gauge                        Circular gauge control block  */
/*    start_angle                           Retrieved current angle.      */
/*    target_angle                          Retrieved target angle.       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_circular_gauge_angle_get          Actual circular gauge angle   */
/*                                          get call.                     */
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
UINT _gxe_circular_gauge_angle_get(GX_CIRCULAR_GAUGE *circular_gauge, INT *angle)
{
UINT status;

    /* Check for invalid input pointers.  */
    if (circular_gauge == GX_NULL || angle == GX_NULL)
    {
        return GX_PTR_ERROR;
    }

    /* Call the actual gauge angle get function.  */
    status = _gx_circular_gauge_angle_get(circular_gauge, angle);

    return status;
}

