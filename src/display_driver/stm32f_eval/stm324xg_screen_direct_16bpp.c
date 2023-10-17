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
/**   Application Interface (API)                                         */
/**                                                                       */
/**************************************************************************/

#include   "tx_api.h"
#include   "gx_api.h"
#include   "stm324xg_eval_lcd.h"


// macros for reading each color component in 5:6:5 format
#define REDVAL(_c) (UCHAR)(((_c) >> 11) & 0x1f)
#define GREENVAL(_c) (UCHAR)(((_c) >> 5) & 0x3f)
#define BLUEVAL(_c) (UCHAR)(((_c) ) & 0x1f)

// macro for assembling a 16-bit r:g:b value from 3 components

#define ASSEMBLECOLOR(_r, _g, _b) \
    ((((_r) & 0x1f) << 11) | \
     (((_g) & 0x3f) << 5) | \
     (((_b) & 0x1f) ))


void stm324xg_16bpp_buffer_toggle(GX_CANVAS *canvas, GX_RECTANGLE *dirty)
{

    return;
}


VOID stm324xg_16bpp_pixel_direct_write(GX_DRAW_CONTEXT *context,
        INT x, INT y, GX_COLOR color)
{
    
    LCD_SetCursor(x, y);
    LCD_WriteRAM_Prepare();
    LCD_WriteRAM(color);
}


VOID stm324xg_16bpp_line_direct_draw(GX_DRAW_CONTEXT *context,
    INT xs, INT ys, INT xe, INT ye)
{
    
INT curx;
INT cury;
INT x_sign;
INT y_sign;
INT decision;
INT nextx;
INT nexty;

INT dx = abs(xe - xs);
INT dy = abs(ye - ys);

GX_RECTANGLE *clip = context->gx_draw_context_clip;
GX_COLOR linecolor = context->gx_draw_context_brush.gx_brush_line_color;


    if (((dx >= dy && (xs > xe)) || ((dy > dx) && ys > ye)))
    {
        GX_SWAP_VALS(xe, xs);
        GX_SWAP_VALS(ye, ys);
    }

    x_sign = ((INT) xe - (INT) xs) / dx;
    y_sign = ((INT) ye - (INT) ys) / dy;

    if (dx >= dy)
    {
        for (curx = xs, cury = ys, nextx = xe, nexty = ye,
             decision = (dx >> 1); curx <= nextx; curx++, nextx--,
             decision += dy)
        {
            if (decision >= dx)
            {
                decision -= dx;
                cury += y_sign;
                nexty -= y_sign;
            }
        
            /* we should test for to see if the entire line fits
               within clipping rectangle, and if so we can 
               remove this from the inner loop. That's an
               optimization.
            */
            if (curx >= clip -> gx_rectangle_left &&
                curx <= clip -> gx_rectangle_right &&
                cury >= clip -> gx_rectangle_top &&
                cury <= clip -> gx_rectangle_bottom)
            {
                stm324xg_16bpp_pixel_direct_write(context, curx, cury, linecolor);
            }

            if (nextx >= clip -> gx_rectangle_left &&
                nextx <= clip -> gx_rectangle_right &&
                nexty >= clip -> gx_rectangle_top &&
                nexty <= clip -> gx_rectangle_bottom)
            {
                stm324xg_16bpp_pixel_direct_write(context, nextx, nexty, linecolor);
            }
        }
    }
    else
    {
        for (curx = xs, cury = ys, nextx = xe, nexty = ye,
             decision = (dy >> 1); cury <= nexty; cury++, nexty--,
             decision += dx)
        {
            if (decision >= dy)
            {
                decision -= dy;
                curx += x_sign;
                nextx -= x_sign;
            }

            if (curx >= clip -> gx_rectangle_left &&
                curx <= clip -> gx_rectangle_right &&
                cury >= clip -> gx_rectangle_top &&
                cury <= clip -> gx_rectangle_bottom)
            {
                stm324xg_16bpp_pixel_direct_write(context, curx, cury, linecolor);
            }

            if (nextx >= clip -> gx_rectangle_left &&
                nextx <= clip -> gx_rectangle_right &&
                nexty >= clip -> gx_rectangle_top &&
                nexty <= clip -> gx_rectangle_bottom)
            {
                stm324xg_16bpp_pixel_direct_write(context, nextx, nexty, linecolor);
            }
        }
    }

}




void stm324xg_16bpp_horizontal_line_direct_draw(GX_DRAW_CONTEXT *context,
    INT xstart, INT xend, INT ypos, INT Width, GX_COLOR Color)
{
    
INT pix_count;


    LCD_WriteReg(LCD_REG_80, xstart);
    LCD_WriteReg(LCD_REG_81, xend);
    LCD_WriteReg(LCD_REG_82, ypos);
    LCD_WriteReg(LCD_REG_83, ypos + Width - 1);
    
    LCD_SetCursor(xstart, ypos);
    LCD_WriteRAM_Prepare();
    
    for (pix_count = (xend - xstart + 1) * Width; pix_count; pix_count--)
    {
        LCD_WriteRAM(Color);
    }
    
    LCD_WriteReg(LCD_REG_80, 0);
    LCD_WriteReg(LCD_REG_81, 239);
    LCD_WriteReg(LCD_REG_82, 0);
    LCD_WriteReg(LCD_REG_83, 319);
}

void stm324xg_16bpp_vertical_line_direct_draw(GX_DRAW_CONTEXT *context,
    INT ystart, INT yend, INT xpos, INT Width, GX_COLOR Color)
{
    
    LCD_WriteReg(LCD_REG_3, 0x1038);
    
    INT row;
    INT column;
    GX_COLOR *put;
    GX_COLOR *rowstart;

    INT len = yend - ystart + 1;
    
    for (row = 0; row < Width; row++)
    {

        LCD_SetCursor(xpos + row, ystart);
        LCD_WriteRAM_Prepare();
        
        for (column = 0; column < len; column++)
        {
            LCD_WriteRAM(Color);
        }
        
    }    
    LCD_WriteReg(LCD_REG_3, 0x1030);
}

void stm324xg_16bpp_pixel_direct_blend(GX_DRAW_CONTEXT *context,
    INT x, INT y, GX_COLOR fcolor, UCHAR alpha)
{
    
    UCHAR fred, fgreen, fblue;
    UCHAR bred, bgreen, bblue;
    UCHAR balpha;

    GX_COLOR bcolor;
    GX_COLOR *put;

    if (alpha > 0)
    {
        fred = REDVAL(fcolor);
        fgreen = GREENVAL(fcolor);
        fblue = BLUEVAL(fcolor);

        LCD_SetCursor(x, y);
        
        bcolor = LCD_ReadRAM();
        
        bred = REDVAL(bcolor);
        bgreen = GREENVAL(bcolor);
        bblue = BLUEVAL(bcolor);

        balpha = 256 - alpha;

        fred = ((bred * balpha) + (fred * alpha)) >> 8;
        fgreen = ((bgreen * balpha) + (fgreen * alpha)) >> 8;
        fblue = ((bblue * balpha) + (fblue * alpha)) >> 8;
        
        LCD_WriteRAM_Prepare();
        LCD_WriteRAM(ASSEMBLECOLOR(fred, fgreen, fblue));
    }

}


UINT stm324xg_graphics_driver_16bpp(GX_DISPLAY *display)
{
    /* initialize the low-level drawing function pointers

       for windows, these are always just the generic funcions,
       but for some hardware, these will be customized,
       optimized functions specific to that hardware
    */

    display -> gx_display_driver_simple_line_draw      = _gx_display_driver_generic_simple_line_draw;
    display -> gx_display_driver_simple_wide_line_draw  = _gx_display_driver_generic_simple_wide_line_draw;
    display -> gx_display_driver_anti_aliased_line_draw = _gx_display_driver_generic_aliased_line_draw;
    display -> gx_display_driver_anti_aliased_wide_line_draw  = _gx_display_driver_generic_aliased_wide_line_draw;
    display -> gx_display_driver_horizontal_line_draw = stm324xg_16bpp_horizontal_line_direct_draw;
    display -> gx_display_driver_vertical_line_draw = stm324xg_16bpp_vertical_line_direct_draw;

    display -> gx_display_driver_pixel_write = stm324xg_16bpp_pixel_direct_write;
    display -> gx_display_driver_pixel_blend = stm324xg_16bpp_pixel_direct_blend;
    display -> gx_display_driver_buffer_toggle = stm324xg_16bpp_buffer_toggle;

    display -> gx_display_handle =                        STM324XG_SCREEN_HANDLE;

    /* Return successful status.  */
    return(GX_SUCCESS);
}

