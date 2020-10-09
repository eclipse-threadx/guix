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
#include "gx_widget.h"
#include "gx_window.h"
#include "gx_multi_line_text_view.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_view_create                     PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates a multi-line text view widget.                */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_view                             Multi-line text view widget   */
/*                                            control block               */
/*    name                                  Name of the text input widget */
/*    parent                                Pointer to parent widget      */
/*    text_id                               Resource ID of the text string*/
/*    style                                 Style of text view widget     */
/*    id                                    Application-defined ID of text*/
/*                                            view                        */
/*    size                                  Dimension of text view widget */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_window_create                     Create the window             */
/*    _gx_widget_link                       Link the widget to its parent */
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
/*                                            added logic to init new     */
/*                                            structure member for        */
/*                                            dynamic bidi text support,  */
/*                                            fixed compiler warnings,    */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
UINT    _gx_multi_line_text_view_create(GX_MULTI_LINE_TEXT_VIEW *text_view,
                                        GX_CONST GX_CHAR *name,
                                        GX_WIDGET *parent,
                                        GX_RESOURCE_ID text_id,
                                        ULONG style,
                                        USHORT id,
                                        GX_CONST GX_RECTANGLE *size)
{

    /* Call the window create function.  */
    _gx_window_create((GX_WINDOW *)text_view, name, GX_NULL, style, id, size);

    text_view -> gx_widget_type = GX_TYPE_MULTI_LINE_TEXT_VIEW;
    text_view -> gx_widget_normal_fill_color = GX_COLOR_ID_TEXT_INPUT_FILL;
    text_view -> gx_multi_line_text_view_normal_text_color = GX_COLOR_ID_TEXT_INPUT_TEXT;
    text_view -> gx_multi_line_text_view_selected_text_color = GX_COLOR_ID_TEXT_INPUT_TEXT;
    text_view -> gx_multi_line_text_view_disabled_text_color = GX_COLOR_ID_DISABLED_TEXT;
    text_view -> gx_widget_draw_function = (VOID (*)(GX_WIDGET *))_gx_multi_line_text_view_draw;
    text_view -> gx_widget_event_process_function = (UINT (*)(GX_WIDGET *, GX_EVENT *))_gx_multi_line_text_view_event_process;
    text_view -> gx_window_scroll_info_get = (VOID (*)(struct GX_WINDOW_STRUCT *, ULONG, GX_SCROLL_INFO *))(void (*)(void))_gx_multi_line_text_view_scroll_info_get;
    text_view -> gx_multi_line_text_view_text_id = text_id;
    text_view -> gx_multi_line_text_view_font_id = GX_FONT_ID_TEXT_INPUT;
    text_view -> gx_multi_line_text_view_text.gx_string_ptr = GX_NULL;
    text_view -> gx_multi_line_text_view_text.gx_string_length = 0;
    text_view -> gx_multi_line_text_view_text_scroll_shift = 0;
    text_view -> gx_multi_line_text_view_text_total_rows = 0;
    text_view -> gx_multi_line_text_view_text_visible_rows = 0;
    text_view -> gx_multi_line_text_view_whitespace = 0;
    text_view -> gx_multi_line_text_view_line_space = 0;
    text_view -> gx_multi_line_text_view_line_index_old = GX_TRUE;
    text_view -> gx_multi_line_text_view_first_cache_line = 0;
    text_view -> gx_multi_line_text_view_cache_size = 0;

#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
    text_view -> gx_multi_line_text_view_bidi_resolved_text_info = GX_NULL;
#endif

    text_view -> gx_widget_status |= GX_STATUS_RESIZE_NOTIFY;

    memset(text_view -> gx_multi_line_text_view_line_index, 0, GX_MULTI_LINE_INDEX_CACHE_SIZE * sizeof(UINT));

    /* Determine if a parent widget was provided.  */
    if (parent)
    {
        _gx_widget_link(parent, (GX_WIDGET *)text_view);
    }

    /* Return the error status from window create.  */
    return(GX_SUCCESS);
}

