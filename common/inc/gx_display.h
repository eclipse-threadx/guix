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
/**   Display Management (Display)                                        */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/
/*                                                                        */
/*  COMPONENT DEFINITION                                   RELEASE        */
/*                                                                        */
/*    gx_display.h                                        PORTABLE C      */
/*                                                           6.1.10       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file defines the GUIX Display component, including all data    */
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
/*  12-31-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            added rotated display       */
/*                                            driver declarations,        */
/*                                            resulting in version 6.1.3  */
/*  02-02-2021     Kenneth Maxwell          Modified comment(s),          */
/*                                            added 8bpp/32bpp rotated    */
/*                                            display driver declarations,*/
/*                                            resulting in version 6.1.4  */
/*  03-02-2021     Ting Zhu                 Modified comment(s),          */
/*                                            added macro                 */
/*                                            GX_SET_32BPP_BLEND_FUNCTION,*/
/*                                            resulting in version 6.1.5  */
/*  01-31-2022     Ting Zhu                 Modified comment(s),          */
/*                                            added language direction    */
/*                                            table set declarations,     */
/*                                            resulting in version 6.1.10 */
/*                                                                        */
/**************************************************************************/

#ifndef GX_DISPLAY_H
#define GX_DISPLAY_H


/* Define Display management constants.  */

#define GX_DISPLAY_ID ((ULONG)0x53435245)
#define GX_MAX_DISPLAY_RESOLUTION 8192

/* Define display management function prototypes.  */

UINT _gx_display_active_language_set(GX_DISPLAY *display, GX_UBYTE language);
UINT _gx_display_create(GX_DISPLAY *display, GX_CONST GX_CHAR *name, UINT (*display_driver_setup)(GX_DISPLAY *), GX_VALUE width, GX_VALUE height);
UINT _gx_display_delete(GX_DISPLAY *display, VOID (*display_driver_cleanup)(GX_DISPLAY *));
VOID _gx_display_canvas_dirty(GX_DISPLAY *display);
UINT _gx_display_color_set(GX_DISPLAY *display, GX_RESOURCE_ID id, GX_COLOR color);
UINT _gx_display_color_table_set(GX_DISPLAY *display, GX_COLOR *color_table, INT number_of_colors);
UINT _gx_display_font_table_set(GX_DISPLAY *display, GX_FONT **font_table, UINT number_of_fonts);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gx_display_language_table_get(GX_DISPLAY *display, GX_CHAR ****table, GX_UBYTE *language_count, UINT *string_count);
UINT _gx_display_language_table_set(GX_DISPLAY *display, GX_CHAR ***table, GX_UBYTE num_languages, UINT number_of_strings);
#endif
UINT _gx_display_language_table_get_ext(GX_DISPLAY *display, GX_STRING ***table, GX_UBYTE *language_count, UINT *string_count);
UINT _gx_display_language_table_set_ext(GX_DISPLAY *display, GX_CONST GX_STRING **table, GX_UBYTE num_languages, UINT number_of_strings);
#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
UINT _gx_display_language_direction_table_set(GX_DISPLAY *display, GX_CONST GX_UBYTE *language_direction_table, GX_UBYTE num_languages);
#endif
UINT _gx_display_pixelmap_table_set(GX_DISPLAY *display, GX_PIXELMAP **pixelmap_table, UINT number_of_pixelmaps);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gx_display_string_get(GX_DISPLAY *display, GX_RESOURCE_ID string_id, GX_CONST GX_CHAR **return_string);
UINT _gx_display_string_table_get(GX_DISPLAY *display, GX_UBYTE language, GX_CHAR ***table, UINT *size);
#endif
UINT _gx_display_string_get_ext(GX_DISPLAY *display, GX_RESOURCE_ID string_id, GX_STRING *return_string);
UINT _gx_display_string_table_get_ext(GX_DISPLAY *display, GX_UBYTE language, GX_STRING **table, UINT *size);
UINT _gx_display_theme_install(GX_DISPLAY *display, GX_CONST GX_THEME *theme_ptr);

UINT _gxe_display_active_language_set(GX_DISPLAY *display, GX_UBYTE language);
UINT _gxe_display_create(GX_DISPLAY *display, GX_CONST GX_CHAR *name, UINT (*display_driver_setup)(GX_DISPLAY *), GX_VALUE width, GX_VALUE height, UINT display_control_block_size);
UINT _gxe_display_color_set(GX_DISPLAY *display, GX_RESOURCE_ID resource_id, GX_COLOR new_color);
UINT _gxe_display_color_table_set(GX_DISPLAY *display, GX_COLOR *color_table, INT number_of_colors);
UINT _gxe_display_delete(GX_DISPLAY *display, VOID (*display_driver_cleanup)(GX_DISPLAY *));
UINT _gxe_display_font_table_set(GX_DISPLAY *display, GX_FONT **font_table, UINT number_of_fonts);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gxe_display_language_table_get(GX_DISPLAY *display, GX_CHAR ****table, GX_UBYTE *language_count, UINT *string_count);
UINT _gxe_display_language_table_set(GX_DISPLAY *display, GX_CHAR ***table, GX_UBYTE num_languages, UINT number_of_strings);
#endif
UINT _gxe_display_language_table_get_ext(GX_DISPLAY *display, GX_STRING ***table, GX_UBYTE *language_count, UINT *string_count);
UINT _gxe_display_language_table_set_ext(GX_DISPLAY *display, GX_CONST GX_STRING **table, GX_UBYTE num_languages, UINT number_of_strings);
#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
UINT _gxe_display_language_direction_table_set(GX_DISPLAY *display, GX_CONST GX_UBYTE *language_direction_table, GX_UBYTE num_languages);
#endif
UINT _gxe_display_pixelmap_table_set(GX_DISPLAY *display, GX_PIXELMAP **pixelmap_table, UINT number_of_pixelmaps);

#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gxe_display_string_get(GX_DISPLAY *display, GX_RESOURCE_ID string_id, GX_CONST GX_CHAR **return_string);
UINT _gxe_display_string_table_get(GX_DISPLAY *display, GX_UBYTE language, GX_CHAR ***table, UINT *size);
#endif
UINT _gxe_display_string_get_ext(GX_DISPLAY *display, GX_RESOURCE_ID string_id, GX_STRING *return_string);
UINT _gxe_display_string_table_get_ext(GX_DISPLAY *display, GX_UBYTE language, GX_STRING **table, UINT *size);
UINT _gxe_display_theme_install(GX_DISPLAY *display, GX_CONST GX_THEME *theme_ptr);

#if defined(GX_MOUSE_SUPPORT)
VOID _gx_display_driver_generic_mouse_define(GX_DISPLAY *display, GX_CANVAS *canvas, GX_MOUSE_CURSOR_INFO *info);

#if !defined(GX_HARDWARE_MOUSE_SUPPORT)
VOID _gx_display_driver_generic_mouse_position_set(GX_DISPLAY *display, GX_POINT *point);
VOID _gx_display_driver_generic_mouse_enable(GX_DISPLAY *display, GX_BOOL enable);
VOID _gx_display_driver_generic_drawing_complete(GX_DISPLAY *display, GX_CANVAS *canvas);
VOID _gx_display_driver_generic_drawing_initiate(GX_DISPLAY *display, GX_CANVAS *canvas);
VOID _gx_display_driver_generic_mouse_draw(GX_DISPLAY *display);
VOID _gx_display_driver_24xrgb_mouse_capture(GX_DISPLAY *display);
VOID _gx_display_driver_24xrgb_mouse_restore(GX_DISPLAY *display);

VOID _gx_display_driver_16bpp_mouse_capture(GX_DISPLAY *display);
VOID _gx_display_driver_16bpp_mouse_restore(GX_DISPLAY *display);

VOID _gx_display_driver_8bpp_mouse_capture(GX_DISPLAY *display);
VOID _gx_display_driver_8bpp_mouse_restore(GX_DISPLAY *display);

VOID _gx_display_driver_4bpp_mouse_capture(GX_DISPLAY *display);
VOID _gx_display_driver_4bpp_mouse_restore(GX_DISPLAY *display);

VOID _gx_display_driver_1bpp_mouse_capture(GX_DISPLAY *display);
VOID _gx_display_driver_1bpp_mouse_restore(GX_DISPLAY *display);
#endif
#endif

/* Generic driver level functions (not specific to color depth) */

VOID _gx_display_driver_generic_simple_wide_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT ystart, INT xend, INT yend);
VOID _gx_display_driver_generic_aliased_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT ystart, INT xend, INT yend);
VOID _gx_display_driver_generic_aliased_fixed_point_line_draw(GX_DRAW_CONTEXT *context,
                                                              GX_FIXED_VAL xstart, GX_FIXED_VAL ystart,
                                                              GX_FIXED_VAL xend, GX_FIXED_VAL yend);
VOID _gx_display_driver_generic_aliased_wide_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT ystart, INT xend, INT yend);
VOID _gx_display_driver_generic_filled_circle_draw(GX_DRAW_CONTEXT *context, GX_FIXED_VAL xcenter, GX_FIXED_VAL ycenter, GX_FIXED_VAL r);
VOID _gx_display_driver_generic_aliased_filled_circle_draw(GX_DRAW_CONTEXT *context, GX_FIXED_VAL xcenter, GX_FIXED_VAL ycenter, GX_FIXED_VAL r);
VOID _gx_display_driver_generic_glyph_8bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area, GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph);
VOID _gx_display_driver_generic_glyph_4bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area, GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph);
VOID _gx_display_driver_32bpp_glyph_1bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area, GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph);
VOID _gx_display_driver_16bpp_glyph_1bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area, GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph);


/* Define screen driver function prototypes.  */
VOID     _gx_display_driver_1bpp_canvas_copy(GX_CANVAS *source, GX_CANVAS *dest);
VOID     _gx_display_driver_1bpp_horizontal_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT xend, INT ypos, INT width, GX_COLOR color);
VOID     _gx_display_driver_1bpp_horizontal_pattern_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT xend, INT ypos);
VOID     _gx_display_driver_1bpp_horizontal_pixelmap_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT xend, INT ypos, GX_FILL_PIXELMAP_INFO *info);
GX_COLOR _gx_display_driver_1bpp_native_color_get(GX_DISPLAY *, GX_COLOR rawcolor);
VOID     _gx_display_driver_1bpp_pixel_write(GX_DRAW_CONTEXT *context, INT xcoord, INT ycoord, GX_COLOR color);
VOID     _gx_display_driver_1bpp_vertical_line_draw(GX_DRAW_CONTEXT *context, INT ystart, INT yend, INT xpos, INT width, GX_COLOR color);
VOID     _gx_display_driver_1bpp_vertical_pattern_line_draw(GX_DRAW_CONTEXT *context, INT ystart, INT yend, INT xpos);
VOID     _gx_display_driver_1bpp_pixelmap_draw(GX_DRAW_CONTEXT *context, INT xstart, INT ystart, GX_PIXELMAP *pmp);
VOID     _gx_display_driver_1bpp_pixelmap_rotate(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap,
                                                 INT angle, INT rot_cx, INT rot_cy);
VOID     _gx_display_driver_1bpp_block_move(GX_DRAW_CONTEXT *context, GX_RECTANGLE *src, INT xshift, INT yshift);
USHORT   _gx_display_driver_1bpp_row_pitch_get(USHORT width);
VOID     _gx_display_driver_1bpp_simple_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT ystart, INT xend, INT yend);
VOID     _gx_display_driver_1bpp_glyph_1bpp_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area, GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph);

VOID     _gx_display_driver_332rgb_horizontal_pixelmap_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT xend, INT ypos, GX_FILL_PIXELMAP_INFO *info);
VOID     _gx_display_driver_332rgb_pixel_blend(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR fcolor, GX_UBYTE alpha);
VOID     _gx_display_driver_332rgb_pixelmap_draw(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap);
VOID     _gx_display_driver_332rgb_pixelmap_rotate(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap,
                                                   INT angle, INT rot_cx, INT rot_cy);
GX_COLOR _gx_display_driver_332rgb_native_color_get(GX_DISPLAY *display, GX_COLOR rawcolor);

VOID     _gx_display_driver_4bpp_block_move(GX_DRAW_CONTEXT *context, GX_RECTANGLE *src, INT xshift, INT yshift);
VOID     _gx_display_driver_4bpp_canvas_copy(GX_CANVAS *source, GX_CANVAS *dest);
VOID     _gx_display_driver_4bpp_glyph_1bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area, GX_POINT *map_offset, const GX_GLYPH *glyph);
VOID     _gx_display_driver_4bpp_glyph_4bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area, GX_POINT *map_offset, const GX_GLYPH *glyph);
VOID     _gx_display_driver_4bpp_horizontal_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT xend, INT ypos, INT width, GX_COLOR color);
VOID     _gx_display_driver_4bpp_horizontal_pattern_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT xend, INT ypos);
VOID     _gx_display_driver_4bpp_horizontal_pixelmap_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT xend, INT ypos, GX_FILL_PIXELMAP_INFO *info);
GX_COLOR _gx_display_driver_4bpp_native_color_get(GX_DISPLAY *, GX_COLOR rawcolor);
VOID     _gx_display_driver_4bpp_pixel_write(GX_DRAW_CONTEXT *context, INT xcoord, INT ycoord, GX_COLOR color);
VOID     _gx_display_driver_4bpp_pixelmap_draw(GX_DRAW_CONTEXT *context, INT xstart, INT ystart, GX_PIXELMAP *pmp);
VOID     _gx_display_driver_4bpp_pixelmap_rotate(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap, INT angle, INT rot_cx, INT rot_cy);
USHORT   _gx_display_driver_4bpp_row_pitch_get(USHORT width);
VOID     _gx_display_driver_4bpp_simple_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT ystart, INT xend, INT yend);
VOID     _gx_display_driver_4bpp_vertical_line_draw(GX_DRAW_CONTEXT *context, INT ystart, INT yend, INT xpos, INT width, GX_COLOR color);
VOID     _gx_display_driver_4bpp_vertical_pattern_line_draw(GX_DRAW_CONTEXT *context, INT ystart, INT yend, INT xpos);

VOID     _gx_display_driver_8bpp_canvas_copy(GX_CANVAS *source, GX_CANVAS *dest);
VOID     _gx_display_driver_8bpp_horizontal_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT xend, INT ypos, INT width, GX_COLOR color);
VOID     _gx_display_driver_8bpp_horizontal_pattern_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT xend, INT ypos);
VOID     _gx_display_driver_8bpp_horizontal_pixelmap_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT xend, INT ypos, GX_FILL_PIXELMAP_INFO *info);
GX_COLOR _gx_display_driver_8bit_palette_native_color_get(GX_DISPLAY *, GX_COLOR rawcolor);
VOID     _gx_display_driver_8bpp_pixel_write(GX_DRAW_CONTEXT *context, INT xcoord, INT ycoord, GX_COLOR color);
VOID     _gx_display_driver_8bpp_vertical_line_draw(GX_DRAW_CONTEXT *context, INT ystart, INT yend, INT xpos, INT width, GX_COLOR color);
VOID     _gx_display_driver_8bpp_pixelmap_blend(GX_DRAW_CONTEXT *context, INT xstart, INT ystart, GX_PIXELMAP *pmp, GX_UBYTE alpha);
VOID     _gx_display_driver_8bpp_vertical_pattern_line_draw(GX_DRAW_CONTEXT *context, INT ystart, INT yend, INT xpos);
VOID     _gx_display_driver_8bpp_pixelmap_draw(GX_DRAW_CONTEXT *context, INT xstart, INT ystart, GX_PIXELMAP *pmp);
VOID     _gx_display_driver_8bpp_pixelmap_simple_rotate(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap, INT angle, INT cx, INT cy);
VOID     _gx_display_driver_8bpp_pixelmap_rotate(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap, INT angle, INT rot_cx, INT rot_cy);
VOID     _gx_display_driver_8bpp_block_move(GX_DRAW_CONTEXT *context, GX_RECTANGLE *src, INT xshift, INT yshift);
USHORT   _gx_display_driver_8bpp_row_pitch_get(USHORT width);
VOID     _gx_display_driver_8bpp_simple_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT ystart, INT xend, INT yend);

VOID     _gx_display_driver_8bpp_glyph_1bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area, GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph);
VOID     _gx_display_driver_8bpp_glyph_3bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area, GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph);
VOID     _gx_display_driver_8bpp_glyph_4bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area, GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph);

VOID     _gx_display_driver_8bpp_rotated_block_move(GX_DRAW_CONTEXT *context, GX_RECTANGLE *src, INT xshift, INT yshift);
VOID     _gx_display_driver_8bpp_rotated_canvas_copy(GX_CANVAS *source, GX_CANVAS *dest);
VOID     _gx_display_driver_8bpp_rotated_glyph_1bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area, GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph);
VOID     _gx_display_driver_8bpp_rotated_glyph_3bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area, GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph);
VOID     _gx_display_driver_8bpp_rotated_glyph_4bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area, GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph);
VOID     _gx_display_driver_8bpp_rotated_horizontal_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT xend, INT ypos, INT width, GX_COLOR color);
VOID     _gx_display_driver_8bpp_rotated_horizontal_pattern_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT xend, INT ypos);
VOID     _gx_display_driver_8bpp_rotated_horizontal_pixelmap_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT xend, INT ypos, GX_FILL_PIXELMAP_INFO *info);
VOID     _gx_display_driver_8bpp_rotated_pixel_write(GX_DRAW_CONTEXT *context, INT xcoord, INT ycoord, GX_COLOR color);
VOID     _gx_display_driver_8bpp_rotated_pixelmap_draw(GX_DRAW_CONTEXT *context, INT xstart, INT ystart, GX_PIXELMAP *pmp);
VOID     _gx_display_driver_8bpp_rotated_pixelmap_rotate(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap, INT angle, INT cx, INT cy);
VOID     _gx_display_driver_8bpp_rotated_simple_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT ystart, INT xend, INT yend);
VOID     _gx_display_driver_8bpp_rotated_vertical_line_draw(GX_DRAW_CONTEXT *context, INT ystart, INT yend, INT xpos, INT width, GX_COLOR color);
VOID     _gx_display_driver_8bpp_rotated_vertical_pattern_line_draw(GX_DRAW_CONTEXT *context, INT ystart, INT yend, INT xpos);

VOID     _gx_display_driver_565rgb_canvas_blend(GX_CANVAS *source, GX_CANVAS *dest);
VOID     _gx_display_driver_16bpp_canvas_copy(GX_CANVAS *source, GX_CANVAS *dest);
VOID     _gx_display_driver_16bpp_horizontal_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT xend, INT ypos, INT width, GX_COLOR color);
VOID     _gx_display_driver_16bpp_horizontal_pattern_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT xend, INT ypos);
VOID     _gx_display_driver_565rgb_horizontal_pixelmap_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT xend, INT ypos, GX_FILL_PIXELMAP_INFO *info);
VOID     _gx_display_driver_565rgb_pixel_blend(GX_DRAW_CONTEXT *context, INT xcoord, INT ycoord, GX_COLOR fcolor, GX_UBYTE alpha);
VOID     _gx_display_driver_16bpp_pixel_write(GX_DRAW_CONTEXT *context, INT xcoord, INT ycoord, GX_COLOR color);
VOID     _gx_display_driver_16bpp_vertical_line_draw(GX_DRAW_CONTEXT *context, INT ystart, INT yend, INT xpos, INT width, GX_COLOR color);
VOID     _gx_display_driver_16bpp_vertical_pattern_line_draw(GX_DRAW_CONTEXT *context, INT ystart, INT yend, INT xpos);
VOID     _gx_display_driver_565rgb_pixelmap_blend(GX_DRAW_CONTEXT *context, INT xstart, INT ystart, GX_PIXELMAP *pmp, GX_UBYTE alpha);
VOID     _gx_display_driver_565rgb_pixelmap_draw(GX_DRAW_CONTEXT *context, INT xstart, INT ystart, GX_PIXELMAP *pmp);
#if defined(GX_SOFTWARE_DECODER_SUPPORT)
VOID     _gx_display_driver_565rgb_jpeg_draw(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap);
VOID     _gx_display_driver_565rgb_png_draw(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap);
#endif
VOID     _gx_display_driver_16bpp_block_move(GX_DRAW_CONTEXT *context, GX_RECTANGLE *src, INT xshift, INT yshift);
GX_COLOR _gx_display_driver_565rgb_native_color_get(GX_DISPLAY *, GX_COLOR rawcolor);
USHORT   _gx_display_driver_16bpp_row_pitch_get(USHORT width);
VOID     _gx_display_driver_16bpp_simple_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT ystart, INT xend, INT yend);
VOID     _gx_display_driver_16bpp_pixelmap_simple_alpha_rotate(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap, INT angle, INT cx, INT cy);
VOID     _gx_display_driver_16bpp_pixelmap_simple_rotate(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap, INT angle, INT cx, INT cy);
VOID     _gx_display_driver_16bpp_pixelmap_rotate(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap,
                                                  INT angle, INT rot_cx, INT rot_cy);

VOID     _gx_display_driver_16bpp_rotated_block_move(GX_DRAW_CONTEXT *context, GX_RECTANGLE *block, INT xshift, INT yshift);
VOID     _gx_display_driver_16bpp_rotated_canvas_copy(GX_CANVAS *canvas, GX_CANVAS *composite);
VOID     _gx_display_driver_16bpp_rotated_horizontal_pattern_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT xend, INT ypos);
VOID     _gx_display_driver_16bpp_rotated_horizontal_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT xend, INT ypos, INT width, GX_COLOR color);
VOID     _gx_display_driver_16bpp_rotated_pixel_write(GX_DRAW_CONTEXT *context, INT xcoord, INT ycoord, GX_COLOR color);
VOID     _gx_display_driver_16bpp_rotated_simple_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT ystart, INT xend, INT yend);
VOID     _gx_display_driver_16bpp_rotated_vertical_line_draw(GX_DRAW_CONTEXT *context, INT ystart, INT yend, INT xpos, INT width, GX_COLOR color);
VOID     _gx_display_driver_16bpp_rotated_vertical_pattern_line_draw(GX_DRAW_CONTEXT *context, INT ystart, INT yend, INT xpos);
VOID     _gx_display_driver_565rgb_rotated_canvas_blend(GX_CANVAS *canvas, GX_CANVAS *composite);
VOID     _gx_display_driver_565rgb_rotated_glyph_1bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area, GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph);
VOID     _gx_display_driver_565rgb_rotated_horizontal_pixelmap_line_draw(GX_DRAW_CONTEXT *context,
                                                                         INT xstart, INT xend, INT y, GX_FILL_PIXELMAP_INFO *info);
VOID     _gx_display_driver_565rgb_rotated_pixel_blend(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR fcolor, GX_UBYTE alpha);
VOID     _gx_display_driver_565rgb_rotated_pixelmap_draw(GX_DRAW_CONTEXT *context, INT xstart, INT ystart, GX_PIXELMAP *pmp);
VOID     _gx_display_driver_565rgb_rotated_pixelmap_blend(GX_DRAW_CONTEXT *context, INT xstart, INT ystart, GX_PIXELMAP *pmp, GX_UBYTE alpha);
VOID     _gx_display_driver_565rgb_rotated_pixelmap_rotate(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap,
                                                           INT angle, INT rot_cx, INT rot_cy);
#if defined(GX_SOFTWARE_DECODER_SUPPORT)
USHORT   _gx_display_driver_565rgb_YCbCr2RGB(INT y, INT cb, INT cr);
VOID     _gx_display_driver_565rgb_rotated_jpeg_draw(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap);
VOID     _gx_display_driver_565rgb_rotated_png_draw(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap);
#endif
VOID     _gx_display_driver_565rgb_rotated_setup(GX_DISPLAY *display, VOID *aux_data,
                                                 VOID (*toggle_function)(struct GX_CANVAS_STRUCT *canvas,
                                                                         GX_RECTANGLE *dirty_area));

VOID     _gx_display_driver_24xrgb_canvas_blend(GX_CANVAS *source, GX_CANVAS *dest);
VOID     _gx_display_driver_24xrgb_pixel_blend(GX_DRAW_CONTEXT *context, INT xcoord, INT ycoord, GX_COLOR fcolor, GX_UBYTE alpha);
VOID     _gx_display_driver_24xrgb_pixelmap_blend(GX_DRAW_CONTEXT *context, INT xstart, INT ystart, GX_PIXELMAP *pmp, GX_UBYTE alpha);
VOID     _gx_display_driver_24xrgb_pixelmap_draw(GX_DRAW_CONTEXT *context, INT xstart, INT ystart, GX_PIXELMAP *pmp);

VOID     _gx_display_driver_24xrgb_rotated_canvas_blend(GX_CANVAS *source, GX_CANVAS *dest);
VOID     _gx_display_driver_24xrgb_rotated_pixel_blend(GX_DRAW_CONTEXT *context, INT xcoord, INT ycoord, GX_COLOR fcolor, GX_UBYTE alpha);


#if defined(GX_SOFTWARE_DECODER_SUPPORT)
UINT     _gx_display_driver_24xrgb_YCbCr2RGB(INT y, INT cb, INT cr);
VOID     _gx_display_driver_24xrgb_jpeg_draw(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap);
VOID     _gx_display_driver_24xrgb_png_draw(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap);
VOID     _gx_display_driver_24xrgb_rotated_jpeg_draw(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap);
VOID     _gx_display_driver_24xrgb_rotated_png_draw(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap);
#endif
GX_COLOR _gx_display_driver_24xrgb_native_color_get(GX_DISPLAY *, GX_COLOR rawcolor);

VOID     _gx_display_driver_32bpp_canvas_copy(GX_CANVAS *source, GX_CANVAS *dest);
VOID     _gx_display_driver_32bpp_horizontal_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT xend, INT ypos, INT width, GX_COLOR color);
VOID     _gx_display_driver_32bpp_horizontal_pattern_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT xend, INT ypos);
VOID     _gx_display_driver_32bpp_horizontal_pixelmap_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT xend, INT ypos, GX_FILL_PIXELMAP_INFO *info);
VOID     _gx_display_driver_32bpp_pixel_write(GX_DRAW_CONTEXT *context, INT xcoord, INT ycoord, GX_COLOR color);
VOID     _gx_display_driver_32bpp_vertical_line_draw(GX_DRAW_CONTEXT *context, INT ystart, INT yend, INT xpos, INT width, GX_COLOR color);
VOID     _gx_display_driver_32bpp_vertical_pattern_line_draw(GX_DRAW_CONTEXT *context, INT ystart, INT yend, INT xpos);
VOID     _gx_display_driver_32bpp_block_move(GX_DRAW_CONTEXT *context, GX_RECTANGLE *src, INT xshift, INT yshift);
USHORT   _gx_display_driver_32bpp_row_pitch_get(USHORT width);
VOID     _gx_display_driver_32bpp_simple_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT ystart, INT xend, INT yend);
VOID     _gx_display_driver_32bpp_pixelmap_rotate(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap,
                                                  INT angle, INT rot_cx, INT rot_cy);

VOID     _gx_display_driver_32bpp_rotated_canvas_copy(GX_CANVAS *source, GX_CANVAS *dest);
VOID     _gx_display_driver_32bpp_rotated_glyph_1bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area, GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph);
VOID     _gx_display_driver_32bpp_rotated_horizontal_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT xend, INT ypos, INT width, GX_COLOR color);
VOID     _gx_display_driver_32bpp_rotated_horizontal_pattern_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT xend, INT ypos);
VOID     _gx_display_driver_32bpp_rotated_horizontal_pixelmap_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT xend, INT ypos, GX_FILL_PIXELMAP_INFO *info);
VOID     _gx_display_driver_32bpp_rotated_pixel_write(GX_DRAW_CONTEXT *context, INT xcoord, INT ycoord, GX_COLOR color);
VOID     _gx_display_driver_32bpp_rotated_pixelmap_blend(GX_DRAW_CONTEXT *context, INT xstart, INT ystart, GX_PIXELMAP *pmp, GX_UBYTE alpha);
VOID     _gx_display_driver_32bpp_rotated_pixelmap_draw(GX_DRAW_CONTEXT *context, INT xstart, INT ystart, GX_PIXELMAP *pmp);
VOID     _gx_display_driver_32bpp_rotated_pixelmap_rotate(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap, INT angle, INT rot_cx, INT rot_cy);
VOID     _gx_display_driver_32bpp_rotated_vertical_line_draw(GX_DRAW_CONTEXT *context, INT ystart, INT yend, INT xpos, INT width, GX_COLOR color);
VOID     _gx_display_driver_32bpp_rotated_vertical_pattern_line_draw(GX_DRAW_CONTEXT *context, INT ystart, INT yend, INT xpos);
VOID     _gx_display_driver_32bpp_rotated_block_move(GX_DRAW_CONTEXT *context, GX_RECTANGLE *src, INT xshift, INT yshift);
VOID     _gx_display_driver_32bpp_rotated_simple_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT ystart, INT xend, INT yend);

VOID     _gx_display_driver_32argb_pixel_blend(GX_DRAW_CONTEXT *context, INT xcoord, INT ycoord, GX_COLOR fcolor, GX_UBYTE alpha);
VOID     _gx_display_driver_32argb_pixelmap_blend(GX_DRAW_CONTEXT *context, INT xstart, INT ystart, GX_PIXELMAP *pmp, GX_UBYTE alpha);
VOID     _gx_display_driver_32argb_pixelmap_draw(GX_DRAW_CONTEXT *context, INT xstart, INT ystart, GX_PIXELMAP *pmp);
GX_COLOR _gx_display_driver_32argb_native_color_get(GX_DISPLAY *, GX_COLOR rawcolor);

VOID     _gx_display_driver_32argb_rotated_pixel_blend(GX_DRAW_CONTEXT *context, INT xcoord, INT ycoord, GX_COLOR fcolor, GX_UBYTE alpha);

VOID     _gx_display_driver_4444argb_canvas_blend(GX_CANVAS *canvas, GX_CANVAS *composite);
VOID     _gx_display_driver_4444argb_pixel_blend(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR fcolor, GX_UBYTE alpha);
VOID     _gx_display_driver_4444argb_horizontal_pixelmap_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT xend, INT ypos, GX_FILL_PIXELMAP_INFO *info);
VOID     _gx_display_driver_4444argb_pixelmap_draw(GX_DRAW_CONTEXT *context,
                                                   INT xpos, INT ypos, GX_PIXELMAP *pixelmap);
VOID     _gx_display_driver_4444argb_pixelmap_blend(GX_DRAW_CONTEXT *context,
                                                    INT xpos, INT ypos, GX_PIXELMAP *pixelmap, GX_UBYTE alpha);
VOID     _gx_display_driver_4444argb_pixelmap_rotate(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap,
                                                     INT angle, INT rot_cx, INT rot_cy);
GX_COLOR _gx_display_driver_4444argb_native_color_get(GX_DISPLAY *display, GX_COLOR rawcolor);


VOID     _gx_display_driver_1555xrgb_jpeg_draw(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap);
GX_COLOR _gx_display_driver_1555xrgb_native_color_get(GX_DISPLAY *display, GX_COLOR rawcolor);
VOID     _gx_display_driver_1555xrgb_pixel_blend(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR fcolor, GX_UBYTE alpha);
VOID     _gx_display_driver_1555xrgb_pixelmap_blend(GX_DRAW_CONTEXT *context,
                                                    INT xpos, INT ypos, GX_PIXELMAP *pixelmap, GX_UBYTE alpha);
VOID     _gx_display_driver_1555xrgb_pixelmap_draw(GX_DRAW_CONTEXT *context,
                                                   INT xpos, INT ypos, GX_PIXELMAP *pixelmap);
VOID     _gx_display_driver_1555xrgb_pixelmap_rotate(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap,
                                                     INT angle, INT rot_cx, INT rot_cy);
VOID     _gx_display_driver_1555xrgb_png_draw(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap);

#if defined(GX_BRUSH_ALPHA_SUPPORT)
VOID     _gx_display_driver_horizontal_line_alpha_draw(GX_DRAW_CONTEXT *context, INT xstart, INT xend, INT ypos, INT width, GX_COLOR color, GX_UBYTE alpha);
VOID     _gx_display_driver_vertical_line_alpha_draw(GX_DRAW_CONTEXT *context, INT ystart, INT yend, INT xpos, INT width, GX_COLOR color, GX_UBYTE alpha);
VOID     _gx_display_driver_simple_line_alpha_draw(GX_DRAW_CONTEXT *context, INT xstart, INT ystart, INT xend, INT yend, GX_UBYTE alpha);
#endif

VOID     _gx_display_driver_generic_alphamap_draw(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pmp);

GX_FIXED_POINT *_gx_display_driver_generic_wide_line_points_calculate(GX_DRAW_CONTEXT *context, INT xStart, INT yStart,
                                                                INT xEnd, INT yEnd, INT brush_width, GX_BOOL outline);
VOID     _gx_display_driver_generic_wide_line_fill(GX_DRAW_CONTEXT *context, GX_FIXED_POINT *pPoints);

VOID     _gx_display_driver_generic_polygon_draw(GX_DRAW_CONTEXT *context, GX_POINT *vertex, INT num);
VOID     _gx_display_driver_generic_polygon_fill(GX_DRAW_CONTEXT *context, GX_POINT *vertex, INT num);
VOID     _gx_display_driver_generic_rotated_wide_line_fill(GX_DRAW_CONTEXT *context, GX_FIXED_POINT *pPoints);
VOID     _gx_display_driver_generic_rotated_polygon_fill(GX_DRAW_CONTEXT *context, GX_POINT *vertex, INT num);
#if defined(GX_ARC_DRAWING_SUPPORT)
VOID     _gx_display_driver_generic_circle_draw(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r);
VOID     _gx_display_driver_generic_wide_circle_draw(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r);
VOID     _gx_display_driver_generic_aliased_circle_draw(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r);
VOID     _gx_display_driver_generic_aliased_wide_circle_draw(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r);
VOID     _gx_display_driver_generic_circle_fill(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r);
VOID     _gx_display_driver_generic_pie_fill(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r, INT start_angle, INT end_angle);
VOID     _gx_display_driver_generic_simple_pie_fill(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r, INT start_angle, INT end_angle, GX_BOOL skip_end);

VOID     _gx_display_driver_generic_arc_draw(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r, INT start_angle, INT end_angle);
VOID     _gx_display_driver_generic_wide_arc_draw(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r, INT start_angle, INT end_angle);
VOID     _gx_display_driver_generic_aliased_arc_draw(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r, INT start_angle, INT end_angle);
VOID     _gx_display_driver_generic_aliased_wide_arc_draw(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r, INT start_angle, INT end_angle);
VOID     _gx_display_driver_generic_arc_fill(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r, INT start_angle, INT end_angle);
VOID     _gx_display_driver_arc_clipping_get(INT xcenter, INT ycenter, UINT r, INT start_angle, INT end_angle,
                                             GX_RECTANGLE *clip_1, GX_RECTANGLE *clip_2, GX_RECTANGLE *clip_3, GX_RECTANGLE *clip_4);

VOID     _gx_display_driver_generic_rotated_arc_fill(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r, INT start_angle, INT end_angle);
VOID     _gx_display_driver_generic_rotated_circle_fill(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r);
VOID     _gx_display_driver_generic_rotated_ellipse_fill(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, INT a, INT b);
VOID     _gx_display_driver_generic_rotated_pie_fill(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r, INT start_angle, INT end_angle);
#endif

VOID     _gx_display_driver_generic_aliased_ellipse_draw(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, INT a, INT b);
VOID     _gx_display_driver_generic_aliased_wide_ellipse_draw(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, INT a, INT b);
VOID     _gx_display_driver_generic_ellipse_draw(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, INT a, INT b);
VOID     _gx_display_driver_generic_ellipse_fill(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, INT a, INT b);
VOID     _gx_display_driver_generic_rotated_alphamap_draw(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pmp);
VOID     _gx_display_driver_generic_rotated_glyph_4bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area, GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph);
VOID     _gx_display_driver_generic_rotated_glyph_8bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area, GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph);
VOID     _gx_display_driver_generic_wide_ellipse_draw(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, INT a, INT b);

VOID _gx_display_driver_32argb_setup(GX_DISPLAY *display, VOID *aux_data,
                                     VOID (*toggle_function)(struct GX_CANVAS_STRUCT *canvas,
                                                             GX_RECTANGLE *dirty_area));

VOID _gx_display_driver_32argb_rotated_setup(GX_DISPLAY *display, VOID *aux_data,
                                             VOID (*toggle_function)(struct GX_CANVAS_STRUCT *canvas,
                                                                     GX_RECTANGLE *dirty_area));

VOID _gx_display_driver_24xrgb_setup(GX_DISPLAY *display, VOID *aux_data,
                                     VOID (*toggle_function)(struct GX_CANVAS_STRUCT *canvas,
                                                             GX_RECTANGLE *dirty_area));

VOID _gx_display_driver_24xrgb_rotated_setup(GX_DISPLAY *display, VOID *aux_data,
                                             VOID (*toggle_function)(struct GX_CANVAS_STRUCT *canvas,
                                                                     GX_RECTANGLE *dirty_area));

VOID _gx_display_driver_4444argb_setup(GX_DISPLAY *display, VOID *aux_data,
                                       VOID (*toggle_function)(struct GX_CANVAS_STRUCT *canvas,
                                                               GX_RECTANGLE *dirty_area));

VOID _gx_display_driver_4bpp_grayscale_setup(GX_DISPLAY *display, VOID *aux_data,
                                             VOID (*toggle_function)(struct GX_CANVAS_STRUCT *canvas,
                                                                     GX_RECTANGLE *dirty_area));

VOID _gx_display_driver_565rgb_setup(GX_DISPLAY *display, VOID *aux_data,
                                     VOID (*toggle_function)(struct GX_CANVAS_STRUCT *canvas,
                                                             GX_RECTANGLE *dirty_area));

VOID _gx_display_driver_8bit_palette_setup(GX_DISPLAY *display, VOID *aux_data,
                                           VOID (*toggle_function)(struct GX_CANVAS_STRUCT *canvas,
                                                                   GX_RECTANGLE *dirty_area));

VOID _gx_display_driver_8bit_palette_rotated_setup(GX_DISPLAY *display, VOID *aux_data,
                                                   VOID (*toggle_function)(struct GX_CANVAS_STRUCT *canvas,
                                                                           GX_RECTANGLE *dirty_area));

VOID _gx_display_driver_332rgb_setup(GX_DISPLAY *display, VOID *aux_data,
                                     VOID (*toggle_function)(struct GX_CANVAS_STRUCT *canvas,
                                                             GX_RECTANGLE *dirty_area));

VOID _gx_display_driver_monochrome_setup(GX_DISPLAY *display, VOID *aux_data,
                                         VOID (*toggle_function)(struct GX_CANVAS_STRUCT *canvas,
                                                                 GX_RECTANGLE *dirty_area));

VOID _gx_display_driver_1555xrgb_setup(GX_DISPLAY *display, VOID *aux_data,
                                       VOID (*toggle_function)(struct GX_CANVAS_STRUCT *canvas,
                                                               GX_RECTANGLE *dirty_area));

VOID _gx_display_driver_1555xrgb_canvas_blend(GX_CANVAS *canvas, GX_CANVAS *composite);

#ifdef GX_TARGET_WIN32
VOID *_win32_canvas_memory_prepare(GX_CANVAS *canvas, GX_RECTANGLE *dirty);
#endif

#define REDVAL_16BPP(_c)      (GX_UBYTE)(((_c) >> 11) & 0x1f)
#define GREENVAL_16BPP(_c)    (GX_UBYTE)(((_c) >> 5) & 0x3f)
#define BLUEVAL_16BPP(_c)     (GX_UBYTE)(((_c)) & 0x1f)

/* Define macros for assembling a 16-bit r:g:b value from 3 components.  */

#define ASSEMBLECOLOR_16BPP(_r, _g, _b) \
    ((((_r) & 0x1f) << 11) |            \
     (((_g) & 0x3f) << 5) |             \
     (((_b) & 0x1f)))


#define REDVAL_24BPP(_c)      (GX_UBYTE)((_c) >> 16)
#define GREENVAL_24BPP(_c)    (GX_UBYTE)((_c) >> 8)
#define BLUEVAL_24BPP(_c)     (GX_UBYTE)(_c)

/* Define macros for assembling a 24-bit r:g:b value from 3 components.  */
#define ASSEMBLECOLOR_24BPP(_r, _g, _b) \
    (((_r) << 16) |                     \
     ((_g) << 8) |                      \
     (_b))


#define ALPHAVAL_32BPP(_c)    (GX_UBYTE)((_c) >> 24)
#define REDVAL_32BPP(_c)      (GX_UBYTE)((_c) >> 16)
#define GREENVAL_32BPP(_c)    (GX_UBYTE)((_c) >> 8)
#define BLUEVAL_32BPP(_c)     (GX_UBYTE)(_c)

/* Define macros for assembling a 32-bit r:g:b value from 3 components.  */

#define ASSEMBLECOLOR_32BPP(_r, _g, _b) \
    (((_r) << 16) |                     \
     ((_g) << 8) |                      \
     (_b))


#define ASSEMBLECOLOR_32ARGB(_a, _r, _g, _b) \
    (((_a) << 24) |                          \
     ((_r) << 16) |                          \
     ((_g) << 8) |                           \
     (_b))

#define GX_SET_BLEND_FUNCTION(blend_func, color_format)     \
    switch (color_format)                                   \
    {                                                       \
    case GX_COLOR_FORMAT_565RGB:                            \
        blend_func = _gx_display_driver_565rgb_pixel_blend; \
        break;                                              \
    case GX_COLOR_FORMAT_24XRGB:                            \
        blend_func = _gx_display_driver_24xrgb_pixel_blend; \
        break;                                              \
    case GX_COLOR_FORMAT_32ARGB:                            \
        blend_func = _gx_display_driver_32argb_pixel_blend; \
        break;                                              \
    default:                                                \
        /* Not supported. */                                \
        return;                                             \
    }

#define GX_SET_32BPP_BLEND_FUNCTION(blend_func, color_format) \
    switch(color_format)                                      \
    {                                                         \
    case GX_COLOR_FORMAT_24XRGB:                              \
        blend_func = _gx_display_driver_24xrgb_pixel_blend;   \
        break;                                                \
    case GX_COLOR_FORMAT_32ARGB:                              \
        blend_func = _gx_display_driver_32argb_pixel_blend;   \
        break;                                                \
    default:                                                  \
        /* Not supported. */                                  \
        return;                                               \
    }
#endif

