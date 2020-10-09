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
/**   Text Scroll Wheel Management (Scroll Wheel)                         */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_scroll_wheel.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_text_scroll_wheel_create                        PORTABLE C      */
/*                                                           6.1          */
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
/*    _gx_scroll_wheel_create               Create a text scroll wheel    */
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
/*                                            added logic to init new     */
/*                                            structure member for        */
/*                                            dynamic bidi text support,  */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
UINT  _gx_text_scroll_wheel_create(GX_TEXT_SCROLL_WHEEL *wheel,
                                   GX_CONST GX_CHAR *name, GX_WIDGET *parent, INT total_rows,
                                   ULONG style, USHORT Id, GX_CONST GX_RECTANGLE *size)
{

    /* Call default widget create. */
    _gx_scroll_wheel_create((GX_SCROLL_WHEEL *)wheel, name, parent, total_rows, style, Id, size);

#if defined(GX_ENABLE_DEPRECATED_STRING_API)
    wheel -> gx_text_scroll_wheel_text_get_deprecated = GX_NULL;
#endif
    wheel -> gx_text_scroll_wheel_text_get = GX_NULL;
    wheel -> gx_widget_type = GX_TYPE_TEXT_SCROLL_WHEEL;
    wheel -> gx_text_scroll_wheel_normal_font = GX_FONT_ID_DEFAULT;
    wheel -> gx_text_scroll_wheel_selected_font = GX_FONT_ID_DEFAULT;
    wheel -> gx_text_scroll_wheel_normal_text_color = GX_COLOR_ID_SHADOW;
    wheel -> gx_text_scroll_wheel_selected_text_color = GX_COLOR_ID_TEXT;
    wheel -> gx_text_scroll_wheel_disabled_text_color = GX_COLOR_ID_DISABLED_TEXT;
    wheel -> gx_widget_draw_function = (VOID (*)(GX_WIDGET *))_gx_text_scroll_wheel_draw;
    wheel -> gx_widget_event_process_function = (UINT (*)(GX_WIDGET *, GX_EVENT *))_gx_text_scroll_wheel_event_process;

#ifdef GX_DYNAMIC_BIDI_TEXT_SUPPORT
    wheel -> gx_text_scroll_wheel_bidi_resolved_text_info = GX_NULL;
#endif  // GX_DYNAMIC_BIDI_TEXT_SUPPORT

    return(GX_SUCCESS);
}

