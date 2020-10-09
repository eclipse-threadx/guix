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
/**   Text Input Management (Single Line Text Input)                      */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/
/*                                                                        */
/*  COMPONENT DEFINITION                                   RELEASE        */
/*                                                                        */
/*    gx_single_line_text_input.h                         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file defines the GUIX text input management component,         */
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

#ifndef GX_SINGLE_LINE_TEXT_INPUT_H
#define GX_SINGLE_LINE_TEXT_INPUT_H

#define ID_TEXT_INPUT_TIMER 2
#define TIME_OUT_PERIOD     20

#define GX_MARK_TIMER       3
#define GX_MARK_INTERVAL    5

/* Define text input management function prototypes.  */
UINT _gx_single_line_text_input_backspace(GX_SINGLE_LINE_TEXT_INPUT *text_input);
UINT _gx_single_line_text_input_buffer_clear(GX_SINGLE_LINE_TEXT_INPUT *text_input);
UINT _gx_single_line_text_input_buffer_get(GX_SINGLE_LINE_TEXT_INPUT *text_input_ptr, GX_CHAR **buffer_address,
                                           UINT *content_size, UINT *buffer_size);
UINT _gx_single_line_text_input_character_delete(GX_SINGLE_LINE_TEXT_INPUT *text_input);
UINT _gx_single_line_text_input_character_insert(GX_SINGLE_LINE_TEXT_INPUT *text_input, GX_UBYTE *str, UINT str_size);
UINT _gx_single_line_text_input_copy(GX_SINGLE_LINE_TEXT_INPUT *input);
UINT _gx_single_line_text_input_create(GX_SINGLE_LINE_TEXT_INPUT *text_input_ptr, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                                       GX_CHAR *input_buffer, UINT buffer_size, UINT style, USHORT Id,
                                       GX_CONST GX_RECTANGLE *size);
UINT _gx_single_line_text_input_cut(GX_SINGLE_LINE_TEXT_INPUT *input);
VOID _gx_single_line_text_input_draw(GX_SINGLE_LINE_TEXT_INPUT *text_input);
UINT _gx_single_line_text_input_draw_position_get(GX_SINGLE_LINE_TEXT_INPUT *text_input, GX_VALUE *xpos, GX_VALUE *ypos);
UINT _gx_single_line_text_input_end(GX_SINGLE_LINE_TEXT_INPUT *text_input);
UINT _gx_single_line_text_input_event_process(GX_SINGLE_LINE_TEXT_INPUT *text_input, GX_EVENT *event_ptr);
UINT _gx_single_line_text_input_fill_color_set(GX_SINGLE_LINE_TEXT_INPUT *input,
                                               GX_RESOURCE_ID normal_fill_color_id,
                                               GX_RESOURCE_ID selected_fill_color_id,
                                               GX_RESOURCE_ID disabled_fill_color_id,
                                               GX_RESOURCE_ID readonly_fill_color_id);
UINT _gx_single_line_text_input_home(GX_SINGLE_LINE_TEXT_INPUT *text_input);
VOID _gx_single_line_text_input_keydown_process(GX_SINGLE_LINE_TEXT_INPUT *text_input, GX_EVENT *event_ptr);
UINT _gx_single_line_text_input_left_arrow(GX_SINGLE_LINE_TEXT_INPUT *text_input);
UINT _gx_single_line_text_input_mark_end(GX_SINGLE_LINE_TEXT_INPUT *text_input);
UINT _gx_single_line_text_input_mark_home(GX_SINGLE_LINE_TEXT_INPUT *text_input);
UINT _gx_single_line_text_input_mark_next(GX_SINGLE_LINE_TEXT_INPUT *text_input);
UINT _gx_single_line_text_input_mark_previous(GX_SINGLE_LINE_TEXT_INPUT *text_input);
UINT _gx_single_line_text_input_paste(GX_SINGLE_LINE_TEXT_INPUT *input);
UINT _gx_single_line_text_input_position_get(GX_SINGLE_LINE_TEXT_INPUT *text_input, INT pixel_position);
UINT _gx_single_line_text_input_position_update(GX_SINGLE_LINE_TEXT_INPUT *text_input);
UINT _gx_single_line_text_input_right_arrow(GX_SINGLE_LINE_TEXT_INPUT *text_input);
UINT _gx_single_line_text_input_style_add(GX_SINGLE_LINE_TEXT_INPUT *text_input_ptr, ULONG style);
UINT _gx_single_line_text_input_style_remove(GX_SINGLE_LINE_TEXT_INPUT *text_input_ptr, ULONG style);
UINT _gx_single_line_text_input_style_set(GX_SINGLE_LINE_TEXT_INPUT *text_input_ptr, ULONG style);
UINT _gx_single_line_text_input_text_color_set(GX_SINGLE_LINE_TEXT_INPUT *input,
                                               GX_RESOURCE_ID normal_text_color_id,
                                               GX_RESOURCE_ID selected_text_color_id,
                                               GX_RESOURCE_ID disabled_text_color_id,
                                               GX_RESOURCE_ID readonly_text_color_id);
UINT _gx_single_line_text_input_text_rectangle_get(GX_SINGLE_LINE_TEXT_INPUT *input, INT shift, GX_RECTANGLE *rect);
UINT _gx_single_line_text_input_text_select(GX_SINGLE_LINE_TEXT_INPUT *input, UINT start_index, UINT end_index);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gx_single_line_text_input_text_set(GX_SINGLE_LINE_TEXT_INPUT *text_input, GX_CONST GX_CHAR *text);
#endif
UINT _gx_single_line_text_input_text_set_ext(GX_SINGLE_LINE_TEXT_INPUT *text_input, GX_CONST GX_STRING *text);


/* error checking versions of the animation API functions */
UINT _gxe_single_line_text_input_backspace(GX_SINGLE_LINE_TEXT_INPUT *text_input);
UINT _gxe_single_line_text_input_buffer_clear(GX_SINGLE_LINE_TEXT_INPUT *text_input_ptr);
UINT _gxe_single_line_text_input_buffer_get(GX_SINGLE_LINE_TEXT_INPUT *text_input_ptr, GX_CHAR **buffer_address,
                                            UINT *content_size, UINT *buffer_size);
UINT _gxe_single_line_text_input_character_delete(GX_SINGLE_LINE_TEXT_INPUT *text_input);
UINT _gxe_single_line_text_input_character_insert(GX_SINGLE_LINE_TEXT_INPUT *text_input, GX_UBYTE *str, UINT str_size);
UINT _gxe_single_line_text_input_create(GX_SINGLE_LINE_TEXT_INPUT *text_input_ptr, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                                        GX_CHAR *input_buffer, UINT buffer_size, UINT style, USHORT Id,
                                        GX_CONST GX_RECTANGLE *size, UINT text_input_control_block_size);
UINT _gxe_single_line_text_input_draw_position_get(GX_SINGLE_LINE_TEXT_INPUT *text_input, GX_VALUE *xpos, GX_VALUE *ypos);
UINT _gxe_single_line_text_input_end(GX_SINGLE_LINE_TEXT_INPUT *text_input);
UINT _gxe_single_line_text_input_event_process(GX_SINGLE_LINE_TEXT_INPUT *text_input_ptr, GX_EVENT *event_ptr);
UINT _gxe_single_line_text_input_home(GX_SINGLE_LINE_TEXT_INPUT *text_input);
UINT _gxe_single_line_text_input_left_arrow(GX_SINGLE_LINE_TEXT_INPUT *text_input);
UINT _gxe_single_line_text_input_position_get(GX_SINGLE_LINE_TEXT_INPUT *text_input, INT pixel_position);
UINT _gxe_single_line_text_input_right_arrow(GX_SINGLE_LINE_TEXT_INPUT *text_input);
UINT _gxe_single_line_text_input_style_add(GX_SINGLE_LINE_TEXT_INPUT *text_input_ptr, ULONG style);
UINT _gxe_single_line_text_input_style_remove(GX_SINGLE_LINE_TEXT_INPUT *text_input_ptr, ULONG style);
UINT _gxe_single_line_text_input_style_set(GX_SINGLE_LINE_TEXT_INPUT *text_input_ptr, ULONG style);
UINT _gxe_single_line_text_input_fill_color_set(GX_SINGLE_LINE_TEXT_INPUT *input,
                                                GX_RESOURCE_ID normal_fill_color_id,
                                                GX_RESOURCE_ID selected_fill_color_id,
                                                GX_RESOURCE_ID disabled_fill_color_id,
                                                GX_RESOURCE_ID readonly_fill_color_id);
UINT _gxe_single_line_text_input_text_color_set(GX_SINGLE_LINE_TEXT_INPUT *input,
                                                GX_RESOURCE_ID normal_text_color_id,
                                                GX_RESOURCE_ID selected_text_color_id,
                                                GX_RESOURCE_ID disabled_text_color_id,
                                                GX_RESOURCE_ID readonly_text_color_id);
UINT _gxe_single_line_text_input_text_select(GX_SINGLE_LINE_TEXT_INPUT *input, UINT start_index, UINT end_index);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gxe_single_line_text_input_text_set(GX_SINGLE_LINE_TEXT_INPUT *input, GX_CONST GX_CHAR *text);
#endif
UINT _gxe_single_line_text_input_text_set_ext(GX_SINGLE_LINE_TEXT_INPUT *input, GX_CONST GX_STRING *text);

#endif

