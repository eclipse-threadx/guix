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
/*    _gx_canvas_hide                                     PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function makes a canvas invisible                              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    canvas                                Canvas control block          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_canvas_dirty_mark                 Set the canvas dirty flag     */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application                                                         */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  12-31-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            modified canvas dirty logic,*/
/*                                            resulting in version 6.1.3  */
/*                                                                        */
/**************************************************************************/
UINT  _gx_canvas_hide(GX_CANVAS *canvas)
{
VOID         (*hide_function)(INT layer);
GX_CANVAS   *head = _gx_system_canvas_created_list;
GX_RECTANGLE dirty;
GX_RECTANGLE combine;

    /* change canvas status flag. */
    canvas -> gx_canvas_status &= ~(UINT)(GX_CANVAS_VISIBLE);

    if (canvas -> gx_canvas_hardware_layer >= 0)
    {
        hide_function = canvas -> gx_canvas_display -> gx_display_layer_services -> gx_display_layer_hide;

        if (hide_function)
        {
            hide_function(canvas -> gx_canvas_hardware_layer);
            return(GX_SUCCESS);
        }
    }

    /* mark the canvas dirty so that it get refreshed */
    _gx_utility_rectangle_define(&dirty, 0, 0, (GX_VALUE)(canvas -> gx_canvas_x_resolution - 1), (GX_VALUE)(canvas -> gx_canvas_y_resolution - 1));
    if (canvas -> gx_canvas_display_offset_x || canvas -> gx_canvas_display_offset_y)
    {
        _gx_utility_rectangle_shift(&dirty, canvas -> gx_canvas_display_offset_x, canvas -> gx_canvas_display_offset_y);
    }

    while (head)
    {
        if (head -> gx_canvas_status & GX_CANVAS_VISIBLE)
        {
            _gx_utility_rectangle_define(&combine, 0, 0, (GX_VALUE)(head -> gx_canvas_x_resolution - 1), (GX_VALUE)(head -> gx_canvas_y_resolution - 1));

            if (head -> gx_canvas_display_offset_x || head -> gx_canvas_display_offset_y)
            {
                _gx_utility_rectangle_shift(&combine, head -> gx_canvas_display_offset_x, head -> gx_canvas_display_offset_y);
            }

            _gx_utility_rectangle_combine(&combine, &dirty);
            _gx_canvas_dirty_mark(head, &combine);
        }

        head = head -> gx_canvas_created_next;
    }

    /* Return successful status.  */
    return(GX_SUCCESS);
}

