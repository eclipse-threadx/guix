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
#include   "gx_display.h"

#include <stdint.h>
#include <stdbool.h>



#define LPC40XX_SCREEN_WIDTH                 480
#define LPC40XX_SCREEN_HEIGHT                272

#define LPC40XX_SCREEN_HANDLE                0x1234567A

#define LPC40XX_TOUCH_SCREEN_TOLERANCE_X     300
#define LPC40XX_TOUCH_SCREEN_TOLERANCE_Y     500

extern unsigned int ui32SysClock;
