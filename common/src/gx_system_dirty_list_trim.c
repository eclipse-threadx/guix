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
/*    _gx_system_dirty_overlap_check                      PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks if it has overlapping dirty area with others.  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    dirty                                 Pointer to dirty area         */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_rectangle_overlap_detect  Detect overlap of rectangles  */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_system_dirty_list_trim            Calculate and trim dirty list */
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
static VOID _gx_system_dirty_overlap_check(GX_DIRTY_AREA *dirty)
{
GX_WIDGET      *dirty_widget;
GX_WIDGET      *sibling;
GX_WIDGET      *parent;

    dirty_widget = dirty -> gx_dirty_area_widget;
    parent = dirty_widget -> gx_widget_parent;

    while (parent)
    {
        /* check to see if this is a viewport owner */
        if (dirty -> gx_dirty_area_widget -> gx_widget_type >= GX_TYPE_WINDOW)
        {
            if (parent -> gx_widget_type == GX_TYPE_ROOT_WINDOW)
            {
                /* this widget is a view owner, no need to go further */
                return;
            }
        }

        /* not a view owner, so check to see if it has overlapping siblings */
        sibling = dirty_widget -> gx_widget_next;

        while(sibling)
        {
            if (_gx_utility_rectangle_overlap_detect(&sibling -> gx_widget_size, &dirty -> gx_dirty_area_rectangle, NULL))
            {
                /* mark the parent as dirty, so that both children are drawn */
                dirty_widget = parent;
                dirty -> gx_dirty_area_widget = dirty_widget;
                break;
            }
            sibling = sibling->gx_widget_next;
        }
        dirty_widget = parent;
        parent = dirty_widget -> gx_widget_parent;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_system_dirty_list_trim                          PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function trims the dirty list to reduce redundancy and         */
/*    eliminate invalid entries prior to refreshing the canvas. This      */
/*    function returns the sum of the remaining dirty areas               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    dirty_area                            Pointer to return value       */
/*    root                                  Point to the root window      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    [GX_TRUE | GX_FALSE]                  GX_TRUE if dirty, otherwise   */
/*                                            GX_FALSE                    */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_rectangle_combine         Combine two rectangles        */
/*    _gx_utility_rectangle_define          Define a rectangle            */
/*    _gx_utility_rectangle_overlap_detect  Detect overlap of rectangles  */
/*    _gx_system_dirty_overlap_check        Detect overlapping children   */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_system_canvas_refresh             Refresh the canvas            */
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
UINT  _gx_system_dirty_list_trim(GX_RECTANGLE *dirty_area, GX_WINDOW_ROOT *root)
{

UINT           index_1;
UINT           index_2;
GX_DIRTY_AREA *dirty;
GX_DIRTY_AREA *dirty2;
GX_CANVAS     *canvas;
GX_WIDGET     *parent;

    /* pick up pointer to canvas */
    canvas = root -> gx_window_root_canvas;

    /* Initialize a dirty area rectangle.  */
    _gx_utility_rectangle_define(dirty_area, canvas -> gx_canvas_x_resolution,
                                 canvas -> gx_canvas_y_resolution, -1, -1);

    /* Setup pointer to start of dirty area list.  */
    dirty =  canvas -> gx_canvas_dirty_list;

    /* This loop looks for invalid entries (invisible) and 
       entries that have overlappers in the z order, in
       which case we need to back up the drawing layer
     */

    for (index_1 = 0; index_1 < canvas -> gx_canvas_dirty_count; index_1++)
    {
        /* Is there a widget for this dirty area entry?  */
        if (dirty -> gx_dirty_area_widget == GX_NULL)
        {
            dirty++;
            continue;
        }

        if (!(dirty -> gx_dirty_area_widget -> gx_widget_status & GX_STATUS_VISIBLE))
        {
            dirty -> gx_dirty_area_widget = GX_NULL;
            dirty++;
            continue;
        }

        /* check to see if we need to modify the drawing level because of overlapping
           siblings
        */
         _gx_system_dirty_overlap_check(dirty);
        dirty++;
    }

    /* This loop tests to see if a dirty entry also has a parent
       that is marked as dirty. If the dirty rectangles overlap,
       just combine the rectangles and get rid of the child entry
     */

    dirty =  canvas -> gx_canvas_dirty_list;

    for (index_1 = 0; index_1 < canvas -> gx_canvas_dirty_count; index_1++)
    {
        /* Is there a widget for this dirty area entry?  */
        if (dirty -> gx_dirty_area_widget == GX_NULL)
        {
            dirty++;
            continue;
        }

        /* Yes, there is a widget. Loop through the dirty area array.  */
        for (index_2 = 0; index_2 < canvas -> gx_canvas_dirty_count; index_2++)
        {
            /* Pickup the next dirty area entry.  */
            dirty2 = &canvas -> gx_canvas_dirty_list[index_2];

            /* Is is valid?  */
            if ((dirty2 -> gx_dirty_area_widget) && (dirty != dirty2))
            {
                parent = dirty2 -> gx_dirty_area_widget -> gx_widget_parent;

                while (parent && (parent != dirty -> gx_dirty_area_widget))
                {
                    parent = parent -> gx_widget_parent;
                }

                /* check to see if one is parent of the other */
                if (parent == dirty -> gx_dirty_area_widget)
                {
                    /* dirty1 is parent of dirty2, do the dirty
                        rectangles overlap?  */
                    if (_gx_utility_rectangle_overlap_detect(&dirty -> gx_dirty_area_rectangle, &dirty2 -> gx_dirty_area_rectangle, NULL))
                    {
                        /* Yes, combine the two entries into one and NULL out the child entry.  */
                        _gx_utility_rectangle_combine(&dirty -> gx_dirty_area_rectangle, &dirty2 -> gx_dirty_area_rectangle);

                        /* NULL out the child entry.  */
                        dirty2 -> gx_dirty_area_widget = GX_NULL;
                        continue;
                    }
                }

                /* check to see if two entries for same widget */
                if (dirty2 -> gx_dirty_area_widget == dirty -> gx_dirty_area_widget)
                {
                    /* same widget marked twice */
                    /* do the dirty rectangles overlap?  */
                    if (_gx_utility_rectangle_overlap_detect(&dirty -> gx_dirty_area_rectangle, &dirty2 -> gx_dirty_area_rectangle, NULL))
                    {
                        /* Yes, combine the two entries into one and NULL out the child entry.  */
                        _gx_utility_rectangle_combine(&dirty -> gx_dirty_area_rectangle, &dirty2 -> gx_dirty_area_rectangle);

                        /* NULL out the second entry.  */
                        dirty2 -> gx_dirty_area_widget =  GX_NULL;
                    }
                }
            }
        }

        /* Add to our overall sum of dirty area.  */
        _gx_utility_rectangle_combine(dirty_area, &(dirty -> gx_dirty_area_rectangle));

        /* Move to next dirty area entry.  */
        dirty++;
    }

    /* Do we have anything to draw?  */
    if ((dirty_area -> gx_rectangle_left <= dirty_area -> gx_rectangle_right) &&
        (dirty_area -> gx_rectangle_top <= dirty_area -> gx_rectangle_bottom))
    {

        /* Yes, we have something to draw.  */
        return(GX_TRUE);
    }
    else
    {

        /* No, nothing left after trimming duplicates.  */
        return(GX_FALSE);
    }
}

