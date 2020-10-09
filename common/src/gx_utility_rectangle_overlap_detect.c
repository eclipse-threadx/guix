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
/*    _gx_utility_rectangle_overlap_detect                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service detects any overlap of the supplied rectangles.        */
/*    If overlap is found, the service returns GX_TRUE and the            */
/*    overlapping rectangle.                                              */
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
GX_BOOL  _gx_utility_rectangle_overlap_detect(GX_RECTANGLE *first_rectangle, GX_RECTANGLE *second_rectangle,
                                              GX_RECTANGLE *return_overlap_area)
{

GX_RECTANGLE test;
GX_BOOL      overlap = GX_FALSE;

    if (second_rectangle -> gx_rectangle_left < first_rectangle -> gx_rectangle_left)
    {
        test.gx_rectangle_left = first_rectangle -> gx_rectangle_left;
    }
    else
    {
        test.gx_rectangle_left = second_rectangle -> gx_rectangle_left;
    }

    if (second_rectangle -> gx_rectangle_top < first_rectangle -> gx_rectangle_top)
    {
        test.gx_rectangle_top = first_rectangle -> gx_rectangle_top;
    }
    else
    {
        test.gx_rectangle_top = second_rectangle -> gx_rectangle_top;
    }

    if (second_rectangle -> gx_rectangle_right > first_rectangle -> gx_rectangle_right)
    {
        test.gx_rectangle_right = first_rectangle -> gx_rectangle_right;
    }
    else
    {
        test.gx_rectangle_right = second_rectangle -> gx_rectangle_right;
    }
    if (second_rectangle -> gx_rectangle_bottom > first_rectangle -> gx_rectangle_bottom)
    {
        test.gx_rectangle_bottom = first_rectangle -> gx_rectangle_bottom;
    }
    else
    {
        test.gx_rectangle_bottom = second_rectangle -> gx_rectangle_bottom;
    }
    if (test.gx_rectangle_left <= test.gx_rectangle_right &&
        test.gx_rectangle_top <= test.gx_rectangle_bottom)
    {
        overlap = GX_TRUE;
    }
    if (return_overlap_area)
    {
        *return_overlap_area = test;
    }
    return(overlap);
}

