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
/*    _gx_canvas_pixelmap_tile                            PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function prepares to draw the specified pixelmap at the        */
/*    requested position.                                                 */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    fill                                  Area to fill with pixelmap    */
/*    pixelmap                              Pointer to actual pixelmap    */
/*                                            to draw                     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_rectangle_overlap_detect  Detect rectangle overlap      */
/*    _gx_canvas_pixelmap_draw              Screen pixelmap draw routine  */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
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
UINT  _gx_canvas_pixelmap_tile(GX_RECTANGLE *fill, GX_PIXELMAP *pixelmap)
{
GX_VALUE         xpos;
GX_VALUE         ypos;
GX_DRAW_CONTEXT *context;
GX_RECTANGLE     oldclip;

    /* pick up the current drawing context */
    context = _gx_system_current_draw_context;

    /* save the current dirty area */
    oldclip = context -> gx_draw_context_dirty;

    /* clip to the fill rectangle */
    if (!_gx_utility_rectangle_overlap_detect(fill, &context -> gx_draw_context_dirty,
                                              &context -> gx_draw_context_dirty))
    {
        context -> gx_draw_context_dirty = oldclip;

        /* nothing to draw, return */
        return GX_SUCCESS;
    }

    if (!pixelmap -> gx_pixelmap_width ||
        !pixelmap -> gx_pixelmap_height)
    {
        context -> gx_draw_context_dirty = oldclip;
        return GX_FAILURE;
    }

    /* pickup starting y position */
    ypos = fill -> gx_rectangle_top;

    /* loop from top to bottom */
    while (ypos <= fill -> gx_rectangle_bottom)
    {
        /* pickup starting x position */
        xpos = fill -> gx_rectangle_left;

        /* loop from left to right */
        while (xpos <= fill -> gx_rectangle_right)
        {
            /* draw one tile */
            _gx_canvas_pixelmap_draw(xpos, ypos, pixelmap);

            /* move to the right by one tile width */
            xpos = (GX_VALUE)(xpos + pixelmap -> gx_pixelmap_width);
        }

        /* move down by one tile height */
        ypos = (GX_VALUE)(ypos + pixelmap -> gx_pixelmap_height);
    }

    /* restore dirty rectangle */
    context -> gx_draw_context_dirty = oldclip;

    /* Return successful completion.  */

    return(GX_SUCCESS);
}

