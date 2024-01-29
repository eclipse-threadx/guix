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
#ifdef WIN32
#include "tx_api.h"
#include "gx_api.h"
#include "gx_system.h"
#include "gx_display.h"
#include "windows.h"
#include "gx_win32_studio_display_driver.h"

extern BOOL                         win32_graphics_data_initialized;
extern GX_WIN32_DISPLAY_DRIVER_DATA win32_instance_data[GX_MAX_WIN32_DISPLAYS];


/**************************************************************************/
/*    Do cleanup operation for specific display driver.                   */
/**************************************************************************/
VOID win32_graphics_driver_cleanup(GX_DISPLAY* display)
{
    GX_WIN32_DISPLAY_DRIVER_DATA* instance;

    instance = (GX_WIN32_DISPLAY_DRIVER_DATA*)display->gx_display_driver_data;

    /* mark this instance as not used: */
    instance->win32_driver_type = 0;
}

/**************************************************************************/
/* Get win32 instance data pointer.                                       */
/**************************************************************************/
GX_WIN32_DISPLAY_DRIVER_DATA* gx_win32_get_data_instance_head(void)
{
    return win32_instance_data;
}

/**************************************************************************/
/* Get win32 instance data by thread id.                                  */
/**************************************************************************/
GX_WIN32_DISPLAY_DRIVER_DATA *gx_win32_get_data_instance_by_thread_id(LONG thread_id)
{
int index;

    if (!win32_graphics_data_initialized || !thread_id)
    {
        return(NULL);
    }

    for (index = 0; index < GX_MAX_WIN32_DISPLAYS; index++)
    {
        if (win32_instance_data[index].win32_driver_type == 0)
        {
            continue;
        }

        if (win32_instance_data[index].win32_window_ThreadId == thread_id ||
            win32_instance_data[index].win32_guix_ThreadId == thread_id)
        {
            return(&win32_instance_data[index]);
        }
    }
    return(0);
}

/**************************************************************************/
/* Set window handler for win32 dirver.                                   */
/**************************************************************************/
VOID gx_win32_set_win_handle(VOID* driver_data, HWND handle)
{
    GX_WIN32_DISPLAY_DRIVER_DATA* data = (GX_WIN32_DISPLAY_DRIVER_DATA*)driver_data;
    data->win32_driver_winhandle = handle;
}

/**************************************************************************/
/* Set thread id for win32 driver.                                        */
/**************************************************************************/
VOID gx_win32_set_thread_id(VOID* driver_data, INT thread_id)
{
    GX_WIN32_DISPLAY_DRIVER_DATA* data = (GX_WIN32_DISPLAY_DRIVER_DATA*)driver_data;
    data->win32_window_ThreadId = thread_id;
}

/**************************************************************************/
/*  Handle windows message.                                               */
/**************************************************************************/
LRESULT CALLBACK gx_win32_studio_event_process(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
GX_WIN32_DISPLAY_DRIVER_DATA *driver_data;

    driver_data = gx_win32_get_data_instance_by_win_handle(hwnd);

    switch (message)
    {
    case WM_DESTROY:
    {
    RECT rect;
        GetWindowRect(hwnd, &rect);

        if (driver_data)
        {
            driver_data -> win32_window_xpos = rect.left;
            driver_data -> win32_window_ypos = rect.top;
        }
    }
    }
    return gx_win32_event_process(hwnd, message, wParam, lParam);
}


/**************************************************************************/
/* Create window for guix and start guix thread in window.                */
/**************************************************************************/
void gx_win32_studio_driver_thread_entry(ULONG thread_input)
{
/* create a MS Windows window to serve as our physical display
   This HAS to be done in the context of the win32 thread,
   otherwise we don't get messages from MS Windows. Would prefer
   to do this as part of the win32_graphics_driver function,
   but that runs in the context of the GUIX thread so we can't do
   it there.
 */
GX_WIN32_DISPLAY_DRIVER_DATA *instance = (GX_WIN32_DISPLAY_DRIVER_DATA *)thread_input;

    if (instance->win32_window_xpos < 0 ||
        instance->win32_window_xpos >= (GetSystemMetrics(SM_CXVIRTUALSCREEN) - 10))
    {
        instance->win32_window_xpos = 20;
    }

    if (instance->win32_window_ypos < 0 ||
        instance->win32_window_ypos >= (GetSystemMetrics(SM_CYVIRTUALSCREEN) - 10))
    {
        instance->win32_window_ypos = 20;
    }

    /* Create Win32 window.  */
    instance->win32_driver_winhandle = gx_win32_window_create(instance, gx_win32_studio_event_process,
                                                              instance->win32_window_xpos,
                                                              instance->win32_window_ypos);


    /* Initialize thread.  */
    gx_win32_driver_thread_initialize(instance);
}
#endif /* WIN32 */

