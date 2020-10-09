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
/**   Canvas Management (Canvas)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_utility.h"
#include "gx_canvas.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_canvas_composite_create                         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks to see if we have a composite canvas created,  */
/*    and if we have an overlay canvas visible. If both are true, the     */
/*    function invokes display driver functions to build the composite    */
/*    canvas and returns a pointer to the resulting composite.            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    return_composite                      Return pointer                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    TRUE/FALSE                            composite created             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    [gx_display_driver_canvas_copy]       copy canvas                   */
/*    [gx_display_driver_canvas_blend]      blend canvas                  */
/*    gx_utility_rectangle_shift            move a rectangle              */
/*    gx_utility_rectangle_combine          combine (add) two rectangles  */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_system_canvas_refresh                                           */
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
GX_BOOL  _gx_canvas_composite_create(GX_CANVAS **return_composite)
{
GX_CANVAS   *composite = GX_NULL;
GX_CANVAS   *canvas;
GX_CANVAS   *last = GX_NULL;
INT          canvas_count = 0;
INT          dirty_canvas_count = 0;
GX_RECTANGLE invalid_sum;
GX_RECTANGLE dirty;

    /* pick up start of canvas list */
    canvas = _gx_system_canvas_created_list;

    /* initialize the invalid_sum to a NULL rectangle */

    invalid_sum.gx_rectangle_left = GX_VALUE_MAX;
    invalid_sum.gx_rectangle_right = -1;
    invalid_sum.gx_rectangle_top = GX_VALUE_MAX;
    invalid_sum.gx_rectangle_bottom = -1;

    /* look for a composite canvas and multiple visible managed cavases.
       Sum the the invalid areas along the way */

    while (canvas)
    {
        /* is this canvas the composite? */
        if (canvas -> gx_canvas_status & GX_CANVAS_COMPOSITE)
        {
            /* yes, we found the canvas on which to build composite */
            composite = canvas;
        }
        else
        {
            /* is this a visible managed canvas? */
            if ((canvas -> gx_canvas_status & GX_CANVAS_MANAGED_VISIBLE) == GX_CANVAS_MANAGED_VISIBLE)
            {
                /* yes, add this canvas invalid area to our sum */
                if (canvas -> gx_canvas_draw_count > 0)
                {
                    dirty = canvas -> gx_canvas_dirty_area;
                    _gx_utility_rectangle_shift(&dirty, canvas -> gx_canvas_display_offset_x,
                                                canvas -> gx_canvas_display_offset_y);
                    _gx_utility_rectangle_combine(&invalid_sum, &dirty);
                    dirty_canvas_count++;
                }
                /* increment number of visible canvases */
                canvas_count++;
            }
        }
        /* keep pointer to last canvas */
        last = canvas;

        /* move to the next */
        canvas = canvas -> gx_canvas_created_next;
    }

    /* if we have found both a comosite canvas a multiple visible managed canvases,
       then build the composite */

    if (composite && canvas_count > 1)
    {
        if (dirty_canvas_count == 0)
        {
            /* nothing has changed, just return */
            *return_composite = composite;
            composite -> gx_canvas_draw_count = 0;
            return GX_TRUE;
        }

        /* make sure the invalid left is not off the screen */
        if (invalid_sum.gx_rectangle_left < 0)
        {
            invalid_sum.gx_rectangle_left = 0;
        }
        /* make sure the invalid top is not off the screen */
        if (invalid_sum.gx_rectangle_top < 0)
        {
            invalid_sum.gx_rectangle_top = 0;
        }
        /* make sure the invalid right is not off the screen */
        if (invalid_sum.gx_rectangle_right >= composite -> gx_canvas_x_resolution)
        {
            invalid_sum.gx_rectangle_right = (GX_VALUE)(composite -> gx_canvas_x_resolution - 1);
        }
        /* make sure the invalid bottom is not off the screen */
        if (invalid_sum.gx_rectangle_bottom >= composite -> gx_canvas_y_resolution)
        {
            invalid_sum.gx_rectangle_bottom = (GX_VALUE)(composite -> gx_canvas_y_resolution - 1);
        }

        /* save the invalid_sum into composite dirty rectangle */
        composite -> gx_canvas_dirty_area = invalid_sum;

        /* start at the last (lowest) canvas */
        canvas = last;
        canvas_count = 0;

        /* copy canvases into composite, moving back to front */
        while (canvas)
        {
            /* is this a visible, managed, overlay canvas? */
            if ((canvas -> gx_canvas_status & GX_CANVAS_MANAGED_VISIBLE) == GX_CANVAS_MANAGED_VISIBLE &&
                ((canvas -> gx_canvas_status & GX_CANVAS_COMPOSITE) == 0))
            {
                /* if this canvas is first to draw into composite, or if this canvas
                   is not blended, copy it into the composite
                 */
                if (canvas_count == 0 || canvas -> gx_canvas_alpha == GX_ALPHA_VALUE_OPAQUE)
                {
                    /* copy the canvas data */
                    canvas -> gx_canvas_display -> gx_display_driver_canvas_copy(canvas, composite);
                }
                else
                {
                    /* here if the canvas is not first and not opaque. Blend it into
                       composite unless it is fully transparent
                     */
                    if ((canvas->gx_canvas_alpha != 0) && (canvas->gx_canvas_display -> gx_display_driver_canvas_blend))
                    {
                        /* blend the canvas data */
                        canvas -> gx_canvas_display -> gx_display_driver_canvas_blend(canvas, composite);
                    }
                }
                canvas_count++;
            }
            canvas -> gx_canvas_dirty_count = 0;
            canvas -> gx_canvas_draw_count = 0;
            canvas = canvas -> gx_canvas_created_previous;
        }

        /* return pointer to composite */
        *return_composite = composite;
        composite -> gx_canvas_draw_count = (UINT)dirty_canvas_count;
        return GX_TRUE;
    }

    *return_composite = GX_NULL;
    return GX_FALSE;
}

