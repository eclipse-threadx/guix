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
/**   Screen Driver                                                       */        
/**                                                                       */
/**************************************************************************/


#define BOARD_SCREEN_HANDLE          0x12345679

#ifdef STM32F429I_DISCO
    #include   "stm32f429i_discovery_lcd.h"
	#include   "stm32f429i_discovery_ts.h"
	#define     BOARD_SCREEN_WIDTH        320
	#define     BOARD_SCREEN_HEIGHT       240
#elif STM32F429I_EVAL
    #include   "stm324x9i_eval_lcd.h"
    #include   "stm324x9i_eval_ts.h"
	#define     BOARD_SCREEN_WIDTH         480
	#define     BOARD_SCREEN_HEIGHT        272
#elif STM32F439I_EVAL
    #include   "stm324x9i_eval_lcd.h"
    #include   "stm324x9i_eval_ts.h"
	#define     BOARD_SCREEN_WIDTH         640
	#define     BOARD_SCREEN_HEIGHT        480
#elif STM32F769I_EVAL
    #include   "stm32f769i_eval_lcd.h"
    #include   "stm32f769i_eval_ts.h"
    #define     BOARD_SCREEN_WIDTH          800
    #define     BOARD_SCREEN_HEIGHT         480
#elif STM32H743I_EVAL
    #include   "stm32h743i_eval_lcd.h"
    #include   "stm32h743i_eval_ts.h"
	#define     BOARD_SCREEN_WIDTH         640
	#define     BOARD_SCREEN_HEIGHT        480
    #define DMA2D_ARGB8888 DMA2D_OUTPUT_ARGB8888
#else
	#error please define board type
#endif