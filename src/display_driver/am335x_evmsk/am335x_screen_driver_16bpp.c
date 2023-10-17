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
/**   AM335x Screen Driver                                                */
/**                                                                       */
/**************************************************************************/

#include "am335x_screen.h"
#include "raster.h"


ULONG frame_buffer[AM335X_SCREEN_WIDTH * AM335X_SCREEN_HEIGHT];


void am335x_16bpp_buffer_toggle(GX_CANVAS *canvas, GX_RECTANGLE *dirty)
{
    GX_RECTANGLE Limit;
    GX_RECTANGLE Copy;
    int x,y;
    GX_COLOR* pixel;

    UCHAR r,g,b;
    
    ULONG* frame_buffer_ptr;

    GX_COLOR *memptr = canvas -> gx_canvas_memory;
        
    gx_utility_rectangle_define(&Limit, 0, 0,
        AM335X_SCREEN_WIDTH - 1, AM335X_SCREEN_HEIGHT - 1);
    
    if (gx_utility_rectangle_overlap_detect(&Limit, &canvas->gx_canvas_dirty_area, &Copy))
    {

        for(y = Copy.gx_rectangle_top;  y <= Copy.gx_rectangle_bottom; y++)
        {

            frame_buffer_ptr = &frame_buffer[y * AM335X_SCREEN_WIDTH + Copy.gx_rectangle_left];
            
            pixel = memptr + y * AM335X_SCREEN_WIDTH;
            for(x = Copy.gx_rectangle_left; x <= Copy.gx_rectangle_right; x++)
            {
                r = REDVAL_16BPP(*(pixel + x));
                g = GREENVAL_16BPP(*(pixel + x));
                b = BLUEVAL_16BPP(*(pixel + x));
                
                r = (r << 3) | (r >> 2);
                g = (g << 2) | (g >> 3);
                b = (b << 3) | (b >> 2);
                
                *frame_buffer_ptr++ = (r << 16) | (g << 8) | b;
            }
        }
    }
#ifdef ENABLE_CACHE
    CacheDataCleanBuff((ULONG)&frame_buffer[0], sizeof(frame_buffer));
#endif /* ENABLE_CACHE */
}

/*
** Configures raster to display image 
*/
static void SetUpLCD(void)
{
    /* Enable clock for LCD Module */ 
    LCDModuleClkConfig();

    LCDPinMuxSetup();

    /* 
    **Clock for DMA,LIDD and for Core(which encompasses
    ** Raster Active Matrix and Passive Matrix logic) 
    ** enabled.
    */
    RasterClocksEnable(SOC_LCDC_0_REGS);

    /* Disable raster */
    RasterDisable(SOC_LCDC_0_REGS);
    
    /* Configure the pclk */
    RasterClkConfig(SOC_LCDC_0_REGS, 23040000, 192000000);

    /* Configuring DMA of LCD controller */ 
    RasterDMAConfig(SOC_LCDC_0_REGS, RASTER_DOUBLE_FRAME_BUFFER,
                    RASTER_BURST_SIZE_16, RASTER_FIFO_THRESHOLD_8,
                    RASTER_BIG_ENDIAN_DISABLE);

    /* Configuring modes(ex:tft or stn,color or monochrome etc) for raster controller */
    RasterModeConfig(SOC_LCDC_0_REGS, RASTER_DISPLAY_MODE_TFT_UNPACKED,
                     RASTER_PALETTE_DATA, RASTER_COLOR, RASTER_RIGHT_ALIGNED);


     /* Configuring the polarity of timing parameters of raster controller */
    RasterTiming2Configure(SOC_LCDC_0_REGS, RASTER_FRAME_CLOCK_LOW |
                                            RASTER_LINE_CLOCK_LOW  |
                                            RASTER_PIXEL_CLOCK_HIGH|
                                            RASTER_SYNC_EDGE_RISING|
                                            RASTER_SYNC_CTRL_ACTIVE|
                                            RASTER_AC_BIAS_HIGH     , 0, 255);

#if 0
    /* Configuring horizontal timing parameter */
    RasterHparamConfig(SOC_LCDC_0_REGS, 800, 48, 40, 40);

    /* Configuring vertical timing parameters */
    RasterVparamConfig(SOC_LCDC_0_REGS, 480, 3, 13, 29);
#else
    /* Configuring horizontal timing parameter */
    RasterHparamConfig(SOC_LCDC_0_REGS, 480, 4, 8, 43);

    /* Configuring vertical timing parameters */
    RasterVparamConfig(SOC_LCDC_0_REGS, 272, 10, 4, 12);

#endif

    RasterFIFODMADelayConfig(SOC_LCDC_0_REGS, 128);

}




UINT am335x_graphics_driver_16bpp(GX_DISPLAY *display)
{
    /* initialize the low-level drawing function pointers

       for windows, these are always just the generic funcions,
       but for some hardware, these will be customized,
       optimized functions specific to that hardware
    */
    
    display -> gx_display_color_format                  = GX_COLOR_FORMAT_16BIT_RGB;
    display -> gx_display_driver_canvas_copy            = _gx_display_driver_16bpp_canvas_copy;
    display -> gx_display_driver_canvas_blend           = _gx_display_driver_16bpp_canvas_blend;

    display -> gx_display_driver_simple_line_draw      = _gx_display_driver_generic_simple_line_draw;
    display -> gx_display_driver_simple_wide_line_draw  = _gx_display_driver_generic_simple_wide_line_draw;
    display -> gx_display_driver_anti_aliased_line_draw = _gx_display_driver_generic_aliased_line_draw;
    display -> gx_display_driver_anti_aliased_wide_line_draw  = _gx_display_driver_generic_aliased_wide_line_draw;

    display -> gx_display_driver_horizontal_line_draw   = _gx_display_driver_16bpp_horizontal_line_draw;
    display -> gx_display_driver_vertical_line_draw     = _gx_display_driver_16bpp_vertical_line_draw;
    display -> gx_display_driver_horizontal_pattern_line_draw   = _gx_display_driver_16bpp_horizontal_pattern_line_draw;
    display -> gx_display_driver_vertical_pattern_line_draw     = _gx_display_driver_16bpp_vertical_pattern_line_draw;
    display -> gx_display_driver_pixelmap_draw          = _gx_display_driver_16bpp_pixelmap_draw;
    display -> gx_display_driver_polygon_draw           = _gx_display_driver_16bpp_polygon_draw;

    display -> gx_display_driver_pixel_write            = _gx_display_driver_16bpp_pixel_write;
    display -> gx_display_driver_pixel_blend            = _gx_display_driver_16bpp_pixel_blend;
    display -> gx_display_driver_block_move             = _gx_display_driver_16bpp_block_move;
    display -> gx_display_driver_native_color_get       = _gx_display_driver_16bpp_native_color_get;

    display -> gx_screen_driver_buffer_toggle           = am335x_16bpp_buffer_toggle;
    display -> gx_display_handle                        = AM335X_SCREEN_HANDLE;
    display -> gx_display_driver_data                   = (void*)AM335X_SCREEN_HANDLE;
    
#if 1
    SetUpLCD();
    
#if 1    

        /* Configuring the base ceiling */
    RasterDMAFBConfig(SOC_LCDC_0_REGS, 
                      (unsigned int)frame_buffer - 32,
                      (unsigned int)frame_buffer + sizeof(frame_buffer) - 2 - 32,
                      0);

    RasterDMAFBConfig(SOC_LCDC_0_REGS, 
                      (unsigned int)frame_buffer - 32,
                      (unsigned int)frame_buffer + sizeof(frame_buffer) - 2 - 32,
                      1);
#else

    RasterDMAFBConfig(SOC_LCDC_0_REGS, 
                      (unsigned int)image1,
                      (unsigned int)image1 + sizeof(image1) -  2,
                      0);

    RasterDMAFBConfig(SOC_LCDC_0_REGS, 
                      (unsigned int)image1,
                      (unsigned int)image1 + sizeof(image1) - 2,
                      1);


#endif
    /* Enable raster */
    RasterEnable(SOC_LCDC_0_REGS);
    
    LCDBackLightEnable();
#endif
    /* Return successful status.  */
    return(GX_SUCCESS);
}

