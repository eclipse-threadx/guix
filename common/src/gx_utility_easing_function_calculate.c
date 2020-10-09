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

/* Predefined table for power calculation, which used in expo and elastic easing function.
   The table value have been enlarged by 256 for precision need. */
#if defined(GX_EXPO_ELASTIC_EASING_SUPPORT)
static UINT _gx_utility_pow_0_10_table[501] = {
    0x100, 0x103, 0x107, 0x10a, 0x10e, 0x112, 0x116, 0x11a, 0x11e, 0x122, 0x126, 0x12a, 0x12e, 0x132, 0x136, 0x13b, 0x13f, 0x144, 0x148, 0x14d,
    0x151, 0x156, 0x15b, 0x160, 0x165, 0x16a, 0x16f, 0x174, 0x179, 0x17e, 0x184, 0x189, 0x18e, 0x194, 0x19a, 0x19f, 0x1a5, 0x1ab, 0x1b1, 0x1b7,
    0x1bd, 0x1c3, 0x1ca, 0x1d0, 0x1d7, 0x1dd, 0x1e4, 0x1eb, 0x1f1, 0x1f8, 0x200, 0x207, 0x20e, 0x215, 0x21d, 0x224, 0x22c, 0x234, 0x23c, 0x244,
    0x24c, 0x254, 0x25c, 0x265, 0x26d, 0x276, 0x27f, 0x288, 0x291, 0x29a, 0x2a3, 0x2ad, 0x2b6, 0x2c0, 0x2ca, 0x2d4, 0x2de, 0x2e8, 0x2f2, 0x2fd,
    0x308, 0x312, 0x31d, 0x329, 0x334, 0x33f, 0x34b, 0x357, 0x363, 0x36f, 0x37b, 0x387, 0x394, 0x3a1, 0x3ae, 0x3bb, 0x3c8, 0x3d6, 0x3e3, 0x3f1,
    0x400, 0x40e, 0x41c, 0x42b, 0x43a, 0x449, 0x458, 0x468, 0x478, 0x488, 0x498, 0x4a8, 0x4b9, 0x4ca, 0x4db, 0x4ec, 0x4fe, 0x510, 0x522, 0x534,
    0x547, 0x55a, 0x56d, 0x580, 0x594, 0x5a8, 0x5bc, 0x5d0, 0x5e5, 0x5fa, 0x610, 0x625, 0x63b, 0x652, 0x668, 0x67f, 0x696, 0x6ae, 0x6c6, 0x6de,
    0x6f6, 0x70f, 0x729, 0x742, 0x75c, 0x776, 0x791, 0x7ac, 0x7c7, 0x7e3, 0x800, 0x81c, 0x839, 0x856, 0x874, 0x892, 0x8b1, 0x8d0, 0x8f0, 0x910,
    0x930, 0x951, 0x972, 0x994, 0x9b6, 0x9d9, 0x9fc, 0xa20, 0xa44, 0xa69, 0xa8e, 0xab4, 0xada, 0xb01, 0xb28, 0xb50, 0xb78, 0xba1, 0xbcb, 0xbf5,
    0xc20, 0xc4b, 0xc77, 0xca4, 0xcd1, 0xcfe, 0xd2d, 0xd5c, 0xd8c, 0xdbc, 0xded, 0xe1f, 0xe52, 0xe85, 0xeb9, 0xeed, 0xf23, 0xf59, 0xf8f, 0xfc7,
    0x1000, 0x1039, 0x1073, 0x10ad, 0x10e9, 0x1125, 0x1163, 0x11a1, 0x11e0, 0x1220, 0x1261, 0x12a2, 0x12e5, 0x1328, 0x136d, 0x13b2, 0x13f9, 0x1440, 0x1488, 0x14d2,
    0x151c, 0x1568, 0x15b4, 0x1602, 0x1650, 0x16a0, 0x16f1, 0x1743, 0x1796, 0x17ea, 0x1840, 0x1897, 0x18ee, 0x1948, 0x19a2, 0x19fd, 0x1a5a, 0x1ab9, 0x1b18, 0x1b79,
    0x1bdb, 0x1c3f, 0x1ca4, 0x1d0a, 0x1d72, 0x1ddb, 0x1e46, 0x1eb2, 0x1f1f, 0x1f8f, 0x1fff, 0x2072, 0x20e6, 0x215b, 0x21d3, 0x224b, 0x22c6, 0x2342, 0x23c0, 0x2440,
    0x24c2, 0x2545, 0x25ca, 0x2651, 0x26da, 0x2765, 0x27f2, 0x2881, 0x2911, 0x29a4, 0x2a39, 0x2ad0, 0x2b69, 0x2c04, 0x2ca1, 0x2d41, 0x2de2, 0x2e86, 0x2f2d, 0x2fd5,
    0x3080, 0x312e, 0x31dd, 0x3290, 0x3344, 0x33fb, 0x34b5, 0x3572, 0x3631, 0x36f2, 0x37b7, 0x387e, 0x3948, 0x3a14, 0x3ae4, 0x3bb6, 0x3c8c, 0x3d64, 0x3e3f, 0x3f1e,
    0x3fff, 0x40e4, 0x41cc, 0x42b7, 0x43a6, 0x4497, 0x458d, 0x4685, 0x4781, 0x4881, 0x4984, 0x4a8a, 0x4b95, 0x4ca3, 0x4db5, 0x4ecb, 0x4fe4, 0x5102, 0x5223, 0x5349,
    0x5472, 0x55a0, 0x56d2, 0x5808, 0x5943, 0x5a82, 0x5bc5, 0x5d0d, 0x5e5a, 0x5fab, 0x6101, 0x625c, 0x63bb, 0x6520, 0x6689, 0x67f7, 0x696b, 0x6ae4, 0x6c62, 0x6de5,
    0x6f6e, 0x70fc, 0x7290, 0x7429, 0x75c8, 0x776d, 0x7918, 0x7ac9, 0x7c7f, 0x7e3c, 0x7fff, 0x81c9, 0x8399, 0x856f, 0x874c, 0x892f, 0x8b1a, 0x8d0b, 0x8f03, 0x9102,
    0x9308, 0x9515, 0x972a, 0x9946, 0x9b6a, 0x9d96, 0x9fc9, 0xa204, 0xa447, 0xa692, 0xa8e5, 0xab41, 0xada5, 0xb011, 0xb286, 0xb504, 0xb78b, 0xba1b, 0xbcb4, 0xbf57,
    0xc203, 0xc4b8, 0xc777, 0xca40, 0xcd12, 0xcfef, 0xd2d6, 0xd5c8, 0xd8c4, 0xdbca, 0xdedc, 0xe1f8, 0xe520, 0xe853, 0xeb91, 0xeedb, 0xf230, 0xf592, 0xf8ff, 0xfc79,
    0xffff, 0x10392, 0x10732, 0x10adf, 0x10e98, 0x1125f, 0x11634, 0x11a16, 0x11e06, 0x12204, 0x12611, 0x12a2b, 0x12e55, 0x1328d, 0x136d5, 0x13b2c, 0x13f92, 0x14408, 0x1488e, 0x14d24,
    0x151cb, 0x15682, 0x15b4a, 0x16023, 0x1650d, 0x16a09, 0x16f17, 0x17437, 0x17969, 0x17eae, 0x18406, 0x18970, 0x18eee, 0x19480, 0x19a25, 0x19fdf, 0x1a5ad, 0x1ab90, 0x1b188, 0x1b795,
    0x1bdb8, 0x1c3f1, 0x1ca40, 0x1d0a6, 0x1d722, 0x1ddb6, 0x1e461, 0x1eb24, 0x1f1ff, 0x1f8f3, 0x1ffff, 0x20725, 0x20e64, 0x215be, 0x21d31, 0x224bf, 0x22c68, 0x2342c, 0x23c0d, 0x24409,
    0x24c22, 0x25457, 0x25cab, 0x2651b, 0x26daa, 0x27658, 0x27f25, 0x28811, 0x2911d, 0x29a49, 0x2a396, 0x2ad04, 0x2b694, 0x2c047, 0x2ca1b, 0x2d413, 0x2de2f, 0x2e86f, 0x2f2d3, 0x2fd5c,
    0x3080c, 0x312e1, 0x31ddd, 0x32900, 0x3344b, 0x33fbf, 0x34b5b, 0x35721, 0x36311, 0x36f2b, 0x37b71, 0x387e3, 0x39481, 0x3a14c, 0x3ae45, 0x3bb6d, 0x3c8c3, 0x3d649, 0x3e3ff, 0x3f1e6,
    0x3ffff
};
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_pow_0_10                                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to get the power value, with base value    */
/*    equal to 2, exponent value between (0, 10).                         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    exponent                               Exponent value that have     */
/*                                             been enlarged by 256       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_utility_easing_function_calculate                               */
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
#if defined(GX_EXPO_ELASTIC_EASING_SUPPORT)
static INT _gx_utility_pow_0_10(INT b)
{
    b >>= 1;
    return (INT)_gx_utility_pow_0_10_table[b];
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_bounce_ease_out                         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to get the ratio of current value to total */
/*    value with bounce ease out animation applied.                       */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    t                                     Current step                  */
/*    d                                     Total steps                   */
/*    return_tt                             Numerator of returned ratio   */
/*    return_dd                             Denominator of returned ratio */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_utility_easing_function_calculate                               */
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
static VOID _gx_utility_bounce_ease_out(INT t, INT d, INT *return_tt, INT *return_dd)
{
INT tt;
INT dd;

    tt = 11 * t;
    if (tt < 4 * d)
    {
        tt = tt * tt;
        dd = 16 * d * d;
    }
    else if (tt < 8 * d)
    {
        dd = d * d;
        tt = (tt - 6 * d);
        tt = tt * tt + 12 * dd;
        dd = 16 * dd;
    }
    else if (tt < 10 * d)
    {
        dd = d * d;
        tt = tt - 9 * d;
        tt = tt * tt + 15 * dd;
        dd = 16 * dd;
    }
    else
    {
        dd = d * d;
        tt = 2 * tt - 21 * d;
        tt = tt * tt + 63 * dd;
        dd = 64 * dd;
    }

    *return_tt = tt;
    *return_dd = dd;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_easing_function_calculate               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to calculate next value with specified     */
/*    easing function.                                                    */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    easing_function_type                  Type of easing function       */
/*    start_val                             Start value of animation      */
/*    end_val                               End value of animation        */
/*    t                                     Current time of animation     */
/*    d                                     Duration time of animation    */
/*    current_val                           Retrieved current value       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_system_timer_update               Update active system timers   */
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
UINT _gx_utility_easing_function_calculate(USHORT easing_function_type, INT start_val, INT end_val, INT t, INT d, INT *current_val)
{
INT y;
INT tt;
INT dd;

    switch (easing_function_type & GX_ANIMATION_EASING_FUNC_MASK)
    {
    case GX_ANIMATION_BACK_EASE_IN:
        y = (end_val - start_val) * t / d;
        y = y * t / d;
        y = y * (27 * t - 17 * d) / 10 / d;
        y = y + start_val;
        break;

    case GX_ANIMATION_BACK_EASE_OUT:
        y = (end_val - start_val) * (t - d) / d;
        y = y * (t - d) / d;
        y = y * (27 * (t - d) + 17 * d) / 10 / d;
        y = y + end_val;
        break;

    case GX_ANIMATION_BACK_EASE_IN_OUT:
        if ((t << 1) < d)
        {
            y = (end_val - start_val) * t / d;
            y = y * t / d;
            y = y * (144 * t - 52 * d) / 10 / d;
            y = y + start_val;
        }
        else
        {
            y = (end_val - start_val) * (t - d) / d;
            y = y * (t - d) / d;
            y = y * (144 * (t - d) + 52 * d) / 10 / d;
            y = y + end_val;
        }
        break;

    case GX_ANIMATION_BOUNCE_EASE_IN:
        t = d - t;
        _gx_utility_bounce_ease_out(t, d, &tt, &dd);
        tt = dd - tt;
        y = ((end_val - start_val) * tt / dd) + start_val;
        break;
    case GX_ANIMATION_BOUNCE_EASE_OUT:
        _gx_utility_bounce_ease_out(t, d, &tt, &dd);
        y = ((end_val - start_val) * tt / dd) + start_val;
        break;
    case GX_ANIMATION_BOUNCE_EASE_IN_OUT:
        t <<= 1;
        if (t < d)
        {
            t = d - t;
            _gx_utility_bounce_ease_out(t, d, &tt, &dd);

            y = (end_val - start_val) * tt / dd / 2;
            y = (end_val + start_val) / 2 - y;
        }
        else
        {
            t = t - d;
            _gx_utility_bounce_ease_out(t, d, &tt, &dd);
            y = (end_val - start_val) * tt / dd / 2;
            y = y + (end_val + start_val) / 2;
        }
        break;
    case GX_ANIMATION_CIRC_EASE_IN:
        dd = (1 << 16);
        dd = dd * t / d;
        dd = dd * t / d;
        dd = (1 << 16) - dd;

        tt = (INT)_gx_utility_math_sqrt((UINT)dd);
        tt = 256 - tt;
        dd = 256;
        y = ((end_val - start_val) * tt / dd) + start_val;
        break;
    case GX_ANIMATION_CIRC_EASE_OUT:
        dd = (1 << 16);
        dd = dd * (t - d) / d;
        dd = dd * (t - d) / d;
        dd = (1 << 16) - dd;

        tt = (INT)_gx_utility_math_sqrt((UINT)dd);
        dd = 256;
        y = ((end_val - start_val) * tt / dd) + start_val;
        break;
    case GX_ANIMATION_CIRC_EASE_IN_OUT:
        if ((t << 1) < d)
        {
            dd = (1 << 18);
            dd = dd * t / d;
            dd = dd * t / d;
            dd = (1 << 16) - dd;

            tt = (INT)_gx_utility_math_sqrt((UINT)dd);
            tt = 256 - tt;
        }
        else
        {
            dd = (1 << 18);
            dd = dd * (t - d) / d;
            dd = dd * (t - d) / d;
            dd = (1 << 16) - dd;

            tt = (INT)_gx_utility_math_sqrt((UINT)dd);
            tt = 256 + tt;
        }
        dd = 512;
        y = ((end_val - start_val) * tt / dd) + start_val;
        break;
    case GX_ANIMATION_CUBIC_EASE_IN:
        y = (end_val - start_val) * t / d;
        y = y * t / d;
        y = y * t / d + start_val;
        break;
    case GX_ANIMATION_CUBIC_EASE_OUT:
        t = t - d;

        y = (end_val - start_val) * t / d;
        y = y * t / d;
        y = y * t / d + end_val;
        break;
    case GX_ANIMATION_CUBIC_EASE_IN_OUT:
        if ((t << 1) < d)
        {
            y = (end_val - start_val) * (t << 2) / d;
            y = y * t / d;
            y = y * t / d + start_val;
        }
        else
        {
            t = t - d;
            y = (end_val - start_val) * (t << 2) / d;
            y = y * t / d;
            y = y * t / d + end_val;
        }
        break;
#if defined(GX_EXPO_ELASTIC_EASING_SUPPORT)
    case GX_ANIMATION_ELASTIC_EASE_IN:
        tt = 1000 * (d - t) / d;
        dd = _gx_utility_pow_0_10(tt);
        tt = 900 * t / d - 967;
        tt = _gx_utility_math_sin(GX_FIXED_VAL_MAKE(tt));
        tt = GX_FIXED_VAL_TO_INT(tt << 8);
        y = (start_val - end_val) * tt / dd + start_val;
        break;
    case GX_ANIMATION_ELASTIC_EASE_OUT:
        tt = 1000 * t / d;
        dd = _gx_utility_pow_0_10(tt);
        tt = 900 * t / d - 67;
        tt = _gx_utility_math_sin(GX_FIXED_VAL_MAKE(tt));
        tt = GX_FIXED_VAL_TO_INT(tt << 8);
        y = (end_val - start_val) * tt / dd + end_val;
        break;
    case GX_ANIMATION_ELASTIC_EASE_IN_OUT:
        t <<= 1;
        if (t < d)
        {
            tt = 1000 * (d - t) / d;
            dd = _gx_utility_pow_0_10(tt);
            tt = 400 * t / d - 445;
            tt = _gx_utility_math_sin(GX_FIXED_VAL_MAKE(tt));
            tt = GX_FIXED_VAL_TO_INT(tt << 8);

            y = (start_val - end_val) * tt / dd;
            y = (y >> 1) + start_val;
        }
        else
        {
            tt = 1000 * (t - d) / d;
            dd = _gx_utility_pow_0_10(tt);
            tt = 400 * t / d - 445;
            tt = _gx_utility_math_sin(GX_FIXED_VAL_MAKE(tt));
            tt = GX_FIXED_VAL_TO_INT(tt << 8);

            y = (end_val - start_val) * tt / dd;
            y = (y >> 1) + end_val;
        }
        break;
    case GX_ANIMATION_EXPO_EASE_IN:
        t = 1000 * (d - t) / d;
        d = _gx_utility_pow_0_10(t);

        y = ((end_val - start_val) << 8) / d + start_val;
        break;
    case GX_ANIMATION_EXPO_EASE_OUT:
        t = 1000 * t / d;
        d = _gx_utility_pow_0_10(t);

        y = ((start_val - end_val) << 8) / d + end_val;
        break;
    case GX_ANIMATION_EXPO_EASE_IN_OUT:
        t <<= 1;
        if (t < d)
        {
            t = 1000 * (d - t) / d;
            d = _gx_utility_pow_0_10(t);

            y = ((end_val - start_val) << 7) / d + start_val;
        }
        else
        {
            t = 1000 * (t - d) / d;
            d = _gx_utility_pow_0_10(t);

            y = ((start_val - end_val) << 7) / d + end_val;
        }
        break;
#endif
    case GX_ANIMATION_QUAD_EASE_IN:
        y = (end_val - start_val) * t / d;
        y = y * t / d + start_val;
        break;
    case GX_ANIMATION_QUAD_EASE_OUT:
        t = t * ((d << 1) - t);
        d = d * d;

        y = ((end_val - start_val) * t / d) + start_val;
        break;
    case GX_ANIMATION_QUAD_EASE_IN_OUT:
        tt = t * t;
        dd = d * d;
        if ((t << 1) < d)
        {
            tt = (tt << 1);
        }
        else
        {
            tt = -(2 * tt - 4 * t * d + dd);
        }

        y = ((end_val - start_val) * tt / dd) + start_val;
        break;
    case GX_ANIMATION_QUART_EASE_IN:
        y = (end_val - start_val) * t / d;
        y = y * t / d;
        y = y * t / d;
        y = y * t / d + start_val;
        break;
    case GX_ANIMATION_QUART_EASE_OUT:
        t = t - d;
        y = (start_val - end_val) * t / d;
        y = y * t / d;
        y = y * t / d;
        y = y * t / d + end_val;
        break;
    case GX_ANIMATION_QUART_EASE_IN_OUT:
        if ((t << 1) < d)
        {
            y = (end_val - start_val) * (t << 3) / d;
            y = y * t / d;
            y = y * t / d;
            y = y * t / d + start_val;
        }
        else
        {
            t = t - d;
            y = (start_val - end_val) * (t << 3) / d;
            y = y * t / d;
            y = y * t / d;
            y = y * t / d + end_val;
        }
        break;
    case GX_ANIMATION_QUINT_EASE_IN:
        y = (end_val - start_val)  * t / d;
        y = y * t / d;
        y = y * t / d;
        y = y * t / d;
        y = y * t / d + start_val;
        break;
    case GX_ANIMATION_QUINT_EASE_OUT:
        t = t - d;
        y = (end_val - start_val) * t / d;
        y = y * t / d;
        y = y * t / d;
        y = y * t / d;
        y = y * t / d + end_val;
        break;
    case GX_ANIMATION_QUINT_EASE_IN_OUT:
        if ((t << 1) < d)
        {
            y = (end_val - start_val)  * (t << 4) / d;
            y = y * t / d;
            y = y * t / d;
            y = y * t / d;
            y = y * t / d + start_val;
        }
        else
        {
            t = t - d;
            y = (end_val - start_val) * (t << 4) / d;
            y = y * t / d;
            y = y * t / d;
            y = y * t / d;
            y = y * t / d + end_val;
        }
        break;
    case GX_ANIMATION_SINE_EASE_IN:
        /* cos(t/d * PI / 2 * 256). */
        t = _gx_utility_math_cos(GX_FIXED_VAL_MAKE(90) * t / d);
        t = GX_FIXED_VAL_ONE - t;

        y = GX_FIXED_VAL_TO_INT((end_val - start_val) * t);
        y += start_val;
        break;
    case GX_ANIMATION_SINE_EASE_OUT:
        /* sin(t/d * PI / 2). */
        t = _gx_utility_math_sin(GX_FIXED_VAL_MAKE(90) * t / d);

        y = GX_FIXED_VAL_TO_INT((end_val - start_val) * t);
        y += start_val;
        break;
    case GX_ANIMATION_SINE_EASE_IN_OUT:
        /* cos(t/d * PI * 256). */
        t = _gx_utility_math_cos(GX_FIXED_VAL_MAKE(180) * t / d);
        t = GX_FIXED_VAL_ONE - t;

        y = GX_FIXED_VAL_TO_INT((end_val - start_val) * t);
        y = (y >> 1) + start_val;
        break;
    default:
        y = (end_val - start_val) * t / d + start_val;
        break;
    }

    (*current_val) = y;
    return GX_SUCCESS;
}

