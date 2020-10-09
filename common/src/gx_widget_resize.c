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
/*    _gx_widget_resize                                   PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function  resizes the widget.                                  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    widget                                Pointer to widget             */
/*    new_size                              New widget size               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_rectangle_compare          Compares two rectangles      */
/*    _gx_utility_rectangle_inside_detect    Detect if a second rectangle */
/*                                             is completely within the   */
/*                                             first one                  */
/*    _gx_system_dirty_partial_add           Add dirty area               */
/*    _gx_utility_rectangle_combine          Combines two rectangles      */
/*    _gx_widget_client_get                  Retrieves client area        */
/*    _gx_widget_shift                       Changes the position of      */
/*                                             a widget                   */
/*    _gx_widget_clipping_update             Update the clipping members  */
/*                                             of each child widget       */
/*    _gx_widget_event_process_function      Indirect call to widget      */
/*                                             event processing function  */
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
UINT  _gx_widget_resize(GX_WIDGET *widget, GX_RECTANGLE *new_size)
{
GX_WIDGET      *child;
GX_WINDOW      *win;
GX_WINDOW_ROOT *root;
GX_RECTANGLE    area_sum;
GX_VALUE        LeftShift;
GX_VALUE        TopShift;
GX_VALUE        RightShift;
GX_VALUE        BottomShift;
GX_EVENT        new_event;

    /* first, check to see if the old and new size are the same */

    if (_gx_utility_rectangle_compare(&widget -> gx_widget_size, new_size))
    {
        return GX_SUCCESS;
    }

    if (widget -> gx_widget_status & GX_STATUS_VISIBLE)
    {
        /* If my new size encompasses my old size, then I just need to
           mark myself as dirty
         */
        if (_gx_utility_rectangle_inside_detect(new_size, &widget -> gx_widget_size))
        {
            _gx_system_dirty_partial_add(widget, new_size);
        }
        else
        {
            /* here if my new size is smaller or in a different position,
               I need to mark my parent as dirty
             */
            area_sum = *new_size;
            _gx_utility_rectangle_combine(&area_sum, &widget -> gx_widget_size);
            _gx_system_dirty_partial_add(widget -> gx_widget_parent, &area_sum);
        }
    }

    /* keep track of how far I am shifting */
    LeftShift = (GX_VALUE)(new_size -> gx_rectangle_left - widget -> gx_widget_size.gx_rectangle_left);
    TopShift = (GX_VALUE)(new_size -> gx_rectangle_top - widget -> gx_widget_size.gx_rectangle_top);
    RightShift = (GX_VALUE)(new_size -> gx_rectangle_right - widget -> gx_widget_size.gx_rectangle_right);
    BottomShift = (GX_VALUE)(new_size -> gx_rectangle_bottom - widget -> gx_widget_size.gx_rectangle_bottom);

    /* reset my size */
    widget -> gx_widget_size = *new_size;

    /* if I am a window, update my client area */

    if (widget -> gx_widget_type >= GX_TYPE_WINDOW)
    {
        win = (GX_WINDOW *)widget;
        win -> gx_window_client.gx_rectangle_left   = (GX_VALUE)(win -> gx_window_client.gx_rectangle_left + LeftShift);
        win -> gx_window_client.gx_rectangle_top    = (GX_VALUE)(win -> gx_window_client.gx_rectangle_top + TopShift);
        win -> gx_window_client.gx_rectangle_right  = (GX_VALUE)(win -> gx_window_client.gx_rectangle_right + RightShift);
        win -> gx_window_client.gx_rectangle_bottom = (GX_VALUE)(win -> gx_window_client.gx_rectangle_bottom + BottomShift);

        /* if parent is root window, then viewports need to be updated */
        if (widget -> gx_widget_parent)
        {
            if (widget -> gx_widget_parent -> gx_widget_type == GX_TYPE_ROOT_WINDOW)
            {
                root = (GX_WINDOW_ROOT *)widget -> gx_widget_parent;
                root -> gx_window_root_views_changed = GX_TRUE;
            }
        }
    }

    /* if my top-left shifted, I need to move my child widgets */

    if (LeftShift || TopShift)
    {
        /* pick up pointer to first child widget */
        child = widget -> gx_widget_first_child;

        /* loop through and shift all my child widgets */

        while (child)
        {
            _gx_widget_shift(child, LeftShift, TopShift, GX_FALSE);
            child = child -> gx_widget_next;
        }
    }

    if (widget -> gx_widget_type >= GX_TYPE_WINDOW)
    {
        /* tell all my child widgets that I have been re-sized */
        new_event.gx_event_type = GX_EVENT_PARENT_SIZED;
        child = widget -> gx_widget_first_child;

        while (child)
        {
            child -> gx_widget_event_process_function(child, &new_event);
            child = child -> gx_widget_next;
        }
    }

    if (widget -> gx_widget_status & GX_STATUS_VISIBLE)
    {

        /* update this widget clip rectangle */
        _gx_widget_clipping_update(widget);

        if (widget -> gx_widget_status & GX_STATUS_RESIZE_NOTIFY)
        {
            /* Notify widget of size change.  */
            memset(&new_event, 0, sizeof(GX_EVENT));
            new_event.gx_event_target = widget;
            new_event.gx_event_type = GX_EVENT_RESIZED;
            _gx_system_event_fold(&new_event);
        }
    }
    return(GX_SUCCESS);
}

