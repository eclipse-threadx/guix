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

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_canvas_drawing_complete                         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function completes drawing on the specified canvas.            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    canvas                                Canvas control block          */
/*    flush                                 If GX_TRUE, the content of    */
/*                                            the canvas is flushed to    */
/*                                            the display                 */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    [gx_display_driver_buffer_toggle]     Toggle visible frame buffer   */
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
UINT  _gx_canvas_drawing_complete(GX_CANVAS *canvas, GX_BOOL flush)
{
GX_DISPLAY      *display = canvas -> gx_canvas_display;

    /* Determing if canvas drawing has been initiated.  */

    if (canvas -> gx_canvas_draw_nesting > 0)
    {
        if (display -> gx_display_driver_drawing_complete)
        {
            display -> gx_display_driver_drawing_complete(display, canvas);
        }

        /* Decrement the drawing nesting counter.  */
        canvas -> gx_canvas_draw_nesting = (GX_UBYTE)(canvas->gx_canvas_draw_nesting - 1);

        /* Pop the previous draw context. */
        _gx_system_current_draw_context++;

        if (_gx_system_current_draw_context == _gx_system_draw_context_stack_end)
        {
            _gx_system_current_draw_context = GX_NULL;
        }

        if (canvas -> gx_canvas_draw_nesting == 0 && flush)
        {
            canvas -> gx_canvas_display -> gx_display_driver_buffer_toggle(canvas, &canvas -> gx_canvas_dirty_area);

            /* reset the canvas dirty counter */
            canvas -> gx_canvas_draw_count = 0;
        }
    }

    return(GX_SUCCESS);
}

