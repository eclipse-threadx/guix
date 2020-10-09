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
/**   System Management (System)                                          */
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
/*    _gx_system_string_width_compressed_font_get         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service gets the width (in pixels) of the supplied string,     */
/*    for a given compressed font.                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    font                                  Pointer to font of strings    */
/*    string                                Pointer to string             */
/*    string_length                         Raw length of string, in bytes*/
/*    return_width                          Destination for width of      */
/*                                            string                      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_utf8_string_character_get                               */
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
static UINT  _gx_system_string_width_compressed_font_get(GX_CONST GX_FONT *font,
                                                         GX_STRING *string,
                                                         GX_VALUE *return_width)
{
INT                           width = 0;
GX_CHAR_CODE                  char_val;
GX_CONST GX_COMPRESSED_GLYPH *glyph;
GX_CONST GX_FONT             *font_link;
GX_STRING                     string_copy = *string;

#ifdef GX_UTF8_SUPPORT
UINT ret;
#endif /* GX_UTF8_SUPPORT */

    /* Process complete string.  */
    while (string_copy.gx_string_length > 0)
    {
#ifdef GX_UTF8_SUPPORT
        ret = _gx_utility_utf8_string_character_get(&string_copy, &char_val, GX_NULL);

        if ((ret != GX_SUCCESS) || (char_val == 0))
#else
        char_val = (GX_CHAR_CODE)(*string_copy.gx_string_ptr);
        string_copy.gx_string_ptr++;
        string_copy.gx_string_length--;

        if (char_val == 0)
#endif /* GX_UTF8_SUPPORT */
        {
            break;
        }

        font_link = font;
        while (font_link)
        {
            if (char_val >= font_link -> gx_font_first_glyph &&
                char_val <= font_link -> gx_font_last_glyph)
            {
                break;
            }
            font_link = font_link -> gx_font_next_page;
        }
        if (font_link)
        {
            char_val = (GX_CHAR_CODE)(char_val - font_link -> gx_font_first_glyph);
            glyph = &font_link -> gx_font_glyphs.gx_font_compressed_glyphs[char_val];

            width += glyph -> gx_glyph_advance;
        }
    }

    /* Setup return width.  */
    *return_width =  (GX_VALUE)width;

    /* Return successful completion.  */
    return(GX_SUCCESS);
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_system_string_width_kerning_font_get            PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service gets the width (in pixels) of the supplied string,     */
/*    for a given compressed font.                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    font                                  Pointer to font of strings    */
/*    string                                Pointer to string             */
/*    string_length                         Raw length of string, in bytes*/
/*    return_width                          Destination for width of      */
/*                                            string                      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_utf8_string_character_get                               */
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
#if defined(GX_FONT_KERNING_SUPPORT)
static UINT  _gx_system_string_width_kerning_font_get(GX_CONST GX_FONT *font,
                                                      GX_CONST GX_STRING *string,
                                                      GX_VALUE *return_width)
{
INT                        width = 0;
GX_CHAR_CODE               char_val;
GX_CONST GX_KERNING_GLYPH *glyph;
GX_CONST GX_FONT          *font_link;
GX_STRING                  string_copy;

/* Used for kerning glyph. */
GX_CHAR_CODE       pre_char_val = 0;
GX_CHAR            kerning_offset = 0;
GX_CONST GX_UBYTE *kerning_table;
INT                kerning_counts;
INT                index;
GX_UBYTE          *left_glyph_ptr;

#ifdef GX_UTF8_SUPPORT
UINT ret;
#endif /* GX_UTF8_SUPPORT */

    string_copy = *string;

    /* Process complete string.  */
    while (string_copy.gx_string_length > 0)
    {
#ifdef GX_UTF8_SUPPORT
        ret = _gx_utility_utf8_string_character_get(&string_copy, &char_val, GX_NULL);
        if ((ret != GX_SUCCESS) || (char_val == 0))
#else
        char_val = (GX_CHAR_CODE)(*string_copy.gx_string_ptr++);
        string_copy.gx_string_length--;
        if (char_val == 0)
#endif /* GX_UTF8_SUPPORT */
        {
            break;
        }

        font_link = font;
        while (font_link)
        {
            if (char_val >= font_link -> gx_font_first_glyph &&
                char_val <= font_link -> gx_font_last_glyph)
            {
                break;
            }
            font_link = font_link -> gx_font_next_page;
        }
        if (font_link)
        {
            char_val = (GX_CHAR_CODE)(char_val - font_link -> gx_font_first_glyph);

            glyph = &font_link -> gx_font_glyphs.gx_font_kerning_glyphs[char_val];
            kerning_table = glyph -> gx_kerning_table;

            if (kerning_table && (pre_char_val != 0))
            {
                /* Search the kerning table for the kerning value. */
                kerning_counts = *kerning_table;
                left_glyph_ptr = (GX_UBYTE *)(kerning_table + 1);

                for (index = 0; index < kerning_counts; index++)
                {
                    if ((*left_glyph_ptr) == (pre_char_val + font_link -> gx_font_first_glyph))
                    {
                        kerning_offset = (GX_CHAR)(*(left_glyph_ptr + 1));
                        break;
                    }
                    left_glyph_ptr += 2;
                }
            }

            width += glyph -> gx_glyph_advance;
            width += kerning_offset;
        }
        /* Store previous character value. */
        pre_char_val = char_val;
        kerning_offset = 0;
    }

    /* Setup return width.  */
    *return_width =  (GX_VALUE)width;

    /* Return successful completion.  */
    return(GX_SUCCESS);
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_system_string_width_get                         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service gets the width (in pixels) of the supplied string,     */
/*    for a given font.                                                   */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    font                                  Pointer to font of strings    */
/*    string                                Pointer to string             */
/*    string_length                         Raw length of string, in bytes*/
/*    return_width                          Destination for width of      */
/*                                            string                      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_utf8_string_character_get                               */
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
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT  _gx_system_string_width_get(GX_CONST GX_FONT *font, GX_CONST GX_CHAR *text, INT string_length, GX_VALUE *return_width)
{
UINT      status = GX_SUCCESS;
UINT      length = 0;
GX_STRING string;

    string.gx_string_ptr = text;
    if (string_length >= 0)
    {
        string.gx_string_length = (UINT)string_length;
    }
    else
    {
        status = _gx_utility_string_length_check(text, &length, GX_MAX_STRING_LENGTH);
        if (status == GX_SUCCESS)
        {
            string.gx_string_length = length;
        }
    }
    if (status == GX_SUCCESS)
    {
        status = _gx_system_string_width_get_ext(font, &string, return_width);
    }
    return status;
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_system_string_width_get_ext                     PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service gets the width (in pixels) of the supplied string,     */
/*    for a given font.                                                   */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    font                                  Pointer to font of strings    */
/*    string                                Pointer to string             */
/*    string_length                         Raw length of string, in bytes*/
/*    return_width                          Destination for width of      */
/*                                            string                      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_utf8_string_character_get                               */
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
UINT  _gx_system_string_width_get_ext(GX_CONST GX_FONT *font, GX_CONST GX_STRING *string, GX_VALUE *return_width)
{

INT                width = 0;
GX_CHAR_CODE       char_val;
GX_CONST GX_GLYPH *glyph;
GX_CONST GX_FONT  *font_link;
GX_STRING          string_copy = *string;

#ifdef GX_UTF8_SUPPORT
UINT ret;
#endif /* GX_UTF8_SUPPORT */

    /* Check for NULL pointers.  */
    if (!font || !string_copy.gx_string_ptr)
    {
        /* Yes, one of the input pointers are NULL.  */
        return(GX_PTR_ERROR);
    }

    if (font -> gx_font_format & GX_FONT_FORMAT_COMPRESSED)
    {
        return _gx_system_string_width_compressed_font_get(font, &string_copy, return_width);
    }
#if defined(GX_FONT_KERNING_SUPPORT)
    else
    {
        if (font -> gx_font_format & GX_FONT_FORMAT_KERNING)
        {
            return _gx_system_string_width_kerning_font_get(font, &string_copy, return_width);
        }
    }
#endif

    /* Process complete string.  */
    while (string_copy.gx_string_length > 0)
    {
        /* Pickup first character of the string.  */
#ifdef GX_UTF8_SUPPORT
        ret = _gx_utility_utf8_string_character_get(&string_copy, &char_val, GX_NULL);

        if ((ret != GX_SUCCESS) || (char_val == 0))
#else
        char_val = (GX_CHAR_CODE)(*string_copy.gx_string_ptr);
        string_copy.gx_string_ptr++;
        string_copy.gx_string_length--;

        if (char_val == 0)
#endif /* GX_UTF8_SUPPORT */
        {
            break;
        }

        font_link = font;
        while (font_link)
        {
            if (char_val >= font_link -> gx_font_first_glyph &&
                char_val <= font_link -> gx_font_last_glyph)
            {
                break;
            }
            font_link = font_link -> gx_font_next_page;
        }
        if (font_link)
        {
            char_val = (GX_CHAR_CODE)(char_val - font_link -> gx_font_first_glyph);
            glyph = &font_link -> gx_font_glyphs.gx_font_normal_glyphs[char_val];
            width += glyph -> gx_glyph_advance;
        }
    }

    /* Setup return width.  */
    if (width > 0x7fff)
    {
        width = 0x7fff;
    }
    *return_width =  (GX_VALUE)width;

    /* Return successful completion.  */
    return(GX_SUCCESS);
}

