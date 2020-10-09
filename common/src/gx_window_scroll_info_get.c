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


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_window_scroll_info_get                          PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service gets the window scroll information.                    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    window                               Pointer to window              */
/*    type                                 GX_SCROLLBAR_HORIZONTAL        */
/*                                           or GX_SCROLLBAR_VERTICAL     */
/*    return_scroll_info                   Pointer to destination for     */
/*                                           scroll info                  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                               Completion status              */
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
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
UINT _gx_window_scroll_info_get(GX_WINDOW *window, ULONG type, GX_SCROLL_INFO *return_scroll_info)
{
GX_WIDGET *child;

    /* default scroll information is to gather the limits of my child
       widgets, so that they can all be scrolled into view
     */
    return_scroll_info -> gx_scroll_increment = 1;

    if (type & GX_SCROLLBAR_VERTICAL)
    {
        return_scroll_info -> gx_scroll_maximum = window -> gx_window_client.gx_rectangle_bottom;
        return_scroll_info -> gx_scroll_minimum = window -> gx_window_client.gx_rectangle_top;
        return_scroll_info -> gx_scroll_visible = (GX_VALUE)(return_scroll_info -> gx_scroll_maximum - return_scroll_info -> gx_scroll_minimum + 1);
        return_scroll_info -> gx_scroll_value = window -> gx_window_client.gx_rectangle_top;
        child = window -> gx_widget_first_child;

        while (child)
        {
            if (!(child -> gx_widget_status & GX_STATUS_NONCLIENT))
            {
                if (child -> gx_widget_size.gx_rectangle_top < return_scroll_info -> gx_scroll_minimum)
                {
                    return_scroll_info -> gx_scroll_minimum = child -> gx_widget_size.gx_rectangle_top;
                }
                if (child -> gx_widget_size.gx_rectangle_bottom > return_scroll_info -> gx_scroll_maximum)
                {
                    return_scroll_info -> gx_scroll_maximum = child -> gx_widget_size.gx_rectangle_bottom;
                }
            }
            child = child -> gx_widget_next;
        }
    }
    else
    {
        return_scroll_info -> gx_scroll_maximum = window -> gx_window_client.gx_rectangle_right;
        return_scroll_info -> gx_scroll_minimum = window -> gx_window_client.gx_rectangle_left;
        return_scroll_info -> gx_scroll_visible = (GX_VALUE)(return_scroll_info -> gx_scroll_maximum - return_scroll_info -> gx_scroll_minimum + 1);
        return_scroll_info -> gx_scroll_value = window -> gx_window_client.gx_rectangle_left;
        child = window -> gx_widget_first_child;

        while (child)
        {
            if (!(child -> gx_widget_status & GX_STATUS_NONCLIENT))
            {
                if (child -> gx_widget_size.gx_rectangle_left < return_scroll_info -> gx_scroll_minimum)
                {
                    return_scroll_info -> gx_scroll_minimum = child -> gx_widget_size.gx_rectangle_left;
                }
                if (child -> gx_widget_size.gx_rectangle_right > return_scroll_info -> gx_scroll_maximum)
                {
                    return_scroll_info -> gx_scroll_maximum = child -> gx_widget_size.gx_rectangle_right;
                }
            }
            child = child -> gx_widget_next;
        }
    }

    return_scroll_info -> gx_scroll_increment = (GX_VALUE)(return_scroll_info -> gx_scroll_maximum - return_scroll_info -> gx_scroll_minimum) / 10;

    return(GX_SUCCESS);
}

