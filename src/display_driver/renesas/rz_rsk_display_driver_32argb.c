
#include    <stdio.h>
#include    <string.h>

#define DO_VIDEO
#define DO_GRAPHICS

#include    "r_typedefs.h"

#include    "r_vdc5.h"
#include    "r_vdc5_user.h"

#include    "iodefine.h"
#include    "devdrv_intc.h"
#include    "dev_drv.h"

#include    "video_decoder.h"
#include    "graphics.h"
#include    "graphics_drv_wrapper.h"

#include    "gx_api.h"
#include    "gx_display.h"

void  BSP_DCache_FlushRange       (void *, unsigned long);

#define GRAPHICS_CHANNEL            (VDC5_CHANNEL_0)
#define GRAPHICS_FORMAT             (VDC5_GR_FORMAT_ARGB8888)

#define GRAPHICS_LAYER_XRES 800
#define GRAPHICS_LAYER_YRES 480
#define GRAPHICS_LAYER_XSHIFT 0
#define GRAPHICS_LAYER_YSHIFT 0
#define GRAPHICS_LAYER_STRIDE (((GRAPHICS_LAYER_XRES * 4) + 31) & ~31)

#define RZ_SCREEN_HANDLE                0x525A0000

#define     IMGCAP_SIZE_NTSC_HS         (122u * 2u)
#define     IMGCAP_SIZE_NTSC_HW         (720u * 2u)
#define     IMGCAP_SIZE_NTSC_VS         (16u)
#define     IMGCAP_SIZE_NTSC_VW         (480u/2u)

#define     IMGCAP_SIZE_PAL_HS          (132u * 2u)
#define     IMGCAP_SIZE_PAL_HW          (720u * 2u)
#define     IMGCAP_SIZE_PAL_VS          (19u)
#define     IMGCAP_SIZE_PAL_VW          (560u/2u)

/*! Video format: YCbCr422, 2 bytes per pixel */
#define     VIDEO_FORMAT                (VDC5_RES_MD_YCBCR422)
#define VIDEO_LAYER_XRES 800
#define VIDEO_LAYER_YRES 480

/*! Frame buffer stride: Frame buffer stride should be set to a multiple of 32 or 128
 in accordance with the frame buffer burst transfer mode. */
#define     VIDEO_BUFFER_STRIDE         (((LCD_CH0_DISP_HW * 4u) + 31u) & ~31u)
#define     VIDEO_BUFFER_HEIGHT         (IMGCAP_SIZE_PAL_VW)

//#pragma location = ".vram"
static uint8_t FrameBuffer_Video[VIDEO_BUFFER_STRIDE * VIDEO_BUFFER_HEIGHT * 2u];
static GX_COLOR FrameBuffer_Graphics[GRAPHICS_LAYER_STRIDE * GRAPHICS_LAYER_YRES / 4];

static vdc5_gr_disp_sel_t gr_disp_sel[VDC5_GR_TYPE_NUM] =
{
    VDC5_DISPSEL_IGNORED,       /* Graphics 0 */
    VDC5_DISPSEL_IGNORED,       /* Graphics 1 */
    VDC5_DISPSEL_IGNORED,       /* Graphics 2 */
    VDC5_DISPSEL_IGNORED,       /* Graphics 3 */
    VDC5_DISPSEL_IGNORED,       /* VIN */
    VDC5_DISPSEL_IGNORED        /* OIR */
};

/*! Interrupt parameter */
int vsync_count = 0;
vdc5_period_rect_t      period_rect;

///////////////////////////////////////////////////////////////////////////////
static void rz_32argb_InitParameters(vdc5_channel_t ch)
{
	uint32_t index;

	vsync_count = 0;

	for (index = 0; index < (uint32_t) VDC5_GR_TYPE_NUM; index++)
    {
		gr_disp_sel[index] = VDC5_DISPSEL_IGNORED;
	}

	if (ch == VDC5_CHANNEL_0)
    {
		period_rect.vs = (uint16_t) LCD_CH0_DISP_VS;
		period_rect.vw = (uint16_t) LCD_CH0_DISP_VW;
		period_rect.hs = (uint16_t) LCD_CH0_DISP_HS;
		period_rect.hw = (uint16_t) LCD_CH0_DISP_HW;
	}
}


///////////////////////////////////////////////////////////////////////////////
static void rz_32argb_SetVideoDecoder(vdc5_channel_t ch, vdc5_period_rect_t * cap_rect,
		vdec_adc_vinsel_t adc_vinsel)
{
	vdec_channel_t channel;
	graphics_col_sys_t col_sys;

	channel = (ch == VDC5_CHANNEL_0) ? VDEC_CHANNEL_0 : VDEC_CHANNEL_1;

	col_sys = Graphics_VideoDecoderInit(adc_vinsel, channel);
	if (col_sys == GRPH_COL_SYS_UNKNOWN)
    { /* If not detected, ... */
		col_sys = GRPH_COL_SYS_NTSC_358;
	}

	if ((col_sys == GRPH_COL_SYS_NTSC_358)
			|| (col_sys == GRPH_COL_SYS_NTSC_443))
    { /* Size of NTSC video image to be captured */
		cap_rect->vs = (uint16_t) IMGCAP_SIZE_NTSC_VS;
		cap_rect->vw = (uint16_t) IMGCAP_SIZE_NTSC_VW;
		cap_rect->hs = (uint16_t) IMGCAP_SIZE_NTSC_HS;
		cap_rect->hw = (uint16_t) IMGCAP_SIZE_NTSC_HW;
	}
    else
    { /* Size of PAL video image to be captured */
		cap_rect->vs = (uint16_t) IMGCAP_SIZE_PAL_VS;
		cap_rect->vw = (uint16_t) IMGCAP_SIZE_PAL_VW;
		cap_rect->hs = (uint16_t) IMGCAP_SIZE_PAL_HS;
		cap_rect->hw = (uint16_t) IMGCAP_SIZE_PAL_HW;
	}
}


///////////////////////////////////////////////////////////////////////////////
void CopyCanvasToBackBuffer32argb(GX_CANVAS *canvas, GX_RECTANGLE *copy)
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

    gx_utility_rectangle_define(&display_size, 0, 0, GRAPHICS_LAYER_XRES - 1, GRAPHICS_LAYER_YRES - 1);
    copy_clip = *copy;

    gx_utility_rectangle_shift(&copy_clip, canvas->gx_canvas_display_offset_x, canvas->gx_canvas_display_offset_y);
    gx_utility_rectangle_overlap_detect(&copy_clip, &display_size, &copy_clip);
    copy_width = (copy_clip.gx_rectangle_right - copy_clip.gx_rectangle_left + 1);
    copy_height = copy_clip.gx_rectangle_bottom - copy_clip.gx_rectangle_top + 1;

    if (copy_width <= 0 ||
        copy_height <= 0)
    {
        return;
    }
    
    pGetRow = (ULONG *) canvas -> gx_canvas_memory;
    pGetRow += copy->gx_rectangle_top * canvas->gx_canvas_x_resolution;
    pGetRow += copy->gx_rectangle_left;

    pPutRow = FrameBuffer_Graphics;
    pPutRow += (canvas ->gx_canvas_display_offset_y + copy->gx_rectangle_top) * (GRAPHICS_LAYER_STRIDE / 4);
    pPutRow += (canvas ->gx_canvas_display_offset_x + copy->gx_rectangle_left);
    flushaddress = pPutRow;

    for (row = 0; row < copy_height; row++)
    {
        pGet = pGetRow;
        pPut = pPutRow;
        
        for (col = 0; col < copy_width; col++)
        {
            *pPut++ = *pGet++;
        }
        pGetRow += canvas->gx_canvas_x_resolution;
        pPutRow += (GRAPHICS_LAYER_STRIDE / 4);
    }
    BSP_DCache_FlushRange(flushaddress, copy_height * GRAPHICS_LAYER_STRIDE);
}

///////////////////////////////////////////////////////////////////////////////
void rz_32argb_buffer_toggle(GX_CANVAS *canvas, GX_RECTANGLE *dirty)
{
    GX_RECTANGLE Limit;
    GX_RECTANGLE Copy;
        
    gx_utility_rectangle_define(&Limit, 0, 0,
        canvas->gx_canvas_x_resolution -1,
        canvas->gx_canvas_y_resolution -1);
    
    if (gx_utility_rectangle_overlap_detect(&Limit, &canvas->gx_canvas_dirty_area, &Copy))
    {
        /* copy our canvas to the back buffer */
        CopyCanvasToBackBuffer32argb(canvas, &Copy);
    }
}

///////////////////////////////////////////////////////////////////////////////
void fill_graphics_buffer_32argb(unsigned long color)
{
    unsigned long *put_start = (unsigned long *) FrameBuffer_Graphics;
    unsigned long *put;

    int row;
    int col;

    for (row = 0; row < GRAPHICS_LAYER_YRES; row++)
    {
        put = put_start;

        for (col = 0; col < GRAPHICS_LAYER_XRES; col++)
        {
            *put++ = color;
        }
        put_start += GRAPHICS_LAYER_STRIDE / 4;
    }
}


///////////////////////////////////////////////////////////////////////////////
void ConfigureGUIXDisplayHardware32argb(GX_DISPLAY *display)
{
	vdc5_error_t error;

    vdc5_channel_t ch;
	vdc5_channel_t ch_input;

	vdc5_layer_id_t layer_vd_id;
	vdc5_layer_id_t layer_gr_id;
	vdc5_graphics_type_t gr_type_vd;
	vdc5_graphics_type_t gr_type_gr;

	vdc5_res_md_t res_md;
	vdc5_gr_format_t gr_format;
	vdc5_period_rect_t res;
 
    ch = GRAPHICS_CHANNEL;
	layer_vd_id = VDC5_LAYER_ID_0_WR;
	gr_type_vd = VDC5_GR_TYPE_GR0;
	res_md = VIDEO_FORMAT;
	layer_gr_id = VDC5_LAYER_ID_2_RD;
	gr_type_gr = VDC5_GR_TYPE_GR2;
	gr_format = GRAPHICS_FORMAT;

	rz_32argb_InitParameters(ch);

    period_rect.vs = (uint16_t) LCD_CH0_DISP_VS;
	period_rect.vw = (uint16_t) LCD_CH0_DISP_VW;
	period_rect.hs = (uint16_t) LCD_CH0_DISP_HS;
	period_rect.hw = (uint16_t) LCD_CH0_DISP_HW;

	if (layer_vd_id == VDC5_LAYER_ID_0_WR)
    {
		ch_input = ch;
	}
    else
    {
		ch_input = (ch == VDC5_CHANNEL_0) ? VDC5_CHANNEL_1 : VDC5_CHANNEL_0;
	}

	rz_32argb_SetVideoDecoder(ch_input, &res, VDEC_ADC_VINSEL_VIN1);
	if (ch_input == VDC5_CHANNEL_0)
    {
		error = GrpDrv_Init(ch, VDC5_ON, VDC5_OFF);
	}
    else
    {
		error = GrpDrv_Init(ch, VDC5_OFF, VDC5_ON);
	}
	if (error != VDC5_OK)
    {
		return;
	}

#ifdef DO_VIDEO
	/* Create video surface */
	error = GrpDrv_VideoCreateSurface(ch, layer_vd_id, FrameBuffer_Video,
			(uint32_t) VIDEO_BUFFER_STRIDE, /* 2 bytes per pixel */
			res_md, &res, &period_rect);

	if (error != VDC5_OK)
    {
		return;
	}
#endif

	/* Create graphics surface */
    res.hs = LCD_CH0_DISP_HS + GRAPHICS_LAYER_XSHIFT;
    res.hw = GRAPHICS_LAYER_XRES;
    res.vs = LCD_CH0_DISP_VS + GRAPHICS_LAYER_YSHIFT;
    res.vw = GRAPHICS_LAYER_YRES;

    gr_format = GRAPHICS_FORMAT;

#ifdef DO_GRAPHICS
	error = GrpDrv_GraphicsCreateSurface(ch, layer_gr_id, FrameBuffer_Graphics,
			(uint32_t) GRAPHICS_LAYER_STRIDE,
			gr_format, &res);
	if (error != VDC5_OK)
    {
		return;
	}
#endif


#ifdef DO_VIDEO
	gr_disp_sel[gr_type_vd] = VDC5_DISPSEL_LOWER;
#endif


#ifdef DO_GRAPHICS
#ifdef DO_VIDEO
	gr_disp_sel[gr_type_gr] = VDC5_DISPSEL_BLEND;
#else
    gr_disp_sel[gr_type_gr] = VDC5_DISPSEL_CURRENT;
#endif
#endif

	/* Start surfaces */

	error = GrpDrv_StartSurfaces(ch, gr_disp_sel);
	if (error != VDC5_OK)
    {
		return;
	}

    fill_graphics_buffer_32argb(0);
}

///////////////////////////////////////////////////////////////////////////////
UINT rz_graphics_driver_setup_32argb(GX_DISPLAY *display)
{
    _gx_display_driver_32argb_setup(display, GX_NULL, rz_32argb_buffer_toggle);
    display -> gx_display_handle =                       RZ_SCREEN_HANDLE;
    
    ConfigureGUIXDisplayHardware32argb(display);
    return GX_SUCCESS;
}
    