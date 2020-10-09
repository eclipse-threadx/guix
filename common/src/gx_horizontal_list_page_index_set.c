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
/*    _gx_horizontal_list_page_index_set                  PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service sets the index of first list item                      */
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
UINT _gx_horizontal_list_page_index_set(GX_HORIZONTAL_LIST *list, INT index)
{
GX_WIDGET    *child;
GX_SCROLLBAR *pScroll;
INT           scroll_value;

    if ((list -> gx_horizontal_list_total_columns > 0) &&
        ((index <= list -> gx_horizontal_list_total_columns - list -> gx_horizontal_list_visible_columns) || (list -> gx_widget_style & GX_STYLE_WRAP)))
    {
        child = _gx_widget_first_client_child_get((GX_WIDGET *)list);

        if (child)
        {
            /* Calculate scroll value. */
            scroll_value = child -> gx_widget_size.gx_rectangle_left;
            scroll_value += (index - list -> gx_horizontal_list_top_index) * list -> gx_horizontal_list_child_width;
            scroll_value = list -> gx_window_client.gx_rectangle_left - scroll_value;

            /* Scroll to make index as the first visible entry. */
            _gx_horizontal_list_scroll(list, scroll_value);

            _gx_window_scrollbar_find((GX_WINDOW *)list, GX_TYPE_HORIZONTAL_SCROLL, &pScroll);

            if (pScroll)
            {
                _gx_scrollbar_reset(pScroll, GX_NULL);
            }
        }
    }

    return GX_SUCCESS;
}

