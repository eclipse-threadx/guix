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
/*    _gx_utility_rectangle_compare                       PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service compares the first and second rectangle. If they are   */
/*    equal, a value of GX_TRUE is returned.                              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    first_rectangle                       First rectangle               */
/*    second_rectangle                      Second rectangle              */
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
GX_BOOL  _gx_utility_rectangle_compare(GX_RECTANGLE *first_rectangle, GX_RECTANGLE *second_rectangle)
{

    if (second_rectangle -> gx_rectangle_left == first_rectangle -> gx_rectangle_left &&
        second_rectangle -> gx_rectangle_top == first_rectangle -> gx_rectangle_top &&
        second_rectangle -> gx_rectangle_right == first_rectangle -> gx_rectangle_right &&
        second_rectangle -> gx_rectangle_bottom == first_rectangle -> gx_rectangle_bottom)
    {
        return GX_TRUE;
    }
    return GX_FALSE;
}

