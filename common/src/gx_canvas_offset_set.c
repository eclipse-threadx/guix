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
/*    _gx_canvas_offset_set                               PORTABLE C      */
/*                                                           6.1.11       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function assigns a canvas x,y display offset                   */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    canvas                                Canvas control block          */
/*    x                                     X coordinate of offset        */
/*    y                                     Y coordinate of offset        */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    gx_canvas_dirty_mark                  mark canvas as needing refresh*/
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_animation_start                                                 */
/*    _gx_animation_update                                                */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  04-25-2022     Ting Zhu                 Modified comment(s), fixed    */
/*                                            canvas dirty mark logic,    */
/*                                            resulting in version 6.1.11 */
/*                                                                        */
/**************************************************************************/
UINT  _gx_canvas_offset_set(GX_CANVAS *canvas, GX_VALUE xoffset, GX_VALUE yoffset)
{
GX_RECTANGLE oldpos;
GX_CANVAS   *backcanvas;

VOID (*offset_function)(INT layer, GX_VALUE x, GX_VALUE y);

    /* if there is a background canvas under this one it needs
       to be marked as dirty in the area
       this canvas is moving from
     */

    if (canvas -> gx_canvas_hardware_layer >= 0)
    {
        offset_function = canvas -> gx_canvas_display -> gx_display_layer_services -> gx_display_layer_offset_set;

        if (offset_function)
        {
            offset_function(canvas -> gx_canvas_hardware_layer, xoffset, yoffset);
            /* move the canvas display position */
            canvas -> gx_canvas_display_offset_x = xoffset;
            canvas -> gx_canvas_display_offset_y = yoffset;
            return(GX_SUCCESS);
        }
    }

    if ((canvas -> gx_canvas_status & GX_CANVAS_MANAGED) &&
        canvas -> gx_canvas_created_next)
    {
        backcanvas = canvas -> gx_canvas_created_next;

        /* find the bottom layer canvas */
        while (backcanvas -> gx_canvas_created_next)
        {
            backcanvas = backcanvas -> gx_canvas_created_next;
        }

        /* calculate rectangle bounding this canvas */

        oldpos.gx_rectangle_left  = canvas -> gx_canvas_display_offset_x;
        oldpos.gx_rectangle_top = canvas -> gx_canvas_display_offset_y;
        oldpos.gx_rectangle_right = (GX_VALUE)(oldpos.gx_rectangle_left + canvas -> gx_canvas_x_resolution - 1);
        oldpos.gx_rectangle_bottom = (GX_VALUE)(oldpos.gx_rectangle_top + canvas -> gx_canvas_y_resolution - 1);

        if (backcanvas -> gx_canvas_draw_count > 0)
        {
             _gx_utility_rectangle_combine(&oldpos, &backcanvas -> gx_canvas_dirty_area);
        }

        /* mark the background as dirty */
        _gx_canvas_dirty_mark(backcanvas, &oldpos);
    }

    /* move the canvas display position */
    canvas -> gx_canvas_display_offset_x = xoffset;
    canvas -> gx_canvas_display_offset_y = yoffset;

    /* now mark the foreground canvas as dirty and modified */
    _gx_canvas_dirty_mark(canvas, NULL);
    return GX_SUCCESS;
}

