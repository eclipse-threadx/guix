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
/**   Scroll Management (Scroll)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_button.h"
#include "gx_scrollbar.h"
#include "gx_utility.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_scroll_thumb_create                             PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates a bitmap prompt, which is a special type of   */
/*    widget.                                                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    scroll_thumb                          Scroll thumb control block    */
/*    parent                                Pointer to parent scrollbar   */
/*    style                                 Style of scroll bar           */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_button_create                     Create a button               */
/*    _gx_widget_status_remove              Clear the widget status flag  */
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
UINT  _gx_scroll_thumb_create(GX_SCROLL_THUMB *scroll_thumb, GX_SCROLLBAR *parent,
                              ULONG style)
{
GX_RECTANGLE size;

    if (parent)
    {
        size = parent -> gx_widget_size;
    }
    else
    {
        size.gx_rectangle_bottom = size.gx_rectangle_left = size.gx_rectangle_right = size.gx_rectangle_top = 0;
    }

    /* Clear the scroll control block.  */
    _gx_button_create((GX_BUTTON *)scroll_thumb, GX_NULL, GX_NULL, style, GX_ID_SCROLL_THUMB, &size);
    _gx_widget_status_remove((GX_WIDGET *)scroll_thumb, GX_STATUS_ACCEPTS_FOCUS);

    scroll_thumb -> gx_widget_draw_function = (VOID (*)(struct GX_WIDGET_STRUCT *))_gx_scroll_thumb_draw;
    scroll_thumb -> gx_widget_event_process_function = (UINT (*)(GX_WIDGET *, GX_EVENT *))_gx_scroll_thumb_event_process;
    scroll_thumb -> gx_widget_normal_fill_color = GX_COLOR_ID_SCROLL_BUTTON;
    scroll_thumb -> gx_widget_selected_fill_color = GX_COLOR_ID_SCROLL_BUTTON;
    scroll_thumb -> gx_scroll_thumb_border_color = GX_COLOR_ID_SCROLL_BUTTON;

    /* Determine if a parent widget was provided.  */
    if (parent)
    {
        scroll_thumb -> gx_scroll_thumb_border_color = parent -> gx_scrollbar_appearance.gx_scroll_thumb_border_color;
        scroll_thumb -> gx_widget_normal_fill_color = parent -> gx_scrollbar_appearance.gx_scroll_thumb_color;
        scroll_thumb -> gx_widget_selected_fill_color = parent -> gx_scrollbar_appearance.gx_scroll_thumb_color;
        scroll_thumb -> gx_scroll_thumb_pixelmap = parent -> gx_scrollbar_appearance.gx_scroll_thumb_pixelmap;

        _gx_widget_link((GX_WIDGET *)parent, (GX_WIDGET *)scroll_thumb);
    }
    return GX_SUCCESS;
}

