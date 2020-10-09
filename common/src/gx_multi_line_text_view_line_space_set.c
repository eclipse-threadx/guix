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
#include "gx_scrollbar.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_view_line_pace_set              PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function sets the line space of a multi line text view widget. */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    view                                  Multi-line text view widget   */
/*                                            control block               */
/*    line_space                            Value to set                  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_multi_line_text_view_string_total_rows_compute                  */
/*                                          Update string total rows      */
/*    _gx_window_scrollbar_find             Find scrollbar for a window   */
/*    _gx_scrollbar_reset                   Reset scrollbar information   */
/*    _gx_multi_line_text_view_line_cache_update                          */
/*                                          Update line cache             */
/*    _gx_system_dirty_mark                 Mark the widget dirty         */
/*    _gx_system_event_fold                 Fold a event                  */
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
UINT _gx_multi_line_text_view_line_space_set(GX_MULTI_LINE_TEXT_VIEW *view, GX_BYTE line_space)
{
GX_EVENT      newevent;

    view -> gx_multi_line_text_view_line_space = line_space;
    view -> gx_multi_line_text_view_line_index_old = GX_TRUE;

    if (view -> gx_widget_status & GX_STATUS_VISIBLE)
    {
        memset(&newevent, 0, sizeof(GX_EVENT));
        newevent.gx_event_type = GX_EVENT_CLIENT_UPDATED;
        newevent.gx_event_target = (GX_WIDGET *)view;
        _gx_system_event_fold(&newevent);
    }

    return(GX_SUCCESS);
}

