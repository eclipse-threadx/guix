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
/*    _gx_utility_utf8_string_backward_character_length_get               */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function returns the glyph length of the previous character    */
/*    from the specified byte position.                                   */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    string                                UTF-8 string                  */
/*    start_index                           Specified byte position       */
/*    glyph_len                             Length of glyph value in byte */
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
UINT  _gx_utility_utf8_string_backward_character_length_get(GX_STRING *string, INT start_index, UINT *glyph_len)
{
UINT len = 0;

    /* Get glyph lengh of the character in backward direction. */
    while (start_index >= 0)
    {
        len++;

        if (((string -> gx_string_ptr[start_index] & 0x80) == 0) ||
            ((string -> gx_string_ptr[start_index] & 0xC0) == 0xC0))
        {
            /* End loop when byte match 0xxxxxxx or 11xxxxxx. */
            break;
        }

        start_index--;
    }

    *glyph_len = len;

    return GX_SUCCESS;
}
#endif /* GX_UTF8_SUPPORT */

