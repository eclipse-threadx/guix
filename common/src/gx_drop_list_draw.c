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
/**   Drop List Management (List)                                         */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_display.h"
#include "gx_context.h"
#include "gx_widget.h"
#include "gx_window.h"
#include "gx_utility.h"
#include "gx_drop_list.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_drop_list_draw                                  PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function draws the specified widget.                           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    drop_list                             Pointer to drop list widget   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_drop_list_background_draw         Draw the widget background    */
/*    _gx_widget_children_draw              Draw children widgets         */
/*    _gx_vertical_list_selected_widget_get Get selected child item of    */
/*                                            popup list                  */
/*    _gx_widget_client_get                 Find the client area of a     */
/*                                            widget                      */
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
/*                                                                        */
/**************************************************************************/
VOID _gx_drop_list_draw(GX_DROP_LIST *drop_list)
{
GX_RECTANGLE      client;
GX_WIDGET        *selected;
GX_WIDGET        *widget;
GX_VERTICAL_LIST *list;
GX_RECTANGLE      selected_size;
GX_RECTANGLE      selected_clip;
ULONG             selected_style;

    widget = (GX_WIDGET *)drop_list;
    _gx_drop_list_background_draw(drop_list);

    list = (GX_VERTICAL_LIST *)&drop_list -> gx_drop_list_popup.gx_popup_list_list;
    _gx_vertical_list_selected_widget_get(list, &selected);

    if (selected)
    {
        /* draw the selected widget into my client area: */
        _gx_widget_client_get(widget, -1, &client);
        selected_size = selected -> gx_widget_size;
        selected_clip = selected -> gx_widget_clip;
        selected_style = selected -> gx_widget_style;

        if (drop_list -> gx_widget_status & GX_STATUS_HAS_FOCUS)
        {
            selected -> gx_widget_style |= ~GX_STYLE_DRAW_SELECTED;
        }
        else
        {
            selected -> gx_widget_style &= ~GX_STYLE_DRAW_SELECTED;
        }
        selected -> gx_widget_size = client;
        selected -> gx_widget_clip = client;
        selected -> gx_widget_draw_function(selected);
        selected -> gx_widget_size = selected_size;
        selected -> gx_widget_clip = selected_clip;
        selected -> gx_widget_style = selected_style;
    }

    _gx_widget_children_draw(widget);
}

