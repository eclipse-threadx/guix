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
#include "gx_display.h"
#include "gx_canvas.h"
#include "gx_utility.h"
#include "gx_scrollbar.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_window_client_scroll                            PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service scrolls the window client area by the specified amount */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    window                                Pointer to window             */
/*    x_scroll                              Amount to scroll on x-axis    */
/*    y_scroll                              Amount to scroll on y-axis    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_scroll                     main scroll function          */
/*    _gx_scrollbar_value_set               reset scrollbar thumb pos     */
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
UINT _gx_window_client_scroll(GX_WINDOW *window, GX_VALUE x_scroll, GX_VALUE y_scroll)
{
GX_SCROLLBAR *scroll;
INT           scroll_value;

    /* This function is designed to be called by the application, i.e. not driven
       by the user operating a scrollbar. However if the window does have scrollbars,
       we will satisfy the scrolling request by assigning the scroll bar value.
     */
    scroll = GX_NULL;

    if (x_scroll)
    {
        _gx_window_scrollbar_find(window, (USHORT)GX_TYPE_HORIZONTAL_SCROLL, &scroll);

        if (scroll)
        {
            scroll_value = scroll -> gx_scrollbar_info.gx_scroll_value;
            scroll_value = scroll_value - x_scroll;

            /* this function will generate a scroll event, which will cause
               the parent window to scroll
            */
            _gx_scrollbar_value_set(scroll, scroll_value);
            x_scroll = 0;
        }
    }

    if (y_scroll)
    {
        _gx_window_scrollbar_find(window, GX_TYPE_VERTICAL_SCROLL, &scroll);

        if (scroll)
        {
            scroll_value = scroll -> gx_scrollbar_info.gx_scroll_value;
            scroll_value = scroll_value - y_scroll;
            _gx_scrollbar_value_set(scroll, scroll_value);
            y_scroll = 0;
        }
    }

    /* If we get to here with non-zero scroll values, the window is being
       scrolled with no scrollbars.
       Manually invoke the _gx_window_scroll functions to accomplish
       the requested scrolling
    */

    if (x_scroll)
    {
        if (y_scroll)
        {
            /* If we are scrolling both x and y, we need to do two
               operations for the block-move to work correctly
             */
            _gx_window_scroll(window, x_scroll, 0);
            _gx_window_scroll(window, 0, y_scroll);
        }
        else
        {
            /* just scrolling in x */
            _gx_window_scroll(window, x_scroll, 0);
        }
    }
    else
    {
        /* just scrolling in y */
        _gx_window_scroll(window, 0, y_scroll);
    }

    return GX_SUCCESS;
}

