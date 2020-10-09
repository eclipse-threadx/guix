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
/*    _gx_vertical_list_page_index_set                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service sets the starting page index for vertical list.        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    vertical_list                         Vertical list widget control  */
/*                                          block                         */
/*    index                                 The new top index             */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_vertical_list_selected_set        Process drop list event       */
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
UINT _gx_vertical_list_page_index_set(GX_VERTICAL_LIST *list, INT index)
{
GX_WIDGET    *child;
GX_SCROLLBAR *pScroll;
INT           scroll_value;

    if ((list -> gx_vertical_list_total_rows > 0) &&
        ((index <= list -> gx_vertical_list_total_rows - list -> gx_vertical_list_visible_rows) || (list -> gx_widget_style & GX_STYLE_WRAP)))
    {
        child = _gx_widget_first_client_child_get((GX_WIDGET *)list);

        if (child)
        {
            scroll_value = child -> gx_widget_size.gx_rectangle_top;
            scroll_value += (index - list -> gx_vertical_list_top_index) * list -> gx_vertical_list_child_height;
            scroll_value = list -> gx_window_client.gx_rectangle_top - scroll_value;

            /* Scroll to make index as the first visible entry. */
            _gx_vertical_list_scroll(list, scroll_value);

            _gx_window_scrollbar_find((GX_WINDOW *)list, GX_TYPE_VERTICAL_SCROLL, &pScroll);

            if (pScroll)
            {
                _gx_scrollbar_reset(pScroll, GX_NULL);
            }
        }
    }

    return GX_SUCCESS;
}

