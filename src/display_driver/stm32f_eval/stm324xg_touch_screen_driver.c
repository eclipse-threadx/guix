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


#include   "tx_api.h"
#include   "gx_api.h"
#include   "gx_system.h"
#include   "gx_canvas.h"

#include   "stm324xg_screen.h"
#include   "stm324xg_touch_screen_driver.h"


static UCHAR            state_device_address;
static UCHAR            state_register_address;

static int              state_busy;
static int              new_interrupt_during_busy;
static UCHAR            tsc_ctrl[2];
static UCHAR            fifo_sta_size[2];
static ULONG            fifo_data[FIFO_DATA_COUNT];
static UCHAR            clear_all_int_pending_bits[2] = {IOE_REG_INT_STA, 0xFF};
static GX_EVENT         eventpad;
static int              x_scale = 95;
static int              x_offset = 1410;
static int              y_scale = 124;
static int              y_offset = 1360;

static STATE_HANDLER    i2c_event_sequence_write_bytes[] = {
                                        send_address,           /* Send device_address after SB event.  */
                                        clear_addr_flag,        /* Clear the ADDR flag after address acked.  */
                                        send_stop,              /* Data transfer finished, clear BTF, send STOP.  */
                                        error_handler
};

static STATE_HANDLER    i2c_event_sequence_read_bytes[] = {
                                        send_address,           /* Send device_address after SB event.  */
                                        clear_addr_flag,        /* Clear the ADDR flag after address acked.  */
                                        send_restart,           /* Data transfer finished, clear BTF, send reSTART.  */
                                        send_address,
                                        clear_addr_flag,
                                        error_handler
};

static STATE_HANDLER*   i2c_event_sequence;
static STATE_HANDLER*   dma_rx_sequence;
static STATE_HANDLER*   dma_tx_sequence; 

static STATE_HANDLER    dma_rx_sequence_ioe_int_handler[] = {
                                        tsc_ctrl_handler,       /* Got TSC_CRTL, see if touch is detected.  */
                                        fifo_size_handler,
                                        process_fifo_data,
                                        error_handler
};

static STATE_HANDLER    dma_tx_sequence_ioe_int_handler[] = {
                                        read_tsc_ctrl,
                                        error_handler
};


static VOID error_handler(VOID)
{
    while(1);
}


static VOID send_address(VOID)
{


    if (IOE_I2C_SR1_SB_BIT)
    {

        IOE_I2C -> DR = state_device_address;
        IOE_I2C -> DR;          /* Read DR to ensure APB write is done.  */
    }
    else
        error_handler();
}


static VOID clear_addr_flag(VOID)
{


    if (IOE_I2C_SR1_ADDR_BIT)
    {

        if (IOE_I2C_SR2_TRA_BIT)
        {

            IOE_DMA_TX_STREAM_EN_BIT = 1;
        }
        else
        {

            IOE_DMA_RX_STREAM_EN_BIT = 1;
        }
    }
    else
        error_handler();
}


static VOID send_restart(VOID)
{


    if (IOE_I2C_SR1_BTF_BIT)
    {
        
        /* Clear the BTF flag by reading the DR.  */
        IOE_I2C -> DR;
        
        state_device_address |= 1;
        
        /* Generate a reSTART condition.  */
        IOE_I2C_CR1_START_BIT = 1;
    }
    else
        error_handler();
}


static VOID send_stop(VOID)
{


    if (IOE_I2C_SR1_BTF_BIT)
    {
        
        /* Clear the BTF flag by reading the DR.  */
        IOE_I2C -> DR;
        
        /* Generate a STOP condition.  */
        IOE_I2C_CR1_STOP_BIT = 1;
        
        /* Enable DMA TX transfer complete interrupt.  */
        IOE_DMA_TX_STREAM_TCIE_BIT = 1;
    }
    else
        error_handler();
}


static VOID tsc_ctrl_handler(VOID)
{


    if (tsc_ctrl[0] & 0x80)
    {
        
        /* Touch detected, read the size of fifo.  */
        i2c_read_bytes(IOE_1_ADDR, IOE_REG_FIFO_STA, fifo_sta_size, sizeof(fifo_sta_size));
        
    }
    else
    {
        
        /* Touch lifted, queue the event. */
        send_event_to_guix(0, 0, GX_FALSE);
        check_end();
    }
    
}


static VOID fifo_size_handler(VOID)
{


    if (fifo_sta_size[1] >= FIFO_DATA_COUNT)
    {
        
        i2c_read_bytes(IOE_1_ADDR, 0xD7, fifo_data, sizeof(fifo_data));
        
    }
    else if (fifo_sta_size[1] > 0)
    {
        
        i2c_read_bytes(IOE_1_ADDR, 0xD7, fifo_data, fifo_sta_size[1] * 4);
    }
    else
    {
        
        check_end();
    }
}


static VOID read_tsc_ctrl(VOID)
{
    
    i2c_read_bytes(IOE_1_ADDR, IOE_REG_TSC_CTRL, &tsc_ctrl, 2);
}


static VOID process_fifo_data(VOID)
{
    
INT     x;
INT     y;
INT     z;
INT     i;
UINT    xyz_data;


    if (fifo_sta_size[1] < FIFO_DATA_COUNT)
    {

        check_end();
        return;
    }
    
    for (x = y = z = i = 0; i < FIFO_DATA_COUNT; i++)
    {
        
        xyz_data = __REV(fifo_data[i]);
        y += ((xyz_data & 0xFFF00000) >> 20);
        x += ((xyz_data & 0x000FFF00) >> 8);
        z +=  (xyz_data & 0x000000FF);
    }
    
    /* Send the event to GUIX.  */
    send_event_to_guix(x, y, GX_TRUE);
    
    if (fifo_sta_size[1] >= 2 * FIFO_DATA_COUNT)
    {

        /* More data to read.  */
        dma_rx_sequence -= 3;
        i2c_read_bytes(IOE_1_ADDR, IOE_REG_TSC_CTRL, &tsc_ctrl, 2);
    }
    else
    {

        check_end();
    }
}


static VOID check_end(VOID)
{
    
    if (new_interrupt_during_busy)
    {
        
        new_interrupt_during_busy = 0;
        dma_rx_sequence = dma_rx_sequence_ioe_int_handler;
        dma_tx_sequence = dma_tx_sequence_ioe_int_handler;
        i2c_write_bytes(IOE_1_ADDR, clear_all_int_pending_bits, sizeof(clear_all_int_pending_bits));
    }
    else
    {
        
        state_busy = 0;
    }
}


static VOID send_event_to_guix(INT x, INT y, INT touch_detected)
{
    
static UINT previous_touch_state = 0;
static UINT previous_x;
static UINT previous_y;
INT         screen_x;
INT         screen_y;


    if (touch_detected)
    {
        
        if (!previous_touch_state || 
            (x >= previous_x ? x - previous_x : previous_x - x) > STM324XG_TOUCH_SCREEN_TOLERANCE_X ||
            (y >= previous_y ? y - previous_y : previous_y - y) > STM324XG_TOUCH_SCREEN_TOLERANCE_Y)
        {
            
            if (!previous_touch_state || 
                (x >= previous_x ? x - previous_x : previous_x - x) > STM324XG_TOUCH_SCREEN_TOLERANCE_X)
            {

                previous_x = x;
                screen_x = x - x_offset >= 0 ? x - x_offset : 0;
                screen_x = screen_x / x_scale <= (STM324XG_SCREEN_WIDTH - 1) ? screen_x / x_scale : (STM324XG_SCREEN_WIDTH - 1);
                eventpad.gx_event_payload.gx_event_pointdata.gx_point_x = screen_x;
            }
            
            if (!previous_touch_state || 
                (y >= previous_y ? y - previous_y : previous_y - y) > STM324XG_TOUCH_SCREEN_TOLERANCE_Y)
            {

                previous_y = y;
                screen_y = y - y_offset >= 0 ? y - y_offset : 0;
                screen_y = screen_y / y_scale <= (STM324XG_SCREEN_HEIGHT - 1) ? screen_y / y_scale : (STM324XG_SCREEN_HEIGHT - 1);
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
        }
        
    }
    else if (previous_touch_state)
    {
        
        eventpad.gx_event_type = GX_EVENT_PEN_UP;
        gx_system_event_send(&eventpad);
    }
    
    previous_touch_state = touch_detected;

}


static VOID i2c_read_bytes(uint8_t device_address, uint8_t register_address, VOID* buffer, uint32_t length)
{
    
    /* Save the addresses to global variables.  */
    state_device_address = device_address;
    state_register_address = register_address;

    /* Configure the DMA for I2C data transfer.  */
    IOE_DMA_TX_STREAM -> NDTR = 1;
    IOE_DMA_TX_STREAM -> M0AR = (uint32_t)&state_register_address;
    IOE_DMA_TX_STREAM_TCIE_BIT = 0;

    IOE_DMA_RX_STREAM -> NDTR = length;
    IOE_DMA_RX_STREAM -> M0AR = (uint32_t)buffer;

    i2c_event_sequence = i2c_event_sequence_read_bytes;

    /* Generate a START condition.  */
    IOE_I2C_CR1_START_BIT = 1;
    
}


static VOID i2c_write_bytes(uint8_t device_address, VOID const* buffer, uint32_t length)
{


    state_device_address = device_address;

    IOE_DMA_TX_STREAM -> NDTR = length;
    IOE_DMA_TX_STREAM -> M0AR = (uint32_t)buffer;

    IOE_DMA_TX_STREAM_TCIE_BIT = 0;

    i2c_event_sequence = i2c_event_sequence_write_bytes;
    
    /* Generate a START condition.  */
    IOE_I2C_CR1_START_BIT = 1;
}


VOID EXTI2_IRQHandler(VOID)
{
    

    if (IOE_EXTI_PENDING_BIT)
    {
        
        IOE_EXTI_PENDING_BIT = 1;
        
        if (state_busy)
        {

            new_interrupt_during_busy = 1;
        }
        else
        {

            state_busy = 1;
            
            dma_rx_sequence = dma_rx_sequence_ioe_int_handler;
            dma_tx_sequence = dma_tx_sequence_ioe_int_handler;
            
            /* Start processing the IOE events by starting a register access.  */
            i2c_write_bytes(IOE_1_ADDR, clear_all_int_pending_bits, sizeof(clear_all_int_pending_bits));
        }
    }
    __DSB();
}


VOID dma1_stream0_isr(VOID)
{


    /* Receive finished, generate a STOP condition.  */
    IOE_I2C_CR1_STOP_BIT = 1;

    /* Clear DMA RX Transfer Complete Flag.  */
    CLEAR_FLAG_IOE_DMA_RX_STREAM;

    /* Clear DMA TX Transfer Complete Flag.  */
    CLEAR_FLAG_IOE_DMA_TX_STREAM;

    (*dma_rx_sequence++)();

}


VOID DMA1_Stream6_IRQHandler(VOID)
{


    /* Clear DMA TX Transfer Complete Flag.  */
    CLEAR_FLAG_IOE_DMA_TX_STREAM;

    (*dma_tx_sequence++)();

}


VOID I2C1_EV_IRQHandler(VOID)
{

    (*i2c_event_sequence++)();
}


VOID I2C1_ER_IRQHandler(VOID)
{


    LED3_BIT = 1;
}


VOID stm324xg_touch_screen_init(VOID)
{

NVIC_InitTypeDef NVIC_InitStructure;
DMA_InitTypeDef DMA_InitStructure;


    eventpad.gx_event_display_handle = STM324XG_SCREEN_HANDLE;
    
    /* Configure the IO Expander.  */
    IOE_Config();

    /* Enable the Touch Screen and Joystick interrupts.  */
    IOE_ITConfig(IOE_ITSRC_JOYSTICK | IOE_ITSRC_TSC);

    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0f;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0f;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQn;
    NVIC_Init(&NVIC_InitStructure);
    NVIC_InitStructure.NVIC_IRQChannel = I2C1_EV_IRQn;
    NVIC_Init(&NVIC_InitStructure);
    NVIC_InitStructure.NVIC_IRQChannel = I2C1_ER_IRQn;
    NVIC_Init(&NVIC_InitStructure);
    
    I2C_ITConfig(IOE_I2C, I2C_IT_EVT | I2C_IT_ERR, ENABLE);
    
    /* Init DMA for I2C.  */
    RCC_AHB1PeriphClockCmd(IOE_DMA_CLK, ENABLE);
    
    DMA_InitStructure.DMA_Channel = IOE_DMA_CHANNEL;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&IOE_I2C -> DR;
    DMA_InitStructure.DMA_Memory0BaseAddr = 0;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    
    
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_BufferSize = 2;
    DMA_DeInit(IOE_DMA_RX_STREAM);
    DMA_Init(IOE_DMA_RX_STREAM, &DMA_InitStructure);

    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize = 1;    
    DMA_DeInit(IOE_DMA_TX_STREAM);
    DMA_Init(IOE_DMA_TX_STREAM, &DMA_InitStructure);
    
    DMA_ITConfig(IOE_DMA_RX_STREAM, DMA_IT_TC, ENABLE);
    DMA_ITConfig(IOE_DMA_TX_STREAM, DMA_IT_TC, ENABLE);
    
    
    /* Enable DMA NACK automatic generation */
    I2C_DMALastTransferCmd(IOE_I2C, ENABLE);
  
    /* Enable I2C DMA request.  */
    I2C_DMACmd(IOE_I2C, ENABLE);
    
    



}

