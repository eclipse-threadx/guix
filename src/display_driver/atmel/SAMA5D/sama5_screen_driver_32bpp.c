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

//#include   "lpc40xx_screen_driver_32bpp.h"
//#include   "drv_glcd.h"
//#include   "LPC407x_8x.h"
#include "board.h"


#include   "sama5_screen_driver_32bpp.h"

#include "include/lcdd.h"

static GX_EVENT         eventpad;

//extern uint32_t SDRAM_VRAM_BASE_ADDR;
//extern void TouchScrInit(void);


VOID send_event_to_guix(INT x, INT y, INT touch_detected);

/* please modify youre framebuffer */
//#define LCD_VRAM_BASE_ADDR ((uint32_t)&SDRAM_VRAM_BASE_ADDR)
//#define SDRAM _Pragma("location=\"DMA_BUF\"")

//SDRAM ULONG frame_buffer1[800][480];


#define DDR_CS_ADDR    (0x20000000u) /**< DDR Chip Select base address */

#define LCD_BASE (DDR_CS_ADDR + 16*1024*1024)  /*   0x1000000  == 16M*/

/** LCD base image buffer  0x21000000  */
#define ADDR_LCD_BUFFER_BASE    (LCD_BASE)
/** Size of base image buffer   0x177000 */
#define SIZE_LCD_BUFFER_BASE    (BOARD_LCD_WIDTH * BOARD_LCD_HEIGHT * 4)

/** LCD Overlay 1 (OVR1) buffer  0x21177000  */
#define ADDR_LCD_BUFFER_OVR1    (ADDR_LCD_BUFFER_BASE + SIZE_LCD_BUFFER_BASE)
/** Size of Overlay 1 buffer   0x177000 */
#define SIZE_LCD_BUFFER_OVR1    (BOARD_LCD_WIDTH * BOARD_LCD_HEIGHT * 4)

/** LCD Overlay 2 (OVR2) buffer 0x212EE000 */
#define ADDR_LCD_BUFFER_OVR2    (ADDR_LCD_BUFFER_OVR1 + SIZE_LCD_BUFFER_OVR1)
/** Size of Overlay 2 buffer  0x177000 */
#define SIZE_LCD_BUFFER_OVR2    (BOARD_LCD_WIDTH * BOARD_LCD_HEIGHT * 4)

/** LCD High End Overlay (HEO) buffer  0x21465000*/
#define ADDR_LCD_BUFFER_HEO     (ADDR_LCD_BUFFER_OVR2 + SIZE_LCD_BUFFER_OVR2)
/** Size of High End Overlay buffer  0x177000 */
#define SIZE_LCD_BUFFER_HEO     (BOARD_LCD_WIDTH * BOARD_LCD_HEIGHT * 4)

/** LCD Hardware Cursor (HCR) buffer 0x215DC000 */
#define ADDR_LCD_BUFFER_HCR     (ADDR_LCD_BUFFER_HEO + SIZE_LCD_BUFFER_HEO)


//ULONG * frame_buffer = (ULONG *)LCD_VRAM_BASE_ADDR;

static uint8_t *pBaseBuffer = (uint8_t *)ADDR_LCD_BUFFER_BASE;

ULONG * frame_buffer = (ULONG *)ADDR_LCD_BUFFER_BASE;


/* Touch */
//TX_SEMAPHORE my_semaphore;

//TX_EVENT_FLAGS_GROUP my_event_group;


//static unsigned char touch_thread_ram[2048]={0};
//TX_THREAD  tx_touch_thread;
//void tx_touch_thread_entry(ULONG arg);


//static int touch_ram[128] = { 0} ;
//TX_QUEUE touch_queue;
//ULONG my_message[4];
 


/** R for touchpoint */
#define TP_R        6

/** X, Y, R for touched point */
//static uint16_t wX = 0, wY = 0, wR;


/**
 * ADC interrupt handler
 */

static void ADC_IrqHandler(void)
{
   uint32_t dwStatus = ADC_GetStatus(ADC); 
   TSD_Handler(dwStatus);
}



void Touch_Initialize()
{
  
    uint8_t bRc;
  
  /* Configure ADC for touchscreen */
  //  bit 6   5
  //    0     0  INT_LEVEL_SENSITIVE
  //    0     1  INT_EDGE_TRIGGERED
  //    1     0  EXT_HIGH_LEVEL
  //    1     1  EXT_POSITIVE_EDGE
  // bit  2 1 0  Priority Level
  
    
    IRQ_ConfigureIT(ID_ADC, 0x00, ADC_IrqHandler);
    IRQ_EnableIT(ID_ADC);
    TSD_Initialize();    
    
 
#if 0
     while(1)
    {
        bRc = TSD_Calibrate();
        if ( bRc )
        {
            printf( "-I- Calibration successful !\n\r" ) ;
            break ;
        }
        else
        {
            printf( "-E- Error too big ! Retry...\n\r" ) ;
        }
    }   
    
#else
    
    
     bRc = TSD_Calibrate();
     
     TSD_Enable(bRc );
     
#endif
    
} 


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
void sama5_32bpp_buffer_toggle(GX_CANVAS *canvas, GX_RECTANGLE *dirty)
{

GX_RECTANGLE    Limit;
GX_RECTANGLE    Copy;
INT             x;
INT             y;
ULONG*       pixel;
ULONG* frame_buffer_ptr;


    ULONG *memptr = (ULONG*)canvas -> gx_canvas_memory;

    gx_utility_rectangle_define(&Limit, 0, 0,
                                SAMA5_SCREEN_WIDTH - 1, SAMA5_SCREEN_HEIGHT - 1);

    if (gx_utility_rectangle_overlap_detect(&Limit, &canvas->gx_canvas_dirty_area, &Copy))
    {
        for(y = Copy.gx_rectangle_top;  y <= Copy.gx_rectangle_bottom; y++)
        {

            frame_buffer_ptr = &frame_buffer[y * SAMA5_SCREEN_WIDTH + Copy.gx_rectangle_left];
            
            pixel = memptr + y * SAMA5_SCREEN_WIDTH;
            for(x = Copy.gx_rectangle_left; x <= Copy.gx_rectangle_right; x++)
            {           
                *frame_buffer_ptr++ = *(pixel + x);
            }
              
        }
    }
    
    
    LCDD_Flush_CurrentCanvas();  
    
}


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    sama5_graphics_driver_setup_32bpp                 PORTABLE C      */
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
UINT sama5_graphics_driver_setup_32bpp(GX_DISPLAY *display)
{
  
     UINT  status;
     
     status =  GX_SUCCESS;
    /* initialize the low-level drawing function pointers

     
       for windows, these are always just the generic funcions,
       but for some hardware, these will be customized,
       optimized functions specific to that hardware
    */
    
     LCDD_Initialize();   
     
     //LCDD_SetBacklight(0xF0);
     
     //LCDD_CreateCanvas( LCDD_BASE, 
      LCDD_CreateCanvas(LCDD_HEO,
                        pBaseBuffer, 
                       32,
                       0, 
                       0, 
                       800, 
                       480);   
     
     //tx_thread_sleep(50);
     
    _gx_display_driver_24xrgb_setup(display, (VOID*)SAMA5_SCREEN_HANDLE, sama5_32bpp_buffer_toggle);


    eventpad.gx_event_display_handle = SAMA5_SCREEN_HANDLE;
    
    /*Disable LCD controller*/
    Touch_Initialize();



    /* Return successful status.  */
    return status;
}


/* Data for LPC4088-SK */

//static int              x_scale = 115;
//static int              x_offset = 5120;
//static int              x_offset = 0;
//static int              y_scale = 147;
//static int              y_offset = 12288;
//static int              y_offset = 0;
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
volatile int debug_x[1024];
volatile int debug_y[1024];
volatile int debug_touch[1024];
volatile int debug_i;

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
            (x >= previous_x ? x - previous_x : previous_x - x) > SAMA5_TOUCH_SCREEN_TOLERANCE_X ||
            (y >= previous_y ? y - previous_y : previous_y - y) > SAMA5_TOUCH_SCREEN_TOLERANCE_Y)
        {
            
            if (!previous_touch_state || 
                (x >= previous_x ? x - previous_x : previous_x - x) > SAMA5_TOUCH_SCREEN_TOLERANCE_X)
            {

                previous_x = x;
                 
                  screen_x = x + 8 >= 0 ? x + 8 : 0;
                 
                screen_x =  screen_x <= (SAMA5_SCREEN_WIDTH - 1) ? screen_x : (SAMA5_SCREEN_WIDTH - 1);
               
                  
                //screen_x = x - x_offset >= 0 ? x - x_offset : 0;
               // screen_x = screen_x / x_scale <= (SAMA5_SCREEN_WIDTH - 1) ? screen_x / x_scale : (SAMA5_SCREEN_WIDTH - 1);

                eventpad.gx_event_payload.gx_event_pointdata.gx_point_x = screen_x;
                
            }
            
            if (!previous_touch_state || 
                (y >= previous_y ? y - previous_y : previous_y - y) > SAMA5_TOUCH_SCREEN_TOLERANCE_Y)
            {

                previous_y = y; 
         
                
                screen_y = y + 16 >= 0 ? y + 16 : 0;
            
                screen_y =  screen_y <= (SAMA5_SCREEN_HEIGHT - 1) ? screen_y : (SAMA5_SCREEN_HEIGHT - 1);
                
               // screen_y = y - y_offset >= 0 ? y - y_offset : 0;
               // screen_y = screen_y / y_scale <= (SAMA5_SCREEN_HEIGHT - 1) ? screen_y / y_scale : (SAMA5_SCREEN_HEIGHT - 1);
              //  screen_y = (SAMA5_SCREEN_HEIGHT - 1) - screen_y;
                eventpad.gx_event_payload.gx_event_pointdata.gx_point_y = screen_y;
            }

            if (previous_touch_state)
            {
                eventpad.gx_event_type = GX_EVENT_PEN_DRAG;
                gx_system_event_fold(&eventpad);
            }
            else
            {
                eventpad.gx_event_type = GX_EVENT_PEN_DOWN;
                gx_system_event_send(&eventpad);
            }
            if (debug_i<1024)
            {
                debug_x[debug_i] = eventpad.gx_event_payload.gx_event_pointdata.gx_point_x;
                debug_y[debug_i] = eventpad.gx_event_payload.gx_event_pointdata.gx_point_y;
                debug_touch[debug_i] = eventpad.gx_event_type;
                debug_i++;
            }

        }
        
    }
    else if (previous_touch_state)
    {

        eventpad.gx_event_type = GX_EVENT_PEN_UP;
        gx_system_event_send(&eventpad);
    }
    
    previous_touch_state = touch_detected;
}

