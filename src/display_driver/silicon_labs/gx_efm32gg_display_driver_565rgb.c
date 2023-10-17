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

#include "bsp.h"
#include "tftdirect.h"

/* prototype generic setup function */
VOID _gx_display_driver_565rgb_setup(GX_DISPLAY *display, VOID *aux_data,
                                     VOID (*toggle_function)(struct GX_CANVAS_STRUCT *canvas,
                                                             GX_RECTANGLE *dirty_area));

#define DISPLAY_WIDTH  320
#define DISPLAY_HEIGHT 240


/**************************************************************************/
/* Configure TFT direct drive from EBI BANK2 */
static const EBI_TFTInit_TypeDef tftInit =
{   
    ebiTFTBank2,                  /* Select EBI Bank 2 */
    ebiTFTWidthHalfWord,          /* Select 2-byte (16-bit RGB565) increments */
    ebiTFTColorSrcMem,            /* Use memory as source for mask/blending */
    ebiTFTInterleaveUnlimited,    /* Unlimited interleaved accesses */
    ebiTFTFrameBufTriggerVSync,   /* VSYNC as frame buffer update trigger */
    false,                        /* Drive DCLK from negative edge of internal clock */
    ebiTFTMBDisabled,             /* No masking and alpha blending enabled */
    ebiTFTDDModeExternal,         /* Drive from external memory */
    ebiActiveLow,                 /* CS Active Low polarity */
    ebiActiveHigh,                /* DCLK Active High polarity */
    ebiActiveLow,                 /* DATAEN Active Low polarity */
    ebiActiveLow,                 /* HSYNC Active Low polarity */
    ebiActiveLow,                 /* VSYNC Active Low polarity */
    DISPLAY_WIDTH,                /* Horizontal size in pixels */
    1,                            /* Horizontal Front Porch */
    30,                           /* Horizontal Back Porch */
    2,                            /* Horizontal Synchronization Pulse Width */
    DISPLAY_HEIGHT,               /* Vertical size in pixels */
    1,                            /* Vertical Front Porch */
    4,                            /* Vertical Back Porch */
    2,                            /* Vertical Synchronization Pulse Width */
    0x0000,                       /* Frame Address pointer offset to EBI memory base */
    8,                            /* DCLK Period */
    0,                            /* DCLK Start cycles */
    0,                            /* DCLK Setup cycles */
    0,                            /* DCLK Hold cycles */
};

/**************************************************************************/
static GX_COLOR                 *frame_buffer_a;
static GX_COLOR                 *frame_buffer_b;
ULONG                           frame_b_offset;

/**************************************************************************/
VOID gx_efm32_frame_buffer_toggle(struct GX_CANVAS_STRUCT *canvas, GX_RECTANGLE *dirty_area)
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
        // buffer_a is working, buffer_b was visible. Switch visible buffer:
        EBI_TFTFrameBaseSet(0);     // buffer_a is now visible
        canvas->gx_canvas_memory = frame_buffer_b;
        pWrite = frame_buffer_b;
    }
    else
    {
        EBI_TFTFrameBaseSet(frame_b_offset); // buffer b is now visible
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

/**************************************************************************/
UINT gx_efm32_graphics_driver_setup_565rgb(GX_DISPLAY *display)
{
    int pixel_count;
    pixel_count = DISPLAY_WIDTH * DISPLAY_HEIGHT;

    /* create default 565rgb display driver configuration */
    _gx_display_driver_565rgb_setup(display, GX_NULL, gx_efm32_frame_buffer_toggle);

    /* Set frame buffer start address */
    frame_buffer_a = (GX_COLOR *) EBI_BankAddress(EBI_BANK2);
    frame_buffer_b = (GX_COLOR *) (EBI_BankAddress(EBI_BANK2) + (pixel_count * 2));

    frame_b_offset = (ULONG) (ULONG) frame_buffer_b - (ULONG) frame_buffer_a;

    memset(frame_buffer_a, 0, pixel_count * 2);
    memset(frame_buffer_b, 0, pixel_count * 2);

    /* call into Silicon Labs BSP to do hardware setup */
    TFT_DirectInit(&tftInit);
    return GX_SUCCESS;
}

/**************************************************************************/
VOID gx_efm32_canvas_memory_pointer_set(GX_CANVAS *canvas)
{
    canvas->gx_canvas_memory = (ULONG *) frame_buffer_b;
}



