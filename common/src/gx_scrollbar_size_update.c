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
#include "gx_system.h"
#include "gx_display.h"
#include "gx_context.h"
#include "gx_widget.h"
#include "gx_utility.h"
#include "gx_scrollbar.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_scrollbar_size_update                           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Calculate and return the position of a GX_SCROLLBAR thumb button    */
/*    based scrollbar dimensions, current value, and visible range.       */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    scroll                                Scroll control block          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_border_width_get           Obtain widget border width    */
/*    _gx_widget_resize                     Resize a widget               */
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
VOID _gx_scrollbar_size_update(GX_SCROLLBAR *scroll)
{
GX_WINDOW   *parent;
GX_RECTANGLE size;
GX_VALUE     border_width;
UINT         style = scroll -> gx_widget_style;

    parent = (GX_WINDOW *)scroll -> gx_widget_parent;

    if (parent)
    {
        _gx_widget_border_width_get((GX_WIDGET *) parent, &border_width);

        if (style & GX_SCROLLBAR_VERTICAL)
        {
            size.gx_rectangle_top = parent -> gx_window_client.gx_rectangle_top;
            size.gx_rectangle_bottom = parent -> gx_window_client.gx_rectangle_bottom;
            size.gx_rectangle_right = (GX_VALUE)(parent -> gx_widget_size.gx_rectangle_right - border_width);
            size.gx_rectangle_left = (GX_VALUE)(size.gx_rectangle_right - scroll -> gx_scrollbar_appearance.gx_scroll_width + 1);
        }
        else
        {
            size.gx_rectangle_left = parent -> gx_window_client.gx_rectangle_left;
            size.gx_rectangle_right = parent -> gx_window_client.gx_rectangle_right;
            size.gx_rectangle_bottom = (GX_VALUE)(parent -> gx_widget_size.gx_rectangle_bottom - border_width);
            size.gx_rectangle_top = (GX_VALUE)(size.gx_rectangle_bottom - scroll -> gx_scrollbar_appearance.gx_scroll_width + 1);
        }
        _gx_widget_resize((GX_WIDGET *)scroll, &size);

        if (style & GX_SCROLLBAR_END_BUTTONS)
        {
            if (style & GX_SCROLLBAR_VERTICAL)
            {
                /* calculate size for up button */
                size.gx_rectangle_top = (GX_VALUE)(scroll -> gx_widget_size.gx_rectangle_top + 1);
                size.gx_rectangle_right = (GX_VALUE)(scroll -> gx_widget_size.gx_rectangle_right - 1);
                size.gx_rectangle_left = (GX_VALUE)(scroll->gx_widget_size.gx_rectangle_left + 1);
                size.gx_rectangle_bottom = (GX_VALUE)(size.gx_rectangle_top + scroll -> gx_scrollbar_appearance.gx_scroll_width - 1);
                _gx_widget_resize((GX_WIDGET *)&scroll -> gx_scrollbar_upleft, &size);

                /* calculate size for down button */
                size.gx_rectangle_bottom = (GX_VALUE)(scroll -> gx_widget_size.gx_rectangle_bottom - 1);
                size.gx_rectangle_top = (GX_VALUE)(size.gx_rectangle_bottom - scroll -> gx_scrollbar_appearance.gx_scroll_width + 1);
                _gx_widget_resize((GX_WIDGET *)&scroll -> gx_scrollbar_downright, &size);
            }
            else
            {
                /* calculate size for left button */
                size.gx_rectangle_left = (GX_VALUE)(scroll -> gx_widget_size.gx_rectangle_left + 1);
                size.gx_rectangle_bottom = (GX_VALUE)(scroll -> gx_widget_size.gx_rectangle_bottom - 1);
                size.gx_rectangle_top = (GX_VALUE)(scroll->gx_widget_size.gx_rectangle_top + 1);
                size.gx_rectangle_right = (GX_VALUE)(size.gx_rectangle_left + scroll -> gx_scrollbar_appearance.gx_scroll_width - 1);
                _gx_widget_resize((GX_WIDGET *)&scroll -> gx_scrollbar_upleft, &size);

                /* calculate size for right button */
                size.gx_rectangle_right = (GX_VALUE)(scroll -> gx_widget_size.gx_rectangle_right - 1);
                size.gx_rectangle_left = (GX_VALUE)(size.gx_rectangle_right - scroll -> gx_scrollbar_appearance.gx_scroll_width);
                _gx_widget_resize((GX_WIDGET *)&scroll -> gx_scrollbar_downright, &size);
            }
        }
    }
}

