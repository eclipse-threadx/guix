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
/**   Dispaly Management (Dispaly)                                        */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_utility.h"
#include "gx_display.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_canvas_dirty                            PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function marks all root windows as dirty. This is done when    */
/*    the system resources are changed so that we re-draw everything.     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    display                               Display control block         */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*     _gx_system_dirty_mark                Mark the widget dirty         */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_color_set                                               */
/*    _gx_display_color_table_set                                         */
/*    _gx_display_font_table_set                                          */
/*    _gx_display_pixelmap_table_set                                      */
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
VOID _gx_display_canvas_dirty(GX_DISPLAY *display)
{
GX_WIDGET      *win;
GX_WINDOW_ROOT *root = _gx_system_root_window_created_list;

    while (root)
    {
        if (root -> gx_widget_status & GX_STATUS_VISIBLE &&
            root -> gx_window_root_canvas -> gx_canvas_display == display)
        {
            _gx_system_dirty_mark((GX_WIDGET *)root);
            win = root -> gx_widget_first_child;

            while (win)
            {
                _gx_system_dirty_mark(win);
                win = win -> gx_widget_next;
            }
        }
        root = (GX_WINDOW_ROOT *)root -> gx_widget_next;
    }
}

