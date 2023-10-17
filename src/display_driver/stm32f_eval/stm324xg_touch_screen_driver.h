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
/**   STM324XG Touch Screen Driver                                        */
/**                                                                       */
/**************************************************************************/


#include   "stm324xg_eval_lcd.h"
#include   "stm324xg_eval_ioe.h"


#define BITBAND(_address, _bitnum)  ((_address & 0xF0000000) + 0x2000000 + ((_address & 0xFFFFF) << 5) + (_bitnum << 2)) 
#define MEM_ADDR(_address)          *((volatile unsigned long *)(_address)) 
#define BIT_ADDR(_address, _bitnum) MEM_ADDR(BITBAND((unsigned long)&(_address), _bitnum)) 

#define LED1_BIT                    BIT_ADDR(GPIOG -> ODR, 6)
#define LED2_BIT                    BIT_ADDR(GPIOG -> ODR, 8)
#define LED3_BIT                    BIT_ADDR(GPIOI -> ODR, 9)
#define LED4_BIT                    BIT_ADDR(GPIOC -> ODR, 7)

#define IOE_I2C_CR1_STOP_BIT        BIT_ADDR(IOE_I2C -> CR1, 9)
#define IOE_I2C_CR1_START_BIT       BIT_ADDR(IOE_I2C -> CR1, 8)
#define IOE_I2C_SR1_SB_BIT          BIT_ADDR(IOE_I2C -> SR1, 0)
#define IOE_I2C_SR1_ADDR_BIT        BIT_ADDR(IOE_I2C -> SR1, 1)
#define IOE_I2C_SR1_BTF_BIT         BIT_ADDR(IOE_I2C -> SR1, 2)
#define IOE_I2C_SR2_TRA_BIT         BIT_ADDR(IOE_I2C -> SR2, 2)



#define IOE_DMA_TX_STREAM_EN_BIT    BIT_ADDR(IOE_DMA_TX_STREAM -> CR, 0)
#define IOE_DMA_TX_STREAM_TCIE_BIT  BIT_ADDR(IOE_DMA_TX_STREAM -> CR, 4)
#define IOE_DMA_RX_STREAM_EN_BIT    BIT_ADDR(IOE_DMA_RX_STREAM -> CR, 0)
#define IOE_DMA_RX_STREAM_TCIE_BIT  BIT_ADDR(IOE_DMA_RX_STREAM -> CR, 4)

#define IOE_EXTI_PENDING_BIT        BIT_ADDR(EXTI -> PR, 2)
#define IOE_EXTI_ENABLE_BIT         BIT_ADDR(EXTI -> IMR, 2)



/*
#define RAMFUNC __ramfunc
*/
#define RAMFUNC

#define CLEAR_FLAG_IOE_DMA_TX_STREAM    DMA1 -> HIFCR = 0x003D0000
#define CLEAR_FLAG_IOE_DMA_RX_STREAM    DMA1 -> LIFCR = 0x0000003D


#define FIFO_DATA_COUNT                     8
#define STM324XG_TOUCH_SCREEN_TOLERANCE_X   100
#define STM324XG_TOUCH_SCREEN_TOLERANCE_Y   133


RAMFUNC static VOID send_address(VOID);
RAMFUNC static VOID clear_addr_flag(VOID);
RAMFUNC static VOID send_restart(VOID);
RAMFUNC static VOID send_stop(VOID);
RAMFUNC static VOID error_handler(VOID);
RAMFUNC static VOID int_sta_handler(VOID);
RAMFUNC static VOID tsc_ctrl_handler(VOID);
RAMFUNC static VOID clear_int_bits(VOID);
RAMFUNC static VOID fifo_size_handler(VOID);
RAMFUNC static VOID reset_fifo_handler(VOID);
RAMFUNC static VOID process_fifo_data(VOID);
RAMFUNC static VOID read_tsc_ctrl(VOID);
RAMFUNC static VOID check_end(VOID);
RAMFUNC static VOID send_event_to_guix(INT x, INT y, INT touch_detected);

RAMFUNC static VOID i2c_read_bytes(uint8_t device_address, uint8_t register_address, VOID* buffer, uint32_t length);
RAMFUNC static VOID i2c_write_bytes(uint8_t device_address, VOID const* buffer, uint32_t length);


typedef VOID(*STATE_HANDLER)(VOID);


