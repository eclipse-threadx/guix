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
/**   AM335x Touch Screen Driver                                          */
/**                                                                       */
/**************************************************************************/

#include    "am335x_screen.h"
#include    "gx_system.h"
#include    "tsc_adc.h"
#include    "interrupt.h"

static VOID idle_step_init(VOID);
static VOID ts_charge_step_init(VOID);
static VOID step_init(VOID);

extern ULONG frame_buffer[];

static GX_EVENT         eventpad;
static int              x_scale = 258;
static int              x_offset = 3390;
static int              y_scale = 423;
static int              y_offset = 7670;

#if 0
volatile int cal_x[32];
volatile int cal_y[32];
volatile int cal_i;
volatile int cal_ii;
#endif


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
            (x >= previous_x ? x - previous_x : previous_x - x) > AM335X_TOUCH_SCREEN_TOLERANCE_X ||
            (y >= previous_y ? y - previous_y : previous_y - y) > AM335X_TOUCH_SCREEN_TOLERANCE_Y)
        {
            
            if (!previous_touch_state || 
                (x >= previous_x ? x - previous_x : previous_x - x) > AM335X_TOUCH_SCREEN_TOLERANCE_X)
            {
                
                previous_x = x;
                screen_x = x - x_offset >= 0 ? x - x_offset : 0;
                screen_x = screen_x / x_scale <= (AM335X_SCREEN_WIDTH - 1) ? screen_x / x_scale : (AM335X_SCREEN_WIDTH - 1);
                screen_x = (AM335X_SCREEN_WIDTH - 1) - screen_x;
                eventpad.gx_event_payload.gx_event_pointdata.gx_point_x = screen_x;
            }
            if (!previous_touch_state || 
                (y >= previous_y ? y - previous_y : previous_y - y) > AM335X_TOUCH_SCREEN_TOLERANCE_Y)
            {
                
                previous_y = y;
                screen_y = y - y_offset >= 0 ? y - y_offset : 0;
                screen_y = screen_y / y_scale <= (AM335X_SCREEN_HEIGHT - 1) ? screen_y / y_scale : (AM335X_SCREEN_HEIGHT - 1);
                screen_y = (AM335X_SCREEN_HEIGHT - 1) - screen_y;
                eventpad.gx_event_payload.gx_event_pointdata.gx_point_y = screen_y;
            }
            
            eventpad.gx_event_type = previous_touch_state ? GX_EVENT_PEN_DRAG : GX_EVENT_PEN_DOWN;
            gx_system_event_send(&eventpad);
            
#if 0
            frame_buffer[AM335X_SCREEN_WIDTH * eventpad.gx_event_payload.gx_event_pointdata.gx_point_y
                                + eventpad.gx_event_payload.gx_event_pointdata.gx_point_x] = 0;
#ifdef ENABLE_CACHE
            CacheDataCleanBuff((ULONG)&frame_buffer[AM335X_SCREEN_WIDTH * eventpad.gx_event_payload.gx_event_pointdata.gx_point_y
                                + eventpad.gx_event_payload.gx_event_pointdata.gx_point_x], sizeof(ULONG));
#endif /* ENABLE_CACHE */
#endif
        
        }

        
#if 0
        /* Used for gathering calibration data.  */
        cal_x[cal_i] += x;
        cal_y[cal_i] += y;
        cal_ii++;
        if (cal_ii == 128)
        {
            
            cal_ii = 0;
            
            cal_x[cal_i] /= 128;
            cal_y[cal_i] /= 128;
            
            cal_i ++;
            if (cal_i == 32) cal_i = 0;
        }
#endif
    }
    else if (previous_touch_state)
    {
        
        eventpad.gx_event_type = GX_EVENT_PEN_UP;
        gx_system_event_send(&eventpad);
        
#if 0
        screen_x = eventpad.gx_event_payload.gx_event_pointdata.gx_point_x;
        screen_y = eventpad.gx_event_payload.gx_event_pointdata.gx_point_y;
        frame_buffer[AM335X_SCREEN_WIDTH * screen_y + screen_x]= 0xFFFFFFFF;
#ifdef ENABLE_CACHE
        CacheDataCleanBuff((ULONG)&frame_buffer[AM335X_SCREEN_WIDTH * screen_y + screen_x], sizeof(ULONG));
#endif /* ENABLE_CACHE */
#endif
        
    }
    
    previous_touch_state = touch_detected;
}


VOID am335x_touch_screen_isr(VOID)
{
    
INT     x;
INT     y;

INT status;
INT i;

    status = TSCADCIntStatus(SOC_ADC_TSC_0_REGS);

    if (status & TSCADC_PEN_UP_EVENT_INT)
    {
        
        i = TSCADCFIFOWordCountRead(SOC_ADC_TSC_0_REGS, TSCADC_FIFO_0);
        for ( ; i; i--)
        {
            
            x = TSCADCFIFOADCDataRead(SOC_ADC_TSC_0_REGS, TSCADC_FIFO_0);
            y = TSCADCFIFOADCDataRead(SOC_ADC_TSC_0_REGS, TSCADC_FIFO_1);
        }
        
        send_event_to_guix(x, y, GX_FALSE);
        TSCADCIntStatusClear(SOC_ADC_TSC_0_REGS, TSCADC_FIFO0_UNDER_FLOW_INT | 
                                                 TSCADC_FIFO1_UNDER_FLOW_INT |
                                                 TSCADC_FIFO0_THRESHOLD_INT | 
                                                 TSCADC_FIFO1_THRESHOLD_INT | 
                                                 TSCADC_FIFO0_OVER_RUN_INT |
                                                 TSCADC_FIFO1_OVER_RUN_INT |
                                                 TSCADC_PEN_UP_EVENT_INT); 
        
        return;
    }
    if (status & TSCADC_FIFO0_THRESHOLD_INT)
    {
        
        while (TSCADCFIFOWordCountRead(SOC_ADC_TSC_0_REGS, TSCADC_FIFO_0) >= 32)
        {
            
            for (x = y = i = 0; i < 32; i++)
            {
                
                x += TSCADCFIFOADCDataRead(SOC_ADC_TSC_0_REGS, TSCADC_FIFO_0);
                y += TSCADCFIFOADCDataRead(SOC_ADC_TSC_0_REGS, TSCADC_FIFO_1);
            }
            /* Send the event to GUIX.  */
            send_event_to_guix(x, y, GX_TRUE);
        }
        TSCADCIntStatusClear(SOC_ADC_TSC_0_REGS, TSCADC_FIFO0_THRESHOLD_INT);
    }
    
}



VOID am335x_touch_screen_init(VOID)
{
    
    
    eventpad.gx_event_screen_handle = AM335X_SCREEN_HANDLE;
    
    TSCADCModuleClkConfig();
    
    TSCADCPinMuxSetUp();

    /* Configures ADC AFE clock to 3MHz.  */ 
    TSCADCConfigureAFEClock(SOC_ADC_TSC_0_REGS, 24000000, 3000000);

    /* Enable transistor bias.  */
    TSCADCTSTransistorConfig(SOC_ADC_TSC_0_REGS, TSCADC_TRANSISTOR_ENABLE);

    /* Map hardware event to Pen Touch IRQ.  */
    TSCADCHWEventMapSet(SOC_ADC_TSC_0_REGS, TSCADC_PEN_TOUCH);

    /* Set touchscreen mode to 4 wire mode.  */
    TSCADCTSModeConfig(SOC_ADC_TSC_0_REGS, TSCADC_FOUR_WIRE_MODE);

    /* Disable storing ID Tag in fifo.  */
    TSCADCStepIDTagConfig(SOC_ADC_TSC_0_REGS, 0);
    
    TSCADCStepConfigProtectionDisable(SOC_ADC_TSC_0_REGS);
    
    /* Init and enable Steps.  */
    idle_step_init();
    ts_charge_step_init();
    step_init();
    
    /* Configure FIFO0 threshold value.  */
    TSCADCFIFOIRQThresholdLevelConfig(SOC_ADC_TSC_0_REGS, TSCADC_FIFO_0, 32);
    
    /* Enable the FIFO Threshold and Pen Up interrupt.  */
    TSCADCEventInterruptEnable(SOC_ADC_TSC_0_REGS, TSCADC_FIFO0_THRESHOLD_INT | TSCADC_PEN_UP_EVENT_INT);
    
    /* Enable the Touchscreen module.  */
    TSCADCModuleStateSet(SOC_ADC_TSC_0_REGS, TSCADC_MODULE_ENABLE);
    
    TSCADCIntStatusClear(SOC_ADC_TSC_0_REGS, TSCADC_PEN_UP_EVENT_INT); 
    
    irq_register(am335x_touch_screen_isr, SYS_INT_ADC_TSC_GENINT, 0);
    
    interrupt_enable(SYS_INT_ADC_TSC_GENINT);
}


static VOID idle_step_init(VOID)
{


    /* Turn off all the Analog Supply to touchscreen X.  */
    TSCADCIdleStepAnalogSupplyConfig(SOC_ADC_TSC_0_REGS, TSCADC_XPPSW_PIN_OFF,
                                     TSCADC_XNPSW_PIN_OFF, TSCADC_YPPSW_PIN_OFF);

    /* Ground the touchscreen Y. */
    TSCADCIdleStepAnalogGroundConfig(SOC_ADC_TSC_0_REGS, TSCADC_XNNSW_PIN_OFF,
                                     TSCADC_YPNSW_PIN_ON, TSCADC_YNNSW_PIN_ON,
                                     TSCADC_WPNSW_PIN_OFF);
}

static VOID ts_charge_step_init(VOID)
{


    /* Turn off all the Analog Supply to touchscreen X.  */
    TSCADCChargeStepAnalogSupplyConfig(SOC_ADC_TSC_0_REGS, TSCADC_XPPSW_PIN_OFF,
                                       TSCADC_XNPSW_PIN_OFF, TSCADC_XPPSW_PIN_OFF);

    /* Ground the touchscreen Y. */
    TSCADCChargeStepAnalogGroundConfig(SOC_ADC_TSC_0_REGS, TSCADC_XNNSW_PIN_OFF,
                                       TSCADC_YPNSW_PIN_ON, TSCADC_YNNSW_PIN_ON,
                                       TSCADC_WPNSW_PIN_OFF);

    TSCADCTSChargeStepOpenDelayConfig(SOC_ADC_TSC_0_REGS, 0x500);
    
    TSCADCConfigureStepEnable(SOC_ADC_TSC_0_REGS, 0, 1);
    
}


static VOID step_init(VOID)
{

INT i;


    for (i = 0; i < 8; i++)
    {
        
        /* Configure ADC to Single ended operation mode.  */
        TSCADCTSStepOperationModeControl(SOC_ADC_TSC_0_REGS,
                                        TSCADC_SINGLE_ENDED_OPER_MODE, i);

        /* Configure reference volatage and input for X data sampling.  */ 
        TSCADCTSStepConfig(SOC_ADC_TSC_0_REGS, i, TSCADC_NEGATIVE_REF_XNUR,
                           TSCADC_POSITIVE_INP_CHANNEL3,TSCADC_NEGATIVE_INP_ADCREFM,
                           TSCADC_POSITIVE_REF_XPUL);

        /* Configure the Analog Supply for X data sampling.  */
        TSCADCTSStepAnalogSupplyConfig(SOC_ADC_TSC_0_REGS, TSCADC_XPPSW_PIN_ON,
                                       TSCADC_XNPSW_PIN_OFF, TSCADC_YPPSW_PIN_OFF,
                                       i);

        /* Configure the Analong Ground for X data sampling.  */
        TSCADCTSStepAnalogGroundConfig(SOC_ADC_TSC_0_REGS, TSCADC_XNNSW_PIN_ON,
                                       TSCADC_YPNSW_PIN_OFF, TSCADC_YNNSW_PIN_OFF,
                                       TSCADC_WPNSW_PIN_OFF, i);

        /* Select FIFO 0 for X data.  */
        TSCADCTSStepFIFOSelConfig(SOC_ADC_TSC_0_REGS, i, TSCADC_FIFO_0);

        /* Configure in continuous hardware sync mode.  */
        TSCADCTSStepModeConfig(SOC_ADC_TSC_0_REGS, i, TSCADC_CONTINIOUS_HARDWARE_SYNC);

        TSCADCTSStepAverageConfig(SOC_ADC_TSC_0_REGS, i, TSCADC_SIXTEEN_SAMPLES_AVG); 

        TSCADCTSStepOpenDelayConfig(SOC_ADC_TSC_0_REGS, i, 1);
        TSCADCTSStepSampleDelayConfig(SOC_ADC_TSC_0_REGS, i, 15);
        TSCADCConfigureStepEnable(SOC_ADC_TSC_0_REGS, i + 1, 1);
    }
    
    /* Set more delay for the first Step.  */
    TSCADCTSStepOpenDelayConfig(SOC_ADC_TSC_0_REGS, 0, 0x500);
    
    for (i = 8; i < 16; i++)
    {
        
        /* Configure ADC to Single ended operation mode.  */
        TSCADCTSStepOperationModeControl(SOC_ADC_TSC_0_REGS, 
                                         TSCADC_SINGLE_ENDED_OPER_MODE, i);

        /* Configure reference volatage and input for Y data sampling.  */ 
        TSCADCTSStepConfig(SOC_ADC_TSC_0_REGS, i, TSCADC_NEGATIVE_REF_YNLR,
                           TSCADC_POSITIVE_INP_CHANNEL1, TSCADC_NEGATIVE_INP_ADCREFM,
                           TSCADC_POSITIVE_REF_YPLL);

        /* Configure the Analog Supply for Y data sampling.  */
        TSCADCTSStepAnalogSupplyConfig(SOC_ADC_TSC_0_REGS, TSCADC_XPPSW_PIN_OFF,
                                       TSCADC_XNPSW_PIN_OFF, TSCADC_YPPSW_PIN_ON, i);

        /* Configure the Analong Ground for Y data sampling.  */
        TSCADCTSStepAnalogGroundConfig(SOC_ADC_TSC_0_REGS, TSCADC_XNNSW_PIN_OFF,
                                       TSCADC_YPNSW_PIN_OFF, TSCADC_YNNSW_PIN_ON,
                                       TSCADC_WPNSW_PIN_OFF, i);

        /* Select FIFO 1 for Y data.  */
        TSCADCTSStepFIFOSelConfig(SOC_ADC_TSC_0_REGS, i, TSCADC_FIFO_1);

        /* Configure in continuous hardware sync mode.  */
        TSCADCTSStepModeConfig(SOC_ADC_TSC_0_REGS, i, TSCADC_CONTINIOUS_HARDWARE_SYNC);

        TSCADCTSStepAverageConfig(SOC_ADC_TSC_0_REGS, i, TSCADC_SIXTEEN_SAMPLES_AVG); 
        
        TSCADCTSStepOpenDelayConfig(SOC_ADC_TSC_0_REGS, i, 1);
        TSCADCTSStepSampleDelayConfig(SOC_ADC_TSC_0_REGS, i, 15);
        TSCADCConfigureStepEnable(SOC_ADC_TSC_0_REGS, i + 1, 1);

    }
    
    /* Set more delay for the first Step.  */
    TSCADCTSStepOpenDelayConfig(SOC_ADC_TSC_0_REGS, 8, 0x500);
}







