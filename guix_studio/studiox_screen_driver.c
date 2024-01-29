/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation 
 * 
 * This program and the accompanying materials are made available under the
 * terms of the MIT License which is available at
 * https://opensource.org/licenses/MIT.
 * 
 * SPDX-License-Identifier: MIT
 **************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** GUIX Component                                                        */
/**                                                                       */
/**   Win32 Display Management (Display)                                  */
/**                                                                       */
/**************************************************************************/
#define GX_SOURCE_CODE

#include "tx_api.h"
/*#include "tx_thread.h"*/
#include "gx_api.h"
#include "gx_system.h"
#include "gx_widget.h"
#include "gx_window.h"
#include "gx_utility.h"
#include "gx_display.h"
#include "gx_canvas.h"

#include "windows.h"
#include "gx_win32_studio_display_driver.h"
#include "gx_dave2d_simulation_display_driver.h"
#include "studiox_constants.h"
#include "studiox_screen_driver.h"

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

GX_DISPLAY      target_win_display;
GX_CANVAS       target_win_canvas;
GX_WINDOW_ROOT  target_win_root_window;

void           *target_win_canvas_memory = NULL;
HWND            target_win_handle;
extern HPALETTE gx_palette_handle;
UINT            win32_graphics_driver_setup_24xrgb(GX_DISPLAY *graphic);
VOID            win32_graphics_driver_cleanup_24xrgb(GX_DISPLAY *graphic);
UINT            win32_graphics_driver_setup_565rgb(GX_DISPLAY *graphic);
VOID            win32_graphics_driver_cleanup_565rgb(GX_DISPLAY *graphic);
UINT            win32_graphics_driver_setup_4444argb(GX_DISPLAY *display);
VOID            win32_graphics_driver_cleanup_4444argb(GX_DISPLAY *graphic);
UINT            win32_graphics_driver_setup_565bgr(GX_DISPLAY *graphic);
VOID            win32_graphics_driver_cleanup_565bgr(GX_DISPLAY *graphic);
UINT            win32_graphics_driver_setup_1555xrgb(GX_DISPLAY *display);
VOID            win32_graphics_driver_cleanup_1555xrgb(GX_DISPLAY *display);
UINT            win32_graphics_driver_setup_8bit_palette(GX_DISPLAY *graphic);
VOID            win32_graphics_driver_cleanup_8bit_palette(GX_DISPLAY *graphic);
UINT            win32_graphics_driver_setup_332rgb(GX_DISPLAY *display);
VOID            win32_graphics_driver_cleanup_332rgb(GX_DISPLAY *display);
UINT            win32_graphics_driver_setup_4bpp_grayscale(GX_DISPLAY *display);
VOID            win32_graphics_driver_cleanup_4bpp_grayscale(GX_DISPLAY *display);
UINT            win32_graphics_driver_setup_monochrome(GX_DISPLAY *graphic);
VOID            win32_graphics_driver_cleanup_monochrome(GX_DISPLAY *graphic);

UINT win32_dave2d_graphics_driver_setup_8bit_palette(GX_DISPLAY *display);
UINT win32_dave2d_graphics_driver_setup_565rgb(GX_DISPLAY *display);
UINT win32_dave2d_graphics_driver_setup_24xrgb(GX_DISPLAY *display);
UINT win32_chromeart_graphics_driver_setup_565rgb(GX_DISPLAY *display);

BOOL guix_configure_canvas(int xres, int yres, int colorformat, int cpu_type, int IsSynergyD2D, GX_COLOR *palette, int pal_size);
UINT gx_system_focus_claim_standard(GX_WIDGET *widget);
HWND GetGuixWinHandle(void);

/*LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);*/
/**************************************************************************/
VOID gx_studio_buffer_toggle_stub(GX_CANVAS *canvas, GX_RECTANGLE *dirty)
{
}

/**************************************************************************/
VOID guix_canvas_flush(HDC win_device, int xpos, int ypos,
                       GX_CANVAS *canvas, GX_RECTANGLE *dirty, int scale, GRID_SNAP_INFO *gsi)
{
INT                           Top;
INT                           Left;
INT                           Height;
INT                           Width;
//HPALETTE      old_palette;
GX_DISPLAY                   *display;
GX_RECTANGLE                  Limit;
GX_RECTANGLE                  Copy;

GX_WIN32_DISPLAY_DRIVER_DATA *driver_instance;
VOID                         *memptr;
HDC                           hdcMem;
HBITMAP                       hbmMem;
HANDLE                        hOld;
INT                           copy_width;
INT                           copy_height;
INT                           canvas_width;
INT                           canvas_height;
INT                           row;
INT                           col;

    display = canvas -> gx_canvas_display;
    driver_instance = (GX_WIN32_DISPLAY_DRIVER_DATA *)(display -> gx_display_driver_data);

    _gx_utility_rectangle_define(&Limit, 0, 0,
                                 canvas -> gx_canvas_x_resolution - 1,
                                 canvas -> gx_canvas_y_resolution - 1);

    if (dirty)
    {
        if (!_gx_utility_rectangle_overlap_detect(&Limit, dirty, &Copy))
        {
            return;
        }
    }
    else
    {
        Copy = Limit;
    }
    SetMapMode(win_device, MM_TEXT);

    Top = Copy.gx_rectangle_top;
    Left = Copy.gx_rectangle_left;
    Width = Copy.gx_rectangle_right - Copy.gx_rectangle_left + 1;
    Height = Copy.gx_rectangle_bottom - Copy.gx_rectangle_top + 1;

    memptr = _win32_canvas_memory_prepare(canvas, &Copy);

    copy_width = Width * scale / 100;
    copy_height = Height * scale / 100;

    Left = Left * scale / 100;
    Top = Top * scale / 100;
    xpos = xpos * scale / 100;
    ypos = ypos * scale / 100;

    int grid_space = gsi->grid_space;

    if (grid_space || gsi->crosshair_enabled || gsi->snap_line_count)
    {
        canvas_width = canvas->gx_canvas_x_resolution * scale / 100;
        canvas_height = canvas->gx_canvas_y_resolution * scale / 100;

        /* Create an off-screen DC for double-buffering */
        hdcMem = CreateCompatibleDC(win_device);
        hbmMem = CreateCompatibleBitmap(win_device, canvas_width, canvas_height);
        hOld = SelectObject(hdcMem, hbmMem);

        /* Flush canvas memory to off-screen DC. */
        StretchDIBits(hdcMem, Left + xpos, Top + ypos, copy_width, copy_height,
            Copy.gx_rectangle_left,
            Copy.gx_rectangle_top + Height + 1,
            Width, -Height, memptr,
            (BITMAPINFO*)&(driver_instance->win32_driver_bmpinfo),
            DIB_RGB_COLORS,
            SRCCOPY);

        if (grid_space)
        {
            grid_space *= scale / 100;

            /* Draw grid. */
            for (col = grid_space; col < canvas_height; col += grid_space)
            {
                for (row = grid_space; row < canvas_width; row += grid_space)
                {
                    SetPixel(hdcMem, row, col, RGB(128, 128, 128));
                }
            }
        }


        if (gsi->crosshair_enabled)
        {
            // Draw crosshair
            SelectObject(hdcMem, GetStockObject(DC_PEN));
            SetDCPenColor(hdcMem, RGB(128, 128, 128));

            col = (gsi->crosshair_cx - gsi->crosshair_size) * scale / 100;
            row = gsi->crosshair_cy * scale / 100;

            MoveToEx(hdcMem, col, row, NULL);

            col = (gsi->crosshair_cx + gsi->crosshair_size + 1) * scale / 100;
            LineTo(hdcMem, col, row);

            col = gsi->crosshair_cx * scale / 100;
            row = (gsi->crosshair_cy - gsi->crosshair_size) * scale / 100;
            MoveToEx(hdcMem, col, row, NULL);

            row = (gsi->crosshair_cy + gsi->crosshair_size + 1) * scale / 100;
            LineTo(hdcMem, col, row);
        }

        if (gsi->snap_line_count)
        {
            // Draw snap lines;
            SelectObject(hdcMem, GetStockObject(DC_PEN));
            SetDCPenColor(hdcMem, RGB(255, 0, 0));

            SNAPLINE_INFO *snapline;

            for (int index = 0; index < gsi->snap_line_count; index++)
            {
                snapline = &gsi->snap_lines[index];

                if (snapline->direction == SNAP_LINE_DIRECTION_VERTICAL)
                {
                    // Draw vertical line
                    col = snapline->target_x_y * scale / 100;
                    row = snapline->target_top_left * scale / 100;

                    MoveToEx(hdcMem, col, row, NULL);

                    row = (snapline->target_bottom_right + 1) * scale / 100;

                    LineTo(hdcMem, col, row);
                }
                else
                {
                    // Draw horizontal line
                    col = snapline->target_top_left * scale / 100;
                    row = snapline->target_x_y * scale / 100;

                    MoveToEx(hdcMem, col, row, NULL);

                    col = (snapline->target_bottom_right + 1) * scale / 100;

                    LineTo(hdcMem, col, row);
                }
            }
        }

        /* Transfer the off-screen DC to the screen. */
        BitBlt(win_device, Left + xpos, Top + ypos, copy_width, copy_height, hdcMem, Left + xpos, Top + ypos, SRCCOPY);

        /* Free-up the off-screen DC.  */
        SelectObject(hdcMem, hOld);
        DeleteObject(hbmMem);
        DeleteDC(hdcMem);
    }
    else
    {
        /* Flush canvas memory to the screen. */
        StretchDIBits(win_device, Left + xpos, Top + ypos, copy_width, copy_height,
                      Copy.gx_rectangle_left,
                      Copy.gx_rectangle_top + Height + 1,
                      Width, -Height, memptr,
                      (BITMAPINFO *)&(driver_instance->win32_driver_bmpinfo),
                      DIB_RGB_COLORS,
                      SRCCOPY);
    }
}

/**************************************************************************/
void guix_bitmap_flush(HDC win_device, int xpos, int ypos, int Width,
                       VOID *memptr, GX_BMP_INFO *bmpinfo)
{
INT Height;

    SetMapMode(win_device, MM_TEXT);

    Height = bmpinfo -> gx_bmp_header.biHeight;

    StretchDIBits(win_device, xpos, ypos, Width, Height,
                  0, Height + 1, Width, -Height, memptr,
                  (BITMAPINFO *)bmpinfo,
                  DIB_RGB_COLORS,
                  SRCCOPY);
}

/**************************************************************************/
void guix_canvas_paint(HDC dc, GX_RECTANGLE dirty, int scale, GRID_SNAP_INFO *gsi)
{
GX_CANVAS      *canvas;

    canvas = &target_win_canvas;

    /* Call the driver buffer toggle function for this canvas.  */
    #if 0
    _gx_utility_rectangle_define(&dirty, 0, 0,
                                 canvas -> gx_canvas_x_resolution - 1,
                                 canvas -> gx_canvas_y_resolution - 1);
            
    #else
        //dirty = canvas ->gx_canvas_dirty_area;
    #endif
    guix_canvas_flush(dc, 0, 0, canvas, &dirty, scale, gsi);
}

/**************************************************************************/
VOID *guix_memory_allocate(ULONG size)
{
    VOID *pointer;
    pointer = malloc(size);
    return pointer;
}

/**************************************************************************/
VOID guix_memory_free(VOID *mem)
{
    free(mem);
}

/**************************************************************************/
HWND GetGuixWinHandle(void)
{
    return target_win_handle;
}

/**************************************************************************/
void initialize_guix(HWND target_win)
{
    target_win_handle = target_win;
    _gx_system_initialize();

    _gx_system_memory_allocator_set(guix_memory_allocate, guix_memory_free);
}

int gx_main(int argc, char ** argv)
{
  return(0);
}

/**************************************************************************/
INT RunGuixDisplaySetupFunction(GX_DISPLAY *display, char *name, int xres, int yres,
                                int colorformat, int target_cpu, int IsRenesasDave2D, int IsDave2DFontFormat,
                                GX_COLOR *palette, int palsize, int aliased_font_palsize)
{
UINT (*setup_func)(GX_DISPLAY *);
INT  memsize = xres * yres;

    setup_func = GX_NULL;

    /* figure out which setup function to run */
    switch (colorformat)
    {
    case GX_COLOR_FORMAT_MONOCHROME:
    case GX_COLOR_FORMAT_MONOCHROME_INVERTED:
        setup_func = win32_graphics_driver_setup_monochrome;
        memsize = ((xres + 7) >> 3) * yres;
        break;

    case GX_COLOR_FORMAT_2BIT_GRAY:
    case GX_COLOR_FORMAT_2BIT_GRAY_INVERTED:
    case GX_COLOR_FORMAT_4BIT_GRAY_INVERTED:
    case GX_COLOR_FORMAT_4BIT_VGA:
    case GX_COLOR_FORMAT_8BIT_GRAY:
    case GX_COLOR_FORMAT_8BIT_GRAY_INVERTED:
        /* FIXME: These formats are not done yet */
        break;

    case GX_COLOR_FORMAT_4BIT_GRAY:
        setup_func = win32_graphics_driver_setup_4bpp_grayscale;
        memsize = ((xres + 1) >> 1) * yres;
        break;

    case GX_COLOR_FORMAT_8BIT_PALETTE:
        if (IsDave2DFontFormat)
        {
            setup_func = win32_dave2d_graphics_driver_setup_8bit_palette;
        }
        else
        {
            setup_func = win32_graphics_driver_setup_8bit_palette;
        }
        break;

    case GX_COLOR_FORMAT_8BIT_PACKED_PIXEL: 
        setup_func = win32_graphics_driver_setup_332rgb;
        break;

    case GX_COLOR_FORMAT_5551BGRX:
    case GX_COLOR_FORMAT_1555XRGB:
        if (!IsRenesasDave2D)
        {
            setup_func = win32_graphics_driver_setup_1555xrgb;
        }
        memsize *= 2;
        break;

    case GX_COLOR_FORMAT_565RGB:
        switch(target_cpu)
        {
        case CPU_SYNERGY:
        case CPU_RA:
        case CPU_RX:
            if (IsRenesasDave2D)
            {
                setup_func = win32_dave2d_graphics_driver_setup_565rgb;
            }
            else
            {
                setup_func = win32_graphics_driver_setup_565rgb;
            }
            break;

        case CPU_ST_CHROMEART:
            setup_func = win32_chromeart_graphics_driver_setup_565rgb;
            break;
        
        default:
            setup_func = win32_graphics_driver_setup_565rgb;
            break;
        }
        memsize *= 2;
        break;

    case GX_COLOR_FORMAT_4444ARGB:
    case GX_COLOR_FORMAT_4444BGRA:
        setup_func = win32_graphics_driver_setup_4444argb;
        memsize *= 2;
        break;

    case GX_COLOR_FORMAT_565BGR:
        setup_func = win32_graphics_driver_setup_565bgr;
        memsize *= 2;
        break;

    case GX_COLOR_FORMAT_24RGB:
    case GX_COLOR_FORMAT_24BGR:
    case GX_COLOR_FORMAT_24XRGB:
    case GX_COLOR_FORMAT_24BGRX:
    case GX_COLOR_FORMAT_32ARGB:
    case GX_COLOR_FORMAT_32BGRA:
        /* counts go in the aux data array: */
        if (IsRenesasDave2D)
        {
            setup_func = win32_dave2d_graphics_driver_setup_24xrgb;
        }
        else
        {
            setup_func = win32_graphics_driver_setup_24xrgb;
        }
        memsize *= 4;
        break;
    }

    if (setup_func)
    {
        if (_gx_display_create(display, name, setup_func, xres, yres) == GX_SUCCESS)
        {
            if (colorformat == GX_COLOR_FORMAT_8BIT_PALETTE)
            {
                if (!IsDave2DFontFormat)
                {
                    if (aliased_font_palsize == 8)
                    {
                        display->gx_display_driver_4bit_glyph_draw = _gx_display_driver_8bpp_glyph_3bit_draw;
                    }
                    else
                    {
                        display->gx_display_driver_4bit_glyph_draw = _gx_display_driver_8bpp_glyph_4bit_draw;
                    }
                }
            }
        
            return memsize;
        }
    }
    return 0;
}

/**************************************************************************/
INT guix_create_app_display(GX_DISPLAY *display, char *name,
                               int xres, int yres,  int colorformat,
                               int target_cpu, int IsRenesasDave2D, int IsDave2DFontFormat,
                               GX_COLOR *palette, int palsize, int aliased_font_palsize)
{
INT  memsize;

    memsize = RunGuixDisplaySetupFunction(display, name, xres, yres,
                                          colorformat, target_cpu, IsRenesasDave2D, IsDave2DFontFormat,
                                          palette, palsize, aliased_font_palsize);

    if (memsize)
    {
        if (colorformat <= GX_COLOR_FORMAT_8BIT_PALETTE)
        {
            if (display -> gx_display_driver_palette_set && palette)
            {
                display -> gx_display_driver_palette_set(display, palette, palsize);
            }
        }
    }
    return memsize;
}

/**************************************************************************/
INT guix_studio_create_display(GX_DISPLAY *display, char *name,
                               int xres, int yres,  int colorformat,
                               int target_cpu, int IsRenesasDave2D, int IsDave2DFontFormat,
                               GX_COLOR *palette, int palsize, int aliased_font_palsize)
{
INT  memsize;

    memsize = RunGuixDisplaySetupFunction(display, name, xres, yres,
                                          colorformat, target_cpu, IsRenesasDave2D, IsDave2DFontFormat,
                                          palette, palsize, aliased_font_palsize);

    if (memsize)
    {
        // Dummy out the display toggle function, Studio uses it's own canvas_flush function
        // to draw the canvas to the visible window

        display->gx_display_driver_buffer_toggle = gx_studio_buffer_toggle_stub;

        if (colorformat <= GX_COLOR_FORMAT_8BIT_PALETTE)
        {
            if (display -> gx_display_driver_palette_set && palette)
            {
                display -> gx_display_driver_palette_set(display, palette, palsize);
            }
        }
    }
    return memsize;
}

void guix_studio_delete_display(GX_DISPLAY *display)
{
void (*cleanup_func)(GX_DISPLAY *);

    cleanup_func = NULL;

    /* figure out which cleanup function to run if
       this is not first time */
    switch (display -> gx_display_color_format)
    {
    case GX_COLOR_FORMAT_MONOCHROME:
    case GX_COLOR_FORMAT_MONOCHROME_INVERTED:
        cleanup_func = win32_graphics_driver_cleanup;
        break;

    case GX_COLOR_FORMAT_2BIT_GRAY:
    case GX_COLOR_FORMAT_2BIT_GRAY_INVERTED:
    case GX_COLOR_FORMAT_4BIT_GRAY_INVERTED:
    case GX_COLOR_FORMAT_4BIT_VGA:
    case GX_COLOR_FORMAT_8BIT_GRAY:
    case GX_COLOR_FORMAT_8BIT_GRAY_INVERTED:
        break;

    case GX_COLOR_FORMAT_4BIT_GRAY:
    case GX_COLOR_FORMAT_8BIT_PALETTE:
    case GX_COLOR_FORMAT_8BIT_PACKED_PIXEL:
    case GX_COLOR_FORMAT_5551BGRX:
    case GX_COLOR_FORMAT_1555XRGB:
    case GX_COLOR_FORMAT_565RGB:
    case GX_COLOR_FORMAT_4444ARGB:
    case GX_COLOR_FORMAT_4444BGRA:
    case GX_COLOR_FORMAT_565BGR:
    case GX_COLOR_FORMAT_24RGB:
    case GX_COLOR_FORMAT_24BGR:
    case GX_COLOR_FORMAT_24BGRX:
    case GX_COLOR_FORMAT_24XRGB:
    case GX_COLOR_FORMAT_32ARGB:
    case GX_COLOR_FORMAT_32BGRA:
        /* counts go in the aux data array: */
        cleanup_func = win32_graphics_driver_cleanup;
        break;
    }

    if (cleanup_func)
    {
        _gx_display_delete(display, cleanup_func);
    }
}

/**************************************************************************/
VOID guix_cleanup_target_win_canvas(VOID)
{
    if (target_win_canvas_memory != NULL)
    {
        free(target_win_canvas_memory);
        target_win_canvas_memory = NULL;
        guix_studio_delete_display(&target_win_display);
        _gx_canvas_delete(&target_win_canvas);
        _gx_window_root_delete(&target_win_root_window);
    }
}


/**************************************************************************/
BOOL guix_configure_target_win_canvas(int xres,
                           int yres,
                           int colorformat,
                           int target_cpu,
                           int IsSynergyD2D,
                           int IsSynergyFontFormat,
                           GX_COLOR *palette,
                           int palsize,
                           int aliased_font_palsize)
{
GX_RECTANGLE size;
int          memsize;

    if (target_win_canvas_memory != NULL)
    {
        _gx_widget_hide((GX_WIDGET *)&target_win_root_window);
        free(target_win_canvas_memory);
        target_win_canvas_memory = NULL;
        guix_studio_delete_display(&target_win_display);
        _gx_canvas_delete(&target_win_canvas);
        _gx_window_root_delete(&target_win_root_window);
    }

    memsize = guix_studio_create_display(&target_win_display, "StudioX Main Display", 
                                         xres, yres, colorformat, target_cpu, IsSynergyD2D, IsSynergyFontFormat, palette, palsize, aliased_font_palsize);

    if (memsize)
    {

        gx_win32_set_win_handle(target_win_display.gx_display_driver_data, GetGuixWinHandle());
        gx_win32_set_thread_id(target_win_display.gx_display_driver_data, GetCurrentThreadId());

        target_win_canvas_memory = malloc(memsize);

        //_gx_canvas_create(&target_win_canvas, "StudioX Canvas",
        //                  &target_win_display, GX_CANVAS_MANAGED | GX_CANVAS_VISIBLE,
        //                  xres, yres, target_win_canvas_memory, memsize);

        _gx_canvas_create(&target_win_canvas, "StudioX Canvas",
                          &target_win_display, GX_CANVAS_VISIBLE,
                          xres, yres, target_win_canvas_memory, memsize);

        /* Create a background root window and attach to the background canvas.  */

        _gx_utility_rectangle_define(&size, 0, 0, xres - 1, yres - 1);
        _gx_window_root_create(&target_win_root_window, "GUIX root window", &target_win_canvas,
                               GX_STYLE_BORDER_NONE, GX_ID_NONE, &size);
        return TRUE;
    }
    return FALSE;
}

/**************************************************************************/
GX_WINDOW_ROOT *get_root_window(void)
{
GX_WINDOW_ROOT *root = _gx_system_root_window_created_list;

    return &target_win_root_window;
}

/**************************************************************************/
GX_DISPLAY *get_target_view_display(void)
{
    return &target_win_display;
}

/**************************************************************************/
GX_CANVAS *get_target_win_canvas(void)
{
    return &target_win_canvas;
}

/**************************************************************************/
HWND get_target_win_handle()
{
    return target_win_handle;
}

/**************************************************************************/
/* kgm- replace the standard implementation of this function (remove the
   file gx_system_dirty_partial_add from GUIX library build for studiox.

    This replacement sends an event to studioX target window when the
    root canvas is marked dirty */

UINT  _gx_system_dirty_partial_add(GX_WIDGET *widget, GX_RECTANGLE *dirty_area)
{

UINT            index;
GX_DIRTY_AREA  *dirty_entry;
GX_WIDGET      *test;
GX_WINDOW_ROOT *root;
GX_CANVAS      *canvas;
GX_DIRTY_AREA  *empty_dirty_entry = GX_NULL;

    if (!widget)
    {
        return GX_PTR_ERROR;
    }
    if (!(widget -> gx_widget_status & GX_STATUS_VISIBLE))
    {
        return GX_PTR_ERROR;
    }

    /* if a widget is transparent, we actually need to mark it's
       first non-transparent parent as being dirty
     */
    while ((widget -> gx_widget_status & GX_STATUS_TRANSPARENT) &&
           (widget -> gx_widget_parent))
    {
        widget = widget -> gx_widget_parent;
    }

    /* walk up to find root window */
    test = widget;

    while (test -> gx_widget_parent)
    {
        test = test -> gx_widget_parent;
    }

    if (test -> gx_widget_type != GX_TYPE_ROOT_WINDOW)
    {
        return GX_PTR_ERROR;
    }

    root = (GX_WINDOW_ROOT *)test;

    /* pick up pointer to canvas */
    canvas = root -> gx_window_root_canvas;

    /***************************************
       THIS IS THE ADDED CODE FOR STUDIOX
    ***************************************/

    if (canvas == &target_win_canvas)
    {
        WPARAM wparam = MAKEWPARAM(dirty_area->gx_rectangle_left, dirty_area->gx_rectangle_top);
        LPARAM lparam = MAKELPARAM(dirty_area->gx_rectangle_right, dirty_area->gx_rectangle_bottom);
        PostMessage(target_win_handle, GUIX_DIRTY, wparam, lparam);

        // When marking widgets dirty for the target win, just always mark the top-level
        // window dirty to fix any drawing issues caused by Studio intercepting the
        // target widget drawing, or a transparent target widget not being given
        // transparent style. We want the drawing inside Studio to look right.

        if (widget != (GX_WIDGET *) root)
        {
            widget = (GX_WIDGET *) root->gx_widget_first_child;
        }
    }

    /***************************************
       END OF THE ADDED CODE
    ***************************************/

    /* Setup pointer to dirty list.  */

    dirty_entry = canvas -> gx_canvas_dirty_list;

    /* Check to see if widget already has an entry.  */
    for (index = 0; index < canvas -> gx_canvas_dirty_count; index++)
    {
        /* Is the same widget is present. */
        if (dirty_entry -> gx_dirty_area_widget == widget)
        {
            /* if rectangles overlap, combine them rather
               than adding another entry
             */
            if (_gx_utility_rectangle_overlap_detect(&dirty_entry -> gx_dirty_area_rectangle,
                                                     dirty_area, GX_NULL))
            {
                /* Combine the dirty rectangles.  */
                _gx_utility_rectangle_combine(&dirty_entry -> gx_dirty_area_rectangle, dirty_area);

                /* Return success.  */
                return(GX_SUCCESS);
            }
        }
        else if ((empty_dirty_entry == GX_NULL) && (dirty_entry -> gx_dirty_area_widget == GX_NULL))
        {
            empty_dirty_entry = dirty_entry;
        }

        /* Move to next dirty entry.  */
        dirty_entry++;
    }

    if (empty_dirty_entry)
    {

        /* Reuse the invalid dirty entry.  */
        empty_dirty_entry -> gx_dirty_area_widget = widget;
        empty_dirty_entry -> gx_dirty_area_rectangle = *dirty_area;

        return(GX_SUCCESS);
    }

    /* If we get here, we didn't find an entry for this caller. Add a new entry.  */

    /* Are there more dirty entries?  */
    if (canvas -> gx_canvas_dirty_count < GX_MAX_DIRTY_AREAS)
    {
        /* Yes, more dirty entries, add this one!  */
        dirty_entry = &canvas -> gx_canvas_dirty_list[canvas -> gx_canvas_dirty_count];
        canvas -> gx_canvas_dirty_count++;
        dirty_entry -> gx_dirty_area_widget =    widget;
        dirty_entry -> gx_dirty_area_rectangle = *dirty_area;
    }

    /* Return success.  */
    return(GX_SUCCESS);
}

/**************************************************************************/
/* replace the standard implementation of this function (remove the
file gx_scroll_wheeel_selected_set from GUIX library build for studiox.)

This replacement can be removed when timer issue in studio get fix. */
UINT _gx_scroll_wheel_selected_set(GX_SCROLL_WHEEL *wheel, INT row)
{
    if ((row < 0) || wheel->gx_scroll_wheel_total_rows <= 0)
    {
        row = 0;
    }
    else if (row > wheel->gx_scroll_wheel_total_rows - 1)
    {
        row = wheel->gx_scroll_wheel_total_rows - 1;
    }

    wheel->gx_scroll_wheel_selected_row = row;

    if (wheel->gx_widget_style & GX_STATUS_VISIBLE)
    {
        _gx_system_dirty_mark((GX_WIDGET *)wheel);
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/* replace the standard implementation of this function
  (guix focus is not need in studio edit mode) */
UINT _gx_system_focus_claim(GX_WIDGET *widget)
{
    GX_WINDOW_ROOT *root = get_root_window();
    GX_BOOL child_detect;

    if (root)
    {
        _gx_widget_child_detect((GX_WIDGET *)root, widget, &child_detect);
        if (child_detect)
        {
            /* GUIX edit mode. */
            return 0;
        }
        else
        {
            /* GUIX running mode. */
            return gx_system_focus_claim_standard(widget);
        }
    }
    return 0;
}

