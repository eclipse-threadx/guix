/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_utility.h"

#include "canonical_normalize_32bpp_resources.h"
#include "canonical_normalize_32bpp_specifications.h"

#if defined(GX_CANONICAL_NORMALIZE_SUPPORT)

extern TX_BYTE_POOL       mem_pool;
extern VOID memory_free(VOID *mem);

GX_CHAR file_name[256] = "..\\NormalizationTest.txt";

ULONG source[255];
GX_UBYTE utf8_source[2014];
ULONG nfc[255];
int source_count;
int utf8_source_count;
int nfc_count;

/* Read test data. */
static int read_line(FILE *file)
{
    char ch;
    int field_index = 0;
    ULONG code = 0;
    INT digit;
    GX_BOOL skip = GX_FALSE;

    source_count = 0;
    nfc_count = 0;

    ch = getc(file);

    while (ch == '\n' || ch == '\r')
    {
        ch = getc(file);
    }

    while ((ch != '\n') && (ch != '\r') && (ch != EOF))
    {
        if ((!skip) && (field_index < 2))
        {
            switch (ch)
            {
            case '#':
            case '@':
                skip = GX_TRUE;
                break;

            case ' ':
            case ';':
                if (field_index == 0)
                {
                    source[source_count++] = code;
                }
                else
                {
                    nfc[nfc_count++] = code;
                }

                code = 0;

                if (ch == ';')
                {
                    field_index++;
                }
                break;
            default:

                digit = (ch - '0');

                if (ch >= 'A' && ch <= 'F')
                {
                    digit = 10 + (ch - 'A');
                }
                else if (ch >= 'a' && ch <= 'f')
                {
                    digit = 10 + (ch - 'a');
                }

                code =  (code << 4) + digit;
                break;
            }
        }

        ch = getc(file);
    }

    if (ch == EOF)
        return(-1);

    return(1);

}

/* Convert unicode format input text to utf8 format text. */
void unicode_to_utf8_str()
{
    INT index;
    UINT glyph_len;

    utf8_source_count = 0;

    for (index = 0; index < source_count; index++)
    {
        _gx_utility_unicode_to_utf8(source[index], utf8_source + utf8_source_count, &glyph_len);
        utf8_source_count += glyph_len;
    }
}

/* Compare result. */
UINT result_compare(GX_CHAR *utf8_str)
{
    INT  index;
    UINT glyph_len;
    GX_CHAR_CODE unicode;

    for (index = 0; index < nfc_count; index++)
    {
        _gx_utility_utf8_string_character_get((GX_CONST CHAR **)&utf8_str, GX_STRLEN(utf8_str), &unicode, &glyph_len);

        if (unicode != nfc[index])
        {
            return GX_FAILURE;
        }
    }

    return GX_SUCCESS;
}

UINT normalization_test()
{
    FILE *file;
    GX_UBYTE *normalized_string;
    UINT normalized_count;
    UINT   result = GX_SUCCESS;
    INT  test_count = 0;
    UINT status;
    ULONG pool_size;

    gx_system_text_render_style_set(GX_TEXT_RENDER_CANONICAL_NORMALIZE);
    file = fopen(file_name, "rb");

    if (!file)
    {
        return GX_FAILURE;
    }

    while (read_line(file) > 0)
    {
        if (source_count == 0 || nfc_count == 0)
        {
            continue;
        }

        if (test_count == 23 || test_count == 24)
        {
            /* Skip hangul sylable normalization. */
            test_count++;
            continue;
        }

        unicode_to_utf8_str();

        pool_size = mem_pool.tx_byte_pool_available;

        normalized_string = GX_NULL;

        status = _gx_utility_canonical_normalize(utf8_source, utf8_source_count, &normalized_string, &normalized_count);

        if (status == GX_NO_CHANGE)
        {
            if (result_compare(utf8_source))
            {
                result = GX_FAILURE;
            }
        }
        else
        {
            if ((status != GX_SUCCESS || result_compare(normalized_string)))
            {
                result = GX_FAILURE;
            }
        }

        if (normalized_string)
        {
            memory_free(normalized_string);
        }

        if (pool_size != mem_pool.tx_byte_pool_available)
        {
            result = GX_FAILURE;
        }

        if (result != GX_SUCCESS)
        {
            break;
        }
        test_count++;
    }

    return result;
}

#endif