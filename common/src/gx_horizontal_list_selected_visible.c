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
/**   Horizontal List (List)                                              */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_window.h"
#include "gx_system.h"
#include "gx_scrollbar.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_horizontal_list_selected_visible                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service sets the list entry at the current list index.         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    horizontal_list                       Horizontal list widget        */
/*                                            control block               */
/*    list_entry                            Pointer to new list entry     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_event_generate             Creates a new GX_EVENT        */
/*    _gx_system_dirty_mark                 Mark the widget dirty         */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_horizontal_list_event_process                                   */
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
VOID _gx_horizontal_list_selected_visible(GX_HORIZONTAL_LIST *list, GX_WIDGET *child)
{
GX_SCROLLBAR *pScroll;
GX_BOOL       update_scroll = GX_FALSE;

    if (child -> gx_widget_size.gx_rectangle_left < list -> gx_window_client.gx_rectangle_left)
    {
        /* need to scroll children down */
        _gx_horizontal_list_scroll(list, list -> gx_window_client.gx_rectangle_left - child -> gx_widget_size.gx_rectangle_left);
        update_scroll = GX_TRUE;
    }
    else
    {
        if (child -> gx_widget_size.gx_rectangle_right > list -> gx_window_client.gx_rectangle_right)
        {
            /* need to scroll children up */
            _gx_horizontal_list_scroll(list, list -> gx_window_client.gx_rectangle_right - child -> gx_widget_size.gx_rectangle_right);
            update_scroll = GX_TRUE;
        }
    }

    if (update_scroll)
    {
        _gx_window_scrollbar_find((GX_WINDOW *)list, GX_TYPE_HORIZONTAL_SCROLL, &pScroll);

        if (pScroll)
        {
            _gx_scrollbar_reset(pScroll, GX_NULL);
        }
    }
}

