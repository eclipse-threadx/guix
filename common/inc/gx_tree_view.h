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
/**************************************************************************/

/**************************************************************************/
/*                                                                        */
/*  COMPONENT DEFINITION                                   RELEASE        */
/*                                                                        */
/*    gx_tree_view.h                                      PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file defines the GUIX tree view component, including all data  */
/*    types and external references.  It is assumed that gx_api.h and     */
/*    gx_port.h have already been included.                               */
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

#ifndef GX_TREE_VIEW_H
#define GX_TREE_VIEW_H

#define GX_TREE_VIEW_ROOT_LINE_PATTERN 0x55555555

/* Define tree view management function prototypes.  */

UINT _gx_tree_view_create(GX_TREE_VIEW *tree, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                          ULONG style, USHORT tree_menu_id, GX_CONST GX_RECTANGLE *size);
VOID _gx_tree_view_draw(GX_TREE_VIEW *tree);
UINT _gx_tree_view_event_process(GX_TREE_VIEW *tree, GX_EVENT *event_ptr);
UINT _gx_tree_view_indentation_set(GX_TREE_VIEW *tree, GX_VALUE indentation);
UINT _gx_tree_view_position(GX_TREE_VIEW *tree);
UINT _gx_tree_view_root_line_color_set(GX_TREE_VIEW *tree, GX_RESOURCE_ID color);
UINT _gx_tree_view_root_pixelmap_set(GX_TREE_VIEW *tree, GX_RESOURCE_ID expand_map_id, GX_RESOURCE_ID collapse_map_id);
UINT _gx_tree_view_scroll(GX_TREE_VIEW *tree, GX_VALUE x_scroll, GX_VALUE y_scroll);
UINT _gx_tree_view_scroll_info_get(GX_TREE_VIEW *tree, ULONG type, GX_SCROLL_INFO *return_scroll_info);
UINT _gx_tree_view_selected_get(GX_TREE_VIEW *tree, GX_WIDGET **selected);
UINT _gx_tree_view_selected_set(GX_TREE_VIEW *tree, GX_WIDGET *selected);

/* error checking versions of the tree view API functions */

UINT _gxe_tree_view_create(GX_TREE_VIEW *tree, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                           ULONG style, USHORT tree_id, GX_CONST GX_RECTANGLE *size, UINT control_block_size);
UINT _gxe_tree_view_event_process(GX_TREE_VIEW *tree, GX_EVENT *event_ptr);
UINT _gxe_tree_view_indentation_set(GX_TREE_VIEW *tree, GX_VALUE indentation);
UINT _gxe_tree_view_position(GX_TREE_VIEW *tree);
UINT _gxe_tree_view_root_line_color_set(GX_TREE_VIEW *tree, GX_RESOURCE_ID color);
UINT _gxe_tree_view_root_pixelmap_set(GX_TREE_VIEW *tree, GX_RESOURCE_ID expand_map_id, GX_RESOURCE_ID collapse_map_id);
UINT _gxe_tree_view_selected_get(GX_TREE_VIEW *tree, GX_WIDGET **selected);
UINT _gxe_tree_view_selected_set(GX_TREE_VIEW *tree, GX_WIDGET *selected);

#endif

