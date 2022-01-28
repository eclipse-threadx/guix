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
/**   Vertical List (List)                                                */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_window.h"
#include "gx_system.h"
#include "gx_utility.h"
#include "gx_scrollbar.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_vertical_list_total_rows_set                    PORTABLE C      */
/*                                                           6.1.10       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function assigns total number of list rows.                    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    vertical_list                         Vertical list control block   */
/*    count                                 Number of rows                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_lock                       Obtain GUIX system lock       */
/*    _gx_system_unlock                     Release GUIX system lock      */
/*    _gx_first_client_child_get            Get the first client child    */
/*    [gx_vertical_list_callback]           Vertical list callback        */
/*    _gx_window_scrollbar_find             Find the scrollbar            */
/*    _gx_scrollbar_reset                   Reset the schollbar           */
/*    _gx_system_dirty_mark                 Mark the widget dirty         */
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
/*  01-31-2022     Ting Zhu                 Modified comment(s),          */
/*                                            improved logic,             */
/*                                            resulting in version 6.1.10 */
/*                                                                        */
/**************************************************************************/
UINT _gx_vertical_list_total_rows_set(GX_VERTICAL_LIST *list, INT count)
{
INT           page_index;
INT           index;
GX_WIDGET    *test;
GX_SCROLLBAR *pScroll;

    _gx_system_lock();

    /* Update total count of rows. */
    list -> gx_vertical_list_total_rows = count;

    /* Update selected index. */
    if (list -> gx_vertical_list_selected < 0)
    {
        list -> gx_vertical_list_selected = 0;
    }

    if (list -> gx_vertical_list_selected > count - 1)
    {
        list -> gx_vertical_list_selected = count - 1;
    }

    /* Calculate current page index. */
    page_index = list -> gx_vertical_list_top_index;

    test = _gx_widget_first_client_child_get((GX_WIDGET *)list);

    while (test && (test -> gx_widget_size.gx_rectangle_bottom <= list -> gx_widget_size.gx_rectangle_top))
    {
        page_index++;
        test = _gx_widget_next_client_child_get(test);
    }

    /* Calculate new page index */
    if (page_index + list -> gx_vertical_list_visible_rows > count)
    {
        if (count > list -> gx_vertical_list_visible_rows)
        {
            page_index = count - list -> gx_vertical_list_visible_rows;
        }
        else
        {
            page_index = 0;
        }
    }

    /* Add idle children back to vertical list.  */
    if (list -> gx_vertical_list_idle_child_list)
    {
        while (list -> gx_vertical_list_idle_child_list)
        {
            test = list -> gx_vertical_list_idle_child_list;
            list -> gx_vertical_list_idle_child_list = list -> gx_vertical_list_idle_child_list -> gx_widget_next;

            _gx_widget_attach((GX_WIDGET *)list, test);
            list -> gx_vertical_list_child_count++;
        }
    }

    /* Check whether list child count is larger than count. */
    while (list -> gx_vertical_list_child_count > count)
    {
        test = _gx_widget_last_client_child_get((GX_WIDGET *)list);

        if (test)
        {
            _gx_widget_detach(test);

            /* Put detached widget to idle list.  */
            test -> gx_widget_next = list -> gx_vertical_list_idle_child_list;
            list -> gx_vertical_list_idle_child_list = test;
            list -> gx_vertical_list_child_count--;
        }
        else
        {
            return GX_FAILURE;
        }
    }

    list -> gx_vertical_list_top_index = 0;
    index = 0;
    test = _gx_widget_first_client_child_get((GX_WIDGET *)list);

    while (test)
    {
        list -> gx_vertical_list_callback(list, test, index++);

        test = _gx_widget_next_client_child_get(test);
    }

    /* Reposition child widgets.  */
    _gx_vertical_list_children_position(list);

    /* Make new page index visible */
    _gx_vertical_list_page_index_set(list, page_index);

    _gx_window_scrollbar_find((GX_WINDOW *)list, GX_TYPE_VERTICAL_SCROLL, &pScroll);

    if (pScroll)
    {
        _gx_scrollbar_reset(pScroll, GX_NULL);
    }

    _gx_system_unlock();

    /* Refresh screen. */
    if (list -> gx_widget_status & GX_STATUS_VISIBLE)
    {
        _gx_system_dirty_mark((GX_WIDGET *)list);
    }
    return GX_SUCCESS;
}

