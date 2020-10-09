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
/*    _gx_window_scroll                                   PORTABLE C      */
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
/*    _gx_widget_scroll_shift               Widget scroll shift           */
/*    _gx_widget_block_move                 Widget block move             */
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
UINT _gx_window_scroll(GX_WINDOW *window, GX_VALUE x_scroll, GX_VALUE y_scroll)
{
GX_WIDGET *child = window -> gx_widget_first_child;
GX_BOOL    has_transparent_nonclient_children = GX_FALSE;

    /* if this window is not visible than nothing to do */

    if (!(window -> gx_widget_status & GX_STATUS_VISIBLE))
    {
        return(GX_SUCCESS);
    }

    /* test to determine if transparent non-client children exist */

    while (child)
    {
        if (child -> gx_widget_status & GX_STATUS_NONCLIENT)
        {
            if (child -> gx_widget_style & GX_STYLE_TRANSPARENT)
            {
                has_transparent_nonclient_children = GX_TRUE;
                break;
            }
        }
        child = child -> gx_widget_next;
    }

    /* reset to first child */
    child = window -> gx_widget_first_child;

    while (child)
    {
        if (!(child -> gx_widget_status & GX_STATUS_NONCLIENT))
        {
            _gx_widget_scroll_shift(child, x_scroll, y_scroll, GX_TRUE);
        }
        child = child -> gx_widget_next;
    }

    if (has_transparent_nonclient_children)
    {
        /* In this case we cannot use block-move. Just shift all child widgets */
        _gx_system_dirty_partial_add((GX_WIDGET *)window, &window -> gx_window_client);
    }
    else
    {
        /* here if we can do faster scrolling using block-move function */
        _gx_widget_block_move((GX_WIDGET *)window, &window -> gx_window_client, x_scroll, y_scroll);
    }

    return(GX_SUCCESS);
}

