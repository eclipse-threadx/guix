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
/**   LPC40XX Screen Driver                                               */
/**                                                                       */
/**************************************************************************/


#include   "tx_api.h"
#include   "gx_api.h"
#include   "gx_system.h"
#include   "gx_display.h"

#include   "lpc40xx_screen_driver_32bpp.h"
#include   "drv_glcd.h"
#include   "LPC407x_8x.h"

static GX_EVENT         eventpad;

extern uint32_t SDRAM_VRAM_BASE_ADDR;
extern void TouchScrInit(void);
#define LCD_VRAM_BASE_ADDR ((uint32_t)&SDRAM_VRAM_BASE_ADDR)
ULONG * frame_buffer = (ULONG *)LCD_VRAM_BASE_ADDR;



/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    lpc40xx_32bpp_buffer_toggle                         PORTABLE C      */
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
void lpc40xx_32bpp_buffer_toggle(GX_CANVAS *canvas, GX_RECTANGLE *dirty)
{

GX_RECTANGLE    Limit;
GX_RECTANGLE    Copy;
INT             x;
INT             y;
ULONG*       pixel;
ULONG* frame_buffer_ptr;


    ULONG *memptr = (ULONG*)canvas -> gx_canvas_memory;

    gx_utility_rectangle_define(&Limit, 0, 0,
                                LPC40XX_SCREEN_WIDTH - 1, LPC40XX_SCREEN_HEIGHT - 1);

    if (gx_utility_rectangle_overlap_detect(&Limit, &canvas->gx_canvas_dirty_area, &Copy))
    {
        for(y = Copy.gx_rectangle_top;  y <= Copy.gx_rectangle_bottom; y++)
        {

            frame_buffer_ptr = &frame_buffer[y * LPC40XX_SCREEN_WIDTH + Copy.gx_rectangle_left];
            
            pixel = memptr + y * LPC40XX_SCREEN_WIDTH;
            for(x = Copy.gx_rectangle_left; x <= Copy.gx_rectangle_right; x++)
            {           
                *frame_buffer_ptr++ = *(pixel + x);
            }
        }
    }
    
}


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    lpc40xx_graphics_driver_setup_32bpp                 PORTABLE C      */
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
UINT lpc40xx_graphics_driver_setup_32bpp(GX_DISPLAY *display)
{
    /* initialize the low-level drawing function pointers

       for windows, these are always just the generic funcions,
       but for some hardware, these will be customized,
       optimized functions specific to that hardware
    */
    _gx_display_driver_32bpp_setup(display, (VOID*)LPC40XX_SCREEN_HANDLE, lpc40xx_32bpp_buffer_toggle);


    eventpad.gx_event_display_handle = LPC40XX_SCREEN_HANDLE;
    
    /*Disable LCD controller*/
    GLCD_Ctrl (FALSE);
    GLCD_Init (NULL, NULL);
    /*Enable LCD*/
    GLCD_Ctrl (TRUE);
    /*Disable cursor*/
    GLCD_Cursor_Dis(0);
    
    LPC_LCD -> CTRL |= 1 << 8;
    
    TouchScrInit();
    
    /* Return successful status.  */
    return(GX_SUCCESS);
}


/* Data for LPC4088-SK */
static int              x_scale = 115;
static int              x_offset = 5120;
static int              y_scale = 147;
static int              y_offset = 12288;
/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    send_event_to_guix                                  PORTABLE C      */
/*                                                           6.0          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function is called by the touch screen ISR routine to pass the */
/*    touch position to GUIX.                                             */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    x                                     X-coordinate                  */
/*    y                                     Y-coordinate                  */
/*    touch_detected                        Flag for touch being detected */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    gx_system_event_send                  Send the touch data to GUIX   */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Touch driver ISR                                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*                                                                        */
/**************************************************************************/
VOID send_event_to_guix(INT x, INT y, INT touch_detected)
{
    
static UINT previous_touch_state = 0;
static UINT previous_x;
static UINT previous_y;
INT         screen_x;
INT         screen_y;


    if (touch_detected)
    {
        
        if (!previous_touch_state || 
            (x >= previous_x ? x - previous_x : previous_x - x) > LPC40XX_TOUCH_SCREEN_TOLERANCE_X ||
            (y >= previous_y ? y - previous_y : previous_y - y) > LPC40XX_TOUCH_SCREEN_TOLERANCE_Y)
        {
            
            if (!previous_touch_state || 
                (x >= previous_x ? x - previous_x : previous_x - x) > LPC40XX_TOUCH_SCREEN_TOLERANCE_X)
            {

                previous_x = x;
                screen_x = x - x_offset >= 0 ? x - x_offset : 0;
                screen_x = screen_x / x_scale <= (LPC40XX_SCREEN_WIDTH - 1) ? screen_x / x_scale : (LPC40XX_SCREEN_WIDTH - 1);

                eventpad.gx_event_payload.gx_event_pointdata.gx_point_x = screen_x;
                
            }
            
            if (!previous_touch_state || 
                (y >= previous_y ? y - previous_y : previous_y - y) > LPC40XX_TOUCH_SCREEN_TOLERANCE_Y)
            {

                previous_y = y;
                screen_y = y - y_offset >= 0 ? y - y_offset : 0;
                screen_y = screen_y / y_scale <= (LPC40XX_SCREEN_HEIGHT - 1) ? screen_y / y_scale : (LPC40XX_SCREEN_HEIGHT - 1);
                screen_y = (LPC40XX_SCREEN_HEIGHT - 1) - screen_y;
                eventpad.gx_event_payload.gx_event_pointdata.gx_point_y = screen_y;
            }

            eventpad.gx_event_type = previous_touch_state ? GX_EVENT_PEN_DRAG : GX_EVENT_PEN_DOWN;
            gx_system_event_send(&eventpad);
        }
        
    }
    else if (previous_touch_state)
    {

        eventpad.gx_event_type = GX_EVENT_PEN_UP;
        gx_system_event_send(&eventpad);
    }
    
    previous_touch_state = touch_detected;
}

