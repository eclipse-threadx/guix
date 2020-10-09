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
/**   Widget Management (Widget)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_widget.h"
#include "gx_utility.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_widget_unlink                                   PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function unlinks a widget.                                     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    widget                                Widget control block          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    GX_WIDET *                            Widget being unlinked         */
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
GX_WIDGET *_gx_widget_unlink(GX_WIDGET *widget)
{

GX_WIDGET *sibling;
GX_WIDGET *parent;

    /* Determine if the widget has a parent.  */
    parent = widget -> gx_widget_parent;

    if (parent)
    {
        /* Is this widget the first child? */
        if (parent -> gx_widget_first_child == widget)
        {
            /* Yes, unlink and done*/
            parent -> gx_widget_first_child = widget -> gx_widget_next;
            if (parent -> gx_widget_first_child)
            {
                parent -> gx_widget_first_child -> gx_widget_previous = GX_NULL;
            }

            /* if last was also pointing at this widget, then it was the only
               child. Update last pointer: */
            if (parent -> gx_widget_last_child == widget)
            {
                parent -> gx_widget_last_child = parent -> gx_widget_first_child;
            }
        }
        else
        {
            sibling = parent -> gx_widget_first_child;

            while (sibling -> gx_widget_next != widget)
            {
                sibling = sibling -> gx_widget_next;
            }
            sibling -> gx_widget_next = widget -> gx_widget_next;

            if (parent -> gx_widget_last_child == widget)
            {
                parent -> gx_widget_last_child = sibling;
            }

            if (sibling -> gx_widget_next)
            {
                sibling -> gx_widget_next -> gx_widget_previous = sibling;
            }
        }
        if (widget -> gx_widget_status & GX_STATUS_VISIBLE)
        {
            _gx_widget_hide(widget);
        }
        widget -> gx_widget_parent = GX_NULL;
        widget -> gx_widget_next = GX_NULL;
        widget -> gx_widget_previous = GX_NULL;
        widget -> gx_widget_nav_next = GX_NULL;
        widget -> gx_widget_nav_previous = GX_NULL;
    }
    /* Return widget pointer  */
    return(widget);
}

