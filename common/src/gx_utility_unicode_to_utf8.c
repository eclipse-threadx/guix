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
/*    _gx_utility_utf8_string_character_get               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function convert a unicode to a utf8.                          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    unicode                               Unicode to convert            */
/*    return_utf8_str                       Returned utf8 string          */
/*    return_utf8_size                      utf8 string size              */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
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
#ifdef GX_UTF8_SUPPORT
UINT  _gx_utility_unicode_to_utf8(ULONG unicode, GX_UBYTE *return_utf8_str, UINT *return_utf8_size)
{
    if (unicode < 0x0080)
    {
        *return_utf8_size = 1;
        return_utf8_str[0] = (GX_UBYTE)unicode;
    }
    else if (unicode < 0x0800)
    {
        *return_utf8_size = 2;
        return_utf8_str[0] = (GX_UBYTE)(0xC0 + (GX_UBYTE)((unicode & 0x7C0) >> 6));
        return_utf8_str[1] = (GX_UBYTE)(0x80 + (unicode & 0x3F));
    }
    else if (unicode < 0x10000)
    {
        *return_utf8_size = 3;
        return_utf8_str[0] = (GX_UBYTE)(0xE0 + (GX_UBYTE)((unicode & 0xF000) >> 12));
        return_utf8_str[1] = (GX_UBYTE)(0x80 + (GX_UBYTE)((unicode & 0xFC0) >> 6));
        return_utf8_str[2] = (GX_UBYTE)(0x80 + (unicode & 0x3F));
    }
    else if (unicode < 0x200000)
    {
        *return_utf8_size = 4;
        return_utf8_str[0] = (GX_UBYTE)(0xF0 + (GX_UBYTE)((unicode & 0x1C0000) >> 18));
        return_utf8_str[1] = (GX_UBYTE)(0x80 + (GX_UBYTE)((unicode & 0x3F000) >> 12));
        return_utf8_str[2] = (GX_UBYTE)(0x80 + (GX_UBYTE)((unicode & 0xFC0) >> 6));
        return_utf8_str[3] = (GX_UBYTE)(0x80 + (unicode & 0x3F));
    }
    else if (unicode < 0x4000000)
    {
        *return_utf8_size = 5;
        return_utf8_str[0] = (GX_UBYTE)(0xFC + (GX_UBYTE)((unicode & 0x3000000) >> 24));
        return_utf8_str[1] = (GX_UBYTE)(0x80 + (GX_UBYTE)((unicode & 0xFC0000) >> 18));
        return_utf8_str[2] = (GX_UBYTE)(0x80 + (GX_UBYTE)((unicode & 0x3F000) >> 12));
        return_utf8_str[3] = (GX_UBYTE)(0x80 + (GX_UBYTE)((unicode & 0xFC0) >> 6));
        return_utf8_str[4] = (GX_UBYTE)(0x80 + (unicode & 0x3F));
    }
    else
    {
        *return_utf8_size = 6;
        return_utf8_str[0] = (GX_UBYTE)(0xFC + (GX_UBYTE)((unicode & 0x40000000) >> 30));
        return_utf8_str[1] = (GX_UBYTE)(0x80 + (GX_UBYTE)((unicode & 0x3F000000) >> 24));
        return_utf8_str[2] = (GX_UBYTE)(0x80 + (GX_UBYTE)((unicode & 0xFC0000) >> 18));
        return_utf8_str[3] = (GX_UBYTE)(0x80 + (GX_UBYTE)((unicode & 0x3F000) >> 12));
        return_utf8_str[4] = (GX_UBYTE)(0x80 + (GX_UBYTE)((unicode & 0xFC0) >> 6));
        return_utf8_str[5] = (GX_UBYTE)(0x80 + (unicode & 0x3F));
    }

    return GX_SUCCESS;
}
#endif /* GX_UTF8_SUPPORT */

