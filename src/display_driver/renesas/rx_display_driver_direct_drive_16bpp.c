
#include "tx_api.h"
#include "gx_api.h"
#include "gx_display.h"

#include "iodefine.h"
#include "r_ddlcd.h"
#include "r_DDLCD_public.h"
#include "r_DDLCD_private.h"


#define DISPLAY_XRES 480
#define DISPLAY_YRES 272

#define RX_SCREEN_HANDLE                0x525A0000
#define DISPLAY_BACKGROUND_COLOR        0x03e0
#define TEST_COLOR    0x8210
#define FRAME_BUFFER_SIZE (DISPLAY_XRES * DISPLAY_YRES * 2)

int     visible_frame = 1;
ULONG *pFrameBufferA;
ULONG *pFrameBufferB;

extern TX_THREAD _gx_system_thread;

/*****************************************************************************/
void CopyVisibleToBackBuffer16bpp(GX_CANVAS *canvas, GX_RECTANGLE *copy)
{
    GX_RECTANGLE display_size;
    GX_RECTANGLE copy_clip;
    ULONG *pPutRow;
    ULONG *pGetRow;
    ULONG *pGet;
    ULONG *pPut;
    int row;
    int col;
    int copy_width;
    int copy_height;

    gx_utility_rectangle_define(&display_size, 0, 0, DISPLAY_XRES - 1, DISPLAY_YRES - 1);
    copy_clip = *copy;
    copy_clip.gx_rectangle_left &= 0xfffe;
    copy_clip.gx_rectangle_right += 1;

    gx_utility_rectangle_shift(&copy_clip, canvas->gx_canvas_display_offset_x, canvas->gx_canvas_display_offset_y);
    gx_utility_rectangle_overlap_detect(&copy_clip, &display_size, &copy_clip);
    copy_width = (copy_clip.gx_rectangle_right - copy_clip.gx_rectangle_left + 1) / 2;
    copy_height = copy_clip.gx_rectangle_bottom - copy_clip.gx_rectangle_top + 1;

    if (copy_width <= 0 ||
        copy_height <= 0)
    {
        return;
    }
    
    if (visible_frame == 1)
    {
        pGetRow = pFrameBufferB;
        pPutRow = pFrameBufferA;
    }
    else
    {
        pGetRow = pFrameBufferA;
        pPutRow = pFrameBufferB;
    }
    
    pGetRow += copy->gx_rectangle_top * (canvas->gx_canvas_x_resolution / 2);
    pGetRow += copy->gx_rectangle_left / 2;

    pPutRow += (canvas ->gx_canvas_display_offset_y + copy->gx_rectangle_top) * (DISPLAY_XRES / 2);
    pPutRow += (canvas ->gx_canvas_display_offset_x + copy->gx_rectangle_left) / 2;;

    for (row = 0; row < copy_height; row++)
    {
        pGet = pGetRow;
        pPut = pPutRow;
        
        for (col = 0; col < copy_width; col++)
        {
            *pPut++ = *pGet++;
        }
        pGetRow += canvas->gx_canvas_x_resolution / 2;
        pPutRow += DISPLAY_XRES / 2;
    }
}

/*****************************************************************************/
void rx_16bpp_buffer_toggle(GX_CANVAS *canvas, GX_RECTANGLE *dirty)
{
    GX_RECTANGLE Limit;
    GX_RECTANGLE Copy;

    gx_utility_rectangle_define(&Limit, 0, 0,
        canvas->gx_canvas_x_resolution -1,
        canvas->gx_canvas_y_resolution -1);
    
    if (gx_utility_rectangle_overlap_detect(&Limit, &canvas->gx_canvas_dirty_area, &Copy))
    {
        if (visible_frame == 0)
        {
            R_DDLCD_SetActiveRaster(1);
            visible_frame = 1;
            canvas ->gx_canvas_memory = pFrameBufferA;
        }
        else
        {
            R_DDLCD_SetActiveRaster(0);
            visible_frame = 0;
            canvas->gx_canvas_memory = pFrameBufferB;
        }

        /* copy modified portion of visible buffer to new back buffer */
        CopyVisibleToBackBuffer16bpp(canvas, &Copy);
    }
}

/*****************************************************************************/
void ConfigureGUIXDisplayHardware16bpp(GX_DISPLAY *display)
{
    int loop;
    R_DDLCD_Init();
    R_DDLCD_SetFrameRate(60);
    R_DDLCD_Backlight(1);

    pFrameBufferA = (ULONG *) R_DDLCD_SetActiveRaster(0);
    pFrameBufferB = (ULONG *) R_DDLCD_SetActiveRaster(1);

    USHORT *put1 = (USHORT *) pFrameBufferA;
    USHORT *put2 = (USHORT *) pFrameBufferB;

    for (loop = 0; loop < DISPLAY_XRES * DISPLAY_YRES; loop++)
    {
        *put1++ = DISPLAY_BACKGROUND_COLOR;
        *put2++ = TEST_COLOR;
    }
}

/*****************************************************************************/
UINT rx_graphics_driver_setup_16bpp(GX_DISPLAY *display)
{
    _gx_display_driver_16bpp_setup(display, GX_NULL, rx_16bpp_buffer_toggle);
    display -> gx_display_handle =                       RX_SCREEN_HANDLE;
    
    ConfigureGUIXDisplayHardware16bpp(display);
    return GX_SUCCESS;
}

/*****************************************************************************/
void _rx_graphics_driver_ExMemory_isr(void)
{
    DD_TMR_ICU(IEN,DOTCLK) = 0;
    if (DDLCD_BusActive)
    {
        tx_thread_suspend(&_gx_system_thread);
    }
    else
    {
        tx_thread_resume(&_gx_system_thread);
    }
}     
    