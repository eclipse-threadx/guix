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
/**   Widget Management (Widget)                                          */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/
/*                                                                        */
/*  COMPONENT DEFINITION                                   RELEASE        */
/*                                                                        */
/*    gx_widget.h                                         PORTABLE C      */
/*                                                           6.1.7        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file defines the GUIX widget management component,             */
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
/*  06-02-2021     Ting Zhu                 Modified comment(s),          */
/*                                            added new child widget      */
/*                                            search prototypes,          */
/*                                            resulting in version 6.1.7  */
/*                                                                        */
/**************************************************************************/

#ifndef GX_WIDGET_H
#define GX_WIDGET_H


/* Define widget management function prototypes.  */

UINT       _gx_widget_allocate(GX_WIDGET **widget, ULONG memsize);
UINT       _gx_widget_attach(GX_WIDGET *parent, GX_WIDGET *child);
UINT       _gx_widget_back_attach(GX_WIDGET *parent, GX_WIDGET *child);
VOID       _gx_widget_background_draw(GX_WIDGET *widget);
UINT       _gx_widget_back_move(GX_WIDGET *widget, GX_BOOL *widget_moved);
UINT       _gx_widget_block_move(GX_WIDGET *widget, GX_RECTANGLE *block, INT x_shift, INT y_shift);
VOID       _gx_widget_border_draw(GX_WIDGET *widget, GX_RESOURCE_ID border_color, GX_RESOURCE_ID upper_color, GX_RESOURCE_ID lower_color, GX_BOOL fill);
UINT       _gx_widget_border_style_set(GX_WIDGET *widget, ULONG Style);
UINT       _gx_widget_border_width_get(GX_WIDGET *widget, GX_VALUE *return_width);
UINT       _gx_widget_canvas_get(GX_WIDGET *widget, GX_CANVAS **return_canvas);
UINT       _gx_widget_child_detect(GX_WIDGET *parent, GX_WIDGET *child, GX_BOOL *return_detect);
VOID       _gx_widget_children_draw(GX_WIDGET *widget);
VOID       _gx_widget_children_event_process(GX_WIDGET *widget, GX_EVENT *event_ptr);
UINT       _gx_widget_client_get(GX_WIDGET *widget, GX_VALUE border_width, GX_RECTANGLE *return_size);
VOID       _gx_widget_clipping_update(GX_WIDGET *widget);
UINT       _gx_widget_color_get(GX_WIDGET *widget, GX_RESOURCE_ID color_id, GX_COLOR *color);
VOID       _gx_widget_child_clipping_update(GX_WIDGET *parent);
VOID       _gx_widget_child_focus_assign(GX_WIDGET *parent);
VOID       _gx_widget_context_fill_set(GX_WIDGET *widget);
UINT       _gx_widget_create(GX_WIDGET *widget, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                             ULONG style, USHORT Id, GX_CONST GX_RECTANGLE *size);
UINT       _gx_widget_created_test(GX_WIDGET *widget, GX_BOOL *return_test);
UINT       _gx_widget_detach(GX_WIDGET *child);
UINT       _gx_widget_delete(GX_WIDGET *widget);
VOID       _gx_widget_draw(GX_WIDGET *widget);
UINT       _gx_widget_draw_set(GX_WIDGET *widget, VOID (*draw_func)(GX_WIDGET *));
UINT       _gx_widget_event_generate(GX_WIDGET *widget, USHORT event_type, LONG value);
UINT       _gx_widget_event_process(GX_WIDGET *widget, GX_EVENT *event_ptr);
UINT       _gx_widget_event_process_set(GX_WIDGET *widget, UINT (*event_processing_function)(GX_WIDGET *, GX_EVENT *));
UINT       _gx_widget_event_to_parent(GX_WIDGET *widget, GX_EVENT *event_ptr);
UINT       _gx_widget_fill_color_set(GX_WIDGET *widget, GX_RESOURCE_ID normal_color, GX_RESOURCE_ID selected_color, GX_RESOURCE_ID disabled_color);
UINT       _gx_widget_find(GX_WIDGET *parent, USHORT widget_id, INT search_depth, GX_WIDGET **return_widget);
UINT       _gx_widget_first_child_get(GX_WIDGET *parent, GX_WIDGET **child_return);
UINT       _gx_widget_font_get(GX_WIDGET *widget, GX_RESOURCE_ID font_id, GX_FONT **return_font);
UINT       _gx_widget_free(GX_WIDGET *widget);
UINT       _gx_widget_front_move(GX_WIDGET *widget, GX_BOOL *return_moved);
UINT       _gx_widget_focus_next(GX_WIDGET *widget);
UINT       _gx_widget_focus_previous(GX_WIDGET *widget);
UINT       _gx_widget_height_get(GX_WIDGET *widget, GX_VALUE *return_height);
UINT       _gx_widget_hide(GX_WIDGET *widget);
UINT       _gx_widget_last_child_get(GX_WIDGET *parent, GX_WIDGET **child_return);
VOID       _gx_widget_link(GX_WIDGET *parent, GX_WIDGET *child);
VOID       _gx_widget_nav_order_initialize(GX_WIDGET *widget);
UINT       _gx_widget_next_sibling_get(GX_WIDGET *current, GX_WIDGET **sibling_return);
UINT       _gx_widget_parent_get(GX_WIDGET *current, GX_WIDGET **parent_return);
UINT       _gx_widget_previous_sibling_get(GX_WIDGET *current, GX_WIDGET **sibling_return);
UINT       _gx_widget_pixelmap_get(GX_WIDGET *widget, GX_RESOURCE_ID pixelmap_id, GX_PIXELMAP **return_map);
VOID       _gx_widget_back_link(GX_WIDGET *parent, GX_WIDGET *child);
UINT       _gx_widget_resize(GX_WIDGET *widget, GX_RECTANGLE *new_size);
UINT       _gx_widget_scroll_shift(GX_WIDGET *widget, INT xShift, INT yShift, GX_BOOL clip);
UINT       _gx_widget_shift(GX_WIDGET *widget, GX_VALUE x_shift, GX_VALUE  y_shift, GX_BOOL mark_dirty);
UINT       _gx_widget_show(GX_WIDGET *widget);
UINT       _gx_widget_status_add(GX_WIDGET *widget, ULONG status);
UINT       _gx_widget_status_get(GX_WIDGET *widget, ULONG *return_status);
UINT       _gx_widget_status_remove(GX_WIDGET *widget, ULONG status);
UINT       _gx_widget_status_test(GX_WIDGET *widget, ULONG status, GX_BOOL *return_test);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT       _gx_widget_string_get(GX_WIDGET *widget, GX_RESOURCE_ID string_id, GX_CONST GX_CHAR **return_string);
#endif
UINT       _gx_widget_string_get_ext(GX_WIDGET *widget, GX_RESOURCE_ID string_id, GX_STRING *return_string);

UINT       _gx_widget_style_add(GX_WIDGET *widget, ULONG style);
UINT       _gx_widget_style_get(GX_WIDGET *widget, ULONG *return_style);
UINT       _gx_widget_style_remove(GX_WIDGET *widget, ULONG style);
UINT       _gx_widget_style_set(GX_WIDGET *widget, ULONG style);

#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT       _gx_widget_text_blend(GX_WIDGET *widget, UINT tColor, UINT font_id, GX_CONST GX_CHAR *string,
                           INT x_offset, INT y_offset, UCHAR alpha);
VOID       _gx_widget_text_draw(GX_WIDGET *widget, UINT tColor, UINT font_id, GX_CONST GX_CHAR *string,
                          INT x_offset, INT y_offset);
#endif
UINT       _gx_widget_text_blend_ext(GX_WIDGET *widget, UINT tColor, UINT font_id, GX_CONST GX_STRING *string,
                                     INT x_offset, INT y_offset, UCHAR alpha);
VOID       _gx_widget_text_draw_ext(GX_WIDGET *widget, UINT tColor, UINT font_id, GX_CONST GX_STRING *string,
                                    INT x_offset, INT y_offset);
VOID       _gx_widget_text_id_draw(GX_WIDGET *widget, UINT tColor, UINT font_id, UINT text_id,
                                   INT x_offset, INT y_offset);
UINT       _gx_widget_top_visible_child_find(GX_WIDGET *parent, GX_WIDGET **child_return);
GX_BOOL    _gx_widget_transparent_pixelmap_detect(GX_WIDGET *widget, GX_RESOURCE_ID pixelmap_id);
UINT       _gx_widget_type_find(GX_WIDGET *parent, USHORT widget_id, GX_WIDGET **return_widget);

GX_WIDGET *_gx_widget_unlink(GX_WIDGET *widget);
GX_WIDGET *_gx_widget_first_client_child_get(GX_WIDGET *parent);
GX_WIDGET *_gx_widget_first_visible_client_child_get(GX_WIDGET *parent);
GX_WIDGET *_gx_widget_last_client_child_get(GX_WIDGET *parent);
GX_WIDGET *_gx_widget_last_visible_client_child_get(GX_WIDGET *parent);
GX_WIDGET *_gx_widget_next_client_child_get(GX_WIDGET *current);
GX_WIDGET *_gx_widget_next_visible_client_child_get(GX_WIDGET *current);
INT        _gx_widget_client_index_get(GX_WIDGET *parent, GX_WIDGET *child);

UINT       _gx_widget_width_get(GX_WIDGET *widget, GX_VALUE *return_width);


/* Define error checking shells for API services.  These are only referenced by the
   application.  */

UINT _gxe_widget_allocate(GX_WIDGET **widget, ULONG memsize);
UINT _gxe_widget_attach(GX_WIDGET *parent, GX_WIDGET *child);
UINT _gxe_widget_back_attach(GX_WIDGET *parent, GX_WIDGET *child);
UINT _gxe_widget_back_move(GX_WIDGET *widget, GX_BOOL *widget_moved);
UINT _gxe_widget_block_move(GX_WIDGET *widget, GX_RECTANGLE *block, INT x_shift, INT y_shift);
UINT _gxe_widget_border_style_set(GX_WIDGET *widget, ULONG Style);
UINT _gxe_widget_border_width_get(GX_WIDGET *widget, GX_VALUE *return_width);
UINT _gxe_widget_canvas_get(GX_WIDGET *widget, GX_CANVAS **return_canvas);
UINT _gxe_widget_child_detect(GX_WIDGET *parent, GX_WIDGET *child, GX_BOOL *return_detect);
UINT _gxe_widget_client_get(GX_WIDGET *widget, GX_VALUE border_width, GX_RECTANGLE *return_size);
UINT _gxe_widget_color_get(GX_WIDGET *widget, GX_RESOURCE_ID color_id, GX_COLOR *return_color);
UINT _gxe_widget_create(GX_WIDGET *widget, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                        ULONG style, USHORT Id, GX_CONST GX_RECTANGLE *size, UINT widget_block_size);
UINT _gxe_widget_created_test(GX_WIDGET *widget, GX_BOOL *return_test);
UINT _gxe_widget_delete(GX_WIDGET *widget);
UINT _gxe_widget_detach(GX_WIDGET *widget);
UINT _gxe_widget_draw_set(GX_WIDGET *widget, VOID (*draw_func)(GX_WIDGET *));
UINT _gxe_widget_event_generate(GX_WIDGET *widget, USHORT event_type, LONG value);
UINT _gxe_widget_event_process(GX_WIDGET *widget, GX_EVENT *event_ptr);
UINT _gxe_widget_event_process_set(GX_WIDGET *widget, UINT (*event_processing_function)(GX_WIDGET *, GX_EVENT *));
UINT _gxe_widget_fill_color_set(GX_WIDGET *widget, GX_RESOURCE_ID normal_color, GX_RESOURCE_ID selected_color, GX_RESOURCE_ID disabled_color);
UINT _gxe_widget_find(GX_WIDGET *parent, USHORT widget_id, INT search_depth, GX_WIDGET **return_widget);
UINT _gxe_widget_first_child_get(GX_WIDGET *parent, GX_WIDGET **child_return);
UINT _gxe_widget_focus_next(GX_WIDGET *widget);
UINT _gxe_widget_focus_previous(GX_WIDGET *widget);
UINT _gxe_widget_font_get(GX_WIDGET *widget, GX_RESOURCE_ID resource_id, GX_FONT **return_font);
UINT _gxe_widget_free(GX_WIDGET *widget);
UINT _gxe_widget_front_move(GX_WIDGET *widget, GX_BOOL *return_moved);
UINT _gxe_widget_height_get(GX_WIDGET *widget, GX_VALUE *return_height);
UINT _gxe_widget_hide(GX_WIDGET *widget);
UINT _gxe_widget_last_child_get(GX_WIDGET *parent, GX_WIDGET **child_return);
UINT _gxe_widget_next_sibling_get(GX_WIDGET *current, GX_WIDGET **sibling_return);
UINT _gxe_widget_parent_get(GX_WIDGET *current, GX_WIDGET **parent_return);
UINT _gxe_widget_previous_sibling_get(GX_WIDGET *current, GX_WIDGET **sibling_return);
UINT _gxe_widget_pixelmap_get(GX_WIDGET *widget, GX_RESOURCE_ID resource_id, GX_PIXELMAP **return_pixelmap);
UINT _gxe_widget_resize(GX_WIDGET *widget, GX_RECTANGLE *new_size);
UINT _gxe_widget_shift(GX_WIDGET *widget, GX_VALUE x_shift, GX_VALUE y_shift, GX_BOOL mark_dirty);
UINT _gxe_widget_show(GX_WIDGET *widget);
UINT _gxe_widget_status_add(GX_WIDGET *widget, ULONG status);
UINT _gxe_widget_status_get(GX_WIDGET *widget, ULONG *return_status);
UINT _gxe_widget_status_remove(GX_WIDGET *widget, ULONG status);
UINT _gxe_widget_status_test(GX_WIDGET *widget, ULONG status, GX_BOOL *return_test);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gxe_widget_string_get(GX_WIDGET *widget, GX_RESOURCE_ID string_id, GX_CONST GX_CHAR **return_string);
#endif
UINT _gxe_widget_string_get_ext(GX_WIDGET *widget, GX_RESOURCE_ID string_id, GX_STRING *return_string);
UINT _gxe_widget_style_add(GX_WIDGET *widget, ULONG style);
UINT _gxe_widget_style_get(GX_WIDGET *widget, ULONG *return_style);
UINT _gxe_widget_style_set(GX_WIDGET *widget, ULONG style);
UINT _gxe_widget_style_remove(GX_WIDGET *widget, ULONG style);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gxe_widget_text_blend(GX_WIDGET *widget,
                            UINT tColor, UINT font_id,
                            GX_CONST GX_CHAR *string, INT x_offset, INT y_offset, UCHAR alpha);
#endif
UINT _gxe_widget_text_blend_ext(GX_WIDGET *widget,
                                UINT tColor, UINT font_id,
                                GX_CONST GX_STRING *string, INT x_offset, INT y_offset, UCHAR alpha);
UINT _gxe_widget_top_visible_child_find(GX_WIDGET *parent, GX_WIDGET **child_return);
UINT _gxe_widget_type_find(GX_WIDGET *parent, USHORT widget_id, GX_WIDGET **return_widget);
UINT _gxe_widget_width_get(GX_WIDGET *widget, GX_VALUE *return_width);
UINT _gxe_widget_event_to_parent(GX_WIDGET *widget, GX_EVENT *event_ptr);

#endif

