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
/*    _gx_drop_list_selected_children_draw                PORTABLE C      */
/*                                                           6.3.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function draws the children of the selected item to the drop   */
/*    list client area with the specified shift values.                   */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    drop_list                             Pointer to drop list widget   */
/*    widget                                Widget to be drawn            */
/*    xshift                                Shift value in x coordinate   */
/*    yshift                                Shift value in y coordinate   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_rectangle_shift           Shift rectangle               */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_drop_list_draw                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  10-31-2023     Ting Zhu                 Initial Version 6.3.0         */
/*                                                                        */
/**************************************************************************/
static VOID _gx_drop_list_selected_children_draw(GX_DROP_LIST *drop_list, GX_WIDGET *widget, GX_VALUE xshift, GX_VALUE yshift)
{
GX_WIDGET   *child;
GX_WIDGET   *child_child;
GX_RECTANGLE old_size;
GX_RECTANGLE old_clip;
ULONG        old_style;

    /* Save the first child.  */
    child = widget -> gx_widget_first_child;

    /* Draw the children.  */
    while (child)
    {
        /* Save the widget size.  */
        old_size = child -> gx_widget_size;

        /* Save the widget clip size.  */
        old_clip = child -> gx_widget_clip;

        /* Save the widget style.  */
        old_style = child -> gx_widget_style;

        /* Update the widget style temporarily.  */
        if (drop_list -> gx_widget_status & GX_STATUS_HAS_FOCUS)
        {
            child -> gx_widget_style |= ~GX_STYLE_DRAW_SELECTED;
        }
        else
        {
            child -> gx_widget_style &= ~GX_STYLE_DRAW_SELECTED;
        }

        /* Shift the widget size and clip with the specified shift values temporarily.  */
        _gx_utility_rectangle_shift(&child -> gx_widget_size, xshift, yshift);
        _gx_utility_rectangle_shift(&child -> gx_widget_clip, xshift, yshift);

        child_child = child -> gx_widget_first_child;

        /* Set the first child to NULL temporarily.  */
        child -> gx_widget_first_child = GX_NULL;

        /* Draw the widget.  */
        child -> gx_widget_draw_function(child);

        if (child_child)
        {
            child -> gx_widget_first_child = child_child;

            _gx_drop_list_selected_children_draw(drop_list, child, xshift, yshift);
        }

        /* Recover the widget properties.  */
        child -> gx_widget_size = old_size;
        child -> gx_widget_clip = old_clip;
        child -> gx_widget_style = old_style;

        child = child -> gx_widget_next;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_drop_list_draw                                  PORTABLE C      */
/*                                                           6.3.0        */
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
/*  10-31-2023     Ting Zhu                 Modified comment(s),          */
/*                                            improved drawing of the     */
/*                                            selected item,              */
/*                                            resulting in version 6.3.0  */
/*                                                                        */
/**************************************************************************/
VOID _gx_drop_list_draw(GX_DROP_LIST *drop_list)
{
GX_RECTANGLE      client;
GX_WIDGET        *selected;
GX_VALUE          xshift;
GX_VALUE          yshift;
INT               selected_index;
GX_VERTICAL_LIST *list = &drop_list -> gx_drop_list_popup.gx_popup_list_list;
GX_RECTANGLE      size;
GX_RECTANGLE      clip;
ULONG             style;
GX_WIDGET        *child = GX_NULL;
GX_BOOL           reuse_list_item = GX_FALSE;

    /* Draw the background.  */
    _gx_drop_list_background_draw(drop_list);

    /* Pick the selected item.  */
    _gx_vertical_list_selected_widget_get(list, &selected);

    if (!selected && list -> gx_vertical_list_callback)
    {
        /* If not be able to retrieve the selected widget, its possible that the selcted item
           has been reused for displaying the other list items during the scrolling.
           In this case, try to get the selected index first, then reuse the list child to draw the selected item. */

        /* Get the selected index.  */
        _gx_vertical_list_selected_index_get(list, &selected_index);

        if((selected_index >= 0) && (selected_index < list -> gx_vertical_list_total_rows))
        {
            /* Make the first list child as the selected widget temporarily.  */
            selected = _gx_widget_first_client_child_get((GX_WIDGET *)list);

            /* Call the list callback function to create the selected widget.  */
            list -> gx_vertical_list_callback(list, selected, selected_index);

            reuse_list_item = GX_TRUE;
        }
    }

    if (selected)
    {

        _gx_widget_client_get((GX_WIDGET *)drop_list, -1, &client);

        size = selected -> gx_widget_size;
        clip = selected -> gx_widget_clip;

        if (selected -> gx_widget_first_child)
        {
            child = selected -> gx_widget_first_child;
            selected -> gx_widget_first_child = GX_NULL;

            /* Calculate the distance from the selected widget to the client area.  */
            xshift = (GX_VALUE)(client.gx_rectangle_left - size.gx_rectangle_left);
            yshift = (GX_VALUE)((client.gx_rectangle_bottom + client.gx_rectangle_top - size.gx_rectangle_bottom - size.gx_rectangle_top) >> 1);
        }

        selected -> gx_widget_size = client;
        selected -> gx_widget_clip = client;


        /* Save the widget style.  */
        style = selected -> gx_widget_style;

        /* Update the widget style temporarily.  */
        if (drop_list -> gx_widget_status & GX_STATUS_HAS_FOCUS)
        {
            selected -> gx_widget_style |= ~GX_STYLE_DRAW_SELECTED;
        }
        else
        {
            selected -> gx_widget_style &= ~GX_STYLE_DRAW_SELECTED;
        }

        selected -> gx_widget_draw_function(selected);

        if (child)
        {
            selected -> gx_widget_first_child = child;

            /* Draw the selected widget into my client area: */
            _gx_drop_list_selected_children_draw(drop_list, selected, xshift, yshift);
        }

        selected -> gx_widget_size = size;
        selected -> gx_widget_clip = clip;
        selected -> gx_widget_style = style;
    }

    if (reuse_list_item)
    {
        /* Call the list callback funtion to create the widget with its original index. */
        list -> gx_vertical_list_callback(list, selected, list -> gx_vertical_list_top_index);
    }

    /* Draw the children.  */
    _gx_widget_children_draw((GX_WIDGET *)drop_list);
}

