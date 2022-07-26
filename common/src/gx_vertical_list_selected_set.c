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
#include "gx_scrollbar.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_vertical_list_selected_set                      PORTABLE C      */
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
/*    vertical_list                         Vertical list widget control  */
/*                                          block                         */
/*    index                                 Index based position of new   */
/*                                          list entry                    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_first_client_child_get                                   */
/*    _gx_widget_next_client_child_get                                    */
/*    _gx_vertical_list_page_index_set                                    */
/*    _gx_widget_event_generate                                           */
/*    _gx_system_dirty_mark                                               */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*    _gx_drop_list_event_process           Process drop list event       */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  07-29-2022     Kenneth Maxwell          Added support for GX_STYLE_   */
/*                                            REPEAT_SELECT,              */
/*                                            resulting in version 6.1.12 */
/*                                                                        */
/**************************************************************************/
UINT _gx_vertical_list_selected_set(GX_VERTICAL_LIST *vertical_list, INT index)
{
GX_WIDGET *child;
GX_WIDGET *found = GX_NULL;
INT        page_index = vertical_list -> gx_vertical_list_top_index;
INT        top_index;
INT        bottom_index;

   if (vertical_list -> gx_vertical_list_selected == index)
    {
        if ((vertical_list -> gx_widget_style & GX_STYLE_REPEAT_SELECT) == 0)
        {
            return GX_SUCCESS;
        }
    }

    if (index < 0)
    {
        _gx_vertical_list_selected_widget_get(vertical_list, &child);

        if (child)
        {
            child -> gx_widget_style &= ~GX_STYLE_DRAW_SELECTED;
            _gx_system_dirty_mark(child);
        }
        vertical_list -> gx_vertical_list_selected = index;
        return GX_SUCCESS;
    }

    child = _gx_widget_first_client_child_get((GX_WIDGET *)vertical_list);

    while (child && (child -> gx_widget_size.gx_rectangle_bottom <= vertical_list -> gx_widget_size.gx_rectangle_top))
    {
        page_index++;
        child = _gx_widget_next_client_child_get(child);
    }

    top_index = vertical_list -> gx_vertical_list_top_index;
    bottom_index = top_index + vertical_list -> gx_vertical_list_child_count - 1;

    /* Reset page index when needed and calculate the child count from the top index to new index.  */
    if (bottom_index >= vertical_list -> gx_vertical_list_total_rows)
    {
        bottom_index -= vertical_list -> gx_vertical_list_total_rows;

        if (index >= top_index)
        {
            page_index = index - vertical_list -> gx_vertical_list_top_index;
        }
        else if (index <= bottom_index)
        {
            page_index = vertical_list -> gx_vertical_list_total_rows + index - vertical_list -> gx_vertical_list_top_index;
        }
        else
        {
            _gx_vertical_list_page_index_set(vertical_list, index);
            page_index = index - vertical_list -> gx_vertical_list_top_index;
        }
    }
    else
    {
        if (index < top_index)
        {
            _gx_vertical_list_page_index_set(vertical_list, index);
        }
        else if (index > bottom_index)
        {
            _gx_vertical_list_page_index_set(vertical_list, index - vertical_list -> gx_vertical_list_visible_rows + 1);
        }

        page_index = index - vertical_list -> gx_vertical_list_top_index;
    }

    if (page_index < 0)
    {
        /* consider the situation that top index is bigger than current select index.  */
        page_index += vertical_list -> gx_vertical_list_total_rows;
    }

    child = _gx_widget_first_client_child_get((GX_WIDGET *)vertical_list);

    /* Select new index and update widget draw style.  */
    while (child)
    {
        if (page_index == 0)
        {
            found = child;
            vertical_list -> gx_vertical_list_selected = index;
            _gx_vertical_list_selected_visible(vertical_list, found);
            found -> gx_widget_style |= GX_STYLE_DRAW_SELECTED;
            _gx_system_dirty_mark(found);

            if (vertical_list -> gx_widget_id)
            {
                _gx_widget_event_generate((GX_WIDGET *)vertical_list, GX_EVENT_LIST_SELECT, vertical_list -> gx_vertical_list_selected);
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

