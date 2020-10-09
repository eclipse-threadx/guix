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
/**   System Management (System)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_utility.h"
#include "gx_widget.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_system_root_view_add                            PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Adds a view to a root window, checking to detect if the view is     */
/*    obscured by a child window                                          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    root                                  Root window that we want to   */
/*                                            add the view to.            */
/*    inrect                                Rectangle defining the view   */
/*                                            that we want to add.        */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    view is added to root window or split into smaller views.           */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_view_split                 Split a view                  */
/*    _gx_system_view_add                   Add view to window            */
/*    _gx_utility_rectangle_overlap_detect  Detect overlap of rectangles  */
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
VOID _gx_system_root_view_add(GX_WINDOW_ROOT *root, GX_RECTANGLE *inrect)
{
GX_RECTANGLE overlap;
GX_WINDOW   *win = GX_NULL;
GX_WIDGET   *widget;

    /* pick up pointer to frontmost child window */
    widget = root -> gx_widget_last_child;

    while (widget)
    {
        if (widget -> gx_widget_type >= GX_TYPE_WINDOW &&
            (widget -> gx_widget_status & GX_STATUS_VISIBLE))
        {
            /* if this window is non-transparent, give it views */
            if (!(widget -> gx_widget_status & GX_STATUS_TRANSPARENT))
            {
                win = (GX_WINDOW *)widget;
                if (_gx_utility_rectangle_overlap_detect(inrect, &win -> gx_widget_size, &overlap))
                {
                    /* split the viewport into pieces */
                    _gx_system_view_split(win, root, inrect);

                    /* add the overlap to the over window: */
                    _gx_system_view_add(win, &overlap);
                    return;
                }
            }
        }
        widget = widget -> gx_widget_previous;
    }

    /* if we didn't split the view, then add it here */
    _gx_system_view_add((GX_WINDOW *)root, inrect);
}

