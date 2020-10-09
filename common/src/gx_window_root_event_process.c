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
/**   Window Management (Window)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_widget.h"
#include "gx_window.h"
#include "gx_canvas.h"
#include "gx_utility.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_window_root_event_process                       PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function processes events for the specified root window.       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    widget                                Window's widget control block */
/*    event_ptr                             Incoming event to process     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_canvas_shift                      Shift the canvas              */
/*    _gx_window_event_process              Call widget event processing  */
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
UINT  _gx_window_root_event_process(GX_WINDOW_ROOT *win, GX_EVENT *event_ptr)
{
UINT     status;
GX_VALUE xShift;
GX_VALUE yShift;

    status = GX_SUCCESS;

    /* Process relative to the type of event.  */
    switch (event_ptr -> gx_event_type)
    {
    case GX_EVENT_PEN_DRAG:
        if (win -> gx_window_move_mode)
        {
            /* to move a root window we have to move the canvas. Calculate
               the x,y shift amounts
             */
            xShift = (GX_VALUE)(event_ptr -> gx_event_payload.gx_event_pointdata.gx_point_x -
                                win -> gx_window_move_start.gx_point_x);
            yShift = (GX_VALUE)(event_ptr -> gx_event_payload.gx_event_pointdata.gx_point_y -
                                win -> gx_window_move_start.gx_point_y);

            /* reset the starting point for next time */
            win -> gx_window_move_start.gx_point_x =
                (GX_VALUE)(event_ptr -> gx_event_payload.gx_event_pointdata.gx_point_x - xShift);
            win -> gx_window_move_start.gx_point_y =
                (GX_VALUE)(event_ptr -> gx_event_payload.gx_event_pointdata.gx_point_y - yShift);

            /* move the canvas */
            _gx_canvas_shift(win -> gx_window_root_canvas, xShift, yShift);
        }
        return(GX_SUCCESS);



    default:

        /* Call the window default processing function.  */
        status =  _gx_window_event_process((GX_WINDOW *)win, event_ptr);
    }
    /* Return widget event processing status.  */
    return(status);
}

