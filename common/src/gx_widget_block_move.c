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
#include "gx_widget.h"
#include "gx_canvas.h"
#include "gx_utility.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_widget_block_move                               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service move a rectangular block of pixels.                    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    widget                                Pointer to widget             */
/*    block                                 Rectangle to move             */
/*    x_shift                               Number of pixels to shift on  */
/*                                            the x-axis                  */
/*    y_shift                               Number of pixels to shift on  */
/*                                            the y-axis                  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_dirty_partial_add          Add area to the dirty list    */
/*    _gx_widget_canvas_get                 Find canvas associated with   */
/*                                            the specified widget        */
/*    _gx_canvas_drawing_initiate           Start canvas draw             */
/*    _gx_canvas_block_move                 Canvas-level block move call  */
/*    _gx_canvas_drawing_complete           Finish canvas draw            */
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
UINT _gx_widget_block_move(GX_WIDGET *widget, GX_RECTANGLE *block, INT x_shift, INT y_shift)
{
GX_CANVAS   *canvas;
UINT         status;
GX_RECTANGLE dirty;
GX_VALUE     width;
GX_VALUE     height;

    if (widget -> gx_widget_status & GX_STATUS_TRANSPARENT)
    {
        /* cannot do block move if transparent widget */
        _gx_system_dirty_partial_add(widget, block);
        return GX_SUCCESS;
    }

    _gx_widget_canvas_get(widget, &canvas);

    if (!canvas)
    {
        return GX_INVALID_CANVAS;
    }

    GX_ENTER_CRITICAL

    _gx_widget_width_get(widget, &width);
    _gx_widget_height_get(widget, &height);

    if ((GX_ABS(x_shift) >= width) || (GX_ABS(y_shift) >= height))
    {
        /* Shift is too large, just invalidate the widget. */
        _gx_system_dirty_partial_add(widget, block);
    }
    else
    {
        _gx_canvas_drawing_initiate(canvas, widget, block);
        status = _gx_canvas_block_move(block, (GX_VALUE)x_shift, (GX_VALUE)y_shift, &dirty);

        if (status == GX_SUCCESS)
        {
            /* re-draw the portion that could not be moved */
            _gx_canvas_drawing_initiate(canvas, widget, &dirty);
            widget->gx_widget_draw_function(widget);
            _gx_canvas_drawing_complete(canvas, GX_FALSE);
            _gx_canvas_drawing_complete(canvas, GX_TRUE);
        }
        else
        {
            /* block move was not successful, just invalidate the widget client */
            _gx_system_dirty_partial_add(widget, block);
            _gx_canvas_drawing_complete(canvas, GX_FALSE);
        }
    }
    
    GX_EXIT_CRITICAL
    return(GX_SUCCESS);
}

