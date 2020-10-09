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
/**   Scrollbar Management (Scrollbar)                                    */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_button.h"
#include "gx_utility.h"
#include "gx_scrollbar.h"
#include "gx_system.h"
#include "gx_window.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_horizontal_scrollbar_create                     PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates a horizontal scrollbar.                       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    scrollbar                             scroll control block          */
/*    name                                  name of scroll                */
/*    parent                                parent window                 */
/*    appearance                            appearance of scroll bar      */
/*    style                                 style of scroll bar           */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_rectangle_define          Define a rectangle            */
/*    _gx_widget_create                     Create the underlying prompt  */
/*    _gx_system_scroll_appearance_get      Get the scrollbar appearance  */
/*    _gx_button_create                     Button create call            */
/*    [gx_window_scroll_info_get]           Retrieve window scroll info   */
/*    _gx_scroll_thumb_create               Create the thrum button       */
/*    _gx_widget_link                       Link the widget to its parent */
/*    _gx_widget_child_clipping_update      Update the clipping area      */
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
UINT  _gx_horizontal_scrollbar_create(GX_SCROLLBAR *scrollbar, GX_CONST GX_CHAR *name, GX_WINDOW *parent,
                                      GX_SCROLLBAR_APPEARANCE *appearance, ULONG style)
{

GX_RECTANGLE size;
UINT         id;

    _gx_utility_rectangle_define(&size, 0, 0, 0, 0);

    id = GX_ID_HORIZONTAL_SCROLL;

    if (appearance)
    {
        scrollbar -> gx_scrollbar_appearance = *appearance;
    }
    else
    {
        _gx_system_scroll_appearance_get(GX_SCROLLBAR_HORIZONTAL, &scrollbar -> gx_scrollbar_appearance);
    }

    /* make sure style is set correctly */
    style &= (ULONG)(~GX_SCROLLBAR_VERTICAL);
    style |= GX_SCROLLBAR_HORIZONTAL;

    /* Call the base widget create function.  */
    _gx_widget_create((GX_WIDGET *)scrollbar, name, GX_NULL, style, (USHORT)id, &size);

    /* Check for completion status on the widget's create.  */
    scrollbar -> gx_widget_type = GX_TYPE_HORIZONTAL_SCROLL;
    scrollbar -> gx_widget_normal_fill_color = GX_COLOR_ID_SCROLL_FILL;
    scrollbar -> gx_widget_selected_fill_color = GX_COLOR_ID_SCROLL_FILL;

    if (style & GX_SCROLLBAR_END_BUTTONS)
    {
        /* create the up/left button */
        _gx_button_create(&scrollbar -> gx_scrollbar_upleft, NULL, (GX_WIDGET *)scrollbar,
                          GX_STYLE_ENABLED | GX_STYLE_BORDER_RAISED, GX_ID_SCROLL_UP_LEFT, &size);

        _gx_widget_fill_color_set((GX_WIDGET *)&scrollbar -> gx_scrollbar_upleft,
                                  scrollbar -> gx_scrollbar_appearance.gx_scroll_button_color,
                                  scrollbar -> gx_scrollbar_appearance.gx_scroll_button_color,
                                  scrollbar -> gx_scrollbar_appearance.gx_scroll_button_color);

        scrollbar -> gx_scrollbar_upleft.gx_widget_status &= ~GX_STATUS_ACCEPTS_FOCUS;

        /* create the down/right button */
        _gx_button_create(&scrollbar -> gx_scrollbar_downright, NULL, (GX_WIDGET *)scrollbar,
                          GX_STYLE_ENABLED | GX_STYLE_BORDER_RAISED, GX_ID_SCROLL_DOWN_RIGHT, &size);
        _gx_widget_fill_color_set((GX_WIDGET *)&scrollbar -> gx_scrollbar_downright,
                                  scrollbar -> gx_scrollbar_appearance.gx_scroll_button_color,
                                  scrollbar -> gx_scrollbar_appearance.gx_scroll_button_color,
                                  scrollbar -> gx_scrollbar_appearance.gx_scroll_button_color);

        scrollbar -> gx_scrollbar_downright.gx_widget_status &= ~GX_STATUS_ACCEPTS_FOCUS;
    }

    /* create the thumb button */
    _gx_scroll_thumb_create(&scrollbar -> gx_scrollbar_thumb, scrollbar, (ULONG)((scrollbar -> gx_scrollbar_appearance.gx_scroll_thumb_border_style) | GX_SCROLLBAR_HORIZONTAL) | GX_STYLE_ENABLED);

    /* initialize the drawing function */
    scrollbar -> gx_widget_draw_function = (VOID (*)(GX_WIDGET *))_gx_scrollbar_draw;

    /* initialize the event handler function */
    scrollbar -> gx_widget_event_process_function = (UINT (*)(GX_WIDGET *, GX_EVENT *))_gx_scrollbar_event_process;

    /* scroll-bars have non-client area status and do not accept focus */

    scrollbar -> gx_widget_status |= GX_STATUS_NONCLIENT;
    scrollbar -> gx_widget_status &= ~GX_STATUS_ACCEPTS_FOCUS;

    /* Determine if a parent widget was provided.  */
    if (parent)
    {
        _gx_widget_link((GX_WIDGET *)parent, (GX_WIDGET *)scrollbar);

        if (parent -> gx_widget_status & GX_STATUS_VISIBLE)
        {
            _gx_widget_child_clipping_update((GX_WIDGET *)parent);
        }
    }

    return(GX_SUCCESS);
}

