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
/**   Rich Text View Management (Rich Text View)                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_rich_text_view.h"
#include "gx_utility.h"
#include "gx_system.h"

/* Define rich text end tags. */
static GX_CONST GX_CHAR close_tag_bold[] = "</b>";
static GX_CONST GX_CHAR close_tag_italic[] = "</i>";
static GX_CONST GX_CHAR close_tag_underline[] = "</u>";
static GX_CONST GX_CHAR close_tag_font[] = "</f>";
static GX_CONST GX_CHAR close_tag_color[] = "</c>";
static GX_CONST GX_CHAR close_tag_hicolor[] = "</hc>";
static GX_CONST GX_CHAR close_tag_align[] = "</align>";

/* Define end tag string list. */
static GX_CONST GX_STRING gx_rich_text_end_tag_list[] =
{
    {close_tag_bold, sizeof(close_tag_bold) - 1},
    {close_tag_italic, sizeof(close_tag_italic) - 1},
    {close_tag_underline, sizeof(close_tag_underline) - 1},
    {close_tag_font, sizeof(close_tag_font) - 1},
    {close_tag_color, sizeof(close_tag_color) - 1},
    {close_tag_hicolor, sizeof(close_tag_hicolor) - 1},
    {close_tag_align, sizeof(close_tag_align) - 1}
};

/* Define tag index to access tag list. */
enum gx_rich_text_tag_index
{
    GX_RICH_TEXT_TAG_INDEX_BOLD = 0,
    GX_RICH_TEXT_TAG_INDEX_ITALIC,
    GX_RICH_TEXT_TAG_INDEX_UNDERLINE,
    GX_RICH_TEXT_TAG_INDEX_FONT,
    GX_RICH_TEXT_TAG_INDEX_COLOR,
    GX_RICH_TEXT_TAG_INDEX_HICOLOR,
    GX_RICH_TEXT_TAG_INDEX_ALIGN
};

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_rich_text_resource_id_read                      PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to read resource id.                       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text                                  Pointer to rich text          */
/*    start_index                           Start index of the resource   */
/*                                            text                        */
/*    length                                Length of the input string    */
/*    resource_id                           Retrieved resource id         */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                                              */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_rich_text_view_tag_open                                         */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  09-30-2020     Kenneth Maxwell          Initial Version 6.1           */
/*                                                                        */
/**************************************************************************/
static UINT _gx_rich_text_resource_id_read(GX_STRING *text, UINT *resource_id)
{
UINT    status = GX_FAILURE;
UINT    id = 0;
CHAR    ch;

    if (text -> gx_string_length < 2)
    {
        return status;
    }

    ch = text -> gx_string_ptr[0];

    if ((ch > '9') || (ch < '0'))
    {
        /* Invalid id. */
        return status;
    }

    while (text -> gx_string_length > 0)
    {
        ch = text -> gx_string_ptr[0];

        text -> gx_string_ptr++;
        text -> gx_string_length--;

        if ((ch <= '9') && (ch >= '0'))
        {
            /* Read id value. */
            id = id * 10 + (UINT)(ch - '0');
        }
        else
        {
            if (ch == '>')
            {
                status = GX_SUCCESS;
            }
            break;
        }
    }

    if (status == GX_SUCCESS)
    {
        *resource_id = id;
    }

    return status;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_rich_text_view_tag_open                         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function detects rich text open tag from specified text        */
/*    position and updated rich text draw style accordion to tag type.    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    view                                  Rich text view control block  */
/*    text                                  Text for processing           */
/*    format                                Current rich text format      */
/*    handled_bytes                         Bytes been processed          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                                              */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_string_compare            Test if two strings equal     */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_rich_text_view_tag_enter                                        */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  09-30-2020     Kenneth Maxwell          Initial Version 6.1           */
/*                                                                        */
/**************************************************************************/
static UINT _gx_rich_text_view_tag_open(GX_RICH_TEXT_VIEW *view, GX_CONST GX_STRING *text, GX_RICH_TEXT_FORMAT *format, GX_UBYTE *handled_bytes)
{
UINT                 status = GX_FAILURE;
GX_STRING            string;
GX_STRING            tag_string;
GX_UBYTE             alignment;
UINT                 resource_id = 0;
GX_RICH_TEXT_CONTEXT context;
UINT                 tag_index = 0;

    string = *text;

    context.gx_rich_text_context_format = *format;

    switch (string.gx_string_ptr[1])
    {
    case 'b':
        /* <b>: bold */
        if (string.gx_string_ptr[2] == '>')
        {
            string.gx_string_ptr += 3;
            string.gx_string_length -= 3;

            format -> gx_rich_text_flags |= GX_RICH_TEXT_BOLD;
            if (format -> gx_rich_text_flags & GX_RICH_TEXT_ITALIC)
            {
                format -> gx_rich_text_font_id = view -> gx_rich_text_view_fonts.gx_rich_text_fonts_bold_italic_id;
            }
            else
            {
                format -> gx_rich_text_font_id = view -> gx_rich_text_view_fonts.gx_rich_text_fonts_bold_id;
            }
            tag_index = GX_RICH_TEXT_TAG_INDEX_BOLD;
            status = GX_SUCCESS;
        }
        break;

    case 'i':
        /* <i>: italic */
        if (string.gx_string_ptr[2] == '>')
        {
            string.gx_string_ptr += 3;
            string.gx_string_length -= 3;

            format -> gx_rich_text_flags |= GX_RICH_TEXT_ITALIC;
            if (format -> gx_rich_text_flags & GX_RICH_TEXT_BOLD)
            {
                format -> gx_rich_text_font_id = view -> gx_rich_text_view_fonts.gx_rich_text_fonts_bold_italic_id;
            }
            else
            {
                format -> gx_rich_text_font_id = view -> gx_rich_text_view_fonts.gx_rich_text_fonts_italic_id;
            }
            tag_index = GX_RICH_TEXT_TAG_INDEX_ITALIC;
            status = GX_SUCCESS;
        }
        break;

    case 'u':
        /* <u>: underline*/
        if (string.gx_string_ptr[2] == '>')
        {
            string.gx_string_ptr += 3;
            string.gx_string_length -= 3;

            format -> gx_rich_text_flags |= GX_RICH_TEXT_UNDERLINE;

            tag_index = GX_RICH_TEXT_TAG_INDEX_UNDERLINE;
            status = GX_SUCCESS;
        }
        break;

    case 'f':
        /* <f font-id>: select font */
        if (string.gx_string_ptr[2] == ' ')
        {
            string.gx_string_ptr += 3;
            string.gx_string_length -= 3;

            /* font id should <= 0xff */
            status = _gx_rich_text_resource_id_read(&string, &resource_id);

            if (status != GX_SUCCESS)
            {
                return status;
            }

            tag_index = GX_RICH_TEXT_TAG_INDEX_FONT;
            format -> gx_rich_text_font_id = resource_id;
        }
        break;

    case 'c':
        /* <c color_id>: text color */
        if (string.gx_string_ptr[2] == ' ')
        {
            string.gx_string_ptr += 3;
            string.gx_string_length -= 3;

            status = _gx_rich_text_resource_id_read(&string,  &resource_id);

            if (status != GX_SUCCESS)
            {
                return status;
            }

            tag_index = GX_RICH_TEXT_TAG_INDEX_COLOR;
            format -> gx_rich_text_color = resource_id;
        }
        break;

    case 'h':
        /* <hc color_id>: highlight color */
        if (string.gx_string_length > 3)
        {
            if (string.gx_string_ptr[2] == 'c' && string.gx_string_ptr[3] == ' ')
            {
                string.gx_string_ptr += 4;
                string.gx_string_length -= 4;
                status = _gx_rich_text_resource_id_read(&string, &resource_id);

                if (status != GX_SUCCESS)
                {
                    return status;
                }

                tag_index = GX_RICH_TEXT_TAG_INDEX_HICOLOR;
                format -> gx_rich_text_highlight_color = resource_id;
            }
        }
        break;

    case 'a':
        /* <align align-value>: alignment */
        tag_string.gx_string_ptr = "lign ";
        tag_string.gx_string_length = sizeof("lign ") - 1;

        string.gx_string_ptr += 2;
        string.gx_string_length -= 2;

        if (tag_string.gx_string_length <= string.gx_string_length)
        {
            if (_gx_utility_string_compare(&string, &tag_string, tag_string.gx_string_length) == GX_TRUE)
            {
                string.gx_string_ptr += tag_string.gx_string_length;
                string.gx_string_length -= tag_string.gx_string_length;

                switch (string.gx_string_ptr[0])
                {
                case 'c':
                    tag_string.gx_string_ptr = "enter>";
                    tag_string.gx_string_length = sizeof("enter>") - 1;
                    alignment = GX_RICH_TEXT_CENTER;
                    break;

                case 'r':
                    tag_string.gx_string_ptr = "ight>";
                    tag_string.gx_string_length = sizeof("ight>") - 1;
                    alignment = GX_RICH_TEXT_RIGHT;
                    break;

                case 'l':
                    tag_string.gx_string_ptr = "eft>";
                    tag_string.gx_string_length = sizeof("eft>") - 1;
                    alignment = GX_RICH_TEXT_LEFT;
                    break;

                default:
                    return GX_FAILURE;
                }

                string.gx_string_ptr++;
                string.gx_string_length--;

                if (tag_string.gx_string_length <= string.gx_string_length)
                {
                    if (_gx_utility_string_compare(&string, &tag_string, tag_string.gx_string_length) == GX_TRUE)
                    {
                        format -> gx_rich_text_flags &= (GX_UBYTE)(~GX_RICH_TEXT_ALIGN_MASK);
                        format -> gx_rich_text_flags |= alignment;
                        string.gx_string_length -= tag_string.gx_string_length;

                        tag_index = GX_RICH_TEXT_TAG_INDEX_ALIGN;
                        status = GX_SUCCESS;
                    }
                }
            }
        }
        break;

    default:
        return GX_FAILURE;
    }

    if (status == GX_SUCCESS)
    {
        context.gx_rich_text_context_tag = &gx_rich_text_end_tag_list[tag_index];

        /* Push draw style to stack. */
        status = _gx_rich_text_view_context_push(&context);
    }

    if (status == GX_SUCCESS)
    {
        *handled_bytes = (GX_UBYTE)(text -> gx_string_length - string.gx_string_length);
    }

    return status;
}


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_rich_text_view_tag_close                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function detects rich text close tag from specified text       */
/*    position and updated rich text draw style accordion to tag type.    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text                                  Text for processing           */
/*    format                                Current rich text format      */
/*    handled_bytes                         Bytes been processed          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                                              */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_string_compare            Test if two strings equal     */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_rich_text_view_tag_enter                                        */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  09-30-2020     Kenneth Maxwell          Initial Version 6.1           */
/*                                                                        */
/**************************************************************************/
static UINT _gx_rich_text_view_tag_close(GX_CONST GX_STRING *text, GX_RICH_TEXT_FORMAT *format, GX_UBYTE *handled_bytes)
{
UINT                 status = GX_FAILURE;
GX_RICH_TEXT_CONTEXT context;
GX_CONST GX_STRING  *tag;

    /* Peek tag in top of the stack. */
    status = _gx_rich_text_view_context_peek(&context);

    if (status == GX_SUCCESS)
    {
        tag = context.gx_rich_text_context_tag;

        /* Test if we meet the end tag in stack top. */
        if (_gx_utility_string_compare(text, tag, tag -> gx_string_length) == GX_TRUE)
        {
            /* Yes, it's time to pop the style. */
            _gx_rich_text_view_context_pop();
            *format = context.gx_rich_text_context_format;
            *handled_bytes = (GX_UBYTE)tag -> gx_string_length;
        }
        else
        {
            status = GX_FAILURE;
        }
    }

    return status;
}


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_rich_text_view_tag_enter                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function detects rich text tag from specified text position,   */
/*    and updated rich text draw style accordion to tag type.             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    view                                  Rich text view control block  */
/*    text                                  Text for processing           */
/*    format                                Current rich text format      */
/*    handled_bytes                         Bytes been processed          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                                              */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_rich_text_view_tag_open           Process open tag              */
/*    _gx_rich_text_view_tag_close          Prpcess close tag             */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  09-30-2020     Kenneth Maxwell          Initial Version 6.1           */
/*                                                                        */
/**************************************************************************/
UINT _gx_rich_text_view_tag_enter(GX_RICH_TEXT_VIEW *view, GX_CONST GX_STRING *text, GX_RICH_TEXT_FORMAT *format, GX_UBYTE *handled_bytes)
{

    if (text -> gx_string_ptr[0] != '<')
    {
        /* Tag must start with '<'. */
        return GX_FAILURE;
    }

    if (text -> gx_string_length < 3)
    {
        /* Tag contains at least 3 character: <tag-name>, <tag-name tag-value>, </tag-name>. */
        return GX_FAILURE;
    }

    if (text -> gx_string_ptr[1] == '/')
    {
        if (text -> gx_string_length < 4)
        {
            /* Close tag contains at least 4 character: </tag-name>. */
            return GX_FAILURE;
        }

        return _gx_rich_text_view_tag_close(text, format, handled_bytes);
    }
    else
    {
        return _gx_rich_text_view_tag_open(view, text, format, handled_bytes);
    }
}

