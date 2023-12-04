#include "tx_api.h"

#include "gx_api.h"
#include "gx_system.h"
#include "gx_win32_studio_display_driver.h"
#include "studiox_screen_driver.h"

#ifdef _DEBUG
#define new DEBUG_NEW

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#endif

GX_WINDOW_ROOT *get_root_window(void);
extern HWND GetGuixWinHandle(void);

#define MAX_WIN32_EVENTS 64
HANDLE gx_win32_event_semaphore;
HANDLE gx_win32_lock_mutex;

typedef struct win32_event_struct {
  GX_EVENT event;
  struct win32_event_struct *next;
} win32_event;

win32_event win32_event_memory[MAX_WIN32_EVENTS];

win32_event *gx_win32_event_queue;
win32_event *gx_win32_free_events;

VOID  gx_generic_rtos_initialize(VOID)
{
    int loop;
    win32_event *new_event;

    gx_win32_free_events = NULL;
    gx_win32_event_queue = NULL;

    gx_win32_lock_mutex = CreateMutex(NULL, FALSE, NULL);

    gx_win32_event_semaphore = CreateSemaphore( 
        NULL,           // default security attributes
        0,  // initial count
        MAX_WIN32_EVENTS,  // maximum count
        NULL);          // unnamed semaphore
    
    new_event = win32_event_memory;
    new_event += MAX_WIN32_EVENTS - 1;

    for (loop = 0; loop < MAX_WIN32_EVENTS; loop++)
    {
        new_event->next = gx_win32_free_events;
        gx_win32_free_events = new_event;
        new_event--;
    }
}

UINT  gx_generic_thread_start(VOID(*guix_thread_entry)(ULONG))
{
    return 0;
}

UINT  gx_generic_event_post(GX_EVENT *event_ptr)
{
    win32_event *new_event;
    win32_event *last_event;
    GX_WIN32_DISPLAY_DRIVER_DATA *data = gx_win32_get_data_instance_by_thread_id(GetCurrentThreadId());

    if (!data)
    {
        data = gx_win32_get_data_instance_by_win_handle(get_target_win_handle());
    }

    if (!gx_win32_free_events || !data)
    {
        return 0;
    }

    if (data->win32_driver_winhandle == GetGuixWinHandle())
    {
        return _gx_system_event_dispatch(event_ptr);
    }

    if (data->win32_root_win && !(data->win32_root_win->gx_widget_status & GX_STATUS_VISIBLE))
    {
        return 0;
    }

    GX_ENTER_CRITICAL
    new_event = gx_win32_free_events;
    gx_win32_free_events = new_event->next;

    new_event->event = *event_ptr;
    new_event->next = NULL;

    if (gx_win32_event_queue)
    {
        last_event = gx_win32_event_queue;
        while(last_event->next)
        {
            last_event = last_event->next;
        }
        last_event->next = new_event;
    }
    else
    {
        gx_win32_event_queue = new_event;
    }
    ReleaseSemaphore(gx_win32_event_semaphore, 1, NULL);
    GX_EXIT_CRITICAL
    return 0;
}

UINT  gx_generic_event_fold(GX_EVENT *event_ptr)
{
    gx_generic_event_post(event_ptr);
    return 0;
}

VOID  gx_generic_event_purge(GX_WIDGET *widget)
{
}

UINT  gx_generic_event_pop(GX_EVENT *put_event, GX_BOOL wait)
{
    win32_event *pEvent;
    DWORD result;

    if (wait)
    {
        result = WaitForSingleObject(gx_win32_event_semaphore, INFINITE);
    }
    else
    {
        result = WaitForSingleObject(gx_win32_event_semaphore, 0);
    }

    if (result == 0)
    {
        // succcess
        GX_ENTER_CRITICAL
            pEvent = gx_win32_event_queue;

        if (pEvent == NULL)
        {
            GX_EXIT_CRITICAL
                return GX_FAILURE;
        }

        // pop the event from active queue:
        gx_win32_event_queue = pEvent->next;;

        // store event in GUIX event structure
        *put_event = pEvent->event;

        // link the event into the free event queue:
        pEvent->next = gx_win32_free_events;
        gx_win32_free_events = pEvent;

        GX_EXIT_CRITICAL
        return GX_SUCCESS;
    }

    return GX_FAILURE;
}

VOID  gx_generic_timer_start(VOID)
{
}

VOID  gx_generic_timer_stop(VOID)
{
}

VOID  gx_generic_system_mutex_lock(VOID)
{
    WaitForSingleObject(gx_win32_lock_mutex, INFINITE);    
}

VOID  gx_generic_system_mutex_unlock(VOID)
{
    ReleaseMutex(gx_win32_lock_mutex);   
}

ULONG gx_generic_system_time_get(VOID)
{
    return GetTickCount() / GX_SYSTEM_TIMER_MS;
}

VOID *gx_generic_thread_identify(VOID)
{
    return (VOID *) GetCurrentThreadId();
}





