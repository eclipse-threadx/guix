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
/**   Win32 Display Management (Display)                                  */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/
/*                                                                        */
/*  COMPONENT DEFINITION                                   RELEASE        */
/*                                                                        */
/*    gx_win32_driver.h                                   PORTABLE C      */
/*                                                           6.1.4        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file defines the GUIX Win32 display drivers.                   */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  12-31-2020     Kenneth Maxwell          Initial Version 6.1.3         */
/*  02-02-2021     Kenneth Maxwell          Modified comment(s),          */
/*                                            added 8bpp/24bpp rotated    */
/*                                            display driver declarations,*/
/*                                            resulting in version 6.1.4  */
/*                                                                        */
/**************************************************************************/

#ifndef GX_WIN32_DRIVER_H
#define GX_WIN32_DRIVER_H

#ifdef __cplusplus

/* Yes, C++ compiler is present.  Use standard C.  */
extern   "C" {

#endif

#include "tx_api.h"
#include "gx_api.h"
#include "gx_system.h"
#include "gx_display.h"
#include "windows.h"

#define GX_MAX_WIN32_DISPLAYS 2
#define GX_WIN32_STACK_SIZE   (16 * 1024)

/* Define Windows bitmap structure. */
typedef struct GUIX_BMP_INFO_STRUCT
{
    BITMAPINFOHEADER gx_bmp_header;
    DWORD            gx_bmp_colors[256];
} GX_BMP_INFO;

/* Define win32 display driver data structure. */
typedef struct GX_WIN32_DISPLAY_DRIVER_STRUCT
{
    ULONG       win32_driver_type;
    GX_BMP_INFO win32_driver_bmpinfo;
    HWND        win32_driver_winhandle;
    INT         win32_driver_ready;
    TX_THREAD   win32_driver_thread;
    ULONG       win32_driver_thread_stack[GX_WIN32_STACK_SIZE / sizeof(ULONG)];
    GX_WIN32_DISPLAY_DRIVER_EXTRA_MEMBERS_DECLEARE
} GX_WIN32_DISPLAY_DRIVER_DATA;

/* Define win32 driver functions. */
UINT                          win32_graphics_driver_setup_monochrome(GX_DISPLAY *display);
UINT                          win32_graphics_driver_setup_4bpp_grayscale(GX_DISPLAY *display);
UINT                          win32_graphics_driver_setup_8bit_palette(GX_DISPLAY *display);
UINT                          win32_graphics_driver_setup_8bit_palette_rotated(GX_DISPLAY* display);
UINT                          win32_graphics_driver_setup_565rgb(GX_DISPLAY *display);
UINT                          win32_graphics_driver_setup_565rgb_rotated(GX_DISPLAY *display);
UINT                          win32_graphics_driver_setup_565bgr(GX_DISPLAY *display);
UINT                          win32_graphics_driver_setup_1555xrgb(GX_DISPLAY *display);
UINT                          win32_graphics_driver_setup_4444argb(GX_DISPLAY *display);
UINT                          win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);
UINT                          win32_graphics_driver_setup_24xrgb_rotated(GX_DISPLAY *display);
UINT                          win32_graphics_driver_setup_32argb(GX_DISPLAY *display);
UINT                          win32_graphics_driver_setup_32argb_rotated(GX_DISPLAY *display);

UINT                          win32_chromeart_graphics_driver_setup_565rgb(GX_DISPLAY *display);

VOID                          win32_32bpp_bitmap_header_create(GX_DISPLAY *display);
VOID                          win32_display_driver_8bit_palette_set(GX_DISPLAY *display, GX_COLOR *palette, INT count);
VOID                          win32_8bit_palette_bitmap_header_create(GX_DISPLAY *display);
void                          gx_win32_display_buffer_toggle(GX_CANVAS *canvas, GX_RECTANGLE *dirty);
void                          gx_win32_driver_thread_entry(ULONG thread_input);
HWND                          gx_win32_window_create(GX_WIN32_DISPLAY_DRIVER_DATA *gx_driver_ptr, WNDPROC gx_win32_event_process, INT xpos, INT ypos);
LRESULT CALLBACK              gx_win32_event_process(HWND, UINT, WPARAM, LPARAM);
void                          gx_win32_message_to_guix(USHORT event_type);
void                          gx_win32_input_driver();
GX_WIN32_DISPLAY_DRIVER_DATA *gx_win32_get_free_data_instance();
GX_WIN32_DISPLAY_DRIVER_DATA *gx_win32_get_data_instance_by_win_handle(HWND winHandle);

/* Determine if a C++ compiler is being used.  If so, complete the standard
   C conditional started above. */
#ifdef __cplusplus
}
#endif

#endif /* GX_WIN32_DRIVER_H */

