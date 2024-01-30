/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation 
 * 
 * This program and the accompanying materials are made available under the
 * terms of the MIT License which is available at
 * https://opensource.org/licenses/MIT.
 * 
 * SPDX-License-Identifier: MIT
 **************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** GUIX Component                                                        */
/**                                                                       */
/**    Screen Driver                                              		  */
/**                                                                       */
/**************************************************************************/


#include   "tx_api.h"
#include   "gx_api.h"
#include   "gx_system.h"
#include   "gx_display.h"
#include   "gx_utility.h"
#include   "gx_display_driver_hardware.h"
#if __ARM_FEATURE_MVE & 1 
#include <arm_mve.h>
#endif  

/* turn this on to enable Chrome Art Accelerator. Comment out to disable */
//#define GX_CHROMEART_ENABLE     // comment out to disable using accelerator

/* turn this on to enable timing via IO pins */
#define GX_TIMING_ENABLE        // comment out to disable IO pin timing

#define CR_MASK                     ((uint32_t)0xFFFCE0FC)  /* DMA2D CR Mask */
#define PFCCR_MASK                  ((uint32_t)0x00FC00C0)  /* DMA2D FGPFCCR Mask */
#define DEAD_MASK                   ((uint32_t)0xFFFF00FE)  /* DMA2D DEAD Mask */

#define SDRAM _Pragma("location=\"SD_RAM\"")

SDRAM static ULONG frame_buffer[BOARD_SCREEN_WIDTH][BOARD_SCREEN_HEIGHT];

static DMA2D_HandleTypeDef    Dma2dHandle;
static TX_SEMAPHORE           transfer_complete_semaphore;

static UINT    (*dma_wait_function)(void *) = GX_NULL;
static VOID    *wait_func_param;

#if defined(GX_TIMING_ENABLE)
    #ifdef STM32H743I_EVAL
        #define TIMING_FRAME_CALC_BEGIN  GPIOG->BSRRL=GPIO_PIN_6;
        #define TIMING_FRAME_CALC_END    GPIOG->BSRRH=GPIO_PIN_6;
        #define TIMING_CPU_WORK_BEGIN    GPIOG->BSRRL=GPIO_PIN_7;
        #define TIMING_CPU_WORK_END      GPIOG->BSRRH=GPIO_PIN_7;
    #else
        #define TIMING_FRAME_CALC_BEGIN  GPIOG->BSRR=GPIO_PIN_6;
        #define TIMING_FRAME_CALC_END    GPIOG->BSRR=GPIO_PIN_6<<16;
        #define TIMING_CPU_WORK_BEGIN    GPIOG->BSRR=GPIO_PIN_7;
        #define TIMING_CPU_WORK_END      GPIOG->BSRR=GPIO_PIN_7<<16;
    #endif
#else
    #define TIMING_FRAME_CALC_BEGIN  
    #define TIMING_FRAME_CALC_END    
    #define TIMING_CPU_WORK_BEGIN    
    #define TIMING_CPU_WORK_END     
#endif

VOID gx_display_wait_function_set(UINT(*wait_func)(void *), void *param)
{
    dma_wait_function = wait_func;
	wait_func_param = param;
}

VOID DMA2D_IRQHandler(VOID)
{
    if(DMA2D->ISR & DMA2D_ISR_TCIF)
    {
        tx_thread_resume(&_gx_system_thread);
        
        /* clear the status flag */  
        DMA2D->IFCR = DMA2D_IFCR_CTCIF;
        
        tx_semaphore_put(&transfer_complete_semaphore);
    }
}

/**************************************************************************/
static void DefaultDmaInitStruct()  
{
    /* Configure DMA2D */    
    Dma2dHandle.Instance = DMA2D; 
    HAL_DMA2D_DeInit(&Dma2dHandle);  
    Dma2dHandle.Init.ColorMode = DMA2D_ARGB8888;      

    /* Configures the color mode of the output image */
    DMA2D->OPFCCR &= ~(uint32_t)DMA2D_OPFCCR_CM;
    DMA2D->OPFCCR |= DMA2D_ARGB8888;
    
    /* Enable Transfer complete interrupt. */
    DMA2D->CR |= DMA2D_CR_TCIE;

}  

/**************************************************************************/
static void gx_dma_init(uint32_t Mode, uint32_t OutputMemoryAdd, uint32_t OutputOffset, uint32_t PixelPerLine, uint32_t NumberOfLine)
{
    uint32_t pixline  = 0;

    /* Configures the DMA2D operation mode */
    DMA2D->CR &= (uint32_t)(~DMA2D_CR_MODE_Msk);
    DMA2D->CR |= Mode;

    /* Configures the output memory address */
    DMA2D->OMAR = OutputMemoryAdd;

    /* Configure  the line Offset */
    DMA2D->OOR &= ~(uint32_t)DMA2D_OOR_LO;
    DMA2D->OOR |= (OutputOffset);

    /* Configure the number of line and pixel per line */
    pixline = PixelPerLine << 16; 
    DMA2D->NLR &= ~(DMA2D_NLR_NL | DMA2D_NLR_PL);
    DMA2D->NLR |= ((NumberOfLine) | (pixline));
}

/**************************************************************************/
static void gx_dma_run()
{
    SCB_CleanInvalidateDCache();
    
    if (dma_wait_function == GX_NULL)
    {
        TIMING_CPU_WORK_END     // cpu work is done
    }

    /* tell the engine to start */
    DMA2D->CR |= (uint32_t)DMA2D_CR_START;

    if (dma_wait_function)
    {
        dma_wait_function((GX_CIRCULAR_GAUGE *)wait_func_param);
        dma_wait_function = GX_NULL;
        TIMING_CPU_WORK_END     // cpu work is done
    }
    
    /* Wait for transfer complete interrupt. */
    tx_semaphore_get(&transfer_complete_semaphore, TX_WAIT_FOREVER);

    TIMING_CPU_WORK_BEGIN // back to doing cpu work
}
  
/**************************************************************************/
static VOID gx_chromeart_horizontal_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT xend, INT ypos, INT width, GX_COLOR color)
{
    uint32_t  put;
    int length;
    GX_CANVAS *canvas = context->gx_draw_context_canvas;

    put = (uint32_t) canvas->gx_canvas_memory;
    put += (canvas->gx_canvas_x_resolution * 4 * ypos) + (xstart * 4);              

    length = xend - xstart + 1;

    if(length <= 0)
    {
        return;
    }
       
    gx_dma_init(DMA2D_R2M, put, canvas->gx_canvas_x_resolution - length, length, width);  
    DMA2D->OCOLR = color;

    gx_dma_run();
}

/**************************************************************************/
static VOID gx_chromeart_vertical_line_draw(GX_DRAW_CONTEXT *context, INT ystart, INT yend, INT xpos, INT width, GX_COLOR color)
{
    uint32_t  put;
    GX_CANVAS *canvas = context->gx_draw_context_canvas;
  
    put = (uint32_t) canvas->gx_canvas_memory;

    put += (canvas->gx_canvas_x_resolution * 4 * ystart) + (xpos * 4);

    gx_dma_init(DMA2D_R2M, put, canvas->gx_canvas_x_resolution - width, width, yend - ystart + 1);  
    DMA2D->OCOLR = color;
    gx_dma_run();
}

/**************************************************************************/
static void gx_chromeart_canvas_copy(GX_CANVAS *canvas, GX_CANVAS *composite)
{
GX_RECTANGLE dirty;
GX_RECTANGLE overlap;
ULONG      *read;
ULONG      *write;
INT          width;

    dirty.gx_rectangle_left = dirty.gx_rectangle_top = 0;
    dirty.gx_rectangle_right = canvas -> gx_canvas_x_resolution - 1;
    dirty.gx_rectangle_bottom = canvas -> gx_canvas_y_resolution - 1;

    _gx_utility_rectangle_shift(&dirty, canvas -> gx_canvas_display_offset_x, canvas -> gx_canvas_display_offset_y);

    if (_gx_utility_rectangle_overlap_detect(&dirty, &composite -> gx_canvas_dirty_area, &overlap))
    {
        width = overlap.gx_rectangle_right - overlap.gx_rectangle_left + 1;
        read = (ULONG *)canvas -> gx_canvas_memory;

        /* index into starting row */
        read += (overlap.gx_rectangle_top - dirty.gx_rectangle_top) * canvas -> gx_canvas_x_resolution;

        /* index into pixel */

        read += overlap.gx_rectangle_left - dirty.gx_rectangle_left;

        /* calculate the write pointer */
        write = (ULONG *)composite -> gx_canvas_memory;
        write += overlap.gx_rectangle_top * composite -> gx_canvas_x_resolution;
        write += overlap.gx_rectangle_left;

        gx_dma_init(DMA2D_M2M, (uint32_t) write, BOARD_SCREEN_WIDTH - width, width, overlap.gx_rectangle_bottom - overlap.gx_rectangle_top + 1);  

        DMA2D->FGPFCCR = (0xff000000) | DMA2D_ARGB8888;  // foreground format conversion
        DMA2D->FGMAR = (uint32_t) read;            // foreground memory address
        DMA2D->FGOR = canvas->gx_canvas_x_resolution - width;  // foreground offset
        gx_dma_run();
    }
}

/**************************************************************************/
/*  gx_chromeart_alphamap_draw                                            */ 
/*    Override generic function to utilize DMA2D                          */
/**************************************************************************/
static void gx_chromeart_alphamap_draw(GX_DRAW_CONTEXT *context,
                                INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT           width;
INT           height;
ULONG        *putrow;
GX_UBYTE     *getrow;
GX_CANVAS    *canvas;
GX_COLOR      text_color;
GX_UBYTE      alpha;
ULONG         pfc_reg;

    text_color =  context -> gx_draw_context_brush.gx_brush_fill_color;
    alpha = context ->gx_draw_context_brush.gx_brush_alpha;

    if (alpha == 0)
    {
        return;
    }
    
    /* if the pixelmap is compressed, we must draw
       in software
    */
    if (pixelmap->gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
    {
        _gx_display_driver_generic_alphamap_draw(context, xpos, ypos, pixelmap);
        return;
    }

    /* the pixelmap is raw, use ChromeArt */
    GX_RECTANGLE *clip = context -> gx_draw_context_clip;
    canvas = context->gx_draw_context_canvas;

    putrow = (ULONG *)context -> gx_draw_context_memory;
    putrow += clip -> gx_rectangle_top * context->gx_draw_context_pitch;
    putrow += clip -> gx_rectangle_left;

    getrow = (GX_UBYTE *)(pixelmap -> gx_pixelmap_data);
    getrow += pixelmap -> gx_pixelmap_width * (clip -> gx_rectangle_top - ypos);
    getrow += (clip -> gx_rectangle_left - xpos);

    width = clip->gx_rectangle_right - clip -> gx_rectangle_left + 1;
    height = clip->gx_rectangle_bottom - clip->gx_rectangle_top + 1;
   
    gx_dma_init(DMA2D_M2M_BLEND, (uint32_t) putrow, canvas->gx_canvas_x_resolution - width, width, height);  

    DMA2D->FGMAR = (uint32_t) getrow;           // foreground memory address
    DMA2D->FGOR = pixelmap->gx_pixelmap_width - width;  // foreground offset
    pfc_reg = alpha;
    pfc_reg <<= 24;
    pfc_reg |= 0x00020000;
    pfc_reg |= DMA2D_INPUT_A8;

    DMA2D->FGPFCCR = pfc_reg;                        // A8 PFC, blend the alphas
    DMA2D->FGCOLR = text_color;

    DMA2D->BGMAR = (uint32_t) putrow;          // background memory address
    DMA2D->BGOR = canvas->gx_canvas_x_resolution - width;  // background offset
    DMA2D->BGPFCCR = 0xff010000;

    gx_dma_run();
}

/**************************************************************************/
static void gx_chromeart_pixelmap_draw(GX_DRAW_CONTEXT *context,
                                INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
INT           width;
INT           height;
ULONG        *putrow;
ULONG        *getrow;
GX_CANVAS    *canvas;
uint32_t      DMA2D_Mode;


    /* if the pixelmap is compressed, we must draw
       in software
    */
    if (pixelmap->gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
    {
        _gx_display_driver_24xrgb_pixelmap_draw(context, xpos, ypos, pixelmap);
        return;
    }

    /* the pixelmap is raw, use ChromeArt */
    GX_RECTANGLE *clip = context -> gx_draw_context_clip;
    canvas = context->gx_draw_context_canvas;

    putrow = (ULONG *)context -> gx_draw_context_memory;
    putrow += clip -> gx_rectangle_top * context->gx_draw_context_pitch;
    putrow += clip -> gx_rectangle_left;

    getrow = (ULONG *)(pixelmap -> gx_pixelmap_data);
    getrow += pixelmap -> gx_pixelmap_width * (clip -> gx_rectangle_top - ypos);
    getrow += (clip -> gx_rectangle_left - xpos);

    width = clip->gx_rectangle_right - clip -> gx_rectangle_left + 1;
    height = clip->gx_rectangle_bottom - clip->gx_rectangle_top + 1;

    if (pixelmap->gx_pixelmap_flags & GX_PIXELMAP_ALPHA)
    {
        DMA2D->FGPFCCR = (0xff000000) | DMA2D_ARGB8888;  // foreground format conversion
        DMA2D_Mode = DMA2D_M2M_BLEND;       
    }
    else
    {
        DMA2D->FGPFCCR = (0xff010000) | DMA2D_ARGB8888;  // foreground format conversion
        DMA2D_Mode = DMA2D_M2M;       
    }
    gx_dma_init(DMA2D_Mode, (uint32_t) putrow, canvas->gx_canvas_x_resolution - width, width, height);  

    DMA2D->FGMAR = (uint32_t) getrow;             // foreground memory address
    DMA2D->FGOR = pixelmap->gx_pixelmap_width - width;  // foreground offset

    if (pixelmap->gx_pixelmap_flags & GX_PIXELMAP_ALPHA)
    {
        DMA2D->BGMAR = (uint32_t) putrow;          // background memory address
        DMA2D->BGOR = canvas->gx_canvas_x_resolution - width;  // background offset
        DMA2D->BGPFCCR = 0xff010000;
    }
    gx_dma_run();
}

/**************************************************************************/
static void gx_chromeart_pixelmap_blend(GX_DRAW_CONTEXT *context,
                                INT xpos, INT ypos, GX_PIXELMAP *pixelmap, UCHAR alpha)
{
INT           width;
INT           height;
ULONG        *putrow;
ULONG        *getrow;
GX_CANVAS    *canvas;
ULONG        blend_reg;

    /* if the pixelmap is compressed, we must draw
       in software
    */
    if (pixelmap->gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
    {
        _gx_display_driver_24xrgb_pixelmap_blend(context, xpos, ypos, pixelmap, alpha);
        return;
    }

    /* the pixelmap is raw, use ChromeArt */
    GX_RECTANGLE *clip = context -> gx_draw_context_clip;
    canvas = context->gx_draw_context_canvas;

    putrow = (ULONG *)context -> gx_draw_context_memory;
    putrow += clip -> gx_rectangle_top * context->gx_draw_context_pitch;
    putrow += clip -> gx_rectangle_left;

    getrow = (ULONG *)(pixelmap -> gx_pixelmap_data);
    getrow += pixelmap -> gx_pixelmap_width * (clip -> gx_rectangle_top - ypos);
    getrow += (clip -> gx_rectangle_left - xpos);

    width = clip->gx_rectangle_right - clip -> gx_rectangle_left + 1;
    height = clip->gx_rectangle_bottom - clip->gx_rectangle_top + 1;

    gx_dma_init(DMA2D_M2M_BLEND, (uint32_t) putrow, canvas->gx_canvas_x_resolution - width, width, height);  

    blend_reg = alpha;
    blend_reg <<= 24;

    if (pixelmap->gx_pixelmap_flags & GX_PIXELMAP_ALPHA)
    {
        // blend original alpha with alpha
        blend_reg |= 0x00020000 | DMA2D_ARGB8888;
    }
    else
    {
        // replace alpha
        blend_reg |= 0x00010000 | DMA2D_ARGB8888;
    }
    DMA2D->FGPFCCR = blend_reg;  // foreground format conversion
    DMA2D->FGMAR = (uint32_t) getrow;             // foreground memory address
    DMA2D->FGOR = pixelmap->gx_pixelmap_width - width;  // foreground offset

    DMA2D->BGMAR = (uint32_t) putrow;          // background memory address
    DMA2D->BGOR = canvas->gx_canvas_x_resolution - width;  // background offset
    DMA2D->BGPFCCR = 0xff010000;

    gx_dma_run();
}

/**************************************************************************/
static void gx_chromeart_glyph_8bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area,
        GX_POINT *map_offset, const GX_GLYPH *glyph)
{
GX_CANVAS  *canvas;
GX_UBYTE   *read_glyph;
ULONG      *write_row;
GX_COLOR    text_color;
UINT        height;
UINT        width;
GX_UBYTE    alpha;
ULONG    pfc_reg;

    text_color =  context -> gx_draw_context_brush.gx_brush_line_color;
    alpha = context ->gx_draw_context_brush.gx_brush_alpha;

    if (alpha == 0)
    {
        return;
    }

    width = draw_area -> gx_rectangle_right - draw_area -> gx_rectangle_left + 1;

    /* pickup pointer to current display driver */
    canvas = context->gx_draw_context_canvas;

    read_glyph = (GX_UBYTE *)glyph -> gx_glyph_map;

    if (map_offset -> gx_point_y)
    {
        read_glyph = read_glyph + (glyph -> gx_glyph_width * map_offset -> gx_point_y);
    }

    read_glyph += map_offset -> gx_point_x;

    width = draw_area->gx_rectangle_right - draw_area->gx_rectangle_left + 1;
    height = draw_area -> gx_rectangle_bottom - draw_area -> gx_rectangle_top + 1;

    write_row = (ULONG *) canvas->gx_canvas_memory;
    write_row += draw_area->gx_rectangle_top * canvas ->gx_canvas_x_resolution;
    write_row += draw_area->gx_rectangle_left;
   
    gx_dma_init(DMA2D_M2M_BLEND, (uint32_t) write_row, canvas->gx_canvas_x_resolution - width, width, height);  

    DMA2D->FGMAR = (uint32_t) read_glyph;           // foreground memory address
    DMA2D->FGOR = glyph->gx_glyph_width - width;  // foreground offset
    pfc_reg = alpha;
    pfc_reg <<= 24;
    pfc_reg |= 0x00020009;

    DMA2D->FGPFCCR = pfc_reg;                        // A8 PFC, blend the alphas
    DMA2D->FGCOLR = text_color;

    DMA2D->BGMAR = (uint32_t) write_row;          // background memory address
    DMA2D->BGOR = canvas->gx_canvas_x_resolution - width;  // background offset
    DMA2D->BGPFCCR = 0xff010000;

    gx_dma_run();
}

static VOID gx_chromeart_drawing_initiate(GX_DISPLAY *display, GX_CANVAS *canvas)
{
    TIMING_FRAME_CALC_BEGIN
    TIMING_CPU_WORK_BEGIN
}

static VOID gx_chromeart_drawing_complete(GX_DISPLAY *display, GX_CANVAS *canvas)
{
    TIMING_FRAME_CALC_END
    TIMING_CPU_WORK_END
}


/**************************************************************************/
static void hardware_24xrgb_buffer_toggle(GX_CANVAS *canvas, GX_RECTANGLE *dirty)
{
GX_RECTANGLE    Limit;
GX_RECTANGLE    Copy;
ULONG           offset;
INT             copy_width;
INT             copy_height;

#if !defined(GX_CHROMEART_ENABLE)
INT             row;
INT             src_stride_ulongs;
INT             dest_stride_ulongs;
#endif

ULONG *get;
ULONG *put;

    gx_utility_rectangle_define(&Limit, 0, 0,
                                canvas->gx_canvas_x_resolution - 1,
                                canvas->gx_canvas_y_resolution - 1);

    if (gx_utility_rectangle_overlap_detect(&Limit, &canvas->gx_canvas_dirty_area, &Copy))
    {
        copy_width = Copy.gx_rectangle_right - Copy.gx_rectangle_left + 1;
        copy_height = Copy.gx_rectangle_bottom - Copy.gx_rectangle_top + 1;

        offset = Copy.gx_rectangle_top * canvas->gx_canvas_x_resolution;
        offset += Copy.gx_rectangle_left;
        get = canvas ->gx_canvas_memory;
        get += offset;

        // offset by dirty rect pos
        put = (ULONG *) frame_buffer;

        // offset by canvas offset:
        offset = (canvas->gx_canvas_display_offset_y + Copy.gx_rectangle_top)* BOARD_SCREEN_WIDTH;
        offset += canvas->gx_canvas_display_offset_x + Copy.gx_rectangle_left;
        put += offset;
        

        #if defined(GX_CHROMEART_ENABLE)
        gx_dma_init(DMA2D_M2M_PFC, (uint32_t) put, BOARD_SCREEN_WIDTH - copy_width, copy_width, copy_height);  

        DMA2D->FGPFCCR = (0xff010000) | DMA2D_ARGB8888;  // foreground format conversion
        DMA2D->FGMAR = (uint32_t) get;            // foreground memory address
        DMA2D->FGOR = canvas->gx_canvas_x_resolution - copy_width;  // foreground offset
        gx_dma_run();
        
        #else
        src_stride_ulongs = canvas ->gx_canvas_x_resolution;
        dest_stride_ulongs = BOARD_SCREEN_WIDTH;

        for(row = 0; row < copy_height; row++)
        {
            memcpy(put, get, copy_width * 4);
            put += dest_stride_ulongs;
            get += src_stride_ulongs;
        }
        #endif
    }
    TIMING_CPU_WORK_END
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
UINT hardware_graphics_driver_setup_24xrgb(GX_DISPLAY *display)
{
    /* initialize the low-level drawing function pointers

       for windows, these are always just the generic funcions,
       but for some hardware, these will be customized,
       optimized functions specific to that hardware
    */
    _gx_display_driver_24xrgb_setup(display, (VOID*)BOARD_SCREEN_HANDLE, hardware_24xrgb_buffer_toggle);

    #if defined(GX_TIMING_ENABLE)
    display->gx_display_driver_drawing_initiate     = gx_chromeart_drawing_initiate;
    display->gx_display_driver_drawing_complete     = gx_chromeart_drawing_complete;
    #endif


    #if defined(GX_CHROMEART_ENABLE)
    /* override those functions that can be accelerated with DMA2D */
    display->gx_display_driver_horizontal_line_draw = gx_chromeart_horizontal_line_draw;
    display -> gx_display_driver_vertical_line_draw = gx_chromeart_vertical_line_draw;
    display -> gx_display_driver_canvas_copy        = gx_chromeart_canvas_copy;
    display -> gx_display_driver_alphamap_draw      = gx_chromeart_alphamap_draw;
    display -> gx_display_driver_pixelmap_draw      = gx_chromeart_pixelmap_draw;
    display -> gx_display_driver_pixelmap_blend     = gx_chromeart_pixelmap_blend;
    display -> gx_display_driver_8bit_glyph_draw    = gx_chromeart_glyph_8bit_draw;
    //display -> gx_display_driver_callback_assign    = gx_display_wait_function_set;


    //display -> gx_display_driver_canvas_blend                  = _gx_display_driver_24xrgb_canvas_blend;
    //display -> gx_display_driver_4bit_glyph_draw               =  _gx_display_driver_generic_glyph_4bit_draw;
    //display -> gx_display_driver_1bit_glyph_draw               =  _gx_display_driver_24bpp_glyph_1bit_draw;

    #endif
    return 0;

    /* Hardware Initialization */
    BSP_LCD_Init();

    BSP_LCD_LayerDefaultInit(0, (uint32_t)&frame_buffer);
    BSP_LCD_SelectLayer(0);


    // clear the frame buffer
    memset(frame_buffer, 0, BOARD_SCREEN_WIDTH * BOARD_SCREEN_HEIGHT * 4);

    /* Set DAM2D Interrupt priority */
    HAL_NVIC_SetPriority(DMA2D_IRQn, 4, 0);
    
    /* Enable DAM2D Interrupt */
    HAL_NVIC_EnableIRQ(DMA2D_IRQn);
    
    tx_semaphore_create(&transfer_complete_semaphore, "dma2d transfer complete semaphore", 0);
    
    // configure default DMA params
    DefaultDmaInitStruct();

    BSP_LCD_DisplayOn();
    
    // drive the timing pins high
    TIMING_FRAME_CALC_END
    TIMING_CPU_WORK_END

    /* Return successful status.  */
    return(GX_SUCCESS);
}

