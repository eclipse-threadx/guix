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
/**   Scroll Wheel Management (Generic Scroll Wheel)                      */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_window.h"
#include "gx_scroll_wheel.h"
#include "gx_widget.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_generic_scroll_wheel_create                     PORTABLE C      */
/*                                                           6.1.7        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates a generic scroll wheel selector widget.       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    wheel                                 Scroll wheel control block    */
/*    name                                  Name of widget                */
/*    parent                                Parent widget control block   */
/*    total_rows                            Total rows of the scroll wheel*/
/*    callback                              Callback function to create a */
/*                                            widget row                  */
/*    style                                 Style of widget               */
/*    id                                    Application-defined ID of the */
/*                                            the widget                  */
/*    size                                  Widget size                   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_scroll_wheel_create               Create a scroll wheel widget  */
/*    _gx_widget_link                       Link a widget to its parent   */
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
/*  06-02-2021     Ting Zhu                 Initial Version 6.1.7         */
/*                                                                        */
/**************************************************************************/

UINT _gx_generic_scroll_wheel_create(GX_GENERIC_SCROLL_WHEEL *wheel,
                                    GX_CONST GX_CHAR *name,
                                    GX_WIDGET *parent,
                                    INT total_rows,
                                    VOID (*callback)(GX_GENERIC_SCROLL_WHEEL *, GX_WIDGET *, INT),
                                    ULONG style,
                                    USHORT id,
                                    GX_CONST GX_RECTANGLE *size)
{

    /* Call default widget create. */
    _gx_scroll_wheel_create((GX_SCROLL_WHEEL *)wheel, name, parent, total_rows, style, id, size);

    wheel -> gx_widget_type = GX_TYPE_GENERIC_SCROLL_WHEEL;
    wheel -> gx_generic_scroll_wheel_top_index = 0;
    wheel -> gx_generic_scroll_wheel_callback = callback;
    wheel -> gx_generic_scroll_wheel_child_count = 0;
    wheel -> gx_generic_scroll_wheel_visible_rows = 0;
    wheel -> gx_widget_event_process_function = (UINT (*)(GX_WIDGET *, GX_EVENT *))_gx_generic_scroll_wheel_event_process;
    wheel -> gx_widget_draw_function = (VOID (*)(GX_WIDGET *))_gx_generic_scroll_wheel_draw;
    wheel -> gx_scroll_wheel_scroll = (UINT(*)(GX_SCROLL_WHEEL*, GX_VALUE))_gx_generic_scroll_wheel_scroll;
    wheel -> gx_scroll_wheel_wrap_style_check = (GX_BOOL(*)(GX_SCROLL_WHEEL*))_gx_generic_scroll_wheel_wrap_style_check;

    /* Determine if a parent widget was provided.  */
    if (parent)
    {
        _gx_widget_link((GX_WIDGET *)parent, (GX_WIDGET *)wheel);
    }

    return GX_SUCCESS;
}

