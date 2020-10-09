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
/* define the the table of sin values for one quadrant,
   angle value is enlarged by 4, sine value is enlarged by 256.*/
static GX_CONST GX_UBYTE sin_value_table[360] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1e, 0x1f, 0x20,
    0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30,
    0x31, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3f, 0x40, 0x41,
    0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4e, 0x4f, 0x50, 0x51,
    0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 0x60,
    0x61, 0x62, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70,
    0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
    0x80, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d,
    0x8e, 0x8f, 0x90, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0x9a,
    0x9b, 0x9c, 0x9d, 0x9e, 0x9f, 0xa0, 0xa1, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa7,
    0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb1, 0xb2, 0xb3, 0xb4,
    0xb5, 0xb5, 0xb6, 0xb7, 0xb8, 0xb8, 0xb9, 0xba, 0xbb, 0xbb, 0xbc, 0xbd, 0xbe, 0xbe, 0xbf,
    0xc0, 0xc1, 0xc1, 0xc2, 0xc3, 0xc4, 0xc4, 0xc5, 0xc6, 0xc6, 0xc7, 0xc8, 0xc9, 0xc9, 0xca,
    0xcb, 0xcb, 0xcc, 0xcd, 0xcd, 0xce, 0xcf, 0xcf, 0xd0, 0xd1, 0xd1, 0xd2, 0xd2, 0xd3, 0xd4,
    0xd4, 0xd5, 0xd6, 0xd6, 0xd7, 0xd7, 0xd8, 0xd9, 0xd9, 0xda, 0xda, 0xdb, 0xdc, 0xdc, 0xdd,
    0xdd, 0xde, 0xde, 0xdf, 0xdf, 0xe0, 0xe0, 0xe1, 0xe2, 0xe2, 0xe3, 0xe3, 0xe4, 0xe4, 0xe5,
    0xe5, 0xe6, 0xe6, 0xe7, 0xe7, 0xe8, 0xe8, 0xe8, 0xe9, 0xe9, 0xea, 0xea, 0xeb, 0xeb, 0xec,
    0xec, 0xec, 0xed, 0xed, 0xee, 0xee, 0xee, 0xef, 0xef, 0xf0, 0xf0, 0xf0, 0xf1, 0xf1, 0xf2,
    0xf2, 0xf2, 0xf3, 0xf3, 0xf3, 0xf4, 0xf4, 0xf4, 0xf5, 0xf5, 0xf5, 0xf6, 0xf6, 0xf6, 0xf6,
    0xf7, 0xf7, 0xf7, 0xf8, 0xf8, 0xf8, 0xf8, 0xf9, 0xf9, 0xf9, 0xf9, 0xfa, 0xfa, 0xfa, 0xfa,
    0xfb, 0xfb, 0xfb, 0xfb, 0xfb, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd,
    0xfd, 0xfd, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_math_sin_5_4_0                          PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service computes the sine of the supplied angl, which is an    */
/*    old version of _gx_utility_math_sin.                                */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    angle                                 Angle to compute sine of,     */
/*                                          the input angle should be     */
/*                                          scaled by 256.                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    sine                                  Sine of supplied angle        */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
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
INT _gx_utility_math_sin_5_4_0(INT angle)
{
INT y_sign;
INT sine;

    /* normalize the incoming angle */
    while (angle < 0)
    {
        angle += (256 * 360);
    }
    while (angle > (256 * 360))
    {
        angle -= (256 * 360);
    }

    if ((angle >= 0) && (angle <= 23040))
    {
        y_sign = 1;
    }
    else if ((angle > 23040) && (angle <= 46080))
    {
        angle = 46080 - angle;
        y_sign = 1;
    }
    else if ((angle > 46080) && (angle <= 69120))
    {
        angle -= 46080;
        y_sign = -1;
    }
    else
    {
        angle = 92160 - angle;
        y_sign = -1;
    }

    angle = (angle + 32) >> 6;

    if (angle == 360)
    {
        sine = 256 * y_sign;
    }
    else
    {
        sine = sin_value_table[angle] * y_sign;
    }
    return sine;
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_math_sin                                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service computes the sine of the supplied angle.               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    angle                                 Angle to compute sine of.     */
/*                                          The input angle is a fixed    */
/*                                          point data type, call         */
/*                                          GX_FIXED_VAL_MAKE to covnert  */
/*                                          from INT to fixed point data. */
/*                                          type.                         */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    sine                                  Sine of supplied angle.       */
/*                                          The return sin is a fixed     */
/*                                          point math data type, call    */
/*                                          GX_FIXED_VAL_TO_INT to convert*/
/*                                          from fixed point data type to */
/*                                          int.                          */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
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
GX_FIXED_VAL _gx_utility_math_sin(GX_FIXED_VAL angle)
{
INT y_sign;
INT sine;

    /* normalize the incoming angle */
    while (angle < 0)
    {
        angle += GX_FIXED_VAL_MAKE(360);
    }
    while (angle > GX_FIXED_VAL_MAKE(360))
    {
        angle -= GX_FIXED_VAL_MAKE(360);
    }

    if (angle <= GX_FIXED_VAL_MAKE(90))
    {
        y_sign = 1;
    }
    else if (angle <= GX_FIXED_VAL_MAKE(180))
    {
        angle = GX_FIXED_VAL_MAKE(180) - angle;
        y_sign = 1;
    }
    else if (angle <= GX_FIXED_VAL_MAKE(270))
    {
        angle -= GX_FIXED_VAL_MAKE(180);
        y_sign = -1;
    }
    else
    {
        angle = GX_FIXED_VAL_MAKE(360) - angle;
        y_sign = -1;
    }

    angle = angle + (GX_FIXED_VAL_HALF >> 2);
    angle = GX_FIXED_VAL_TO_INT(angle << 2);

    if (angle == 360)
    {
        sine = GX_FIXED_VAL_MAKE(y_sign);
    }
    else
    {
        sine = GX_FIXED_VAL_MAKE(sin_value_table[angle] * y_sign) >> 8;
    }
    return sine;
}

