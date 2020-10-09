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
/**   Scrollbar Management (Scrollbar)                                    */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE

/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_scrollbar.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_scrollbar_reset                                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service resets the scrollbar.                                  */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    scrollbar                             Scrollbar control block       */
/*    info                                  Pointer to GX_SCROLL          */
/*                                            structure that defines the  */
/*                                            scrollbar limits, current   */
/*                                            value, and step or          */
/*                                            increment.                  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    [gx_window_scroll_info_get]           Get the window scroll info    */
/*    _gx_scrollbar_size_update             Update the scroll size        */
/*    _gx_scrollbar_thumb_position_calculate                              */
/*                                          Calculate the scrollbar thumb */
/*                                          position                      */
/*    _gx_system_dirty_mark                 Mark system block dirty       */
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
UINT _gx_scrollbar_reset(GX_SCROLLBAR *scrollbar, GX_SCROLL_INFO *info)
{
GX_WINDOW *win;

    if (info)
    {
        scrollbar -> gx_scrollbar_info = *info;
    }
    else
    {
        win = (GX_WINDOW *)scrollbar -> gx_widget_parent;

        if (win)
        {
            win -> gx_window_scroll_info_get(win, scrollbar -> gx_widget_style, &scrollbar -> gx_scrollbar_info);
        }
    }
    _gx_scrollbar_size_update(scrollbar);
    _gx_scrollbar_thumb_position_calculate(scrollbar);

    if (scrollbar -> gx_widget_status & GX_STATUS_VISIBLE)
    {
        _gx_system_dirty_mark((GX_WIDGET *)scrollbar);
    }

    return(GX_SUCCESS);
}

