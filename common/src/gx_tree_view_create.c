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
/**   Tree View Management (Tree View)                                    */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_window.h"
#include "gx_widget.h"
#include "gx_tree_view.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_tree_view_create                                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates an tree view.                                 */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    tree                                  Pointer to the tree view      */
/*                                            control block               */
/*    name                                  Name of the tree view         */
/*    parent                                Parent widget control block   */
/*    style                                 Style of the widget           */
/*    tree_menu_id                          Application-defined ID of     */
/*                                          the tree view                 */
/*    size                                  Tree view size                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_window_create                     Create a window               */
/*    _gx_widget_link                       Link a widget to its parent   */
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
/*                                            fixed compiler warnings,    */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
UINT  _gx_tree_view_create(GX_TREE_VIEW *tree, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                           ULONG style, USHORT tree_menu_id, GX_CONST GX_RECTANGLE *size)
{

    /* Call the window create function.  */
    _gx_window_create((GX_WINDOW *)tree, name, GX_NULL, style, tree_menu_id, size);

    /* Populate the rest of the tree view control block - overriding as necessary.  */
    tree -> gx_widget_type = GX_TYPE_TREE_VIEW;
    tree -> gx_widget_event_process_function = (UINT (*)(GX_WIDGET *, GX_EVENT *))_gx_tree_view_event_process;
    tree -> gx_widget_draw_function = (VOID (*)(GX_WIDGET *))_gx_tree_view_draw;
    tree -> gx_window_scroll_info_get = (VOID (*)(struct GX_WINDOW_STRUCT *, ULONG, GX_SCROLL_INFO *))(void (*)(void))_gx_tree_view_scroll_info_get;
    tree -> gx_tree_view_collapse_pixelmap_id = 0;
    tree -> gx_tree_view_expand_pixelmap_id = 0;
    tree -> gx_tree_view_root_line_color = GX_COLOR_ID_SHADOW;
    tree -> gx_tree_view_indentation = 22;
    tree -> gx_tree_view_x_shift = 0;
    tree -> gx_tree_view_y_shift = 0;
    tree -> gx_tree_view_tree_width = 0;
    tree -> gx_tree_view_tree_height = 0;
    tree -> gx_tree_view_selected = GX_NULL;

    /* Determine if a parent widget was provided.  */
    if (parent)
    {
        _gx_widget_link(parent, (GX_WIDGET *)tree);
    }

    /* Return completion status code. */
    return(GX_SUCCESS);
}

