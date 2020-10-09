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
/*    This function parses utf8 string to multibyte glyph.                */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    utf8_str                              UTF-8 string                  */
/*    glyph_value                           Multibyte value of glyph      */
/*    byte_count                            Length of UTF-8 string in byte*/
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
/*    _gx_system_string_width_get                                         */
/*    _gx_display_driver_indexed_color_text_draw                          */
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
UINT  _gx_utility_utf8_string_character_get(GX_STRING *utf8_str, GX_CHAR_CODE *glyph_value, UINT *glyph_len)
{
GX_CONST GX_CHAR *ch;
UINT              bytes;
GX_CHAR_CODE      value = 0;
UINT              len;
UINT              byte_count;

    ch = utf8_str -> gx_string_ptr;
    byte_count = utf8_str -> gx_string_length;

    /* BOM check. */
    if ((byte_count >= 3) && (*ch == (char)0xEF) && (*(ch + 1) == (char)0xBB) && (*(ch + 2) == (char)0xBF))
    {
        /* It is BOM. Skip it. */
        ch += 3;
        byte_count -= 3;
    }

    if (byte_count == 0)
    {
        /* Zero length string. Return error. */
        if (glyph_value)
        {
            *glyph_value = 0;
        }

        if (glyph_len)
        {
            *glyph_len = 0;
        }
        return GX_INVALID_VALUE;
    }

    /* Reset glyph length to 1. */
    len = 1;

    /* Check the first byte */
    if ((*ch & 0x80) == 0)
    {

        utf8_str -> gx_string_ptr = ch + 1;
        utf8_str -> gx_string_length -= 1;

        /* One byte glyph. */
        if (glyph_value)
        {
            *glyph_value = (*ch & ~0x80) & 0xFF;
        }

        if (glyph_len)
        {
            *glyph_len = 1;
        }

        return GX_SUCCESS;
    }
    else if ((*ch & 0xE0) == 0xC0)
    {

        /* Two bytes glyph. */
        bytes = 2;
        value = (*ch & ~0xE0) & 0xFF;
    }
    else if ((*ch & 0xF0) == 0xE0)
    {

        /* Three bytes glyph. */
        bytes = 3;
        value = (*ch & ~0xF0) & 0xFF;
    }
    else if ((*ch & 0xF8) == 0xF0)
    {

        /* Four bytes glyph. */
        bytes = 4;
        value = (*ch & ~0xF8) & 0xFF;
    }
    else if ((*ch & 0xFC) == 0xF8)
    {

        /* Five bytes glyph. */
        bytes = 5;
        value = (*ch & ~0xFC) & 0xFF;
    }
    else if ((*ch & 0xFE) == 0xFC)
    {

        /* Six bytes glyph. */
        bytes = 6;
        value = (*ch & ~0xFE) & 0xFF;
    }
    else
    {
        /* Not a valid utf8 glyph. */
        utf8_str -> gx_string_ptr = ch + 1;
        utf8_str -> gx_string_length -= 1;

        if (glyph_value)
        {
            *glyph_value = 0;
        }

        if (glyph_len)
        {
            *glyph_len = 1;
        }

        return GX_INVALID_VALUE;
    }

    if (byte_count < bytes)
    {
        /* Not a valid utf8 glyph. */
        if (glyph_value)
        {
            *glyph_value = 0;
        }

        if (glyph_len)
        {
            *glyph_len = bytes;
        }
        utf8_str -> gx_string_length -= bytes;

        return GX_INVALID_VALUE;
    }

    while (len < bytes)
    {
        ch++;
        len++;

        if ((*ch & 0xC0) != 0x80)
        {

            /* Not a valid utf8 glyph. */
            if (glyph_len)
            {
                *glyph_len = len;
            }

            utf8_str -> gx_string_ptr = ch;
            utf8_str -> gx_string_length -= len;

            return GX_INVALID_VALUE;
        }

        value = (GX_CHAR_CODE)(value << 6);
        value = (GX_CHAR_CODE)(value + ((*ch & ~0xC0) & 0xFF));
    }

    if (glyph_value)
    {
        *glyph_value = value;
    }

    if (glyph_len)
    {
        *glyph_len = len;
    }

    utf8_str -> gx_string_ptr = ch + 1;
    utf8_str -> gx_string_length -= len;

    return GX_SUCCESS;
}

#endif /* GX_UTF8_SUPPORT */

