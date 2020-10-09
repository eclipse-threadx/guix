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
/**   Vertical List (List)                                                */
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
/*    _gx_vertical_list_children_position                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function positions the children for the vertical list          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    vertical_list                         Vertical list control block   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_height_get                 retrieves the height of the   */
/*                                            widget                      */
/*    _gx_widget_resize                     resizes the widget            */
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
UINT _gx_vertical_list_children_position(GX_VERTICAL_LIST *vertical_list)
{
GX_RECTANGLE childsize = vertical_list -> gx_window_client;
GX_WIDGET   *child = vertical_list -> gx_widget_first_child;
INT          index    = vertical_list -> gx_vertical_list_top_index;
GX_VALUE     height;
GX_VALUE     client_height;

    vertical_list -> gx_vertical_list_child_height = 0;
    vertical_list -> gx_vertical_list_child_count = 0;

    while (child)
    {
        if (!(child -> gx_widget_status & GX_STATUS_NONCLIENT))
        {
            /* increment child count */
            vertical_list -> gx_vertical_list_child_count++;

            /* assign this child's id */

            if (!(child -> gx_widget_id))
            {
                child -> gx_widget_id = (USHORT)(LIST_CHILD_ID_START + vertical_list -> gx_vertical_list_child_count);
            }

            if (index == vertical_list -> gx_vertical_list_selected)
            {
                child -> gx_widget_style |= GX_STYLE_DRAW_SELECTED;
            }
            else
            {
                child -> gx_widget_style &= ~GX_STYLE_DRAW_SELECTED;
            }
            index++;

            child -> gx_widget_status &= ~GX_STATUS_ACCEPTS_FOCUS;

            /* pick up child item height, needed for scrolling */
            _gx_widget_height_get(child, &height);
            if (height > vertical_list -> gx_vertical_list_child_height)
            {
                vertical_list -> gx_vertical_list_child_height = height;
            }

            /* move this child into position */
            childsize.gx_rectangle_bottom = (GX_VALUE)(childsize.gx_rectangle_top + height - 1);
            _gx_widget_resize(child, &childsize);
            childsize.gx_rectangle_top = (GX_VALUE)(childsize.gx_rectangle_bottom + 1);
        }
        child = child -> gx_widget_next;
    }

    /* calculate number of visible rows, needed for scrolling info */
    if (vertical_list -> gx_vertical_list_child_height > 0)
    {
        _gx_window_client_height_get((GX_WINDOW *)vertical_list, &client_height);
        vertical_list -> gx_vertical_list_visible_rows = (GX_VALUE)((client_height + vertical_list -> gx_vertical_list_child_height - 1) /
                                                                    vertical_list -> gx_vertical_list_child_height);
    }
    else
    {
        vertical_list -> gx_vertical_list_visible_rows = 1;
    }

    return(GX_SUCCESS);
}

