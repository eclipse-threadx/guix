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
/**   Win32 Display Drivers for GUIX Studio                               */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/

#ifndef GX_WIN32_STUDIO_DRIVER_H
#define GX_WIN32_STUDIO_DRIVER_H

#ifdef __cplusplus

/* Yes, C++ compiler is present.  Use standard C.  */
extern   "C" {

#endif

#include "tx_api.h"
#include "gx_api.h"
#include "gx_system.h"
#include "gx_display.h"
#include "windows.h"
#include "gx_win32_display_driver.h"

VOID                          win32_graphics_driver_cleanup(GX_DISPLAY* display);

void                          gx_win32_studio_driver_thread_entry(ULONG thread_input);
LRESULT CALLBACK              gx_win32_studio_event_process(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
GX_WIN32_DISPLAY_DRIVER_DATA *gx_win32_get_data_instance_head(void);
GX_WIN32_DISPLAY_DRIVER_DATA *gx_win32_get_data_instance_by_thread_id(LONG thread_id);
VOID                          gx_win32_set_win_handle(VOID *driver_data, HWND handle);
VOID                          gx_win32_set_thread_id(VOID *driver_data, INT thread_id);

/* Determine if a C++ compiler is being used.  If so, complete the standard
   C conditional started above. */
#ifdef __cplusplus
}
#endif

#endif /* GX_WIN32_DRIVER_H */

