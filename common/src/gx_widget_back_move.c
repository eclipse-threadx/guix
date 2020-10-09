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
#include "gx_utility.h"
#include "gx_widget.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_widget_back_move                                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function moves a widget to the back in the Z order.            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    widget                                Widget control block          */
/*    widget_moved                          Return flag indicate widget   */
/*                                            moved                       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_dirty_partial_add          Add dirty area                */
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
UINT  _gx_widget_back_move(GX_WIDGET *widget, GX_BOOL *widget_moved)
{
GX_WIDGET *parent;

    /* Pickup parent widget.  */
    parent = widget -> gx_widget_parent;

    /* Check for invalid parent.  */
    if (!parent)
    {
        return(GX_PTR_ERROR);
    }

    /* First check to see if the widget is already in back.  */
    if (parent -> gx_widget_first_child == widget)
    {
        /* Yes, widget is already in back, so nothing to do.  */

        /* Return no change.  */
        return(GX_NO_CHANGE);
    }

    /* Relink widget to the end
        2) unlink and stitch linked list
        3) relink to the end
        4) call dirty so that we will get redraw
     */
    _gx_system_dirty_partial_add(parent, &widget -> gx_widget_size);

    /* Is the widget last (front) child of parent?  */
    if (parent -> gx_widget_last_child == widget)
    {
        /* Yes, the first child, easy remove the first child.  */
        parent -> gx_widget_last_child =  widget -> gx_widget_previous;
        widget -> gx_widget_previous -> gx_widget_next =  NULL;
    }
    else
    {
        /* No, not the first child. Remove from the middle.  */
        widget -> gx_widget_previous -> gx_widget_next =  widget -> gx_widget_next;
        widget -> gx_widget_next -> gx_widget_previous =  widget -> gx_widget_previous;
    }

    /* Link the widget to the back (head) of the list.  */
    widget -> gx_widget_next = parent -> gx_widget_first_child;
    parent -> gx_widget_first_child -> gx_widget_previous = widget;
    widget -> gx_widget_previous = GX_NULL;
    parent -> gx_widget_first_child = widget;

    /* Indicate the widget was moved.  */

    if (widget_moved)
    {
        *widget_moved =  GX_TRUE;
    }

    /* Return successful completion.  */
    return(GX_SUCCESS);
}

