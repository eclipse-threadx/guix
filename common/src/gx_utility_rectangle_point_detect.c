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
/*    _gx_utility_rectangle_point_detect                  PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service detects if the specified point resides in the          */
/*    rectangle. If the point does reside in the rectangle, the service   */
/*    returns GX_TRUE.                                                    */
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
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*    GUIX Internal Code                                                  */
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
GX_BOOL  _gx_utility_rectangle_point_detect(GX_RECTANGLE *rectangle, GX_POINT point)
{
    if (rectangle -> gx_rectangle_left <= point.gx_point_x &&
        rectangle -> gx_rectangle_right >= point.gx_point_x &&
        rectangle -> gx_rectangle_top <= point.gx_point_y &&
        rectangle -> gx_rectangle_bottom >= point.gx_point_y)
    {

        /* Yes, the point is within the rectangle.  */
        return(GX_TRUE);
    }
    else
    {

        /* No, the point is not within the rectangle.  */
        return(GX_FALSE);
    }
}

