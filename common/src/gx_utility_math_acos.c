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
/*    _gx_utility_math_acos_5_4_0                         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service computes the angle value of the arc cosine x.          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    x                                     Value whose arc cosine is     */
/*                                          computed, in the interval     */
/*                                          [-256, 256].                  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    angle                                 Angle value of arc cosine of x*/
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_math_asin_5_4_0           Compute arc sin               */
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
#if defined(GUIX_5_4_0_COMPATIBILITY)
INT _gx_utility_math_acos_5_4_0(INT x)
{
    INT angle = 0;

    angle = _gx_utility_math_asin_5_4_0(x);
    angle = 90 - angle;

    return angle;
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_math_acos                               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service computes the angle value of the arc cosine x, which is */
/*    an old version of _gx_utility_math_cos.                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    x                                     Value whose arc cosine is     */
/*                                          computed. The value is a      */
/*                                          fixed point math data type,   */
/*                                          call GX_FIXED_VAL_MAKE to do  */
/*                                          type conversion.              */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    angle                                 Angle value of arc cosine of x*/
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_math_asin                 Compute arc sin               */
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
INT _gx_utility_math_acos(GX_FIXED_VAL x)
{
INT angle = 0;

    angle = _gx_utility_math_asin(x);
    angle = 90 - angle;

    return angle;
}

