
#include    <stdio.h>
#include    <string.h>

#include    "r_typedefs.h"

#include    "r_vdc5.h"
#include    "r_vdc5_user.h"

#include    "rea_iodefine.h"
//#include    "devdrv_intc.h"
#include    "dev_drv.h"

#include    "graphics.h"
#include    "graphics_drv_wrapper.h"
#include    "gx_api.h"
#include    "gx_display.h"

#define DISPLAY_XRES 1280
#define DISPLAY_YRES 800
#define RZ_SCREEN_HANDLE                0x525A0000
#define FRAME_BUFFER_SIZE (DISPLAY_XRES * DISPLAY_YRES * 2)

//#define DOUBLE_BUFFER

#define DISPLAY_BACKGROUND_COLOR        0x8418
static ULONG frame_buffer1[DISPLAY_XRES * DISPLAY_YRES / 2];

#ifdef DOUBLE_BUFFER
static ULONG frame_buffer2[DISPLAY_XRES * DISPLAY_YRES / 2];
static vdc5_read_chg_t  read_chg;
#endif

void  BSP_DCache_FlushRange       (void *, unsigned long);

int     visible_frame_buffer = 0;

/*****************************************************************************/
void CopyCanvasToBackBuffer16bpp(GX_CANVAS *canvas, GX_RECTANGLE *copy)
{
    GX_RECTANGLE display_size;
    GX_RECTANGLE copy_clip;
    void   *flushaddress;
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
    
    pGetRow = (ULONG *) canvas -> gx_canvas_memory;
    pGetRow += copy->gx_rectangle_top * (canvas->gx_canvas_x_resolution / 2);
    pGetRow += copy->gx_rectangle_left / 2;

    #if defined(DOUBLE_BUFFER)
    if (visible_frame_buffer == 0)
    {
        pPutRow = frame_buffer2;    // copy into buffer 2
    }
    else
    {
        pPutRow = frame_buffer1;    // copy into buffer 1
    }
    #else
        pPutRow = frame_buffer1;    // copy into buffer 1
    #endif

    pPutRow += (canvas ->gx_canvas_display_offset_y + copy->gx_rectangle_top) * (DISPLAY_XRES / 2);
    pPutRow += (canvas ->gx_canvas_display_offset_x + copy->gx_rectangle_left) / 2;;
    flushaddress = pPutRow;

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
    BSP_DCache_FlushRange(flushaddress, copy_height * DISPLAY_XRES * 2);
}

/*****************************************************************************/
void rz_16bpp_buffer_toggle(GX_CANVAS *canvas, GX_RECTANGLE *dirty)
{
    GX_RECTANGLE Limit;
    GX_RECTANGLE Copy;

    gx_utility_rectangle_define(&Limit, 0, 0,
        canvas->gx_canvas_x_resolution -1,
        canvas->gx_canvas_y_resolution -1);
    
    if (gx_utility_rectangle_overlap_detect(&Limit, &canvas->gx_canvas_dirty_area, &Copy))
    {
        /* copy our canvas to the back buffer */
        CopyCanvasToBackBuffer16bpp(canvas, &Copy);

        #ifdef DOUBLE_BUFFER
        /* switch the visible hardware frame buffer */

        if (visible_frame_buffer == 0)
        {
            read_chg.gr_base = frame_buffer2;        /* Frame buffer base address */
            visible_frame_buffer = 1;
        }
        else
        {
            read_chg.gr_base = frame_buffer1;
            visible_frame_buffer = 0;
        }
        read_chg.width_read_fb  = NULL;             /* Width of the image read from frame buffer */
        read_chg.gr_grc         = NULL;             /* Display area */
        read_chg.gr_disp_sel    = NULL;             /* Graphics display mode */
        /* Change read process */
         R_VDC5_ChangeReadProcess(VDC5_CHANNEL_1, VDC5_LAYER_ID_1_RD, &read_chg);

        /* copy canvas to new back buffer */
        CopyCanvasToBackBuffer16bpp(canvas, &Copy);
        #endif
    }
}

/*****************************************************************************/
void ConfigureGUIXDisplayHardware16bpp(GX_DISPLAY *display)
{
    vdc5_channel_t          ch;
    vdc5_gr_format_t        gr_format;
    uint32_t                fb_stride;
    vdc5_period_rect_t      period_rect;
    vdc5_layer_id_t         layer_id;
    vdc5_graphics_type_t    graphics_type;
    void                   *framebuff;      
    int                     loop;

    USHORT                  *put;
    
    vdc5_gr_disp_sel_t      gr_disp_sel[VDC5_GR_TYPE_NUM] =
    {
        VDC5_DISPSEL_IGNORED,       /* Graphics 0 */
        VDC5_DISPSEL_IGNORED,       /* Graphics 1 */
        VDC5_DISPSEL_IGNORED,       /* Graphics 2 */
        VDC5_DISPSEL_IGNORED,       /* Graphics 3 */
        VDC5_DISPSEL_IGNORED,       /* VIN */
        VDC5_DISPSEL_IGNORED        /* OIR */
    };

    vdc5_error_t            error;
 
    ch = VDC5_CHANNEL_1;

    gr_format       = VDC5_GR_FORMAT_RGB565;
    fb_stride       = (uint32_t)(DISPLAY_XRES * 2);     /* bytes per pixel */  
    layer_id        = VDC5_LAYER_ID_1_RD;
    graphics_type   = VDC5_GR_TYPE_GR1;          
    
    /* Frame buffer stride should be set to a multiple of 32 or 128
       in accordance with the frame buffer burst transfer mode. */
    fb_stride       += 31u;
    fb_stride       &= ~31u;
    
    framebuff = frame_buffer1;
    
    /* VDC5 Initialization */
    error = GrpDrv_Init(ch, VDC5_OFF, VDC5_OFF);
    if (error != VDC5_OK)
    {
        printf("VDC5 GrpDrv_Init Error.\n");
        return;
    }
    
    period_rect.vs  = (uint16_t)LCD_CH1_DISP_VS;
    period_rect.vw  = (uint16_t)LCD_CH1_DISP_VW;
    period_rect.hs  = (uint16_t)LCD_CH1_DISP_HS;
    period_rect.hw  = (uint16_t)LCD_CH1_DISP_HW;

    /* Create graphics surface */
    error = GrpDrv_GraphicsCreateSurface(ch, layer_id, framebuff, fb_stride, gr_format, &period_rect);
    if (error != VDC5_OK)
    {
        return;
    }
    
    gr_disp_sel[graphics_type] = VDC5_DISPSEL_CURRENT;

    /* Start surface */
    error = GrpDrv_StartSurfaces(ch, &gr_disp_sel[0]);
    if (error != VDC5_OK)
    {
        return;
    }    

    put = (USHORT *) frame_buffer1;
    for (loop = 0; loop < DISPLAY_XRES * DISPLAY_YRES; loop++)
    {
        *put++ = DISPLAY_BACKGROUND_COLOR;
    }

    #ifdef DOUBLE_BUFFER
    put = (USHORT *) frame_buffer2;
    for (loop = 0; loop < DISPLAY_XRES * DISPLAY_YRES; loop++)
    {
        *put++ = DISPLAY_BACKGROUND_COLOR;
    }
    #endif
}

/*****************************************************************************/
UINT rz_graphics_driver_setup_16bpp(GX_DISPLAY *display)
{
    _gx_display_driver_16bpp_setup(display, GX_NULL, rz_16bpp_buffer_toggle);
    
    display -> gx_display_handle =                       RZ_SCREEN_HANDLE;
    
    ConfigureGUIXDisplayHardware16bpp(display);
    return GX_SUCCESS;
}
    