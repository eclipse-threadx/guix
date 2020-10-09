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
#include "gx_utility.h"
#include "gx_window.h"
#include "gx_widget.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_view_text_id_set                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates a text input widget.                          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_view                             Multi-line view control block */
/*    text_id                               Resource ID for the text      */
/*                                            string                      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_string_length_check       Test string length            */
/*    _gx_system_string_get                 Get the string from the ID    */
/*    _gx_system_memory_free                Application-defined memory    */
/*                                            free function               */
/*    _gx_multi_line_text_view_string_total_rows_compute                  */
/*                                          Compute the total rows of     */
/*                                            of the display text         */
/*    _gx_multi_line_text_view_line_cache_update                          */
/*                                          Update line cache             */
/*    _gx_window_scrollbar_find             Find scrollbar for a window   */
/*    _gx_scrollbar_reset                   Reset scrollbar information   */
/*    _gx_utility_utf8_string_character_count_get                         */
/*                                          Get character count of utf8   */
/*                                            string                      */
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
UINT _gx_multi_line_text_view_text_id_set(GX_MULTI_LINE_TEXT_VIEW *text_view,
                                          GX_RESOURCE_ID text_id)
{
    if (text_view -> gx_widget_style & GX_STYLE_TEXT_COPY)
    {
        if (text_view -> gx_multi_line_text_view_text.gx_string_ptr)
        {
            _gx_system_memory_free((void *)text_view -> gx_multi_line_text_view_text.gx_string_ptr);
        }
    }

    text_view -> gx_multi_line_text_view_text_id = text_id;
    text_view -> gx_multi_line_text_view_text_total_rows = 0;
    text_view -> gx_multi_line_text_view_line_index_old = GX_TRUE;

    _gx_widget_string_get_ext((GX_WIDGET *)text_view, text_id, &text_view -> gx_multi_line_text_view_text);
    text_view -> gx_multi_line_text_view_text.gx_string_ptr = GX_NULL;

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

    return(GX_SUCCESS);
}

