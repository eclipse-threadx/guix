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

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_next_client_child_get                           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function get the next client child.                            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    start                                Pointer to starting widget     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    child                                Next client child              */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_drop_list_event_process                                         */
/*    _gx_horizontal_list_event_process                                   */
/*    _gx_horizontal_list_left_wrap                                       */
/*    _gx_horizontal_list_right_wrap                                      */
/*    _gx_horizontal_list_scroll_info_get                                 */
/*    _gx_horizontal_list_slide_back_check                                */
/*    _gx_vertical_list_down_wrap                                         */
/*    _gx_vertical_list_event_process                                     */
/*    _gx_vertical_list_scroll_info_get                                   */
/*    _gx_vertical_list_slide_back_check                                  */
/*    _gx_vertical_list_up_wrap                                           */
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
GX_WIDGET *_gx_widget_next_client_child_get(GX_WIDGET *start)
{
GX_WIDGET *child = start -> gx_widget_next;

    while (child)
    {
        if (!(child -> gx_widget_status & GX_STATUS_NONCLIENT))
        {
            break;
        }
        child = child -> gx_widget_next;
    }
    return child;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_widget_next_visible_client_child_get            PORTABLE C      */
/*                                                           6.1.7        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function get the next client child that is visible.            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    start                                Pointer to starting widget     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    child                                Next visible client child      */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  06-02-2021     Ting Zhu                 Initial Version 6.1.7         */
/*                                                                        */
/**************************************************************************/
GX_WIDGET *_gx_widget_next_visible_client_child_get(GX_WIDGET *start)
{
GX_WIDGET *child = start -> gx_widget_next;

    while (child)
    {
        if (!(child -> gx_widget_status & GX_STATUS_NONCLIENT) &&
            (child -> gx_widget_status & GX_STATUS_VISIBLE))
        {
        	
        	/* Find the next child that is client and visible. */
            return child;
        }
        child = child -> gx_widget_next;
    }
    return GX_NULL;
}

