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
/**   System Management (System)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_utility.h"
#include "gx_system.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_system_top_root_find                            PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function finds the top root window for a given incoming event. */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    in_event                              Pointer to event              */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    GX_WINDOW_ROOT                        Pointer to found root window  */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_rectangle_shift           Shift a rectangle             */
/*    _gx_utility_rectangle_point_detect    Detect point in rectangle     */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_system_event_dispatch             Dispatch GUIX event           */
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
GX_WINDOW_ROOT  *_gx_system_top_root_find(GX_EVENT *in_event)
{
GX_WINDOW_ROOT *root = _gx_system_root_window_created_list;
GX_RECTANGLE    testsize;

    /* Loop through the widgets in reverse order.  */
    while (root)
    {
        if (root -> gx_widget_status & GX_STATUS_VISIBLE)
        {
            if (in_event -> gx_event_display_handle == (ULONG)(root -> gx_window_root_canvas -> gx_canvas_display -> gx_display_driver_data))
            {
                /* get the root window size */
                testsize = root -> gx_widget_size;

                /* offset the size by the canvas offset */
                _gx_utility_rectangle_shift(&testsize,
                                            root -> gx_window_root_canvas -> gx_canvas_display_offset_x,
                                            root -> gx_window_root_canvas -> gx_canvas_display_offset_y);

                /* Is the point in this widget?  */
                if (_gx_utility_rectangle_point_detect(&testsize, in_event -> gx_event_payload.gx_event_pointdata))
                {
                    break;
                }
            }
        }

        /* Move to the next root window.  */
        root = (GX_WINDOW_ROOT *)root -> gx_widget_next;
    }

    /* Return the root widget.  */
    return(root);
}

