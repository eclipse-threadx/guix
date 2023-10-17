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
/**   sama5 Screen Driver                                               */
/**                                                                       */
/**************************************************************************/


#include   "tx_api.h"
#include   "gx_api.h"
#include   "gx_system.h"
#include   "gx_display.h"

#include "board.h"
#include "sama5d2_screen_driver_32bpp.h"
#include "lcdd.h"

#define DDR_CS_ADDR    (0x20000000u) /**< DDR Chip Select base address */

#define LCD_BASE (DDR_CS_ADDR + 16*1024*1024)  /*   0x1000000  == 16M*/

/** LCD base image buffer  0x21000000  */
#define LCD_BUFFER_BASE    (LCD_BASE)

uint8_t *pBaseBuffer = (uint8_t *) LCD_BUFFER_BASE;
uint8_t *pGuixCanvas;
uint8_t *pScratchpad;

INT _gx_display_width = 800;
INT _gx_display_height = 480;

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    sama5_32bpp_buffer_toggle                         PORTABLE C      */
/*                                                           6.0          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function sends pixel data of the dirty area to the display     */
/*    device.                                                             */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    canvas                                The canvas to be displayed    */
/*    dirty                                 Dirty area to be refreshed    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    gx_utlity_rectangle_define            Define the rectangle area     */
/*    gx_utility_rectangle_overlap_detect   Detect whether or not two     */
/*                                            rectangle areas overlap     */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Logic                                                 */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*                                                                        */
/**************************************************************************/
void sama5d2_32bpp_buffer_toggle(GX_CANVAS *canvas, GX_RECTANGLE *dirty)
{
GX_RECTANGLE    Limit;
GX_RECTANGLE    Copy;
INT             column;
INT             row;
INT             width;
ULONG    *read_base;
ULONG    *write_base;
ULONG    *read_pixel;
ULONG    *write_pixel;
ULONG    offset;

ULONG *dma_start;
ULONG *dma_end;

    width = canvas->gx_canvas_x_resolution;

    gx_utility_rectangle_define(&Limit, 0, 0,
                                width - 1, canvas -> gx_canvas_y_resolution - 1);

    if (gx_utility_rectangle_overlap_detect(&Limit, &canvas->gx_canvas_dirty_area, &Copy))
    {
        offset = Copy.gx_rectangle_top * width;
        offset += Copy.gx_rectangle_left;
        read_base = canvas->gx_canvas_memory;
        write_base = (ULONG *) pBaseBuffer;
        
        // make the canvas memory the visible frame buffer
        
        read_base += offset;
        write_base += offset;

        dma_start = write_base;

        // copy from canvas memory to previously visible frame buffer
        for (row = Copy.gx_rectangle_top;  row <= Copy.gx_rectangle_bottom; row++)
        {
            read_pixel = read_base;
            write_pixel = write_base;
            
            for (column = Copy.gx_rectangle_left; column <= Copy.gx_rectangle_right; column++)
            {           
                *write_pixel++ = *read_pixel++;
            }
            read_base += width;
            write_base += width;
        }

        dma_end = write_base;
    }

    cp15_clean_dcache_for_dma((ULONG) dma_start, (ULONG) dma_end);
}


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    sama5d2_graphics_driver_setup_32bpp                 PORTABLE C      */
/*                                                           6.0          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function sets up the display driver data structure to support  */
/*    32bpp (24RGB unpacked mode) operation and the touch screen driver.  */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    display                               The display control block     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_display_driver_32bpp_setup        Generic 32bpp display driver  */
/*                                            setup                       */
/*    GLCD_Ctrl                             BSP level function to disable */
/*                                            LCD display                 */
/*    GLCD_Init                             BSP level LCD initialization  */
/*    GLCD_Cursor_Dis                       BSP level LCD Cursor disable  */
/*    TouchScrInit                          BSP level touch screen        */
/*                                            initialization              */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Logic                                                 */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*                                                                        */
/**************************************************************************/
UINT sama5d2_graphics_driver_setup_32bpp(GX_DISPLAY *display)
{
    UINT  status;
    ULONG buffer_size;
    status =  GX_SUCCESS;

    /* initialize the low-level drawing function pointers
       for windows, these are always just the generic funcions,
       but for some hardware, these will be customized,
       optimized functions specific to that hardware
    */

    buffer_size = _gx_display_width * _gx_display_height * 4;
    pGuixCanvas = pBaseBuffer + buffer_size;
    pScratchpad = pGuixCanvas + buffer_size;

	const struct _pin pins_lcd[] = BOARD_LCD_PINS;
	const struct _lcdd_desc lcd_desc = {
		.width = _gx_display_width,
		.height = _gx_display_height,
		.framerate = BOARD_LCD_FRAMERATE,
		.timing_vfp = BOARD_LCD_TIMING_VFP,
		.timing_vbp = BOARD_LCD_TIMING_VBP,
		.timing_vpw = BOARD_LCD_TIMING_VPW,
		.timing_hfp = BOARD_LCD_TIMING_HFP,
		.timing_hbp = BOARD_LCD_TIMING_HBP,
		.timing_hpw = BOARD_LCD_TIMING_HPW,
	};

    memset(pBaseBuffer, 0, buffer_size);
    memset(pGuixCanvas, 0, buffer_size);
     
	pio_configure(pins_lcd, ARRAY_SIZE(pins_lcd));
	lcdd_configure(&lcd_desc);
 	lcdd_on();
	//lcdd_set_backlight(bBackLight);

    lcdd_create_canvas(LCDD_BASE,
                      pBaseBuffer, 
                      32,
                      0, 
                      0, 
                      _gx_display_width, 
                      _gx_display_height);

    lcdd_stop_heo();
    lcdd_stop_ovr1();
    lcdd_show_base(pBaseBuffer, 32, GX_FALSE);

    _gx_display_driver_24xrgb_setup(display, (VOID*)SAMA5_SCREEN_HANDLE, sama5d2_32bpp_buffer_toggle);

    /* Return successful status.  */
    return status;
}


