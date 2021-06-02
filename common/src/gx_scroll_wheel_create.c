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
/**   Scroll Wheel Management (Scroll Wheel)                              */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_window.h"
#include "gx_scroll_wheel.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_scroll_wheel_create                             PORTABLE C      */
/*                                                           6.1.7        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates a scroll wheel selector widget.               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    wheel                                 Scroll wheel control block    */
/*    name                                  Name of widget                */
/*    parent                                Parent widget control block   */
/*    total_rows                            Total rows of the scroll wheel*/
/*    style                                 Style of widget               */
/*    Id                                    Application-defined ID of the */
/*                                            the widget                  */
/*    size                                  Widget size                   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_window_create                     Create a window               */
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
/*  06-02-2021     Ting Zhu                 Modified comment(s),          */
/*                                            updated with scroll wheel   */
/*                                            control block change,       */
/*                                            resulting in version 6.1.7  */
/*                                                                        */
/**************************************************************************/

UINT _gx_scroll_wheel_create(GX_SCROLL_WHEEL *wheel, GX_CONST GX_CHAR *name,
                             GX_WIDGET *parent, INT total_rows,
                             ULONG style, USHORT Id, GX_CONST GX_RECTANGLE *size)
{

    GX_PARAMETER_NOT_USED(parent);

    _gx_window_create((GX_WINDOW *)wheel, name, GX_NULL, style, Id, size);

    wheel -> gx_widget_type = GX_TYPE_SCROLL_WHEEL;
    wheel -> gx_scroll_wheel_total_rows = total_rows;
    wheel -> gx_scroll_wheel_row_height = 30;

    memset(&wheel -> gx_scroll_wheel_gradient, 0, sizeof(GX_GRADIENT));

    wheel -> gx_scroll_wheel_selected_row = 0;
    wheel -> gx_scroll_wheel_selected_yshift = 0;
    wheel -> gx_scroll_wheel_shift_error = 0;
    wheel -> gx_scroll_wheel_animation_speed = 0;
    wheel -> gx_scroll_wheel_animation_end_speed = 0;
    wheel -> gx_scroll_wheel_animation_steps = 0;
    wheel -> gx_scroll_wheel_animation_max_steps = 10;
    wheel -> gx_scroll_wheel_animation_delay = 2;
    wheel -> gx_scroll_wheel_animation_start_speed_rate = GX_FIXED_VAL_MAKE(1);
    wheel -> gx_scroll_wheel_animation_end_speed_rate = 200;
    wheel -> gx_scroll_wheel_selected_background = GX_NULL;
    wheel -> gx_widget_event_process_function = (UINT (*)(GX_WIDGET *, GX_EVENT *))_gx_scroll_wheel_event_process;
    wheel -> gx_scroll_wheel_scroll = _gx_scroll_wheel_scroll;
    wheel -> gx_scroll_wheel_wrap_style_check = _gx_scroll_wheel_wrap_style_check;

    return(GX_SUCCESS);
}

