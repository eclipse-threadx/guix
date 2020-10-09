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
/**   Rich Text View Management (Rich Text View)                          */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/
/*                                                                        */
/*  COMPONENT DEFINITION                                   RELEASE        */
/*                                                                        */
/*    gx_rich_text_view.h                                 PORTABLE C      */
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
/*  09-30-2020     Kenneth Maxwell          Initial Version 6.1           */
/*                                                                        */
/**************************************************************************/
#ifndef GX_RICH_TEXT_VIEW_H
#define GX_RICH_TEXT_VIEW_H

typedef struct GX_RICH_TEXT_LINE_INFO_STRUCT
{
    GX_STRING           gx_rich_text_line_info_text;
    GX_RICH_TEXT_FORMAT gx_rich_text_line_info_start_format;
    GX_RICH_TEXT_FORMAT gx_rich_text_line_info_end_format;
    UINT                gx_rich_text_line_info_text_width;
    GX_UBYTE            gx_rich_text_line_info_line_height;
    GX_UBYTE            gx_rich_text_line_info_baseline;
} GX_RICH_TEXT_LINE_INFO;


/* Define rich text view management function prototypes. */
UINT _gx_rich_text_view_context_peek(GX_RICH_TEXT_CONTEXT *context);
UINT _gx_rich_text_view_context_pop();
UINT _gx_rich_text_view_context_push(GX_RICH_TEXT_CONTEXT *context);
UINT _gx_rich_text_view_context_save();
UINT _gx_rich_text_view_context_reset();
UINT _gx_rich_text_view_context_restore();
UINT _gx_rich_text_view_create(GX_RICH_TEXT_VIEW *text_view,
                               GX_CONST GX_CHAR *name,
                               GX_WIDGET *parent,
                               GX_RESOURCE_ID text_id,
                               GX_RICH_TEXT_FONTS *fonts,
                               ULONG style,
                               USHORT id,
                               GX_CONST GX_RECTANGLE *size);
UINT _gx_rich_text_view_tag_enter(GX_RICH_TEXT_VIEW *view, GX_CONST GX_STRING *text, GX_RICH_TEXT_FORMAT *format, GX_UBYTE *handled_bytes);
VOID _gx_rich_text_view_draw(GX_RICH_TEXT_VIEW *text_view);
UINT _gx_rich_text_view_fonts_set(GX_RICH_TEXT_VIEW *view, GX_RICH_TEXT_FONTS *fonts);
UINT _gx_rich_text_view_line_info_get(GX_RICH_TEXT_VIEW *text_view, GX_STRING text, GX_RICH_TEXT_LINE_INFO *line_info, GX_VALUE availlable_width);
UINT _gx_rich_text_view_scroll_info_get(GX_RICH_TEXT_VIEW *text_view, ULONG style, GX_SCROLL_INFO *info);
VOID _gx_rich_text_view_text_draw(GX_RICH_TEXT_VIEW *text_view);
UINT _gx_rich_text_view_text_total_height_calculate(GX_RICH_TEXT_VIEW *text_view);

/* Define error checking shells for API services. */
UINT _gxe_rich_text_view_create(GX_RICH_TEXT_VIEW *text_view,
                                GX_CONST GX_CHAR *name,
                                GX_WIDGET *parent,
                                GX_RESOURCE_ID text_id,
                                GX_RICH_TEXT_FONTS *fonts,
                                ULONG style,
                                USHORT id,
                                GX_CONST GX_RECTANGLE *size,
                                UINT control_block_size);
UINT _gxe_rich_text_view_fonts_set(GX_RICH_TEXT_VIEW *view, GX_RICH_TEXT_FONTS *fonts);

#endif
