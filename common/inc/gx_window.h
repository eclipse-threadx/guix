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
/**   Window Management (Window)                                          */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/
/*                                                                        */
/*  COMPONENT DEFINITION                                   RELEASE        */
/*                                                                        */
/*    gx_window.h                                         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file defines the GUIX window management component,             */
/*    including all data types and external references.  It is assumed    */
/*    that gx_api.h and gx_port.h have already been included.             */
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

#ifndef GX_WINDOW_H
#define GX_WINDOW_H

#define LIST_CHILD_ID_START 4096
#define GX_SNAP_TIMER       1000
#define GX_FLICK_TIMER      1001

/* Define vertical list management function prototypes. */

UINT _gx_horizontal_list_children_position(GX_HORIZONTAL_LIST *list);
UINT _gx_horizontal_list_create(GX_HORIZONTAL_LIST *horizontal_list, GX_CONST GX_CHAR *name,
                                GX_WIDGET *parent, INT total_columns,
                                VOID (*callback)(GX_HORIZONTAL_LIST *, GX_WIDGET *, INT),
                                ULONG style, USHORT horizontal_list_id, GX_CONST GX_RECTANGLE *size);

UINT _gx_horizontal_list_event_process(GX_HORIZONTAL_LIST *list, GX_EVENT *event_ptr);
VOID _gx_horizontal_list_left_wrap(GX_HORIZONTAL_LIST *list);
UINT _gx_horizontal_list_page_index_set(GX_HORIZONTAL_LIST *list, INT index);
VOID _gx_horizontal_list_right_wrap(GX_HORIZONTAL_LIST *list);
UINT _gx_horizontal_list_selected_index_get(GX_HORIZONTAL_LIST *horizontal_list, INT *return_index);
UINT _gx_horizontal_list_selected_widget_get(GX_HORIZONTAL_LIST *horizontal_list, GX_WIDGET **return_list_entry);
UINT _gx_horizontal_list_selected_set(GX_HORIZONTAL_LIST *horizontal_list, INT entry);
VOID _gx_horizontal_list_selected_visible(GX_HORIZONTAL_LIST *list, GX_WIDGET *child);
VOID _gx_horizontal_list_scroll(GX_HORIZONTAL_LIST *list, INT amount);
VOID _gx_horizontal_list_scroll_info_get(GX_WINDOW *win, ULONG style, GX_SCROLL_INFO *info);
VOID _gx_horizontal_list_slide_back_check(GX_HORIZONTAL_LIST *list);
UINT _gx_horizontal_list_total_columns_set(GX_HORIZONTAL_LIST *list, INT count);

UINT _gx_popup_list_event_process(GX_POPUP_LIST *popup_list, GX_EVENT *event_ptr);

UINT _gx_vertical_list_children_position(GX_VERTICAL_LIST *list);
UINT _gx_vertical_list_create(GX_VERTICAL_LIST *vertical_list, GX_CONST GX_CHAR *name,
                              GX_WIDGET *parent, INT total_rows,
                              VOID (*callback)(GX_VERTICAL_LIST *, GX_WIDGET *, INT),
                              ULONG style, USHORT vertical_list_id, GX_CONST GX_RECTANGLE *size);

UINT _gx_vertical_list_event_process(GX_VERTICAL_LIST *list, GX_EVENT *event_ptr);
VOID _gx_vertical_list_down_wrap(GX_VERTICAL_LIST *list);
UINT _gx_vertical_list_page_index_set(GX_VERTICAL_LIST *list, INT index);
UINT _gx_vertical_list_selected_index_get(GX_VERTICAL_LIST *vertical_list, INT *return_index);
UINT _gx_vertical_list_selected_widget_get(GX_VERTICAL_LIST *vertical_list, GX_WIDGET **return_list_entry);
UINT _gx_vertical_list_selected_set(GX_VERTICAL_LIST *vertical_list, INT index);
VOID _gx_vertical_list_selected_visible(GX_VERTICAL_LIST *list, GX_WIDGET *child);
VOID _gx_vertical_list_scroll(GX_VERTICAL_LIST *list, INT amount);
VOID _gx_vertical_list_scroll_info_get(GX_VERTICAL_LIST *list, ULONG style, GX_SCROLL_INFO *info);
VOID _gx_vertical_list_slide_back_check(GX_VERTICAL_LIST *list);
UINT _gx_vertical_list_total_rows_set(GX_VERTICAL_LIST *list, INT count);
VOID _gx_vertical_list_up_wrap(GX_VERTICAL_LIST *list);

/* Define window management function prototypes.  */

VOID _gx_window_background_draw(GX_WINDOW *win);
VOID _gx_window_border_draw(GX_WINDOW *win, GX_COLOR fill_color);
UINT _gx_window_client_scroll(GX_WINDOW *window, GX_VALUE x_scroll, GX_VALUE y_scroll);
UINT _gx_window_client_height_get(GX_WINDOW *window, GX_VALUE *return_height);
UINT _gx_window_client_width_get(GX_WINDOW *window, GX_VALUE *return_width);
UINT _gx_window_close(GX_WINDOW *window);
UINT _gx_window_create(GX_WINDOW *window, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                       ULONG style, USHORT Id, GX_CONST GX_RECTANGLE *size);
VOID _gx_window_draw(GX_WINDOW *window);
UINT _gx_window_event_process(GX_WINDOW *window, GX_EVENT *event_ptr);
UINT _gx_window_execute(GX_WINDOW *window, ULONG *return_ptr);

UINT _gx_window_root_create(GX_WINDOW_ROOT *root_window, GX_CONST GX_CHAR *name, GX_CANVAS *canvas,
                            ULONG style, USHORT Id, GX_CONST GX_RECTANGLE *size);
UINT _gx_window_root_delete(GX_WINDOW_ROOT *root_window);
UINT _gx_window_root_find(GX_WIDGET *widget, GX_WINDOW_ROOT **return_root_window);
UINT _gx_window_root_event_process(GX_WINDOW_ROOT *root, GX_EVENT *event_ptr);
UINT _gx_window_scroll(GX_WINDOW *window, GX_VALUE x_scroll, GX_VALUE y_scroll);
UINT _gx_window_scroll_info_get(GX_WINDOW *window, ULONG type, GX_SCROLL_INFO *return_scroll_info);
UINT _gx_window_scrollbar_find(GX_WINDOW *window, USHORT type, GX_SCROLLBAR **return_scrollbar);
VOID _gx_window_view_update_detect(GX_WINDOW *win);
UINT _gx_window_wallpaper_get(GX_WINDOW *window, GX_RESOURCE_ID *return_wallpaper_id);
UINT _gx_window_wallpaper_set(GX_WINDOW *window, GX_RESOURCE_ID wallpaper_id, GX_BOOL tile);

/* Define error checking shells for API services.  These are only referenced by the
   application.  */


UINT _gxe_window_client_scroll(GX_WINDOW *window, GX_VALUE x_scroll, GX_VALUE y_scroll);
UINT _gxe_window_client_height_get(GX_WINDOW *window, GX_VALUE *return_height);
UINT _gxe_window_client_width_get(GX_WINDOW *window, GX_VALUE *return_width);
UINT _gxe_window_close(GX_WINDOW *window);
UINT _gxe_window_create(GX_WINDOW *window, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                        ULONG style, USHORT Id, GX_CONST GX_RECTANGLE *size, UINT window_control_block_size);
UINT _gxe_window_event_process(GX_WINDOW *window, GX_EVENT *event_ptr);
UINT _gxe_window_execute(GX_WINDOW *window, ULONG *return_ptr);
UINT _gxe_window_root_find(GX_WIDGET *widget, GX_WINDOW_ROOT **return_root_window);
UINT _gxe_window_scroll_info_get(GX_WINDOW *window, ULONG type, GX_SCROLL_INFO *return_scroll_info);

UINT _gxe_window_scrollbar_find(GX_WINDOW *window, USHORT type, GX_SCROLLBAR **return_scrollbar);
UINT _gxe_window_wallpaper_get(GX_WINDOW *window, GX_RESOURCE_ID *return_wallpaper_id);
UINT _gxe_window_wallpaper_set(GX_WINDOW *window, GX_RESOURCE_ID wallpaper_id, GX_BOOL tile);


UINT _gxe_horizontal_list_children_position(GX_HORIZONTAL_LIST *list);
UINT _gxe_horizontal_list_create(GX_HORIZONTAL_LIST *horizontal_list,
                                 GX_CONST GX_CHAR *name,
                                 GX_WIDGET *parent, INT total_rows,
                                 VOID (*callback)(GX_HORIZONTAL_LIST *, GX_WIDGET *, INT),
                                 ULONG style, USHORT horizontal_list_id,
                                 GX_CONST GX_RECTANGLE *size,
                                 UINT horizontal_list_control_block_size);
UINT _gxe_horizontal_list_event_process(GX_HORIZONTAL_LIST *list, GX_EVENT *event_ptr);
UINT _gxe_horizontal_list_selected_index_get(GX_HORIZONTAL_LIST *horizontal_list, INT *return_index);
UINT _gxe_horizontal_list_selected_set(GX_HORIZONTAL_LIST *horizontal_list, INT index);
UINT _gxe_horizontal_list_selected_widget_get(GX_HORIZONTAL_LIST *horizontal_list, GX_WIDGET **return_list_entry);
UINT _gxe_horizontal_list_total_columns_set(GX_HORIZONTAL_LIST *list, INT count);
UINT _gxe_horizontal_list_page_index_set(GX_HORIZONTAL_LIST *vertical_list, INT index);

UINT _gxe_vertical_list_children_position(GX_VERTICAL_LIST *list);
UINT _gxe_vertical_list_create(GX_VERTICAL_LIST *vertical_list, GX_CONST GX_CHAR *name,
                               GX_WIDGET *parent, INT total_rows,
                               VOID (*callback)(GX_VERTICAL_LIST *, GX_WIDGET *, INT),
                               ULONG style, USHORT vertical_list_id, GX_CONST GX_RECTANGLE *size,
                               UINT vertical_list_control_block_size);
UINT _gxe_vertical_list_event_process(GX_VERTICAL_LIST *list, GX_EVENT *event_ptr);
UINT _gxe_vertical_list_selected_set(GX_VERTICAL_LIST *vertical_list, INT index);
UINT _gxe_vertical_list_selected_index_get(GX_VERTICAL_LIST *vertical_list, INT *return_index);
UINT _gxe_vertical_list_selected_widget_get(GX_VERTICAL_LIST *vertical_list, GX_WIDGET **return_list_entry);
UINT _gxe_vertical_list_total_rows_set(GX_VERTICAL_LIST *list, INT count);
UINT _gxe_vertical_list_page_index_set(GX_VERTICAL_LIST *horizontal_list, INT list_entry);

UINT _gxe_window_root_create(GX_WINDOW_ROOT *root_window, GX_CONST GX_CHAR *name,
                             GX_CANVAS *canvas, ULONG style, USHORT Id, GX_CONST GX_RECTANGLE *size, UINT root_window_control_block_size);
UINT _gxe_window_root_delete(GX_WINDOW_ROOT *root_window);
UINT _gxe_window_root_event_process(GX_WINDOW_ROOT *root, GX_EVENT *event_ptr);

#endif

