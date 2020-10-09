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
#include "gx_window.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_widget_front_move                               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function moves the widget to the front.                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    widget                                Pointer to widget to move     */
/*    return_moved                          Pointer to destination for    */
/*                                            indication widget was moved */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_dirty_partial_add          Add dirty area                */
/*    _gx_utility_rectangle_combine         Combine rectangles            */
/*    _gx_utility_rectangle_define          Define rectangle              */
/*    _gx_utility_rectangle_overlap_detect  Check for overlap             */
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
UINT  _gx_widget_front_move(GX_WIDGET *widget, GX_BOOL *return_moved)
{
GX_WIDGET   *parent;
GX_WIDGET   *sibling;
GX_RECTANGLE dirty_area;
GX_RECTANGLE overlap;

    /* Pickup parent widget.  */
    parent =  widget -> gx_widget_parent;

    /* Is parent valid?  */
    if (!parent)
    {

        /* Return error.  */
        return(GX_PTR_ERROR);
    }

    /* First check to see if the widget is already in front.  */
    if (parent -> gx_widget_last_child == widget)
    {

        /* Yes, widget is already in front, so nothing to do.  */

        /* Return no change.  */
        return(GX_NO_CHANGE);
    }

    /* Relink widget to the end
        1) Determine what to dirty, all or partial
        2) unlink and stitch linked list
        3) relink to the end
        4) call dirty so that I will get redraw
     */
    _gx_utility_rectangle_define(&dirty_area, GX_VALUE_MAX, GX_VALUE_MAX, -1, -1);

    /* Pickup sibling widget.  */
    sibling =  widget -> gx_widget_next;

    /* Traverse the sibling list.  */
    while (sibling)
    {
        /* Check for an overlap of siblings.  */
        if (_gx_utility_rectangle_overlap_detect(&widget -> gx_widget_size, &sibling -> gx_widget_size, &overlap))
        {
            /* Yes, calculate the dirty area.  */
            if (dirty_area.gx_rectangle_left > dirty_area.gx_rectangle_right)
            {
                dirty_area = overlap;
            }
            else
            {
                _gx_utility_rectangle_combine(&dirty_area, &overlap);
            }
        }

        /* Move to next sibling.  */
        sibling =  sibling -> gx_widget_next;
    }


    if (dirty_area.gx_rectangle_left <= dirty_area.gx_rectangle_right)
    {

        /* Add dirty area.  */
        _gx_system_dirty_partial_add(widget, &dirty_area);
    }

    /* Is widget the first child of it's parent?  */
    if (parent -> gx_widget_first_child == widget)
    {
        /* Yes, the first child, easy remove the first child.  */
        parent -> gx_widget_first_child =  widget -> gx_widget_next;
        widget -> gx_widget_next -> gx_widget_previous =  NULL;
    }
    else
    {
        /* No, not the first child. Remove from the middle.  */
        widget -> gx_widget_previous -> gx_widget_next =  widget -> gx_widget_next;
        widget -> gx_widget_next -> gx_widget_previous =  widget -> gx_widget_previous;
    }

    /* Link the widget to the end of the list.  */

    sibling  =  parent -> gx_widget_last_child;
    sibling -> gx_widget_next =  widget;
    widget  -> gx_widget_previous =  sibling;
    widget  -> gx_widget_next =  NULL;
    parent  -> gx_widget_last_child =  widget;

    if (widget -> gx_widget_type >= GX_TYPE_WINDOW)
    {
        /* if parent is root window, then viewports need to be updated */
        _gx_window_view_update_detect((GX_WINDOW *)widget);

        /* if window accepts focus and parent has focus, focus should be moved */
        if (parent -> gx_widget_status & GX_STATUS_HAS_FOCUS &&
            (widget -> gx_widget_status & GX_STATUS_ACCEPTS_FOCUS) &&
            !(widget -> gx_widget_status & GX_STATUS_HAS_FOCUS))
        {
            _gx_system_focus_claim(widget);
        }
    }


    /* Indicate the widget was moved.  */

    if (return_moved)
    {
        *return_moved =  GX_TRUE;
    }

    /* Return successful completion.  */
    return(GX_SUCCESS);
}

