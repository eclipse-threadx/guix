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
/**   Utility (Utility)                                                   */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_utility.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_utility_rectangle_point_detect                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in the utility rectangle point      */
/*    detect function call.                                               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    rectangle                             Rectangle                     */
/*    point                                 Point                         */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    [GX_TRUE | GX_FALSE]                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_rectangle_point_detect    Actual utility rectangle      */
/*                                          point detect function         */
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
GX_BOOL  _gxe_utility_rectangle_point_detect(GX_RECTANGLE *rectangle, GX_POINT point)
{
GX_BOOL status;

    /* Check for invalid input pointers.  */
    if (rectangle == GX_NULL)
    {
        return(GX_FALSE);
    }

    /* Check for valid rectangle.  */
    if ((rectangle -> gx_rectangle_left > rectangle -> gx_rectangle_right) ||
        (rectangle -> gx_rectangle_top > rectangle -> gx_rectangle_bottom))
    {
        return(GX_FALSE);
    }

    /* Call the actual utility rectangle point detect function.  */
    status = _gx_utility_rectangle_point_detect(rectangle, point);

    /* Return completion status.  */
    return status;
}

