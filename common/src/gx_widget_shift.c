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
/*    _gx_widget_shift_helper                             PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to shift one widget and optionally marks   */
/*    it as dirty.                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    widget                                Pointer to widget             */
/*    x_shift                               Number of pixels to shift on  */
/*                                            the x-axis                  */
/*    y_shift                               Number of pixels to shift on  */
/*                                            the y-axis                  */
/*    mark_dirty                            GX_TRUE to indicate dirty,    */
/*                                            otherwise GX_FALSE          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_rectangle_shift           Adjust a rectangle            */
/*    _gx_utility_rectangle_combine         Combine rectangles            */
/*    _gx_system_dirty_partial_add          Add area to the dirty list    */
/*    _gx_widget_shift                      Shift a widget                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_widget_shift                                                    */
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
static VOID _gx_widget_shift_helper(GX_WIDGET *widget, GX_VALUE x_shift, GX_VALUE y_shift, GX_BOOL mark_dirty)
{
GX_WINDOW   *win;
GX_RECTANGLE area_sum;
GX_RECTANGLE newsize;
GX_EVENT     new_event;

    newsize = widget -> gx_widget_size;
    _gx_utility_rectangle_shift(&newsize, x_shift, y_shift);

    if (mark_dirty && (widget -> gx_widget_status & GX_STATUS_VISIBLE))
    {
        /* here if my new size is smaller or in a different position,
           I need to mark my parent as dirty
         */
        area_sum = newsize;
        _gx_utility_rectangle_combine(&area_sum, &widget -> gx_widget_size);
        _gx_system_dirty_partial_add(widget -> gx_widget_parent, &area_sum);
    }

    widget -> gx_widget_size = newsize;

    if (widget -> gx_widget_type >= GX_TYPE_WINDOW)
    {
        win = (GX_WINDOW *)widget;
        _gx_utility_rectangle_shift(&win -> gx_window_client, x_shift, y_shift);

        if (win -> gx_widget_status & GX_STATUS_VISIBLE)
        {
            /* check to see if viewports need to be updated */
            _gx_window_view_update_detect(win);
        }
    }

    if ((widget -> gx_widget_status & GX_STATUS_VISIBLE) &&
        (widget -> gx_widget_status & GX_STATUS_RESIZE_NOTIFY))
    {
        /* Notify widget of size change. */
        memset(&new_event, 0, sizeof(GX_EVENT));
        new_event.gx_event_target = widget;
        new_event.gx_event_type = GX_EVENT_RESIZED;
        _gx_system_event_fold(&new_event);
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_widget_shift                                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service shifts the widget and optionally marks it as dirty.    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    widget                                Pointer to widget             */
/*    x_shift                               Number of pixels to shift on  */
/*                                            the x-axis                  */
/*    y_shift                               Number of pixels to shift on  */
/*                                            the y-axis                  */
/*    mark_dirty                            GX_TRUE to indicate dirty,    */
/*                                            otherwise GX_FALSE          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_shift_helper               Shift a widget                */
/*    _gx_widget_clipping_update            Update widget clipping area   */
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
UINT  _gx_widget_shift(GX_WIDGET *widget, GX_VALUE x_shift, GX_VALUE y_shift, GX_BOOL mark_dirty)
{
GX_WIDGET *child;

    if (x_shift == 0 && y_shift == 0)
    {
        return GX_SUCCESS;
    }

    _gx_widget_shift_helper(widget, x_shift, y_shift, mark_dirty);

    /* pick up pointer to first child widget */
    child = widget -> gx_widget_first_child;

    /* loop through and shift all my child widgets */

    while (child)
    {
        _gx_widget_shift_helper(child, x_shift, y_shift, GX_FALSE);

        if (child -> gx_widget_first_child)
        {
            child = child -> gx_widget_first_child;
            continue;
        }

        while ((child -> gx_widget_next == GX_NULL) && (child != widget))
        {
            child = child -> gx_widget_parent;
        }

        if (child == widget)
        {
            break;
        }

        child = child -> gx_widget_next;
    }

    if (mark_dirty && (widget -> gx_widget_status & GX_STATUS_VISIBLE))
    {
        _gx_widget_clipping_update(widget);
    }

    return(GX_SUCCESS);
}

