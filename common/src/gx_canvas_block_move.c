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
#include "gx_display.h"
#include "gx_window.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_canvas_block_move.c                             PORTABLE C      */
/*                                                           6.3.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function copies a block of pixel from one reference position   */
/*    to another.                                                         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    block                                 Rectangle to move             */
/*    x_shift                               Distance to move in x         */
/*                                            direction                   */
/*    y_shift                               Distance to move in y         */
/*                                            direction                   */
/*    dirty                                 Flag of dirty block           */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_rectangle_inside_detect   Detect if a second rectangle  */
/*                                            is completely within the    */
/*                                            first one                   */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*    _gx_window_client_scroll                                            */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  10-31-2023     Ting Zhu                 Modified comment(s),          */
/*                                            added canvas status check,  */
/*                                            resulting in version 6.3.0  */
/*                                                                        */
/**************************************************************************/
UINT _gx_canvas_block_move(GX_RECTANGLE *block, GX_VALUE x_shift, GX_VALUE y_shift, GX_RECTANGLE *dirty)
{
GX_DRAW_CONTEXT *context;
GX_DISPLAY      *display;
GX_VIEW         *view;
UINT             status = GX_FAILURE;


    /* pick up the current drawing context */
    context = _gx_system_current_draw_context;

    /* If the caller hasn't opened the canvas, we can't do anything */
    if (!context)
    {
        return status;
    }

    /* pick up current display driver */
    display = context -> gx_draw_context_display;

    /* check to see if this driver supports block move */
    if (!display -> gx_display_driver_block_move || (context -> gx_draw_context_canvas -> gx_canvas_status & GX_CANVAS_PARTIAL_FRAME_BUFFER))
    {
        /* this driver doesn't support block move. If we are
           partial drawing just mark the caller as dirty so that
           a normal redraw operation occurs.
         */
        return status;
    }

    /* test to determine if any viewport of the caller contains the block to move.
       If yes, we can do the block move. If no, we have to
       do a normal invalidation and redraw.
     */
    view = context -> gx_draw_context_view_head;

    while (view)
    {
        if (_gx_utility_rectangle_inside_detect(&view -> gx_view_rectangle, block))
        {
            context -> gx_draw_context_clip = block;
            display -> gx_display_driver_block_move(context, block, x_shift, y_shift);
            status = GX_SUCCESS;
            break;
        }
        view = view -> gx_view_next;
    }

    if (status == GX_SUCCESS && dirty)
    {
        /* we were able to aid drawing by doing a block move. Mark
            the remainder that was not updated as dirty so that the
            caller can redraw that portion
         */
        *dirty = *block;

        if (x_shift)
        {
            if (x_shift > 0)
            {
                dirty -> gx_rectangle_right = (GX_VALUE)(dirty -> gx_rectangle_left + x_shift - 1);
            }
            else
            {
                dirty -> gx_rectangle_left = (GX_VALUE)(dirty -> gx_rectangle_right + x_shift + 1);
            }
        }
        else
        {
            if (y_shift > 0)
            {
                dirty -> gx_rectangle_bottom = (GX_VALUE)(dirty -> gx_rectangle_top + y_shift - 1);
            }
            else
            {
                dirty -> gx_rectangle_top = (GX_VALUE)(dirty -> gx_rectangle_bottom + y_shift + 1);
            }
        }
    }
    return(status);
}

