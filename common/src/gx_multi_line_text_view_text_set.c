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
/**   Multi Line Text View Management (Multi Line Text View)              */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_multi_line_text_view.h"
#include "gx_scrollbar.h"
#include "gx_window.h"
#include "gx_widget.h"
#include "gx_utility.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_view_text_set                   PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION (Deprecated)                                              */
/*                                                                        */
/*    This function assigns a text string to the multi line text view     */
/*    widget.                                                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_view                             Multi-line text view widget   */
/*                                            control block               */
/*    text                                  Null-terminated text string   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_multi_line_text_view_text_set_ext                               */
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
UINT _gx_multi_line_text_view_text_set(GX_MULTI_LINE_TEXT_VIEW *text_view,
                                       GX_CONST GX_CHAR *text)
{
UINT      status = GX_SUCCESS;
UINT      length = 0;
GX_STRING string;

    if (text)
    {
        status = _gx_utility_string_length_check(text, &length, GX_MAX_STRING_LENGTH);
    }

    if (status == GX_SUCCESS)
    {
        string.gx_string_ptr = text;
        string.gx_string_length = length;
        status = _gx_multi_line_text_view_text_set_ext(text_view, &string);
    }
    return status;
}
#endif



/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_view_text_set_ext               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function assigns a text string to the multi line text view     */
/*    widget.                                                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_view                             Multi-line text view widget   */
/*                                            control block               */
/*    text                                  GX_STRING type text string    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_string_length_check       Test string length            */
/*    _gx_system_private_string_copy        Make a private copy for       */
/*                                            assigned text               */
/*    _gx_system_dirty_mark                 Mark a widget as dirty        */
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
/*                                            added logic to delete       */
/*                                            dynamic bidi text,          */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
UINT _gx_multi_line_text_view_text_set_ext(GX_MULTI_LINE_TEXT_VIEW *text_view, GX_CONST GX_STRING *text)
{
UINT status = GX_SUCCESS;

    if (text_view -> gx_widget_style & GX_STYLE_TEXT_COPY)
    {
        status = _gx_system_private_string_copy(&text_view -> gx_multi_line_text_view_text, text);
    }
    else
    {
        if (text)
        {
            text_view -> gx_multi_line_text_view_text = *text;
        }
        else
        {
            text_view -> gx_multi_line_text_view_text.gx_string_ptr = GX_NULL;
            text_view -> gx_multi_line_text_view_text.gx_string_length = 0;
        }
    }

    text_view -> gx_multi_line_text_view_text_id = 0;
    text_view -> gx_multi_line_text_view_line_index_old = GX_TRUE;

#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
    if (text_view -> gx_multi_line_text_view_bidi_resolved_text_info)
    {
        _gx_utility_bidi_resolved_text_info_delete(&text_view -> gx_multi_line_text_view_bidi_resolved_text_info);
    }
#endif

    if (text_view -> gx_widget_status & GX_STATUS_VISIBLE)
    {
        _gx_system_dirty_mark((GX_WIDGET *)text_view);
    }

    return(status);
}

