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
/**   Tree View Management (Menu)                                         */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_tree_view.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_tree_view_scroll_info_get                       PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function gets the tree view scroll information.                */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    tree                                 Pointer to tree view control   */
/*                                           block                        */
/*    type                                 GX_SCROLLBAR_HORIZONTAL        */
/*                                           or GX_SCROLLBAR_VERTICAL     */
/*    info                                 Pointer to destination for     */
/*                                           scroll info                  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                               Completion status              */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_tree_view_scroll                 Scroll tree view client area   */
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
UINT _gx_tree_view_scroll_info_get(GX_TREE_VIEW *tree, ULONG type, GX_SCROLL_INFO *info)
{
GX_RECTANGLE *client;
INT           shift;
INT           value;

    client = &tree -> gx_window_client;

    if (type & GX_SCROLLBAR_VERTICAL)
    {
        info -> gx_scroll_minimum = client -> gx_rectangle_top;
        info -> gx_scroll_maximum = info -> gx_scroll_minimum + tree -> gx_tree_view_tree_height - 1;
        info -> gx_scroll_visible = (GX_VALUE)(client -> gx_rectangle_bottom - client -> gx_rectangle_top + 1);

        if (tree -> gx_tree_view_tree_height < info -> gx_scroll_visible)
        {
            info -> gx_scroll_maximum = info -> gx_scroll_minimum + info -> gx_scroll_visible - 1;
        }

        shift = tree -> gx_tree_view_y_shift;
        value = client -> gx_rectangle_top - shift;

        if (value < info -> gx_scroll_minimum)
        {
            value = info -> gx_scroll_minimum;
        }
        else if (value > info -> gx_scroll_maximum - info -> gx_scroll_visible + 1)
        {
            value = info -> gx_scroll_maximum - info -> gx_scroll_visible + 1;
        }

        shift = client -> gx_rectangle_top - value;

        if (shift != tree -> gx_tree_view_y_shift)
        {
            _gx_tree_view_scroll(tree, 0, (GX_VALUE)(shift - tree->gx_tree_view_y_shift));
        }

        info -> gx_scroll_value = value;
    }
    else
    {
        info -> gx_scroll_minimum = client -> gx_rectangle_left;
        info -> gx_scroll_maximum = info -> gx_scroll_minimum + tree -> gx_tree_view_tree_width - 1;
        info -> gx_scroll_visible = (GX_VALUE)(client -> gx_rectangle_right - client -> gx_rectangle_left + 1);

        if (tree -> gx_tree_view_tree_width < info -> gx_scroll_visible)
        {
            info -> gx_scroll_maximum = info -> gx_scroll_minimum + info -> gx_scroll_visible - 1;
        }

        shift = tree -> gx_tree_view_x_shift;
        value = client -> gx_rectangle_left - shift;

        if (value < info -> gx_scroll_minimum)
        {
            value = info -> gx_scroll_minimum;
        }
        else if (value > info -> gx_scroll_maximum - info -> gx_scroll_visible + 1)
        {
            value = info -> gx_scroll_maximum - info -> gx_scroll_visible + 1;
        }

        shift = client -> gx_rectangle_left - value;
        if (shift != tree -> gx_tree_view_x_shift)
        {
            _gx_tree_view_scroll(tree, (GX_VALUE)(shift - tree->gx_tree_view_x_shift), 0);
        }
        info -> gx_scroll_value = value;
    }

    info -> gx_scroll_increment = (GX_VALUE)(info -> gx_scroll_maximum - info -> gx_scroll_minimum) / 10;

    return(GX_SUCCESS);
}

