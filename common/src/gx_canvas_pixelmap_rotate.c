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
#include "gx_display.h"
#include "gx_canvas.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_canvas_rotated_pixelmap_bound_calculate         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function calcuates the bounding rectangle of a rotated         */
/*    pixelmap.                                                           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    rect                                  Rectangle of source pixelmao. */
/*    angle                                 Angle of rotation in degrees. */
/*    rot_cx                                x-coord of rotating center    */
/*    rot_cy                                y-coord of rotating center    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
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
static UINT  _gx_canvas_rotated_pixelmap_bound_calculate(GX_RECTANGLE *rect, INT angle, INT rot_cx, INT rot_cy)
{
INT mx[4] = {-1, 1, 1, -1};
INT my[4] = {1, 1, -1, -1};
INT idxminx;
INT idxmaxx;
INT idxmaxy;
INT srcxres;
INT srcyres;
INT cosv;
INT sinv;
INT xres;
INT yres;
INT width;
INT height;
INT x;
INT y;

    angle %= 360;

    if (angle < 0)
    {
        angle += 360;
    }

    /* Calculate rectangle width and height. */
    width = (GX_VALUE)(rect -> gx_rectangle_right - rect -> gx_rectangle_left + 1);
    height = (GX_VALUE)(rect -> gx_rectangle_bottom - rect -> gx_rectangle_top + 1);

    if (angle == 0)
    {
        return GX_SUCCESS;
    }
    if (angle == 90)
    {
        GX_SWAP_VALS(width, height);

        rect -> gx_rectangle_left = (GX_VALUE)(rect -> gx_rectangle_left + rot_cx - (width - 1 - rot_cy));
        rect -> gx_rectangle_top = (GX_VALUE)(rect -> gx_rectangle_top + rot_cy - rot_cx);
    }
    else if (angle == 180)
    {
        rect -> gx_rectangle_left = (GX_VALUE)(rect -> gx_rectangle_left + rot_cx - (width - 1 - rot_cx));
        rect -> gx_rectangle_top = (GX_VALUE)(rect -> gx_rectangle_top + rot_cy - (height - 1 - rot_cy));
    }
    else if (angle == 270)
    {
        GX_SWAP_VALS(width, height);
        rect -> gx_rectangle_left = (GX_VALUE)(rect -> gx_rectangle_left + rot_cx - rot_cy);
        rect -> gx_rectangle_top = (GX_VALUE)(rect -> gx_rectangle_top + rot_cy - (height - 1 - rot_cx));
    }
    else
    {
        idxminx = (angle / 90) & 0x3;
        idxmaxx = (idxminx + 2) & 0x3;
        idxmaxy = (idxminx + 1) & 0x3;

        /* Calculate the source x and y center. */
        srcxres = width >> 1;
        srcyres = height >> 1;

        cosv = _gx_utility_math_cos(GX_FIXED_VAL_MAKE(angle));
        sinv = _gx_utility_math_sin(GX_FIXED_VAL_MAKE(angle));

        xres = mx[idxmaxx] * (srcxres + 2) * cosv - my[idxmaxx] * (srcyres + 2) * sinv;
        yres = my[idxmaxy] * (srcyres + 2) * cosv + mx[idxmaxy] * (srcxres + 2) * sinv;

        xres = GX_FIXED_VAL_TO_INT(xres);
        yres = GX_FIXED_VAL_TO_INT(yres);

        /* Calculate destination width and height. */
        width = (xres << 1);
        height = (yres << 1);

        /* Calculate the new rotation axis. */
        x = GX_FIXED_VAL_TO_INT((rot_cx - srcxres) * cosv - (rot_cy - srcyres) * sinv);
        y = GX_FIXED_VAL_TO_INT((rot_cy - srcyres) * cosv + (rot_cx - srcxres) * sinv);

        x += xres;
        y += yres;

        rect -> gx_rectangle_left = (GX_VALUE)(rect -> gx_rectangle_left + rot_cx - x);
        rect -> gx_rectangle_top = (GX_VALUE)(rect -> gx_rectangle_top + rot_cy - y);
    }
    rect -> gx_rectangle_right = (GX_VALUE)(rect -> gx_rectangle_left + width - 1);
    rect -> gx_rectangle_bottom = (GX_VALUE)(rect -> gx_rectangle_top + height - 1);

    return(GX_SUCCESS);
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_canvas_pixelmap_rotate                          PORTABLE C      */
/*                                                           6.1.7        */
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
/*    x_position                            Top-left x-coord to place     */
/*                                            pixelmap                    */
/*    y_position                            Top-left y-coord to place     */
/*                                            pixelmap                    */
/*    pixelmap                              Pointer to actual pixelmap    */
/*                                            to draw                     */
/*    angle                                 The angle to rotate           */
/*    rot_cx                                x-coord of rotating center, if*/
/*                                            -1 is set, default it to    */
/*                                            image center.               */
/*    rot_cy                                y-coord of rotationg center.  */
/*                                            -1 is set, default it to    */
/*                                            image center.               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_rectangle_define                                        */
/*    _gx_utility_rectangle_overlap_detect                                */
/*    _gx_canvas_rotated_pixelmap_bound_calculate                         */
/*    [gx_display_driver_pixelmap_rotate]                                 */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application code                                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  06-02-2021     Kenneth Maxwell          Modified comment(s),          */
/*                                            removed duplicate variable  */
/*                                            assignment,                 */
/*                                            resulting in version 6.1.7  */
/*                                                                        */
/**************************************************************************/
UINT  _gx_canvas_pixelmap_rotate(GX_VALUE x_position, GX_VALUE y_position, GX_PIXELMAP *pixelmap,
                                 INT angle, INT rot_cx, INT rot_cy)
{
GX_DRAW_CONTEXT *context;
GX_DISPLAY      *display;
GX_RECTANGLE     clip_rect;
GX_RECTANGLE     bound;
GX_VIEW         *view;
VOID             (*pmp_function)(GX_DRAW_CONTEXT *, INT, INT, GX_PIXELMAP *, INT, INT, INT);
INT              cx;
INT              cy;

    /* pick up the current drawing context */
    context = _gx_system_current_draw_context;

    /* pick up current display driver */
    display = context -> gx_draw_context_display;

    /* calculate rectangle that bounds the pixelmap */
    _gx_utility_rectangle_define(&bound, x_position, y_position,
                                 (GX_VALUE)(x_position + pixelmap -> gx_pixelmap_width - 1),
                                 (GX_VALUE)(y_position + pixelmap -> gx_pixelmap_height - 1));

    if (rot_cx == -1)
    {
        cx = pixelmap -> gx_pixelmap_width >> 1;
    }
    else
    {
        cx = rot_cx;
    }

    if (rot_cy == -1)
    {
        cy = pixelmap -> gx_pixelmap_height >> 1;
    }
    else
    {
        cy = rot_cy;
    }

    /* calculate rectangle that bounds the rotated pixelmap */
    _gx_canvas_rotated_pixelmap_bound_calculate(&bound, angle, cx, cy);

    /* clip the line bounding box to the dirty rectangle */
    if (!_gx_utility_rectangle_overlap_detect(&bound, &context -> gx_draw_context_dirty, &bound))
    {
        /* nothing to draw, return */
        return GX_SUCCESS;
    }

    /* Range angle to [0, 360). */
    angle = angle % 360;

    if (angle < 0)
    {
        angle += 360;
    }

    if (angle == 0)
    {
        if (!display->gx_display_driver_pixelmap_draw)
        {
            return GX_FAILURE;
        }
    }

    if (pixelmap->gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
    {
        return GX_NOT_SUPPORTED;
    }

    /* pickup pointer to correct pixelmap drawing function */
    pmp_function = display -> gx_display_driver_pixelmap_rotate;

    if (!pmp_function)
    {
        /* display driver does not support requested action */
        return GX_FAILURE;
    }

    /* test to determine if the bounding rectangle overlaps the region we are allowed to draw
       into. For each view that overlaps the bounding rectangle, do some drawing.
     */
    view = context -> gx_draw_context_view_head;

    while (view)
    {
        if (!_gx_utility_rectangle_overlap_detect(&view -> gx_view_rectangle, &bound, &clip_rect))
        {
            view = view -> gx_view_next;
            continue;
        }

        /* we have a view into which we can draw the pixelmap, do it */

        /* first, set the context clip rectangle */
        context -> gx_draw_context_clip = &clip_rect;

        if (angle == 0)
        {
            display->gx_display_driver_pixelmap_draw(context, x_position, y_position, pixelmap);
        }
        else
        {
            /* now pass the context and drawing params to driver level function */
            pmp_function(context, x_position, y_position, pixelmap, angle, cx, cy);
        }

        /* go to the next view */
        view = view -> gx_view_next;
    }

    /* Return successful completion.  */
    return(GX_SUCCESS);
}

