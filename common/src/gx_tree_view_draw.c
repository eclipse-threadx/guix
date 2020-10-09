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
/**   Tree View Management (View)                                         */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_tree_view.h"
#include "gx_window.h"
#include "gx_widget.h"
#include "gx_utility.h"
#include "gx_system.h"
#include "gx_context.h"
#include "gx_canvas.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_tree_view_root_draw_helper                      PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to draw the expand/collapse icons and root */
/*    lines for one level menu items.                                     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    tree                                  Pointer the to tree view      */
/*                                            control block               */
/*    start                                 Pointer to start widget       */
/*    owner                                 Parent of the menu item       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Compoletion status            */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_context_pixelmap_get              Get pixelmap associate with   */
/*                                            the supplied id             */
/*    _gx_context_brush_get                 Get brush                     */
/*    _gx_canvas_pixelmap_draw              Draw a pixelmap to canvas     */
/*    _gx_canvas_line_draw                  Draw a line to canvas         */
/*    _gx_widget_first_client_child_get     Get the first client child    */
/*    _gx_widget_next_client_child_get      Get the next chilent child    */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_tree_view_root_draw                                             */
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
static VOID _gx_tree_view_root_draw_helper(GX_TREE_VIEW *tree, GX_WIDGET *start, GX_WIDGET *owner)
{
GX_PIXELMAP  *map;
GX_PIXELMAP  *collapse_map;
GX_PIXELMAP  *expand_map;
GX_WIDGET    *child;
GX_MENU_LIST *list;
GX_VALUE      xpos;
GX_VALUE      ypos;
GX_VALUE      yend;
GX_VALUE      map_width = 0;
GX_VALUE      map_height = 0;
ULONG         vertical_mask = 0x80000000;
GX_BRUSH     *brush;
GX_WIDGET    *pre = GX_NULL;

    _gx_context_pixelmap_get(tree -> gx_tree_view_collapse_pixelmap_id, &collapse_map);
    _gx_context_pixelmap_get(tree -> gx_tree_view_expand_pixelmap_id, &expand_map);

    if (collapse_map)
    {
        map_width = collapse_map -> gx_pixelmap_width;
        map_height = collapse_map -> gx_pixelmap_height;
    }

    child = _gx_widget_first_client_child_get(start);

    while (child)
    {
        if (child -> gx_widget_type == GX_TYPE_MENU_LIST)
        {
            /* Not a valid item type. */
            child = _gx_widget_next_client_child_get(child);
            continue;
        }

        map = GX_NULL;
        xpos = (GX_VALUE)(child -> gx_widget_size.gx_rectangle_left - (tree -> gx_tree_view_indentation >> 1));
        ypos = (GX_VALUE)(child -> gx_widget_size.gx_rectangle_top + child -> gx_widget_size.gx_rectangle_bottom - 1);
        ypos = (GX_VALUE)(ypos >> 1);

        if (child -> gx_widget_type == GX_TYPE_MENU)
        {
            list = &((GX_MENU *)child) -> gx_menu_list;

            if (list -> gx_widget_first_child)
            {
                if (child -> gx_widget_style & GX_STYLE_MENU_EXPANDED)
                {
                    map = collapse_map;
                }
                else
                {
                    map = expand_map;
                }

                if (map)
                {
                    /* Calculate collapse/expand icon draw area. */
                    xpos = (GX_VALUE)(xpos - (map_width >> 1));
                    ypos = (GX_VALUE)(ypos - (map_height >> 1));

                    /* Draw collapse/expand icons. */
                    _gx_canvas_pixelmap_draw(xpos, ypos, map);
                }
            }
        }

        if (tree -> gx_widget_style & GX_STYLE_TREE_VIEW_SHOW_ROOT_LINES)
        {
            /* Draw root lines.*/
            _gx_context_brush_get(&brush);
            brush -> gx_brush_pattern_mask = 0x80000000;

            if (map)
            {
                /* Get right center of the collapse/expand icon. */
                xpos = (GX_VALUE)(xpos + map_width);
                ypos = (GX_VALUE)(ypos + (map_height >> 1));
            }
            else
            {
                xpos = (GX_VALUE)(child -> gx_widget_size.gx_rectangle_left - (tree -> gx_tree_view_indentation >> 1) + 1);
                ypos = (GX_VALUE)((child -> gx_widget_size.gx_rectangle_top + child -> gx_widget_size.gx_rectangle_bottom) >> 1);
            }

            /* Draw horizontal root line that connection collapse/expand icon with the menu item. */
            _gx_canvas_line_draw(xpos, ypos, (GX_VALUE)(child -> gx_widget_size.gx_rectangle_left - 1), ypos);

            if (pre || owner)
            {
                if (map)
                {
                    /* Get top center of the collapse/expand icon. */
                    xpos = (GX_VALUE)(xpos - (map_width >> 1));
                    ypos = (GX_VALUE)(ypos - (map_height >> 1) - 1);
                }
                else
                {
                    xpos = (GX_VALUE)(xpos - 1);
                }

                if (pre)
                {
                    if ((pre -> gx_widget_type == GX_TYPE_MENU) &&
                        ((GX_MENU *)pre) -> gx_menu_list.gx_widget_first_child)
                    {
                        yend = (GX_VALUE)((pre -> gx_widget_size.gx_rectangle_top + pre -> gx_widget_size.gx_rectangle_bottom) >> 1);
                        yend = (GX_VALUE)(yend - (map_height >> 1) + map_height);
                    }
                    else
                    {
                        yend = (GX_VALUE)((pre -> gx_widget_size.gx_rectangle_top + pre -> gx_widget_size.gx_rectangle_bottom) >> 1);
                        yend = (GX_VALUE)(yend + 1);
                    }
                }
                else
                {
                    yend = owner -> gx_widget_size.gx_rectangle_bottom;
                }

                brush -> gx_brush_pattern_mask = vertical_mask;

                /* Draw vertical root line that connect to the previous or parent item. */
                _gx_canvas_line_draw(xpos, yend, xpos, ypos);

                vertical_mask = brush -> gx_brush_pattern_mask;
            }
        }

        pre = child;

        /* Pick next client child. */
        child = _gx_widget_next_client_child_get(child);
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_tree_view_root_draw                             PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to draw the expand/collapse icons and root */
/*    lines.                                                              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    tree                                  Pointer the to tree view      */
/*                                            control block               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Compoletion status            */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_first_client_child_get     Get the first client child    */
/*    _gx_widget_next_client_child_get      Get the next chilent child    */
/*    _gx_tree_view_root_draw_helper        Draw tree view root           */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_tree_view_draw                                                  */
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
static VOID _gx_tree_view_root_draw(GX_TREE_VIEW *tree)
{
GX_WIDGET    *child;
GX_WIDGET    *next;
GX_MENU_LIST *list;
GX_WIDGET    *parent = (GX_WIDGET *)tree;
GX_WIDGET    *owner;

    child = _gx_widget_first_client_child_get(parent);

    while (child)
    {
        if (child -> gx_widget_type == GX_TYPE_MENU)
        {
            list = &((GX_MENU *)child) -> gx_menu_list;

            if ((list -> gx_widget_first_child) &&
                (list -> gx_widget_status & GX_STATUS_VISIBLE))
            {
                child = list -> gx_widget_first_child;
                continue;
            }
        }

        /* Pick next client child. */
        next = _gx_widget_next_client_child_get(child);

        while ((next == GX_NULL) && (child != parent))
        {
            child = child -> gx_widget_parent;

            if (child -> gx_widget_type == GX_TYPE_MENU_LIST)
            {
                owner = ((GX_MENU_LIST *)child) -> gx_menu_list_owner;
                next = owner;
            }
            else
            {
                owner = GX_NULL;
                next = child;
            }

            _gx_tree_view_root_draw_helper(tree, child, owner);

            child = next;
            next = _gx_widget_next_client_child_get(next);
        }

        if (child == parent)
        {
            break;
        }

        child = next;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_tree_view_draw                                  PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function draws the specified tree view, which is a special type*/
/*    of widget.                                                          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    tree                                  Pointer the to tree view      */
/*                                            control block               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_window_background_draw            Draw window background        */
/*    _gx_context_brush_pattern_set         Set brush pattern             */
/*    _gx_context_brush_width_set           Set brush width               */
/*    _gx_context_line_color_set            Set line color                */
/*    _gx_utility_rectangle_overlap_detect  Detect overlap of supplied    */
/*                                            rectangles                  */
/*    _gx_tree_view_root_draw               Draw root lines and icons     */
/*    _gx_widget_children_draw              Draw widget children          */
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
VOID  _gx_tree_view_draw(GX_TREE_VIEW *tree)
{
GX_DRAW_CONTEXT *context;
GX_RECTANGLE     dirty_area;

    /* Draw tree menu background. */
    _gx_window_background_draw((GX_WINDOW *)tree);

    _gx_context_brush_pattern_set(GX_TREE_VIEW_ROOT_LINE_PATTERN);

    _gx_context_brush_width_set(1);
    _gx_context_line_color_set(tree -> gx_tree_view_root_line_color);

    /* pick up the current context */
    context = _gx_system_current_draw_context;
    dirty_area = context -> gx_draw_context_dirty;

    _gx_utility_rectangle_overlap_detect(&tree -> gx_window_client,
                                         &context -> gx_draw_context_dirty,
                                         &context -> gx_draw_context_dirty);

    /* Draw collapse/expand icons and root lines. */
    _gx_tree_view_root_draw(tree);

    context -> gx_draw_context_dirty = dirty_area;

    _gx_context_brush_pattern_set(0);

    /* Draw all the child widgets.  */
    _gx_widget_children_draw((GX_WIDGET *)tree);
}

