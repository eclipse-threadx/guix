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
/**   List Management (List)                                              */
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
/*    _gx_vertical_list_slide_back_check                  PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks the sliding back of the scrollbar.             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    vertical_list                         Vertical list widget control  */
/*                                            block                       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_first_client_child_get            Get the first child widget    */
/*    _gx_system_timer_start                Allocate a free timer and     */
/*                                            activate it                 */
/*    _gx_last_client_child_get             Get the last client child     */
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
VOID _gx_vertical_list_slide_back_check(GX_VERTICAL_LIST *list)
{
GX_WIDGET *child;
INT        page_index;

    if (list -> gx_vertical_list_child_count <= 0)
    {
        return;
    }

    /* Calculate page index. */
    page_index = list -> gx_vertical_list_top_index;

    child = _gx_widget_first_client_child_get((GX_WIDGET *)list);

    while (child && (child -> gx_widget_size.gx_rectangle_top <= list -> gx_widget_size.gx_rectangle_top))
    {
        page_index++;
        child = _gx_widget_next_client_child_get(child);
    }

    list -> gx_vertical_list_snap_back_distance = 0;

    if (page_index == 0)
    {
        child = _gx_widget_first_client_child_get((GX_WIDGET *)list);
        if (child && (child -> gx_widget_size.gx_rectangle_top > list -> gx_window_client.gx_rectangle_top))
        {
            list -> gx_vertical_list_snap_back_distance = (GX_VALUE)(list -> gx_window_client.gx_rectangle_top -
                                                                     child -> gx_widget_size.gx_rectangle_top);
            _gx_system_timer_start((GX_WIDGET *)list, GX_SNAP_TIMER, 1, 1);
        }
    }
    else
    {
        if (list -> gx_vertical_list_top_index + list -> gx_vertical_list_child_count >= list -> gx_vertical_list_total_rows - 1)
        {
            child = _gx_widget_last_client_child_get((GX_WIDGET *)list);
            if (child && (child -> gx_widget_size.gx_rectangle_bottom < list -> gx_window_client.gx_rectangle_bottom))
            {
                list -> gx_vertical_list_snap_back_distance = (GX_VALUE)(list -> gx_window_client.gx_rectangle_bottom -
                                                                         child -> gx_widget_size.gx_rectangle_bottom);
                _gx_system_timer_start((GX_WIDGET *)list, GX_SNAP_TIMER, 1, 1);
            }
        }
    }
}

