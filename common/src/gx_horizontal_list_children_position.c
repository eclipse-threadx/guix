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
/**   Horizontal List (List)                                              */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_window.h"
#include "gx_system.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_horizontal_list_children_position               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function positions the children for the horizontal list.       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    horizontal_list                       Horizontal list widget        */
/*                                            control block               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_width_get                  Retrieves the width of the    */
/*                                            widget                      */
/*    _gx_widget_resize                     Resizes the widget            */
/*    _gx_window_client_width_get           Retrieves the width of the    */
/*                                            client                      */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_horizontal_list_event_process                                   */
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
UINT _gx_horizontal_list_children_position(GX_HORIZONTAL_LIST *horizontal_list)
{
GX_RECTANGLE childsize = horizontal_list -> gx_window_client;
GX_WIDGET   *child = horizontal_list -> gx_widget_first_child;
INT          index    = horizontal_list -> gx_horizontal_list_top_index;

GX_VALUE     width;
GX_VALUE     client_width;

    horizontal_list -> gx_horizontal_list_child_width = 0;
    horizontal_list -> gx_horizontal_list_child_count = 0;

    while (child)
    {
        if (!(child -> gx_widget_status & GX_STATUS_NONCLIENT))
        {
            /* increment child count */
            horizontal_list -> gx_horizontal_list_child_count++;

            /* assign this child's id */

            if (!(child -> gx_widget_id))
            {
                child -> gx_widget_id = (USHORT)(LIST_CHILD_ID_START + horizontal_list -> gx_horizontal_list_child_count);
            }

            if (index == horizontal_list -> gx_horizontal_list_selected)
            {
                child -> gx_widget_style |= GX_STYLE_DRAW_SELECTED;
            }
            else
            {
                child -> gx_widget_style &= ~GX_STYLE_DRAW_SELECTED;
            }
            index++;

            child -> gx_widget_status &= ~GX_STATUS_ACCEPTS_FOCUS;

            /* pick up child item width, needed for scrolling */
            _gx_widget_width_get(child, &width);
            if (width > horizontal_list -> gx_horizontal_list_child_width)
            {
                horizontal_list -> gx_horizontal_list_child_width = width;
            }

            /* move this child into position */
            childsize.gx_rectangle_right = (GX_VALUE)(childsize.gx_rectangle_left + width - 1);
            _gx_widget_resize(child, &childsize);
            childsize.gx_rectangle_left = (GX_VALUE)(childsize.gx_rectangle_right + 1);
        }
        child = child -> gx_widget_next;
    }

    /* calculate number of visible columns, needed for scrolling info */

    if (horizontal_list -> gx_horizontal_list_child_width > 0)
    {
        _gx_window_client_width_get((GX_WINDOW *)horizontal_list, &client_width);
        horizontal_list -> gx_horizontal_list_visible_columns = (GX_VALUE)((client_width + horizontal_list -> gx_horizontal_list_child_width - 1) /
                                                                           horizontal_list -> gx_horizontal_list_child_width);
    }
    else
    {
        horizontal_list -> gx_horizontal_list_visible_columns = 1;
    }
    return(GX_SUCCESS);
}

