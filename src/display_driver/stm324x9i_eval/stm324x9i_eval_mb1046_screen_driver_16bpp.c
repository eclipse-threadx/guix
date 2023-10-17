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
#include   "stm324x9i_eval_mb1046_screen_driver_16bpp.h"
#include   "stm324x9i_eval_lcd.h"


VOID stm32f429_touch_screen_init(VOID);

#define SDRAM _Pragma("location=\"SD_RAM\"")

SDRAM USHORT frame_buffer[272][480];


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    stm32f429_16bpp_buffer_toggle                       PORTABLE C      */
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
void stm32f429_16bpp_buffer_toggle(GX_CANVAS *canvas, GX_RECTANGLE *dirty)
{

GX_RECTANGLE    Limit;
GX_RECTANGLE    Copy;
INT             x;
INT             y;
USHORT*       pixel;


    USHORT *memptr = (USHORT*)canvas -> gx_canvas_memory;

    gx_utility_rectangle_define(&Limit, 0, 0,
                                STM32F429_DISCO_SCREEN_WIDTH - 1, STM32F429_DISCO_SCREEN_HEIGHT - 1);

    if (gx_utility_rectangle_overlap_detect(&Limit, &canvas->gx_canvas_dirty_area, &Copy))
    {

        for(y = Copy.gx_rectangle_top;  y <= Copy.gx_rectangle_bottom; y++)
        {


            pixel = memptr + y * STM32F429_DISCO_SCREEN_WIDTH;
            for(x = Copy.gx_rectangle_left; x <= Copy.gx_rectangle_right; x++)
            {
                frame_buffer[y][x] = pixel[x];
            }
        }
    }
}


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    stm32f429_graphics_driver_16bpp                     PORTABLE C      */
/*                                                           6.0          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function sets up the display driver data structure to support  */
/*    16bpp (565RGB) mode operation and the touch screen driver for the   */
/*    STM32F429I-DISCO board.                                             */
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
/*    _gx_display_driver_16bpp_setup        Generic 16bpp display driver  */
/*                                            setup                       */
/*    stm324xg_touch_screen_init            STM32F429 touch screen        */
/*                                            initialization routine      */
/*    LCD_Init                              STM32F429 LCD controller setup*/
/*    LTDC_LayerInit                        STM32F429 LCD layer setup     */
/*    LTDC_ReloadConfig                     STM32F429 LCD reload config   */
/*    LTDC_LayerCmd                         STM32F429 LCD layer config    */
/*    LTDC_DitherCmd                        STM32F429 LCD dither config   */
/*    LTDC_Cmd                              STM32F429 LCD config          */
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
UINT stm32f429_graphics_driver_16bpp(GX_DISPLAY *display)
{



    /* initialize the low-level drawing function pointers

       for windows, these are always just the generic funcions,
       but for some hardware, these will be customized,
       optimized functions specific to that hardware
    */
    _gx_display_driver_565rgb_setup(display, (VOID*)STM32F429_DISCO_SCREEN_HANDLE, stm32f429_16bpp_buffer_toggle);

    //stm32f429_touch_screen_init();

    LCD_Init();

    LCD_LayerInit();

    LCD_SetAddress((uint32_t)&frame_buffer);

    LCD_DisplayOn();
    
    /* Return successful status.  */
    return(GX_SUCCESS);
}

