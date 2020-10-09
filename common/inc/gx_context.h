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
/**   Context Management (Context)                                        */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/
/*                                                                        */
/*  COMPONENT DEFINITION                                   RELEASE        */
/*                                                                        */
/*    gx_context.h                                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file defines the GUIX draw context component, including all    */
/*    data types and external references.  It is assumed that gx_api.h    */
/*    and gx_port.h have already been included.                           */
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

#ifndef GX_DRAW_CONTEXT_H
#define GX_DRAW_CONTEXT_H

/* Define context management function prototypes.  */

UINT _gx_brush_default(GX_BRUSH *brush);
UINT _gx_brush_define(GX_BRUSH *brush, GX_COLOR line_color, GX_COLOR fill_color, UINT style);

UINT _gx_context_brush_get(GX_BRUSH **return_brush);
UINT _gx_context_brush_set(GX_BRUSH *brush);
UINT _gx_context_brush_default(GX_DRAW_CONTEXT *context);
UINT _gx_context_brush_define(GX_RESOURCE_ID line_color_id, GX_RESOURCE_ID fill_color_id, UINT style);
UINT _gx_context_brush_pattern_set(ULONG pattern);
UINT _gx_context_brush_style_set(UINT style);
UINT _gx_context_brush_width_set(UINT width);

UINT _gx_context_color_get(GX_RESOURCE_ID id, GX_COLOR *color);
UINT _gx_context_fill_color_set(GX_RESOURCE_ID fill_color_id);
UINT _gx_context_font_get(GX_RESOURCE_ID resource_id, GX_FONT **return_font);
UINT _gx_context_font_set(GX_RESOURCE_ID font_id);
UINT _gx_context_line_color_set(GX_RESOURCE_ID line_color_id);

UINT _gx_context_pixelmap_get(GX_RESOURCE_ID resource_id, GX_PIXELMAP **return_pixelmap);
UINT _gx_context_pixelmap_set(GX_RESOURCE_ID pixelmap_id);

/* functins for defining brush using r:g:b color description */
UINT _gx_context_raw_brush_define(GX_COLOR line_color, GX_COLOR fill_color, UINT style);
UINT _gx_context_raw_fill_color_set(GX_COLOR line_color);
UINT _gx_context_raw_line_color_set(GX_COLOR line_color);

#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gx_context_string_get(GX_RESOURCE_ID string_id, GX_CONST GX_CHAR **return_string);
#endif
UINT _gx_context_string_get_ext(GX_RESOURCE_ID resource_id, GX_STRING *return_string);

/* Define error checking shells for API services.  These are only referenced by the
   application.  */

UINT _gxe_brush_define(GX_BRUSH *brush, GX_COLOR line_color, GX_COLOR fill_color, UINT style);
UINT _gxe_brush_default(GX_BRUSH *brush);

UINT _gxe_context_brush_default(GX_DRAW_CONTEXT *context);
UINT _gxe_context_brush_define(GX_RESOURCE_ID line_color_id, GX_RESOURCE_ID fill_color_id, UINT style);
UINT _gxe_context_brush_get(GX_BRUSH **return_brush);
UINT _gxe_context_brush_set(GX_BRUSH *brush);
UINT _gxe_context_brush_pattern_set(ULONG pattern);
UINT _gxe_context_brush_style_set(UINT style);
UINT _gxe_context_brush_width_set(UINT width);
UINT _gxe_context_color_get(GX_RESOURCE_ID color_id, GX_COLOR *return_color);
UINT _gxe_context_fill_color_set(GX_RESOURCE_ID fill_color_id);
UINT _gxe_context_font_set(GX_RESOURCE_ID font_id);
UINT _gxe_context_font_get(GX_RESOURCE_ID resource_id, GX_FONT **return_font);
UINT _gxe_context_line_color_set(GX_RESOURCE_ID line_color_id);
UINT _gxe_context_pixelmap_get(GX_RESOURCE_ID resource_id, GX_PIXELMAP **return_pixelmap);
UINT _gxe_context_pixelmap_set(GX_RESOURCE_ID pixelmap_id);
UINT _gxe_context_raw_brush_define(GX_COLOR line_color, GX_COLOR fill_color, UINT style);
UINT _gxe_context_raw_fill_color_set(GX_COLOR line_color);
UINT _gxe_context_raw_line_color_set(GX_COLOR line_color);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gxe_context_string_get(GX_RESOURCE_ID string_id, GX_CONST GX_CHAR **return_string);
#endif
UINT _gxe_context_string_get_ext(GX_RESOURCE_ID string_id, GX_STRING *return_string);

#endif

