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
/**   STM324XG Screen Driver                                              */
/**                                                                       */
/**************************************************************************/


#include   "tx_api.h"
#include   "gx_api.h"
#include   "gx_system.h"
#include   "gx_display.h"
#include   "gx_utility.h"
#include   "gx_display_driver_stm324x9i.h"
#include   "stm324x9i_eval_lcd.h"

#define SDRAM _Pragma("location=\"SD_RAM\"")

SDRAM USHORT frame_buffer[STM32F429_DISCO_SCREEN_WIDTH][STM32F429_DISCO_SCREEN_HEIGHT];

static DMA2D_HandleTypeDef Dma2dHandle;
static UINT                (*dma_wait_function)(void *) = GX_NULL;
static VOID               *wait_func_param;
static USHORT             *visible_frame_buffer = (USHORT *)frame_buffer;
#ifdef GX_PING_PONG_FRAME_BUFFERS
static TX_SEMAPHORE        register_reload_semaphore;
static GX_BOOL             frame_buffer_toggle_start = GX_FALSE;
#endif

/**************************************************************************/
/*  gx_display_wait_function_set                                          */
/*    Default function override, provide a service to call back into      */
/*    the application while waiting for DMA2D operation to complete       */
/**************************************************************************/
VOID gx_display_wait_function_set_565rgb(UINT (*wait_func)(void *), void *param)
{
    dma_wait_function = wait_func;
    wait_func_param = param;
}

/**************************************************************************/
/*  DefaultDmaInitStruct                                                  */
/*    Configure DMA2D default structure values                            */
/**************************************************************************/
static void DefaultDmaInitStruct()
{
    /* Configure DMA2D */
    Dma2dHandle.Instance = DMA2D;
    HAL_DMA2D_DeInit(&Dma2dHandle);
    Dma2dHandle.Init.ColorMode = DMA2D_RGB565;

    /* Configures the color mode of the output image */
    DMA2D -> OPFCCR &= ~(uint32_t)DMA2D_OPFCCR_CM;
    DMA2D -> OPFCCR |= DMA2D_RGB565;
}

/**************************************************************************/
/*  gx_dma_init                                                           */
/*    Program DMA2D control structure values                              */
/**************************************************************************/
static void gx_dma_init(uint32_t Mode, uint32_t OutputMemoryAdd, uint32_t OutputOffset, uint32_t PixelPerLine, uint32_t NumberOfLine)
{
uint32_t pixline  = 0;

    /* Configures the DMA2D operation mode */
    DMA2D -> CR &= (uint32_t)CR_MASK;
    DMA2D -> CR |= Mode;

    /* Configures the output memory address */
    DMA2D -> OMAR = OutputMemoryAdd;

    /* Configure  the line Offset */
    DMA2D -> OOR &= ~(uint32_t)DMA2D_OOR_LO;
    DMA2D -> OOR |= (OutputOffset);

    /* Configure the number of line and pixel per line */
    pixline = PixelPerLine << 16;
    DMA2D -> NLR &= ~(DMA2D_NLR_NL | DMA2D_NLR_PL);
    DMA2D -> NLR |= ((NumberOfLine) | (pixline));
}

/**************************************************************************/
/*  gx_dma_run                                                            */
/*    Kick off a DMA2D operation                                          */
/*    Possibly call back into application while waiting for operation to  */
/*    complete.                                                           */
/**************************************************************************/
static void gx_dma_run()
{
    if (dma_wait_function == GX_NULL)
    {
        TIMING_CPU_WORK_END     // cpu work is done
    }

    /* tell the engine to start */
    DMA2D -> CR |= (uint32_t)DMA2D_CR_START;

    if (dma_wait_function)
    {
        dma_wait_function(wait_func_param);
        dma_wait_function = GX_NULL;
        TIMING_CPU_WORK_END     // cpu work is done
    }

    /* Wait for TCIF Flag activation */

    while (((DMA2D -> ISR) & 0x02) == 0)
    {
    }

    /* clear the status flag */

    DMA2D -> IFCR |= 0x02;
    TIMING_CPU_WORK_BEGIN // back to doing cpu work
}

/**************************************************************************/
/*  gx_chromeart_horizontal_line_draw                                     */
/*    Override generic function to utilize DMA2D                          */
/**************************************************************************/
static VOID gx_chromeart_rotated_horizontal_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT xend, INT ypos, INT width, GX_COLOR color)
{
uint32_t   put;
int        length;
GX_CANVAS *canvas = context -> gx_draw_context_canvas;

    length = xend - xstart + 1;

    if (canvas -> gx_canvas_display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
    {
        xstart = ypos;
        ypos = (canvas -> gx_canvas_x_resolution - xend - 1);
    }
    else
    {
        GX_SWAP_VALS(xstart, ypos);
        xstart = (canvas -> gx_canvas_y_resolution - xstart - width);
    }

    put = (uint32_t)canvas -> gx_canvas_memory;
    put += (canvas -> gx_canvas_y_resolution * 2 * ypos) + (xstart * 2);


    gx_dma_init(DMA2D_R2M, put, canvas -> gx_canvas_y_resolution - width, width, length);
    DMA2D -> OCOLR = color;

    gx_dma_run();
}


/**************************************************************************/
/*  gx_chromeart_vertial_line_draw                                        */
/*    Override generic function to utilize DMA2D                          */
/**************************************************************************/
static VOID gx_chromeart_rotated_vertical_line_draw(GX_DRAW_CONTEXT *context, INT ystart, INT yend, INT xpos, INT width, GX_COLOR color)
{
uint32_t   put;
GX_CANVAS *canvas = context -> gx_draw_context_canvas;
int        length;

    length = yend - ystart + 1;

    if (canvas -> gx_canvas_display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
    {
        GX_SWAP_VALS(xpos, ystart)
        ystart = canvas -> gx_canvas_x_resolution - ystart - width;
    }
    else
    {
        ystart = xpos;
        xpos = canvas -> gx_canvas_y_resolution - yend - 1;
    }

    put = (uint32_t)canvas -> gx_canvas_memory;
    put += (canvas -> gx_canvas_y_resolution * 2 * ystart) + (xpos * 2);

    gx_dma_init(DMA2D_R2M, put, canvas -> gx_canvas_y_resolution - length, length, width);
    DMA2D -> OCOLR = color;
    gx_dma_run();
}

/**************************************************************************/
/*  gx_chromeart_canvas_copy                                              */
/*    Override generic function to utilize DMA2D                          */
/**************************************************************************/
static void gx_chromeart_rotated_canvas_copy(GX_CANVAS *canvas, GX_CANVAS *composite)
{
GX_RECTANGLE dirty;
GX_RECTANGLE overlap;
USHORT      *read;
USHORT      *write;
INT          width;
GX_RECTANGLE rotated_overlap;
INT          rotated_dirty_left;
INT          rotated_dirty_top;

    dirty.gx_rectangle_left = dirty.gx_rectangle_top = 0;
    dirty.gx_rectangle_right = canvas -> gx_canvas_x_resolution - 1;
    dirty.gx_rectangle_bottom = canvas -> gx_canvas_y_resolution - 1;

    _gx_utility_rectangle_shift(&dirty, canvas -> gx_canvas_display_offset_x, canvas -> gx_canvas_display_offset_y);

    if (_gx_utility_rectangle_overlap_detect(&dirty, &composite -> gx_canvas_dirty_area, &overlap))
    {
        if (canvas -> gx_canvas_display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
        {
            rotated_overlap.gx_rectangle_left = overlap.gx_rectangle_top;
            rotated_overlap.gx_rectangle_right = overlap.gx_rectangle_bottom;
            rotated_overlap.gx_rectangle_top = canvas -> gx_canvas_x_resolution - overlap.gx_rectangle_right - 1;
            rotated_overlap.gx_rectangle_bottom = canvas -> gx_canvas_x_resolution - overlap.gx_rectangle_left - 1;

            rotated_dirty_left = dirty.gx_rectangle_top;
            rotated_dirty_top = canvas -> gx_canvas_x_resolution - dirty.gx_rectangle_right - 1;
        }
        else
        {
            rotated_overlap.gx_rectangle_left = canvas -> gx_canvas_y_resolution - overlap.gx_rectangle_bottom - 1;
            rotated_overlap.gx_rectangle_right = canvas -> gx_canvas_y_resolution - overlap.gx_rectangle_top - 1;
            rotated_overlap.gx_rectangle_top = overlap.gx_rectangle_left;
            rotated_overlap.gx_rectangle_bottom = overlap.gx_rectangle_right;

            rotated_dirty_left = canvas -> gx_canvas_y_resolution - dirty.gx_rectangle_bottom - 1;
            rotated_dirty_top = dirty.gx_rectangle_left;
        }

        width = rotated_overlap.gx_rectangle_right - rotated_overlap.gx_rectangle_left + 1;
        read = (USHORT *)canvas -> gx_canvas_memory;

        /* index into starting row */
        read += (rotated_overlap.gx_rectangle_top - rotated_dirty_top) * canvas -> gx_canvas_y_resolution;

        /* index into pixel */

        read += rotated_overlap.gx_rectangle_left - rotated_dirty_left;

        /* calculate the write pointer */
        write = (USHORT *)composite -> gx_canvas_memory;
        write += rotated_overlap.gx_rectangle_top * composite -> gx_canvas_y_resolution;
        write += rotated_overlap.gx_rectangle_left;

        gx_dma_init(DMA2D_M2M, (uint32_t)write, STM32F429_DISCO_SCREEN_WIDTH - width, width, rotated_overlap.gx_rectangle_bottom - rotated_overlap.gx_rectangle_top + 1);

        DMA2D -> FGPFCCR = (0xff000000) | DMA2D_RGB565;           // foreground format conversion
        DMA2D -> FGMAR = (uint32_t)read;                          // foreground memory address
        DMA2D -> FGOR = canvas -> gx_canvas_y_resolution - width; // foreground offset
        gx_dma_run();
    }
}


/**************************************************************************/
/*  gx_chromeart_rotated_pixelmap_blend                                   */
/*    Override generic function to utilize DMA2D                          */
/**************************************************************************/
static void gx_chromeart_rotated_pixelmap_blend(GX_DRAW_CONTEXT *context,
                                                INT xpos, INT ypos, GX_PIXELMAP *pixelmap, UCHAR alpha)
{
INT           width;
INT           height;
USHORT       *putrow;
USHORT       *getshort;
GX_CANVAS    *canvas;
ULONG         blend_reg;
GX_RECTANGLE *clip;
GX_RECTANGLE  rotated_clip;

    /* if the pixelmap is compressed, we must draw
       in software
     */
    if ((pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED) ||
        (pixelmap -> gx_pixelmap_format == GX_COLOR_FORMAT_565RGB) ||
        ((pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_ALPHA) && (pixelmap -> gx_pixelmap_format == GX_COLOR_FORMAT_565RGB)))
    {
        _gx_display_driver_565rgb_rotated_pixelmap_blend(context, xpos, ypos, pixelmap, alpha);
        return;
    }

    /* the pixelmap is raw, use ChromeArt */
    clip = context -> gx_draw_context_clip;
    canvas = context -> gx_draw_context_canvas;

    GX_SWAP_VALS(xpos, ypos)
    if (canvas -> gx_canvas_display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
    {
        rotated_clip.gx_rectangle_left = clip -> gx_rectangle_top;
        rotated_clip.gx_rectangle_right = clip -> gx_rectangle_bottom;
        rotated_clip.gx_rectangle_top = canvas -> gx_canvas_x_resolution - clip -> gx_rectangle_right - 1;
        rotated_clip.gx_rectangle_bottom = canvas -> gx_canvas_x_resolution - clip -> gx_rectangle_left - 1;

        ypos = canvas -> gx_canvas_x_resolution - ypos - pixelmap -> gx_pixelmap_width;
    }
    else
    {
        rotated_clip.gx_rectangle_left = canvas -> gx_canvas_y_resolution - clip -> gx_rectangle_top - 1;
        rotated_clip.gx_rectangle_right = canvas -> gx_canvas_y_resolution - clip -> gx_rectangle_bottom - 1;
        rotated_clip.gx_rectangle_top = clip -> gx_rectangle_left;
        rotated_clip.gx_rectangle_bottom = clip -> gx_rectangle_right;

        xpos = canvas -> gx_canvas_y_resolution - xpos - pixelmap -> gx_pixelmap_height;
    }

    putrow = (USHORT *)context -> gx_draw_context_memory;
    putrow += rotated_clip.gx_rectangle_top * context -> gx_draw_context_pitch;
    putrow += rotated_clip.gx_rectangle_left;

    getshort = (USHORT *)(pixelmap -> gx_pixelmap_data);
    getshort += pixelmap -> gx_pixelmap_height * (rotated_clip.gx_rectangle_top - ypos);
    getshort += (rotated_clip.gx_rectangle_left - xpos);

    width = rotated_clip.gx_rectangle_right - rotated_clip.gx_rectangle_left + 1;
    height = rotated_clip.gx_rectangle_bottom - rotated_clip.gx_rectangle_top + 1;

    gx_dma_init(DMA2D_M2M_BLEND, (uint32_t)putrow, canvas -> gx_canvas_y_resolution - width, width, height);

    /* Set foreground memory address.  */
    DMA2D -> FGMAR = (uint32_t)getshort;

    blend_reg = (alpha << 24);
    blend_reg |= DMA2D_RGB565;

    if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_ALPHA)
    {
        /* Blend original alpha with alpha.  */
        blend_reg |= (DMA2D_COMBINE_ALPHA << 16);
    }
    else
    {
        /* Replace alpha.  */
        blend_reg |= (DMA2D_REPLACE_ALPHA << 16);
    }

    /* Set foreground format conversion.  */
    DMA2D -> FGPFCCR = blend_reg;

    /* Set foreground offset.  */
    DMA2D -> FGOR = pixelmap -> gx_pixelmap_height - width;

    /* Set background memory address. */
    DMA2D -> BGMAR = (uint32_t)putrow;

    /* Set background offset.  */
    DMA2D -> BGOR = canvas -> gx_canvas_y_resolution - width;
    DMA2D -> BGPFCCR = 0xff000000 | (DMA2D_REPLACE_ALPHA << 16) | CM_RGB565;

    gx_dma_run();
}

/**************************************************************************/
/*  gx_chromeart_rotated_pixelmap_draw                                    */
/*    Override generic function to utilize DMA2D                          */
/**************************************************************************/
static void gx_chromeart_rotated_pixelmap_draw(GX_DRAW_CONTEXT *context,
                                               INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT           width;
INT           height;
USHORT       *putrow;
GX_UBYTE     *get;
GX_CANVAS    *canvas;
ULONG         reg_val;
uint32_t      DMA2D_Mode;
GX_RECTANGLE *clip;
GX_RECTANGLE  rotated_clip;
INT           map_offset;


    /* If the pixelmap is compressed, we must draw in software.  */
    if (pixelmap -> gx_pixelmap_flags & (GX_PIXELMAP_COMPRESSED | GX_PIXELMAP_TRANSPARENT))
    {
        _gx_display_driver_565rgb_rotated_pixelmap_draw(context, xpos, ypos, pixelmap);
        return;
    }

    /* If the pixelmap is 565 format with alpha channel, we must draw in software.  */
    if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_ALPHA &&
        pixelmap -> gx_pixelmap_format == GX_COLOR_FORMAT_565RGB)
    {
        _gx_display_driver_565rgb_rotated_pixelmap_draw(context, xpos, ypos, pixelmap);
        return;
    }
    
    if(context -> gx_draw_context_brush.gx_brush_alpha != 0xff)
    {
        gx_chromeart_rotated_pixelmap_blend(context, xpos, ypos, pixelmap, context -> gx_draw_context_brush.gx_brush_alpha);
        return;
    }

    /* The pixelmap is raw, use ChromeArt.  */
    clip = context -> gx_draw_context_clip;
    canvas = context -> gx_draw_context_canvas;

    GX_SWAP_VALS(xpos, ypos)
    if (canvas -> gx_canvas_display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
    {
        rotated_clip.gx_rectangle_left = clip -> gx_rectangle_top;
        rotated_clip.gx_rectangle_right = clip -> gx_rectangle_bottom;
        rotated_clip.gx_rectangle_top = canvas -> gx_canvas_x_resolution - clip -> gx_rectangle_right - 1;
        rotated_clip.gx_rectangle_bottom = canvas -> gx_canvas_x_resolution - clip -> gx_rectangle_left - 1;

        ypos = canvas -> gx_canvas_x_resolution - ypos - pixelmap -> gx_pixelmap_width;
    }
    else
    {
        rotated_clip.gx_rectangle_left = canvas -> gx_canvas_y_resolution - clip -> gx_rectangle_bottom - 1;
        rotated_clip.gx_rectangle_right = canvas -> gx_canvas_y_resolution - clip -> gx_rectangle_top - 1;
        rotated_clip.gx_rectangle_top = clip -> gx_rectangle_left;
        rotated_clip.gx_rectangle_bottom = clip -> gx_rectangle_right;

        xpos = canvas -> gx_canvas_y_resolution - xpos - pixelmap -> gx_pixelmap_height;
    }

    width = rotated_clip.gx_rectangle_right - rotated_clip.gx_rectangle_left + 1;
    height = rotated_clip.gx_rectangle_bottom - rotated_clip.gx_rectangle_top + 1;

    putrow = (USHORT *)context -> gx_draw_context_memory;
    putrow += rotated_clip.gx_rectangle_top * context -> gx_draw_context_pitch;
    putrow += rotated_clip.gx_rectangle_left;

    map_offset = pixelmap -> gx_pixelmap_height * (rotated_clip.gx_rectangle_top - ypos);
    map_offset += (rotated_clip.gx_rectangle_left - xpos);

    reg_val = 0xff000000;
    if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_ALPHA)
    {
        DMA2D_Mode = DMA2D_M2M_BLEND;

        /* Set background memory address.  */
        DMA2D -> BGMAR = (uint32_t)putrow;

        /* Set background offset.  */
        DMA2D -> BGOR = canvas -> gx_canvas_y_resolution - width;

        /* Set background alpha mode and color mode. */
        DMA2D -> BGPFCCR = 0xff000000 | (DMA2D_REPLACE_ALPHA << 16) | CM_RGB565;
    }
    else
    {
        reg_val |= (DMA2D_REPLACE_ALPHA << 16);
        DMA2D_Mode = DMA2D_M2M;
    }

    switch (pixelmap -> gx_pixelmap_format)
    {
    case GX_COLOR_FORMAT_565RGB:
        map_offset <<= 1;
        reg_val |= CM_RGB565;
        break;

    case GX_COLOR_FORMAT_4444ARGB:
        map_offset <<= 1;
        reg_val |= CM_ARGB4444;
        break;

    case GX_COLOR_FORMAT_8BIT_PALETTE:
        reg_val |= CM_L8;

        /* Set CLUT address.  */
        DMA2D -> FGCMAR = (uint32_t)pixelmap -> gx_pixelmap_aux_data;

        /* Set CLUT size.  */
        DMA2D -> FGPFCCR |= (pixelmap -> gx_pixelmap_aux_data_size << 8);

        /* Start the automatic loading of CLUT.  */
        DMA2D -> FGPFCCR |= (0x1 << 5);

        /* Wait CLUT loading complete.  */
        while (((DMA2D -> ISR) & DMA2D_ISR_CTCIF) == 0)
        {
        }

        DMA2D_Mode = DMA2D_M2M_PFC;
        break;

    default:
        return;
    }

    get = (GX_UBYTE *)(pixelmap -> gx_pixelmap_data);
    get += map_offset;

    /* Set forground memory address.  */
    DMA2D -> FGMAR = (uint32_t)(get);
    DMA2D -> FGPFCCR = reg_val;

    /* Set forground offset. */
    DMA2D -> FGOR = pixelmap -> gx_pixelmap_height - width;

    gx_dma_init(DMA2D_Mode, (uint32_t)putrow, canvas -> gx_canvas_y_resolution - width, width, height);
    gx_dma_run();
}

/**************************************************************************/
/*  gx_chromeart_glyph_8bit_draw                                          */
/*    Override generic function to utilize DMA2D                          */
/**************************************************************************/
static void gx_chromeart_rotated_glyph_8bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area,
                                                 GX_POINT *map_offset, const GX_GLYPH *glyph)
{
GX_CANVAS *canvas;
GX_UBYTE  *read_glyph;
USHORT    *write_row;
GX_COLOR   text_color;
UINT       height;
UINT       width;
UCHAR      red, green, blue;
GX_UBYTE   alpha;
ULONG      pfc_reg;
GX_VALUE   rotated_map_offset_x;
GX_VALUE   rotated_map_offset_y;
GX_VALUE   rotated_left;
GX_VALUE   rotated_top;

    text_color =  context -> gx_draw_context_brush.gx_brush_line_color;

    alpha = context -> gx_draw_context_brush.gx_brush_alpha;

    if (alpha == 0)
    {
        return;
    }

    red = (text_color & 0xf800) >> 8;
    green = (text_color & 0x07e0) >> 3;
    blue = (text_color & 0x001f) << 3;
    text_color = (red << 16) | (green << 8) | blue;

    /* pickup pointer to current display driver */
    canvas = context -> gx_draw_context_canvas;

    if (context -> gx_draw_context_display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
    {
        rotated_left = draw_area -> gx_rectangle_top;
        rotated_top = (GX_VALUE)(canvas -> gx_canvas_x_resolution - draw_area -> gx_rectangle_right - 1);

        rotated_map_offset_x = map_offset -> gx_point_y;
        rotated_map_offset_y = (GX_VALUE)(glyph -> gx_glyph_width - map_offset -> gx_point_x - (GX_VALUE)(draw_area -> gx_rectangle_right - draw_area -> gx_rectangle_left + 1));
    }
    else
    {
        rotated_left = (GX_VALUE)(canvas -> gx_canvas_display_offset_y - draw_area -> gx_rectangle_bottom - 1);
        rotated_top = draw_area -> gx_rectangle_left;

        rotated_map_offset_x = (GX_VALUE)(glyph -> gx_glyph_height - map_offset -> gx_point_y - (GX_VALUE)(draw_area -> gx_rectangle_bottom - draw_area -> gx_rectangle_top + 1));
        rotated_map_offset_y = map_offset -> gx_point_x;
    }

    read_glyph = (GX_UBYTE *)glyph -> gx_glyph_map;

    if (rotated_map_offset_y)
    {
        read_glyph = read_glyph + (glyph -> gx_glyph_height * rotated_map_offset_y);
    }

    read_glyph += rotated_map_offset_x;

    height = draw_area -> gx_rectangle_right - draw_area -> gx_rectangle_left + 1;
    width = draw_area -> gx_rectangle_bottom - draw_area -> gx_rectangle_top + 1;

    write_row = (USHORT *)canvas -> gx_canvas_memory;
    write_row += rotated_top * context -> gx_draw_context_pitch;
    write_row += rotated_left;

    gx_dma_init(DMA2D_M2M_BLEND, (uint32_t)write_row, canvas -> gx_canvas_y_resolution - width, width, height);

    DMA2D -> FGMAR = (uint32_t)read_glyph;            // foreground memory address
    DMA2D -> FGOR = glyph -> gx_glyph_height - width; // foreground offset
    pfc_reg = alpha;
    pfc_reg <<= 24;
    pfc_reg |= 0x00020009;

    DMA2D -> FGPFCCR = pfc_reg;
    DMA2D -> FGCOLR = text_color;

    DMA2D -> BGMAR = (uint32_t)write_row;                     // background memory address
    DMA2D -> BGOR = canvas -> gx_canvas_y_resolution - width; // background offset
    DMA2D -> BGPFCCR = 0xff000002;

    gx_dma_run();
}

#if defined(GX_TIMING_ENABLE)
/**************************************************************************/
/*  gx_chromeart_drawing_initiate                                         */
/*    Override generic function to toggle I/O to allow precise timing     */
/*    measurement.                                                        */
/**************************************************************************/
static VOID gx_chromeart_drawing_initiate(GX_DISPLAY *display, GX_CANVAS *canvas)
{
TIMING_FRAME_CALC_BEGIN
    TIMING_CPU_WORK_BEGIN
}

/**************************************************************************/
/*  gx_chromeart_drawing_complete                                         */
/*    Override generic function to toggle I/O to allow precise timing     */
/*    measurement.                                                        */
/**************************************************************************/
static VOID gx_chromeart_drawing_complete(GX_DISPLAY *display, GX_CANVAS *canvas)
{
TIMING_FRAME_CALC_END
    TIMING_CPU_WORK_END
}
#endif

/**************************************************************************/
/*  stm32f429_24xrgb_buffer_toggle                                        */
/*    Override generic function to utilize DMA2D                          */
/**************************************************************************/
static void stm32f429_565rgb_buffer_rotated_toggle(GX_CANVAS *canvas, GX_RECTANGLE *dirty)
{
GX_RECTANGLE Limit;
GX_RECTANGLE Copy;
GX_RECTANGLE rotated_copy;
INT          rotated_offset_x;
INT          rotated_offset_y;
ULONG        offset;
INT          copy_width;
INT          copy_height;

#if !defined(GX_CHROMEART_ENABLE)
INT row;
INT src_stride_ulongs;
INT dest_stride_ulongs;
#endif

ULONG  *get;
ULONG  *put;
#ifdef GX_PING_PONG_FRAME_BUFFERS
USHORT *temp;

    frame_buffer_toggle_start = GX_TRUE;

    /* Set frame buffer start address. */
    LTDC_Layer1 -> CFBAR = (uint32_t)canvas -> gx_canvas_memory;
    LTDC -> SRCR = LTDC_SRCR_VBR;

    /* Wait for register reload interrupt. */
    tx_semaphore_get(&register_reload_semaphore, TX_WAIT_FOREVER);
#endif

    gx_utility_rectangle_define(&Limit, 0, 0,
                                canvas -> gx_canvas_x_resolution - 1,
                                canvas -> gx_canvas_y_resolution - 1);

    if (gx_utility_rectangle_overlap_detect(&Limit, &canvas -> gx_canvas_dirty_area, &Copy))
    {
        if (canvas -> gx_canvas_display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
        {
            rotated_copy.gx_rectangle_left = Copy.gx_rectangle_top;
            rotated_copy.gx_rectangle_right = Copy.gx_rectangle_bottom;
            rotated_copy.gx_rectangle_top = canvas -> gx_canvas_x_resolution - Copy.gx_rectangle_right - 1;
            rotated_copy.gx_rectangle_bottom = canvas -> gx_canvas_x_resolution - Copy.gx_rectangle_left - 1;

            rotated_offset_x = canvas -> gx_canvas_display_offset_y;
            rotated_offset_y = STM32F429_DISCO_SCREEN_HEIGHT - canvas -> gx_canvas_display_offset_x - canvas -> gx_canvas_x_resolution;
        }
        else
        {
            rotated_copy.gx_rectangle_left = canvas -> gx_canvas_y_resolution - Copy.gx_rectangle_bottom - 1;
            rotated_copy.gx_rectangle_right = canvas -> gx_canvas_y_resolution - Copy.gx_rectangle_top - 1;
            rotated_copy.gx_rectangle_top = Copy.gx_rectangle_left;
            rotated_copy.gx_rectangle_bottom = Copy.gx_rectangle_right;

            rotated_offset_x = STM32F429_DISCO_SCREEN_WIDTH - canvas -> gx_canvas_display_offset_y - canvas -> gx_canvas_y_resolution;
            rotated_offset_y = canvas -> gx_canvas_display_offset_x;
        }

        rotated_copy.gx_rectangle_left &= 0xfffe;
        rotated_copy.gx_rectangle_right |= 0x01;
        copy_width = rotated_copy.gx_rectangle_right - rotated_copy.gx_rectangle_left + 1;

        copy_height = rotated_copy.gx_rectangle_bottom - rotated_copy.gx_rectangle_top + 1;

        offset = rotated_copy.gx_rectangle_top * canvas -> gx_canvas_y_resolution;
        offset += rotated_copy.gx_rectangle_left;
        offset /= 2;        // in ulongs
        get = canvas -> gx_canvas_memory;
        get += offset;

        // offset by dirty rect pos
        put = (ULONG *)visible_frame_buffer;

        // offset by canvas offset:
        offset = (rotated_offset_y + rotated_copy.gx_rectangle_top) * STM32F429_DISCO_SCREEN_WIDTH;
        offset += rotated_offset_x + rotated_copy.gx_rectangle_left;
        offset /= 2;
        put += offset;


#if defined(GX_CHROMEART_ENABLE)
        gx_dma_init(DMA2D_M2M, (uint32_t)put, STM32F429_DISCO_SCREEN_WIDTH - copy_width, copy_width, copy_height);

        DMA2D -> FGPFCCR = (0xff000000) | DMA2D_RGB565;                // foreground format conversion
        DMA2D -> FGMAR = (uint32_t)get;                                // foreground memory address
        DMA2D -> FGOR = canvas -> gx_canvas_y_resolution - copy_width; // foreground offset
        gx_dma_run();

#else
        src_stride_ulongs = canvas -> gx_canvas_y_resolution / 2;
        dest_stride_ulongs = STM32F429_DISCO_SCREEN_WIDTH / 2;
        copy_width /= 2;     // in ulongs

        for (row = 0; row < copy_height; row++)
        {
            memcpy(put, get, copy_width * 4); /* Use case of memcpy is verified. */
            put += dest_stride_ulongs;
            get += src_stride_ulongs;
        }
#endif

#ifdef GX_PING_PONG_FRAME_BUFFERS
        /* Toggle frame buffer. */
        temp = (USHORT *)canvas -> gx_canvas_memory;
        canvas -> gx_canvas_memory = (ULONG *)visible_frame_buffer;
        visible_frame_buffer = temp;
#endif
    }
    TIMING_CPU_WORK_END
}

/*****************************************************************************/
/* Functions for configuring and using Graphics Layer 2                      */
/*****************************************************************************/
static LTDC_HandleTypeDef hltdc_anim;
static GX_LAYER_PARAMS    layer2_params;

/**************************************************************************/
/*  GraphicsLayerInitialize                                               */
/*    Configure graphics layer2, the overlay layer                        */
/**************************************************************************/
static UINT GraphicsLayerInitialize(INT layer, GX_CANVAS *canvas)
{
int       loop;
GX_COLOR *color;
ULONG     regval;

    if (layer != 2)
    {
        // graphics layer 1 is configured during initial setup, nothing
        // to do here.
        return GX_FAILURE;
}

hltdc_anim    .Instance = LTDC;
LCD_LayerCfgTypeDef Layercfg;

    Layercfg.WindowX0 = 0;
    Layercfg.WindowX1 = canvas -> gx_canvas_y_resolution;
    Layercfg.WindowY0 = 0;
    Layercfg.WindowY1 = canvas -> gx_canvas_x_resolution;

    Layercfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
    Layercfg.FBStartAdress = (uint32_t)canvas -> gx_canvas_memory;
    Layercfg.Alpha = 255;
    Layercfg.Alpha0 = 0;
    Layercfg.Backcolor.Blue = 0;
    Layercfg.Backcolor.Green = 0;
    Layercfg.Backcolor.Red = 0;
    Layercfg.BlendingFactor1 = LTDC_BLENDING_FACTOR2_PAxCA;
    Layercfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
    Layercfg.ImageWidth = canvas -> gx_canvas_x_resolution;
    Layercfg.ImageHeight = canvas -> gx_canvas_y_resolution;
    //Layercfg.ImageWidth = STM32F429_DISCO_SCREEN_WIDTH;
    //Layercfg.ImageHeight = STM32F429_DISCO_SCREEN_HEIGHT;

    HAL_LTDC_ConfigLayer(&hltdc_anim, &Layercfg, layer - 1);

    // set the initial alpha to 0 and disable graphics layer2
    LTDC_Layer2 -> CACR = 0;
    LTDC_Layer2 -> CR = 0x00;
    LTDC -> SRCR = LTDC_SRCR_IMR;

    color = canvas -> gx_canvas_memory;

    for (loop = 0; loop < (canvas -> gx_canvas_x_resolution * canvas -> gx_canvas_y_resolution) / 2; loop++)
    {
        *color++ = 0x00;
    }

    layer2_params.canvas_width = canvas -> gx_canvas_y_resolution;
    layer2_params.canvas_height = canvas -> gx_canvas_x_resolution;

    // save off the vertical and horizontal timing params, so that
    // we can offset from here if the app request a canvas shift

    regval = LTDC_Layer2 -> WHPCR;
    layer2_params.horizontal_start = regval & 0xffff;
    regval = LTDC_Layer2 -> WVPCR;
    layer2_params.vertical_start = regval & 0xffff;


    layer2_params.canvas_memory = (USHORT *)canvas -> gx_canvas_memory;
    return GX_SUCCESS;
}

/**************************************************************************/
/*  GraphicsLayerShow                                                     */
/*    Show the specified graphics layer                                   */
/**************************************************************************/
static VOID GraphicsLayerShow(INT Layer)
{
LTDC_Layer_TypeDef *layer_regs;

    if (Layer == 1)
    {
        layer_regs = LTDC_Layer1;
    }
    else
    {
        layer_regs = LTDC_Layer2;
    }
    layer_regs -> CR = 0x01;
    LTDC -> SRCR = LTDC_SRCR_VBR;
}

/**************************************************************************/
/*  GraphicsLayerHide                                                     */
/*    HIde the specified graphics layer                                   */
/**************************************************************************/
static void GraphicsLayerHide(INT Layer)
{
LTDC_Layer_TypeDef *layer_regs;

    if (Layer == 1)
    {
        layer_regs = LTDC_Layer1;
    }
    else
    {
        layer_regs = LTDC_Layer2;
    }

    layer_regs -> CR = 0x00;
    layer_regs -> CACR = 0;
    LTDC -> SRCR = LTDC_SRCR_VBR;
}

/**************************************************************************/
/*  GraphicsLayerAlphaSet                                                 */
/*    Assign graphics layer alpha value                                   */
/**************************************************************************/
static void GraphicsLayerAlphaSet(INT Layer, UCHAR alpha)
{
LTDC_Layer_TypeDef *layer_regs;

    if (Layer == 1)
    {
        layer_regs = LTDC_Layer1;
    }
    else
    {
        layer_regs = LTDC_Layer2;
    }

    layer_regs -> CACR = alpha;
    LTDC -> SRCR = LTDC_SRCR_VBR;
}

/**************************************************************************/
/*  GraphicsLayerPosSet                                                   */
/*    Assign graphics layer offset within display                         */
/**************************************************************************/
static void GraphicsLayerPosSet(INT layer, GX_VALUE xpos, GX_VALUE ypos)
{
uint32_t regval;
uint32_t frame_address;
INT      offset;
INT      width;
int      height;

    if (layer != 2)
    {
        return;
    }

    if (xpos >= 0)
    {
        width = layer2_params.canvas_width;

        if ((width + xpos) >= STM32F429_DISCO_SCREEN_WIDTH)
        {
            width = STM32F429_DISCO_SCREEN_WIDTH - xpos;
        }
        frame_address = (uint32_t)layer2_params.canvas_memory;
        LTDC_Layer2 -> CFBAR = frame_address;

        offset = xpos + layer2_params.horizontal_start;
        regval = ((offset + width - 1) << 16) | offset;
        LTDC_Layer2 -> WHPCR = regval;
        regval = (layer2_params.canvas_width * 2) << 16;    // horizontal pitch
        regval |= (width * 2) + 3;                          // horizontal line length
        LTDC_Layer2 -> CFBLR = regval;
    }
    else
    {
        offset = -xpos;
        frame_address = (uint32_t)layer2_params.canvas_memory;
        frame_address += offset * 2;
        LTDC_Layer2 -> CFBAR = frame_address;

        width = layer2_params.canvas_width - offset;
        regval = ((layer2_params.horizontal_start + width - 1) << 16) | (layer2_params.horizontal_start);
        LTDC_Layer2 -> WHPCR = regval;
        regval = (layer2_params.canvas_width * 2) << 16;    // horizontal pitch
        regval |= (width * 2) + 3;                          // horizontal line length
        LTDC_Layer2 -> CFBLR = regval;
    }

    // here for shift in Y direction
    if (ypos >= 0)
    {
        height = layer2_params.canvas_height;
        if ((ypos + height) > STM32F429_DISCO_SCREEN_HEIGHT)
        {
            height = STM32F429_DISCO_SCREEN_HEIGHT - ypos;
        }

        LTDC_Layer2 -> CFBLNR = height;

        offset = layer2_params.vertical_start + ypos;
        regval = ((offset + height - 1) << 16) | offset;
        LTDC_Layer2 -> WVPCR = regval;
    }
    else
    {
        regval = frame_address;
        offset = -ypos;
        regval += offset * layer2_params.canvas_width * 2;
        LTDC_Layer2 -> CFBAR = regval;

        height = layer2_params.canvas_height - offset;
        regval = ((layer2_params.vertical_start + height - 1) << 16) | (layer2_params.vertical_start);
        LTDC_Layer2 -> WVPCR = regval;
        LTDC_Layer2 -> CFBLNR = height;
    }
    LTDC -> SRCR = LTDC_SRCR_VBR;
}


/**************************************************************************/
/* define layer services supporting graphics overlay layer                */
/**************************************************************************/
static GX_DISPLAY_LAYER_SERVICES stm32_graphics_layer_services = {
    GraphicsLayerInitialize,
    GraphicsLayerShow,
    GraphicsLayerHide,
    GraphicsLayerAlphaSet,
    GraphicsLayerPosSet
};

/**************************************************************************/
/*  LTDC_IRQHandler                                                       */
/*    Handle line interrupt and register reload interrupt                 */
/**************************************************************************/
#if defined(GX_DISABLE_THREADX_TIMER_SOURCE) || defined(GX_PING_PONG_FRAME_BUFFERS)
void LTDC_IRQHandler()
{
#ifdef GX_DISABLE_THREADX_TIMER_SOURCE
    if (LTDC -> ISR & LTDC_ISR_LIF)
    {
        /* Clear the line interrupt flag. */
        LTDC -> ICR |= LTDC_ICR_CLIF;

        /* If there are no active timers just stop the timer from running. */
        if (_gx_system_active_timer_list == GX_NULL &&
            _gx_system_animation_list == GX_NULL)
        {
            return;
    }

    GX_EVENT timer_event;

        timer_event.gx_event_type = GX_EVENT_TIMER;
        timer_event.gx_event_sender = 0;
        timer_event.gx_event_target = GX_NULL;
        timer_event.gx_event_payload.gx_event_ulongdata = 1;
        _gx_system_event_fold(&timer_event);
    }
#endif

#ifdef GX_PING_PONG_FRAME_BUFFERS
    if (LTDC -> ISR & LTDC_ISR_RRIF)
    {
        /* Clear register reload interrupt. */
        LTDC -> ICR |= LTDC_ICR_CRRIF;

        if (frame_buffer_toggle_start)
        {
            frame_buffer_toggle_start = GX_FALSE;
            tx_semaphore_put(&register_reload_semaphore);
        }
    }
#endif
}
#endif

/**************************************************************************/
/* stm32f429_graphics_driver_setup_565rgb                                 */
/*   Configure hardware layer 1 and initialize GX_DISPLAY drawing         */
/*   services.                                                            */
/**************************************************************************/
UINT stm32f429_graphics_driver_setup_565rgb_rotated(GX_DISPLAY *display)
{
	/* initialize the low-level drawing function pointers
	
	   for windows, these are always just the generic funcions,
	   but for some hardware, these will be customized,
	   optimized functions specific to that hardware
	 */
	_gx_display_driver_565rgb_rotated_setup    (display, (VOID *)STM32F429_DISCO_SCREEN_HANDLE, stm32f429_565rgb_buffer_rotated_toggle);

#if defined(GX_TIMING_ENABLE)
	display     -> gx_display_driver_drawing_initiate     = gx_chromeart_drawing_initiate;
	display     -> gx_display_driver_drawing_complete     = gx_chromeart_drawing_complete;
#endif

	// setup support for second graphics layer:
	display     -> gx_display_layer_services = &stm32_graphics_layer_services;

#if defined(GX_CHROMEART_ENABLE)
	/* override those functions that can be accelerated with DMA2D */
	display     -> gx_display_driver_horizontal_line_draw = gx_chromeart_rotated_horizontal_line_draw;
	display     -> gx_display_driver_vertical_line_draw = gx_chromeart_rotated_vertical_line_draw;
	display     -> gx_display_driver_canvas_copy        = gx_chromeart_rotated_canvas_copy;
	display     -> gx_display_driver_pixelmap_draw      = gx_chromeart_rotated_pixelmap_draw;
	display     -> gx_display_driver_pixelmap_blend     = gx_chromeart_rotated_pixelmap_blend;
	display     -> gx_display_driver_8bit_glyph_draw    = gx_chromeart_rotated_glyph_8bit_draw;
	display     -> gx_display_driver_callback_assign    = gx_display_wait_function_set_565rgb;
#endif

	/* Hardware Initialization */
	BSP_LCD_Init    ();
	
	BSP_LCD_LayerDefaultInit    (0, (uint32_t)&frame_buffer);
	BSP_LCD_SelectLayer    (0);
	
	//LCD_SetPixelFormat(DMA2D_RGB565);
	LTDC_Layer1     -> CFBLR = LTDC_Layer1 -> CFBLR / 2;
	LTDC_Layer1     -> PFCR = DMA2D_RGB565;
	LTDC     -> SRCR = LTDC_SRCR_IMR;

#if defined(GX_DISABLE_THREADX_TIMER_SOURCE) || defined(GX_PING_PONG_FRAME_BUFFERS)
#ifdef GX_DISABLE_THREADX_TIMER_SOURCE
	/* Define the position of the line interrupt. */
	LTDC     -> LIPCR = 0;
	
	/* Enable line interrupt. */
	LTDC     -> IER |= LTDC_IER_LIE;
#endif

#ifdef GX_PING_PONG_FRAME_BUFFERS
	/* Enable reload interrupt. */
	LTDC     -> IER |= LTDC_IER_RRIE;
	
	tx_semaphore_create    (&register_reload_semaphore, "register reload semaphore", 0);
#endif

	/* Set USBHS Interrupt priority */
	HAL_NVIC_SetPriority    (LTDC_IRQn, 4, 0);
	
	/* Enable USBHS Interrupt */
	HAL_NVIC_EnableIRQ    (LTDC_IRQn);
#endif

	// clear the frame buffer
	memset    (frame_buffer, 0, STM32F429_DISCO_SCREEN_WIDTH * STM32F429_DISCO_SCREEN_HEIGHT * 2);
	
	// configure default DMA params
	DefaultDmaInitStruct    ();
	
	BSP_LCD_DisplayOn    ();
	
	// drive the timing pins high
	TIMING_FRAME_CALC_END
    TIMING_CPU_WORK_END

    /* Return successful status.  */
    return(GX_SUCCESS);
}

