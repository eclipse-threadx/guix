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
/**   Window Management (Window)                                          */
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
/*    _gx_horizontal_list_total_columns_set               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function assigns the number of list columns                    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    vertical_list                         Vertical list widget control  */
/*                                          block                         */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_lock                       Obtain GUIX system lock       */
/*    _gx_system_unlock                     Release GUIX system lock      */
/*    _gx_first_client_child_get            Get the first client child    */
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
/*                                                                        */
/**************************************************************************/
UINT _gx_horizontal_list_total_columns_set(GX_HORIZONTAL_LIST *list, INT count)
{
INT           page_index;
INT           row;
GX_WIDGET    *test;
GX_BOOL       list_count_change = GX_FALSE;
GX_SCROLLBAR *pScroll;

    _gx_system_lock();

    /* Update total count of rows. */
    list -> gx_horizontal_list_total_columns = count;

    /* Calculate current page index. */
    page_index = list -> gx_horizontal_list_top_index;

    test = _gx_widget_first_client_child_get((GX_WIDGET *)list);

    while (test && (test -> gx_widget_size.gx_rectangle_right <= list -> gx_widget_size.gx_rectangle_left))
    {
        page_index++;
        test = _gx_widget_next_client_child_get(test);
    }

    /* Calculate new page index */
    if (page_index + list -> gx_horizontal_list_visible_columns > count)
    {
        if (count > list -> gx_horizontal_list_visible_columns)
        {
            page_index = count - list -> gx_horizontal_list_visible_columns;
        }
        else
        {
            page_index = 0;
        }
    }

    /* Check whether list child count is larger than count. */
    while (list -> gx_horizontal_list_child_count > count)
    {
        test = _gx_widget_last_client_child_get((GX_WIDGET *)list);

        if (test)
        {
            _gx_widget_detach(test);
            list -> gx_horizontal_list_child_count--;

            list_count_change = GX_TRUE;
        }
        else
        {
            return GX_FAILURE;
        }
    }

    if ((!list_count_change && list -> gx_horizontal_list_child_count == count) ||
        ((list -> gx_widget_style & GX_STYLE_WRAP) &&
         (list -> gx_horizontal_list_visible_columns == count)))
    {
        /* Re-populate list child. */
        row = 0;

        list -> gx_horizontal_list_top_index = 0;

        test = _gx_widget_first_client_child_get((GX_WIDGET *)list);

        while (test)
        {
            /* Reset child id. */
            test -> gx_widget_id = (USHORT)(LIST_CHILD_ID_START + row);
            test -> gx_widget_style &= ~GX_STYLE_DRAW_SELECTED;
            list -> gx_horizontal_list_callback(list, test, row);

            if (row == list -> gx_horizontal_list_selected)
            {
                test -> gx_widget_style |= GX_STYLE_DRAW_SELECTED;
                _gx_system_dirty_mark(test);
            }

            test = _gx_widget_next_client_child_get(test);
            row++;
        }
    }

    /* Make new page index visible */
    _gx_horizontal_list_page_index_set(list, page_index);

    _gx_window_scrollbar_find((GX_WINDOW *)list, GX_TYPE_HORIZONTAL_SCROLL, &pScroll);

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

