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
#include "gx_canvas.h"
#include "gx_utility.h"
#include "gx_widget.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_widget_children_draw                            PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service draws all children of widget.                          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    widget                                Pointer to widget             */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_canvas_drawing_complete           Complete drawing on canvas    */
/*    _gx_canvas_drawing_initiate           Initiate drawing on canvas    */
/*    _gx_utility_rectangle_overlap_detect  Check for overlap             */
/*    [gx_widget_draw_function]             Child widget drawing function */
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
VOID  _gx_widget_children_draw(GX_WIDGET *widget)
{
UINT             status;
GX_WIDGET       *child;
GX_CANVAS       *canvas;
GX_DRAW_CONTEXT *context;
GX_RECTANGLE    *dirty_area;
GX_RECTANGLE     overlap;

    /* Pickup the first child.  */
    child =   widget -> gx_widget_first_child;

    if (!child)
    {
        return;
    }

    /* pick up the current context */
    context = _gx_system_current_draw_context;

    /* pickup the current canvas */

    canvas =  context -> gx_draw_context_canvas;
    dirty_area = &(context -> gx_draw_context_dirty);

    /* Draw all the child widgets that overlap the clipping area.  */

    while (child)
    {
        if (child -> gx_widget_status & GX_STATUS_VISIBLE)
        {
            if (_gx_utility_rectangle_overlap_detect(&child -> gx_widget_clip, dirty_area, &overlap))
            {
                /* Initiate drawing on this canvas.  */
                status = _gx_canvas_drawing_initiate(canvas, child, &overlap);

                if (status == GX_SUCCESS)
                {
                    /* Yes, draw the child widget.  */
                    child -> gx_widget_draw_function(child);
                    _gx_canvas_drawing_complete(canvas, GX_FALSE);
                }
                else
                {
                    if (status == GX_NO_VIEWS)
                    {
                        /* Complete drawing on this canvas.  */
                        _gx_canvas_drawing_complete(canvas, GX_FALSE);
                    }
                }
            }
        }

        /* Move to next child.  */
        child =  child -> gx_widget_next;
    }
}

