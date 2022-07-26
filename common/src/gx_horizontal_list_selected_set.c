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
/*    _gx_horizontal_list_selected_set                    PORTABLE C      */
/*                                                           6.1.12       */
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
/*    index                                 Index based position of new   */
/*                                            list entry                  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_first_client_child_get                                   */
/*    _gx_widget_next_client_child_get                                    */
/*    _gx_horizontal_list_page_index_set                                  */
/*    _gx_widget_event_generate                                           */
/*    _gx_system_dirty_mark                                               */
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
/*  07-29-2022     Kenneth Maxwell          Modified comment(s),          */
/*                                            added support for           */
/*                                            GX_STYLE_REPEAT_SELECT,     */
/*                                            resulting in version 6.1.12 */
/*                                                                        */
/**************************************************************************/

UINT _gx_horizontal_list_selected_set(GX_HORIZONTAL_LIST *horizontal_list, INT index)
{
GX_WIDGET *child;
GX_WIDGET *found = GX_NULL;
INT        page_index = horizontal_list -> gx_horizontal_list_top_index;
INT        left_index;
INT        right_index;

    if (horizontal_list -> gx_horizontal_list_selected == index)
    {
        if ((horizontal_list -> gx_widget_style & GX_STYLE_REPEAT_SELECT) == 0)
        {
            return GX_SUCCESS;
        }
    }

    if (index < 0)
    {
        _gx_horizontal_list_selected_widget_get(horizontal_list, &child);

        if (child)
        {
            child -> gx_widget_style &= ~GX_STYLE_DRAW_SELECTED;
            _gx_system_dirty_mark(child);
        }
        horizontal_list -> gx_horizontal_list_selected = index;
        return GX_SUCCESS;
    }

    /* Calculate page index. */
    child = _gx_widget_first_client_child_get((GX_WIDGET *)horizontal_list);

    while (child && (child -> gx_widget_size.gx_rectangle_right <= horizontal_list -> gx_widget_size.gx_rectangle_left))
    {
        page_index++;
        child = _gx_widget_next_client_child_get(child);
    }

    left_index = horizontal_list -> gx_horizontal_list_top_index;
    right_index = left_index + horizontal_list -> gx_horizontal_list_child_count - 1;

    /* Reset page index when needed and calculate the child count from the top index to new index.  */
    if (right_index >= horizontal_list -> gx_horizontal_list_total_columns)
    {
        right_index -= horizontal_list -> gx_horizontal_list_total_columns;

        if (index >= left_index)
        {
            page_index = index - horizontal_list -> gx_horizontal_list_top_index;
        }
        else if (index <= right_index)
        {
            page_index = horizontal_list -> gx_horizontal_list_total_columns + index - horizontal_list -> gx_horizontal_list_top_index;
        }
        else
        {
            _gx_horizontal_list_page_index_set(horizontal_list, index);
            page_index = index - horizontal_list -> gx_horizontal_list_top_index;
        }
    }
    else
    {
        if (index < left_index)
        {
            _gx_horizontal_list_page_index_set(horizontal_list, index);
        }
        else if (index > right_index)
        {
            _gx_horizontal_list_page_index_set(horizontal_list, index - horizontal_list -> gx_horizontal_list_visible_columns + 1);
        }

        page_index = index - horizontal_list -> gx_horizontal_list_top_index;
    }

    if (page_index < 0)
    {
        /* consider the situation that top index is bigger than current select index.  */
        page_index += horizontal_list -> gx_horizontal_list_total_columns;
    }

    child = _gx_widget_first_client_child_get((GX_WIDGET *)horizontal_list);

    /* Select new index and update widget draw style.  */
    while (child)
    {
        if (page_index == 0)
        {
            found = child;

            horizontal_list -> gx_horizontal_list_selected = index;
            _gx_horizontal_list_selected_visible(horizontal_list, found);
            found -> gx_widget_style |= GX_STYLE_DRAW_SELECTED;
            _gx_system_dirty_mark(found);

            if (horizontal_list -> gx_widget_id)
            {
                _gx_widget_event_generate((GX_WIDGET *)horizontal_list, GX_EVENT_LIST_SELECT, horizontal_list -> gx_horizontal_list_selected);
            }
        }
        else
        {
            if (child -> gx_widget_style & GX_STYLE_DRAW_SELECTED)
            {
                child -> gx_widget_style &= ~GX_STYLE_DRAW_SELECTED;
                _gx_system_dirty_mark(child);
            }
        }
        child = _gx_widget_next_client_child_get(child);
        page_index--;
    }

    if (found)
    {
        return GX_SUCCESS;
    }
    return GX_FAILURE;
}

