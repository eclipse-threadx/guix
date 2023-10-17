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
/**   Synergy Simulation Display Management (Display)                     */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/
/*                                                                        */
/*  COMPONENT DEFINITION                                   RELEASE        */
/*                                                                        */
/*    gx_dave2d_simulation_display_driver.h              PORTABLE C       */
/*                                                           6.x          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Define drivers for Synergy Win32 simulation.                        */
/*                                                                        */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  12-31-2020     Kenneth Maxwell          Initial Version 6.1.3         */
/*  xx-xx-xxxx     Kenneth Maxwell          Modify comments, change       */
/*                                            file/function names,        */
/*                                            resulting in version 6.x    */
/*                                                                        */
/**************************************************************************/

#ifndef GX_DAVE2D_SIMULATION_DISPLAY_H
#define GX_DAVE2D_SIMULATION_DISPLAY_H



VOID _gx_dave2d_simulation_display_driver_16bpp_compressed_glyph_1bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area,
                                                                            GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph);
VOID _gx_dave2d_simulation_display_driver_16bpp_glyph_1bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area,
                                                                 GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph);
VOID _gx_dave2d_simulation_display_driver_16bpp_pixelmap_draw(GX_DRAW_CONTEXT *context,
                                                               INT xpos, INT ypos, GX_PIXELMAP *pixelmap);
VOID _gx_dave2d_simulation_display_driver_565rgb_pixelmap_blend(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap, GX_UBYTE alpha);
VOID _gx_dave2d_simulation_display_driver_565rgb_setup(GX_DISPLAY *display, VOID *aux_data,
                                                        VOID (*toggle_function)(struct GX_CANVAS_STRUCT *canvas, GX_RECTANGLE *dirty_area));
VOID _gx_dave2d_simulation_display_driver_565rgb_horizontal_pixelmap_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT xend, INT y, struct GX_FILL_PIXELMAP_INFO_STRUCT *info);


VOID _gx_dave2d_simulation_display_driver_32bpp_compressed_glyph_1bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area,
                                                                            GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph);
VOID _gx_dave2d_simulation_display_driver_32bpp_glyph_1bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area,
                                                                 GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph);
VOID _gx_dave2d_simulation_display_driver_32bpp_horizontal_pixelmap_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT xend, INT y, struct GX_FILL_PIXELMAP_INFO_STRUCT *info);
VOID _gx_dave2d_simulation_display_driver_24xrgb_pixelmap_blend(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap, GX_UBYTE alpha);
VOID _gx_dave2d_simulation_display_driver_24xrgb_pixelmap_draw(GX_DRAW_CONTEXT *context,
                                                                INT xpos, INT ypos, GX_PIXELMAP *pixelmap);
VOID _gx_dave2d_simulation_display_driver_24xrgb_setup(GX_DISPLAY *display, VOID *aux_data,
                                                        VOID (*toggle_function)(struct GX_CANVAS_STRUCT *canvas, GX_RECTANGLE *dirty_area));

VOID _gx_dave2d_simulation_display_driver_8bpp_glyph_4bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area, GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph);
VOID _gx_dave2d_simulation_display_driver_8bpp_glyph_1bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area, GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph);
VOID _gx_dave2d_simulation_display_driver_8bit_palette_setup(GX_DISPLAY *display, VOID *aux_data,
                                                              VOID (*toggle_function)(struct GX_CANVAS_STRUCT *canvas, GX_RECTANGLE *dirty_area));
VOID _gx_dave2d_simulation_display_driver_8bpp_rotated_glyph_4bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area, GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph);
VOID _gx_dave2d_simulation_display_driver_8bpp_rotated_glyph_1bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area, GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph);
VOID _gx_dave2d_simulation_display_driver_8bit_palette_rotated_setup(GX_DISPLAY *display, VOID *aux_data,
                                                                      VOID (*toggle_function)(struct GX_CANVAS_STRUCT *canvas, GX_RECTANGLE *dirty_area));

VOID _gx_dave2d_simulation_display_driver_compressed_glyph_8bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area,
                                                                      GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph);
VOID _gx_dave2d_simulation_display_driver_compressed_glyph_4bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area,
                                                                      GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph);
VOID _gx_dave2d_simulation_display_driver_glyph_4bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area,
                                                           GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph);

/* Declare rotated display drivers prototypes. */
VOID _gx_dave2d_simulation_display_driver_32bpp_rotated_compressed_glyph_1bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area,
                                                                                    GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph);
VOID _gx_dave2d_simulation_display_driver_32bpp_rotated_glyph_1bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area,
                                                                         GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph);
VOID _gx_dave2d_simulation_display_driver_32bpp_rotated_horizontal_pixelmap_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT xend, INT y, struct GX_FILL_PIXELMAP_INFO_STRUCT *info);
VOID _gx_dave2d_simulation_display_driver_24xrgb_rotated_pixelmap_blend(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap, GX_UBYTE alpha);
VOID _gx_dave2d_simulation_display_driver_24xrgb_rotated_pixelmap_draw(GX_DRAW_CONTEXT *context,
                                                                        INT xpos, INT ypos, GX_PIXELMAP *pixelmap);
VOID _gx_dave2d_simulation_display_driver_24xrgb_rotated_setup(GX_DISPLAY *display, VOID *aux_data,
                                                                VOID (*toggle_function)(struct GX_CANVAS_STRUCT *canvas, GX_RECTANGLE *dirty_area));

VOID _gx_dave2d_simulation_display_driver_16bpp_rotated_compressed_glyph_1bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area,
                                                                                    GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph);
VOID _gx_dave2d_simulation_display_driver_16bpp_rotated_glyph_1bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area,
                                                                         GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph);
VOID _gx_dave2d_simulation_display_driver_565rgb_rotated_pixelmap_draw(GX_DRAW_CONTEXT *context,
                                                                        INT xpos, INT ypos, GX_PIXELMAP *pixelmap);
VOID _gx_dave2d_simulation_display_driver_565rgb_rotated_pixelmap_blend(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap, GX_UBYTE alpha);
VOID _gx_dave2d_simulation_display_driver_565rgb_rotated_setup(GX_DISPLAY *display, VOID *aux_data,
                                                                VOID (*toggle_function)(struct GX_CANVAS_STRUCT *canvas, GX_RECTANGLE *dirty_area));
VOID _gx_dave2d_simulation_display_driver_565rgb_rotated_horizontal_pixelmap_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT xend, INT y, struct GX_FILL_PIXELMAP_INFO_STRUCT *info);
VOID _gx_dave2d_simulation_display_driver_rotated_compressed_glyph_8bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area,
                                                                              GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph);
VOID _gx_dave2d_simulation_display_driver_rotated_compressed_glyph_4bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area,
                                                                              GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph);
VOID _gx_dave2d_simulation_display_driver_rotated_glyph_4bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area,
                                                                   GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph);

#endif

