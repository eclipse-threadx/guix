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
/*    _gx_window_view_update_detect                       PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function is called when a window is moved or changes in a way  */
/*    that causes viewports to be invalid. Check to see if this window is */
/*    a child of the root window, and if so mark the root as needing an   */
/*    update.                                                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    window                                Window's widget control block */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    none                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
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
VOID _gx_window_view_update_detect(GX_WINDOW *window)
{
GX_WINDOW_ROOT *root;
GX_WIDGET      *parent;

    /* pick up a pointer to the parent of this widget */

    if (window -> gx_widget_type >= GX_TYPE_WINDOW)
    {
        parent = window -> gx_widget_parent;

        if (parent)
        {
            /* is my parent a GX_ROOT_WINDOW type? */
            if (parent -> gx_widget_type == GX_TYPE_ROOT_WINDOW)
            {
                root = (GX_WINDOW_ROOT *)parent;
                root -> gx_window_root_views_changed = GX_TRUE;
            }
        }
        else
        {
            if (window -> gx_widget_type == GX_TYPE_ROOT_WINDOW)
            {
                root = (GX_WINDOW_ROOT *)window;
                root -> gx_window_root_views_changed = GX_TRUE;
            }
        }
    }
}

