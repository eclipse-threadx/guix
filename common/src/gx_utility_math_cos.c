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
/*    _gx_utility_math_cos_5_4_0                          PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service computes the cosine of the supplied angle, which is an */
/*    old version of _gx_utility_math_cos.                                */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    angle                                 Angle to compute cosine of,   */
/*                                          the input angle should be     */
/*                                          scaled by 256.                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    cosine                                Cosine of supplied angle      */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_math_sin_5_4_0                                          */
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
INT _gx_utility_math_cos_5_4_0(INT angle)
{

    return _gx_utility_math_sin_5_4_0(angle + 23040);
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_math_cos                                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service computes the cosine of the supplied angle.             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    angle                                 Angle to compute cosine of.   */
/*                                          The input angle is a fixed    */
/*                                          point data type, call         */
/*                                          GX_FIXED_VAL_MAKE to covnert  */
/*                                          from INT to fixed point data. */
/*                                          type.                         */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    cosine                                Cosine of supplied angle.     */
/*                                          The return consin is a fixed  */
/*                                          point math data type, call    */
/*                                          GX_FIXED_VAL_TO_INT to convert*/
/*                                          from fixed point data type to */
/*                                          int.                          */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_math_sin                                                */
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
GX_FIXED_VAL _gx_utility_math_cos(GX_FIXED_VAL angle)
{

    return _gx_utility_math_sin(angle + GX_FIXED_VAL_MAKE(90));
}

