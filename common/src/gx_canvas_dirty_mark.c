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
/*    _gx_canvas_dirty_mark                               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function marks the canvas dirty.                               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    canvas                                Canvas control block          */
/*    area                                  Area of canvas to mark as     */
/*                                            dirty                       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
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
VOID  _gx_canvas_dirty_mark(GX_CANVAS *canvas, GX_RECTANGLE *area)
{

    canvas -> gx_canvas_draw_count++;

    /* did the caller pass a specific rectangle? */
    if (area)
    {
        /* yes, used passed in rectangle */
        canvas -> gx_canvas_dirty_area = *area;
    }
    else
    {
        /* no, invalidate the entire canvas */
        canvas -> gx_canvas_dirty_area.gx_rectangle_top = 0;
        canvas -> gx_canvas_dirty_area.gx_rectangle_bottom =
        (GX_VALUE)(canvas -> gx_canvas_y_resolution - 1);
        canvas -> gx_canvas_dirty_area.gx_rectangle_left = 0;
        canvas -> gx_canvas_dirty_area.gx_rectangle_right =
        (GX_VALUE)(canvas -> gx_canvas_x_resolution - 1);
    }
}

