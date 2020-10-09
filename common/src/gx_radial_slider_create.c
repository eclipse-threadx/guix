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
/**   Radial Slider Management (Slider)                                   */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_radial_slider.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_radial_slider_create                            PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates a radial slider, which is a special type of   */
/*    widget.                                                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    slider                                Radial slider control block   */
/*    name                                  Name of radial slider         */
/*    parent                                Parent widget control block   */
/*    info                                  Pointer to radial slider info */
/*    style                                 Style of prompt               */
/*    slider_id                             Application-defined ID of     */
/*                                            radial slider               */
/*    size                                  Widget size                   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_create                     Create the underlying widget  */
/*    _gx_widget_link                       Link the widget to its parent */
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
UINT  _gx_radial_slider_create(GX_RADIAL_SLIDER *slider, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                               GX_RADIAL_SLIDER_INFO *info, ULONG style, USHORT slider_id, GX_CONST GX_RECTANGLE *size)
{

    /* Call the widget create function.  */
    _gx_widget_create((GX_WIDGET *)slider, name, GX_NULL, style, slider_id, size);

    /* Populate the rest of control block - overriding as necessary.  */
    slider -> gx_radial_slider_info = *info;
    slider -> gx_widget_type = GX_TYPE_RADIAL_SLIDER;
    slider -> gx_widget_draw_function = (VOID (*)(GX_WIDGET *))_gx_radial_slider_draw;
    slider -> gx_widget_event_process_function = (UINT (*)(GX_WIDGET *, GX_EVENT *event_ptr))_gx_radial_slider_event_process;
    slider -> gx_radial_slider_start_angle = 0;
    slider -> gx_radial_slider_target_angle = 0;
    slider -> gx_radial_slider_animation_total_steps = 15;
    slider -> gx_radial_slider_animation_step = 0;
    slider -> gx_radial_slider_animation_delay = 2;
    slider -> gx_widget_status |= GX_STATUS_ANIMATION_NONE;
    slider -> gx_radial_slider_animation_style = GX_ANIMATION_CIRC_EASE_IN_OUT;

    /* Determine if a parent widget was provided.  */
    if (parent)
    {
        _gx_widget_link(parent, (GX_WIDGET *)slider);
    }

    /* Return the status from widget create.  */
    return(GX_SUCCESS);
}

