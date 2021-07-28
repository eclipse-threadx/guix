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
/**   Progress Bar Management (Radial Progress Bar)                       */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_radial_progress_bar.h"
#include "gx_utility.h"
#include "gx_system.h"
#include "gx_canvas.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_radial_progress_bar_event_process               PORTABLE C      */
/*                                                           6.1.8        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service processes a radial progress bar event.                 */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    radial_progress_bar                   Radial Progress Bar control   */
/*                                            block                       */
/*    event_ptr                             Pointer to event process      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_event_process              Default widget event process  */
/*    _gx_radial_progress_bar_resize        Resize the progress bar       */
/*    _gx_radial_progress_bar_value_calculate                             */
/*                                          Calculate the value of the    */
/*                                            progress bar                */
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
/*  08-02-2021     Ting Zhu                 Modified comment(s),          */
/*                                            removed size update logic   */
/*                                            from GX_EVENT_SHOW entry,   */
/*                                            resulting in version 6.1.8  */
/*                                                                        */
/**************************************************************************/
UINT  _gx_radial_progress_bar_event_process(GX_RADIAL_PROGRESS_BAR *progress_bar, GX_EVENT *event_ptr)
{
UINT                         status;
INT                          x_dist;
INT                          y_dist;
INT                          dist;
GX_POINT                    *click;
GX_RADIAL_PROGRESS_BAR_INFO *info;


    /* Default status to success.  */
    status =  GX_SUCCESS;

    /* Process relative to the type of event.  */
    switch (event_ptr -> gx_event_type)
    {
    case GX_EVENT_SHOW:
        _gx_widget_event_process((GX_WIDGET *)progress_bar, event_ptr);
        break;

#if defined(GX_BRUSH_ALPHA_SUPPORT)
    case GX_EVENT_HIDE:
        _gx_widget_event_process((GX_WIDGET *)progress_bar, event_ptr);
        if ((progress_bar -> gx_radial_progress_bar_canvas.gx_canvas_memory) &&
            _gx_system_memory_free)
        {
            _gx_system_memory_free((VOID *)progress_bar -> gx_radial_progress_bar_canvas.gx_canvas_memory);
            _gx_canvas_delete(&progress_bar -> gx_radial_progress_bar_canvas);
        }
        break;
#endif

    case GX_EVENT_RESIZED:
        _gx_radial_progress_bar_resize(progress_bar);
        break;

    case GX_EVENT_PEN_DOWN:
        if (progress_bar -> gx_widget_style & GX_STYLE_ENABLED)
        {
            _gx_system_input_capture((GX_WIDGET *)progress_bar);

            info = &progress_bar -> gx_radial_progress_bar_info;
            click = &event_ptr -> gx_event_payload.gx_event_pointdata;

            x_dist = (INT)(click -> gx_point_x - info -> gx_radial_progress_bar_info_xcenter);
            y_dist = (INT)(click -> gx_point_y - info -> gx_radial_progress_bar_info_ycenter);

            dist = (x_dist * x_dist) + (y_dist * y_dist);
            dist = (INT)_gx_utility_math_sqrt((UINT)dist);

            if ((dist <= (INT)(info -> gx_radial_progress_bar_info_radius + (info -> gx_radial_progress_bar_info_selected_brush_width >> 1))) &&
                (dist >= (INT)(info -> gx_radial_progress_bar_info_radius - (info -> gx_radial_progress_bar_info_selected_brush_width >> 1))))
            {
                /* Calculate radial progress bar value.  */
                _gx_radial_progress_bar_value_calculate(progress_bar, event_ptr -> gx_event_payload.gx_event_pointdata);
            }
        }
        else
        {
            _gx_widget_event_to_parent((GX_WIDGET *)progress_bar, event_ptr);
        }
        break;

    case GX_EVENT_PEN_UP:
        if (progress_bar -> gx_widget_status & GX_STATUS_OWNS_INPUT)
        {
            _gx_system_input_release((GX_WIDGET *)progress_bar);
        }
        else
        {
            _gx_widget_event_to_parent((GX_WIDGET *)progress_bar, event_ptr);
        }
        break;

    case GX_EVENT_PEN_DRAG:
        if (progress_bar -> gx_widget_status & GX_STATUS_OWNS_INPUT)
        {
            _gx_radial_progress_bar_value_calculate(progress_bar, event_ptr -> gx_event_payload.gx_event_pointdata);
        }
        else
        {
            _gx_widget_event_to_parent((GX_WIDGET *)progress_bar, event_ptr);
        }
        break;

    default:

        /* Call the widget default processing.  */
        status = _gx_widget_event_process((GX_WIDGET *)progress_bar, event_ptr);
    }

    /* Return completion status.  */
    return(status);
}

