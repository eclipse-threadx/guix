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

#define GX_SNAP_TIMER  1000
#define GX_FLICK_TIMER 1001

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_horizontal_list_slide_back_check                PORTABLE C      */
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
/*    list                                  Horizontal list widget        */
/*                                            control block               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_first_client_child_get            Get the first client child    */
/*    _gx_system_timer_start                Allocate a free timer and     */
/*                                            activates it                */
/*    _gx_last_client_child_get             Get the last client child     */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_horizontal_list_event_process                                   */
/*    sw_horizontal_list_event_process                                    */
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
VOID _gx_horizontal_list_slide_back_check(GX_HORIZONTAL_LIST *list)
{
GX_WIDGET *child;
INT        page_index;

    if (list -> gx_horizontal_list_child_count <= 0)
    {
        return;
    }

    /* Calculate page index. */
    page_index = list -> gx_horizontal_list_top_index;

    child = _gx_widget_first_client_child_get((GX_WIDGET *)list);

    while (child && (child -> gx_widget_size.gx_rectangle_left <= list -> gx_widget_size.gx_rectangle_left))
    {
        page_index++;
        child = _gx_widget_next_client_child_get(child);
    }

    list -> gx_horizontal_list_snap_back_distance = 0;

    if (page_index == 0)
    {
        child = _gx_widget_first_client_child_get((GX_WIDGET *)list);
        if (child && (child -> gx_widget_size.gx_rectangle_left > list -> gx_window_client.gx_rectangle_left))
        {
            list -> gx_horizontal_list_snap_back_distance = (GX_VALUE)(list -> gx_window_client.gx_rectangle_left -
                                                                       child -> gx_widget_size.gx_rectangle_left);
            _gx_system_timer_start((GX_WIDGET *)list, GX_SNAP_TIMER, 1, 1);
        }
    }
    else
    {
        if (list -> gx_horizontal_list_top_index + list -> gx_horizontal_list_child_count >= list -> gx_horizontal_list_total_columns - 1)
        {
            child = _gx_widget_last_client_child_get((GX_WIDGET *)list);
            if (child && (child -> gx_widget_size.gx_rectangle_right < list -> gx_window_client.gx_rectangle_right))
            {
                list -> gx_horizontal_list_snap_back_distance = (GX_VALUE)(list -> gx_window_client.gx_rectangle_right -
                                                                           child -> gx_widget_size.gx_rectangle_right);
                _gx_system_timer_start((GX_WIDGET *)list, GX_SNAP_TIMER, 1, 1);
            }
        }
    }
}

