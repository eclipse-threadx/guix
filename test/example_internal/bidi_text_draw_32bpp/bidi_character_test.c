/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation
 * Copyright (c) 2026 Eclipse ThreadX contributors
 *
 * This program and the accompanying materials are made available under the
 * terms of the MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 **************************************************************************/
// Portions of this file were generated with AI assistance.

/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "bidi_text_draw_32bpp_resources.h"
#include "bidi_text_draw_32bpp_specifications.h"
#include "gx_utility.h"

#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
GX_CHAR file_name[256] = "..\\bidi_character_test_cases.txt";

static GX_UBYTE input_utf8_text[2048];
static ULONG   input_utf8_count;
static ULONG   input_text[512];
static ULONG   input_count;
static ULONG   output_text[512];
static ULONG   output_count;
static GX_UBYTE base_direction;

extern TX_BYTE_POOL       mem_pool;
extern VOID memory_free(VOID *mem);

/* Read test data. */
static int read_line(FILE *file)
{
    char ch;
    int field_index = 0;
    ULONG code = 0;
    ULONG shift = 12;
    INT digit;

    input_count = 0;
    output_count = 0;

    ch = getc(file);

    while (ch == '\n' || ch == '\r')
    {
        ch = getc(file);
    }

    while ((ch != '\n') && (ch != '\r')&& (ch != EOF))
    {
        switch (ch)
        {
        case ' ':
            if (field_index == 2)
            {
                output_text[output_count++] = input_text[code];
                code = 0;
            }
            break;
        case ';':
            field_index++;
            code = 0;
            break;
        default:

            digit = (ch - '0');

            if (field_index == 0)
            {
                if (ch >= 'A' && ch <= 'F')
                {
                    digit = 10 + (ch - 'A');
                }
                else if (ch >= 'a' && ch <= 'f')
                {
                    digit = 10 + (ch - 'a');
                }

                code += digit << shift;

                if (shift == 0)
                {
                    input_text[input_count++] = code;
                    code = 0;
                    shift = 12;
                }
                else
                {
                    shift -= 4;
                }
            }
            else if (field_index == 1)
            {
                base_direction = digit;
            }
            else
            {
                code = code * 10 + digit;
            }
            break;
        }

        ch = getc(file);
    }

    output_text[output_count++] = input_text[code];

    if (ch == EOF)
        return(-1);

    return(1);

}

/* Convert unicode format input text to utf8 format text. */
void unicode_to_utf8_str()
{
    ULONG index;
    UINT glyph_len;

    input_utf8_count = 0;

    for (index = 0; index < input_count; index++)
    {
        _gx_utility_unicode_to_utf8(input_text[index], input_utf8_text + input_utf8_count, &glyph_len);
        input_utf8_count += glyph_len;
    }
}

/* Arabic shaping is on wherever dynamic bidi text is, so a joining Arabic
   letter comes out as one of its presentation forms. The order is what this
   test checks: accept a presentation form where an Arabic letter is
   expected. */
static GX_BOOL arabic_shaped(ULONG expected, ULONG unicode)
{
    if (((expected >= 0x0621 && expected <= 0x064A) || (expected >= 0x0671 && expected <= 0x06D3)) &&
        ((unicode >= 0xFB50 && unicode <= 0xFDFF) || (unicode >= 0xFE70 && unicode <= 0xFEFF)))
    {
        return GX_TRUE;
    }
    return GX_FALSE;
}

/* Compare result. */
UINT result_compare(GX_CONST GX_STRING *utf8_str)
{
    ULONG index;
    UINT glyph_len;
    GX_CHAR_CODE unicode;
    USHORT mirror;
    GX_STRING string = *utf8_str;

    for (index = 0; index < output_count; index++)
    {
       _gx_utility_utf8_string_character_get(&string, &unicode, &glyph_len);

        if (unicode != output_text[index])
        {
            _gx_utility_bidi_mirroring_get((USHORT)unicode, &mirror);

            if (mirror != (USHORT)output_text[index] &&
                !arabic_shaped(output_text[index], unicode))
            {
                return GX_FAILURE;
            }
        }
    }

    return GX_SUCCESS;
}

UINT bidi_character_test()
{
    FILE *file;
    GX_BIDI_TEXT_INFO text_info;
    GX_BIDI_RESOLVED_TEXT_INFO *resolved_bidi = GX_NULL;
    UINT   result = GX_SUCCESS;
    INT  test_count[3] = {0, 0, 0};
    UINT status;
    ULONG pool_size;

    file = fopen(file_name, "rb");

    if (!file)
    {
        return GX_FAILURE;
    }

    text_info.gx_bidi_text_info_font = GX_NULL;
    text_info.gx_bidi_text_info_display_width = -1;

    /* Each case at its own paragraph direction: 0 left to right, 1 right to
       left, 2 automatic. */
    while (read_line(file) > 0)
    {
        if (base_direction > 2)
        {
            result = GX_FAILURE;
            break;
        }

        unicode_to_utf8_str();

        text_info.gx_bidi_text_info_text.gx_string_ptr = (GX_CONST CHAR *)input_utf8_text;
        text_info.gx_bidi_text_info_text.gx_string_length = input_utf8_count;

        pool_size = mem_pool.tx_byte_pool_available;
        
        if (base_direction == 2)
        {
            status = _gx_utility_bidi_paragraph_reorder(&text_info, &resolved_bidi);
        }
        else
        {
            text_info.gx_bidi_text_info_direction = (base_direction == 0) ? GX_LANGUAGE_DIRECTION_LTR : GX_LANGUAGE_DIRECTION_RTL;
            status = _gx_utility_bidi_paragraph_reorder_ext(&text_info, &resolved_bidi);
        }

        if (status != GX_SUCCESS || result_compare(resolved_bidi->gx_bidi_resolved_text_info_text))
        {
            result = GX_FAILURE;
        }

        if (resolved_bidi)
        {
            _gx_utility_bidi_resolved_text_info_delete(&resolved_bidi);
        }

        if (pool_size != mem_pool.tx_byte_pool_available)
        {
            result = GX_FAILURE;
        }

        if (result != GX_SUCCESS)
        {
            break;
        }
        test_count[base_direction]++;
    }

    if (ferror(file) || (test_count[0] + test_count[1] + test_count[2] != 91699))
    {
        result = GX_FAILURE;
    }
    fclose(file);
#ifdef GX_BIDI_CHARACTER_TEST
    printf("BiDi character cases checked: %d left to right, %d right to left, %d automatic.\n",
           test_count[0], test_count[1], test_count[2]);
#endif
    return result;
}
#endif
