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


static GX_CONST GX_UBYTE sqrt_table[256] = {
    0x00, 0x10, 0x16, 0x1b, 0x20, 0x23, 0x27, 0x2a, 0x2d, 0x30, 0x32, 0x35, 0x37, 0x39, 0x3b,
    0x3d, 0x40, 0x41, 0x43, 0x45, 0x47, 0x49, 0x4b, 0x4c, 0x4e, 0x50, 0x51, 0x53, 0x54, 0x56,
    0x57, 0x59, 0x5a, 0x5b, 0x5d, 0x5e, 0x60, 0x61, 0x62, 0x63, 0x65, 0x66, 0x67, 0x68, 0x6a,
    0x6b, 0x6c, 0x6d, 0x6e, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a,
    0x7b, 0x7c, 0x7d, 0x7e, 0x80, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
    0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x96,
    0x97, 0x98, 0x99, 0x9a, 0x9b, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f, 0xa0, 0xa0, 0xa1, 0xa2, 0xa3,
    0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa7, 0xa8, 0xa9, 0xaa, 0xaa, 0xab, 0xac, 0xad, 0xad, 0xae,
    0xaf, 0xb0, 0xb0, 0xb1, 0xb2, 0xb2, 0xb3, 0xb4, 0xb5, 0xb5, 0xb6, 0xb7, 0xb7, 0xb8, 0xb9,
    0xb9, 0xba, 0xbb, 0xbb, 0xbc, 0xbd, 0xbd, 0xbe, 0xbf, 0xc0, 0xc0, 0xc1, 0xc1, 0xc2, 0xc3,
    0xc3, 0xc4, 0xc5, 0xc5, 0xc6, 0xc7, 0xc7, 0xc8, 0xc9, 0xc9, 0xca, 0xcb, 0xcb, 0xcc, 0xcc,
    0xcd, 0xce, 0xce, 0xcf, 0xd0, 0xd0, 0xd1, 0xd1, 0xd2, 0xd3, 0xd3, 0xd4, 0xd4, 0xd5, 0xd6,
    0xd6, 0xd7, 0xd7, 0xd8, 0xd9, 0xd9, 0xda, 0xda, 0xdb, 0xdb, 0xdc, 0xdd, 0xdd, 0xde, 0xde,
    0xdf, 0xe0, 0xe0, 0xe1, 0xe1, 0xe2, 0xe2, 0xe3, 0xe3, 0xe4, 0xe5, 0xe5, 0xe6, 0xe6, 0xe7,
    0xe7, 0xe8, 0xe8, 0xe9, 0xea, 0xea, 0xeb, 0xeb, 0xec, 0xec, 0xed, 0xed, 0xee, 0xee, 0xef,
    0xf0, 0xf0, 0xf1, 0xf1, 0xf2, 0xf2, 0xf3, 0xf3, 0xf4, 0xf4, 0xf5, 0xf5, 0xf6, 0xf6, 0xf7,
    0xf7, 0xf8, 0xf8, 0xf9, 0xf9, 0xfa, 0xfa, 0xfb, 0xfb, 0xfc, 0xfc, 0xfd, 0xfd, 0xfe, 0xfe,
    0xff
};



/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_math_sqrt                               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service computes the square root of the supplied value.        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    value                                 Value to compute square       */
/*                                            root of                     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    square_root                           Square root of supplied value */
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

UINT _gx_utility_math_sqrt(UINT value)
{
UINT square_root;

    if (value >= 0x7FFEA810)
    {
        square_root = 0xB504;
    }
    else if (value >= 0x1000000)
    {
        if (value >= 0x40000000)
        {
            square_root = (UINT)sqrt_table[value >> 24] << 8;
        }
        else if (value >= 0x10000000)
        {
            square_root = (UINT)sqrt_table[value >> 22] << 7;
        }
        else if (value >= 0x4000000)
        {
            square_root = (UINT)sqrt_table[value >> 20] << 6;
        }
        else
        {
            square_root = (UINT)sqrt_table[value >> 18] << 5;
        }

        square_root = (square_root + 1 + (value / square_root)) >> 1;
        square_root = (square_root + 1 + (value / square_root)) >> 1;
    }
    else if (value >= 0x10000)
    {
        if (value >= 0x400000)
        {
            square_root = (UINT)sqrt_table[value >> 16] << 4;
        }
        else if (value >= 0x100000)
        {
            square_root = (UINT)sqrt_table[value >> 14] << 3;
        }
        else if (value >= 0x40000)
        {
            square_root = (UINT)sqrt_table[value >> 12] << 2;
        }
        else
        {
            square_root = (UINT)sqrt_table[value >> 10] << 1;
        }

        square_root = (square_root + 1 + (value / square_root)) >> 1;
    }
    else
    {
        if (value >= 0x4000)
        {
            square_root = (UINT)(sqrt_table[value >> 8]) + 1;
        }
        else if (value >= 0x1000)
        {
            square_root = (UINT)(sqrt_table[value >> 6] >> 1) + 1;
        }
        else if (value >= 0x400)
        {
            square_root = (UINT)(sqrt_table[value >> 4] >> 2) + 1;
        }
        else if (value >= 0x100)
        {
            square_root = (UINT)(sqrt_table[value >> 2] >> 3) + 1;
        }
        else
        {
            square_root = sqrt_table[value] >> 4;
        }
    }

    if ((square_root * square_root) > value)
    {
        square_root--;
    }

    return square_root;
}

