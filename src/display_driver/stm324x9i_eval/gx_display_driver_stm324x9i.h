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


//#define STM32F429_DISCO_SCREEN_WIDTH           480
//#define STM32F429_DISCO_SCREEN_HEIGHT          272
#define STM32F429_DISCO_SCREEN_WIDTH           640
#define STM32F429_DISCO_SCREEN_HEIGHT          480

/* turn this on to enable Chrome Art Accelerator. Comment out to disable */
#define GX_CHROMEART_ENABLE     // comment out to disable using accelerator

/* turn this on to enable timing via IO pins */
//#define GX_TIMING_ENABLE        // comment out to disable IO pin timing


#define STM32F429_DISCO_SCREEN_HANDLE          0x12345679

#define CR_MASK                     ((uint32_t)0xFFFCE0FC)  /* DMA2D CR Mask */
#define PFCCR_MASK                  ((uint32_t)0x00FC00C0)  /* DMA2D FGPFCCR Mask */
#define DEAD_MASK                   ((uint32_t)0xFFFF00FE)  /* DMA2D DEAD Mask */

#if defined(GX_TIMING_ENABLE)
#define TIMING_FRAME_CALC_BEGIN  GPIOG->BSRR=GPIO_PIN_6;
#define TIMING_FRAME_CALC_END    GPIOG->BSRR=GPIO_PIN_6<<16;
#define TIMING_CPU_WORK_BEGIN    GPIOG->BSRR=GPIO_PIN_7;
#define TIMING_CPU_WORK_END      GPIOG->BSRR=GPIO_PIN_7<<16;
#else
#define TIMING_FRAME_CALC_BEGIN  
#define TIMING_FRAME_CALC_END    
#define TIMING_CPU_WORK_BEGIN    
#define TIMING_CPU_WORK_END     
#endif

/*****************************************************************************/
/* Some GLCD control values used to support layer services for layer 2       */
/*****************************************************************************/
typedef struct {
    USHORT *canvas_memory;
    INT canvas_width;
    INT canvas_height;
    INT vertical_stop;
    INT vertical_start;
    INT horizontal_stop;
    INT horizontal_start;
} GX_LAYER_PARAMS;
