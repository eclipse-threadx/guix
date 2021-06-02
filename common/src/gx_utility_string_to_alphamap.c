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
#include "gx_canvas.h"

#define DRAW_PIXEL          if (data & mask) \
    {                                        \
        *write_data = 0xff;                  \
    }                                        \
    write_data++;                            \
    mask = mask >> 1;

#if defined(GX_RENESAS_DAVE2D_FONT_SUPPORT)
#define DRAW_REVERSED_PIXEL if (data & mask) \
    {                                        \
        *write_data = 0xff;                  \
    }                                        \
    write_data++;                            \
    mask = (GX_UBYTE)(mask << 1);
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_string_to_alphamap                      PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION (Deprecated)                                              */
/*                                                                        */
/*    This function draws text to an 8bpp memory alphamap.                */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text                                  Pointer to string             */
/*    font                                  Font for text drawing         */
/*    textmap                               Pointer to pixemap structure  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_string_length_check       Validate string length        */
/*    _gx_utility_string_to_alphamap_ext    New version of this function  */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Software                                                */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  12-31-2020     Kenneth Maxwell          Modified comment(s), added    */
/*                                            display rotation support,   */
/*                                            resulting in version 6.1.3  */
/*                                                                        */
/**************************************************************************/
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gx_utility_string_to_alphamap(GX_CONST GX_CHAR *text, GX_CONST GX_FONT *font, GX_PIXELMAP *textmap)
{
UINT      status;
UINT      length;
GX_STRING string;

    string.gx_string_ptr = text;
    status = _gx_utility_string_length_check(text, &length, GX_MAX_STRING_LENGTH);
    if (status == GX_SUCCESS)
    {
        string.gx_string_length = length;
        status = _gx_utility_string_to_alphamap_ext(&string, font, textmap);
    }
    return status;
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_string_to_alphamap_ext                  PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION (Deprecated)                                              */
/*                                                                        */
/*    This function draws text to an 8bpp memory alphamap.                */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text                                  Pointer to string             */
/*    font                                  Font for text drawing         */
/*    textmap                               Pointer to pixemap structure  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_string_width_get           Get width of the string in    */
/*                                           pixels                       */
/*    _gx_canvas_text_draw                  Draw glyphs on canvas         */
/*    _gx_utility_string_to_alphamap        Convert string to alpha-map   */
/*    _gx_utiity_pixelmap_rotate            Rotate alphaap to desired     */
/*                                           angle                        */
/*    _gx_canvas_pixelmap_draw              Draw text alphamap            */
/*    _gx_system_memory_free                Free memory used for rotated  */
/*                                           alphamap and canvas          */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Software                                                */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  12-31-2020     Kenneth Maxwell          Modified comment(s), added    */
/*                                            display rotation support,   */
/*                                            resulting in version 6.1.3  */
/*                                                                        */
/**************************************************************************/
UINT _gx_utility_string_to_alphamap_ext(GX_CONST GX_STRING *string, GX_CONST GX_FONT *font, GX_PIXELMAP *textmap)
{
GX_VALUE           alphamap_width;
GX_VALUE           alphamap_height;
UINT               status;
GX_CONST GX_FONT  *font_page = font;
GX_CHAR_CODE       char_val;
GX_CONST GX_GLYPH *glyph;
GX_BOOL            first_glyph = GX_TRUE;
GX_STRING          string_copy;

#ifdef GX_UTF8_SUPPORT
UINT glyph_len;
#endif

    string_copy = *string;
    _gx_system_string_width_get_ext(font, &string_copy, &alphamap_width);

    while (string_copy.gx_string_length)
    {
#ifdef GX_UTF8_SUPPORT
        _gx_utility_utf8_string_character_get(&string_copy, &char_val, &glyph_len);
#else
        char_val = (GX_CHAR_CODE)(*string_copy.gx_string_ptr);
        string_copy.gx_string_ptr++;
        string_copy.gx_string_length--;
#endif /* GX_UTF8_SUPPORT */

        if (!char_val)
        {
            break;
        }

        if (first_glyph || string_copy.gx_string_ptr[0] == GX_NULL)
        {
            while (font_page)
            {
                if (font_page -> gx_font_first_glyph <= char_val &&
                    font_page -> gx_font_last_glyph >= char_val)
                {
                    break;
                }
                font_page = font_page -> gx_font_next_page;
            }

            if (font_page)
            {
                glyph = &font_page -> gx_font_glyphs.gx_font_normal_glyphs[char_val - font_page -> gx_font_first_glyph];

                if (first_glyph)
                {
                    first_glyph = GX_FALSE;

                    if (glyph -> gx_glyph_leading < 0)
                    {
                        alphamap_width = (GX_VALUE)(alphamap_width - glyph -> gx_glyph_leading);
                    }
                }
                else
                {
                    /* Last glyph. */
                    alphamap_width = (GX_VALUE)(alphamap_width + glyph -> gx_glyph_leading);

                    if (glyph -> gx_glyph_width > glyph -> gx_glyph_advance)
                    {
                        alphamap_width = (GX_VALUE)(alphamap_width + glyph -> gx_glyph_width - glyph -> gx_glyph_advance);
                    }
                }
            }
        }
    }

    alphamap_height = font -> gx_font_line_height;

    if (alphamap_width && alphamap_height)
    {
        /* create an alphamap into which to draw the text */
        status = _gx_utility_alphamap_create(alphamap_width, alphamap_height, textmap);
        if (status == GX_SUCCESS)
        {
            /* Draw the text into our temporary canvas */
            _gx_utility_string_to_alphamap_draw(string, font, textmap);
        }
    }
    else
    {
        status = GX_FAILURE;
    }

    return status;
}



/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_glyph_8bpp_to_alphamap_draw             PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that renders 8bpp glyph into alpha-map     */
/*    memory.                                                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    map                                   Pixelmap that the glyph is    */
/*                                            drawn to                    */
/*    xpos                                  X-coord where the glyph is    */
/*                                            drawn to                    */
/*    ypos                                  Y-coord where the glyph is    */
/*                                            darwn to                    */
/*    glyph                                 Pointer to glyph structure    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
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
/*  12-31-2020     Kenneth Maxwell          Modified comment(s), added    */
/*                                            display rotation support,   */
/*                                            resulting in version 6.1.3  */
/*                                                                        */
/**************************************************************************/
VOID _gx_utility_glyph_8bpp_to_alphamap_draw(GX_PIXELMAP *map, INT xpos, INT ypos, GX_CONST GX_GLYPH *glyph)
{
GX_UBYTE *read_data;
GX_UBYTE *read_row;
GX_UBYTE *write_data;
GX_UBYTE *write_row;
UINT      row;
UINT      col;
UINT      pixel_width = 0;
UINT      y_height;
USHORT    write_stride;

    if (map -> gx_pixelmap_flags & (GX_PIXELMAP_ROTATED_90 | GX_PIXELMAP_ROTATED_270))
    {
        write_stride = (USHORT)map -> gx_pixelmap_height;
        pixel_width = glyph -> gx_glyph_height;
        y_height = glyph -> gx_glyph_width;
        GX_SWAP_VALS(xpos, ypos)

        if (map -> gx_pixelmap_flags & GX_PIXELMAP_ROTATED_90)
        {
            ypos = (map -> gx_pixelmap_width - ypos - glyph -> gx_glyph_width);
        }
        else
        {
            xpos = (map -> gx_pixelmap_height - xpos - glyph -> gx_glyph_height);
        }
    }
    else
    {
        write_stride = (USHORT)map -> gx_pixelmap_width;
        pixel_width = glyph -> gx_glyph_width;
        y_height = glyph -> gx_glyph_height;
    }

    read_row = (GX_UBYTE *)glyph -> gx_glyph_map;

    write_row = (GX_UBYTE *)map -> gx_pixelmap_data;
    write_row += ypos * write_stride;
    write_row += xpos;

    for (row = 0; row < y_height; row++)
    {
        read_data = read_row;
        write_data = write_row;

        for (col = 0; col < pixel_width; col++)
        {
            *write_data++ = *read_data++;
        }
        read_row +=  pixel_width;
        write_row += write_stride;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_glyph_4bpp_to_alphamap_draw             PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that renders 4bpp glyph into alpha-map     */
/*    memory.                                                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    map                                   Pixelmap that the glyph is    */
/*                                            drawn to                    */
/*    xpos                                  X-coord where the glyph is    */
/*                                            drawn to                    */
/*    ypos                                  Y-coord where the glyph is    */
/*                                            darwn to                    */
/*    glyph                                 Pointer to glyph structure    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
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
/*  12-31-2020     Kenneth Maxwell          Modified comment(s), added    */
/*                                            display rotation support,   */
/*                                            resulting in version 6.1.3  */
/*                                                                        */
/**************************************************************************/
VOID _gx_utility_glyph_4bpp_to_alphamap_draw(GX_PIXELMAP *map, INT xpos, INT ypos, GX_CONST GX_GLYPH *glyph)
{
GX_UBYTE *read_data;
GX_UBYTE *read_row;
GX_UBYTE *write_data;
GX_UBYTE *write_row;
UINT      row;
UINT      col;
UINT      pixel_width = 0;
UINT      read_stride;
USHORT    write_stride;
UINT      y_height;
GX_UBYTE  data;

    if (map -> gx_pixelmap_flags & (GX_PIXELMAP_ROTATED_90 | GX_PIXELMAP_ROTATED_270))
    {
        write_stride = (USHORT)map -> gx_pixelmap_height;
        pixel_width = glyph -> gx_glyph_height;
        y_height = glyph -> gx_glyph_width;

        GX_SWAP_VALS(xpos, ypos)

        if (map -> gx_pixelmap_flags & GX_PIXELMAP_ROTATED_90)
        {
            ypos = (INT)map -> gx_pixelmap_width - (INT)ypos - (INT)y_height;
        }
        else
        {
            xpos = (INT)map -> gx_pixelmap_height - (INT)xpos - (INT)pixel_width;
        }
    }
    else
    {
        pixel_width = (USHORT)glyph -> gx_glyph_width;
        y_height = glyph -> gx_glyph_height;
        write_stride = (USHORT)map -> gx_pixelmap_width;
    }

    read_row = (GX_UBYTE *)glyph -> gx_glyph_map;
    write_row = (GX_UBYTE *)map -> gx_pixelmap_data;
    write_row += ypos * write_stride;
    write_row += xpos;

    read_stride = (pixel_width + 1) / 2;

    for (row = 0; row < y_height; row++)
    {
        read_data = read_row;
        write_data = write_row;

        for (col = 0; col < pixel_width; col++)
        {
            data = *read_data++;

            *write_data++ = (GX_UBYTE)((data & 0xf0) | (data >> 4));
            col++;

            if (col < pixel_width)
            {
                *write_data++ = (GX_UBYTE)((data << 4) | (data & 0x0f));
            }
        }
        read_row += read_stride;
        write_row += write_stride;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_glyph_reversed_4bpp_to_alphamap_draw    PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that renders 4bpp reversed bit order glyph */
/*    into alpha-map memory.                                              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    map                                   Pixelmap that the glyph is    */
/*                                            drawn to                    */
/*    xpos                                  X-coord where the glyph is    */
/*                                            drawn to                    */
/*    ypos                                  Y-coord where the glyph is    */
/*                                            darwn to                    */
/*    glyph                                 Pointer to glyph structure    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
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
/*  12-31-2020     Kenneth Maxwell          Modified comment(s), added    */
/*                                            display rotation support,   */
/*                                            resulting in version 6.1.3  */
/*  06-02-2021     Kenneth Maxwell          Modified comment(s), added    */
/*                                            rename RENESAS_DAVE2D       */
/*                                            support conditional,        */
/*                                            resulting in version 6.1.7  */
/*                                                                        */
/**************************************************************************/
#if defined(GX_RENESAS_DAVE2D_FONT_SUPPORT)
VOID _gx_utility_glyph_reversed_4bpp_to_alphamap_draw(GX_PIXELMAP *map, INT xpos, INT ypos, GX_CONST GX_GLYPH *glyph)
{
GX_UBYTE *read_data;
GX_UBYTE *read_row;
GX_UBYTE *write_data;
GX_UBYTE *write_row;
UINT      row;
UINT      col;
UINT      pixel_width = 0;
UINT      byte_width;
UINT      y_height;
USHORT    write_stride;
GX_UBYTE  data;


    if (map -> gx_pixelmap_flags & (GX_PIXELMAP_ROTATED_90 | GX_PIXELMAP_ROTATED_270))
    {
        write_stride = (USHORT)map -> gx_pixelmap_height;
        pixel_width = glyph -> gx_glyph_height;
        y_height = glyph -> gx_glyph_width;

        GX_SWAP_VALS(xpos, ypos)

        if (map -> gx_pixelmap_flags & GX_PIXELMAP_ROTATED_90)
        {
            ypos = (INT)map -> gx_pixelmap_width - (INT)ypos - (INT)y_height;
        }
        else
        {
            xpos = (INT)map -> gx_pixelmap_height - (INT)xpos - (INT)pixel_width;
        }
    }
    else
    {
        pixel_width = glyph -> gx_glyph_width;
        y_height = glyph -> gx_glyph_height;
        write_stride = (USHORT)map -> gx_pixelmap_width;
    }

    read_row = (GX_UBYTE *)glyph -> gx_glyph_map;
    write_row = (GX_UBYTE *)map -> gx_pixelmap_data;
    write_row += ypos * write_stride;
    write_row += xpos;

    byte_width = (pixel_width + 1) / 2;


    for (row = 0; row < y_height; row++)
    {
        read_data = read_row;
        write_data = write_row;

        for (col = 0; col < pixel_width; col++)
        {
            data = *read_data++;

            *write_data++ = (GX_UBYTE)((data << 4) | (data & 0x0f));
            col++;

            if (col < pixel_width)
            {
                *write_data++ = (GX_UBYTE)((data & 0xf0) | (data >> 4));
            }
        }
        read_row += byte_width;
        write_row += write_stride;
    }
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_glyph_1bpp_to_alphamap_draw             PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that renders 1bpp glyph into alpha-map     */
/*    memory.                                                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    map                                   Pixelmap that the glyph is    */
/*                                            drawn to                    */
/*    xpos                                  X-coord where the glyph is    */
/*                                            drawn to                    */
/*    ypos                                  Y-coord where the glyph is    */
/*                                            darwn to                    */
/*    glyph                                 Pointer to glyph structure    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
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
/*  12-31-2020     Kenneth Maxwell          Modified comment(s), added    */
/*                                            display rotation support,   */
/*                                            resulting in version 6.1.3  */
/*                                                                        */
/**************************************************************************/
VOID _gx_utility_glyph_1bpp_to_alphamap_draw(GX_PIXELMAP *map, INT xpos, INT ypos, GX_CONST GX_GLYPH *glyph)
{
GX_UBYTE *read_data;
GX_UBYTE *read_row;
GX_UBYTE *write_data;
GX_UBYTE *write_row;
UINT      row;
UINT      col;
UINT      y_height;
GX_UBYTE  glyph_width;
GX_UBYTE  data;
UINT      pixel_in_first_byte = 8;
UINT      pixel_in_last_byte;
UINT      num_bits;
UINT      num_bytes;
USHORT    write_stride;
GX_UBYTE  mask;

    if (map -> gx_pixelmap_flags & (GX_PIXELMAP_ROTATED_90 | GX_PIXELMAP_ROTATED_270))
    {
        write_stride = (USHORT)map -> gx_pixelmap_height;
        y_height = glyph -> gx_glyph_width;
        glyph_width = (USHORT)glyph -> gx_glyph_height;
        GX_SWAP_VALS(xpos, ypos)

        if (map -> gx_pixelmap_flags & GX_PIXELMAP_ROTATED_90)
        {
            ypos = (map -> gx_pixelmap_width - ypos - glyph -> gx_glyph_width);
        }
        else
        {
            xpos = (map -> gx_pixelmap_height - xpos - glyph -> gx_glyph_height);
        }
    }
    else
    {
        write_stride = (USHORT)map -> gx_pixelmap_width;
        y_height = glyph -> gx_glyph_height;
        glyph_width = glyph -> gx_glyph_width;
    }

    pixel_in_last_byte = ((UINT)glyph_width) & 0x7;
    if (pixel_in_last_byte == 0)
    {
        pixel_in_last_byte = 8;
    }

    num_bytes = (((UINT)glyph_width) + 7) >> 3;

    if (num_bytes == 1)
    {
        pixel_in_first_byte = pixel_in_last_byte;
    }

    read_row = (GX_UBYTE *)glyph -> gx_glyph_map;

    write_row = (GX_UBYTE *)map -> gx_pixelmap_data;
    write_row += ypos * write_stride;
    write_row += xpos;

    for (row = 0; row < y_height; row++)
    {
        read_data = read_row;
        write_data = write_row;
        num_bits = pixel_in_first_byte;

        for (col = 0; col < num_bytes; col++)
        {
            data = *read_data++;
            mask = 0x80;

            if ((col == (num_bytes - 1)) && (num_bytes > 1))
            {
                num_bits = pixel_in_last_byte;
            }

            switch (num_bits)
            {
            case 8:
                DRAW_PIXEL;
                /* fallthrough */
            case 7:
                DRAW_PIXEL;
                /* fallthrough */
            case 6:
                DRAW_PIXEL;
                /* fallthrough */
            case 5:
                DRAW_PIXEL;
                /* fallthrough */
            case 4:
                DRAW_PIXEL;
                /* fallthrough */
            case 3:
                DRAW_PIXEL;
                /* fallthrough */
            case 2:
                DRAW_PIXEL;
                /* fallthrough */
            default:
                if (data & mask)
                {
                    *write_data = 0xff;
                }
                write_data++;
                break;
            }
        }

        read_row += num_bytes;
        write_row += write_stride;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_glyph_reversed_1bpp_to_alphamap_draw    PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that renders 1bpp reveresed bit order      */
/*    glyph into alpha-map memory.                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    map                                   Pixelmap that the glyph is    */
/*                                            drawn to                    */
/*    xpos                                  X-coord where the glyph is    */
/*                                            drawn to                    */
/*    ypos                                  Y-coord where the glyph is    */
/*                                            darwn to                    */
/*    glyph                                 Pointer to glyph structure    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
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
/*  12-31-2020     Kenneth Maxwell          Modified comment(s), added    */
/*                                            display rotation support,   */
/*                                            resulting in version 6.1.3  */
/*  06-02-2021     Kenneth Maxwell          Modified comment(s),          */
/*                                            rename RENESAS_DAVE2D       */
/*                                            support conditional,        */
/*                                            resulting in version 6.1.7  */
/*                                                                        */
/**************************************************************************/
#if defined(GX_RENESAS_DAVE2D_FONT_SUPPORT)
VOID _gx_utility_glyph_reversed_1bpp_to_alphamap_draw(GX_PIXELMAP *map, INT xpos, INT ypos, GX_CONST GX_GLYPH *glyph)
{
GX_UBYTE *read_data;
GX_UBYTE *read_row;
GX_UBYTE *write_data;
GX_UBYTE *write_row;
UINT      row;
UINT      col;
UINT      y_height;
GX_UBYTE  glyph_width;
GX_UBYTE  data;
UINT      pixel_in_first_byte = 8;
UINT      pixel_in_last_byte;
UINT      num_bits;
UINT      num_bytes;
USHORT    write_stride;
GX_UBYTE  mask;

    if (map -> gx_pixelmap_flags & (GX_PIXELMAP_ROTATED_90 | GX_PIXELMAP_ROTATED_270))
    {
        write_stride = (USHORT)map -> gx_pixelmap_height;
        y_height = glyph -> gx_glyph_width;
        glyph_width = glyph -> gx_glyph_height;
        GX_SWAP_VALS(xpos, ypos)

        if (map -> gx_pixelmap_flags & GX_PIXELMAP_ROTATED_90)
        {
            ypos = (map -> gx_pixelmap_width - ypos - glyph -> gx_glyph_width);
        }
        else
        {
            xpos = (map -> gx_pixelmap_height - xpos - glyph -> gx_glyph_height);
        }
    }
    else
    {
        write_stride = (USHORT)map -> gx_pixelmap_width;
        y_height = glyph -> gx_glyph_height;
        glyph_width = glyph -> gx_glyph_width;
    }

    pixel_in_last_byte = ((UINT)glyph_width) & 0x7;
    if (pixel_in_last_byte == 0)
    {
        pixel_in_last_byte = 8;
    }

    num_bytes = (((UINT)glyph_width) + 7) >> 3;

    if (num_bytes == 1)
    {
        pixel_in_first_byte = pixel_in_last_byte;
    }

    read_row = (GX_UBYTE *)glyph -> gx_glyph_map;

    write_row = (GX_UBYTE *)map -> gx_pixelmap_data;
    write_row += ypos * write_stride;
    write_row += xpos;

    for (row = 0; row < y_height; row++)
    {
        read_data = read_row;
        write_data = write_row;
        num_bits = pixel_in_first_byte;

        for (col = 0; col < num_bytes; col++)
        {
            data = *read_data++;
            mask = 0x01;

            if ((col == (num_bytes - 1)) && (num_bytes > 1))
            {
                num_bits = pixel_in_last_byte;
            }

            switch (num_bits)
            {
            case 8:
                DRAW_REVERSED_PIXEL;
                /* fallthrough */
            case 7:
                DRAW_REVERSED_PIXEL;
                /* fallthrough */
            case 6:
                DRAW_REVERSED_PIXEL;
                /* fallthrough */
            case 5:
                DRAW_REVERSED_PIXEL;
                /* fallthrough */
            case 4:
                DRAW_REVERSED_PIXEL;
                /* fallthrough */
            case 3:
                DRAW_REVERSED_PIXEL;
                /* fallthrough */
            case 2:
                DRAW_REVERSED_PIXEL;
                /* fallthrough */
            default:
                if (data & mask)
                {
                    *write_data = 0xff;
                }
                write_data++;
                break;
            }
        }

        read_row += num_bytes;
        write_row += write_stride;
    }
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_string_to_alphamap_draw                 PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that renders entire string to alpha-map    */
/*    memory.                                                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text                                  Pointer to string             */
/*    font                                  Font for text drawing         */
/*    map                                   Pointer to pixemap structure  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_glyph_8bpp_to_alphamap_draw                             */
/*                                          Render 8bpp glyph to alphamap */
/*    _gx_utility_glyph_4bpp_to_alphamap_draw                             */
/*                                          Render 4bpp glyph to alphamap */
/*    _gx_utility_glyph_1bpp_to_alphamap_draw                             */
/*                                          Render 1bpp glyph to alphamap */
/*    _gx_utility_utf8_string_character_get Parse utf8 string to          */
/*                                            to multibyte glyph          */
/*    _gx_utility_string_length_check       Test string length            */
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
/*  12-31-2020     Kenneth Maxwell          Modified comment(s), added    */
/*                                            display rotation support,   */
/*                                            resulting in version 6.1.3  */
/*  06-02-2021     Kenneth Maxwell          Modified comment(s),          */
/*                                            rename RENESAS_DAVE2D       */
/*                                            support conditional,        */
/*                                            resulting in version 6.1.7  */
/*                                                                        */
/**************************************************************************/
VOID _gx_utility_string_to_alphamap_draw(GX_CONST GX_STRING *string, GX_CONST GX_FONT *font, GX_PIXELMAP *map)
{
INT                xpos;
int                y_offset;
GX_CONST GX_GLYPH *glyph;
GX_CONST GX_FONT  *font_page;
GX_CHAR_CODE       glyph_index;
GX_CHAR_CODE       char_val;
GX_BOOL            first_char = GX_TRUE;
INT                leading;
GX_STRING          string_copy;
VOID               (*glyph_draw)(GX_PIXELMAP *map, INT xpos, INT ypos, GX_CONST GX_GLYPH *glyph);

#ifdef GX_UTF8_SUPPORT
UINT glyph_len;
#endif /* GX_UTF8_SUPPORT */

#if defined(GX_RENESAS_DAVE2D_FONT_SUPPORT)
    if (font -> gx_font_format & GX_FONT_FORMAT_COMPRESSED)
    {
        /* Not supported. */
        return;
    }
#endif

    xpos = 0;
    string_copy = *string;

    if (font -> gx_font_format & GX_FONT_FORMAT_ROTATED_90)
    {
        map -> gx_pixelmap_flags |= GX_PIXELMAP_ROTATED_90;
    }
    else if (font -> gx_font_format & GX_FONT_FORMAT_ROTATED_270)
    {
        map -> gx_pixelmap_flags |= GX_PIXELMAP_ROTATED_270;
    }

    switch (font -> gx_font_format & GX_FONT_FORMAT_BPP_MASK)
    {
    case GX_FONT_FORMAT_8BPP:
        glyph_draw = _gx_utility_glyph_8bpp_to_alphamap_draw;
        break;

    case GX_FONT_FORMAT_4BPP:
#if defined(GX_RENESAS_DAVE2D_FONT_SUPPORT)
        if (font -> gx_font_format & GX_FONT_FORMAT_REVERSED_ORDER)
        {
            glyph_draw = _gx_utility_glyph_reversed_4bpp_to_alphamap_draw;
        }
        else
        {
#endif
            glyph_draw = _gx_utility_glyph_4bpp_to_alphamap_draw;
#if defined(GX_RENESAS_DAVE2D_FONT_SUPPORT)
        }
#endif
        break;

    case GX_FONT_FORMAT_1BPP:
#if defined(GX_RENESAS_DAVE2D_FONT_SUPPORT)
        if (font -> gx_font_format & GX_FONT_FORMAT_REVERSED_ORDER)
        {
            glyph_draw = _gx_utility_glyph_reversed_1bpp_to_alphamap_draw;
        }
        else
        {
#endif
            glyph_draw = _gx_utility_glyph_1bpp_to_alphamap_draw;
#if defined(GX_RENESAS_DAVE2D_FONT_SUPPORT)
        }
#endif
        break;

    default:
        glyph_draw = GX_NULL;
        break;
    }

    if (glyph_draw == GX_NULL)
    {
        return;
    }

    while (string_copy.gx_string_length)
    {
#ifdef GX_UTF8_SUPPORT
        _gx_utility_utf8_string_character_get(&string_copy, &char_val, &glyph_len);
#else
        char_val = (GX_CHAR_CODE)(*string_copy.gx_string_ptr);
        string_copy.gx_string_ptr++;
        string_copy.gx_string_length--;
#endif /* GX_UTF8_SUPPORT */

        if (!char_val)
        {
            break;
        }

        font_page = font;

        while (font_page)
        {
            if (font_page -> gx_font_first_glyph <= char_val &&
                font_page -> gx_font_last_glyph >= char_val)
            {
                break;
            }
            font_page = font_page -> gx_font_next_page;
        }
        if (font_page)
        {
            glyph_index = (GX_CHAR_CODE)(char_val - font_page -> gx_font_first_glyph);
            glyph = &font_page -> gx_font_glyphs.gx_font_normal_glyphs[glyph_index];
            y_offset = font_page -> gx_font_baseline - glyph -> gx_glyph_ascent;

            leading = glyph -> gx_glyph_leading;
            if (first_char)
            {
                first_char = GX_FALSE;

                leading = 0;
            }
            glyph_draw(map, xpos + leading, y_offset, glyph);
            xpos += glyph -> gx_glyph_advance;
        }
    }
}

