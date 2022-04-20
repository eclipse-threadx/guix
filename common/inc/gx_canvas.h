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
/**   Canvas Management (Canvas)                                          */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/
/*                                                                        */
/*  COMPONENT DEFINITION                                   RELEASE        */
/*                                                                        */
/*    gx_canvas.h                                         PORTABLE C      */
/*                                                           6.1.11       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file defines the GUIX canvas component, including all data     */
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
/*  04-25-2022     Ting Zhu                 Modified comment(s),          */
/*                                            added new declarations,     */
/*                                            resulting in version 6.1.11 */
/*                                                                        */
/**************************************************************************/

#ifndef GX_CANVAS_H
#define GX_CANVAS_H


/* Define canvas management constants.  */

#define GX_CANVAS_ID ((ULONG)0x43414E56)


/* Define canvas management function prototypes.  */
UINT    _gx_canvas_alpha_set(GX_CANVAS *canvas, GX_UBYTE alpha);
UINT    _gx_canvas_arc_draw(INT xcenter, INT ycenter, UINT r, INT start_angle, INT end_angle);
VOID    _gx_canvas_anti_alias_line_draw(GX_DRAW_CONTEXT *context, INT xs, INT ys, INT xe, INT ye, INT flag);
UINT    _gx_canvas_block_move(GX_RECTANGLE *block, GX_VALUE x_shift, GX_VALUE y_shift, GX_RECTANGLE *dirty);

UINT    _gx_canvas_circle_draw(INT xcenter, INT ycenter, UINT r);
GX_BOOL _gx_canvas_composite_create(GX_CANVAS **return_composite);
UINT    _gx_canvas_create(GX_CANVAS *canvas, GX_CONST GX_CHAR *name, GX_DISPLAY *dispaly,
                          UINT type, UINT width, UINT height, GX_COLOR *memory_area, ULONG memory_size);
UINT    _gx_canvas_delete(GX_CANVAS *canvas);

VOID    _gx_canvas_dirty_mark(GX_CANVAS *canvas, GX_RECTANGLE *area);

UINT    _gx_canvas_drawing_complete(GX_CANVAS *canvas, GX_BOOL Flush);
UINT    _gx_canvas_drawing_initiate(GX_CANVAS *canvas, GX_WIDGET *who, GX_RECTANGLE *clip_area);

UINT    _gx_canvas_ellipse_draw(INT xcenter, INT ycenter, INT a, INT b);
VOID    _gx_canvas_glyphs_draw(GX_DRAW_CONTEXT *context, GX_POINT *draw_position, GX_CONST GX_STRING *string,
                               GX_RECTANGLE *view,
                               VOID (*draw_glyph)(GX_DRAW_CONTEXT *, GX_RECTANGLE *, GX_POINT *, GX_CONST GX_GLYPH *));

UINT    _gx_canvas_hardware_layer_bind(GX_CANVAS *canvas, INT layer);
UINT    _gx_canvas_hide(GX_CANVAS *canvas);

UINT _gx_canvas_line_draw(GX_VALUE x_start, GX_VALUE y_start, GX_VALUE x_end, GX_VALUE y_end);
UINT _gx_canvas_memory_define(GX_CANVAS *canvas, GX_COLOR *memory, ULONG memsize);

#if defined(GX_MOUSE_SUPPORT)
UINT _gx_canvas_mouse_define(GX_CANVAS *canvas, GX_MOUSE_CURSOR_INFO *info);
UINT _gx_canvas_mouse_show(GX_CANVAS *canvas);
UINT _gx_canvas_mouse_hide(GX_CANVAS *canvas);
#endif
UINT _gx_canvas_offset_set(GX_CANVAS *canvas, GX_VALUE x, GX_VALUE y);
UINT _gx_canvas_pixel_draw(GX_POINT position);
UINT _gx_canvas_pixelmap_blend(GX_VALUE x_position, GX_VALUE y_position, GX_PIXELMAP *pixelmap, GX_UBYTE alpha);
UINT _gx_canvas_pixelmap_draw(GX_VALUE x_position, GX_VALUE y_position, GX_PIXELMAP *pixelmap);
UINT _gx_canvas_pixelmap_get(GX_PIXELMAP *pixelmap);
UINT _gx_canvas_pixelmap_tile(GX_RECTANGLE *fill, GX_PIXELMAP *pixelmap);
UINT _gx_canvas_pixelmap_rotate(GX_VALUE x_position, GX_VALUE y_position, GX_PIXELMAP *pixelmap,
                                INT angle, INT rot_cx, INT rot_cy);
UINT _gx_canvas_polygon_draw(GX_POINT *point_array, INT number_of_points);
UINT _gx_canvas_pie_draw(INT xcenter, INT ycenter, UINT r, INT start_angle, INT end_angle);
UINT _gx_canvas_rectangle_draw(GX_RECTANGLE *rectangle);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gx_canvas_rotated_text_draw(GX_CONST GX_CHAR *text, GX_VALUE xcenter, GX_VALUE ycenter, INT angle);
#endif
UINT _gx_canvas_rotated_text_draw_ext(GX_CONST GX_STRING *text, GX_VALUE xcenter, GX_VALUE ycenter, INT angle);
UINT _gx_canvas_shift(GX_CANVAS *canvas, GX_VALUE x, GX_VALUE y);
UINT _gx_canvas_show(GX_CANVAS *canvas);

UINT _gx_canvas_aligned_text_draw(GX_CONST GX_STRING *string, GX_RECTANGLE *rectangle, ULONG alignment);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gx_canvas_text_draw(GX_VALUE x_start, GX_VALUE y_start, GX_CONST GX_CHAR *string, INT length);
#endif
UINT _gx_canvas_text_draw_ext(GX_VALUE x_start, GX_VALUE y_start, GX_CONST GX_STRING *string);

/* Define error checking shells for API services.  These are only referenced by the
   application.  */

UINT _gxe_canvas_alpha_set(GX_CANVAS *canvas, GX_UBYTE alpha);
UINT _gxe_canvas_arc_draw(INT xcenter, INT ycenter, UINT r, INT start_angle, INT end_angle);
UINT _gxe_canvas_block_move(GX_RECTANGLE *block, GX_VALUE x_shift, GX_VALUE y_shift, GX_RECTANGLE *dirty);
UINT _gxe_canvas_circle_draw(INT xcenter, INT ycenter, UINT r);
UINT _gxe_canvas_create(GX_CANVAS *canvas, GX_CONST GX_CHAR *name, GX_DISPLAY *display, UINT type, UINT width,
                        UINT height, GX_COLOR *memory_area, ULONG memory_size,
                        UINT canvas_control_block_size);
UINT _gxe_canvas_delete(GX_CANVAS *canvas);
UINT _gxe_canvas_drawing_complete(GX_CANVAS *canvas, GX_BOOL Flush);
UINT _gxe_canvas_drawing_initiate(GX_CANVAS *canvas, GX_WIDGET *who, GX_RECTANGLE *dirty_area);
UINT _gxe_canvas_ellipse_draw(INT xcenter, INT ycenter, INT a, INT b);

UINT _gxe_canvas_hardware_layer_bind(GX_CANVAS *canvas, INT layer);
UINT _gxe_canvas_hide(GX_CANVAS *canvas);
UINT _gxe_canvas_line_draw(GX_VALUE x_start, GX_VALUE y_start, GX_VALUE x_end, GX_VALUE y_end);
UINT _gxe_canvas_memory_define(GX_CANVAS *canvas, GX_COLOR *memory, ULONG memsize);

#if defined(GX_MOUSE_SUPPORT)
UINT _gxe_canvas_mouse_define(GX_CANVAS *canvas, GX_MOUSE_CURSOR_INFO *info);
UINT _gxe_canvas_mouse_show(GX_CANVAS *canvas);
UINT _gxe_canvas_mouse_hide(GX_CANVAS *canvas);
#endif
UINT _gxe_canvas_offset_set(GX_CANVAS *canvas, GX_VALUE x, GX_VALUE y);
UINT _gxe_canvas_pie_draw(INT xcenter, INT ycenter, UINT r, INT start_angle, INT end_angle);
UINT _gxe_canvas_pixel_draw(GX_POINT position);
UINT _gxe_canvas_pixelmap_blend(GX_VALUE x_position, GX_VALUE y_position, GX_PIXELMAP *pixelmap, GX_UBYTE alpha);
UINT _gxe_canvas_pixelmap_draw(GX_VALUE x_position, GX_VALUE y_position, GX_PIXELMAP *pixelmap);
UINT _gxe_canvas_pixelmap_get(GX_PIXELMAP *pixelmap);
UINT _gxe_canvas_pixelmap_tile(GX_RECTANGLE *fill, GX_PIXELMAP *pixelmap);
UINT _gxe_canvas_pixelmap_rotate(GX_VALUE x_position, GX_VALUE y_position, GX_PIXELMAP *pixelmap, INT angle, INT rot_cx, INT rot_cy);
UINT _gxe_canvas_polygon_draw(GX_POINT *point_array, INT number_of_points);
UINT _gxe_canvas_rectangle_draw(GX_RECTANGLE *rectangle);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gxe_canvas_rotated_text_draw(GX_CONST GX_CHAR *text, GX_VALUE xcenter,  GX_VALUE ycenter, INT angle);
#endif
UINT _gxe_canvas_rotated_text_draw_ext(GX_CONST GX_STRING *text, GX_VALUE xcenter, GX_VALUE ycenter, INT angle);
UINT _gxe_canvas_shift(GX_CANVAS *canvas, GX_VALUE x, GX_VALUE y);
UINT _gxe_canvas_show(GX_CANVAS *canvas);
UINT _gxe_canvas_aligned_text_draw(GX_CONST GX_STRING *string, GX_RECTANGLE *rectangle, ULONG alignment);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gxe_canvas_text_draw(GX_VALUE x_start, GX_VALUE y_start, GX_CONST GX_CHAR *string, INT length);
#endif
UINT _gxe_canvas_text_draw_ext(GX_VALUE x_start, GX_VALUE y_start, GX_CONST GX_STRING *string);

#ifdef GX_TARGET_WIN32
VOID _win32_compatible_canvas_memory_allocate(GX_CANVAS *canvas);
#endif

#endif

