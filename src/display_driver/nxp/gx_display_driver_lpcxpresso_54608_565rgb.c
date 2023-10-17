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
/**   Display Component    (Display)                                      */
/**                                                                       */
/**************************************************************************/

#include "tx_api.h"
#include "gx_api.h"

#include "board.h"
#include "fsl_lcdc.h"

#include "pin_mux.h"
#include "fsl_sctimer.h"

/* prototype generic setup function */
VOID _gx_display_driver_565rgb_setup(GX_DISPLAY *display, VOID *aux_data,
                                     VOID (*toggle_function)(struct GX_CANVAS_STRUCT *canvas,
                                     GX_RECTANGLE *dirty_area));

/* The new frame address already loaded to the LCD controller. */
static volatile bool s_frameAddrUpdated = false;

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_LCD LCD
#define LCD_PANEL_CLK 9000000
#define LCD_PPL 480
#define LCD_HSW 2
#define LCD_HFP 8
#define LCD_HBP 43
#define LCD_LPP 272
#define LCD_VSW 10
#define LCD_VFP 4
#define LCD_VBP 12
#define LCD_POL_FLAGS kLCDC_InvertVsyncPolarity | kLCDC_InvertHsyncPolarity
//#define LCD_POL_FLAGS 0
#define DISPLAY_WIDTH  480
#define DISPLAY_HEIGHT 272
#define LCD_INPUT_CLK_FREQ CLOCK_GetFreq(kCLOCK_LCD)
#define APP_LCD_IRQHandler LCD_IRQHandler
#define APP_LCD_IRQn LCD_IRQn
#define APP_BIT_PER_PIXEL 16

#define SDRAM_BASE_ADDR 0xa0000000

#if 0

#if (defined(__CC_ARM) || defined(__GNUC__))
__attribute__((aligned(8)))
#elif defined(__ICCARM__)
#pragma data_alignment = 8
#endif
static GX_COLOR frame_buffer_a[4];

#if (defined(__CC_ARM) || defined(__GNUC__))
__attribute__((aligned(8)))
#elif defined(__ICCARM__)
#pragma data_alignment = 8
#endif
static GX_COLOR frame_buffer_b[4];

#else

static GX_COLOR *frame_buffer_a = (GX_COLOR *) SDRAM_BASE_ADDR;
static GX_COLOR *frame_buffer_b = (GX_COLOR *) (SDRAM_BASE_ADDR + (DISPLAY_WIDTH * DISPLAY_HEIGHT * 2));

#endif

/**************************************************************************/
VOID lpc_54608_frame_buffer_toggle(struct GX_CANVAS_STRUCT *canvas, GX_RECTANGLE *dirty_area)
{
GX_RECTANGLE    Limit;
GX_RECTANGLE    Copy;
ULONG           offset;
INT             copy_width;
INT             copy_height;
INT             row;
INT             src_stride_ulongs;
INT             dest_stride_ulongs;
ULONG           *pRead;
ULONG           *pWrite;

    /* switch visible frame pointer to canvas working memory,
       copy modified portion from working to visible,
       switch canvas working memory to old visible
    */

    pRead = canvas->gx_canvas_memory;

    if (canvas->gx_canvas_memory == frame_buffer_a)
    {
        // buffer_0 is working, buffer_1 was visible. Switch visible buffer:
        LCDC_SetPanelAddr(APP_LCD, kLCDC_UpperPanel, (uint32_t) frame_buffer_a);
        canvas->gx_canvas_memory = frame_buffer_b;
        pWrite = frame_buffer_b;
    }
    else
    {
        LCDC_SetPanelAddr(APP_LCD, kLCDC_UpperPanel, (uint32_t) frame_buffer_b);
        canvas->gx_canvas_memory = frame_buffer_a;
        pWrite = frame_buffer_a;
    }

    gx_utility_rectangle_define(&Limit, 0, 0,
                                canvas->gx_canvas_x_resolution - 1,
                                canvas->gx_canvas_y_resolution - 1);

    if (gx_utility_rectangle_overlap_detect(&Limit, &canvas->gx_canvas_dirty_area, &Copy))
    {
        /* align the copy rectangle on ULONG boundry */
        Copy.gx_rectangle_left &= 0xfffe;
        Copy.gx_rectangle_right |= 0x01;
        copy_width = Copy.gx_rectangle_right - Copy.gx_rectangle_left + 1;
        copy_height = Copy.gx_rectangle_bottom - Copy.gx_rectangle_top + 1;

        offset = Copy.gx_rectangle_top * canvas->gx_canvas_x_resolution;
        offset += Copy.gx_rectangle_left;
        offset /= 2;        // in ulongs
        pRead += offset;

        // offset by canvas offset:
        offset = (canvas->gx_canvas_display_offset_y + Copy.gx_rectangle_top)* DISPLAY_WIDTH;
        offset += canvas->gx_canvas_display_offset_x + Copy.gx_rectangle_left;
        offset /= 2;
        pWrite += offset;
        
        src_stride_ulongs = canvas ->gx_canvas_x_resolution / 2;
        dest_stride_ulongs = DISPLAY_WIDTH / 2;
        copy_width /= 2;     // in ulongs

        for(row = 0; row < copy_height; row++)
        {
            memcpy(pWrite, pRead, copy_width * 4); /* Use case of memcpy is verified. */
            pWrite += dest_stride_ulongs;
            pRead += src_stride_ulongs;
        }
    }
}

/*******************************************************************************
 * Code
 ******************************************************************************/
static void BOARD_InitPWM(void)
{
    sctimer_config_t config;
    sctimer_pwm_signal_param_t pwmParam;
    uint32_t event;

    CLOCK_AttachClk(kMCLK_to_SCT_CLK);

    CLOCK_SetClkDiv(kCLOCK_DivSctClk, 2, true);

    SCTIMER_GetDefaultConfig(&config);

    SCTIMER_Init(SCT0, &config);

    pwmParam.output = kSCTIMER_Out_5;
    pwmParam.level = kSCTIMER_HighTrue;
    pwmParam.dutyCyclePercent = 5;

    SCTIMER_SetupPwm(SCT0, &pwmParam, kSCTIMER_CenterAlignedPwm, 1000U, CLOCK_GetFreq(kCLOCK_Sct), &event);
}

void APP_LCD_IRQHandler(void)
{
    uint32_t intStatus = LCDC_GetEnabledInterruptsPendingStatus(APP_LCD);

    LCDC_ClearInterruptsStatus(APP_LCD, intStatus);

    if (intStatus & kLCDC_BaseAddrUpdateInterrupt)
    {
        s_frameAddrUpdated = true;
    }
    __DSB();
}

static const uint32_t palette[] = {0x001F0000U, 0x7C0003E0U};

void lpc_54608_expresso_lcd_hardware_setup(void)
{
    lcdc_config_t lcdConfig;

    /* Set the back light PWM. */
    BOARD_InitPWM();

    s_frameAddrUpdated = false;

    LCDC_GetDefaultConfig(&lcdConfig);

    lcdConfig.panelClock_Hz = LCD_PANEL_CLK;
    lcdConfig.ppl = LCD_PPL;
    lcdConfig.hsw = LCD_HSW;
    lcdConfig.hfp = LCD_HFP;
    lcdConfig.hbp = LCD_HBP;
    lcdConfig.lpp = LCD_LPP;
    lcdConfig.vsw = LCD_VSW;
    lcdConfig.vfp = LCD_VFP;
    lcdConfig.vbp = LCD_VBP;
    lcdConfig.polarityFlags = LCD_POL_FLAGS;
    lcdConfig.upperPanelAddr = (uint32_t) frame_buffer_a;
    lcdConfig.bpp = kLCDC_16BPP565;
    lcdConfig.display = kLCDC_DisplayTFT;
    //lcdConfig.swapRedBlue = false;
    lcdConfig.swapRedBlue = true;

    LCDC_Init(APP_LCD, &lcdConfig, LCD_INPUT_CLK_FREQ);
    LCDC_SetPalette(APP_LCD, palette, ARRAY_SIZE(palette));

    //LCDC_EnableInterrupts(APP_LCD, kLCDC_BaseAddrUpdateInterrupt);
    //NVIC_EnableIRQ(APP_LCD_IRQn);

    LCDC_Start(APP_LCD);
    LCDC_PowerUp(APP_LCD);
}


/**************************************************************************/
UINT lpc_54608_canvas_memory_initialize(INT layer, GX_CANVAS *canvas)
{
    canvas->gx_canvas_memory = (ULONG *) frame_buffer_b;
    return GX_INVALID_CANVAS;
}


GX_DISPLAY_LAYER_SERVICES lpc_54608_layer_services = {
        lpc_54608_canvas_memory_initialize,
        GX_NULL, GX_NULL, GX_NULL, GX_NULL
};


/**************************************************************************/
UINT lpc_54608_display_driver_setup_565rgb(GX_DISPLAY *display)
{
    int pixel_count;

    pixel_count = DISPLAY_WIDTH * DISPLAY_HEIGHT;

    /* create default 565rgb display driver configuration */
    _gx_display_driver_565rgb_setup(display, GX_NULL, lpc_54608_frame_buffer_toggle);
    display->gx_display_layer_services = &lpc_54608_layer_services;

    BOARD_InitSDRAM();

    // clear the frame buffer memories
    memset(frame_buffer_a, 0, pixel_count * 2);
    memset(frame_buffer_b, 0, pixel_count * 2);

    // configure and turn on the hardware
    lpc_54608_expresso_lcd_hardware_setup();

    return GX_SUCCESS;
}




