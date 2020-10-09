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
/*    _gx_widget_scroll_shift_helper                      PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to change the position of a widget.        */
/*    It also adds a dirty entry if required.                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    widget                                Widget control block          */
/*    xShift                                The x-axis shift amount       */
/*    yShift                                The y-axis shift amount       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_rectangle_shift           Shift a rectangle             */
/*    _gx_system_dirty_entry_shift          Mark partial widget area dirty*/
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_widget_scroll_shift                                             */
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
static VOID _gx_widget_scroll_shift_helper(GX_WIDGET *widget, INT xShift, INT yShift)
{
GX_WINDOW *win;

    _gx_utility_rectangle_shift(&widget -> gx_widget_size, (GX_VALUE)xShift, (GX_VALUE)yShift);

    if (widget -> gx_widget_type >= GX_TYPE_WINDOW)
    {
        win = (GX_WINDOW *)widget;
        _gx_utility_rectangle_shift(&win -> gx_window_client, (GX_VALUE)xShift, (GX_VALUE)yShift);
    }

    if (widget -> gx_widget_status & GX_STATUS_DIRTY)
    {
        /* mark it dirty again so that the new position is recorded */
        _gx_system_dirty_entry_shift(widget, xShift, yShift);
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_widget_scroll_shift                             PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function changes the position of a widget. It also adds        */
/*    a dirty entry if required                                           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    widget                                Widget control block          */
/*    xShift                                The x-axis shift amount       */
/*    yShift                                The y-axis shift amount       */
/*    clip                                  Flag, should we update child  */
/*                                          widget clipping areas.        */
/*                                                                        */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_scroll_shift_helper        Scroll a widget               */
/*    _gx_widget_clipping_update            Update the clipping area      */
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
UINT  _gx_widget_scroll_shift(GX_WIDGET *widget, INT xShift, INT yShift, GX_BOOL clip)
{
GX_WIDGET *child;

    /* first, check to see if the old and new size are the same */

    if (xShift == 0 && yShift == 0)
    {
        return GX_SUCCESS;
    }

    _gx_widget_scroll_shift_helper(widget, xShift, yShift);

    /* pick up pointer to first child widget */
    child = widget -> gx_widget_first_child;

    /* loop through and shift all my child widgets */
    while (child)
    {
        _gx_widget_scroll_shift_helper(child, xShift, yShift);

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

    if (clip && (widget -> gx_widget_status & GX_STATUS_VISIBLE))
    {
        _gx_widget_clipping_update(widget);
    }

    return(GX_SUCCESS);
}

