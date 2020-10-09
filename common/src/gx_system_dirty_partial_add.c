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


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_system_dirty_partial_add                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service marks the partial area of this widget as dirty.        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    widget                                Pointer to widget control     */
/*                                            block                       */
/*    dirty_area                            Dirty area of widget to add   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    GX_ENTER_CRITICAL                     Enter critical section        */
/*    _gx_utility_rectangle_overlap_detect  Detect overlapping rectangles */
/*    _gx_utility_rectangle_combine         Combine two rectangles        */
/*    GX_EXIT_CRITICAL                      Exit critical section         */
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
VOID _gx_system_dirty_entry_shift(GX_WIDGET *widget, INT xShift, INT yShift)
{
UINT            index;
GX_DIRTY_AREA  *dirty_entry;
GX_WIDGET      *test;
GX_WINDOW_ROOT *root;
GX_CANVAS      *canvas;

    if (!widget)
    {
        return;
    }
    if ((widget -> gx_widget_status & (GX_STATUS_VISIBLE | GX_STATUS_DIRTY)) != (GX_STATUS_VISIBLE | GX_STATUS_DIRTY))
    {
        return;
    }

    /* lock access before linked list update */
    GX_ENTER_CRITICAL

    /* walk up to find root window */
    test = widget;

    while (test -> gx_widget_parent)
    {
        test = test -> gx_widget_parent;
    }

    if (test -> gx_widget_type != GX_TYPE_ROOT_WINDOW)
    {
        GX_EXIT_CRITICAL
        return;
    }

    root = (GX_WINDOW_ROOT *)test;

    /* pick up pointer to canvas */
    canvas = root -> gx_window_root_canvas;

    /* Setup pointer to dirty list.  */

    dirty_entry = canvas -> gx_canvas_dirty_list;

    /* Find existing entrie(s)  */
    for (index = 0; index < canvas -> gx_canvas_dirty_count; index++)
    {
        if (dirty_entry -> gx_dirty_area_widget == widget)
        {
            _gx_utility_rectangle_shift(&dirty_entry -> gx_dirty_area_rectangle, (GX_VALUE)xShift, (GX_VALUE)yShift);
        }

        /* Move to next dirty entry.  */
        dirty_entry++;
    }
    GX_EXIT_CRITICAL
}

#ifndef GUIX_STUDIO
UINT  _gx_system_dirty_partial_add(GX_WIDGET *widget, GX_RECTANGLE *dirty_area)
{

UINT            index;
GX_DIRTY_AREA  *dirty_entry;
GX_WIDGET      *test;
GX_WINDOW_ROOT *root;
GX_CANVAS      *canvas;

    if (!widget)
    {
        return GX_PTR_ERROR;
    }
    if (!(widget -> gx_widget_status & GX_STATUS_VISIBLE))
    {
        return GX_PTR_ERROR;
    }

    /* lock access before linked list update */
    GX_ENTER_CRITICAL

    /* if a widget is transparent, we actually need to mark it's
       first non-transparent parent as being dirty
     */

    while(widget -> gx_widget_parent)
    {
        if (widget -> gx_widget_status & GX_STATUS_TRANSPARENT)
        {
            widget = widget -> gx_widget_parent;
        }
        else
        {
            break;
        }
    }

    /* walk up to find root window */
    test = widget;

    while (test -> gx_widget_parent)
    {
        test = test -> gx_widget_parent;
    }

    if (test -> gx_widget_type != GX_TYPE_ROOT_WINDOW)
    {
        GX_EXIT_CRITICAL
        return GX_PTR_ERROR;
    }

    root = (GX_WINDOW_ROOT *)test;

    /* pick up pointer to canvas */
    canvas = root -> gx_window_root_canvas;

    /* if we are running low on dirty-area records, only allow
       marking the root as dirty
     */
    if (canvas -> gx_canvas_dirty_count >= GX_MAX_DIRTY_AREAS - 1)
    {
        widget = (GX_WIDGET *)root;
    }

    /* Setup pointer to dirty list.  */

    dirty_entry = canvas -> gx_canvas_dirty_list;

    /* Check to see if widget already has an entry.  */
    if (widget -> gx_widget_status & GX_STATUS_DIRTY)
    {
        for (index = 0; index < canvas -> gx_canvas_dirty_count; index++)
        {
            /* Is the same widget is present. */
            if (dirty_entry -> gx_dirty_area_widget == widget)
            {
                /* if rectangles overlap or if we are running low on dirty area,
                   records, combine them rather than adding another entry
                 */
                if (canvas -> gx_canvas_dirty_count >= GX_MAX_DIRTY_AREAS - 1 ||
                    _gx_utility_rectangle_overlap_detect(&dirty_entry -> gx_dirty_area_rectangle,
                                                         dirty_area, GX_NULL))
                {
                    /* Combine the dirty rectangles.  */
                    _gx_utility_rectangle_combine(&dirty_entry -> gx_dirty_area_rectangle, dirty_area);

                    /* Return success.  */
                    GX_EXIT_CRITICAL
                    return(GX_SUCCESS);
                }
            }

            /* Move to next dirty entry.  */
            dirty_entry++;
        }
    }

    /* If we get here, we didn't find an entry for this caller. Add a new entry.  */

    /* Are there more dirty entries?  */
    if (canvas -> gx_canvas_dirty_count < GX_MAX_DIRTY_AREAS)
    {
        /* Yes, more dirty entries, add this one!  */
        dirty_entry = &canvas -> gx_canvas_dirty_list[canvas -> gx_canvas_dirty_count];
        canvas -> gx_canvas_dirty_count++;
        dirty_entry -> gx_dirty_area_widget =    widget;
        dirty_entry -> gx_dirty_area_rectangle = *dirty_area;
        widget -> gx_widget_status |= GX_STATUS_DIRTY;
    }

    /* end of critical section */
    GX_EXIT_CRITICAL

    /* Return success. */
    return(GX_SUCCESS);
}

#endif

