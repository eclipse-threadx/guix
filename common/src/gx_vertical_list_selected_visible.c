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
/*    _gx_vertical_list_selected_visible                  PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service make the selected list item visible.                   */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    vertical_list                         Vertical list widget control  */
/*                                          block                         */
/*    list_entry                            Pointer to new list entry     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    NONE                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_vertical_list_scroll                                            */
/*    _gx_window_scrollbar_find                                           */
/*    _gx_scrollbar_reset                                                 */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_vertical_list_selected_set                                      */
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
VOID _gx_vertical_list_selected_visible(GX_VERTICAL_LIST *list, GX_WIDGET *child)
{
GX_SCROLLBAR *pScroll;
GX_BOOL       update_scroll = GX_FALSE;

    if (child -> gx_widget_size.gx_rectangle_top < list -> gx_window_client.gx_rectangle_top)
    {
        /* need to scroll children down */
        _gx_vertical_list_scroll(list, list -> gx_window_client.gx_rectangle_top - child -> gx_widget_size.gx_rectangle_top);
        update_scroll = GX_TRUE;
    }
    else
    {
        if (child -> gx_widget_size.gx_rectangle_bottom > list -> gx_window_client.gx_rectangle_bottom)
        {
            /* need to scroll children up */
            _gx_vertical_list_scroll(list, list -> gx_window_client.gx_rectangle_bottom - child -> gx_widget_size.gx_rectangle_bottom);
            update_scroll = GX_TRUE;
        }
    }

    if (update_scroll)
    {
        _gx_window_scrollbar_find((GX_WINDOW *)list, GX_TYPE_VERTICAL_SCROLL, &pScroll);

        if (pScroll)
        {
            _gx_scrollbar_reset(pScroll, GX_NULL);
        }
    }
}

