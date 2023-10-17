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

/***********************************************************************************************************************
 * Copyright [2017] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
 *
 * This file is part of Renesas SynergyTM Software Package (SSP)
 *
 * The contents of this file (the "contents") are proprietary and confidential to Renesas Electronics Corporation
 * and/or its licensors ("Renesas") and subject to statutory and contractual protections.
 *
 * This file is subject to a Renesas SSP license agreement. Unless otherwise agreed in an SSP license agreement with
 * Renesas: 1) you may not use, copy, modify, distribute, display, or perform the contents; 2) you may not use any name
 * or mark of Renesas for advertising or publicity purposes or in connection with your use of the contents; 3) RENESAS
 * MAKES NO WARRANTY OR REPRESENTATIONS ABOUT THE SUITABILITY OF THE CONTENTS FOR ANY PURPOSE; THE CONTENTS ARE PROVIDED
 * "AS IS" WITHOUT ANY EXPRESS OR IMPLIED WARRANTY, INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, AND NON-INFRINGEMENT; AND 4) RENESAS SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, OR
 * CONSEQUENTIAL DAMAGES, INCLUDING DAMAGES RESULTING FROM LOSS OF USE, DATA, OR PROJECTS, WHETHER IN AN ACTION OF
 * CONTRACT OR TORT, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE CONTENTS. Third-party contents
 * included in this file may be subject to different terms.
 **********************************************************************************************************************/

/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** GUIX Display Driver component                                         */
/**************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include    <stdio.h>
#include    <string.h>

#include    "gx_api.h"
#include    "gx_display.h"
#include    "gx_utility.h"

#if (GX_USE_SYNERGY_DRW == 1)
#include    "dave_driver.h"
#endif

/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/
#ifndef GX_DISABLE_ERROR_CHECKING
#define LOG_DAVE_ERRORS
#endif

/** Space used to store int to fixed point polygon vertices. */
#define MAX_POLYGON_VERTICES GX_POLYGON_MAX_EDGE_NUM

/*LDRA_INSPECTED 77 S This macro does not work when surrounded by parentheses. */
#define DRAW_PIXEL  if (glyph_data & mask)              \
                    {                                   \
                        *put = text_color;              \
                    }                                   \
                    put++;                              \
                    mask = (GX_UBYTE)(mask << 1);

#if defined(LOG_DAVE_ERRORS)
/** Macro to check for and log status code from Dave2D engine. */
/*LDRA_INSPECTED 77 S This macro does not work when surrounded by parentheses. */
#define CHECK_DAVE_STATUS(a)    gx_log_dave_error(a);
#else
#define CHECK_DAVE_STATUS(a)    a;
#endif

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/** indicator for the number of visible frame buffer */
typedef enum e_frame_buffers
{
    FRAME_BUFFER_A = 0,
    FRAME_BUFFER_B
} frame_buffers_t;

#if (GX_USE_SYNERGY_DRW == 1)
/** DAVE 2D screen rotation parameters */
typedef struct st_d2_rotation_param
{
    d2_border xmin;
    d2_border ymin;
    d2_border xmax;
    d2_border ymax;
    d2_point  x_texture_zero;
    d2_point  y_texture_zero;
    d2_s32    dxu;
    d2_s32    dxv;
    d2_s32    dyu;
    d2_s32    dyv;
    GX_VALUE  x_resolution;
    GX_VALUE  y_resolution;
    INT       copy_width;
    INT       copy_height;
    INT       copy_width_rotated;
    INT       copy_height_rotated;
    GX_RECTANGLE    copy_clip;
    INT       angle;
} d2_rotation_param_t;
#endif

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
/** indicator for the number of visible frame buffer */
static GX_UBYTE   *visible_frame = NULL;
static GX_UBYTE   *working_frame = NULL;

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
static VOID     gx_copy_visible_to_working_8bpp(GX_CANVAS *canvas, GX_RECTANGLE *copy);

static VOID     gx_flip_canvas_to_working_8bpp(GX_CANVAS *canvas, GX_RECTANGLE *copy);

#if (GX_USE_SYNERGY_DRW == 1)
static VOID     gx_dave2d_set_texture_8bpp(GX_DRAW_CONTEXT *context, d2_device *dave, INT xpos,
                                                                                        INT ypos, GX_PIXELMAP *map);

static VOID     gx_dave2d_copy_visible_to_working_8bpp(GX_CANVAS *canvas, GX_RECTANGLE *copy);

#endif

/** functions shared in Synergy GUIX display driver files */
#if (GX_USE_SYNERGY_DRW == 1)
#if defined(LOG_DAVE_ERRORS)
extern VOID         gx_log_dave_error(d2_s32 status);
extern INT          gx_get_dave_error(INT get_index);
#endif
extern VOID         gx_dave2d_display_list_count(GX_DISPLAY *display);
extern GX_BOOL      gx_dave2d_convex_polygon_test(GX_POINT *vertex, INT num);
extern VOID         gx_display_list_flush(GX_DISPLAY *display);
extern VOID         gx_display_list_open(GX_DISPLAY *display);
extern d2_device  * gx_dave2d_context_clip_set(GX_DRAW_CONTEXT *context);

#endif

/** functions provided by sf_el_gx.c */
extern void     sf_el_gx_frame_toggle (ULONG display_handle, GX_UBYTE **visible);
extern void     sf_el_gx_frame_pointers_get(ULONG display_handle, GX_UBYTE **visible, GX_UBYTE **working);
extern void     sf_el_gx_display_actual_size_get(ULONG display_handle, INT *width, INT *height);
extern void     sf_el_gx_display_8bit_palette_assign(ULONG display_handle);

/***********************************************************************************************************************
 * Synergy GUIX display driver function prototypes (called by GUIX)
 **********************************************************************************************************************/
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
extern VOID         _gx_dave2d_drawing_initiate(GX_DISPLAY *display, GX_CANVAS *canvas);
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_synergy_buffer_toggle_8bpp(GX_CANVAS *canvas, GX_RECTANGLE *dirty);
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_display_driver_8bit_palette_assign(GX_DISPLAY *display, GX_COLOR *palette, INT count);

#if (GX_USE_SYNERGY_DRW == 1)
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_drawing_initiate_8bpp(GX_DISPLAY  *display, GX_CANVAS *canvas);
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_drawing_complete_8bpp(GX_DISPLAY  *display, GX_CANVAS *canvas);
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_horizontal_line_8bpp(GX_DRAW_CONTEXT  *context,
                               INT xstart, INT xend, INT ypos, INT width, GX_COLOR color);
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_vertical_line_8bpp(GX_DRAW_CONTEXT  *context,
                             INT ystart, INT yend, INT xpos, INT width, GX_COLOR color);
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_simple_line_draw_8bpp(GX_DRAW_CONTEXT  *context, INT xstart, INT ystart, INT xend, INT yend);
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_simple_wide_line_8bpp(GX_DRAW_CONTEXT  *context, INT xstart, INT ystart, INT xend, INT yend);
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_horizontal_pattern_line_draw_8bpp(GX_DRAW_CONTEXT  *context, INT xstart, INT xend, INT ypos);
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_vertical_pattern_line_draw_8bpp(GX_DRAW_CONTEXT  *context, INT ystart, INT yend, INT xpos);
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_pixelmap_draw_8bpp(GX_DRAW_CONTEXT  *context, INT xpos, INT ypos, GX_PIXELMAP * pixelmap);
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_polygon_draw_8bpp(GX_DRAW_CONTEXT  *context, GX_POINT * vertex, INT num);
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_polygon_fill_8bpp(GX_DRAW_CONTEXT  *context, GX_POINT * vertex, INT num);
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_pixel_write_8bpp(GX_DRAW_CONTEXT  *context, INT x, INT y, GX_COLOR color);
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_block_move_8bpp(GX_DRAW_CONTEXT  *context, GX_RECTANGLE *block, INT xshift, INT yshift);
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_glyph_1bit_draw_8bpp(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area, GX_POINT *map_offset,
                                                            const GX_GLYPH *glyph);
VOID _gx_dave2d_glyph_3bit_draw_8bpp(GX_DRAW_CONTEXT  *context, GX_RECTANGLE * draw_area, GX_POINT *map_offset,
                                                            const GX_GLYPH *glyph);
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_glyph_4bit_draw_8bpp(GX_DRAW_CONTEXT  *context, GX_RECTANGLE * draw_area, GX_POINT * map_offset,
                                                                                                const GX_GLYPH * glyph);
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_circle_draw_8bpp(GX_DRAW_CONTEXT  *context, INT xcenter, INT ycenter, UINT r);
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_circle_fill_8bpp(GX_DRAW_CONTEXT  *context, INT xcenter, INT ycenter, UINT r);
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_arc_draw_8bpp(GX_DRAW_CONTEXT  *context, INT xcenter, INT ycenter, UINT r, INT start_angle,
                                                                                                    INT end_angle);
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_arc_fill_8bpp(GX_DRAW_CONTEXT  *context, INT xcenter, INT ycenter, UINT r, INT start_angle,
                                                                                                    INT end_angle);
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_pie_fill_8bpp(GX_DRAW_CONTEXT  *context, INT xcenter, INT ycenter, UINT r, INT start_angle,
                                                                                                    INT end_angle);
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_ellipse_draw_8bpp(GX_DRAW_CONTEXT  *context, INT xcenter, INT ycenter, INT a, INT b);
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_wide_ellipse_draw_8bpp(GX_DRAW_CONTEXT  *context, INT xcenter, INT ycenter, INT a, INT b);
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_ellipse_fill_8bpp(GX_DRAW_CONTEXT  *context, INT xcenter, INT ycenter, INT a, INT b);
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_buffer_toggle_8bpp (GX_CANVAS *canvas, GX_RECTANGLE * dirty);
#endif

/***********************************************************************************************************************
 * Functions
 ***********************************************************************************************************************/
#if (GX_USE_SYNERGY_DRW == 1)

/*******************************************************************************************************************//**
 * @addtogroup SF_EL_GX
 * @{
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief  GUIX display driver for Synergy, 8bpp draw function to close previous frame and set new canvas drawing
 * address. This function is called by GUIX to initiate canvas drawing.
 * @param   display[in]         Pointer to a GUIX display context
 * @param   canvas[in]          Pointer to a GUIX canvas
 **********************************************************************************************************************/
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_drawing_initiate_8bpp(GX_DISPLAY  *display, GX_CANVAS *canvas)
{
    /** Close previous frame and set new canvas drawing address. */
    _gx_dave2d_drawing_initiate(display, canvas);

    d2_device  *dave = display -> gx_display_accelerator;

    /* Set default mode */
    CHECK_DAVE_STATUS(d2_setalphablendmode(dave, d2_bm_one, d2_bm_zero))
    CHECK_DAVE_STATUS(d2_setantialiasing(dave, 0))
}

/*******************************************************************************************************************//**
 * @brief  GUIX display driver for Synergy, 8bpp draw function to complete drawing.
 * @param   display[in]         Pointer to a GUIX display context
 * @param   canvas[in]          Pointer to a GUIX canvas
 **********************************************************************************************************************/
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_drawing_complete_8bpp(GX_DISPLAY  *display, GX_CANVAS *canvas)
{
    /*LDRA_INSPECTED 57 Statement with no side effect. */
    GX_PARAMETER_NOT_USED(display);

    /*LDRA_INSPECTED 57 Statement with no side effect. */
    GX_PARAMETER_NOT_USED(canvas);
}

/*******************************************************************************************************************//**
 * @brief  GUIX display driver for Synergy, 8bpp horizontal line draw function with D/AVE 2D acceleration enabled.
 * This function is called by GUIX to draw a horizontal line.
 * @param   context[in]         Pointer to a GUIX draw context
 * @param   xstart[in]          x axis start position of a horizontal line in pixel unit
 * @param   xend[in]            x axis end position of a horizontal line in pixel unit
 * @param   ypos[in]            y position of a horizontal line
 * @param   width[in]           Line width in pixel unit
 * @param   color[in]           GUIX color data
 **********************************************************************************************************************/
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_horizontal_line_8bpp(GX_DRAW_CONTEXT  *context,
                               INT xstart, INT xend, INT ypos, INT width, GX_COLOR color)
{
    d2_device *dave = gx_dave2d_context_clip_set(context);

    CHECK_DAVE_STATUS(d2_setfillmode(dave, d2_fm_color))
    CHECK_DAVE_STATUS(d2_selectrendermode(dave, d2_rm_solid))
    CHECK_DAVE_STATUS(d2_setalpha(dave, (d2_alpha)color))

    CHECK_DAVE_STATUS(d2_renderbox(dave, (d2_point)(D2_FIX4((USHORT)xstart)),
                                         (d2_point)(D2_FIX4((USHORT)ypos)),
                                         (d2_width)(D2_FIX4((USHORT)(xend - xstart) + 1)),
                                         (d2_width)(D2_FIX4((USHORT)width))))

    /** Count the used display list size. */
    gx_dave2d_display_list_count(context->gx_draw_context_display);
}

/*******************************************************************************************************************//**
 * @brief  GUIX display driver for Synergy, 8bpp vertical line draw function with D/AVE 2D acceleration enabled.
 * This function is called by GUIX to draw a vertical line.
 * @param   context[in]         Pointer to a GUIX draw context
 * @param   ystart[in]          y axis start position of a vertical line in pixel unit
 * @param   yend[in]            y axis end position of a vertical line in pixel unit
 * @param   xpos[in]            x position of a vertical line
 * @param   width[in]           Line width in pixel unit
 * @param   color[in]           GUIX color data
 **********************************************************************************************************************/
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_vertical_line_8bpp(GX_DRAW_CONTEXT  *context,
                             INT ystart, INT yend, INT xpos, INT width, GX_COLOR color)
{

    d2_device * dave = gx_dave2d_context_clip_set(context);
    CHECK_DAVE_STATUS(d2_selectrendermode(dave, d2_rm_solid))
    CHECK_DAVE_STATUS(d2_setfillmode(dave, d2_fm_color))
    CHECK_DAVE_STATUS(d2_setalpha(dave, (d2_alpha)color))
    CHECK_DAVE_STATUS(d2_renderbox(dave, (d2_point)(D2_FIX4((USHORT)xpos)),
                                         (d2_point)(D2_FIX4((USHORT)ystart)),
                                         (d2_width)(D2_FIX4((USHORT)width)),
                                         (d2_width)(D2_FIX4((USHORT)((yend - ystart) + 1)))))

    /** Count the used display list size. */
    gx_dave2d_display_list_count(context->gx_draw_context_display);
}

/*******************************************************************************************************************//**
 * @brief  GUIX display driver for Synergy, 8bpp aliased simple line draw function with D/AVE 2D acceleration
 * enabled. This function is called by GUIX to draw a aliased simple line.
 * @param   context[in]         Pointer to a GUIX draw context
 * @param   xstart[in]          x axis start position of a simple line in pixel unit
 * @param   ystart[in]          y axis start position of a simple line in pixel unit
 * @param   xend[in]            x axis end position of a simple line in pixel unit
 * @param   yend[in]            y axis end position of a simple line in pixel unit
 **********************************************************************************************************************/
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_simple_line_draw_8bpp(GX_DRAW_CONTEXT  *context, INT xstart, INT ystart, INT xend, INT yend)
{
    d2_device *dave = gx_dave2d_context_clip_set(context);
    GX_BRUSH  *brush = &context->gx_draw_context_brush;

    CHECK_DAVE_STATUS(d2_selectrendermode(dave, d2_rm_solid))
    CHECK_DAVE_STATUS(d2_setalpha(dave, (d2_alpha)(brush->gx_brush_line_color)))

    CHECK_DAVE_STATUS(d2_renderline(dave, (d2_point)(D2_FIX4((USHORT)xstart)),
                                          (d2_point)(D2_FIX4((USHORT)ystart)),
                                          (d2_point)(D2_FIX4((USHORT)xend)),
                                          (d2_point)(D2_FIX4((USHORT)yend)),
                                          (d2_width)(D2_FIX4((USHORT)brush->gx_brush_width)),
                                          d2_le_exclude_none))

     /** Count the used display list size. */
     gx_dave2d_display_list_count(context->gx_draw_context_display);
}

/*******************************************************************************************************************//**
 * @brief  GUIX display driver for Synergy, 8bpp aliased wide line draw function with D/AVE 2D acceleration enabled.
 * This function is called by GUIX to draw a draw a aliased wide line.
 * @param   context[in]         Pointer to a GUIX draw context
 * @param   xstart[in]          x axis start position of a wide line in pixel unit
 * @param   ystart[in]          y axis start position of a wide line in pixel unit
 * @param   xend[in]            x axis end position of a wide line in pixel unit
 * @param   yend[in]            y axis end position of a wide line in pixel unit
 **********************************************************************************************************************/
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_simple_wide_line_8bpp(GX_DRAW_CONTEXT  *context, INT xstart, INT ystart,
                                INT xend, INT yend)
{
    d2_device *dave = gx_dave2d_context_clip_set(context);
    GX_BRUSH  *brush = &context->gx_draw_context_brush;

    CHECK_DAVE_STATUS(d2_selectrendermode(dave, d2_rm_solid))
    CHECK_DAVE_STATUS(d2_setalpha(dave, (d2_alpha)(brush->gx_brush_line_color)))

    /** Set the lineend style based on brush style. */
    if(brush->gx_brush_style & GX_BRUSH_ROUND)
    {
        CHECK_DAVE_STATUS(d2_setlinecap(dave, d2_lc_round))
    }
    else
    {
        CHECK_DAVE_STATUS(d2_setlinecap(dave, d2_lc_butt))
    }

    CHECK_DAVE_STATUS(d2_renderline(dave, (d2_point)(D2_FIX4((USHORT)xstart)),
                                          (d2_point)(D2_FIX4((USHORT)ystart)),
                                          (d2_point)(D2_FIX4((USHORT)xend)),
                                          (d2_point)(D2_FIX4((USHORT)yend)),
                                          (d2_width)(D2_FIX4((USHORT)brush->gx_brush_width)),
                                          d2_le_exclude_none))

    /** Count the used display list size. */
    gx_dave2d_display_list_count(context->gx_draw_context_display);
}

/*******************************************************************************************************************//**
 * @brief  GUIX display driver for Synergy, 8bpp horizontal pattern draw function with D/AVE 2D acceleration
 * enabled. This function is called by GUIX to draw a horizontal pattern line for 8bpp format.
 * @param   context[in]         Pointer to a GUIX draw context
 * @param   xstart[in]          x axis start position of a horizontal pattern line in pixel unit
 * @param   xend[in]            x axis end position of a horizontal pattern line in pixel unit
 * @param   ypos[in]            y position of a horizontal pattern line
 **********************************************************************************************************************/
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_horizontal_pattern_line_draw_8bpp(GX_DRAW_CONTEXT  *context, INT xstart, INT xend, INT ypos)
{
    /** Flush D/AVE 2D display list first to insure order of operation. */
    gx_display_list_flush(context->gx_draw_context_display);

    /** Call GUIX 8bpp horizontal pattern line draw function. */
    _gx_display_driver_8bpp_horizontal_pattern_line_draw(context, xstart, xend, ypos);

    /** Open next display list before we go. */
    gx_display_list_open(context->gx_draw_context_display);
}

/*******************************************************************************************************************//**
 * @brief  GUIX display driver for Synergy, 8bpp vertical pattern draw function with D/AVE 2D acceleration enabled.
 * This function is called by GUIX to draw a vertical pattern line for 8bpp format.
 * @param   context[in]         Pointer to a GUIX draw context
 * @param   ystart[in]          y axis start position of a vertical pattern line in pixel unit
 * @param   yend[in]            y axis end position of a vertical pattern line in pixel unit
 * @param   xpos[in]            x position of a vertical pattern line
 **********************************************************************************************************************/
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_vertical_pattern_line_draw_8bpp(GX_DRAW_CONTEXT  *context, INT ystart, INT yend, INT xpos)
{
    /** Flush D/AVE 2D display list first to insure order of operation. */
    gx_display_list_flush(context->gx_draw_context_display);

    /** Call GUIX 8bpp horizontal vertical line draw function. */
    _gx_display_driver_8bpp_vertical_pattern_line_draw(context, ystart, yend, xpos);

    /** Open next display list before we go. */
    gx_display_list_open(context->gx_draw_context_display);
}

/*******************************************************************************************************************//**
 * @brief  GUIX display driver for Synergy, 8bpp pixelmap draw function with D/AVE 2D acceleration enabled.
 * This function is called by GUIX to draw a pixelmap. RLE compression is available as an option.
 * @param   context[in]         Pointer to a GUIX draw context
 * @param   xpos[in]            x axis position of a pixelmap in pixel unit
 * @param   ypos[in]            y axis position of a pixelmap in pixel unit
 * @param   pixelmap[in]        Pointer to a pixelmap
 **********************************************************************************************************************/
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_pixelmap_draw_8bpp(GX_DRAW_CONTEXT  *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
    /** If Pixelmap is transparent. */
    if(pixelmap->gx_pixelmap_flags & GX_PIXELMAP_TRANSPARENT)
    {
        /** Flush D/AVE 2D display list first to insure order of operation. */
        gx_display_list_flush(context->gx_draw_context_display);

        /** Call GUIX 8bpp pixelmap draw function. */
        _gx_display_driver_8bpp_pixelmap_draw(context, xpos, ypos, pixelmap);

        /** Open next display list before we go. */
        gx_display_list_open(context->gx_draw_context_display);

        return;
    }
    d2_u32 mode = d2_mode_alpha8;

    if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
    {
        mode |= d2_mode_rle;
    }

    d2_device *dave = gx_dave2d_context_clip_set(context);

    CHECK_DAVE_STATUS(d2_setalpha(dave, 0xff))

    CHECK_DAVE_STATUS(d2_setblitsrc(dave, (void *)pixelmap -> gx_pixelmap_data,
                                    pixelmap -> gx_pixelmap_width, pixelmap -> gx_pixelmap_width,
                                    pixelmap -> gx_pixelmap_height, mode))

    /*LDRA_INSPECTED 96 S D/AVE 2D uses mixed mode arithmetic for the macro d2_bf_no_blitctxbackup. */
    mode = d2_bf_no_blitctxbackup;

    /*LDRA_INSPECTED 96 S D/AVE 2D uses mixed mode arithmetic for the macro d2_bf_usealpha. */
    mode |= d2_bf_usealpha;

    CHECK_DAVE_STATUS(d2_blitcopy(dave,
                                  pixelmap -> gx_pixelmap_width,
                                  pixelmap -> gx_pixelmap_height,
                                  0,
                                  0,
                                  (d2_point)(D2_FIX4((USHORT)pixelmap -> gx_pixelmap_width)),
                                  (d2_point)(D2_FIX4((USHORT)pixelmap -> gx_pixelmap_height)),
                                  (d2_point)(D2_FIX4((USHORT)xpos)),
                                  (d2_point)(D2_FIX4((USHORT)ypos)),
                                  mode))

    /** Count the used display list size. */
    gx_dave2d_display_list_count(context->gx_draw_context_display);
}

/*******************************************************************************************************************//**
 * @brief  GUIX display driver for Synergy, 8bpp polygon draw function with D/AVE 2D acceleration enabled.
 * This function is called by GUIX to draw a polygon.
 * @param   context[in]         Pointer to a GUIX draw context
 * @param   vertex[in]          Pointer to GUIX point data
 * @param   num[in]             Number of points
 **********************************************************************************************************************/
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_polygon_draw_8bpp(GX_DRAW_CONTEXT  *context, GX_POINT * vertex, INT num)
{
    INT loop;
    INT index = 0;
    GX_VALUE val;
    d2_point data[MAX_POLYGON_VERTICES * 2] = {0};
    GX_BRUSH *brush = &context->gx_draw_context_brush;

    /** Return to caller if brush width is 0. */
    if (brush->gx_brush_width < 1)
    {
        return;
    }

    /** Convert incoming point data to d2_point type. */
    for (loop = 0; loop < num; loop++)
    {
        val = vertex[loop].gx_point_x;
        data[index++] = (d2_point)(D2_FIX4((USHORT)val));
        val = vertex[loop].gx_point_y;
        data[index++] = (d2_point)(D2_FIX4((USHORT)val));
    }

    d2_device *dave = gx_dave2d_context_clip_set(context);

    CHECK_DAVE_STATUS(d2_selectrendermode(dave, d2_rm_outline))
    CHECK_DAVE_STATUS(d2_setfillmode(dave, d2_fm_color))
    CHECK_DAVE_STATUS(d2_outlinewidth(dave, (d2_width)(D2_FIX4((USHORT)brush->gx_brush_width))))
    CHECK_DAVE_STATUS(d2_setalpha(dave, (d2_alpha)brush->gx_brush_line_color))

    if (brush->gx_brush_style & GX_BRUSH_ROUND)
    {
        CHECK_DAVE_STATUS(d2_setlinejoin(dave, d2_lj_round))
    }
    else
    {
        CHECK_DAVE_STATUS(d2_setlinejoin(dave, d2_lj_miter))
    }

    CHECK_DAVE_STATUS(d2_renderpolygon(dave, (d2_point *)data, (d2_u32)num, 0))

    /** Count the used display list size. */
    gx_dave2d_display_list_count(context->gx_draw_context_display);
}

/*******************************************************************************************************************//**
 * @brief  GUIX display driver for Synergy, 8bpp polygon fill function with D/AVE 2D acceleration enabled.
 * This function is called by GUIX to fill a polygon.
 * @param   context[in]         Pointer to a GUIX draw context
 * @param   vertex[in]          Pointer to GUIX point data
 * @param   num[in]             Number of points
 **********************************************************************************************************************/
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_polygon_fill_8bpp(GX_DRAW_CONTEXT  *context, GX_POINT * vertex, INT num)
{
    /** Check if polygon to be render is convex polygon. */
    if(!gx_dave2d_convex_polygon_test(vertex, num))
    {
        /** Flush D/AVE 2D display list first to insure order of operation. */
        gx_display_list_flush(context->gx_draw_context_display);

        /** Open next display list before we go. */
        gx_display_list_open(context->gx_draw_context_display);

        _gx_display_driver_generic_polygon_fill(context, vertex, num);

        return;
    }

    INT loop;
    INT index = 0;
    GX_VALUE val;
    d2_point data[MAX_POLYGON_VERTICES * 2] = { 0 };
    GX_BRUSH * brush = &context->gx_draw_context_brush;

    /** Convert incoming point data to d2_point type. */
    for (loop = 0; loop < num; loop++)
    {
        val = vertex[loop].gx_point_x;
        data[index++] = (d2_point)(D2_FIX4((USHORT)val));
        val = vertex[loop].gx_point_y;
        data[index++] = (d2_point)(D2_FIX4((USHORT)val));
    }

    d2_device *dave = gx_dave2d_context_clip_set(context);

    CHECK_DAVE_STATUS(d2_selectrendermode(dave, d2_rm_solid))

    if (brush->gx_brush_style & GX_BRUSH_PIXELMAP_FILL)
    {
        CHECK_DAVE_STATUS(d2_setfillmode(dave, d2_fm_texture))
        CHECK_DAVE_STATUS(d2_setalpha(dave, 0xff))
        gx_dave2d_set_texture_8bpp(context,
                                   dave,
                                   context->gx_draw_context_clip->gx_rectangle_left,
                                   context->gx_draw_context_clip->gx_rectangle_top,
                                   brush->gx_brush_pixelmap);
    }
    else
    {
        CHECK_DAVE_STATUS(d2_setfillmode(dave, d2_fm_color))
        CHECK_DAVE_STATUS(d2_setalpha(dave, (d2_alpha)brush->gx_brush_fill_color))
    }

    CHECK_DAVE_STATUS(d2_renderpolygon(dave, (d2_point *)data, (d2_u32)num, 0))

    /** Count the used display list size. */
    gx_dave2d_display_list_count(context->gx_draw_context_display);
}

/*******************************************************************************************************************//**
 * @brief  GUIX display driver for Synergy, 8bpp pixel write function with D/AVE 2D acceleration enabled.
 * This function is called by GUIX to write a pixel. Although D/AVE 2D acceleration enabled, the GUIX generic pixel
 * write routine is used since there is no performance benefit for single pixel write operation.
 * @param   context[in]         Pointer to a GUIX draw context
 * @param   x[in]               x position in pixel unit
 * @param   y[in]               y position in pixel unit
 * @param   color[in]           GUIX color data
 **********************************************************************************************************************/
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_pixel_write_8bpp(GX_DRAW_CONTEXT  *context, INT x, INT y, GX_COLOR color)
{
    /** Flush D/AVE 2D display list first to insure order of operation. */
    gx_display_list_flush(context -> gx_draw_context_display);

    /** Call the GUIX generic 8bpp pixel write routine. */
    _gx_display_driver_8bpp_pixel_write(context, x, y, color);

    /** Open next display list before we go. */
    gx_display_list_open(context -> gx_draw_context_display);
}

/*******************************************************************************************************************//**
 * @brief  GUIX display driver for Synergy, 8bpp block move function with D/AVE 2D acceleration enabled.
 * This function is called by GUIX to move a block of pixels within a working canvas memory. Mainly used for fast
 * scrolling.
 * @param   context[in]         Pointer to a GUIX draw context
 * @param   block[in]           Pointer to a block to be moved
 * @param   xshift[in]          x axis shift in pixel unit
 * @param   yshift[in]          y axis shift in pixel unit
 **********************************************************************************************************************/
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_block_move_8bpp(GX_DRAW_CONTEXT  *context, GX_RECTANGLE * block, INT xshift, INT yshift)
{
    /** Flush D/AVE 2D display list first to insure order of operation. */
    gx_display_list_flush(context -> gx_draw_context_display);

    /** Call the GUIX generic 8bpp block move routine. */
    _gx_display_driver_8bpp_block_move(context, block, xshift, yshift);

    /** Open next display list before we go. */
    gx_display_list_open(context -> gx_draw_context_display);
}

/*******************************************************************************************************************//**
 * @brief  GUIX display driver for Synergy, 8bpp 1-bit raw glyph draw function with D/AVE 2D acceleration enabled.
 * This function is called by GUIX to draw 1-bit raw glyph.
 * @param   context[in]        Pointer to a GUIX draw context
 * @param   draw_area[in]      Pointer to the draw rectangle area
 * @param   map_offset[in]     Mapping offset
 * @param   glyph[in]          Pointer to glyph data
 **********************************************************************************************************************/
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_glyph_1bit_draw_8bpp(GX_DRAW_CONTEXT  *context, GX_RECTANGLE * draw_area, GX_POINT * map_offset,
                                                                                                const GX_GLYPH * glyph)
{
    /** Flush D/AVE 2D display list first to insure order of operation. */
    gx_display_list_flush(context -> gx_draw_context_display);

    /** Call the GUIX generic 8bpp glyph 1bit draw routine. */
    _gx_display_driver_8bpp_glyph_1bit_draw(context, draw_area, map_offset, glyph);

    /** Open next display list before we go. */
    gx_display_list_open(context -> gx_draw_context_display);
}

/*******************************************************************************************************************//**
 * @brief  GUIX display driver for Synergy, 8bpp 3-bit raw glyph draw function with D/AVE 2D acceleration enabled.
 * This function is called by GUIX to draw 3-bit raw glyph.
 * @param   context[in]        Pointer to a GUIX draw context
 * @param   draw_area[in]      Pointer to the draw rectangle area
 * @param   map_offset[in]     Mapping offset
 * @param   glyph[in]          Pointer to glyph data
 **********************************************************************************************************************/
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_glyph_3bit_draw_8bpp(GX_DRAW_CONTEXT  *context, GX_RECTANGLE * draw_area, GX_POINT * map_offset,
                                                                                                const GX_GLYPH * glyph)
{
    /** Flush D/AVE 2D display list first to insure order of operation. */
    gx_display_list_flush(context -> gx_draw_context_display);

    /** Call the GUIX generic 8bpp glyph 4bit draw routine. */
    _gx_display_driver_8bpp_glyph_3bit_draw(context, draw_area, map_offset, glyph);

    /** Open next display list before we go. */
    gx_display_list_open(context -> gx_draw_context_display);
}


/*******************************************************************************************************************//**
 * @brief  GUIX display driver for Synergy, 8bpp 4-bit raw glyph draw function with D/AVE 2D acceleration enabled.
 * This function is called by GUIX to draw 4-bit raw glyph.
 * @param   context[in]        Pointer to a GUIX draw context
 * @param   draw_area[in]      Pointer to the draw rectangle area
 * @param   map_offset[in]     Mapping offset
 * @param   glyph[in]          Pointer to glyph data
 **********************************************************************************************************************/
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_glyph_4bit_draw_8bpp(GX_DRAW_CONTEXT  *context, GX_RECTANGLE * draw_area, GX_POINT * map_offset,
                                                                                                const GX_GLYPH * glyph)
{
    /** Flush D/AVE 2D display list first to insure order of operation. */
    gx_display_list_flush(context -> gx_draw_context_display);

    /** Call the GUIX generic 8bpp glyph 4bit draw routine. */
    _gx_display_driver_8bpp_glyph_4bit_draw(context, draw_area, map_offset, glyph);

    /** Open next display list before we go. */
    gx_display_list_open(context -> gx_draw_context_display);
}

#if defined(GX_ARC_DRAWING_SUPPORT)
/*******************************************************************************************************************//**
 * @brief  GUIX display driver for Synergy, 8bpp circle outline draw function with D/AVE 2D
 * acceleration enabled. This function is called by GUIX to render a circle outline.
 * @param   context[in]        Pointer to a GUIX draw context
 * @param   xcenter[in]        Center pixel position in the horizontal axis
 * @param   ycenter[in]        Center pixel position in the vertical axis
 * @param   r[in]              Radius in pixel unit
 **********************************************************************************************************************/
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_circle_draw_8bpp(GX_DRAW_CONTEXT  *context, INT xcenter, INT ycenter, UINT r)
{
    GX_BRUSH * brush = &context->gx_draw_context_brush;

    /** Return to caller if brush width is 0. */
    if (brush->gx_brush_width < 1)
    {
        return;
    }

    if (r < (UINT)((brush->gx_brush_width + 1) / 2))
    {
        r = 0U;
    }
    else
    {
        r = (UINT)(r - (UINT)((brush->gx_brush_width + 1) / 2));
    }

    d2_device *dave = gx_dave2d_context_clip_set(context);

    /** Make sure anti-aliasing is off. */
    CHECK_DAVE_STATUS(d2_selectrendermode(dave, d2_rm_outline))
    CHECK_DAVE_STATUS(d2_outlinewidth(dave, (d2_width)(D2_FIX4((USHORT)brush->gx_brush_width))))
    CHECK_DAVE_STATUS(d2_setfillmode(dave, d2_fm_color))
    CHECK_DAVE_STATUS(d2_setalpha(dave, (d2_alpha)(brush->gx_brush_line_color)))

    CHECK_DAVE_STATUS(d2_rendercircle(dave, (d2_point)(D2_FIX4((USHORT)xcenter)),
                                            (d2_point)(D2_FIX4((USHORT)ycenter)),
                                            (d2_width)(D2_FIX4((USHORT)r)),
                                            0))

    /** Count the used display list size. */
    gx_dave2d_display_list_count(context->gx_draw_context_display);
}

/*******************************************************************************************************************//**
 * @brief  GUIX display driver for Synergy, 8bpp circle fill function with D/AVE 2D acceleration enabled.
 * This function is called by GUIX to fill circle.
 * @param   context[in]        Pointer to a GUIX draw context
 * @param   xcenter[in]        Center pixel position in the horizontal axis
 * @param   ycenter[in]        Center pixel position in the vertical axis
 * @param   r[in]              Radius in pixel unit
 **********************************************************************************************************************/
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_circle_fill_8bpp(GX_DRAW_CONTEXT  *context, INT xcenter, INT ycenter, UINT r)
{
    GX_BRUSH  *brush = &context->gx_draw_context_brush;
    d2_device *dave = gx_dave2d_context_clip_set(context);

    CHECK_DAVE_STATUS(d2_selectrendermode(dave, d2_rm_solid))

    if (brush->gx_brush_style & GX_BRUSH_PIXELMAP_FILL)
    {
        CHECK_DAVE_STATUS(d2_setfillmode(dave, d2_fm_texture))
        CHECK_DAVE_STATUS(d2_setalpha(dave, GX_ALPHA_VALUE_OPAQUE))

        gx_dave2d_set_texture_8bpp(context,
                                   dave,
                                   xcenter - (INT)r,
                                   ycenter - (INT)r,
                                   brush->gx_brush_pixelmap);
    }
    else
    {
        CHECK_DAVE_STATUS(d2_setfillmode(dave, d2_fm_color))
        CHECK_DAVE_STATUS(d2_setalpha(dave, (d2_alpha)brush->gx_brush_fill_color))
    }

    CHECK_DAVE_STATUS(d2_rendercircle(dave, (d2_point)(D2_FIX4((USHORT)xcenter)),
                                            (d2_point)(D2_FIX4((USHORT)ycenter)),
                                            (d2_width)(D2_FIX4((USHORT)r)),
                                            0))

    /** Count the used display list size. */
    gx_dave2d_display_list_count(context->gx_draw_context_display);
}

/*******************************************************************************************************************//**
 * @brief  GUIX display driver for Synergy, 8bpp arc draw function with D/AVE 2D acceleration enabled.
 * This function is called by GUIX to draw arc.
 * @param   context[in]        Pointer to a GUIX draw context
 * @param   xcenter[in]        Center pixel position in the horizontal axis
 * @param   ycenter[in]        Center pixel position in the vertical axis
 * @param   r[in]              Radius in pixel unit
 * @param   start_angle[in]    Start angle in degree
 * @param   end_angle[in]      End angle in degree
 **********************************************************************************************************************/
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_arc_draw_8bpp(GX_DRAW_CONTEXT  *context, INT xcenter, INT ycenter, UINT r, INT start_angle,
                                                                                                        INT end_angle)
{
    GX_BRUSH  * brush = &context->gx_draw_context_brush;
    INT         sin1;
    INT         cos1;
    INT         sin2;
    INT         cos2;
    d2_u32      flags;

    if(brush->gx_brush_width < 1)
    {
        return;
    }

    INT s_angle;
    INT e_angle;

    s_angle=  - start_angle;
    e_angle=  - end_angle;

    sin1 = (INT) _gx_utility_math_sin((GX_FIXED_VAL)((UINT) (s_angle - 90) << GX_FIXED_VAL_SHIFT));
    cos1 = (INT) _gx_utility_math_cos((GX_FIXED_VAL)((UINT) (s_angle - 90) << GX_FIXED_VAL_SHIFT));

    sin2 = (INT) _gx_utility_math_sin((GX_FIXED_VAL)((UINT) (e_angle + 90) << GX_FIXED_VAL_SHIFT));
    cos2 = (INT) _gx_utility_math_cos((GX_FIXED_VAL)((UINT) (e_angle + 90) << GX_FIXED_VAL_SHIFT));

    /** Set d2_wf_concave flag if the pie object to draw is concave shape. */
    if (((s_angle - e_angle) > 180) || ((s_angle - e_angle) < 0))
    {
        /*LDRA_INSPECTED 96 S D/AVE 2D uses mixed mode arithmetic for the macro d2_wf_concave. */
        flags = d2_wf_concave;
    }
    else
    {
        flags = 0;
    }

    d2_device *dave = gx_dave2d_context_clip_set(context);

    CHECK_DAVE_STATUS(d2_selectrendermode(dave, d2_rm_outline))
    CHECK_DAVE_STATUS(d2_outlinewidth(dave, (d2_width)((D2_FIX4((USHORT)brush->gx_brush_width)) >> 1)))
    CHECK_DAVE_STATUS(d2_setfillmode(dave, d2_fm_color))
    CHECK_DAVE_STATUS(d2_setalpha(dave, (d2_alpha)(brush->gx_brush_line_color)))

    CHECK_DAVE_STATUS(d2_renderwedge(dave, (d2_point)(D2_FIX4((USHORT)xcenter)),
                                           (d2_point)(D2_FIX4((USHORT)ycenter)),
                                           (d2_width)(D2_FIX4((USHORT)r)),
                                           0,
                                           (d2_s32)((UINT)cos1 << 6),
                                           (d2_s32)((UINT)sin1 << 6),
                                           (d2_s32)((UINT)cos2 << 6),
                                           (d2_s32)((UINT)sin2 << 6),
                                           flags))

    /** Count the used display list size. */
    gx_dave2d_display_list_count(context->gx_draw_context_display);

    /** Check if line connection style is circle segment. */
    if (brush -> gx_brush_style & GX_BRUSH_ROUND)
    {
        UINT brush_width = (UINT)brush->gx_brush_width;
        GX_POINT  startp = {0};
        GX_POINT  endp = {0};

        /** Set a rendering mode to solid. */
        CHECK_DAVE_STATUS(d2_selectrendermode(dave, d2_rm_solid))

        r = (UINT)(r - (UINT)(brush_width >> 1));

        /** Get the point on circle with specified angle and radius. */
        _gx_utility_circle_point_get(xcenter, ycenter, r, start_angle, &startp);
        _gx_utility_circle_point_get(xcenter, ycenter, r + (UINT)brush_width, start_angle, &endp);

        /** Render a circle. */
        CHECK_DAVE_STATUS(d2_rendercircle(dave,
                                          (d2_point)(D2_FIX4((USHORT) (startp.gx_point_x + endp.gx_point_x)) >> 1),
                                          (d2_point)(D2_FIX4((USHORT) (startp.gx_point_y + endp.gx_point_y)) >> 1),
                                          (d2_width)(D2_FIX4(brush_width) >> 1),
                                          0))

        /** Count the used display list size. */
        gx_dave2d_display_list_count(context->gx_draw_context_display);

        /** Get the point on circle with specified angle and radius. */
        _gx_utility_circle_point_get(xcenter, ycenter, r, end_angle, &startp);
        _gx_utility_circle_point_get(xcenter, ycenter, r + (UINT)brush_width, end_angle, &endp);

        /** Render a circle. */
        CHECK_DAVE_STATUS(d2_rendercircle(dave,
                                          (d2_point)(D2_FIX4((USHORT) (startp.gx_point_x + endp.gx_point_x)) >> 1),
                                          (d2_point)(D2_FIX4((USHORT) (startp.gx_point_y + endp.gx_point_y)) >> 1),
                                          (d2_width)(D2_FIX4(brush_width) >> 1),
                                          0))

        /** Count the used display list size. */
        gx_dave2d_display_list_count(context->gx_draw_context_display);
    }
}

/*******************************************************************************************************************//**
 * @brief  GUIX display driver for Synergy, 8bpp arc fill function with D/AVE 2D acceleration enabled.
 * This function is called by GUIX to fill arc. Although D/AVE 2D acceleration enabled, arc fill is done by GUIX generic
 * software draw routine because chord (filled arc) operation is not supported by D/AVE 2D.
 * @param   context[in]        Pointer to a GUIX draw context
 * @param   xcenter[in]        Center pixel position in the horizontal axis
 * @param   ycenter[in]        Center pixel position in the vertical axis
 * @param   r[in]              Radius in pixel unit
 * @param   start_angle[in]    Start angle in degree
 * @param   end_angle[in]      End angle in degree
 **********************************************************************************************************************/
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_arc_fill_8bpp(GX_DRAW_CONTEXT  *context, INT xcenter, INT ycenter, UINT r, INT start_angle, INT end_angle)
{
    /** Flush D/AVE 2D display list first to insure order of operation. */
    gx_display_list_flush(context->gx_draw_context_display);

    /** Open next display list before we go. */
    gx_display_list_open(context->gx_draw_context_display);

    /** Call the GUIX generic arc fill routine. */
    _gx_display_driver_generic_arc_fill(context, xcenter, ycenter, r, start_angle, end_angle);
}

/*******************************************************************************************************************//**
 * @brief  GUIX display driver for Synergy, 8bpp pie fill function with D/AVE 2D acceleration enabled.
 * This function is called by GUIX to fill pie.
 * @param   context[in]        Pointer to a GUIX draw context
 * @param   xcenter[in]        Center pixel position in the horizontal axis
 * @param   ycenter[in]        Center pixel position in the vertical axis
 * @param   r[in]              Radius in pixel unit
 * @param   start_angle[in]    Start angle in degree
 * @param   end_angle[in]      End angle in degree
 **********************************************************************************************************************/
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_pie_fill_8bpp(GX_DRAW_CONTEXT  *context, INT xcenter, INT ycenter, UINT r,
                              INT start_angle, INT end_angle)
{
    GX_BRUSH  * brush = &context->gx_draw_context_brush;
    INT         sin1;
    INT         cos1;
    INT         sin2;
    INT         cos2;
    d2_u32      flags;
    GX_COLOR    brush_color = brush->gx_brush_fill_color;
    d2_device * dave = gx_dave2d_context_clip_set(context);

    INT s_angle =  - start_angle;
    INT e_angle =  - end_angle;

    sin1 = (INT) _gx_utility_math_sin((GX_FIXED_VAL)((UINT) (s_angle - 90) << GX_FIXED_VAL_SHIFT));
    cos1 = (INT) _gx_utility_math_cos((GX_FIXED_VAL)((UINT) (s_angle - 90) << GX_FIXED_VAL_SHIFT));

    sin2 = (INT) _gx_utility_math_sin((GX_FIXED_VAL)((UINT) (e_angle + 90) << GX_FIXED_VAL_SHIFT));
    cos2 = (INT) _gx_utility_math_cos((GX_FIXED_VAL)((UINT) (e_angle + 90) << GX_FIXED_VAL_SHIFT));

    /** Set d2_wf_concave flag if the pie object to draw is concave shape. */
    if (((s_angle - e_angle) > 180) || ((s_angle - e_angle) < 0))
    {
        /*LDRA_INSPECTED 96 S D/AVE 2D uses mixed mode arithmetic for the macro d2_wf_concave. */
        flags = d2_wf_concave;
    }
    else
    {
        flags = 0;
    }

    CHECK_DAVE_STATUS(d2_selectrendermode(dave, d2_rm_solid))

    if (brush->gx_brush_style & GX_BRUSH_PIXELMAP_FILL)
    {
        CHECK_DAVE_STATUS(d2_setfillmode(dave, d2_fm_texture))
        CHECK_DAVE_STATUS(d2_setalpha(dave, GX_ALPHA_VALUE_OPAQUE))
        gx_dave2d_set_texture_8bpp(context,
                                   dave,
                                   xcenter - (INT)r,
                                   ycenter - (INT)r,
                                   brush->gx_brush_pixelmap);
    }
    else
    {
        CHECK_DAVE_STATUS(d2_setfillmode(dave, d2_fm_color))
        CHECK_DAVE_STATUS(d2_setalpha(dave, (d2_alpha)(brush_color)))
    }

    CHECK_DAVE_STATUS(d2_renderwedge(dave, (d2_point)(D2_FIX4((USHORT)xcenter)),
                                           (d2_point)(D2_FIX4((USHORT)ycenter)),
                                           (d2_point)(D2_FIX4((USHORT)(r + 1))),
                                           0,
                                           (d2_s32)((UINT)cos1 << 6),
                                           (d2_s32)((UINT)sin1 << 6),
                                           (d2_s32)((UINT)cos2 << 6),
                                           (d2_s32)((UINT)sin2 << 6),
                                           flags))

    /** Count the used display list size. */
    gx_dave2d_display_list_count(context->gx_draw_context_display);
}

/*******************************************************************************************************************//**
 * @brief  GUIX display driver for Synergy, 8bpp ellipse draw function with D/AVE 2D acceleration enabled.
 * This function is called by GUIX to draw aliased ellipse. Although D/AVE 2D acceleration enabled, ellipse draw is
 * done by GUIX generic software draw routine because the operation is not supported by D/AVE 2D.
 * @param   context[in]        Pointer to a GUIX draw context
 * @param   xcenter[in]        Center pixel position in the horizontal axis
 * @param   ycenter[in]        Center pixel position in the vertical axis
 * @param   a[in]              Semi-major axis
 * @param   b[in]              Semi-minor axis
 **********************************************************************************************************************/
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_ellipse_draw_8bpp(GX_DRAW_CONTEXT  *context, INT xcenter, INT ycenter, INT a, INT b)
{
    /** Flush D/AVE 2D display list first to insure order of operation. */
    gx_display_list_flush(context -> gx_draw_context_display);

    /** Call the GUIX generic ellipse draw routine. */
    _gx_display_driver_generic_ellipse_draw(context, xcenter, ycenter, a, b);

    /** Open next display list before we go. */
    gx_display_list_open(context -> gx_draw_context_display);
}

/*******************************************************************************************************************//**
 * @brief  GUIX display driver for Synergy, 8bpp wide ellipse draw function with D/AVE 2D acceleration enabled.
 * This function is called by GUIX to draw aliased ellipse. Although D/AVE 2D acceleration enabled, ellipse draw is
 * done by GUIX generic software draw routine because the operation is not supported by D/AVE 2D.
 * @param   context[in]        Pointer to a GUIX draw context
 * @param   xcenter[in]        Center pixel position in the horizontal axis
 * @param   ycenter[in]        Center pixel position in the vertical axis
 * @param   a[in]              Semi-major axis
 * @param   b[in]              Semi-minor axis
 **********************************************************************************************************************/
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_wide_ellipse_draw_8bpp(GX_DRAW_CONTEXT  *context, INT xcenter, INT ycenter, INT a, INT b)
{
    /** Flush D/AVE 2D display list first to insure order of operation. */
    gx_display_list_flush(context -> gx_draw_context_display);

    /** Call the GUIX generic wide ellipse draw routine. */
    _gx_display_driver_generic_wide_ellipse_draw(context, xcenter, ycenter, a, b);

    /** Open next display list before we go. */
    gx_display_list_open(context -> gx_draw_context_display);
}

/*******************************************************************************************************************//**
 * @brief  GUIX display driver for Synergy, 8bpp ellipse fill function with D/AVE 2D acceleration enabled.
 * This function is called by GUIX to perform ellipse fill. Although D/AVE 2D acceleration enabled, ellipse fill is done
 * by GUIX generic software draw routine.
 * @param   context[in]        Pointer to a GUIX draw context
 * @param   xcenter[in]        Center pixel position in the horizontal axis
 * @param   ycenter[in]        Center pixel position in the vertical axis
 * @param   a[in]              Semi-major axis
 * @param   b[in]              Semi-minor axis
 **********************************************************************************************************************/
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_ellipse_fill_8bpp(GX_DRAW_CONTEXT  *context, INT xcenter, INT ycenter, INT a, INT b)
{
    /** Flush D/AVE 2D display list first to insure order of operation. */
    gx_display_list_flush(context -> gx_draw_context_display);

    /** Open next display list before we go. */
    gx_display_list_open(context -> gx_draw_context_display);

    /** Call the GUIX generic ellipse fill routine. */
    _gx_display_driver_generic_ellipse_fill(context, xcenter, ycenter, a, b);
}
#endif  /* is GUIX arc drawing support enabled? */

/*******************************************************************************************************************//**
 * @brief  GUIX display driver for Synergy, D/AVE 2D accelerated 8bpp draw function to toggle frame buffer.
 *  This function performs copies canvas memory to working frame buffer if a canvas is used, performs sequence of canvas
 *  refresh drawing commands, toggles frame buffer, and finally copies visible frame buffer to working frame buffer or
 *  copes canvas to working buffer if a canvas is used. This function is called by GUIX if D/AVE 2D hardware rendering
 *  acceleration is enabled.
 * @param   canvas[in]         Pointer to a GUIX canvas
 * @param   dirty[in]          Pointer to a dirty rectangle area
 **********************************************************************************************************************/
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_dave2d_buffer_toggle_8bpp (GX_CANVAS *canvas, GX_RECTANGLE *dirty)
{
    /*LDRA_INSPECTED 57 Statement with no side effect. */
    GX_PARAMETER_NOT_USED(dirty);

    GX_RECTANGLE    Limit;
    GX_RECTANGLE    Copy;
    GX_DISPLAY     *display;
    INT             rotation_angle;

    display = canvas->gx_canvas_display;
    rotation_angle = display->gx_display_rotation_angle;
    sf_el_gx_frame_pointers_get(display->gx_display_handle, &visible_frame, &working_frame);

    _gx_utility_rectangle_define(&Limit, 0, 0,
                                 (GX_VALUE)(canvas->gx_canvas_x_resolution - 1),
                                 (GX_VALUE)(canvas->gx_canvas_y_resolution - 1));

    gx_display_list_flush(display);
    gx_display_list_open(display);

    if (rotation_angle == 180)
    {
        if (_gx_utility_rectangle_overlap_detect(&Limit, &canvas->gx_canvas_dirty_area, &Copy))
        {
            gx_flip_canvas_to_working_8bpp(canvas, &Copy);
        }
    }

    sf_el_gx_frame_toggle(canvas->gx_canvas_display->gx_display_handle, (GX_UBYTE **) &visible_frame);
    sf_el_gx_frame_pointers_get(canvas->gx_canvas_display->gx_display_handle, &visible_frame, &working_frame);

    /* If canvas memory is pointing directly to frame buffer, toggle canvas memory */
    if (canvas->gx_canvas_memory == (GX_COLOR *) visible_frame)
    {
        canvas->gx_canvas_memory = (GX_COLOR *) working_frame;
    }

    if (_gx_utility_rectangle_overlap_detect(&Limit, &canvas->gx_canvas_dirty_area, &Copy))
    {
        if (rotation_angle == 0)
        {
            /* Copies our canvas to the back buffer */
            gx_dave2d_copy_visible_to_working_8bpp(canvas, &Copy);
        }
        else
        {
            gx_flip_canvas_to_working_8bpp(canvas, &Copy);
        }
    }
}
#endif /* GX_USE_SYNERGY_DRW */

/*******************************************************************************************************************//**
 * @brief  GUIX display driver for Synergy, 8-bit Palette setup for display hardware.
 * This function is called by GUIX.
 * @param[in]    display       Pointer to a GUIX display
 * @param[in]    palette       Pointer to a GUIX color palette entries
 * @param[in]    count         Number of palette entries.
 **********************************************************************************************************************/
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_display_driver_8bit_palette_assign(GX_DISPLAY *display, GX_COLOR *palette, INT count)
{
    display -> gx_display_palette = palette;
    display -> gx_display_palette_size = (UINT)count;

    sf_el_gx_display_8bit_palette_assign(display->gx_display_handle);
}

/*******************************************************************************************************************//**
 * @brief  GUIX display driver for Synergy, Software 8bpp draw function to toggle frame buffer.
 *  This function performs copies canvas memory to working frame buffer if a canvas is used, then toggles frame buffer,
 *  finally copies visible frame buffer to working frame buffer. This function is called by GUIX if hardware rendering
 *  acceleration is not enabled.
 * @param[in]   canvas         Pointer to a GUIX canvas
 * @param[in]   dirty          Pointer to a dirty rectangle area
 **********************************************************************************************************************/
/*LDRA_INSPECTED 219 S GUIX defined functions start with underscore. */
VOID _gx_synergy_buffer_toggle_8bpp(GX_CANVAS *canvas, GX_RECTANGLE *dirty)
{
    /*LDRA_INSPECTED 57 Statement with no side effect. */
    GX_PARAMETER_NOT_USED(dirty);

    GX_RECTANGLE Limit;
    GX_RECTANGLE copy;
    GX_DISPLAY *display;
    INT rotation_angle;

    display = canvas->gx_canvas_display;

    _gx_utility_rectangle_define(&Limit, 0, 0,
                                 (GX_VALUE)(canvas->gx_canvas_x_resolution - 1),
                                 (GX_VALUE)(canvas->gx_canvas_y_resolution - 1));

    rotation_angle = display->gx_display_rotation_angle;
    sf_el_gx_frame_pointers_get(display->gx_display_handle, &visible_frame, &working_frame);

    if(rotation_angle != 0)
    {
        if (_gx_utility_rectangle_overlap_detect(&Limit, &canvas->gx_canvas_dirty_area, &copy))
        {
            gx_flip_canvas_to_working_8bpp(canvas, &copy);
        }
    }

    sf_el_gx_frame_toggle(canvas->gx_canvas_display->gx_display_handle, &visible_frame);
    sf_el_gx_frame_pointers_get(canvas->gx_canvas_display->gx_display_handle, &visible_frame, &working_frame);

    if (canvas->gx_canvas_memory == (GX_COLOR *) visible_frame)
    {
        canvas->gx_canvas_memory = (GX_COLOR *) working_frame;
    }

    if (_gx_utility_rectangle_overlap_detect(&Limit, &canvas->gx_canvas_dirty_area, &copy))
    {
        if(rotation_angle == 0)
        {
            gx_copy_visible_to_working_8bpp(canvas, &copy);
        }
        else
        {
            gx_flip_canvas_to_working_8bpp(canvas, &copy);
        }
    }
}


#if (GX_USE_SYNERGY_DRW == 1)
/*******************************************************************************************************************//**
 * @brief  GUIX display driver for Synergy, support function used to apply texture source for all shape drawing.
 * @param   context[in]         Pointer to a GUIX draw context
 * @param   dave[in]            Pointer to D/AVE 2D device context
 * @param   xpos[in]            X position in pixel unit
 * @param   ypos[in]            y position in pixel unit
 * @param   map[in]             Pointer to GUIX pixelmap
 **********************************************************************************************************************/
static VOID gx_dave2d_set_texture_8bpp(GX_DRAW_CONTEXT  *context, d2_device *dave, INT xpos, INT ypos,
                                                                                                    GX_PIXELMAP *map)
{
    /*LDRA_INSPECTED 57 Statement with no side effect. */
    GX_PARAMETER_NOT_USED(context);

    d2_u32 format = d2_mode_alpha8;

    if (map->gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
    {
        format |= d2_mode_rle;
    }

    CHECK_DAVE_STATUS(d2_settexture(dave,
            (void *) map->gx_pixelmap_data,
            map->gx_pixelmap_width,
            map->gx_pixelmap_width,
            map->gx_pixelmap_height,
            format))

    /** Set texture mode. */
    CHECK_DAVE_STATUS(d2_settexturemode(dave, d2_tm_wrapu|d2_tm_wrapv))
    CHECK_DAVE_STATUS(d2_settextureoperation(dave, d2_to_copy, d2_to_zero, d2_to_zero, d2_to_zero));
    CHECK_DAVE_STATUS(d2_settexelcenter(dave, 0, 0))
    CHECK_DAVE_STATUS(d2_settexturemapping(dave,
                                            (d2_point)((UINT)xpos << 4),
                                            (d2_point)((UINT)ypos << 4),
                                            0,
                                            0,
                                            (d2_s32)(1U << 16),
                                            0,
                                            0,
                                            (d2_s32)(1U << 16)))
}



/*******************************************************************************************************************//**
 * @brief  GUIX display driver for Synergy, D/AVE 2D 8bpp draw function sub routine to copy visible frame buffer to
 * working frame buffer. This function is called by _gx_dave2d_buffer_toggle_8bpp to perform image data copy between
 * frame buffers after buffer toggle operation.
 * @param   canvas[in]         Pointer to a GUIX canvas
 * @param   copy[in]           Pointer to a rectangle area to be copied
 **********************************************************************************************************************/
static VOID gx_dave2d_copy_visible_to_working_8bpp(GX_CANVAS *canvas, GX_RECTANGLE *copy)
{
    GX_RECTANGLE display_size;
    GX_RECTANGLE copy_clip;

    ULONG      * pGetRow;
    ULONG      * pPutRow;

    INT          copy_width;
    INT          copy_height;

    GX_DISPLAY  *display = canvas->gx_canvas_display;
    d2_device * dave = (d2_device *) display->gx_display_accelerator;

    _gx_utility_rectangle_define(&display_size, 0, 0,
            (GX_VALUE)(display->gx_display_width - 1),
            (GX_VALUE)(display->gx_display_height - 1));
    copy_clip = *copy;

    copy_clip.gx_rectangle_left   = (GX_VALUE)((USHORT)copy_clip.gx_rectangle_left & 0xfffcU);
    copy_clip.gx_rectangle_top    = (GX_VALUE)((USHORT)copy_clip.gx_rectangle_top  & 0xfffcU);
    copy_clip.gx_rectangle_right  = (GX_VALUE)((USHORT)copy_clip.gx_rectangle_right  | 3U);
    copy_clip.gx_rectangle_bottom = (GX_VALUE)((USHORT)copy_clip.gx_rectangle_bottom | 3U);

    /** Offset canvas within frame buffer. */
    _gx_utility_rectangle_shift(&copy_clip, canvas->gx_canvas_display_offset_x, canvas->gx_canvas_display_offset_y);

    _gx_utility_rectangle_overlap_detect(&copy_clip, &display_size, &copy_clip);
    copy_width  = (copy_clip.gx_rectangle_right - copy_clip.gx_rectangle_left) + 1;
    copy_height = (copy_clip.gx_rectangle_bottom - copy_clip.gx_rectangle_top) + 1;

    if ((copy_width <= 0) || (copy_height <= 0))
    {
        return;
    }

    pGetRow = (ULONG *) visible_frame;
    pPutRow = (ULONG *) working_frame;

    CHECK_DAVE_STATUS(d2_setalphablendmode(dave,d2_bm_one, d2_bm_zero))
    CHECK_DAVE_STATUS(d2_setalpha(dave,0xff))

    CHECK_DAVE_STATUS(d2_framebuffer(dave, pPutRow,
                      (d2_u32)(canvas -> gx_canvas_x_resolution), (d2_u32)(canvas -> gx_canvas_x_resolution),
                      (d2_u32)(canvas -> gx_canvas_y_resolution), d2_mode_alpha8))

    CHECK_DAVE_STATUS(d2_cliprect(dave,
                      copy_clip.gx_rectangle_left,
                      copy_clip.gx_rectangle_top,
                      copy_clip.gx_rectangle_right,
                      copy_clip.gx_rectangle_bottom))

    CHECK_DAVE_STATUS(d2_setblitsrc(dave, (void *) pGetRow,
                      canvas->gx_canvas_x_resolution,
                      canvas->gx_canvas_x_resolution,
                      canvas->gx_canvas_y_resolution,
                      d2_mode_alpha8))

    CHECK_DAVE_STATUS(d2_blitcopy(dave,
                                  copy_width, copy_height,
                                  (d2_blitpos)(copy_clip.gx_rectangle_left),
                                  (d2_blitpos)(copy_clip.gx_rectangle_top),
                                  (d2_width)(D2_FIX4((UINT)copy_width)),
                                  (d2_width)(D2_FIX4((UINT)copy_height)),
                                  (d2_point)(D2_FIX4((USHORT)copy_clip.gx_rectangle_left)),
                                  (d2_point)(D2_FIX4((USHORT)copy_clip.gx_rectangle_top)),
                                  /*LDRA_INSPECTED 96 S Mixed mode arithmetic is used for d2_bf_no_blitctxbackup. */
                                  /*LDRA_INSPECTED 96 S Mixed mode arithmetic is used for the macro d2_bf_usealpha. */
                                  (d2_u32)d2_bf_no_blitctxbackup | (d2_u32)d2_bf_usealpha))

    CHECK_DAVE_STATUS(d2_endframe(display -> gx_display_accelerator))
    CHECK_DAVE_STATUS(d2_startframe(display -> gx_display_accelerator))
}

#endif /* GX_USE_SYNERGY_DRW */

/*******************************************************************************************************************//**
 * @brief  GUIX display driver for Synergy, Frame buffer toggle operation with copying data by software without
 * D/AVE 2D acceleration and screen rotation.
 * This function is called by _gx_synergy_buffer_toggle_8bpp().
 * @param[in]     canvas            Pointer to a canvas
 * @param[in]     copy              Pointer to a rectangle region to copy
 **********************************************************************************************************************/
static VOID gx_copy_visible_to_working_8bpp(GX_CANVAS *canvas, GX_RECTANGLE *copy)
{
    GX_RECTANGLE display_size;
    GX_RECTANGLE copy_clip;

    ULONG        *pGetRow;
    ULONG        *pPutRow;

    INT          copy_width;
    INT          copy_height;
    INT          canvas_stride;
    INT          display_stride;

    ULONG        *pGet;
    ULONG        *pPut;
    INT          row;
    INT          col;

    GX_DISPLAY *display = canvas->gx_canvas_display;

    _gx_utility_rectangle_define(&display_size, 0, 0,
            (GX_VALUE)(display->gx_display_width - 1),
            (GX_VALUE)(display->gx_display_height - 1));
    copy_clip = *copy;

    /** Align copy region on 32-bit boundary. */
    copy_clip.gx_rectangle_left  = (GX_VALUE)((USHORT)copy_clip.gx_rectangle_left & 0xfffcU);
    copy_clip.gx_rectangle_right = (GX_VALUE)((USHORT)copy_clip.gx_rectangle_right | 3U);

    /** Offset canvas within frame buffer. */
    _gx_utility_rectangle_shift(&copy_clip, canvas->gx_canvas_display_offset_x, canvas->gx_canvas_display_offset_y);

    _gx_utility_rectangle_overlap_detect(&copy_clip, &display_size, &copy_clip);
    copy_width  = (INT)(copy_clip.gx_rectangle_right - copy_clip.gx_rectangle_left) + 1;
    copy_height = (INT)(copy_clip.gx_rectangle_bottom - copy_clip.gx_rectangle_top) + 1;

    if ((copy_width <= 0) || (copy_height <= 0))
    {
        return;
    }

    pGetRow = (ULONG *) visible_frame;
    pPutRow = (ULONG *) working_frame;

    copy_width /= 4;
    canvas_stride = (INT)canvas->gx_canvas_x_resolution / 4;
    pPutRow = pPutRow + ((INT)copy_clip.gx_rectangle_top *canvas_stride);
    pPutRow = pPutRow + ((INT)copy_clip.gx_rectangle_left / 4);

    display_stride = (INT)display->gx_display_width / 4;
    pGetRow = pGetRow + ((INT)(canvas->gx_canvas_display_offset_y + copy_clip.gx_rectangle_top) * display_stride);
    pGetRow = pGetRow + (INT)((canvas->gx_canvas_display_offset_x + copy_clip.gx_rectangle_left) / 4);

    for (row = 0; row < copy_height; row++)
    {
        pGet = pGetRow;
        pPut = pPutRow;

        for (col = 0; col < copy_width; col++)
        {
            *pPut = *pGet;
            ++pPut;
            ++pGet;
        }
        pPutRow += canvas_stride;
        pGetRow += display_stride;
    }
}


/*******************************************************************************************************************//**
 * @brief  GUIX display driver for Synergy, 8bpp Frame buffer software copy operation with screen rotation.
 * This function is called by _gx_synergy_buffer_toggle_8bpp().
 * @param[in]     canvas            Pointer to a canvas
 * @param[in]     copy              Pointer to a rectangle region to copy.
 * @param[in]     angle    Rotation angle (0, 90, 180 or 270)
 **********************************************************************************************************************/
static VOID gx_flip_canvas_to_working_8bpp(GX_CANVAS *canvas, GX_RECTANGLE *copy)
{
    GX_RECTANGLE display_size;
    GX_RECTANGLE copy_clip;

    GX_UCHAR *pGet;
    GX_UCHAR *pPut;
    GX_UCHAR *pGetRow;
    GX_UCHAR *pPutRow;

    INT          copy_width;
    INT          copy_height;
    INT          canvas_stride;
    INT          display_stride;
    INT          row;
    INT          col;

    GX_DISPLAY *display = canvas->gx_canvas_display;
    gx_utility_rectangle_define(&display_size, 0, 0,
                                (GX_VALUE)(display->gx_display_width - 1),
                                (GX_VALUE)(display->gx_display_height - 1));
    copy_clip = *copy;

    /** Offset canvas within frame buffer. */
    gx_utility_rectangle_shift(&copy_clip, canvas->gx_canvas_display_offset_x, canvas->gx_canvas_display_offset_y);

    /** Align copy region on 32-bit boundary. */
    //copy_clip.gx_rectangle_left   = (GX_VALUE)((USHORT)copy_clip.gx_rectangle_left & 0xfffcU);
    //copy_clip.gx_rectangle_top    = (GX_VALUE)((USHORT)copy_clip.gx_rectangle_top & 0xfffcU);
    //copy_clip.gx_rectangle_right  = (GX_VALUE)((USHORT)copy_clip.gx_rectangle_right  | 3U);
    //copy_clip.gx_rectangle_bottom = (GX_VALUE)((USHORT)copy_clip.gx_rectangle_bottom | 3U);

    gx_utility_rectangle_overlap_detect(&copy_clip, &display_size, &copy_clip);
    copy_width  = (INT)(copy_clip.gx_rectangle_right - copy_clip.gx_rectangle_left) + 1;
    copy_height = (INT)(copy_clip.gx_rectangle_bottom - copy_clip.gx_rectangle_top) + 1;

    if ((copy_width <= 0) || (copy_height <= 0))
    {
        return;
    }

    pGetRow = (GX_UCHAR *) canvas->gx_canvas_memory;
    pPutRow = (GX_UCHAR *) working_frame;

    canvas_stride = (INT)canvas->gx_canvas_x_resolution;
    display_stride = (INT)display->gx_display_width;

    pPutRow +=  display->gx_display_height * display_stride;
    pPutRow -= copy_clip.gx_rectangle_top * display_stride;
    pPutRow -= copy_clip.gx_rectangle_left + 1;

    pGetRow = pGetRow + ((INT)(canvas->gx_canvas_display_offset_y + copy_clip.gx_rectangle_top) *canvas_stride);
    pGetRow = pGetRow + (INT)(canvas->gx_canvas_display_offset_x + copy_clip.gx_rectangle_left);

    for (row = 0; row < copy_height; row++)
    {
        pGet = pGetRow;
        pPut = pPutRow;

        for (col = 0; col < copy_width; col++)
        {
            *pPut-- = *pGet++;
        }
        pPutRow -= display_stride;
        pGetRow += canvas_stride;
    }
}



/*******************************************************************************************************************//**
 * @} (end addtogroup SF_EL_GX)
 **********************************************************************************************************************/
