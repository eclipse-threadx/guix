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
/**   Multi Line Text View Management (Multi Line Text View)              */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/
/*                                                                        */
/*  COMPONENT DEFINITION                                   RELEASE        */
/*                                                                        */
/*    gx_multi_line_text_view.h                           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file defines the GUIX text view management component,          */
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

/* Define multi-line text view management function prototypes. */
UINT _gx_multi_line_text_view_create(GX_MULTI_LINE_TEXT_VIEW *text_view_ptr,
                                     GX_CONST GX_CHAR *name_ptr,
                                     GX_WIDGET *parent, GX_RESOURCE_ID text_id,
                                     ULONG style, USHORT Id,
                                     GX_CONST GX_RECTANGLE *size);
VOID _gx_multi_line_text_view_display_info_get(GX_MULTI_LINE_TEXT_VIEW *text_view, UINT start_index,
                                               UINT end_index, GX_MULTI_LINE_TEXT_INFO *text_info, GX_VALUE available_width);
VOID _gx_multi_line_text_view_draw(GX_MULTI_LINE_TEXT_VIEW *view);
VOID _gx_multi_line_text_view_text_draw(GX_MULTI_LINE_TEXT_VIEW *text_view, GX_RESOURCE_ID text_color);
UINT _gx_multi_line_text_view_event_process(GX_MULTI_LINE_TEXT_VIEW *view, GX_EVENT *event_ptr);
UINT _gx_multi_line_text_view_font_set(GX_MULTI_LINE_TEXT_VIEW *view, GX_RESOURCE_ID font_id);
UINT _gx_multi_line_text_view_line_space_set(GX_MULTI_LINE_TEXT_VIEW *view, GX_BYTE line_space);
UINT _gx_multi_line_text_view_line_cache_update(GX_MULTI_LINE_TEXT_VIEW *view);
UINT _gx_multi_line_text_view_scroll(GX_MULTI_LINE_TEXT_VIEW *view, GX_VALUE amount_to_scroll);
UINT _gx_multi_line_text_view_scroll_info_get(GX_MULTI_LINE_TEXT_VIEW *view, ULONG style, GX_SCROLL_INFO *info);
UINT _gx_multi_line_text_view_string_total_rows_compute(GX_MULTI_LINE_TEXT_VIEW *text_view);
UINT _gx_multi_line_text_view_text_color_set(GX_MULTI_LINE_TEXT_VIEW *text_view,
                                             GX_RESOURCE_ID normal_text_color_id,
                                             GX_RESOURCE_ID selected_text_color_id,
                                             GX_RESOURCE_ID disabled_text_color_id);
UINT _gx_multi_line_text_view_text_id_set(GX_MULTI_LINE_TEXT_VIEW *text_view_ptr,
                                          GX_RESOURCE_ID text_id);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gx_multi_line_text_view_text_set(GX_MULTI_LINE_TEXT_VIEW *view, GX_CONST GX_CHAR *text);
#endif
UINT _gx_multi_line_text_view_text_set_ext(GX_MULTI_LINE_TEXT_VIEW *view, GX_CONST GX_STRING *text);
UINT _gx_multi_line_text_view_visible_rows_compute(GX_MULTI_LINE_TEXT_VIEW *view);
UINT _gx_multi_line_text_view_whitespace_set(GX_MULTI_LINE_TEXT_VIEW *view, GX_UBYTE whitespace);

/* Define error checking shells for API services.  These are only referenced by the
   application.  */
UINT _gxe_multi_line_text_view_create(GX_MULTI_LINE_TEXT_VIEW *text_view_ptr, GX_CONST GX_CHAR *name_ptr, GX_WIDGET *parent,
                                      GX_RESOURCE_ID text_id, ULONG style, USHORT Id, GX_CONST GX_RECTANGLE *size,
                                      UINT text_view_control_block_size);
UINT _gxe_multi_line_text_view_event_process(GX_MULTI_LINE_TEXT_VIEW *view, GX_EVENT *event_ptr);
UINT _gxe_multi_line_text_view_font_set(GX_MULTI_LINE_TEXT_VIEW *view, GX_RESOURCE_ID font_id);
UINT _gxe_multi_line_text_view_line_space_set(GX_MULTI_LINE_TEXT_VIEW *view, GX_BYTE line_space);
UINT _gxe_multi_line_text_view_scroll_info_get(GX_MULTI_LINE_TEXT_VIEW *view, ULONG style, GX_SCROLL_INFO *info);
UINT _gxe_multi_line_text_view_text_color_set(GX_MULTI_LINE_TEXT_VIEW *view,
                                              GX_RESOURCE_ID normal_text_color_id,
                                              GX_RESOURCE_ID selected_text_color_id,
                                              GX_RESOURCE_ID disabled_text_color_id);
UINT _gxe_multi_line_text_view_text_id_set(GX_MULTI_LINE_TEXT_VIEW *text_view_ptr,
                                           GX_RESOURCE_ID text_id);

#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gxe_multi_line_text_view_text_set(GX_MULTI_LINE_TEXT_VIEW *view, GX_CONST GX_CHAR *text);
#endif
UINT _gxe_multi_line_text_view_text_set_ext(GX_MULTI_LINE_TEXT_VIEW *view, GX_CONST GX_STRING *text);

UINT _gxe_multi_line_text_view_whitespace_set(GX_MULTI_LINE_TEXT_VIEW *view, GX_UBYTE whitespace);

UINT _gxe_rich_text_view_create(GX_RICH_TEXT_VIEW *rich_view,
                                GX_CONST GX_CHAR *name,
                                GX_WIDGET *parent,
                                GX_RESOURCE_ID text_id,
                                GX_RICH_TEXT_FONTS *fonts,
                                ULONG style,
                                USHORT id,
                                GX_CONST GX_RECTANGLE *size,
                                UINT control_block_size);
UINT _gxe_rich_text_view_fonts_set(GX_RICH_TEXT_VIEW *rich_view, GX_RICH_TEXT_FONTS *fonts);

