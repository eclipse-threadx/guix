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
#ifdef WIN32
#pragma comment(lib, "winmm.lib")
#include "tx_api.h"
#include "gx_api.h"
#include "gx_system.h"
#include "gx_display.h"
#include "windows.h"
#include "gx_win32_display_driver.h"

#define GX_KEY_NUMPAD0          0x0030
#define GX_KEY_NUMPAD1          0x0031
#define GX_KEY_NUMPAD2          0x0032
#define GX_KEY_NUMPAD3          0x0033
#define GX_KEY_NUMPAD4          0x0034
#define GX_KEY_NUMPAD5          0x0035
#define GX_KEY_NUMPAD6          0x0036
#define GX_KEY_NUMPAD7          0x0037
#define GX_KEY_NUMPAD8          0x0038
#define GX_KEY_NUMPAD9          0x0039
#define GX_KEY_MULTIPLY         0x002a
#define GX_KEY_ADD              0x002b
#define GX_KEY_SUBTRACT         0x002d
#define GX_KEY_DECIMAL          0x002e
#define GX_KEY_DIVIDE           0x002f
#define GX_KEY_OEM_1            0x003b
#define GX_KEY_SHIFT_OEM_1      0x003a
#define GX_KEY_OEM_PLUS         0x003d
#define GX_KEY_SHIFT_OEM_PLUS   0x002b
#define GX_KEY_OEM_COMMA        0x002c
#define GX_KEY_SHIFT_OEM_COMMA  0x003c
#define GX_KEY_OEM_MINUS        0x002d
#define GX_KEY_SHIFT_OEM_MINUS  0x005f
#define GX_KEY_OEM_PERIOD       0x002e
#define GX_KEY_SHIFT_OME_PERIOD 0x003e
#define GX_KEY_OEM_2            0x002f
#define GX_KEY_SHIFT_OEM_2      0x003f
#define GX_KEY_OEM_3            0x0027
#define GX_KEY_SHIFT_OEM_3      0x007e
#define GX_KEY_OEM_4            0x005b
#define GX_KEY_SHIFT_OEM_4      0x007b
#define GX_KEY_OEM_5            0x005c
#define GX_KEY_SHIFT_OEM_5      0x007c
#define GX_KEY_OEM_6            0x005d
#define GX_KEY_SHIFT_OEM_6      0x007d
#define GX_KEY_OEM_7            0x0060
#define GX_KEY_SHIFT_OEM_7      0x0022
#define GX_KEY_NUM1             0x0031
#define GX_KEY_SHIFT_NUM1       0x0021
#define GX_KEY_NUM2             0x0032
#define GX_KEY_SHIFT_NUM2       0x0040
#define GX_KEY_NUM3             0x0033
#define GX_KEY_SHIFT_NUM3       0x0023
#define GX_KEY_NUM4             0x0034
#define GX_KEY_SHIFT_NUM4       0x0024
#define GX_KEY_NUM5             0x0035
#define GX_KEY_SHIFT_NUM5       0x0025
#define GX_KEY_NUM6             0x0036
#define GX_KEY_SHIFT_NUM6       0x005e
#define GX_KEY_NUM7             0x0037
#define GX_KEY_SHIFT_NUM7       0x0026
#define GX_KEY_NUM8             0x0038
#define GX_KEY_SHIFT_NUM8       0x002a
#define GX_KEY_NUM9             0x0039
#define GX_KEY_SHIFT_NUM9       0x0028
#define GX_KEY_NUM0             0x0030
#define GX_KEY_SHIFT_NUM0       0x0029
#define GX_KEY_A                0x0061
#define GX_KEY_B                0x0062
#define GX_KEY_C                0x0063
#define GX_KEY_D                0x0064
#define GX_KEY_E                0x0065
#define GX_KEY_F                0x0066
#define GX_KEY_G                0x0067
#define GX_KEY_H                0x0068
#define GX_KEY_I                0x0069
#define GX_KEY_J                0x006a
#define GX_KEY_K                0x006b
#define GX_KEY_L                0x006c
#define GX_KEY_M                0x006d
#define GX_KEY_N                0x006e
#define GX_KEY_O                0x006f
#define GX_KEY_P                0x0070
#define GX_KEY_Q                0x0071
#define GX_KEY_R                0x0072
#define GX_KEY_S                0x0073
#define GX_KEY_T                0x0074
#define GX_KEY_U                0x0075
#define GX_KEY_V                0x0076
#define GX_KEY_W                0x0077
#define GX_KEY_X                0x0078
#define GX_KEY_Y                0x0079
#define GX_KEY_Z                0x007a

#define GX_WIN32_TIMER_ID 1

typedef struct
{
    USHORT win32_key;
    USHORT guix_normal_key;
    USHORT guix_shift_key;
} KEY_MAP_ENTRY;

typedef struct
{
    USHORT win32_key;
    USHORT shift_event_type;
    USHORT ctrl_event_type;
} KEY_EVENT_ENTRY;

static KEY_MAP_ENTRY         win32_key_table[] = {

    {VK_BACK, GX_KEY_BACKSPACE, GX_KEY_BACKSPACE},
    {VK_RETURN, GX_KEY_SELECT, GX_KEY_SELECT},
    {VK_SHIFT, GX_KEY_SHIFT, GX_KEY_SHIFT},
    {VK_CONTROL, GX_KEY_CONTROL, GX_KEY_CONTROL},
    {VK_CAPITAL, GX_KEY_CAPS_LOCK, GX_KEY_CAPS_LOCK},
    {VK_PRIOR, GX_KEY_PAGE_UP, GX_KEY_PAGE_UP},
    {VK_NEXT, GX_KEY_PAGE_DOWN, GX_KEY_PAGE_DOWN},
    {VK_END, GX_KEY_END, GX_KEY_END},
    {VK_HOME, GX_KEY_HOME, GX_KEY_HOME},
    {VK_LEFT, GX_KEY_LEFT_ARROW, GX_KEY_LEFT_ARROW},
    {VK_UP, GX_KEY_UP_ARROW, GX_KEY_UP_ARROW},
    {VK_RIGHT, GX_KEY_RIGHT_ARROW, GX_KEY_RIGHT_ARROW},
    {VK_DOWN, GX_KEY_DOWN_ARROW, GX_KEY_DOWN_ARROW},
    {VK_DELETE, GX_KEY_DELETE, GX_KEY_DELETE},
    {VK_NUMLOCK, GX_KEY_NUMLOCK, GX_KEY_NUMLOCK},
    {VK_SEPARATOR, GX_KEY_SEPARATOR, GX_KEY_SEPARATOR},
    {VK_TAB, GX_KEY_NEXT, GX_KEY_PREVIOUS},
    {VK_SPACE, GX_KEY_SPACE, GX_KEY_SPACE},
    {VK_NUMPAD0, GX_KEY_NUMPAD0, GX_KEY_NUMPAD0},
    {VK_NUMPAD1, GX_KEY_NUMPAD1, GX_KEY_NUMPAD1},
    {VK_NUMPAD2, GX_KEY_NUMPAD2, GX_KEY_NUMPAD2},
    {VK_NUMPAD3, GX_KEY_NUMPAD3, GX_KEY_NUMPAD3},
    {VK_NUMPAD4, GX_KEY_NUMPAD4, GX_KEY_NUMPAD4},
    {VK_NUMPAD5, GX_KEY_NUMPAD5, GX_KEY_NUMPAD5},
    {VK_NUMPAD6, GX_KEY_NUMPAD6, GX_KEY_NUMPAD6},
    {VK_NUMPAD7, GX_KEY_NUMPAD7, GX_KEY_NUMPAD7},
    {VK_NUMPAD8, GX_KEY_NUMPAD8, GX_KEY_NUMPAD8},
    {VK_NUMPAD9, GX_KEY_NUMPAD9, GX_KEY_NUMPAD9},
    {VK_MULTIPLY, GX_KEY_MULTIPLY, GX_KEY_MULTIPLY},
    {VK_ADD, GX_KEY_ADD, GX_KEY_ADD},
    {VK_SUBTRACT, GX_KEY_SUBTRACT, GX_KEY_SUBTRACT},
    {VK_DECIMAL, GX_KEY_DECIMAL, GX_KEY_DECIMAL},
    {VK_DIVIDE, GX_KEY_DIVIDE, GX_KEY_DIVIDE},
    {VK_OEM_PLUS, GX_KEY_OEM_PLUS, GX_KEY_SHIFT_OEM_PLUS},
    {VK_OEM_COMMA, GX_KEY_OEM_COMMA, GX_KEY_SHIFT_OEM_COMMA},
    {VK_OEM_MINUS, GX_KEY_OEM_MINUS, GX_KEY_SHIFT_OEM_MINUS},
    {VK_OEM_PERIOD, GX_KEY_OEM_PERIOD, GX_KEY_SHIFT_OME_PERIOD},
    {VK_OEM_1, GX_KEY_OEM_1, GX_KEY_SHIFT_OEM_1},
    {VK_OEM_2, GX_KEY_OEM_2, GX_KEY_SHIFT_OEM_2},
    {VK_OEM_3, GX_KEY_OEM_3, GX_KEY_SHIFT_OEM_3},
    {VK_OEM_4, GX_KEY_OEM_4, GX_KEY_SHIFT_OEM_4},
    {VK_OEM_5, GX_KEY_OEM_5, GX_KEY_SHIFT_OEM_5},
    {VK_OEM_6, GX_KEY_OEM_6, GX_KEY_SHIFT_OEM_6},
    {VK_OEM_7, GX_KEY_OEM_7, GX_KEY_SHIFT_OEM_7},
    {0x31, GX_KEY_NUM1, GX_KEY_SHIFT_NUM1},
    {0x32, GX_KEY_NUM2, GX_KEY_SHIFT_NUM2},
    {0x33, GX_KEY_NUM3, GX_KEY_SHIFT_NUM3},
    {0x34, GX_KEY_NUM4, GX_KEY_SHIFT_NUM4},
    {0x35, GX_KEY_NUM5, GX_KEY_SHIFT_NUM5},
    {0x36, GX_KEY_NUM6, GX_KEY_SHIFT_NUM6},
    {0x37, GX_KEY_NUM7, GX_KEY_SHIFT_NUM7},
    {0x38, GX_KEY_NUM8, GX_KEY_SHIFT_NUM8},
    {0x39, GX_KEY_NUM9, GX_KEY_SHIFT_NUM9},
    {0x30, GX_KEY_NUM0, GX_KEY_SHIFT_NUM0},
    {0x41, GX_KEY_A, GX_KEY_A - 0x20},
    {0x42, GX_KEY_B, GX_KEY_B - 0x20},
    {0x43, GX_KEY_C, GX_KEY_C - 0x20},
    {0x44, GX_KEY_D, GX_KEY_D - 0x20},
    {0x45, GX_KEY_E, GX_KEY_E - 0x20},
    {0x46, GX_KEY_F, GX_KEY_F - 0x20},
    {0x47, GX_KEY_G, GX_KEY_G - 0x20},
    {0x48, GX_KEY_H, GX_KEY_H - 0x20},
    {0x49, GX_KEY_I, GX_KEY_I - 0x20},
    {0x4a, GX_KEY_J, GX_KEY_J - 0x20},
    {0x4b, GX_KEY_K, GX_KEY_K - 0x20},
    {0x4c, GX_KEY_L, GX_KEY_L - 0x20},
    {0x4d, GX_KEY_M, GX_KEY_M - 0x20},
    {0x4e, GX_KEY_N, GX_KEY_N - 0x20},
    {0x4f, GX_KEY_O, GX_KEY_O - 0x20},
    {0x50, GX_KEY_P, GX_KEY_P - 0x20},
    {0x51, GX_KEY_Q, GX_KEY_Q - 0x20},
    {0x52, GX_KEY_R, GX_KEY_R - 0x20},
    {0x53, GX_KEY_S, GX_KEY_S - 0x20},
    {0x54, GX_KEY_T, GX_KEY_T - 0x20},
    {0x55, GX_KEY_U, GX_KEY_U - 0x20},
    {0x56, GX_KEY_V, GX_KEY_V - 0x20},
    {0x57, GX_KEY_W, GX_KEY_W - 0x20},
    {0x58, GX_KEY_X, GX_KEY_X - 0x20},
    {0x59, GX_KEY_Y, GX_KEY_Y - 0x20},
    {0x5a, GX_KEY_Z, GX_KEY_Z - 0x20},
    {0, 0, 0}
};

static KEY_EVENT_ENTRY       win32_key_events[] = {
    {VK_END,   GX_EVENT_MARK_END, 0},
    {VK_HOME,  GX_EVENT_MARK_HOME, 0},
    {VK_LEFT,  GX_EVENT_MARK_PREVIOUS, 0},
    {VK_UP,    GX_EVENT_MARK_UP, 0},
    {VK_RIGHT, GX_EVENT_MARK_NEXT, 0},
    {VK_DOWN,  GX_EVENT_MARK_DOWN,  0},
    {'c',      0,  GX_EVENT_COPY},
    {'C',      0,  GX_EVENT_COPY},
    {'x',      0,  GX_EVENT_CUT},
    {'X',      0,  GX_EVENT_CUT},
    {'v',      0,  GX_EVENT_PASTE},
    {'V',      0,  GX_EVENT_PASTE},
    {0, 0, 0}
};

static UINT                  caps_flag = 0;
static UINT                  shift_flag = 0;
static UINT                  numlock_flag = 0;
static UINT                  ctrl_flag = 0;
static char                  icon_path[MAX_PATH];

BOOL                         win32_graphics_data_initialized = FALSE;
GX_WIN32_DISPLAY_DRIVER_DATA win32_instance_data[GX_MAX_WIN32_DISPLAYS];
UINT                         win32_timer_id = 0;
extern int                   gx_main(int, char **);

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    gx_win32_timer_expiration                          PORTABLE C       */
/*                                                           6.1.10       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This is the callback function for the win32 multi-media timer that  */
/*    is called once upon the expiration of a timer event.                */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    wTimerID                              Identifier of the timer event */
/*    msg                                   Reserved                      */
/*    dwUser                                User instance data            */
/*    dw1                                   Reserved                      */
/*    dw2                                   Reserved                      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  01-31-2022     Ting Zhu                 Initial Version 6.1.10        */
/*                                                                        */
/**************************************************************************/
static void CALLBACK gx_win32_timer_expiration(UINT wTimerID, UINT msg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
    GX_WIN32_DISPLAY_DRIVER_DATA *instance = (GX_WIN32_DISPLAY_DRIVER_DATA *)dwUser;

    SendMessage(instance->win32_driver_winhandle, WM_TIMER, 0, 0);
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    gx_win32_start_multimedia_timer                    PORTABLE C       */
/*                                                           6.1.10       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function starts a periodic multi-media timer event.            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    instance                              Pointer to win32 display      */
/*                                            driver data                 */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    timeGetDevCaps                                                      */
/*    timeBeginPeriod                                                     */
/*    timeSetEvent                                                        */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    gx_win32_driver_thread_initialize                                   */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  01-31-2022     Ting Zhu                 Initial Version 6.1.10        */
/*                                                                        */
/**************************************************************************/
static void gx_win32_start_multimedia_timer(GX_WIN32_DISPLAY_DRIVER_DATA *instance)
{
    TIMECAPS tc;
    UINT     wTimerRes;

    /* Queries the timer device to determine its resolution.  */
    if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR)
    {
        /* Error; application can't continue. */
    }

    wTimerRes = min(max(tc.wPeriodMin, GX_SYSTEM_TIMER_MS), tc.wPeriodMax);

    /* Start a specified timer event. The timer runs in its own thread.
       It calls the specified callback function when the event is activated.  */
    win32_timer_id = timeSetEvent(GX_SYSTEM_TIMER_MS, wTimerRes, gx_win32_timer_expiration, (DWORD)instance, TIME_PERIODIC);
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    gx_win32_stop_multimedia_timer                     PORTABLE C       */
/*                                                           6.1.10       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function stops a multi-media timer event.                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    gx_win32_input_driver                                               */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  01-31-2022     Ting Zhu                 Initial Version 6.1.10        */
/*                                                                        */
/**************************************************************************/
static void gx_win32_stop_multimedia_timer()
{
    if (win32_timer_id)
    {
        /* Cancel a specified timer event.  */
        timeKillEvent(win32_timer_id);

        win32_timer_id = 0;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    gx_win32_map_key_to_guix_event                     PORTABLE C       */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function maps certain input key combinations to guix event     */
/*    type.                                                               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    wParam                                Win32 key type                */
/*    shift                                 Shift key status              */
/*    ctrl                                  Ctrl key status               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    ULONG                                 GUIX event type               */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    gx_win32_event_process                                              */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  12-31-2020     Kenneth Maxwell          Initial Version 6.1.3         */
/*                                                                        */
/**************************************************************************/
static ULONG gx_win32_map_key_to_guix_event(USHORT wParam, int shift, int ctrl)
{
KEY_EVENT_ENTRY *pEntry;

    if (!shift && !ctrl)
    {
        return 0;
    }

    pEntry = win32_key_events;

    while (pEntry -> win32_key)
    {
        if (wParam == pEntry -> win32_key)
        {
            if (shift && pEntry -> shift_event_type)
            {
                return pEntry -> shift_event_type;
            }
            if (ctrl && pEntry -> ctrl_event_type)
            {
                return pEntry -> ctrl_event_type;
            }
        }
        pEntry++;
    }
    return 0;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    gx_win32_map_key_to_guix_key                       PORTABLE C       */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function map windows key value to guix system key value.       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    wParam                                Win32 key value               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    USHORT                                GUIX key value                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    gx_win32_event_process                                              */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  12-31-2020     Kenneth Maxwell          Initial Version 6.1.3         */
/*                                                                        */
/**************************************************************************/
static USHORT gx_win32_map_key_to_guix_key(USHORT wParam)
{
KEY_MAP_ENTRY *pEntry = win32_key_table;

    while (pEntry -> win32_key)
    {
        if (wParam == pEntry -> win32_key)
        {
            if (wParam >= 0x41 && wParam <= 0x5a)
            {
                if ((caps_flag && shift_flag) || ((!caps_flag) && (!shift_flag)))
                {
                    return pEntry -> guix_normal_key;
                }
                else
                {
                    return pEntry -> guix_shift_key;
                }
            }
            else if (shift_flag)
            {
                return pEntry -> guix_shift_key;
            }
            else
            {
                return pEntry -> guix_normal_key;
            }
        }
        pEntry++;
    }
    return wParam;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    gx_win32_get_icon_path                              PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function gets guix logo file path.                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    RegOpenKeyExA                                                       */
/*    RegQueryValueEx                                                     */
/*    RegCloseKey                                                         */
/*    strcat_s                                                            */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    gx_win32_window_create                                              */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  12-31-2020     Kenneth Maxwell          Initial Version 6.1.3         */
/*                                                                        */
/**************************************************************************/
static void gx_win32_get_icon_path()
{
HKEY  key;
DWORD type, size;
LONG  status;

    icon_path[0] = 0;

    status = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Azure_RTOS\\GUIX\\InstallDir", 0, READ_CONTROL | KEY_QUERY_VALUE, &key);

    if (key)
    {
        size = MAX_PATH;
        RegQueryValueEx(key, NULL, 0, &type, (BYTE *)icon_path, &size);
        RegCloseKey(key);

        if (icon_path[0])
        {
            strcat_s(icon_path, MAX_PATH, "\\");
        }
    }
    strcat_s(icon_path, MAX_PATH, "graphics\\system_png\\guix_1616icon.ico");
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    gx_win32_event_to_guix                             PORTABLE C       */
/*                                                           6.1.10       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function sends event to GUIX system.                           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    type                                  GUIX event type               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _tx_thread_context_save                                             */
/*    _tx_thread_context_restore                                          */
/*    gx_system_event_send                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    gx_win32_message_to_guix                                            */
/*    gx_win32_event_process                                              */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  01-31-2022     Ting Zhu                 Initial Version 6.1.10        */
/*                                                                        */
/**************************************************************************/
void gx_win32_event_to_guix(GX_EVENT *event_ptr)
{
#ifdef GX_THREADX_BINDING
    _tx_thread_context_save();
#endif

    gx_system_event_send(event_ptr);

#ifdef GX_THREADX_BINDING
    _tx_thread_context_restore();
#endif
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    gx_win32_message_to_guix                           PORTABLE C       */
/*                                                           6.1.10       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function sends message to guix system.                         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    type                                  GUIX event type               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    gx_system_event_send                  Send message to guix system   */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    gx_win32_driver_thread_entry                                        */
/*    gx_win32_input_driver                                               */
/*    gx_win32_event_process                                              */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  12-31-2020     Kenneth Maxwell          Initial Version 6.1.3         */
/*  01-31-2022     Ting Zhu                 Modified comment(s), modified */
/*                                            GUIX event send logic,      */
/*                                            resulting in version 6.1.10 */
/*                                                                        */
/**************************************************************************/
void gx_win32_message_to_guix(USHORT event_type)
{
GX_EVENT myevent;

    memset(&myevent, 0, sizeof(GX_EVENT));

    myevent.gx_event_target = GX_NULL;
    myevent.gx_event_sender = 0;
    myevent.gx_event_type = event_type;

    gx_win32_event_to_guix(&myevent);
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    gx_win32_get_free_data_instance                     PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function get win32 instance data pointer.                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    GX_WIN32_DISPLAY_DRIVER_DATA *        Return instance data pointer  */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    memset                                                              */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Win32 display driver set up functions.                              */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  12-31-2020     Kenneth Maxwell          Initial Version 6.1.3         */
/*                                                                        */
/**************************************************************************/
GX_WIN32_DISPLAY_DRIVER_DATA *gx_win32_get_free_data_instance(void)
{
int                           index;
GX_WIN32_DISPLAY_DRIVER_DATA *data = win32_instance_data;

    if (!win32_graphics_data_initialized)
    {
        memset(win32_instance_data, 0, sizeof(GX_WIN32_DISPLAY_DRIVER_DATA) * GX_MAX_WIN32_DISPLAYS);
        win32_graphics_data_initialized = GX_TRUE;
    }

    /* Search for an unused data instance. */
    for (index = 0; index < GX_MAX_WIN32_DISPLAYS; index++)
    {
        if (data -> win32_driver_type == 0)
        {
            /* Cleanup the driver instance. */
            memset(data, 0, sizeof(GX_WIN32_DISPLAY_DRIVER_DATA));
            return data;
        }
        data++;
    }
    return GX_NULL;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    gx_win32_get_data_instance_by_win_handle            PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function gets win32 instance data pointer by win32 handle.     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    winHandle                               Win32 handle                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    GX_WIN32_DISPLAY_DRIVER_DATA *          Return instance data pointer*/
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    gx_win32_event_process                                              */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  12-31-2020     Kenneth Maxwell          Initial Version 6.1.3         */
/*                                                                        */
/**************************************************************************/
GX_WIN32_DISPLAY_DRIVER_DATA *gx_win32_get_data_instance_by_win_handle(HWND winHandle)
{
int index;

    if (!win32_graphics_data_initialized)
    {
        return(NULL);
    }

    for (index = 0; index < GX_MAX_WIN32_DISPLAYS; index++)
    {
        if (win32_instance_data[index].win32_driver_type == 0)
        {
            continue;
        }

        if (win32_instance_data[index].win32_driver_winhandle == winHandle)
        {
            return(&win32_instance_data[index]);
        }
    }

    return(0);
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    gx_win32_rotate_canvas_to_bmp_32bpp                PORTABLE C       */
/*                                                           6.1.4        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function prepare canvas memory at which do draw operation.     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    winHandle                               Win32 handle                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    win32_canvas_memory_prepare                                         */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  02-02-2021     Kenneth Maxwell          Initial Version 6.1.4         */
/*                                                                        */
/**************************************************************************/
static void gx_win32_rotate_canvas_to_bmp_32bpp(GX_CANVAS *canvas)
{
/* First pass, just copy the entire canvas, ignoring the dirty
   rectangle. Come back and apply dirty rectangle to improve
   efficiency.  */

ULONG *pReadStart;
ULONG *pWriteStart;
ULONG *pRead;
ULONG *pWrite;
INT    copy_width;
INT    copy_height;
INT    row;
INT    column;
INT    write_sign;

    /* Copy left-to-right from source canvas
       and top-to-bottom in destination bitmap (90 degree rotation)
       and bottom-to-top in destination bitmap (270 degree rotation).  */

    pReadStart = (ULONG *)canvas -> gx_canvas_memory;
    pWriteStart = (ULONG *)canvas -> gx_canvas_padded_memory;
    copy_width = canvas -> gx_canvas_y_resolution;
    copy_height = canvas -> gx_canvas_x_resolution;

    if (canvas -> gx_canvas_display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
    {
        pWriteStart += copy_height - 1;
        write_sign = -1;
    }
    else
    {
        /* Here for 270 degree rotation.  */
        pWriteStart += (copy_width - 1) * copy_height;
        write_sign = 1;
    }

    for (row = 0; row < copy_height; row++)
    {
        pRead = pReadStart;
        pWrite = pWriteStart;

        for (column = 0; column < copy_width; column++)
        {
            *pWrite = *pRead++;
            pWrite -= copy_height * write_sign;
        }
        pReadStart += copy_width;
        pWriteStart += write_sign;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    gx_win32_rotate_canvas_to_bmp_16bpp                PORTABLE C       */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function prepare canvas memory at which do draw operation.     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    winHandle                               Win32 handle                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    win32_canvas_memory_prepare                                         */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  12-31-2020     Kenneth Maxwell          Initial version 6.1.3         */
/*                                                                        */
/**************************************************************************/
static void gx_win32_rotate_canvas_to_bmp_16bpp(GX_CANVAS *canvas)
{
/* first pass, just copy the entire canvas, ignoring the dirty
   rectangle. Come back and apply dirty rectangle to improve
   efficiency.
 */

USHORT *pReadStart;
USHORT *pWriteStart;
USHORT *pRead;
USHORT *pWrite;
INT     copy_width;
INT     copy_height;
INT     row;
INT     column;
INT     write_sign;
INT     write_stride;

    /* copy left-to-right from source canvas
       and top-to-bottom in destination bitmap (90 degree rotation)
       and bottom-to-top in destination bitmap (270 degree rotation)
     */

    pReadStart = (USHORT *)canvas -> gx_canvas_memory;
    pWriteStart = (USHORT *)canvas -> gx_canvas_padded_memory;
    copy_width = canvas -> gx_canvas_y_resolution;
    copy_height = canvas -> gx_canvas_x_resolution;

    write_stride = (copy_height + 3) & 0xfffc;

    if (canvas -> gx_canvas_display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
    {
        pWriteStart += copy_height - 1;
        write_sign = -1;
    }
    else
    {
        /* Here for 270 degree rotation. */
        pWriteStart += (copy_width - 1) * write_stride;
        write_sign = 1;
    }

    for (row = 0; row < copy_height; row++)
    {
        pRead = pReadStart;
        pWrite = pWriteStart;

        for (column = 0; column < copy_width; column++)
        {
            *pWrite = *pRead++;
            pWrite -= write_stride * write_sign;
        }
        pReadStart += copy_width;
        pWriteStart += write_sign;
    }
}


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    gx_win32_rotate_canvas_to_bmp_8bpp                 PORTABLE C       */
/*                                                           6.1.4        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function prepare canvas memory at which do draw operation.     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    winHandle                               Win32 handle                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    win32_canvas_memory_prepare                                         */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  02-02-2021     Kenneth Maxwell          Initial Version 6.1.4         */
/*                                                                        */
/**************************************************************************/
static void gx_win32_rotate_canvas_to_bmp_8bpp(GX_CANVAS *canvas)
{
/* first pass, just copy the entire canvas, ignoring the dirty
   rectangle. Come back and apply dirty rectangle to improve
   efficiency.
 */

GX_UBYTE *pReadStart;
GX_UBYTE *pWriteStart;
GX_UBYTE *pRead;
GX_UBYTE *pWrite;
INT       copy_width;
INT       copy_height;
INT       row;
INT       column;
INT       write_sign;
INT       write_stride;

    /* copy left-to-right from source canvas
       and top-to-bottom in destination bitmap (90 degree rotation)
       and bottom-to-top in destination bitmap (270 degree rotation)
     */

    pReadStart = (GX_UBYTE *)canvas -> gx_canvas_memory;
    pWriteStart = (GX_UBYTE *)canvas -> gx_canvas_padded_memory;
    copy_width = canvas -> gx_canvas_y_resolution;
    copy_height = canvas -> gx_canvas_x_resolution;

    write_stride = (copy_height + 1) & 0xfffc;

    if (canvas -> gx_canvas_display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
    {
        pWriteStart += copy_height - 1;
        write_sign = -1;
    }
    else
    {
        /* Here for 270 degree rotation. */
        pWriteStart += (copy_width - 1) * write_stride;
        write_sign = 1;
    }

    for (row = 0; row < copy_height; row++)
    {
        pRead = pReadStart;
        pWrite = pWriteStart;

        for (column = 0; column < copy_width; column++)
        {
            *pWrite = *pRead++;
            pWrite -= write_stride * write_sign;
        }
        pReadStart += copy_width;
        pWriteStart += write_sign;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _win32_canvas_memory_prepare                       PORTABLE C       */
/*                                                           6.1.5        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function prepare canvas memory at which do draw operation.     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    winHandle                               Win32 handle                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    gx_win32_display_buffer_toggle                                      */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  12-31-2020     Kenneth Maxwell          Initial Version 6.1.3         */
/*  02-02-2021     Kenneth Maxwell          Modified comment(s),          */
/*                                            added 8bpp and 32bpp canvas */
/*                                            rotate logic,               */
/*                                            resulting in version 6.1.4  */
/*  03-02-2021     Ting Zhu                 Modified comment(s), added    */
/*                                            flip rotation support,      */
/*                                            resulting in version 6.1.5  */
/*                                                                        */
/**************************************************************************/
VOID *_win32_canvas_memory_prepare(GX_CANVAS *canvas, GX_RECTANGLE *dirty)
{
VOID       *memptr = (VOID *)canvas -> gx_canvas_memory;
INT         source_stride_bytes;
INT         dest_stride_bytes;
INT         copy_bytes;
UCHAR      *read_start;
UCHAR      *write_start;
GX_DISPLAY *display;
GX_VALUE    dirty_width;
INT         row;

    display = canvas -> gx_canvas_display;
    if (!display)
    {
        return memptr;
    }

    if (canvas -> gx_canvas_padded_memory)
    {
        if ((display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_NONE) ||
            (display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_FLIP))
        {
            /* we had to create padded canvas memory, so copy dirty portion from GUIX canvas memory
               to properly aligned buffer created just for Win32 compatibility */

            /* calculate where to copy from */
            read_start = (UCHAR *)canvas -> gx_canvas_memory;
            source_stride_bytes = canvas -> gx_canvas_display -> gx_display_driver_row_pitch_get(canvas -> gx_canvas_x_resolution);
            read_start += dirty -> gx_rectangle_top * source_stride_bytes;

            /* calculate where to copy to */
            write_start = (UCHAR *)canvas -> gx_canvas_padded_memory;
            dest_stride_bytes = (source_stride_bytes + 3) & 0xfffc;
            write_start += dirty -> gx_rectangle_top * dest_stride_bytes;

            dirty_width = canvas -> gx_canvas_x_resolution;
            copy_bytes = canvas -> gx_canvas_display -> gx_display_driver_row_pitch_get(dirty_width);

            /* copy the dirty rows from source to dest */

            for (row = dirty -> gx_rectangle_top; row <= dirty -> gx_rectangle_bottom; row++)
            {
                memcpy(write_start, read_start, copy_bytes); /* Use case of memcpy is verified. */
                read_start += source_stride_bytes;
                write_start += dest_stride_bytes;
            }
            memptr = (VOID *)canvas -> gx_canvas_padded_memory;
        }
        else
        {
            switch (display -> gx_display_color_format)
            {
            case GX_COLOR_FORMAT_24XRGB:
            case GX_COLOR_FORMAT_32ARGB:
                gx_win32_rotate_canvas_to_bmp_32bpp(canvas);
                memptr = canvas -> gx_canvas_padded_memory;
                break;

            case GX_COLOR_FORMAT_565RGB:
                gx_win32_rotate_canvas_to_bmp_16bpp(canvas);
                memptr = canvas -> gx_canvas_padded_memory;
                break;

            case GX_COLOR_FORMAT_8BIT_PALETTE:
                gx_win32_rotate_canvas_to_bmp_8bpp(canvas);
                memptr = canvas -> gx_canvas_padded_memory;
                break;

            default:
                return canvas -> gx_canvas_memory;
            }
        }
    }
    return memptr;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    gx_win32_display_buffer_toggle                     PORTABLE C       */
/*                                                           6.1.4        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function toggles frame buffer for guix canvas.                 */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    canvas                                Canvas control block pointer  */
/*    dirty                                 dirty area need to toggle     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    gx_utility_rectangle_define           Define a rectangle            */
/*    gx_utility_rectangle_overlap_detect   Detect overlap area of two rec*/
/*    gx_utility_rectangle_shift            Shift a rectangle             */
/*    StretchDIBits                                                       */
/*    SetMapMode                                                          */
/*    ReleaseDC                                                           */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  12-31-2020     Kenneth Maxwell          Initial Version 6.1.3         */
/*  02-02-2021     Kenneth Maxwell          Modified comment(s),          */
/*                                            fixed logic,                */
/*                                            resulting in version 6.1.4  */
/*                                                                        */
/**************************************************************************/
VOID gx_win32_display_buffer_toggle(GX_CANVAS *canvas, GX_RECTANGLE *dirty)
{
INT                           Top;
INT                           Left;
INT                           Height;
INT                           Width;
GX_RECTANGLE                  Limit;
GX_RECTANGLE                  Copy;
HDC                           win_device;
GX_WIN32_DISPLAY_DRIVER_DATA *driver_instance;
VOID                         *memptr;

    /* driver_ready is flag indicating the current state of the driver.
       driver_ready == 0:  Driver is not ready.
       driver_ready == 1:  Driver becomes ready.  This is the first time GUIX refreshes the display.
       driver_ready == 2: Normal operational mode */
    driver_instance = (GX_WIN32_DISPLAY_DRIVER_DATA *)(canvas -> gx_canvas_display -> gx_display_driver_data);

    if (driver_instance -> win32_driver_ready == 0)
    {
        /* If the windows driver is not ready, do not update the bitmap. */
        return;
    }

    /* Driver just becomes ready. So we need to refresh the whole display. */
    if (driver_instance -> win32_driver_ready == 1)
    {
        gx_utility_rectangle_define(&canvas -> gx_canvas_dirty_area, 0, 0,
                                    canvas -> gx_canvas_x_resolution - 1,
                                    canvas -> gx_canvas_y_resolution - 1);
        driver_instance -> win32_driver_ready = 2;
    }

    gx_utility_rectangle_define(&Limit, 0, 0,
                                canvas -> gx_canvas_x_resolution - 1,
                                canvas -> gx_canvas_y_resolution - 1);

    if (gx_utility_rectangle_overlap_detect(&Limit, &canvas -> gx_canvas_dirty_area, &Copy))
    {
        memptr = _win32_canvas_memory_prepare(canvas, &Copy);

        INT xsrc = Copy.gx_rectangle_left;
        INT ysrc = Copy.gx_rectangle_top;

        gx_utility_rectangle_shift(&Copy, canvas -> gx_canvas_display_offset_x, canvas -> gx_canvas_display_offset_y);

        win_device = GetDC(driver_instance -> win32_driver_winhandle);
        SetMapMode(win_device, MM_TEXT);

        driver_instance -> win32_driver_bmpinfo.gx_bmp_header.biWidth = canvas -> gx_canvas_x_resolution;
        driver_instance -> win32_driver_bmpinfo.gx_bmp_header.biHeight = canvas -> gx_canvas_y_resolution;

        Top = Copy.gx_rectangle_top;
        Left = Copy.gx_rectangle_left;
        Width = Copy.gx_rectangle_right - Copy.gx_rectangle_left + 1;
        Height = Copy.gx_rectangle_bottom - Copy.gx_rectangle_top + 1;

        StretchDIBits(win_device, Left, Top, Width, Height,
                      xsrc,
                      ysrc + Height + 1,
                      Width, -Height, memptr,
                      (BITMAPINFO *)&(driver_instance -> win32_driver_bmpinfo),
                      DIB_RGB_COLORS,
                      SRCCOPY);

        ReleaseDC(driver_instance -> win32_driver_winhandle, win_device);
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    gx_win32_window_create                             PORTABLE C       */
/*                                                           6.1.10       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function create a win32 window                                 */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    gx_driver_ptr                         Pointer to driver data        */
/*    event_process                         Win32 even process callback   */
/*    xpos                                  Initial position in x-axis    */
/*    ypos                                  Initial position in y-axis    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    HWND                                  Win32 handle                  */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    GetSystemMetrics                                                    */
/*    CreateWindow                                                        */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    gx_win32_driver_thread_entry                                        */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  12-31-2020     Kenneth Maxwell          Initial Version 6.1.3         */
/*  01-31-2022     Ting Zhu                 Modified comment(s),          */
/*                                            corrected window size,      */
/*                                            resulting in version 6.1.10 */
/*                                                                        */
/**************************************************************************/
HWND gx_win32_window_create(GX_WIN32_DISPLAY_DRIVER_DATA *gx_driver_ptr, WNDPROC event_process, INT xpos, INT ypos)
{
HINSTANCE Instance = GetModuleHandle(NULL);
RECT      TargetSize;
DWORD     WinStyle;
HWND      win32_hwnd = 0;
DWORD     err;
WNDCLASS  wndclass;

    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = event_process;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = Instance;

    gx_win32_get_icon_path();
    wndclass.hIcon = (HICON)LoadImage(NULL, icon_path, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
    err = GetLastError();
    wndclass.hCursor = NULL;
    wndclass.hbrBackground = (HBRUSH)(GetStockObject(WHITE_BRUSH));
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = TEXT("GUIX");

    RegisterClass(&wndclass);

    TargetSize.left = 0;
    TargetSize.top = 0;
    TargetSize.right = gx_driver_ptr -> win32_driver_bmpinfo.gx_bmp_header.biWidth - 1;
    TargetSize.bottom = gx_driver_ptr -> win32_driver_bmpinfo.gx_bmp_header.biHeight - 1;

    WinStyle = WS_CAPTION | WS_SIZEBOX | WS_VISIBLE | WS_MINIMIZEBOX |
        WS_MAXIMIZEBOX | WS_SYSMENU;


    /* This function adjusts the size of the Windows window so that the client
       size is the size of the display, taking into account borders
       and titles and such. We want our window client are to be exactly
       the simulatedd display size. */
    AdjustWindowRect(&TargetSize, WinStyle, FALSE);
    win32_hwnd = CreateWindow(TEXT("GUIX"), TEXT("GUIX"),
                              WinStyle, xpos, ypos,
                              TargetSize.right - TargetSize.left + 1,
                              TargetSize.bottom - TargetSize.top + 1, NULL, NULL,
                              Instance, NULL);

    SetCursor(LoadCursor(NULL, IDC_ARROW));

    return win32_hwnd;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    WndProc                                            PORTABLE C       */
/*                                                           6.1.11       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function handlea windows message.                              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    hwnd                                  Win32 handle                  */
/*    message                               Message type                  */
/*    wParam                                Message data                  */
/*    lParam                                Message data                  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    LRESULT                                                             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    gx_system_event_send                  Send message to guix system   */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    gx_win32_window_create                                              */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  12-31-2020     Kenneth Maxwell          Initial Version 6.1.3         */
/*  01-31-2022     Ting Zhu                 Modified comment(s), modified */
/*                                            GUIX event send logic,      */
/*                                            resulting in version 6.1.10 */
/*  04-25-2022     Ting Zhu                 Modified comment(s), improved */
/*                                            timer event send logic,     */
/*                                            resulting in version 6.1.11 */
/*                                                                        */
/**************************************************************************/
LRESULT CALLBACK gx_win32_event_process(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
GX_EVENT                      NewEvent;
PAINTSTRUCT                   ps;
INT                           WinX;
INT                           WinY;
GX_WIN32_DISPLAY_DRIVER_DATA *driver_data = 0;
SHORT                         s_ch[4];
SHORT                         wide_char;
ULONG                         event_type;
GX_BOOL                       check_key_event;

    driver_data = gx_win32_get_data_instance_by_win_handle(hwnd);
    NewEvent.gx_event_sender = 0;
    NewEvent.gx_event_target = GX_NULL;

    switch (message)
    {
    case WM_TIMER:
        NewEvent.gx_event_type = GX_EVENT_TIMER;
        NewEvent.gx_event_payload.gx_event_ulongdata = 1;

#ifdef GX_THREADX_BINDING
        _tx_thread_context_save();
#endif

        gx_system_event_fold(&NewEvent);

#ifdef GX_THREADX_BINDING
        _tx_thread_context_restore();
#endif
        return 0;

    case WM_PAINT:
        BeginPaint(hwnd, &ps);
        gx_win32_message_to_guix(GX_EVENT_REDRAW);
        /* now queue a message to wake up GUIX */
        EndPaint(hwnd, &ps);

        return 0;


    case WM_LBUTTONDOWN:
        SetCapture(hwnd);
        WinX = LOWORD(lParam);
        WinY = HIWORD(lParam);

        NewEvent.gx_event_type = GX_EVENT_PEN_DOWN;
        NewEvent.gx_event_payload.gx_event_pointdata.gx_point_x = WinX;
        NewEvent.gx_event_payload.gx_event_pointdata.gx_point_y = WinY;
        NewEvent.gx_event_display_handle = (ULONG)driver_data;
        gx_win32_event_to_guix(&NewEvent);
        return 0;

    case WM_LBUTTONUP:
        ReleaseCapture();
        WinX = LOWORD(lParam);
        WinY = HIWORD(lParam);

        NewEvent.gx_event_type = GX_EVENT_PEN_UP;
        NewEvent.gx_event_payload.gx_event_pointdata.gx_point_x = WinX;
        NewEvent.gx_event_payload.gx_event_pointdata.gx_point_y = WinY;
        NewEvent.gx_event_display_handle = (ULONG)driver_data;
        gx_win32_event_to_guix(&NewEvent);
        return 0;

    case WM_KEYDOWN:
        event_type = 0;
        check_key_event = GX_TRUE;

        if (wParam != VK_PROCESSKEY)
        {
            switch (wParam)
            {
            case VK_CAPITAL:
                caps_flag ^= (GX_TRUE);
                break;

            case VK_SHIFT:
                shift_flag = GX_TRUE;
                check_key_event = GX_FALSE;
                break;

            case VK_CONTROL:
                ctrl_flag = GX_TRUE;
                check_key_event = GX_FALSE;
                break;

            default:
                break;
            }

            if (shift_flag || ctrl_flag && check_key_event)
            {
                event_type = gx_win32_map_key_to_guix_event(wParam, shift_flag, ctrl_flag);
            }

            if (event_type)
            {
                NewEvent.gx_event_type = event_type;
            }
            else
            {
                NewEvent.gx_event_type = GX_EVENT_KEY_DOWN;
                NewEvent.gx_event_payload.gx_event_ushortdata[0] = gx_win32_map_key_to_guix_key(wParam);
            }
            gx_win32_event_to_guix(&NewEvent);
        }
        break;

    case WM_KEYUP:
        switch (wParam)
        {
        case VK_SHIFT:
            shift_flag = GX_FALSE;
            break;

        case VK_CONTROL:
            ctrl_flag = GX_FALSE;
            break;

        default:
            break;
        }

        NewEvent.gx_event_type = GX_EVENT_KEY_UP;
        NewEvent.gx_event_payload.gx_event_ushortdata[0] = gx_win32_map_key_to_guix_key(wParam);
        gx_win32_event_to_guix(&NewEvent);
        break;

    case WM_IME_CHAR:
        NewEvent.gx_event_type = GX_EVENT_KEY_DOWN;

        /* big-endian and little endian convert. */
        wide_char = ((wParam & 0xFF) << 8) + ((wParam & 0xFF00) >> 8);

        /* Convert multi-byte to unicode. */
        MultiByteToWideChar(CP_ACP, 0, (LPCSTR)&wide_char, 8, (LPWSTR)s_ch, 3);

        NewEvent.gx_event_payload.gx_event_ushortdata[0]  = s_ch[0];
        gx_win32_event_to_guix(&NewEvent);
        break;

    case WM_MOUSEMOVE:
        if (wParam & MK_LBUTTON)
        {
            WinX = LOWORD(lParam);
            WinY = HIWORD(lParam);

            NewEvent.gx_event_type = GX_EVENT_PEN_DRAG;
            NewEvent.gx_event_payload.gx_event_pointdata.gx_point_x = WinX;
            NewEvent.gx_event_payload.gx_event_pointdata.gx_point_y = WinY;
            NewEvent.gx_event_display_handle = (ULONG)driver_data;
            gx_win32_event_to_guix(&NewEvent);
        }
        return 0;

    case WM_SETCURSOR:
        return FALSE;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    gx_win32_input_driver                              PORTABLE C       */
/*                                                           6.1.10       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function handle windows message and send it to guix system     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    gx_win32_message_to_guix                                            */
/*    GetMessage                                                          */
/*    DispatchMessage                                                     */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    gx_win32_driver_thread_entry                                        */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  12-31-2020     Kenneth Maxwell          Initial Version 6.1.3         */
/*  01-31-2022     Ting Zhu                 Modified comment(s), added    */
/*                                            timer stop and thread       */
/*                                            handle close logic,         */
/*                                            resulting in version 6.1.10 */
/*                                                                        */
/**************************************************************************/
void gx_win32_input_driver(GX_WIN32_DISPLAY_DRIVER_DATA *instance)
{
int status;
MSG msg;
int exit_code = 0;

    while ((status = GetMessage(&msg, NULL, 0, 0)) != 0)
    {
        if ((msg.hwnd == NULL && msg.message == WM_USER) ||
            (status == -1))
        {
            /* Exit thread. */
            exit_code = -1;
            break;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    /* Stop multi-media timer.  */
    gx_win32_stop_multimedia_timer();

    /* Send an event to wake up the GUIX thread.  */
    gx_win32_message_to_guix(GX_EVENT_TERMINATE);

    if ((instance->win32_driver_thread_handle != INVALID_HANDLE_VALUE) &&
        instance->win32_driver_thread_handle)
    {
        CloseHandle(instance->win32_driver_thread_handle);
    }

    GX_WIN32_EVENT_THREAD_EXIT(exit_code);
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    gx_win32_driver_thread_entry                       PORTABLE C       */
/*                                                           6.1.7        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates window for guix and start guix thread in      */
/*    window.                                                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    thread_input                          Pointer to win32 display      */
/*                                            driver data                 */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    DWORD                                                               */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    gx_win32_window_create                Create Win32 window           */
/*    gx_win32_driver_thread_initialize     Initialize thread             */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    win32 guix setup functions                                          */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  12-31-2020     Kenneth Maxwell          Initial Version 6.1.3         */
/*  06-02-2021     Ting Zhu                 Modified comment(s),          */
/*                                            reorganized code,           */
/*                                            resulting in version 6.1.7  */
/*                                                                        */
/**************************************************************************/
void gx_win32_driver_thread_entry(ULONG thread_input)
{
/* create a MS Windows window to serve as our physical display
   This HAS to be done in the context of the win32 thread,
   otherwise we don't get messages from MS Windows. Would prefer
   to do this as part of the win32_graphics_driver function,
   but that runs in the context of the GUIX thread so we can't do
   it there. */
GX_WIN32_DISPLAY_DRIVER_DATA *instance = (GX_WIN32_DISPLAY_DRIVER_DATA *)thread_input;

    /* Create a win32 window for display. */
    instance -> win32_driver_winhandle = gx_win32_window_create(instance, gx_win32_event_process, 20, 20);

    gx_win32_driver_thread_initialize(instance);
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    gx_win32_driver_thread_initialize                  PORTABLE C       */
/*                                                           6.1.10       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function initializes thread.                                   */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    instance                              Pointer to win32 display      */
/*                                            driver data                 */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    DWORD                                                               */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    ShowWindow                            Active the specified window   */
/*    UpdateWindow                          Update the client area of the */
/*                                            specified window            */
/*    gx_win32_message_to_guix              Send message to guix          */
/*    SetTimer                              Set a timer                   */
/*    gx_win32_input_driver                 Handle Win32 messages         */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    win32 thread entry                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  06-02-2021     Ting Zhu                 Initial Version 6.1.7         */
/*  01-31-2022     Ting Zhu                 Modified comment(s), modified */
/*                                            to use multi-media timer,   */
/*                                            resulting in version 6.1.10 */
/*                                                                        */
/**************************************************************************/
void gx_win32_driver_thread_initialize(GX_WIN32_DISPLAY_DRIVER_DATA *instance)
{
    /* Show window. */
    ShowWindow(instance->win32_driver_winhandle, SW_SHOW);
    UpdateWindow(instance->win32_driver_winhandle);

    /* Mark the driver as ready. */
    instance->win32_driver_ready = 1;

    /* Driver is ready, force a redraw. */
    gx_win32_message_to_guix(GX_EVENT_REDRAW);

    /* Create a multi-media timer. */
    gx_win32_start_multimedia_timer(instance);

    /* Handle win32 messages. */
    gx_win32_input_driver(instance);
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    WinMain                                            PORTABLE C       */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function is win32 main function.                               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    int                                   Application exit status       */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    gx_main                               Application entry             */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  12-31-2020     Kenneth Maxwell          Initial Version 6.1.3         */
/*                                                                        */
/**************************************************************************/
int WINAPI WinMain(HINSTANCE Instance, HINSTANCE PrevInstance,
                   LPSTR pCmd, int CmdShow)
{
    return(gx_main(0, (char **)GX_NULL));
}
#endif /* WIN32 */

