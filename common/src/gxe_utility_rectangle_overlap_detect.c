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
/*    _gxe_utility_rectangle_overlap_detect               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in the utility rectangle overlap    */
/*    detect function call.                                               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    first_rectangle                       First rectangle               */
/*    second_rectangle                      Second rectangle              */
/*    return_overlap_area                   Overlapping rectangle area    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    [GX_TRUE | GX_FALSE]                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_rectangle_overlap_detect  Actual utility rectangle      */
/*                                          overlap detect function       */
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
GX_BOOL  _gxe_utility_rectangle_overlap_detect(GX_RECTANGLE *first_rectangle, GX_RECTANGLE *second_rectangle,
                                               GX_RECTANGLE *return_overlap_area)
{
GX_BOOL status;

    /* Check for invalid input pointers.  */
    if ((first_rectangle == GX_NULL) || (second_rectangle == GX_NULL))
    {
        return GX_FALSE;
    }

    /* Check for valid rectangle.  */
    if ((first_rectangle -> gx_rectangle_left > first_rectangle -> gx_rectangle_right) ||
        (first_rectangle -> gx_rectangle_top > first_rectangle -> gx_rectangle_bottom) ||
        (second_rectangle -> gx_rectangle_left > second_rectangle -> gx_rectangle_right) ||
        (second_rectangle -> gx_rectangle_top > second_rectangle -> gx_rectangle_bottom))
    {
        return GX_FALSE;
    }

    /* Call the actual utility rectangle overlap detect function.  */
    status = _gx_utility_rectangle_overlap_detect(first_rectangle, second_rectangle, return_overlap_area);

    /* Return completion status.  */
    return status;
}

