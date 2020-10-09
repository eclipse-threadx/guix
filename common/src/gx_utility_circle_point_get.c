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
#include "gx_system.h"
#include "gx_utility.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_circle_point_get                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function gets the point on circle with specified angle         */
/*    and radius.                                                         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    xcenter                               x-coord of center of circle   */
/*    ycenter                               y-coord of center of circle   */
/*    r                                     Radius of circle              */
/*    angle                                 The angle where the point is  */
/*    point                                 Return value of the point at  */
/*                                            the angle                   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_math_cos                  Compute cosine                */
/*    _gx_utility_math_sin                  Compute sine                  */
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
UINT _gx_utility_circle_point_get(INT xcenter, INT ycenter, UINT r, INT angle, GX_POINT *point)
{

INT     x;
INT     y;
INT     d;
INT     x_sign;
INT     y_sign;
GX_BOOL swap = GX_FALSE;

    angle %= 360;

    if (angle < 0)
    {
        angle += 360;
    }

    if (angle == 0)
    {
        point -> gx_point_x = (GX_VALUE)(xcenter + (INT)r);
        point -> gx_point_y = (GX_VALUE)ycenter;
    }
    else if (angle == 90)
    {
        point -> gx_point_x = (GX_VALUE)xcenter;
        point -> gx_point_y = (GX_VALUE)(ycenter - (INT)r);
    }
    else if (angle == 180)
    {
        point -> gx_point_x = (GX_VALUE)(xcenter - (INT)r);
        point -> gx_point_y = (GX_VALUE)ycenter;
    }
    else if (angle == 270)
    {
        point -> gx_point_x = (GX_VALUE)xcenter;
        point -> gx_point_y = (GX_VALUE)(ycenter + (INT)r);
    }
    else
    {
        point -> gx_point_x = (GX_VALUE)(GX_FIXED_VAL_TO_INT(r * _gx_utility_math_cos(GX_FIXED_VAL_MAKE(angle))));
        point -> gx_point_y = (GX_VALUE)(GX_FIXED_VAL_TO_INT(r * _gx_utility_math_sin(GX_FIXED_VAL_MAKE(angle))));

        if (angle <= 90)
        {
            x_sign = 1;
            y_sign = 1;

            if (angle < 45)
            {
                swap = GX_TRUE;
            }
        }
        else if (angle <= 180)
        {
            x_sign = -1;
            y_sign = 1;

            if (angle > 135)
            {
                swap = GX_TRUE;
            }
        }
        else if (angle <= 270)
        {
            x_sign = -1;
            y_sign = -1;

            if (angle < 225)
            {
                swap = GX_TRUE;
            }
        }
        else
        {
            x_sign = 1;
            y_sign = -1;

            if (angle > 315)
            {
                swap = GX_TRUE;
            }
        }

        x = 0;
        y = (INT)r;
        d = (INT)(5 - 4 * r);

        point -> gx_point_x = (GX_VALUE)(point -> gx_point_x * x_sign);
        point -> gx_point_y = (GX_VALUE)(point -> gx_point_y * y_sign);

        if (swap)
        {
            GX_SWAP_VALS(point -> gx_point_x, point -> gx_point_y);
        }

        while (x <= y)
        {
            if ((x > point -> gx_point_x) || (y < point -> gx_point_y))
            {
                break;
            }

            if (d < 0)
            {
                d += 8 * x + 12;
            }
            else
            {
                d += 8 * (x - y) + 20;
                y--;
            }
            x++;
        }

        if (swap)
        {
            GX_SWAP_VALS(x, y);
        }

        x *= x_sign;
        y *= y_sign;

        point -> gx_point_x = (GX_VALUE)(xcenter + x);
        point -> gx_point_y = (GX_VALUE)(ycenter - y);
    }

    return GX_SUCCESS;
}

