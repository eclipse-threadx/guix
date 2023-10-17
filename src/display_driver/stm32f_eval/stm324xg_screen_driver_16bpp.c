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
#include   "stm324xg_eval_lcd.h"
#include   "stm324xg_screen.h"
#include   "stm324xg_eval_ioe.h"


extern VOID stm324xg_touch_screen_init(VOID);


void stm324xg_16bpp_buffer_toggle(GX_CANVAS *canvas, GX_RECTANGLE *dirty)
{

GX_RECTANGLE    Limit;
GX_RECTANGLE    Copy;
INT             x;
INT             y;
USHORT*         pixel;


    USHORT *memptr = (USHORT*)canvas -> gx_canvas_memory;

    gx_utility_rectangle_define(&Limit, 0, 0,
                                STM324XG_SCREEN_WIDTH - 1, STM324XG_SCREEN_HEIGHT - 1);

    if (gx_utility_rectangle_overlap_detect(&Limit, &canvas->gx_canvas_dirty_area, &Copy))
    {

        for(y = Copy.gx_rectangle_top;  y <= Copy.gx_rectangle_bottom; y++)
        {

            LCD_SetCursor(y, STM324XG_SCREEN_WIDTH - 1 - Copy.gx_rectangle_left);
            LCD_WriteRAM_Prepare();
            pixel = memptr + y * STM324XG_SCREEN_WIDTH;
            for(x = Copy.gx_rectangle_left; x <= Copy.gx_rectangle_right; x++)
            {
                LCD_WriteRAM(*(pixel + x));
            }
        }
    }
}


UINT stm324xg_graphics_driver_565rgb_setup(GX_DISPLAY *display)
{
    /* initialize the low-level drawing function pointers

       for windows, these are always just the generic funcions,
       but for some hardware, these will be customized,
       optimized functions specific to that hardware
    */

    _gx_display_driver_565rgb_setup(display, (VOID*)STM324XG_SCREEN_HANDLE, stm324xg_16bpp_buffer_toggle);
    
    display -> gx_display_handle                        = STM324XG_SCREEN_HANDLE;

    stm324xg_touch_screen_init();

    STM324xG_LCD_Init();

    /* Return successful status.  */
    return(GX_SUCCESS);
}

