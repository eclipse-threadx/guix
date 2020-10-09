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
/*    _gx_widget_back_link                                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function links a widget to the back of the parent child list   */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    parent                                Parent widget control block   */
/*    widget                                Widget control block          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_unlink                     Unlink the widget             */
/*    _gx_widget_show                       Show the widget               */
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
VOID _gx_widget_back_link(GX_WIDGET *parent, GX_WIDGET *widget)
{
GX_WIDGET *sibling;

    /* Determine if a parent widget was provided.  */
    if (parent)
    {
        /* Yes, is there already a child for this parent?  */
        if (parent -> gx_widget_first_child)
        {
            /* Yes, find the last child.  */
            sibling =  parent -> gx_widget_first_child;
            widget -> gx_widget_next = sibling;
            sibling -> gx_widget_previous = widget;
        }
        else
        {
            widget -> gx_widget_next = NULL;
            parent -> gx_widget_last_child = widget;
        }

        parent -> gx_widget_first_child =  widget;

        /* Setup the last child pointer and remember the parent widget.  */
        widget -> gx_widget_parent = parent;

        if (parent -> gx_widget_status & GX_STATUS_VISIBLE)
        {
            _gx_widget_show(widget);
            if (widget -> gx_widget_status & GX_STATUS_ACCEPTS_FOCUS)
            {
                _gx_widget_nav_order_initialize(parent);
            }
        }
    }
}

