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
/**   Multi Line Text Input Management (Multi Line Text Input)            */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/
/*                                                                        */
/*  COMPONENT DEFINITION                                   RELEASE        */
/*                                                                        */
/*    gx_multi_line_text_input.h                          PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file defines the GUIX multi line text input management         */
/*    component, including all data types and external references.  It is */
/*    assumed that gx_api.h and gx_port.h have already been included.     */
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
#define ID_TEXT_INPUT_TIMER 2
#define GX_MARK_TIMER       3

#define NUM_BEF_CURSOR      1
#define NUM_AFT_CURSOR      3

#define GX_MARK_INTERVAL    5

/* Define multi-line text input management function prototypes. */
UINT _gx_multi_line_text_input_backspace(GX_MULTI_LINE_TEXT_INPUT *text_input);
UINT _gx_multi_line_text_input_buffer_clear(GX_MULTI_LINE_TEXT_INPUT *text_input);
UINT _gx_multi_line_text_input_buffer_get(GX_MULTI_LINE_TEXT_INPUT *text_input_ptr, GX_CHAR **buffer_address,
                                          UINT *content_size, UINT *buffer_size);

#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gx_multi_line_text_input_char_insert(GX_MULTI_LINE_TEXT_INPUT *text_input, GX_UBYTE *str, UINT str_size);
#endif
UINT _gx_multi_line_text_input_char_insert_ext(GX_MULTI_LINE_TEXT_INPUT *text_input, GX_CONST GX_STRING *str);
UINT _gx_multi_line_text_input_char_remove(GX_MULTI_LINE_TEXT_INPUT *text_input, UINT index, UINT del_bytes);
UINT _gx_multi_line_text_input_copy(GX_MULTI_LINE_TEXT_INPUT *input);
UINT _gx_multi_line_text_input_create(GX_MULTI_LINE_TEXT_INPUT *text_input_ptr,
                                      GX_CONST GX_CHAR *name_ptr, GX_WIDGET *parent,
                                      GX_CHAR *input_buffer, UINT buffer_size, ULONG style, USHORT Id,
                                      GX_CONST GX_RECTANGLE *size);
UINT _gx_multi_line_text_input_cursor_pos_calculate(GX_MULTI_LINE_TEXT_INPUT *text_input, GX_POINT click_pos);
UINT _gx_multi_line_text_input_cursor_pos_get(GX_MULTI_LINE_TEXT_INPUT *input, GX_POINT *cursor_pos);
UINT _gx_multi_line_text_input_cursor_pos_update(GX_MULTI_LINE_TEXT_INPUT *text_input, GX_BOOL make_cursor_visible);
UINT _gx_multi_line_text_input_cursor_visible(GX_MULTI_LINE_TEXT_INPUT *text_input);
UINT _gx_multi_line_text_input_cut(GX_MULTI_LINE_TEXT_INPUT *input);
UINT _gx_multi_line_text_input_delete(GX_MULTI_LINE_TEXT_INPUT *text_input);
UINT _gx_multi_line_text_input_down_arrow(GX_MULTI_LINE_TEXT_INPUT *text_input);
VOID _gx_multi_line_text_input_draw(GX_MULTI_LINE_TEXT_INPUT *text_input);
UINT _gx_multi_line_text_input_end(GX_MULTI_LINE_TEXT_INPUT *text_input);
UINT _gx_multi_line_text_input_event_process(GX_MULTI_LINE_TEXT_INPUT *text_input, GX_EVENT *event_ptr);
UINT _gx_multi_line_text_input_fill_color_set(GX_MULTI_LINE_TEXT_INPUT *input,
                                              GX_RESOURCE_ID normal_fill_color_id,
                                              GX_RESOURCE_ID selected_fill_color_id,
                                              GX_RESOURCE_ID disabled_fill_color_id,
                                              GX_RESOURCE_ID readonly_fill_color_id);
UINT _gx_multi_line_text_input_home(GX_MULTI_LINE_TEXT_INPUT *text_input);
UINT _gx_multi_line_text_input_highlight_rectangle_get(GX_MULTI_LINE_TEXT_INPUT *input, GX_RECTANGLE *rect);
UINT _gx_multi_line_text_input_keydown_process(GX_MULTI_LINE_TEXT_INPUT *text_input, GX_EVENT *event_ptr);
UINT _gx_multi_line_text_input_left_arrow(GX_MULTI_LINE_TEXT_INPUT *text_input);
UINT _gx_multi_line_text_input_mark_end(GX_MULTI_LINE_TEXT_INPUT *text_input);
UINT _gx_multi_line_text_input_mark_home(GX_MULTI_LINE_TEXT_INPUT *text_input);
UINT _gx_multi_line_text_input_mark_next(GX_MULTI_LINE_TEXT_INPUT *text_input);
UINT _gx_multi_line_text_input_mark_previous(GX_MULTI_LINE_TEXT_INPUT *text_input);
UINT _gx_multi_line_text_input_mark_up(GX_MULTI_LINE_TEXT_INPUT *text_input);
UINT _gx_multi_line_text_input_mark_down(GX_MULTI_LINE_TEXT_INPUT *text_input);
UINT _gx_multi_line_text_input_paste(GX_MULTI_LINE_TEXT_INPUT *input);
UINT _gx_multi_line_text_input_right_arrow(GX_MULTI_LINE_TEXT_INPUT *text_input);
UINT _gx_multi_line_text_input_style_add(GX_MULTI_LINE_TEXT_INPUT *text_input_ptr, ULONG cursor_type);
UINT _gx_multi_line_text_input_style_remove(GX_MULTI_LINE_TEXT_INPUT *text_input_ptr, ULONG cursor_type);
UINT _gx_multi_line_text_input_style_set(GX_MULTI_LINE_TEXT_INPUT *text_input_ptr, ULONG cursor_type);
UINT _gx_multi_line_text_input_text_color_set(GX_MULTI_LINE_TEXT_INPUT *input,
                                              GX_RESOURCE_ID normal_text_color_id,
                                              GX_RESOURCE_ID selected_text_color_id,
                                              GX_RESOURCE_ID disabled_text_color_id,
                                              GX_RESOURCE_ID readonly_text_color_id);
UINT _gx_multi_line_text_input_text_rectangle_get(GX_MULTI_LINE_TEXT_INPUT *input, GX_POINT start_cursor_pos, GX_POINT end_cursor_pos, GX_RECTANGLE *rect);
UINT _gx_multi_line_text_input_text_select(GX_MULTI_LINE_TEXT_INPUT *input, UINT start_index, UINT end_index);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gx_multi_line_text_input_text_set(GX_MULTI_LINE_TEXT_INPUT *text_input, GX_CONST GX_CHAR *text);
#endif
UINT _gx_multi_line_text_input_text_set_ext(GX_MULTI_LINE_TEXT_INPUT *text_input, GX_CONST GX_STRING *text);
UINT _gx_multi_line_text_input_up_arrow(GX_MULTI_LINE_TEXT_INPUT *text_input);

/* Define error checking shells for API services.  These are only referenced by the
   application.  */
UINT _gxe_multi_line_text_input_backspace(GX_MULTI_LINE_TEXT_INPUT *text_input);
UINT _gxe_multi_line_text_input_buffer_clear(GX_MULTI_LINE_TEXT_INPUT *text_input);
UINT _gxe_multi_line_text_input_buffer_get(GX_MULTI_LINE_TEXT_INPUT *text_input_ptr, GX_CHAR **buffer_address,
                                           UINT *content_size, UINT *buffer_size);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gxe_multi_line_text_input_char_insert(GX_MULTI_LINE_TEXT_INPUT *text_input, GX_UBYTE *str, UINT str_size);
#endif
UINT _gxe_multi_line_text_input_char_insert_ext(GX_MULTI_LINE_TEXT_INPUT *text_input, GX_CONST GX_STRING *str);
UINT _gxe_multi_line_text_input_create(GX_MULTI_LINE_TEXT_INPUT *text_input_ptr, GX_CONST GX_CHAR *name_ptr, GX_WIDGET *parent,
                                       GX_CHAR *input_buffer, UINT buffer_size, ULONG style, USHORT Id, GX_CONST GX_RECTANGLE *size,
                                       UINT text_input_control_block_size);
UINT _gxe_multi_line_text_input_cursor_pos_get(GX_MULTI_LINE_TEXT_INPUT *input, GX_POINT *cursor_pos);
UINT _gxe_multi_line_text_input_delete(GX_MULTI_LINE_TEXT_INPUT *text_input);
UINT _gxe_multi_line_text_input_down_arrow(GX_MULTI_LINE_TEXT_INPUT *text_input);
UINT _gxe_multi_line_text_input_end(GX_MULTI_LINE_TEXT_INPUT *text_input);
UINT _gxe_multi_line_text_input_event_process(GX_MULTI_LINE_TEXT_INPUT *input, GX_EVENT *event_ptr);
UINT _gxe_multi_line_text_input_fill_color_set(GX_MULTI_LINE_TEXT_INPUT *input,
                                               GX_RESOURCE_ID normal_fill_color_id,
                                               GX_RESOURCE_ID selected_fill_color_id,
                                               GX_RESOURCE_ID disabled_fill_color_id,
                                               GX_RESOURCE_ID readonly_fill_color_id);
UINT _gxe_multi_line_text_input_home(GX_MULTI_LINE_TEXT_INPUT *text_input);
UINT _gxe_multi_line_text_input_left_arrow(GX_MULTI_LINE_TEXT_INPUT *text_input);
UINT _gxe_multi_line_text_input_right_arrow(GX_MULTI_LINE_TEXT_INPUT *text_input);
UINT _gxe_multi_line_text_input_style_add(GX_MULTI_LINE_TEXT_INPUT *text_input_ptr, ULONG cursor_type);
UINT _gxe_multi_line_text_input_style_remove(GX_MULTI_LINE_TEXT_INPUT *text_input_ptr, ULONG cursor_type);
UINT _gxe_multi_line_text_input_style_set(GX_MULTI_LINE_TEXT_INPUT *text_input_ptr, ULONG cursor_type);
UINT _gxe_multi_line_text_input_text_color_set(GX_MULTI_LINE_TEXT_INPUT *input,
                                               GX_RESOURCE_ID normal_text_color_id,
                                               GX_RESOURCE_ID selected_text_color_id,
                                               GX_RESOURCE_ID disabled_text_color_id,
                                               GX_RESOURCE_ID readonly_text_color_id);
UINT _gxe_multi_line_text_input_text_select(GX_MULTI_LINE_TEXT_INPUT *input, UINT start_index, UINT end_index);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gxe_multi_line_text_input_text_set(GX_MULTI_LINE_TEXT_INPUT *input, GX_CONST GX_CHAR *text);
#endif
UINT _gxe_multi_line_text_input_text_set_ext(GX_MULTI_LINE_TEXT_INPUT *input, GX_CONST GX_STRING *text);
UINT _gxe_multi_line_text_input_up_arrow(GX_MULTI_LINE_TEXT_INPUT *text_input);

