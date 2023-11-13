/**************************************************************************/
/*                                                                        */
/*            Copyright (c) 1996-2016 by Express Logic Inc.               */
/*                                                                        */
/*  This software is copyrighted by and is the sole property of Express   */
/*  Logic, Inc.  All rights, title, ownership, or other interests         */
/*  in the software remain the property of Express Logic, Inc.  This      */
/*  software may only be used in accordance with the corresponding        */
/*  license agreement.  Any unauthorized use, duplication, transmission,  */
/*  distribution, or disclosure of this software is expressly forbidden.  */
/*                                                                        */
/*  This Copyright notice may not be removed or modified without prior    */
/*  written consent of Express Logic, Inc.                                */
/*                                                                        */
/*  Express Logic, Inc. reserves the right to modify this software        */
/*  without notice.                                                       */
/*                                                                        */
/*  Express Logic, Inc.                     info@expresslogic.com         */
/*  11423 West Bernardo Court               http://www.expresslogic.com   */
/*  San Diego, CA  92127                                                  */
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
/**************************************************************************/
/**************************************************************************/
/*                                                                        */
/*  APPLICATION INTERFACE DEFINITION                       RELEASE        */
/*                                                                        */
/*    gx_api.h                                            PORTABLE C      */
/*                                                           5.3.2        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    William E. Lamie, Express Logic, Inc.                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file defines the basic Application Interface (API) to the      */
/*    high-performance GUIX UI framework.  All service prototypes         */
/*    and data structure definitions are defined in this file.            */
/*    Please note that basic data type definitions and other architecture-*/
/*    specific information is contained in the file gx_port.h.            */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  11-24-2014     William E. Lamie         Initial Version 5.2           */
/*  01-16-2015     William E. Lamie         Modified comment(s),          */
/*                                            added missing API,          */
/*                                            resulting in version 5.2.1  */
/*  01-26-2015     William E. Lamie         Modified comment(s),          */
/*                                            resulting in version 5.2.2  */
/*  03-01-2015     William E. Lamie         Modified comment(s),          */
/*                                            resulting in version 5.2.3  */
/*  04-15-2015     William E. Lamie         Modified comment(s), added    */
/*                                            JPEG image resource support,*/
/*                                            resulting in version 5.2.4  */
/*  08-21-2015     William E. Lamie         Modified comment(s), added    */
/*                                            GX_SYSTEM_TIMER_MS so GUIX  */
/*                                            timer value can be modified,*/
/*                                            change GX_EVENT.target to   */
/*                                            GX_WIDGET pointer,          */
/*                                            Add optional user-defined   */
/*                                            data to GX_WIDGET control   */
/*                                            block structure definition, */
/*                                            Add gx_window_execute API,  */
/*                                            Modify prototype of         */
/*                                            gx_widget_event_generate    */
/*                                            API,                        */
/*                                            resulting in version 5.2.5  */
/*  09-21-2015     William E. Lamie         Modified comment(s), modified */
/*                                            circular gauge support,     */
/*                                            resulting in version 5.2.6  */
/*  02-22-2016     William E. Lamie         Modified comment(s), added    */
/*                                            new features and APIs, and  */
/*                                            fixed compiler warnings,    */
/*                                            resulting in version 5.3    */
/*  04-05-2016     William E. Lamie         Modified comment(s), added    */
/*                                            new features and APIs,      */
/*                                            resulting in version 5.3.1  */
/*  06-15-2016     William E. Lamie         Modified conmment(s) , added  */
/*                                            new features and APIs,      */
/*                                            resulting in version 5.3.2  */
/*                                                                        */
/**************************************************************************/

#ifndef GX_API_H
#define GX_API_H


/* Determine if a C++ compiler is being used.  If so, ensure that standard
   C is used to process the API information.  */

#ifdef __cplusplus

/* Yes, C++ compiler is present.  Use standard C.  */
extern   "C" {

#endif


/* Define the major/minor version information that can be used by the application
   and the GUIX source as well.  */

#define __PRODUCT_GUIX__
#define __GUIX_MAJOR_VERSION 5
#define __GUIX_MINOR_VERSION 3


#define GUIX_VALIDATION

/* Ensure that ThreadX error checking is disabled for GUIX source code.  */

#ifdef GX_SOURCE_CODE
#ifndef TX_DISABLE_ERROR_CHECKING
#define TX_DISABLE_ERROR_CHECKING
#endif
#endif


/* Include ThreadX API and GUIX port-specific include file.  */
#ifndef GX_DISABLE_THREADX_BINDING
#include "tx_api.h"
#define GX_THREADX_BINDING
#else

/* Define data types used by GUIX that are normally defined within ThreadX headers  */

#define VOID void
typedef char                                    CHAR;
typedef unsigned char                           UCHAR;
typedef int                                     INT;
typedef unsigned int                            UINT;
typedef long                                    LONG;
typedef unsigned long                           ULONG;
typedef short                                   SHORT;
typedef unsigned short                          USHORT;
#endif

#include "gx_port.h"

/* Define signed 8-bit value */
typedef signed char                             GX_BYTE;

/* Define unsigned 8-bit value */
typedef unsigned char                           GX_UBYTE;

/* Define Resource ID.  */

typedef ULONG                                   GX_RESOURCE_ID;

/* Define Color type.  */

typedef ULONG                                   GX_COLOR;

/* Define constants for GUIX.  */

/* Define configuration-specific constants. These constants can be redefined externally.  */

/* Default 20ms GUIX system timer.  */
#ifndef GX_SYSTEM_TIMER_MS
#define GX_SYSTEM_TIMER_MS        20
#endif

#if defined(GX_THREADX_BINDING)

/* Set default ThreadX timer tick frequency 100Hz (10ms timer). */
#ifndef TX_TIMER_TICKS_PER_SECOND
#define TX_TIMER_TICKS_PER_SECOND ((ULONG)100)
#endif



/* Derive GX_SYSTEM_TIMER_TICKS based on GX_SYSTEM_TIMER_MS value. */
#ifndef GX_SYSTEM_TIMER_TICKS
#define GX_SYSTEM_TIMER_TICKS     ((GX_SYSTEM_TIMER_MS * TX_TIMER_TICKS_PER_SECOND) / 1000)
#endif


#endif /* GX_THREADX_BINDING */

#ifndef GX_DISABLE_MULTITHREAD_SUPPORT
#define GX_MULTITHREAD_SUPPORT
#endif

#ifndef GX_DISABLE_UTF8_SUPPORT
#define GX_UTF8_SUPPORT
#endif

#ifndef GX_DISABLE_ARC_DRAWING_SUPPORT
#define GX_ARC_DRAWING_SUPPORT
#endif

#ifndef GX_DISABLE_SOFTWARE_DECODER_SUPPORT
#define GX_SOFTWARE_DECODER_SUPPORT
#endif

#ifndef GX_REPEAT_BUTTON_INITIAL_TICS
#define GX_REPEAT_BUTTON_INITIAL_TICS       10
#endif

#ifndef GX_REPEAT_BUTTON_REPEAT_TICS
#define GX_REPEAT_BUTTON_REPEAT_TICS        3
#endif

#ifndef GX_MAX_QUEUE_EVENTS
#define GX_MAX_QUEUE_EVENTS                 32
#endif

#ifndef GX_MAX_DIRTY_AREAS
#define GX_MAX_DIRTY_AREAS                  64
#endif

#ifndef GX_MAX_CONTEXT_NESTING
#define GX_MAX_CONTEXT_NESTING              8
#endif

#ifndef GX_MAX_INPUT_CAPTURE_NESTING
#define GX_MAX_INPUT_CAPTURE_NESTING        4
#endif

#ifndef GX_SYSTEM_THREAD_PRIORITY
#define GX_SYSTEM_THREAD_PRIORITY           16
#endif

#ifndef GX_SYSTEM_THREAD_TIMESLICE
#define GX_SYSTEM_THREAD_TIMESLICE          10
#endif

#ifndef GX_DEFAULT_BUFFER_SIZE
#define GX_DEFAULT_BUFFER_SIZE              100
#endif

#ifndef GX_CURSOR_BLINK_INTERVAL
#define GX_CURSOR_BLINK_INTERVAL            20
#endif

#ifndef GX_MULTI_LINE_INDEX_CACHE_SIZE
#define GX_MULTI_LINE_INDEX_CACHE_SIZE      32
#endif

#ifndef GX_MULTI_LINE_TEXT_BUTTON_MAX_LINES
#define GX_MULTI_LINE_TEXT_BUTTON_MAX_LINES 4
#endif

#ifndef GX_POLYGON_MAX_EDGE_NUM
#define GX_POLYGON_MAX_EDGE_NUM             20
#endif

#ifndef GX_DEFAULT_CIRCULAR_GAUGE_ANIMATION_DELAY
#define GX_DEFAULT_CIRCULAR_GAUGE_ANIMATION_DELAY 5
#endif

#ifndef GX_PARAMETER_NOT_USED
#define GX_PARAMETER_NOT_USED(p) ((void) (p))
#endif /* GX_PARAMETER_NOT_USED */

/* API input parameters and general constants.  */

#define GX_TRUE                             1
#define GX_FALSE                            0
#define GX_NULL                             0
#define GX_ID_NONE                          0
#define MIN_PEN_FLICK_SPEED                 20
#define MAX_PEN_SPEED_TICKS                 10


/* API return values.  */

#define GX_SUCCESS                          0x00
#define GX_DELETED                          0x01
#define GX_INVALID_ID                       0x02
#define GX_NO_VIEWS                         0x03
#define GX_OUT_OF_TIMERS                    0x04
#define GX_DRAW_NESTING_EXCEEDED            0x05
#define GX_INVALID_CONTEXT                  0x06
#define GX_PTR_ERROR                        0x07
#define GX_NO_CHANGE                        0x08
#define GX_NOT_FOUND                        0x09
#define GX_FAILURE                          0x10
#define GX_CALLER_ERROR                     0x11
#define GX_INVALID_WIDGET                   0x12
#define GX_ALREADY_CREATED                  0x13
#define GX_INVALID_WIDGET_SIZE              0x14
#define GX_INVALID_COLOR                    0x15
#define GX_INVALID_FONT                     0x16
#define GX_INVALID_STRING                   0x17
#define GX_INVALID_STYLE                    0x18
#define GX_INVALID_SIZE                     0x19
#define GX_INVALID_ALPHA                    0x1A
#define GX_INVALID_TYPE                     0x1B
#define GX_INVALID_CANVAS_SIZE              0x1C
#define GX_INVALID_DISPLAY                  0x1D
#define GX_INVALID_WIDTH                    0x1E
#define GX_INVALID_HEIGHT                   0x1F
#define GX_INVALID_CANVAS                   0x20
#define GX_INVALID_COORDINATE               0x21
#define GX_INVALID_RESOURCE_ID              0x22
#define GX_INVALID_VALUE                    0x22
#define GX_INVALID_DISPLAY_SIZE             0x23
#define GX_INVALID_FORMAT                   0x24
#define GX_INVALID_APPEARANCE               0x25
#define GX_INVALID_STATUS                   0x26
#define GX_INPUT_CAPTURE_NESTING_EXCEEDED   0x27
#define GX_NOT_SUPPORTED                    0x28
#define GX_INVALID_MEMORY_SIZE              0x29
#define GX_SYSTEM_MEMORY_ERROR              0x30
#define GX_SYSTEM_ERROR                     0xFE



/* Define system error constants.  */

#define GX_SYSTEM_THREAD_CREATE_FAILED      1
#define GX_SYSTEM_QUEUE_CREATE_FAILED       2
#define GX_SYSTEM_MUTEX_CREATE_FAILED       3
#define GX_SYSTEM_EVENT_SIZE_MISMATCH       4
#define GX_SYSTEM_EVENT_RECEIVE_ERROR       5
#define GX_SYSTEM_PROTECTION_ERROR          6
#define GX_SYSTEM_DRIVER_SETUP_ERROR        7
#define GX_SYSTEM_OUT_OF_VIEWS              8

/* Define supported frame buffer color formats.  */

#define GX_COLOR_FORMAT_MONOCHROME          1                   /* 0 = black, 1 = white                     */
#define GX_COLOR_FORMAT_MONOCHROME_INVERTED 2                   /* 1 = black, 0 = white                     */
#define GX_COLOR_FORMAT_2BIT_GRAY           3                   /* 2 bit-per-pixel grayscale                */
#define GX_COLOR_FORMAT_2BIT_GRAY_INVERTED  4                   /* 2 bit-per-pixel grayscale inverted       */
#define GX_COLOR_FORMAT_4BIT_GRAY           5
#define GX_COLOR_FORMAT_4BIT_GRAY_INVERTED  6
#define GX_COLOR_FORMAT_4BIT_VGA            7                   /* Obsolete VGA planar format, 16 colors    */
#define GX_COLOR_FORMAT_8BIT_GRAY           8                   /* 256 shades of gray                       */
#define GX_COLOR_FORMAT_8BIT_GRAY_INVERTED  9                   /* 256 shades of gray, 0 = white            */
#define GX_COLOR_FORMAT_8BIT_PALETTE        10                  /* 256 color palette mode                   */
#define GX_COLOR_FORMAT_8BIT_PACKED_PIXEL   11                  /* 3:3:2 color mode                         */
#define GX_COLOR_FORMAT_5551BGRX            12                  /* 15 bpp, b:g:r order                      */
#define GX_COLOR_FORMAT_1555XRGB            13                  /* 15 bpp, r:g:b order                      */
#define GX_COLOR_FORMAT_565RGB              14                  /* 16 bpp, r:g:b order                      */
#define GX_COLOR_FORMAT_4444ARGB            15                  /* 16 bpp, 4:4:4:4 argb order               */
#define GX_COLOR_FORMAT_4444BGRA            16                  /* 16 bpp, 4:4:4:4 bgra order               */
#define GX_COLOR_FORMAT_565BGR              17                  /* 16 bpp, b:g:r order                      */
#define GX_COLOR_FORMAT_24RGB               18                  /* 24 bpp, r:g:b order, packed              */
#define GX_COLOR_FORMAT_24BGR               19                  /* 24 bpp, b:g:r order, packed              */
#define GX_COLOR_FORMAT_24XRGB              20                  /* 24 bpp, r:g:b order, unpacked            */
#define GX_COLOR_FORMAT_24BGRX              21                  /* 24 bpp, b:g:r order, unpacked            */
#define GX_COLOR_FORMAT_32ARGB              22                  /* 32 bpp, a:r:g:b order                    */
#define GX_COLOR_FORMAT_32RGBA              23                  /* 32 bpp, r:g:b:a order                    */
#define GX_COLOR_FORMAT_32ABGR              24                  /* 32 BPP, a:b:g:r order                    */
#define GX_COLOR_FORMAT_32BGRA              25                  /* 32 BPP, b:g:r:a order                    */
#define GX_COLOR_FORMAT_8BIT_ALPHAMAP       26                  /* 8 bit alpha only                         */

/* Predefined color Resource IDs.  */

#define GX_COLOR_ID_CANVAS                  0
#define GX_COLOR_ID_WIDGET_FILL             1
#define GX_COLOR_ID_WINDOW_FILL             2
#define GX_COLOR_ID_DEFAULT_BORDER          3
#define GX_COLOR_ID_WINDOW_BORDER           4
#define GX_COLOR_ID_TEXT                    5
#define GX_COLOR_ID_SELECTED_TEXT           6
#define GX_COLOR_ID_SELECTED_FILL           7

#define GX_COLOR_ID_SHADOW                  8
#define GX_COLOR_ID_SHINE                   9

/* Button color Resource IDs.  */

#define GX_COLOR_ID_BUTTON_BORDER           10
#define GX_COLOR_ID_BUTTON_UPPER            11
#define GX_COLOR_ID_BUTTON_LOWER            12
#define GX_COLOR_ID_BUTTON_TEXT             13

/* Scrollbar color Resource IDs.   */

#define GX_COLOR_ID_SCROLL_FILL             14
#define GX_COLOR_ID_SCROLL_BUTTON           15

/* Text input color Resource IDs.  */

#define GX_COLOR_ID_TEXT_INPUT_TEXT         16
#define GX_COLOR_ID_TEXT_INPUT_FILL         17

/* slider color resource ids */

#define GX_COLOR_ID_SLIDER_TICK             18
#define GX_COLOR_ID_SLIDER_GROOVE_TOP       19      /* LIGHTGRAY */
#define GX_COLOR_ID_SLIDER_GROOVE_BOTTOM    20      /* WHITE */
#define GX_COLOR_ID_SLIDER_NEEDLE_OUTLINE   21      /* BLACK */
#define GX_COLOR_ID_SLIDER_NEEDLE_FILL      22      /* DARKGRAY */
#define GX_COLOR_ID_SLIDER_NEEDLE_LINE1     23      /* LIGHTGRAY */
#define GX_COLOR_ID_SLIDER_NEEDLE_LINE2     24      /* BTN_BORDER */

#define GX_MAX_DEFAULT_COLORS               25
#define GX_FIRST_USER_COLOR                 GX_MAX_DEFAULT_COLORS

/* Default font Resource IDs.  */

#define GX_FONT_ID_DEFAULT                  0
#define GX_FONT_ID_BUTTON                   1
#define GX_FONT_ID_PROMPT                   2
#define GX_FONT_ID_TEXT_INPUT               3
#define GX_DEFAULT_FONT_COUNT               4
#define GX_FIRST_USER_FONT                  GX_DEFAULT_FONT_COUNT

/* Default PIXELMAP Resource IDs. */

#define GX_PIXELMAP_NULL                    0
#define GX_PIXELMAP_RADIO_ON_ID             1
#define GX_PIXELMAP_RADIO_OFF_ID            2
#define GX_PIXELMAP_CHECKBOX_ON_ID          3
#define GX_PIXELMAP_CHECKBOX_OFF_ID         4
#define GX_DEFAULT_PIXELMAP_COUNT           5
#define GX_FIRST_USER_PIXELMAP              GX_DEFAULT_PIXELMAP_COUNT

/* Predefined a:r:g:b color values, these follow the standard 16 VGA color names.  */

#define GX_COLOR_BLACK                      0xff000000
#define GX_COLOR_RED                        0xffb80000
#define GX_COLOR_GREEN                      0xff00bc00
#define GX_COLOR_BROWN                      0xffb8bc00
#define GX_COLOR_BLUE                       0xff0000b8
#define GX_COLOR_MAGENTA                    0xffb800b8
#define GX_COLOR_CYAN                       0xff00bcb8
#define GX_COLOR_LIGHTGRAY                  0xffc0c0c0
#define GX_COLOR_DARKGRAY                   0xff808080
#define GX_COLOR_LIGHTRED                   0xffff0000
#define GX_COLOR_LIGHTGREEN                 0xff00ff00
#define GX_COLOR_YELLOW                     0xffffff00
#define GX_COLOR_LIGHTBLUE                  0xff0000ff
#define GX_COLOR_LIGHTMAGENTA               0xffff00ff
#define GX_COLOR_LIGHTCYAN                  0xff00ffff
#define GX_COLOR_WHITE                      0xffffffff

/* Define events. The the event ranges are:

   0x00000000 to 0x000000ff   256 pre-defined system event types
   0x00000100 to 0x00ffffff   Signals from child widgets
   0x01000000 to 0x3fffffff   Unused
   0x40000000 to 0xffffffff   Application defined

 */
#define GX_FIRST_SIGNAL                     0x100
#define GX_LAST_SIGNAL                      0x00ffffff
#define GX_SIGNAL_EVENT_MASK                0xff


/* Define the pre-defined Widget event types.  */

#define GX_EVENT_TERMINATE                  1
#define GX_EVENT_REDRAW                     2
#define GX_EVENT_SHOW                       3
#define GX_EVENT_HIDE                       4
#define GX_EVENT_RESIZE                     5
#define GX_EVENT_SLIDE                      6
#define GX_EVENT_FOCUS_GAINED               7
#define GX_EVENT_FOCUS_LOST                 8
#define GX_EVENT_HORIZONTAL_SCROLL          9
#define GX_EVENT_VERTICAL_SCROLL            10
#define GX_EVENT_TIMER                      11
#define GX_EVENT_PEN_DOWN                   12
#define GX_EVENT_PEN_UP                     13
#define GX_EVENT_PEN_DRAG                   14
#define GX_EVENT_KEY_DOWN                   15
#define GX_EVENT_KEY_UP                     16
#define GX_EVENT_CLOSE                      17
#define GX_EVENT_DESTROY                    18
#define GX_EVENT_SLIDER_VALUE               19
#define GX_EVENT_TOGGLE_ON                  20
#define GX_EVENT_TOGGLE_OFF                 21
#define GX_EVENT_RADIO_SELECT               22
#define GX_EVENT_RADIO_DESELECT             23
#define GX_EVENT_CLICKED                    24
#define GX_EVENT_LIST_SELECT                25
#define GX_EVENT_VERTICAL_FLICK             26
#define GX_EVENT_HORIZONTAL_FLICK           28
#define GX_EVENT_MOVE                       29
#define GX_EVENT_PARENT_SIZED               30
#define GX_EVENT_CLOSE_POPUP                31
#define GX_EVENT_ZOOM_IN                    32
#define GX_EVENT_ZOOM_OUT                   33
#define GX_EVENT_LANGUAGE_CHANGE            34
#define GX_EVENT_RESOURCE_CHANGE            35
#define GX_EVENT_ANIMATION_COMPLETE         36
#define GX_EVENT_SPRITE_COMPLETE            37
#define GX_EVENT_TEXT_EDITED                40
#define GX_EVENT_TX_TIMER                   41
#define GX_EVENT_FOCUS_NEXT                 42
#define GX_EVENT_FOCUS_PREVIOUS             43
#define GX_EVENT_FOCUS_GAIN_NOTIFY          44
#define GX_EVENT_SELECT                     45
#define GX_EVENT_DESELECT                   46
#define GX_EVENT_PROGRESS_VALUE             47


/* Define the value of the first application defined event type.  */

#define GX_FIRST_APP_EVENT                  0x40000000


/* Define logical key values.  */

#define GX_KEY_PAGE_UP                      0x1001
#define GX_KEY_PAGE_DOWN                    0x1002
#define GX_KEY_END                          0x1003
#define GX_KEY_HOME                         0x1004
#define GX_KEY_LEFT_ARROW                   0x1005
#define GX_KEY_UP_ARROW                     0x1006
#define GX_KEY_RIGHT_ARROW                  0x1007
#define GX_KEY_DOWN_ARROW                   0x1008
#define GX_KEY_NEXT                         0x1009
#define GX_KEY_PREVIOUS                     0x100a

#define GX_KEY_BACKSPACE                    0x08
#define GX_KEY_SPACE                        0x20
#define GX_KEY_DELETE                       0x7f
#define GX_KEY_CARRIAGE_RETURN              0x0d
#define GX_KEY_SELECT                       GX_KEY_CARRIAGE_RETURN


/* Define the Widget types.  */

#define GX_TYPE_WIDGET                      1
#define GX_TYPE_BUTTON                      2
#define GX_TYPE_TEXT_BUTTON                 3
#define GX_TYPE_MULTI_LINE_TEXT_BUTTON      4
#define GX_TYPE_RADIO_BUTTON                5
#define GX_TYPE_CHECKBOX                    6
#define GX_TYPE_PIXELMAP_BUTTON             7
#define GX_TYPE_SHADOW_BUTTON               8
#define GX_TYPE_ICON_BUTTON                 9
#define GX_TYPE_SPIN_BUTTON                 10
#define GX_TYPE_ICON                        11
#define GX_TYPE_SPRITE                      12
#define GX_TYPE_CIRCULAR_GAUGE              13

#define GX_TYPE_SLIDER                      20
#define GX_TYPE_PIXELMAP_SLIDER             21
#define GX_TYPE_VERTICAL_SCROLL             22
#define GX_TYPE_HORIZONTAL_SCROLL           23
#define GX_TYPE_PROGRESS_BAR                24
#define GX_TYPE_RADIAL_PROGRESS_BAR         25

#define GX_TYPE_PROMPT                      30
#define GX_TYPE_NUMERIC_PROMPT              31
#define GX_TYPE_PIXELMAP_PROMPT             32
#define GX_TYPE_NUMERIC_PIXELMAP_PROMPT     33

#define GX_TYPE_SINGLE_LINE_TEXT_INPUT      64
#define GX_TYPE_PIXELMAP_TEXT_INPUT         65
#define GX_TYPE_DROP_LIST                   70

/* Define the Window derived types. These types must always be >= GX_TYPE_WINDOW
   to insure type-safe casting in the GUIX internal code.  */

#define GX_TYPE_WINDOW                      128
#define GX_TYPE_ROOT_WINDOW                 129
#define GX_TYPE_VERTICAL_LIST               131
#define GX_TYPE_HORIZONTAL_LIST             132
#define GX_TYPE_POPUP_LIST                  133
#define GX_TYPE_MULTI_LINE_TEXT_VIEW        134
#define GX_TYPE_MULTI_LINE_TEXT_INPUT       135
#define GX_TYPE_LINE_CHART                  136
#define GX_TYPE_DIALOG                      137
#define GX_TYPE_KEYBOARD                    138

/* A pseudo-type, used by Studio code generator */ 
#define GX_TYPE_TEMPLATE                    200

/* Applications can define their own window types */
#define GX_FIRST_USER_WINDOW_TYPE           256


/* Define Widget status flags.  */

#define GX_STATUS_VISIBLE                   0x00000001UL
#define GX_STATUS_SELECTABLE                0x00000002UL
#define GX_STATUS_ACCEPTS_INPUT             0x00000004UL
#define GX_STATUS_DEFAULT_FOCUS             0x00000010UL
#define GX_STATUS_ACCEPTS_FOCUS             0x00000020UL
#define GX_STATUS_HAS_FOCUS                 0x00000040UL
#define GX_STATUS_NOTIFY_ON_GAIN_FOCUS      0x00000080UL
#define GX_STATUS_NAV_STOP                  0x00000100UL
#define GX_STATUS_NAV_PARENT                0x00000200UL
#define GX_STATUS_MOVABLE                   0x00001000UL
#define GX_STATUS_BUTTON_DERIVED            0x00002000UL
#define GX_STATUS_TOGGLE_UNLOCK             0x00004000UL
#define GX_STATUS_DIRTY                     0x00008000UL
#define GX_STATUS_TRANSPARENT               0x10000000UL
#define GX_STATUS_NONCLIENT                 0x20000000UL
#define GX_STATUS_OWNS_INPUT                0x40000000UL
#define GX_STATUS_DYNAMICALLY_ALLOCATED     0x80000000UL


/* Define Widget style flags.  */

#define GX_STYLE_NONE                       0x00000000UL
#define GX_STYLE_BORDER_NONE                0x00000000UL
#define GX_STYLE_BORDER_RAISED              0x00000001UL
#define GX_STYLE_BORDER_RECESSED            0x00000002UL
#define GX_STYLE_BORDER_THIN                0x00000004UL
#define GX_STYLE_BORDER_THICK               0x00000008UL
#define GX_STYLE_BORDER_MASK                0x0000000fUL


/* Define global style flags.  */

#define GX_STYLE_TRANSPARENT                0x10000000UL
#define GX_STYLE_DRAW_SELECTED              0x20000000UL
#define GX_STYLE_ENABLED                    0x40000000UL
#define GX_STYLE_DYNAMICALLY_ALLOCATED      0x80000000UL

/* Define Button style flags.  */

#define GX_STYLE_BUTTON_PUSHED              0x00000010UL
#define GX_STYLE_BUTTON_TOGGLE              0x00000020UL
#define GX_STYLE_BUTTON_RADIO               0x00000040UL
#define GX_STYLE_BUTTON_EVENT_ON_PUSH       0x00000080UL
#define GX_STYLE_BUTTON_REPEAT              0x00000100UL

/* Define Sprite style flags */

#define GX_STYLE_SPRITE_AUTO                0x00000010UL
#define GX_STYLE_SPRITE_LOOP                0x00000020UL

/* Define List style flags.  */

#define GX_STYLE_CENTER_SELECTED            0x00000010UL
#define GX_STYLE_WRAP                       0x00000020UL
#define GX_STYLE_FLICKABLE                  0x00000040UL


/* Define Pixelmap button and icon button alignment flags.  */

#define GX_STYLE_HALIGN_CENTER              0x00010000UL
#define GX_STYLE_HALIGN_LEFT                0x00020000UL
#define GX_STYLE_HALIGN_RIGHT               0x00040000UL
#define GX_STYLE_VALIGN_CENTER              0x00080000UL
#define GX_STYLE_VALIGN_TOP                 0x00100000UL
#define GX_STYLE_VALIGN_BOTTOM              0x00200000UL

#define GX_PIXELMAP_HALIGN_MASK             0x00070000UL
#define GX_PIXELMAP_VALIGN_MASK             0x00380000UL


/* Define Slider style flags.  */

#define GX_STYLE_SHOW_NEEDLE                0x00000200
#define GX_STYLE_SHOW_TICKMARKS             0x00000400
#define GX_STYLE_SLIDER_VERTICAL            0x00000800

/* Define Pixelmap Slider flags.  */

#define GX_STYLE_TILE_BACKGROUND            0x00001000

/* Define Progress Bar style flags.  */

#define GX_STYLE_PROGRESS_PERCENT           0x00000010
#define GX_STYLE_PROGRESS_TEXT_DRAW         0x00000020
#define GX_STYLE_PROGRESS_VERTICAL          0x00000040

#define GX_STYLE_PROGRESS_SEGMENTED_FILL    0x00000100

/* Define Radial Progress Bar style flags.  */
#define GX_STYLE_RADIAL_PROGRESS_ALIAS      0x00000200
#define GX_STYLE_RADIAL_PROGRESS_ROUND      0x00000400

/* Define Text alignment styles.  */

#define GX_STYLE_TEXT_LEFT                  0x00001000
#define GX_STYLE_TEXT_RIGHT                 0x00002000
#define GX_STYLE_TEXT_CENTER                0x00004000
#define GX_STYLE_TEXT_ALIGNMENT_MASK        0x00007000
#define GX_STYLE_TEXT_COPY                  0x00008000

/* define cursor status flags */
#define GX_STATUS_CURSOR_SHOW               0x00001000
#define GX_STATUS_CURSOR_DRAW               0x00002000

/* define cursor style flags */
#define GX_STYLE_CURSOR_BLINK               0x00000040
#define GX_STYLE_CURSOR_ALWAYS              0x00000080

/* text input style */
#define GX_STYLE_TEXT_INPUT_NOTIFY_ALL      0x00000100

/* Define Window styles.  */
#define GX_STYLE_TILE_WALLPAPER             0x00040000
#define GX_STYLE_AUTO_HORIZONTAL_SCROLL     0x00100000
#define GX_STYLE_AUTO_VERTICAL_SCROLL       0x00200000

/* Define circular gauge styles.  */
#define GX_STYLE_CIRCULAR_GAUGE_CCW         0x00000010

/* Define Scroll Bar styles.  */

#define GX_SCROLLBAR_BACKGROUND_TILE        0x00010000
#define GX_SCROLLBAR_RELATIVE_THUMB         0x00020000
#define GX_SCROLLBAR_END_BUTTONS            0x00040000
#define GX_SCROLLBAR_VERTICAL               0x01000000
#define GX_SCROLLBAR_HORIZONTAL             0x02000000


/* Define pre-defined animation types.  */

#define GX_ANIMATION_NONE                   0x00
#define GX_ANIMATION_SLIDE_IN               0x01
#define GX_ANIMATION_SLIDE_OUT              0x02
#define GX_ANIMATION_FADE_IN                0x10
#define GX_ANIMATION_FADE_OUT               0x20

/* Sprite background operations */

#define GX_SPRITE_BACKGROUND_NO_ACTION      0x00     /* do nothing between frames            */
#define GX_SPRITE_BACKGROUND_SOLID_FILL     0x01     /* draw background with solid color     */
#define GX_SPRITE_BACKGROUND_RESTORE        0x02     /* draw background with first image     */


/* Define Font information.  */

#define GX_FONT_ALIASED                     0x01

/* Define Brush attributes.  */

#define GX_BRUSH_OUTLINE                    0x0000              /* for rectangle, text, polygon              */
#define GX_BRUSH_SOLID_FILL                 0x0001              /* For rectangle, text, polygon              */
#define GX_BRUSH_PIXELMAP_FILL              0x0002              /* For rectangle, polygon                    */
#define GX_BRUSH_ALIAS                      0x0004              /* For lines                                 */
#define GX_BRUSH_UNDERLINE                  0x0008              /* Underlined text                           */
#define GX_BRUSH_ROUND                      0x0010              /* For lines                                 */


/* Define Canvas information.  */

#define GX_CANVAS_SIMPLE                    0x01                /* Canvas is memory-only canvas (no display) */
#define GX_CANVAS_MANAGED                   0x02                /* canvas is composited to frame buffer      */
#define GX_CANVAS_VISIBLE                   0x04                /* Canvas is visible                         */
#define GX_CANVAS_MODIFIED                  0x08
#define GX_CANVAS_COMPOSITE                 0x20                /* Canvas is a buffer for creating composite*/
#define GX_CANVAS_HARDWARE                  0x40

/* Define common combinations.  */

#define GX_CANVAS_MANAGED_VISIBLE           (GX_CANVAS_MANAGED | GX_CANVAS_VISIBLE)
#define GX_ALPHA_VALUE_OPAQUE               255


/* Define pre-defined Widget Resource IDs.  */

#define GX_ID_VERTICAL_SCROLL               0xff00
#define GX_ID_HORIZONTAL_SCROLL             0xff01
#define GX_ID_SCROLL_UP_LEFT                0xff02
#define GX_ID_SCROLL_DOWN_RIGHT             0xff03
#define GX_ID_SCROLL_THUMB                  0xff04
#define GX_ID_DROP_BUTTON                   0xff05

/* pre-defined timer IDs */
#define GX_BUTTON_TIMER                     0xff00
#define GX_CIRCULAR_GAUGE_TIMER             0xff00

/* Define the echo mode of Single-line Text input.  */

#define GX_STAR_ECHO                        0x01
#define GX_ECHO                             0x02

/* Define image type.  */
#define GX_IMAGE_TYPE_BMP                   0x01
#define GX_IMAGE_TYPE_GIF                   0x02
#define GX_IMAGE_TYPE_PNG                   0X03
#define GX_IMAGE_TYPE_JPG                   0X04


/* Define macros used in GUIX.  */
#ifndef GX_MIN
#define GX_MIN(_a, _b)                      (((_a) < (_b)) ? (_a) : (_b))
#endif

#ifndef GX_ABS
#define GX_ABS(_a)                          (((_a) < 0) ? (-(_a)) : (_a))
#endif

#ifndef GX_MEMCPY
#define GX_MEMCPY(a, b, c)                  memcpy(a, b, c)
#endif

#ifndef GX_CHAR
#define GX_CHAR                             char
#endif

#ifndef GX_STRING
#define GX_STRLEN(a)                        strlen((char *)a)
#endif

#ifndef GX_STRCPY
#define GX_STRCPY(a, b)                     strcpy(a, b)
#endif

#ifndef GX_STRCAT
#define GX_STRCAT(a, b)                     strcat(a, b)
#endif

#define GX_SIGNAL(_a, _b)                   (((_a) << 8) | (_b))
#define IS_SIGNAL(_a)                       (_a >= GX_FIRST_SIGNAL && _a <= LAST_SIGNAL)
#define GX_SWAP_VALS(_a, _b)                {_a ^= _b; _b ^= _a; _a ^= _b; }
#define GX_SEARCH_DEPTH_INFINITE            65535

/* Define the Point type.  */

typedef struct GX_POINT_STRUCT
{
    GX_VALUE gx_point_x;
    GX_VALUE gx_point_y;
} GX_POINT;

/* Define the edge type. */
typedef struct GX_POLYGON_EDGE_STRUCT
{
    INT gx_polygon_edge_xi;
    INT gx_polygon_edge_dx;
    INT gx_polygon_edge_dy;
    INT gx_polygon_edge_sign;
    INT gx_polygon_edge_x_decision;
    INT gx_polygon_edge_y_decision;
    INT gx_polygon_edge_ymax;
    INT gx_polygon_edge_ymin;
} GX_POLYGON_EDGE;

/* Define the Rectangle type.  */

typedef struct GX_RECTANGLE_STRUCT
{
    GX_VALUE gx_rectangle_left;
    GX_VALUE gx_rectangle_top;
    GX_VALUE gx_rectangle_right;
    GX_VALUE gx_rectangle_bottom;
} GX_RECTANGLE;


/* Define the View type.  */

typedef struct GX_VIEW_STRUCT
{
    GX_RECTANGLE           gx_view_rectangle;
    struct GX_VIEW_STRUCT *gx_view_next;
} GX_VIEW;


/* Define the Glyph type.  */

typedef struct GX_GLYPH_STRUCT
{
    GX_CONST GX_UBYTE *gx_glyph_map;                            /* Pointer to glyph data                           */
    USHORT             gx_glyph_map_size;                       /* 15bit: 1 for compression, 0 ~ 14 bits: map size */
    GX_BYTE            gx_glyph_ascent;                         /* Glyph ascent above baseline                     */
    GX_BYTE            gx_glyph_descent;                        /* Glyph descent below baseline                    */
    GX_BYTE            gx_glyph_advance;                        /* Glyph advance                                   */
    GX_BYTE            gx_glyph_leading;                        /* Pen x-pre-advance                               */
    GX_UBYTE           gx_glyph_width;
    GX_UBYTE           gx_glyph_height;
} GX_GLYPH;


/* Define the Font type.  */

typedef struct GX_FONT_STRUCT
{
    GX_UBYTE                     gx_font_format;                /* Format flags, defined below              */
    GX_UBYTE                     gx_font_prespace;              /* Line spacing above, pixels               */
    GX_UBYTE                     gx_font_postspace;             /* Line spacing below, pixels               */
    GX_UBYTE                     gx_font_line_height;           /* Font height, excluding spacing           */
    GX_UBYTE                     gx_font_baseline;              /* Font baseline                            */
    USHORT                       gx_font_first_glyph;           /* First glyph on this page                 */
    USHORT                       gx_font_last_glyph;            /* Last glyph on this page                  */
    GX_CONST GX_GLYPH           *gx_font_glyphs;                /* Array of GX_GLYPH structures             */
    const struct GX_FONT_STRUCT *gx_font_next_page;             /* For multiple page fonts (Unicode)        */
} GX_FONT;

#define GX_FONT_FORMAT_BPP_MASK   0x0F
#define GX_FONT_FORMAT_1BPP       0x01
#define GX_FONT_FORMAT_2BPP       0x02
#define GX_FONT_FORMAT_4BPP       0x04
#define GX_FONT_FORMAT_8BPP       0x08

#define GX_FONT_FORMAT_COMPRESSED 0x10
#define GX_FONT_FORMAT_FREETYPE   0x20


/* Define Pixelmap type.  */

typedef struct GX_PIXELMAP_STRUCT
{
    GX_UBYTE           gx_pixelmap_version_major;               /* Pixelmap major version number            */
    GX_UBYTE           gx_pixelmap_version_minor;               /* Pixelmap minor version number            */
    GX_UBYTE           gx_pixelmap_flags;                       /* Flags defined below                      */
    GX_UBYTE           gx_pixelmap_format;                      /* Color format, defined below              */
    GX_CONST GX_UBYTE *gx_pixelmap_data;                        /* Pointer to pixelmap data                 */
    ULONG              gx_pixelmap_data_size;                   /* Size of the pixelmap data array          */
    GX_CONST GX_UBYTE *gx_pixelmap_aux_data;                    /* Auxiliary data array.                    */
    ULONG              gx_pixelmap_aux_data_size;               /* Size of the auxiliary data array         */
    ULONG              gx_pixelmap_transparent_color;           /* Used for translated GIF images           */
    GX_VALUE           gx_pixelmap_width;                       /* Width - in pixels                        */
    GX_VALUE           gx_pixelmap_height;                      /* Height - in pixels                       */
} GX_PIXELMAP;


/* Pixelmap flags that can be in GX_PIXELMAP.gx_pixelmap_flags above.  */

#define GX_PIXELMAP_TRANSPARENT    0x01                         /* Pixelmap has transparent pixels          */
#define GX_PIXELMAP_COMPRESSED     0x02                         /* Pixelmap is compressed format            */
#define GX_PIXELMAP_ALPHA          0x04                         /* Pixelmap has alpha channel               */
#define GX_PIXELMAP_TARGA          0x08                         /* Pixelmap uses Targa format compresssion  */
#define GX_PIXELMAP_RAW_FORMAT     0x10                         /* RAW JPG/PNG format                       */

#define PIXELMAP_IS_TRANSPARENT(a) (a -> gx_pixelmap_flags & (GX_PIXELMAP_TRANSPARENT | GX_PIXELMAP_ALPHA))


/* Define Brush type.  */

typedef struct GX_BRUSH_STRUCT
{
    GX_PIXELMAP *gx_brush_pixelmap;                             /* Used for fills and line ends             */
    GX_FONT     *gx_brush_font;                                 /* Used for drawing text                    */
    ULONG        gx_brush_line_pattern;                         /* Used for drawing dashed lines            */
    ULONG        gx_brush_pattern_mask;                         /* Used for drawing dashed lines            */
    GX_COLOR     gx_brush_fill_color;                           /* For solid color fills                    */
    GX_COLOR     gx_brush_line_color;                           /* For lines and outlines                   */
    UINT         gx_brush_style;                                /* Flags defined below                      */
    GX_VALUE     gx_brush_width;                                /* Line width                               */
    UCHAR        gx_brush_alpha;                                /* for blending                             */
} GX_BRUSH;


/* Define Dirty Area type.  */

struct GX_WIDGET_STRUCT;

typedef struct GX_DIRTY_AREA_STRUCT
{
    GX_RECTANGLE             gx_dirty_area_rectangle;
    struct GX_WIDGET_STRUCT *gx_dirty_area_widget;
} GX_DIRTY_AREA;


/* Define Event type.  Note: the size of this structure must be less than or equal to the constant
   GX_EVENT_SIZE defined previously.  */

typedef struct GX_EVENT_STRUCT
{
    ULONG  gx_event_type;                                       /* Global event type                        */
    ULONG  gx_event_display_handle;
    struct GX_WIDGET_STRUCT *gx_event_target;                   /* receiver of event                        */
    USHORT gx_event_sender;                                     /* ID of the event sender                   */
    union
    {
        UINT     gx_event_timer_id;
        GX_POINT gx_event_pointdata;
        GX_UBYTE gx_event_uchardata[4];
        USHORT   gx_event_ushortdata[2];
        ULONG    gx_event_ulongdata;
        GX_BYTE  gx_event_chardata[4];
        SHORT    gx_event_shortdata[2];
        INT      gx_event_intdata[2];
        LONG     gx_event_longdata;
    } gx_event_payload;
} GX_EVENT;

#define GX_EVENT_SIZE   sizeof(GX_EVENT)
#define GX_EVENT_ULONGS ((GX_EVENT_SIZE + 3) / sizeof(ULONG))


/* Define Scroll information structure. This structure is used to define the position
   and scaling of the scroll bar thumb button.  */

typedef struct GX_SCROLL_INFO_STRUCT
{
    GX_VALUE gx_scroll_value;
    GX_VALUE gx_scroll_minimum;
    GX_VALUE gx_scroll_maximum;
    GX_VALUE gx_scroll_visible;
    GX_VALUE gx_scroll_increment;
} GX_SCROLL_INFO;


/* Define Slider information structure. This structure is used to define the
   appearance of a Pixelmap Slider.  */

typedef struct GX_SLIDER_INFO_STRUCT
{
    INT      gx_slider_info_min_val;
    INT      gx_slider_info_max_val;
    INT      gx_slider_info_current_val;
    INT      gx_slider_info_increment;
    GX_VALUE gx_slider_info_min_travel;
    GX_VALUE gx_slider_info_max_travel;
    GX_VALUE gx_slider_info_needle_pos;
    GX_VALUE gx_slider_info_needle_width;
    GX_VALUE gx_slider_info_needle_height;
    GX_VALUE gx_slider_info_needle_inset;
    GX_VALUE gx_slider_info_needle_hotspot_offset;
} GX_SLIDER_INFO;


typedef struct GX_PIXELMAP_SLIDER_INFO_STRUCT
{
    GX_VALUE       gx_pixelmap_slider_info_needle_inset;
    GX_RESOURCE_ID gx_pixelmap_slider_info_lower_background_pixelmap;
    GX_RESOURCE_ID gx_pixelmap_slider_info_upper_background_pixelmap;
    GX_RESOURCE_ID gx_pixelmap_slider_info_needle_pixelmap;
} GX_PIXELMAP_SLIDER_INFO;

/* Define Progress bar information structure. This structure is used to define the
   appearance of a Progress bar.  */

typedef struct GX_PROGRESS_BAR_INFO_STRUCT
{
    INT            gx_progress_bar_info_min_val;
    INT            gx_progress_bar_info_max_val;
    INT            gx_progress_bar_info_current_val;
    GX_RESOURCE_ID gx_progress_bar_font_id;
    GX_RESOURCE_ID gx_progress_bar_normal_text_color;
    GX_RESOURCE_ID gx_progress_bar_selected_text_color;
    GX_RESOURCE_ID gx_progress_bar_fill_pixelmap;
} GX_PROGRESS_BAR_INFO;

/* Define circular gauge information structure, this structure is used to 
   define the appearance of the circular gauge widget.  */
typedef struct GX_CIRCULAR_GAUGE_INFO_STRUCT
{
    /* Rotating steps.  */
    INT gx_circular_gauge_info_animation_steps;

    /* Delay time between each step.  */
    INT gx_circular_gauge_info_animation_delay;

    /* Offset of needle cor relative to parent.  */
    GX_VALUE gx_circular_gauge_info_needle_xpos;
    GX_VALUE gx_circular_gauge_info_needle_ypos;

    /* Rotation center. */
    GX_VALUE gx_circular_gauge_info_needle_xcor;
    GX_VALUE gx_circular_gauge_info_needle_ycor;
    GX_RESOURCE_ID gx_circular_gauge_info_needle_pixelmap;
} GX_CIRCULAR_GAUGE_INFO;

/* Define radial progress bar information structure, this structure is used to
   define the appearance of the radial progress bar widget.  */

typedef struct GX_RADIAL_PROGRESS_BAR_INFO_STRUCT
{
    GX_VALUE       gx_radial_progress_bar_info_xcenter;
    GX_VALUE       gx_radial_progress_bar_info_ycenter;
    GX_VALUE       gx_radial_progress_bar_info_radius;                 /* radius of circular track.  */
    GX_VALUE       gx_radial_progress_bar_info_current_val;            /* current value.  */
    GX_VALUE       gx_radial_progress_bar_info_anchor_val;             /* starting angle for the circular arc.  */
    GX_RESOURCE_ID gx_radial_progress_bar_info_font_id;
    GX_RESOURCE_ID gx_radial_progress_bar_info_normal_text_color;      /* resource ID of color for normal text.  */
    GX_RESOURCE_ID gx_radial_progress_bar_info_selected_text_color;    /* resource ID of color for selected text.  */
    GX_VALUE       gx_radial_progress_bar_info_normal_brush_width;     /* width of lower track.  */
    GX_VALUE       gx_radial_progress_bar_info_selected_brush_width;   /* width of upper track.  */
    GX_RESOURCE_ID gx_radial_progress_bar_info_normal_brush_color;     /* resource ID of color for lower track.  */
    GX_RESOURCE_ID gx_radial_progress_bar_info_selected_brush_color;   /* resource ID of color for upper track.  */
    
} GX_RADIAL_PROGRESS_BAR_INFO;

/* Define GX_LINE_CHART information structure, this structure is used to
   define the appearance of the line chart widget.  */
typedef struct GX_LINE_CHART_INFO_STRUCT
{
    INT            gx_line_chart_min_val;
    INT            gx_line_chart_max_val;
    INT           *gx_line_chart_data;
    GX_VALUE       gx_line_chart_left_margin;
    GX_VALUE       gx_line_chart_top_margin;
    GX_VALUE       gx_line_chart_right_margin;
    GX_VALUE       gx_line_chart_bottom_margin;
    GX_VALUE       gx_line_chart_max_data_count;
    GX_VALUE       gx_line_chart_active_data_count;
    GX_VALUE       gx_line_chart_axis_line_width;
    GX_VALUE       gx_line_chart_data_line_width;
    GX_RESOURCE_ID gx_line_chart_axis_color;
    GX_RESOURCE_ID gx_line_chart_line_color;
} GX_LINE_CHART_INFO;


/* Define Scroll Appearance structure. This structure is used to define the
   appearance of a scroll bar.  */

typedef struct GX_SCROLLBAR_APPEARANCE_STRUCT
{
    GX_VALUE       gx_scroll_width;
    GX_VALUE       gx_scroll_thumb_width;
    GX_VALUE       gx_scroll_thumb_travel_min;
    GX_VALUE       gx_scroll_thumb_travel_max;
    GX_UBYTE       gx_scroll_thumb_border_style;
    GX_RESOURCE_ID gx_scroll_fill_pixelmap;
    GX_RESOURCE_ID gx_scroll_thumb_pixelmap;
    GX_RESOURCE_ID gx_scroll_up_pixelmap;
    GX_RESOURCE_ID gx_scroll_down_pixelmap;
    GX_RESOURCE_ID gx_scroll_thumb_color;
    GX_RESOURCE_ID gx_scroll_thumb_border_color;
    GX_RESOURCE_ID gx_scroll_button_color;
} GX_SCROLLBAR_APPEARANCE;

/* Sprite frame structure. This structure is used to define the
   parameters of a GX_SPRITE animation sequence.  */

typedef struct GX_SPRITE_FRAME_STRUCT
{
    GX_RESOURCE_ID gx_sprite_frame_pixelmap;
    GX_VALUE       gx_sprite_frame_x_offset;
    GX_VALUE       gx_sprite_frame_y_offset;
    UINT           gx_sprite_frame_delay;
    UINT           gx_sprite_frame_background_operation;
    UCHAR          gx_sprite_frame_alpha;
} GX_SPRITE_FRAME;


/* Define Drawing Context type.  */

typedef struct GX_DRAW_CONTEXT_STRUCT
{
    GX_BRUSH                  gx_draw_context_brush;
    struct GX_CANVAS_STRUCT  *gx_draw_context_canvas;
    struct GX_DISPLAY_STRUCT *gx_draw_context_display;
    GX_VIEW                  *gx_draw_context_view_head;
    GX_VIEW                   gx_draw_context_simple_view;
    GX_COLOR                 *gx_draw_context_memory;
    GX_RECTANGLE              gx_draw_context_dirty;
    GX_RECTANGLE             *gx_draw_context_clip;
    GX_VALUE                  gx_draw_context_pitch;
} GX_DRAW_CONTEXT;


/* Define the display type.  */

typedef struct GX_DISPLAY_STRUCT
{
    /* Define the display ID and name. The display ID is used simply to see if the
       control block is valid.  */
    ULONG                     gx_display_id;                    /* Control block ID                         */
    GX_CONST GX_CHAR         *gx_display_name;                  /* Pointer to display's name                */

    struct GX_DISPLAY_STRUCT *gx_display_created_next;          /* Pointer to next control block            */
    struct GX_DISPLAY_STRUCT *gx_display_created_previous;      /* Pointer to previous control block        */

    GX_VALUE                  gx_display_color_format;
    GX_VALUE                  gx_display_width;
    GX_VALUE                  gx_display_height;

    GX_COLOR                 *gx_display_color_table;           /* color ID to native value mapping table   */
    GX_PIXELMAP             **gx_display_pixelmap_table;        /* pixelmap ID to GX_PIXELMAP mapping table */
    GX_FONT                 **gx_display_font_table;            /* font ID to GX_FONT mapping table         */
    GX_COLOR                 *gx_display_palette;               /* only used for 8-bpp palette mode driver  */

    UINT                      gx_display_color_table_size;
    UINT                      gx_display_pixelmap_table_size;
    UINT                      gx_display_font_table_size;
    UINT                      gx_display_palette_size;          /* only used for 8-bpp palette mode driver */

    ULONG                     gx_display_handle;
    UINT                      gx_display_driver_ready;

    VOID                     *gx_display_driver_data;
    VOID                     *gx_display_accelerator;

    /* function to initiate drawing sequence */
    VOID                    (*gx_display_driver_drawing_initiate)(struct GX_DISPLAY_STRUCT *display, struct GX_CANVAS_STRUCT *canvas);

    /* function to terminate drawing sequence */
    VOID                    (*gx_display_driver_drawing_complete)(struct GX_DISPLAY_STRUCT *display, struct GX_CANVAS_STRUCT *canvas);

    /* function for installing palette (only used for certain palette mode drivers) */
    VOID                     (*gx_display_driver_palette_set)(struct GX_DISPLAY_STRUCT *display, GX_COLOR *palette, INT count);

    /* Function for drawing non-aliased, single pixel line */
    VOID                     (*gx_display_driver_simple_line_draw)(GX_DRAW_CONTEXT *context,
                                                                   INT x1, INT y1, INT x2, INT y2);

    /* Function for drawing non-aliased, wide line */
    VOID                     (*gx_display_driver_simple_wide_line_draw)(GX_DRAW_CONTEXT *context,
                                                                        INT x1, INT y1, INT x2, INT y2);

    /* Function for drawing anti-aliased aliased, single-pixel line */
    VOID                     (*gx_display_driver_anti_aliased_line_draw)(GX_DRAW_CONTEXT *context,
                                                                         INT x1, INT y1, INT x2, INT y2);

    /* Function for drawing anti-aliased aliased, wide line */
    VOID                     (*gx_display_driver_anti_aliased_wide_line_draw)(GX_DRAW_CONTEXT *context,
                                                                              INT x1, INT y1, INT x2, INT y2);

    VOID                     (*gx_display_driver_horizontal_line_draw)(GX_DRAW_CONTEXT *context,
                                                                       INT x1, INT x2, INT ypos, INT width, GX_COLOR color);
    VOID                     (*gx_display_driver_horizontal_pixelmap_line_draw)(GX_DRAW_CONTEXT *context, INT xpos, INT ypos,
                                                                                INT xstart, INT xend, INT y, GX_PIXELMAP *pixelmap);
    VOID                     (*gx_display_driver_vertical_line_draw)(GX_DRAW_CONTEXT *context,
                                                                     INT y1, INT y2, INT xpos, INT width, GX_COLOR color);
    VOID                     (*gx_display_driver_horizontal_pattern_line_draw)(GX_DRAW_CONTEXT *context,
                                                                               INT x1, INT x2, INT ypos);
    VOID                     (*gx_display_driver_vertical_pattern_line_draw)(GX_DRAW_CONTEXT *context,
                                                                             INT y1, INT y2, INT xpos);
    /* Define driver function pointers for canvas composite */
    VOID                     (*gx_display_driver_canvas_copy)(struct GX_CANVAS_STRUCT *source,
                                                              struct GX_CANVAS_STRUCT *dest);
    /* Define driver function pointers for canvas composite */
    VOID                     (*gx_display_driver_canvas_blend)(struct GX_CANVAS_STRUCT *source,
                                                               struct GX_CANVAS_STRUCT *dest);

    /* Define driver function pointers for pixelmap drawing */
    VOID                     (*gx_display_driver_pixelmap_draw)(GX_DRAW_CONTEXT *context,
                                                                INT xpos, INT ypos, GX_PIXELMAP *pmp);
    VOID                     (*gx_display_driver_pixelmap_blend)(GX_DRAW_CONTEXT *context,
                                                                 INT xpos, INT ypos, GX_PIXELMAP *pmp, GX_UBYTE alpha);
    VOID                     (*gx_display_driver_alphamap_draw)(GX_DRAW_CONTEXT *context,
                                                                INT xpos, INT ypos, GX_PIXELMAP *pmp);

#if defined(GX_SOFTWARE_DECODER_SUPPORT)
    /* driver function for decode raw jpg directly to frame buffer */
    VOID                     (*gx_display_driver_jpeg_draw)(GX_DRAW_CONTEXT *context, INT x, INT y, GX_PIXELMAP *pixelmap);
    VOID                     (*gx_display_driver_png_draw)(GX_DRAW_CONTEXT *context, INT x, INT y, GX_PIXELMAP *pixelmap);

    /* driver functionn to decode raw jpg data to pixelmap */
    VOID                     (*gx_display_driver_jpeg_decode)(GX_UBYTE *read_data, ULONG read_data_size, GX_PIXELMAP *out_pixelmap);
#endif

    VOID                     (*gx_display_driver_pixelmap_rotate)(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap,
                                                                  INT angle, INT rot_cx, INT rot_cy);

    /* Define driver function pointer for low-level pixel writing.  */
    VOID                     (*gx_display_driver_pixel_write)(GX_DRAW_CONTEXT *context,
                                                              INT x, INT y, GX_COLOR color);

    /* Define driver function for block move. */
    VOID                     (*gx_display_driver_block_move)(GX_DRAW_CONTEXT *context,
                                                             GX_RECTANGLE *block, INT xshift, INT yshift);

    /* Define driver function pointer for low-level pixel blending.  */
    VOID                     (*gx_display_driver_pixel_blend)(GX_DRAW_CONTEXT *context,
                                                              INT x, INT y, GX_COLOR color, GX_UBYTE alpha);

    /* Define driver function pointer to convert 32-bit color to native format.  */
    GX_COLOR                 (*gx_display_driver_native_color_get)(struct GX_DISPLAY_STRUCT *display, GX_COLOR rawcolor);

    /* Define driver function pointer to return row pitch, in bytes, for given canvas width.  */
    USHORT                   (*gx_display_driver_row_pitch_get)(USHORT width);

    /* Define driver function pointer for low-level buffer toggle.  */
    VOID                     (*gx_display_driver_buffer_toggle)(struct GX_CANVAS_STRUCT *canvas,
                                                                GX_RECTANGLE *dirty_area);

    /* Define driver function pointer for drawing polygon.  */
    VOID                     (*gx_display_driver_polygon_draw)(GX_DRAW_CONTEXT *context, GX_POINT *vertex, INT num);

    /* Define driver function pointer for filling polygon shape.  */
    VOID                     (*gx_display_driver_polygon_fill)(GX_DRAW_CONTEXT *context, GX_POINT *vertex, INT num);

    /* Define driver function pointer for drawing aliased 8bit glyph (may be NULL).  */
    VOID                     (*gx_display_driver_8bit_glyph_draw)(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area,
                                                                  GX_POINT *map_offset, const GX_GLYPH *glyph);

    /* Define driver function pointer for drawing aliased 4bit glyph (may be NULL).  */
    VOID                     (*gx_display_driver_4bit_glyph_draw)(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area,
                                                                  GX_POINT *map_offset, const GX_GLYPH *glyph);

    /* Define driver function pointer for drawing 1bit (monochrome) glyph.  */
    VOID                     (*gx_display_driver_1bit_glyph_draw)(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area,
                                                                  GX_POINT *map_offset, const GX_GLYPH *glyph);

    VOID                     (*gx_display_driver_callback_assign)(UINT(*wait_func)(VOID *), VOID *);

#if defined(GX_ARC_DRAWING_SUPPORT)

    /* Define driver function pointer for drawing circle.  */
    VOID                     (*gx_display_driver_circle_draw)(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r);

    /* Define driver function pointer for drawing circle with wide outlines.  */
    VOID                     (*gx_display_driver_wide_circle_draw)(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r);

    /* Define driver function pointer for filling circle shape.  */
    VOID                     (*gx_display_driver_circle_fill)(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r);

    /* Define driver function pointer for drawing anti-aliased circle.  */
    VOID                     (*gx_display_driver_anti_aliased_circle_draw)(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r);

    /* Define driver function pointer for drawing anti-aliased circle with wide outlines.  */
    VOID                     (*gx_display_driver_anti_aliased_wide_circle_draw)(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r);

    /* Define driver function pointer for drawing circle arc. */
    VOID                     (*gx_display_driver_arc_draw)(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r, INT start_angle, INT end_angle);

    /* Define driver function pointer for drawing circle arc with wide outlines. */
    VOID                     (*gx_display_driver_wide_arc_draw)(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r, INT start_angle, INT end_angle);

    /* Define driver function pointer for drawing anti-aliased circle arc.  */
    VOID                     (*gx_display_driver_anti_aliased_arc_draw)(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r, INT start_angle, INT end_angle);

    /* Define driver function pointer for drawing anti-aliased circle arc with wide outlines. */
    VOID                     (*gx_display_driver_anti_aliased_wide_arc_draw)(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r, INT start_angle, INT end_angle);

    /* Define driver function pointer for filling circle arc shape.  */
    VOID                     (*gx_display_driver_arc_fill)(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r, INT start_angle, INT end_angle);

    /* Define driver function pointer for filling pie shape.  */
    VOID                     (*gx_display_driver_pie_fill)(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r, INT start_angle, INT end_angle);

    /* Define driver function pointer for drawing anti-aliased ellipse.  */
    VOID                     (*gx_display_driver_anti_aliased_ellipse_draw)(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, INT a, INT b);

    /* Define driver function pointer for drawing anti-aliased ellipse with wide outlines.  */
    VOID                     (*gx_display_driver_anti_aliased_wide_ellipse_draw)(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, INT a, INT b);

    /* Define driver function pointer for drawing ellipse.  */
    VOID                     (*gx_display_driver_ellipse_draw)(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, INT a, INT b);

    /* Define driver function pointer for drawing ellipse with wide outlines.  */
    VOID                     (*gx_display_driver_wide_ellipse_draw)(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, INT a, INT b);

    /* Define driver function pointer for drawing a filled ellipse.  */
    VOID                     (*gx_display_driver_ellipse_fill)(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, INT a, INT b);
#endif
} GX_DISPLAY;


/* Define Canvas type.  */

typedef struct GX_CANVAS_STRUCT
{
    /* Define the canvas ID and name. The canvas ID is used simply to see if the
       control block is valid.  */
    ULONG             gx_canvas_id;                                /* Control block ID                         */
    GX_CONST GX_CHAR *gx_canvas_name;                              /* Pointer to canvas name                   */

    /* Define pointers to the next and previous canvases in the created list.  */
    struct GX_CANVAS_STRUCT *gx_canvas_created_next,
                            *gx_canvas_created_previous;

    /* Define the canvas memory pointer.  */
    GX_COLOR *gx_canvas_memory;

    /* Define the size of the canvas memory.  */
    ULONG gx_canvas_memory_size;

    /* Which display driver is used to paint on this canvas.  */
    GX_DISPLAY *gx_canvas_display;

    /* Prior to drawing on a canvas, we must know what portion needs to be updated */
    GX_RECTANGLE  gx_canvas_dirty_area;
    GX_DIRTY_AREA gx_canvas_dirty_list[GX_MAX_DIRTY_AREAS];
    UINT          gx_canvas_dirty_count;
    UINT          gx_canvas_draw_count;
    UINT          gx_canvas_status;

    GX_VALUE      gx_canvas_x_resolution;
    GX_VALUE      gx_canvas_y_resolution;
    GX_VALUE      gx_canvas_display_offset_x;
    GX_VALUE      gx_canvas_display_offset_y;

    GX_UBYTE      gx_canvas_alpha;
    GX_UBYTE      gx_canvas_draw_nesting;
} GX_CANVAS;

/* Define Theme type */
typedef struct GX_THEME_STRUCT
{
    GX_COLOR               *theme_color_table;
    GX_FONT               **theme_font_table;
    GX_PIXELMAP           **theme_pixelmap_table;
    GX_COLOR               *theme_palette;
    GX_SCROLLBAR_APPEARANCE theme_vertical_scrollbar_appearance;
    GX_SCROLLBAR_APPEARANCE theme_horizontal_scrollbar_appearance;
    ULONG                   theme_vertical_scroll_style;
    ULONG                   theme_horizontal_scroll_style;

    USHORT                  theme_color_table_size;
    USHORT                  theme_font_table_size;
    USHORT                  theme_pixelmap_table_size;
    USHORT                  theme_palette_size;
} GX_THEME;

/* Define text input cursor control block */
typedef struct GX_TEXT_INPUT_CURSOR_STRUCT
{
    GX_VALUE gx_text_input_cursor_blink_interval;

    /* User defined cursor width and height */
    GX_VALUE gx_text_input_cursor_width;
    GX_VALUE gx_text_input_cursor_height;

    /* Cursor position information, managed by the cursor control code.*/
    GX_POINT gx_text_input_cursor_pos;

    /* Optional memory provided by user, in order to cache the area covered
       by the cursor.  When we need to restore the backgournd (when cursor
       moves away), we only need to copy the memory back, instead of
       redrawing the content.*/
    VOID *gx_text_input_cursor_background_memory;
    UINT  gx_text_input_cursor_background_memory_size;
} GX_TEXT_INPUT_CURSOR;


/* Define macro for GX_WIDGET members.  */

#if defined(GX_WIDGET_USER_DATA)
#define GX_USER_DATA_FIELD INT gx_widget_user_data;
#else
#define GX_USER_DATA_FIELD
#endif

#define GX_WIDGET_MEMBERS_DECLARE                                                                            \
    GX_CONST GX_CHAR        *gx_widget_name;                                                                 \
    struct GX_WIDGET_STRUCT *gx_widget_parent;                                                               \
    struct GX_WIDGET_STRUCT *gx_widget_first_child;                                                          \
    struct GX_WIDGET_STRUCT *gx_widget_last_child;                                                           \
    struct GX_WIDGET_STRUCT *gx_widget_next;                                                                 \
    struct GX_WIDGET_STRUCT *gx_widget_previous;                                                             \
    struct GX_WIDGET_STRUCT *gx_widget_nav_next;                                                             \
    struct GX_WIDGET_STRUCT *gx_widget_nav_previous;                                                         \
    GX_RECTANGLE             gx_widget_size;                                                                 \
    GX_RECTANGLE             gx_widget_clip;                                                                 \
    UINT                     (*gx_widget_event_process_function)(struct GX_WIDGET_STRUCT *,  GX_EVENT *);    \
    VOID                     (*gx_widget_draw_function)(struct GX_WIDGET_STRUCT *);                          \
    ULONG          gx_widget_status;                                                                         \
    ULONG          gx_widget_style;                                                                          \
    GX_RESOURCE_ID gx_widget_normal_fill_color;                                                              \
    GX_RESOURCE_ID gx_widget_selected_fill_color;                                                            \
    USHORT         gx_widget_type;                                                                           \
    USHORT         gx_widget_id;                                                                             \
    GX_USER_DATA_FIELD                                                                                       


/* Define macro for GX_PROMPT, based on GX_WIDGET.  */

#define GX_PROMPT_MEMBERS_DECLARE                                                                     \
    GX_WIDGET_MEMBERS_DECLARE                                                                         \
    GX_CHAR                     * gx_prompt_string;                                                   \
    VOID                       (*gx_prompt_text_get_function)(struct GX_PROMPT_STRUCT *, GX_CHAR **); \
    GX_RESOURCE_ID gx_prompt_text_id;                                                                 \
    GX_RESOURCE_ID gx_prompt_font_id;                                                                 \
    GX_RESOURCE_ID gx_prompt_normal_text_color;                                                       \
    GX_RESOURCE_ID gx_prompt_selected_text_color;


/* Define macro for PIXELMAP_PROMPT, based on GX_PROMPT.   */

#define GX_PIXELMAP_PROMPT_MEMBERS_DECLARE                  \
    GX_PROMPT_MEMBERS_DECLARE                               \
    GX_RESOURCE_ID              gx_normal_left_pixelmap_id; \
    GX_RESOURCE_ID gx_normal_right_pixelmap_id;             \
    GX_RESOURCE_ID gx_normal_fill_pixelmap_id;              \
    GX_RESOURCE_ID gx_selected_left_pixelmap_id;            \
    GX_RESOURCE_ID gx_selected_right_pixelmap_id;           \
    GX_RESOURCE_ID gx_selected_fill_pixelmap_id;


/* Define macro for GX_SINGLE_LINE_TEXT_INPUT, based on GX_PROMPT.  */

#define GX_SINGLE_LINE_TEXT_INPUT_MEMBERS_DECLARE                   \
    GX_PROMPT_MEMBERS_DECLARE                                       \
    GX_CHAR                     * gx_single_line_text_input_buffer; \
    UINT                 gx_single_line_text_input_buffer_size;     \
    UINT                 gx_single_line_text_input_string_size;     \
    GX_VALUE             gx_single_line_text_input_xoffset;         \
    GX_VALUE             gx_single_line_text_input_yoffset;         \
    UINT                 gx_single_line_text_input_insert_pos;      \
    UINT                 gx_single_line_text_input_was_modified;    \
    GX_TEXT_INPUT_CURSOR gx_single_line_text_input_cursor_instance;


/* Define macro for GX_ICON, based on GX_WIDGET.  */

#define GX_ICON_MEMBERS_DECLARE                          \
    GX_WIDGET_MEMBERS_DECLARE                            \
    GX_RESOURCE_ID              gx_icon_normal_pixelmap; \
    GX_RESOURCE_ID gx_icon_selected_pixelmap;

/* Define macro for GX_SPRITE, based on GX_WIDGET. */

#define GX_SPRITE_MEMBERS_DECLARE           \
    GX_WIDGET_MEMBERS_DECLARE               \
    GX_SPRITE_FRAME * gx_sprite_frame_list; \
    USHORT gx_sprite_frame_count;           \
    USHORT gx_sprite_current_frame;         \
    UCHAR  gx_sprite_run_state;


/* Define macro for GX_BUTTON, based on GX_WIDGET.  */

#define GX_BUTTON_MEMBERS_DECLARE                                               \
    GX_WIDGET_MEMBERS_DECLARE                                                   \
    VOID                        (*gx_button_select_handler)(GX_WIDGET *button); \
    VOID                        (*gx_button_deselect_handler)(GX_WIDGET *button);


/* Define macro for GX_TEXT_BUTTON, based on GX_BUTTON.  */

#define GX_TEXT_BUTTON_MEMBERS_DECLARE                  \
    GX_BUTTON_MEMBERS_DECLARE                           \
    GX_RESOURCE_ID              gx_text_button_text_id; \
    GX_RESOURCE_ID gx_text_button_font_id;              \
    GX_CHAR       *gx_text_button_string;               \
    GX_RESOURCE_ID gx_text_button_normal_text_color;    \
    GX_RESOURCE_ID gx_text_button_selected_text_color;

/* Define macro for GX_MULTI_LINE_TEXT_BUTTON, based on GX_TEXT_BUTTON.  */

#define GX_MULTI_LINE_TEXT_BUTTON_MEMBERS_DECLARE                                   \
    GX_TEXT_BUTTON_MEMBERS_DECLARE                                                  \
    INT                         gx_multi_line_text_button_line_count;               \
    INT      gx_multi_line_text_button_length[GX_MULTI_LINE_TEXT_BUTTON_MAX_LINES]; \
    GX_CHAR *gx_multi_line_text_button_lines[GX_MULTI_LINE_TEXT_BUTTON_MAX_LINES];

/* Define macro for GX_RADIO_BUTTON, based on GX_TEXT_BUTTON.  */

#define GX_RADIO_BUTTON_MEMBERS_DECLARE                          \
    GX_TEXT_BUTTON_MEMBERS_DECLARE                               \
    GX_RESOURCE_ID              gx_radio_button_off_pixelmap_id; \
    GX_RESOURCE_ID gx_radio_button_on_pixelmap_id;               \
    GX_RESOURCE_ID gx_radio_button_off_disabled_pixelmap_id;     \
    GX_RESOURCE_ID gx_radio_button_on_disabled_pixelmap_id;


/* Define macro for GX_CHECKBOX, based on GX_TEXT_BUTTON.  */

#define GX_CHECKBOX_MEMBERS_DECLARE                                \
    GX_TEXT_BUTTON_MEMBERS_DECLARE                                 \
    GX_RESOURCE_ID              gx_checkbox_unchecked_pixelmap_id; \
    GX_RESOURCE_ID gx_checkbox_checked_pixelmap_id;                \
    GX_RESOURCE_ID gx_checkbox_unchecked_disabled_pixelmap_id;     \
    GX_RESOURCE_ID gx_checkbox_checked_disabled_pixelmap_id;

/* Define macro for GX_ICON_BUTTON, based ON GX_BUTTON.  */

#define GX_ICON_BUTTON_MEMBERS_DECLARE \
    GX_BUTTON_MEMBERS_DECLARE          \
    GX_RESOURCE_ID              gx_icon_button_icon_id;

/* Define macro for GX_PIXELMAP_BUTTON, based ON GX_BUTTON.  */

#define GX_PIXELMAP_BUTTON_MEMBERS_DECLARE         \
    GX_BUTTON_MEMBERS_DECLARE                      \
    GX_RESOURCE_ID gx_pixelmap_button_normal_id;   \
    GX_RESOURCE_ID gx_pixelmap_button_selected_id; \
    GX_RESOURCE_ID gx_pixelmap_button_disabled_id;


/* Define macro for GX_WINDOW, based on GX_WIDGET.  */

#define GX_WINDOW_MEMBERS_DECLARE                                                                                   \
    GX_WIDGET_MEMBERS_DECLARE                                                                                       \
    GX_RECTANGLE   gx_window_client;                                                                                \
    GX_VIEW       *gx_window_views;                                                                                 \
    VOID          (*gx_window_scroll_info_get)(struct GX_WINDOW_STRUCT *window, ULONG style, GX_SCROLL_INFO *info); \
    GX_BOOL        gx_window_move_mode;                                                                             \
    GX_POINT       gx_window_move_start;                                                                            \
    GX_RESOURCE_ID gx_window_wallpaper;

/* Define macro for GX_WINDOW_ROOT, based on GX_WINDOW.   */

#define GX_WINDOW_ROOT_MEMBERS_DECLARE                   \
    GX_WINDOW_MEMBERS_DECLARE                            \
    GX_CANVAS                   *gx_window_root_canvas;  \
    GX_BOOL gx_window_root_views_changed;

/* Define macro for GX_SCROLL_THUMB, based on GX_BUTTON.  */
#define GX_SCROLL_THUMB_MEMBERS_DECLARE                       \
    GX_BUTTON_MEMBERS_DECLARE                                 \
    GX_RESOURCE_ID              gx_scroll_thumb_pixelmap;     \
    GX_RESOURCE_ID              gx_scroll_thumb_border_color; \
    GX_BOOL gx_scroll_thumb_drag_mode;                        \
    INT     gx_scroll_thumb_start_drag;


/* Define macro for GX_SCROLLBAR, based on GX_WIDGET.  */

#define GX_SCROLLBAR_MEMBERS_DECLARE                     \
    GX_WIDGET_MEMBERS_DECLARE                            \
    GX_SCROLLBAR_APPEARANCE     gx_scrollbar_appearance; \
    GX_SCROLL_INFO  gx_scrollbar_info;                   \
    GX_BUTTON       gx_scrollbar_upleft;                 \
    GX_BUTTON       gx_scrollbar_downright;              \
    GX_SCROLL_THUMB gx_scrollbar_thumb;


/* Define macro for GX_SLIDER, based on GX_WIDGET.  */

#define GX_SLIDER_MEMBERS_DECLARE               \
    GX_WIDGET_MEMBERS_DECLARE                   \
    GX_SLIDER_INFO              gx_slider_info; \
    INT            gx_slider_tick_count;        \
    GX_RESOURCE_ID gx_slider_tick_color;


/* Define macro for GX_PIXELMAP_SLIDER, based on GX_WIDGET.  */
#define GX_PIXELMAP_SLIDER_MEMBERS_DECLARE               \
    GX_WIDGET_MEMBERS_DECLARE                            \
    GX_SLIDER_INFO              gx_pixelmap_slider_info; \
    GX_PIXELMAP_SLIDER_INFO gx_pixelmap_slider_pixelmap_info;


/* Define macro for GX_VERTICAL_LIST, based on GX_WINDOW.  */
#define GX_VERTICAL_LIST_MEMBERS_DECLARE                                                                                      \
    GX_WINDOW_MEMBERS_DECLARE                                                                                                 \
    VOID                        (*gx_vertical_list_callback)(struct GX_VERTICAL_LIST_STRUCT *, GX_WIDGET *widget, INT index); \
    INT      gx_vertical_list_selected;                                                                                       \
    INT      gx_vertical_list_pen_index;                                                                                      \
    INT      gx_vertical_list_top_index;                                                                                      \
    INT      gx_vertical_list_total_rows;                                                                                     \
    GX_VALUE gx_vertical_list_seperation;                                                                                     \
    GX_VALUE gx_vertical_list_visible_rows;                                                                                   \
    GX_VALUE gx_vertical_list_child_count;                                                                                    \
    GX_VALUE gx_vertical_list_child_height;                                                                                   \
    GX_VALUE gx_vertical_list_snap_back_distance;

/* Define macro for GX_VERTICAL_LIST, based on GX_WINDOW.  */
#define GX_HORIZONTAL_LIST_MEMBERS_DECLARE                                                                                        \
    GX_WINDOW_MEMBERS_DECLARE                                                                                                     \
    VOID                        (*gx_horizontal_list_callback)(struct GX_HORIZONTAL_LIST_STRUCT *, GX_WIDGET *widget, INT index); \
    INT      gx_horizontal_list_selected;                                                                                         \
    INT      gx_horizontal_list_pen_index;                                                                                        \
    INT      gx_horizontal_list_top_index;                                                                                       \
    INT      gx_horizontal_list_total_columns;                                                                                    \
    GX_VALUE gx_horizontal_list_seperation;                                                                                       \
    GX_VALUE gx_horizontal_list_visible_columns;                                                                                  \
    GX_VALUE gx_horizontal_list_child_count;                                                                                      \
    GX_VALUE gx_horizontal_list_child_width;                                                                                      \
    GX_VALUE gx_horizontal_list_snap_back_distance;

#define GX_POPUP_LIST_MEMBERS_DECLARE               \
    GX_VERTICAL_LIST            gx_popup_list_list; \
    GX_WIDGET *gx_popup_list_owner;

/* Define macro for GX_DROP_LIST, based on GX_WIDGET  */
#define GX_DROP_LIST_MEMBERS_DECLARE                \
    GX_WIDGET_MEMBERS_DECLARE                       \
    GX_POPUP_LIST               gx_drop_list_popup; \
    GX_ICON_BUTTON gx_drop_list_button;             \
    INT            gx_drop_list_open_height;        \
    GX_RESOURCE_ID gx_drop_list_pixelmap;           \
    GX_BOOL        gx_drop_list_popup_open;


/* Define macro for GX_MULTI_LINE_TEXT_VIEW, based on GX_WINDOW.  */

#define GX_MULTI_LINE_TEXT_VIEW_MEMBERS_DECLARE                  \
    GX_WINDOW_MEMBERS_DECLARE                                    \
    GX_RESOURCE_ID gx_multi_line_text_view_text_id;              \
    GX_RESOURCE_ID gx_multi_line_text_view_font_id;              \
    GX_RESOURCE_ID gx_multi_line_text_view_normal_text_color;    \
    GX_RESOURCE_ID gx_multi_line_text_view_selected_text_color;  \
    UINT           gx_multi_line_text_view_string_byte_count;    \
    UINT           gx_multi_line_text_view_string_char_count;    \
    UINT           gx_multi_line_text_view_text_total_rows;      \
    UINT           gx_multi_line_text_view_text_visible_rows;    \
    INT            gx_multi_line_text_view_text_scroll_shift;    \
    GX_CHAR       *gx_multi_line_text_view_text;                 \
    GX_FONT       *gx_multi_line_text_view_font;                 \
    GX_BOOL        gx_multi_line_text_view_line_index_old;       \
    GX_VALUE       gx_multi_line_text_view_line_index[GX_MULTI_LINE_INDEX_CACHE_SIZE];

/* GX_MULTI_LINE_TEXT_INPUT, based on GX_MULTI_LINE_TEXT_VIEW */
#define GX_MULTI_LINE_TEXT_INPUT_MEMBERS_DECLARE                        \
    GX_MULTI_LINE_TEXT_VIEW_MEMBERS_DECLARE                             \
    UINT                 gx_multi_line_text_input_buffer_size;          \
    UINT                 gx_multi_line_text_input_text_insert_position; \
    UINT                 gx_multi_line_text_input_text_was_modified;    \
    UINT                 gx_multi_line_text_input_text_cursor_line;     \
    GX_TEXT_INPUT_CURSOR gx_multi_line_text_input_cursor_instance;

/* Define macro for GX_PROGRESS_BAR, based on GX_WIDGET.  */
#define GX_PROGRESS_BAR_MEMBERS_DECLARE \
    GX_WIDGET_MEMBERS_DECLARE           \
    GX_PROGRESS_BAR_INFO       gx_progress_bar_info;

/* Define macro for GX_CIRCULAR_GAUGE members. */
#define GX_CIRCULAR_GAUGE_MEMBERS_DECLARE                              \
    GX_ICON_MEMBERS_DECLARE                                            \
    GX_CIRCULAR_GAUGE_INFO gx_circular_gauge_info;                     \
    GX_PIXELMAP            gx_circular_gauge_needle_rotated;           \
    GX_PIXELMAP            *gx_circular_gauge_needle_source;           \
    INT                    gx_circular_gauge_current_needle_x;         \
    INT                    gx_circular_gauge_current_needle_y;         \
    INT                    gx_circular_gauge_start_angle;              \
    INT                    gx_circular_gauge_current_angle;            \
    INT                    gx_circular_gauge_target_angle;             \
    INT                    gx_circular_gauge_angle_increment;          \
    INT                    gx_circular_gauge_animation_step;           \
    GX_RECTANGLE           gx_circular_gauge_current_needle_rectangle;

/* Define macro for GX_RADIAL_PROGRESS_BAR members. */
#define GX_RADIAL_PROGRESS_BAR_MEMBERS_DECLARE                            \
    GX_WIDGET_MEMBERS_DECLARE                                             \
    GX_RADIAL_PROGRESS_BAR_INFO      gx_radial_progress_bar_info;         \

/* Define macro for GX_LINE_CHART members. */
#define GX_LINE_CHART_MEMBERS_DECLARE               \
    GX_WINDOW_MEMBERS_DECLARE						\
    GX_LINE_CHART_INFO gx_line_chart_info;


/* Define Widget type.  */

typedef struct GX_WIDGET_STRUCT
{
    GX_WIDGET_MEMBERS_DECLARE
} GX_WIDGET;


/* Define Prompt type.  */

typedef struct GX_PROMPT_STRUCT
{
    GX_PROMPT_MEMBERS_DECLARE
} GX_PROMPT;


/* Define Pixelmap type.  */

typedef struct GX_PIXELMAP_PROMPT_STRUCT
{
    GX_PIXELMAP_PROMPT_MEMBERS_DECLARE
} GX_PIXELMAP_PROMPT;


/* Define Icon type.  */

typedef struct GX_ICON_STRUCT
{
    GX_ICON_MEMBERS_DECLARE
} GX_ICON;

/* Define Sprite type. */

typedef struct GX_SPRITE_STRUCT
{
    GX_SPRITE_MEMBERS_DECLARE
} GX_SPRITE;


/* Define Button type.  */

typedef struct GX_BUTTON_STRUCT
{
    GX_BUTTON_MEMBERS_DECLARE
} GX_BUTTON;


/* Define Text Button type.  */

typedef struct GX_TEXT_BUTTON_STRUCT
{
    GX_TEXT_BUTTON_MEMBERS_DECLARE
} GX_TEXT_BUTTON;


/* Define Multi-line Text Button type.  */

typedef struct GX_MULTI_LINE_TEXT_BUTTON_STRUCT
{
    GX_MULTI_LINE_TEXT_BUTTON_MEMBERS_DECLARE
} GX_MULTI_LINE_TEXT_BUTTON;


/* Define Radio Button type.  */

typedef struct GX_RADIO_BUTTON_STRUCT
{
    GX_RADIO_BUTTON_MEMBERS_DECLARE
} GX_RADIO_BUTTON;


/* Define Checkbox type.  */

typedef struct GX_CHECKBOX_STRUCT
{
    GX_CHECKBOX_MEMBERS_DECLARE
} GX_CHECKBOX;


/* Defing Icon Button type.  */

typedef struct GX_ICON_BUTTON_STRUCT
{
    GX_ICON_BUTTON_MEMBERS_DECLARE
} GX_ICON_BUTTON;


/* Define Pixelmap Button type.  */

typedef struct GX_PIXELMAP_BUTTON_STRUCT
{
    GX_PIXELMAP_BUTTON_MEMBERS_DECLARE
} GX_PIXELMAP_BUTTON;


/* Define Scroll Thumb type.  */

typedef struct GX_SCROLL_THUMB_STRUCT
{
    GX_SCROLL_THUMB_MEMBERS_DECLARE
} GX_SCROLL_THUMB;


/* Define Scrollbar type.  */

typedef struct GX_SCROLLBAR_STRUCT
{
    GX_SCROLLBAR_MEMBERS_DECLARE
} GX_SCROLLBAR;


/* Define Slider type.  */

typedef struct GX_SLIDER_STRUCT
{
    GX_SLIDER_MEMBERS_DECLARE
} GX_SLIDER;


/* Define Pixelmap Slider type.  */

typedef struct GX_PIXELMAP_SLIDER_STRUCT
{
    GX_PIXELMAP_SLIDER_MEMBERS_DECLARE
} GX_PIXELMAP_SLIDER;


/* Define Window type.  */

typedef struct GX_WINDOW_STRUCT
{
    GX_WINDOW_MEMBERS_DECLARE
} GX_WINDOW;

/* Define Root Window type.  */

typedef struct GX_WINDOW_ROOT_STRUCT
{
    GX_WINDOW_ROOT_MEMBERS_DECLARE
} GX_WINDOW_ROOT;

/*Define Single Line Text Input type.  */

typedef struct GX_SINGLE_LINE_TEXT_INPUT_STRUCT
{
    GX_SINGLE_LINE_TEXT_INPUT_MEMBERS_DECLARE
} GX_SINGLE_LINE_TEXT_INPUT;


/* Define Multi Line Text View type.  */

typedef struct GX_MULTI_LINE_TEXT_VIEW_STRUCT
{
    GX_MULTI_LINE_TEXT_VIEW_MEMBERS_DECLARE
} GX_MULTI_LINE_TEXT_VIEW;

/*Define GX_MULTI_LINE_TEXT_INFO type. */
typedef struct GX_MULTI_LINE_TEXT_INFO_STRUCT
{
    GX_VALUE gx_text_display_width;
    UINT     gx_text_display_number;
} GX_MULTI_LINE_TEXT_INFO;

/*Define GX_MULTI_TEXT_INPUT type. */

typedef struct GX_MULTI_LINE_TEXT_INPUT_STRUCT
{
    GX_MULTI_LINE_TEXT_INPUT_MEMBERS_DECLARE
} GX_MULTI_LINE_TEXT_INPUT;

typedef struct GX_VERTICAL_LIST_STRUCT
{
    GX_VERTICAL_LIST_MEMBERS_DECLARE
} GX_VERTICAL_LIST;

typedef struct GX_HORIZONTAL_LIST_STRUCT
{
    GX_HORIZONTAL_LIST_MEMBERS_DECLARE
} GX_HORIZONTAL_LIST;

typedef struct GX_POPUP_LIST_STRUCT
{
    GX_POPUP_LIST_MEMBERS_DECLARE
} GX_POPUP_LIST;

typedef struct GX_DROP_LIST_STRUCT
{
    GX_DROP_LIST_MEMBERS_DECLARE
} GX_DROP_LIST;

/* Define GX_PROGRESS_BAR type.  */

typedef struct GX_PROGRESS_BAR_STRUCT
{
    GX_PROGRESS_BAR_MEMBERS_DECLARE
} GX_PROGRESS_BAR;

/* Circular gauge control block */
typedef struct GX_CIRCULAR_GAUGE_STRUCT
{
    GX_CIRCULAR_GAUGE_MEMBERS_DECLARE
} GX_CIRCULAR_GAUGE;

/* Define Radial Progress Bar type */
typedef struct GX_RADIAL_PROGRESS_BAR_STRUCT
{
    GX_RADIAL_PROGRESS_BAR_MEMBERS_DECLARE
} GX_RADIAL_PROGRESS_BAR;

/* Define GX_LINE_CHART control block type */
typedef struct GX_LINE_CHART_STRUCT
{
	GX_LINE_CHART_MEMBERS_DECLARE
} GX_LINE_CHART;


/* Animation information structure. This structure is used to define the
   parameters of a screen transition animation.  */

typedef struct GX_ANIMATION_INFO_STRUCT
{
    GX_WIDGET *gx_animation_target;
    GX_WIDGET *gx_animation_parent;
    GX_UBYTE   gx_animation_type;
    GX_UBYTE   gx_animation_start_alpha;
    GX_UBYTE   gx_animation_end_alpha;
    GX_UBYTE   gx_animation_steps;
    GX_POINT   gx_animation_start_position;
    GX_POINT   gx_animation_end_position;
    USHORT     gx_animation_delay;
} GX_ANIMATION_INFO;


/* Animation control block */
typedef struct GX_ANIMATION_STRUCT
{
    GX_ANIMATION_INFO           gx_animation_info;
    GX_CANVAS                   gx_animation_canvas;
    GX_WINDOW_ROOT              gx_animation_root;
    GX_DISPLAY                 *gx_animation_display;
    struct GX_ANIMATION_STRUCT *gx_animation_next;
    USHORT                      gx_animation_status;
    USHORT                      gx_animation_timer;
} GX_ANIMATION;

typedef struct GX_PIXEL_STRUCT
{
    GX_UBYTE                   gx_pixel_red;
    GX_UBYTE                   gx_pixel_green;
    GX_UBYTE                   gx_pixel_blue;
    GX_UBYTE                   gx_pixel_alpha;
} GX_PIXEL;

/* Image reader control block */
typedef struct GX_IMAGE_READER_STRUCT
{
    UINT                      (*gx_image_reader_pixel_write)(struct GX_IMAGE_READER_STRUCT *image_reader, GX_PIXEL *pixel);
    UINT                      (*gx_image_reader_pixel_read)(struct GX_IMAGE_READER_STRUCT *image_reader, INT index, GX_PIXEL *pixel);
    const GX_UBYTE             *gx_image_reader_source_data;
    UINT                        gx_image_reader_source_data_size;
    GX_UBYTE                   *gx_image_reader_getdata;
    GX_UBYTE                   *gx_image_reader_getauxdata;
    GX_UBYTE                   *gx_image_reader_putdata;
    GX_UBYTE                   *gx_image_reader_putauxdata;
    GX_UBYTE                    gx_image_reader_color_format;
    GX_UBYTE                    gx_image_reader_mode;
    GX_COLOR                   *gx_image_reader_palette;
    UINT                        gx_image_reader_palette_size;
    GX_UBYTE                    gx_image_reader_image_type;
    UINT                        gx_image_reader_input_stride;
    GX_BOOL                     gx_image_reader_size_testing;
    GX_COLOR                   *gx_image_reader_png_trans;
    GX_COLOR                   *gx_image_reader_png_palette;
    UINT                        gx_image_reader_png_palette_size;
    UINT                        gx_image_reader_image_width;
    UINT                        gx_image_reader_image_height;
} GX_IMAGE_READER;

#define GX_IMAGE_READER_MODE_NONE       0x00
#define GX_IMAGE_READER_MODE_COMPRESS   0x01
#define GX_IMAGE_READER_MODE_ALPHA      0x02
#define GX_IMAGE_READER_MODE_DITHER     0x04

/* Define Screen stack control block */
typedef struct GX_SCREEN_STACK_CONTROL_STRUCT
{
    GX_WIDGET                 **gx_screen_stack_control_memory;
    INT                         gx_screen_stack_control_top;
    INT                         gx_screen_stack_control_max;
} GX_SCREEN_STACK_CONTROL;

/* GUIX fixed point math helpers */

typedef signed long GX_FIXED_VAL;

typedef struct GX_FIXED_POINT_STRUCT
{
    GX_FIXED_VAL x;
    GX_FIXED_VAL y;
} GX_FIXED_POINT;

#define GX_FIXED_VAL_SHIFT         10
#define GX_FIXED_VAL_HALF          512
#define GX_FIXED_VAL_ONE           1024
#define GX_FIXED_VAL_FRACTION_MASK 0x3ff

#define GX_FIXED_VAL_MAKE(_a)      (((int)_a) << GX_FIXED_VAL_SHIFT)
#define GX_FIXED_VAL_TO_INT(_a)    ((int)(((int)_a) >> GX_FIXED_VAL_SHIFT))
#define GX_FIXED_VAL_MUL(_a, _b)   ((((int)_a) * (_b)) >> GX_FIXED_VAL_SHIFT)
#define GX_FIXED_VAL_DIV(_a, _b)   ((((int)_a) << GX_FIXED_VAL_SHIFT) / ((int)_b))
#define GX_FIXED_VAL_RND(_a)       (GX_VALUE)((((GX_FIXED_VAL)_a) + GX_FIXED_VAL_HALF) >> GX_FIXED_VAL_SHIFT)
#define GX_FIXED_VAL_RND_UP(_a)    ((((GX_FIXED_VAL)_a) + GX_FIXED_VAL_ONE - 1) >> GX_FIXED_VAL_SHIFT)
#define GX_FIXED_VAL_ADD_ONE(_a)   (((int)_a) + GX_FIXED_VAL_ONE)
#define GX_FIXED_VAL_SUB_ONE(_a)   (((int)_a) - GX_FIXED_VAL_ONE)

/* Define the system API mappings based on the error checking
   selected by the user.  Note: this section is only applicable to
   application source code, hence the conditional that turns off this
   stuff when the include file is processed by the GUIX source. */

#ifndef GX_SOURCE_CODE


/* Determine if error checking is desired.  If so, map API functions
   to the appropriate error checking front-ends.  Otherwise, map API
   functions to the core functions that actually perform the work.
   Note: error checking is enabled by default.  */

#ifdef GX_DISABLE_ERROR_CHECKING


/* Services without error checking.  */

#define gx_animation_canvas_define                               _gx_animation_canvas_define
#define gx_animation_create                                      _gx_animation_create
#define gx_animatin_delete                                       _gx_animation_delete
#define gx_animation_start                                       _gx_animation_start
#define gx_animation_stop                                        _gx_animation_stop

#define gx_brush_default                                         _gx_brush_default
#define gx_brush_define                                          _gx_brush_define

#define gx_button_background_draw                                _gx_button_background_draw
#define gx_button_create(a, b, c, d, e, f)                       _gx_button_create(a, b, (GX_WIDGET *)c, d, e, f)
#define gx_button_deselect                                       _gx_button_deselect
#define gx_button_draw                                           _gx_button_draw
#define gx_button_event_process                                  _gx_button_event_process
#define gx_button_select                                         _gx_button_select

#define gx_canvas_alpha_set                                      _gx_canvas_alpha_set
#define gx_canvas_block_move(a, b, c, d)                         _gx_canvas_block_move(a, b, c, d)
#define gx_canvas_create                                         _gx_canvas_create
#define gx_canvas_delete                                         _gx_canvas_delete
#define gx_canvas_drawing_complete                               _gx_canvas_drawing_complete
#define gx_canvas_drawing_initiate(a, b, c)                      _gx_canvas_drawing_initiate(a, (GX_WIDGET *)b, c)
#define gx_canvas_line_draw                                      _gx_canvas_line_draw
#define gx_canvas_offset_set                                     _gx_canvas_offset_set
#define gx_canvas_pixel_draw                                     _gx_canvas_pixel_draw
#define gx_canvas_pixelmap_blend                                 _gx_canvas_pixelmap_blend
#define gx_canvas_pixelmap_draw                                  _gx_canvas_pixelmap_draw
#define gx_canvas_pixelmap_tile                                  _gx_canvas_pixelmap_tile
#define gx_canvas_pixelmap_rotate                                _gx_canvas_pixelmap_rotate
#define gx_canvas_polygon_draw                                   _gx_canvas_polygon_draw
#define gx_canvas_circle_draw                                    _gx_canvas_circle_draw
#define gx_canvas_arc_draw                                       _gx_canvas_arc_draw
#define gx_canvas_pie_draw                                       _gx_canvas_pie_draw
#define gx_canvas_ellipse_draw                                   _gx_canvas_ellipse_draw
#define gx_canvas_rectangle_draw                                 _gx_canvas_rectangle_draw
#define gx_canvas_shift                                          _gx_canvas_shift
#define gx_canvas_text_draw                                      _gx_canvas_text_draw

#define gx_checkbox_create(a, b, c, d, e, f, g)                  _gx_checkbox_create(a, b, (GX_WIDGET *)c, d, e, f, g)
#define gx_checkbox_draw                                         _gx_checkbox_draw
#define gx_checkbox_event_process                                _gx_checkbox_event_process
#define gx_checkbox_pixelmap_set                                 _gx_checkbox_pixelmap_set
#define gx_checkbox_select                                       _gx_checkbox_select

#define gx_context_brush_default                                 _gx_context_brush_default
#define gx_context_brush_define                                  _gx_context_brush_define
#define gx_context_brush_get                                     _gx_context_brush_get
#define gx_context_brush_pattern_set                             _gx_context_brush_pattern_set
#define gx_context_brush_set                                     _gx_context_brush_set
#define gx_context_brush_style_set                               _gx_context_brush_style_set
#define gx_context_brush_width_set                               _gx_context_brush_width_set
#define gx_context_color_get                                     _gx_context_color_get
#define gx_context_fill_color_set                                _gx_context_fill_color_set
#define gx_context_font_get                                      _gx_context_font_get
#define gx_context_font_set                                      _gx_context_font_set
#define gx_context_line_color_set                                _gx_context_line_color_set
#define gx_context_pixelmap_get                                  _gx_context_pixelmap_get
#define gx_context_pixelmap_set                                  _gx_context_pixelmap_set
#define gx_context_raw_brush_define                              _gx_context_raw_brush_define
#define gx_context_raw_fill_color_set                            _gx_context_raw_fill_color_set
#define gx_context_raw_line_color_set                            _gx_context_raw_line_color_set

#define gx_display_create                                        _gx_display_create
#define gx_display_delete                                        _gx_display_delete
#define gx_display_color_table_set                               _gx_display_color_table_set
#define gx_display_font_table_set                                _gx_display_font_table_set
#define gx_display_pixelmap_table_set                            _gx_display_pixelmap_table_set


#define gx_circular_gauge_angle_get                              _gx_circular_gauge_angle_get
#define gx_circular_gauge_angle_set                              _gx_circular_gauge_angle_set
#define gx_circular_gauge_animation_set                          _gx_circular_gauge_animation_set
#define gx_circular_gauge_background_draw                        _gx_circular_gauge_background_draw
#define gx_circular_gauge_create(a, b, c, d, e, f, g, h, i)      _gx_circular_gauge_create((GX_CIRCULAR_GAUGE *) a, b, c, d, e, f, g, h, i)
#define gx_circular_gauge_draw                                   _gx_circular_gauge_draw
#define gx_circular_gauge_event_process                          _gx_circular_gauge_event_process


#define gx_drop_list_close                                       _gx_drop_list_close
#define gx_drop_list_create(a, b, c, d, e, f, g, h, i)           _gx_drop_list_create(a, b, (GX_WIDGET *)c, d, e, f, g, h, i)
#define gx_drop_list_event_process                               _gx_drop_list_event_process
#define gx_drop_list_open                                        _gx_drop_list_open
#define gx_drop_list_pixelmap_set                                _gx_drop_list_pixelmap_set
#define gx_drop_list_popup_get                                   _gx_drop_list_popup_get

#define gx_horizontal_list_children_position                     _gx_horizontal_list_children_position
#define gx_horizontal_list_create(a, b, c, d, e, f, g, h)        _gx_horizontal_list_create(a, b, (GX_WIDGET *)c, d, e, f, g, h)
#define gx_horizontal_list_event_process                         _gx_horizontal_list_event_process
#define gx_horizontal_list_selected_index_get(a, b)              _gx_horizontal_list_selected_index_get(a, b)
#define gx_horizontal_list_selected_widget_get(a, b)             _gx_horizontal_list_selected_widget_get(a, (GX_WIDGET **)b)
#define gx_horizontal_list_selected_set(a, b)                    _gx_horizontal_list_selected_set(a, b)
#define gx_horizontal_list_total_columns_set(a, b)               _gx_horizontal_list_total_columns_set((GX_HORIZONTAL_LIST *)a, b)
#define gx_horizontal_list_page_index_set(a, b)                  _gx_horizontal_list_page_index_set((GX_HORIZONTAL_LIST *)a, b)

#define gx_horizontal_scrollbar_create(a, b, c, d, e)            _gx_horizontal_scrollbar_create(a, b, (GX_WINDOW *)c, d, e)

#define gx_icon_background_draw                                  _gx_icon_background_draw
#define gx_icon_button_create(a, b, c, d, e, f, g)               _gx_icon_button_create(a, b, (GX_WIDGET *)c, d, e, f, g)
#define gx_icon_button_pixelmap_set                              _gx_icon_button_pixelmap_set
#define gx_icon_create(a, b, c, d, e, f, g, h)                   _gx_icon_create(a, b, (GX_WIDGET *)c, d, e, f, g, h)
#define gx_icon_draw                                             _gx_icon_draw
#define gx_icon_event_process                                    _gx_icon_event_process
#define gx_icon_pixelmap_set                                     _gx_icon_pixelmap_set

#define gx_image_reader_create                                   _gx_image_reader_create
#define gx_image_reader_palette_set                              _gx_image_reader_palette_set
#define gx_image_reader_start                                    _gx_image_reader_start

#define gx_line_chart_axis_draw                                  _gx_line_chart_axis_draw
#define gx_line_chart_create(a, b, c, d, e, f, g)                _gx_line_chart_create(a, b, (GX_WIDGET *) c, d, e, f, g)
#define gx_line_chart_data_draw                                  _gx_line_chart_data_draw
#define gx_line_chart_draw                                       _gx_line_chart_draw
#define gx_line_chart_update                                     _gx_line_chart_update
#define gx_line_chart_y_scale_calculate                          _gx_line_chart_y_scale_calculate

#define gx_multi_line_text_button_create(a, b, c, d, e, f, g)    _gx_multi_line_text_button_create(a, b, (GX_WIDGET *)c, d, e, f, g)
#define gx_multi_line_text_button_draw                           _gx_multi_line_text_button_draw
#define gx_multi_line_text_button_event_process                  _gx_multi_line_text_button_event_process
#define gx_multi_line_text_button_text_draw                      _gx_multi_line_text_button_text_draw
#define gx_multi_line_text_button_text_id_set                    _gx_multi_line_text_button_text_id_set
#define gx_multi_line_text_button_text_set                       _gx_multi_line_text_button_text_set

#define gx_multi_line_text_view_char_index_set                   _gx_multi_line_text_view_char_index_set
#define gx_multi_line_text_input_create(a, b, c, d, e, f, g, h)  _gx_multi_line_text_input_create(a, b, (GX_WIDGET *)c, d, e, f, g, h)
#define gx_multi_line_text_input_style_remove                    _gx_multi_line_text_input_style_remove
#define gx_multi_line_text_input_style_add                       _gx_multi_line_text_input_style_add
#define gx_multi_line_text_input_style_set                       _gx_multi_line_text_input_style_set
#define gx_multi_line_text_input_buffer_get                      _gx_multi_line_text_input_buffer_get
#define gx_multi_line_text_input_buffer_clear                    _gx_multi_line_text_input_buffer_clear

#define gx_multi_line_text_view_create(a, b, c, d, e, f, g)      _gx_multi_line_text_view_create(a, b, (GX_WIDGET *)c, d, e, f, g)
#define gx_multi_line_text_view_event_process                    _gx_multi_line_text_view_event_process
#define gx_multi_line_text_view_font_set                         _gx_multi_line_text_view_font_set
#define gx_multi_line_text_view_scroll                           _gx_multi_line_text_view_scroll
#define gx_multi_line_text_view_text_color_set                   _gx_multi_line_text_view_text_color_set
#define gx_multi_line_text_view_text_id_set                      _gx_multi_line_text_view_text_id_set
#define gx_multi_line_text_view_text_set                         _gx_multi_line_text_view_text_set

#define gx_pixelmap_button_create(a, b, c, d, e, f, g, h, i)     _gx_pixelmap_button_create(a, b, (GX_WIDGET *)c, d, e, f, g, h, i)
#define gx_pixelmap_button_draw                                  _gx_pixelmap_button_draw
#define gx_pixelmap_button_event_process                         _gx_pixelmap_button_event_process
#define gx_pixelmap_button_pixelmap_set                          _gx_pixelmap_button_pixelmap_set

#define gx_pixelmap_prompt_create(a, b, c, d, e, f, g, h)        _gx_pixelmap_prompt_create(a, b, (GX_WIDGET *)c, d, e, f, g, h)
#define gx_pixelmap_prompt_draw                                  _gx_pixelmap_prompt_draw
#define gx_pixelmap_prompt_pixelmap_set                          _gx_pixelmap_prompt_pixelmap_set

#define gx_pixelmap_slider_create(a, b, c, d, e, f, g, h)        _gx_pixelmap_slider_create(a, b, (GX_WIDGET *)c, d, e, f, g, h)
#define gx_pixelmap_slider_draw                                  _gx_pixelmap_slider_draw
#define gx_pixelmap_slider_event_process                         _gx_pixelmap_slider_event_process
#define gx_pixelmap_slider_pixelmap_set                          _gx_pixelmap_slider_pixelmap_set

#define gx_progress_bar_create(a, b, c, d, e, f, g)              _gx_progress_bar_create(a, b, (GX_WIDGET *)c, d, e, f, g)
#define gx_progress_bar_draw                                     _gx_progress_bar_draw
#define gx_progress_bar_event_process                            _gx_progress_bar_event_process
#define gx_progress_bar_font_set                                 _gx_progress_bar_font_set
#define gx_progress_bar_info_set(a, b)                           _gx_progress_bar_info_set((GX_PROGRESS_BAR *)a, b)
#define gx_progress_bar_pixelmap_set                             _gx_progress_bar_pixelmap_set
#define gx_progress_bar_range_set                                _gx_progress_bar_range_set
#define gx_progress_bar_text_color_set                           _gx_progress_bar_text_color_set
#define gx_progress_bar_value_set                                _gx_progress_bar_value_set

#define gx_prompt_create(a, b, c, d, e, f, g)                    _gx_prompt_create(a, b, (GX_WIDGET *)c, d, e, f, g)
#define gx_prompt_draw                                           _gx_prompt_draw
#define gx_prompt_font_set                                       _gx_prompt_font_set
#define gx_prompt_text_color_set                                 _gx_prompt_text_color_set
#define gx_prompt_text_draw                                      _gx_prompt_text_draw
#define gx_prompt_text_get                                       _gx_prompt_text_get
#define gx_prompt_text_id_set                                    _gx_prompt_text_id_set
#define gx_prompt_text_set                                       _gx_prompt_text_set

#define gx_radial_progress_bar_anchor_set                        _gx_radial_progress_bar_anchor_set
#define gx_radial_progress_bar_background_draw                   _gx_radial_progress_bar_background_draw
#define gx_radial_progress_bar_create                            _gx_radial_progress_bar_create
#define gx_radial_progress_bar_draw                              _gx_radial_progress_bar_draw
#define gx_radial_progress_bar_event_process                     _gx_radial_progress_bar_event_process
#define gx_radial_progress_bar_font_set                          _gx_radial_progress_bar_font_set
#define gx_radial_progress_bar_info_set                          _gx_radial_progress_bar_info_set
#define gx_radial_progress_bar_text_color_set                    _gx_radial_progress_bar_text_color_set
#define gx_radial_progress_bar_text_draw                         _gx_radial_progress_bar_text_draw
#define gx_radial_progress_bar_value_set                         _gx_radial_progress_bar_value_set

#define gx_radio_button_create(a, b, c, d, e, f, g)              _gx_radio_button_create(a, b, (GX_WIDGET *)c, d, e, f, g)
#define gx_radio_button_draw                                     _gx_radio_button_draw
#define gx_radio_button_pixelmap_set                             _gx_radio_button_pixelmap_set

#define gx_screen_stack_create                                   _gx_screen_stack_create
#define gx_screen_stack_push                                     _gx_screen_stack_push
#define gx_screen_stack_pop                                      _gx_screen_stack_pop
#define gx_screen_stack_reset                                    _gx_screen_stack_reset

#define gx_scroll_thumb_create                                   _gx_scroll_thumb_create
#define gx_scroll_thumb_draw                                     _gx_scroll_thumb_draw
#define gx_scroll_thumb_event_process                            _gx_scroll_thumb_event_process

#define gx_scrollbar_draw                                        _gx_scrollbar_draw
#define gx_scrollbar_event_process                               _gx_scrollbar_event_process
#define gx_scrollbar_limit_check                                 _gx_scrollbar_limit_check
#define gx_scrollbar_reset                                       _gx_scrollbar_reset

#define gx_single_line_text_input_backspace(a)                   _gx_single_line_text_input_backspace((GX_SINGLE_LINE_TEXT_INPUT *)a)
#define gx_single_line_text_input_buffer_clear(a)                _gx_single_line_text_input_buffer_clear((GX_SINGLE_LINE_TEXT_INPUT *)a);
#define gx_single_line_text_input_buffer_get(a, b, c, d)         _gx_single_line_text_input_buffer_get((GX_SINGLE_LINE_TEXT_INPUT *)a, b, c, d);
#define gx_single_line_text_input_character_delete(a)            _gx_single_line_text_input_character_delete(a)
#define gx_single_line_text_input_character_insert(a, b, c)      _gx_single_line_text_input_character_insert(a, b, c)
#define gx_single_line_text_input_create(a, b, c, d, e, f, g, h) _gx_single_line_text_input_create(a, b, (GX_WIDGET *)c, d, e, f, g, h)
#define gx_single_line_text_input_draw(a)                        _gx_single_line_text_input_draw(a)
#define gx_single_line_text_input_end(a)                         _gx_single_line_text_input_end((GX_SINGLE_LINE_TEXT_INPUT *)a)
#define gx_single_line_text_input_event_process(a, b)            _gx_single_line_text_input_event_process((GX_SINGLE_LINE_TEXT_INPUT *)a, b)
#define gx_single_line_text_input_home(a)                        _gx_single_line_text_input_home((GX_SINGLE_LINE_TEXT_INPUT *)a)
#define gx_single_line_text_input_left_arrow(a)                  _gx_single_line_text_input_left_arrow((GX_SINGLE_LINE_TEXT_INPUT *)a)
#define gx_single_line_text_input_position_get(a, b)             _gx_single_line_text_input_position_get(a, b)
#define gx_single_line_text_input_right_arrow(a)                 _gx_single_line_text_input_right_arrow((GX_SINGLE_LINE_TEXT_INPUT *)a)
#define gx_single_line_text_input_style_add(a, b)                _gx_single_line_text_input_style_add((GX_SINGLE_LINE_TEXT_INPUT *)a, b);
#define gx_single_line_text_input_style_remove(a, b)             _gx_single_line_text_input_style_remove((GX_SINGLE_LINE_TEXT_INPUT *)a, b)
#define gx_single_line_text_input_style_set(a, b)                _gx_single_line_text_input_style_set((GX_SINGLE_LINE_TEXT_INPUT *)a, b)


#define gx_slider_create(a, b, c, d, e, f, g, h)                 _gx_slider_create(a, b, (GX_WIDGET *)c, d, e, f, g, h)
#define gx_slider_draw                                           _gx_slider_draw
#define gx_slider_event_process                                  _gx_slider_event_process
#define gx_slider_info_set                                       _gx_slider_info_set
#define gx_slider_needle_draw                                    _gx_slider_needle_draw
#define gx_slider_needle_position_get                            _gx_slider_needle_position_get
#define gx_slider_tickmarks_draw                                 _gx_slider_tickmarks_draw
#define gx_slider_travel_get                                     _gx_slider_travel_get
#define gx_slider_value_calculate                                _gx_slider_value_calculate
#define gx_slider_value_set                                      _gx_slider_value_set

#define gx_sprite_create(a, b, c, d, e, f, g, h)                 _gx_sprite_create(a, b, (GX_WIDGET *)c, d, e, f, g, h)
#define gx_sprite_current_frame_set                              _gx_sprite_current_frame_set
#define gx_sprite_frame_list_set                                 _gx_sprite_frame_list_set
#define gx_sprite_start(a, b)                                    _gx_sprite_start(a, b)
#define gx_sprite_stop(a)                                        _gx_sprite_stop(a)

#define gx_system_active_language_set                            _gx_system_active_language_set
#define gx_system_canvas_refresh                                 _gx_system_canvas_refresh
#define gx_system_dirty_mark(a)                                  _gx_system_dirty_mark((GX_WIDGET *)a);
#define gx_system_dirty_partial_add(a, b)                        _gx_system_dirty_partial_add((GX_WIDGET *)a, b)
#define gx_system_draw_context_get                               _gx_system_draw_context_get
#define gx_system_event_fold                                     _gx_system_event_fold
#define gx_system_event_send                                     _gx_system_event_send
#define gx_system_focus_claim(a)                                 _gx_system_focus_claim((GX_WIDGET *)a)
#define gx_system_initialize                                     _gx_system_initialize
#define gx_system_language_table_get                             _gx_system_language_table_get
#define gx_system_language_table_set                             _gx_system_language_table_set
#define gx_system_memory_allocator_set                           _gx_system_memory_allocator_set
#define gx_system_scroll_appearance_get                          _gx_system_scroll_appearance_get
#define gx_system_scroll_appearance_set                          _gx_system_scroll_appearance_set
#define gx_system_start                                          _gx_system_start
#define gx_system_string_get                                     _gx_system_string_get
#define gx_system_string_table_get                               _gx_system_string_table_get
#define gx_system_string_width_get                               _gx_system_string_width_get
#define gx_system_timer_start(a, b, c, d)                        _gx_system_timer_start((GX_WIDGET *)a, b, c, d)
#define gx_system_timer_stop(a, b)                               _gx_system_timer_stop((GX_WIDGET *)a, b)
#define gx_system_version_string_get                             _gx_system_version_string_get
#define gx_system_widget_find                                    _gx_system_widget_find

#define gx_text_button_create(a, b, c, d, e, f, g)               _gx_text_button_create(a, b, (GX_WIDGET *)c, d, e, f, g)
#define gx_text_button_draw                                      _gx_text_button_draw
#define gx_text_button_font_set                                  _gx_text_button_font_set
#define gx_text_button_text_color_set                            _gx_text_button_text_color_set
#define gx_text_button_text_draw                                 _gx_text_button_text_draw
#define gx_text_button_text_get                                  _gx_text_button_text_get
#define gx_text_button_text_id_set                               _gx_text_button_text_id_set
#define gx_text_button_text_set                                  _gx_text_button_text_set

#define gx_text_input_cursor_height_set                          _gx_text_input_cursor_height_set
#define gx_text_input_cursor_width_set                           _gx_text_input_cursor_width_set
#define gx_text_input_cursor_blink_interval_set                  _gx_text_input_cursor_blink_interval_set

#define gx_transition_window_create(a, b, c, d, e, f)            _gx_transition_window_create(a, b, (GX_WIDGET *)c, d, e, f)

#define gx_utility_ltoa                                          _gx_utility_ltoa
#define gx_utility_math_acos                                     _gx_utility_math_acos
#define gx_utility_math_asin                                     _gx_utility_math_asin
#define gx_utility_math_cos                                      _gx_utility_math_cos
#define gx_utility_math_sin                                      _gx_utility_math_sin
#define gx_utility_math_sqrt                                     _gx_utility_math_sqrt

#define gx_utility_rectangle_center                              _gx_utility_rectangle_center
#define gx_utility_rectangle_center_find                         _gx_utility_rectangle_center_find
#define gx_utility_rectangle_combine                             _gx_utility_rectangle_combine
#define gx_utility_rectangle_compare                             _gx_utility_rectangle_compare
#define gx_utility_rectangle_define                              _gx_utility_rectangle_define
#define gx_utility_rectangle_resize                              _gx_utility_rectangle_resize
#define gx_utility_rectangle_overlap_detect                      _gx_utility_rectangle_overlap_detect
#define gx_utility_rectangle_point_detect                        _gx_utility_rectangle_point_detect
#define gx_utility_rectangle_shift                               _gx_utility_rectangle_shift
#define gx_utility_pixelmap_rotate                               _gx_utility_pixelmap_rotate
#define gx_utility_pixelmap_simple_rotate                        _gx_utility_pixelmap_simple_rotate
#define gx_utility_pixelmap_resize                               _gx_utility_pixelmap_resize

#define gx_vertical_list_children_position                       _gx_vertical_list_children_position
#define gx_vertical_list_create(a, b, c, d, e, f, g, h)          _gx_vertical_list_create(a, b, (GX_WIDGET *)c, d, e, f, g, h)
#define gx_vertical_list_event_process                           _gx_vertical_list_event_process
#define gx_vertical_list_selected_index_get(a, b)                _gx_vertical_list_selected_index_get(a, b)
#define gx_vertical_list_selected_widget_get(a, b)               _gx_vertical_list_selected_widget_get(a, (GX_WIDGET **)b)
#define gx_vertical_list_selected_set(a, b)                      _gx_vertical_list_selected_set(a, b)
#define gx_vertical_list_total_rows_set(a, b)                    _gx_vertical_list_total_rows_set((GX_VERTICAL_LIST *)a, b)
#define gx_vertical_list_page_index_set(a, b)                    _gx_vertical_list_page_index_set((GX_VERTICAL_LIST *)a, b)

#define gx_vertical_scrollbar_create(a, b, c, d, e)              _gx_vertical_scrollbar_create(a, b, (GX_WINDOW *)c, d, e)

#define gx_widget_allocate(a, b)                                 _gx_widget_allocate((GX_WIDGET **)a, b)
#define gx_widget_attach(a, b)                                   _gx_widget_attach((GX_WIDGET *)a, (GX_WIDGET *)b)
#define gx_widget_background_draw(a)                             _gx_widget_background_draw((GX_WIDGET *)a)
#define gx_widget_back_attach(a, b)                              _gx_widget_back_attach((GX_WIDGET *)a, (GX_WIDGET *)b)
#define gx_widget_back_move(a, b)                                _gx_widget_back_move((GX_WIDGET *)a, b)
#define gx_widget_block_move(a, b, c, d)                         _gx_widget_block_move((GX_WIDGET *)a, b, c, d)
#define gx_widget_border_draw(a, b, c, d, e)                     _gx_widget_border_draw((GX_WIDGET *)a, b, c, d, e)
#define gx_widget_border_style_set(a, b)                         _gx_widget_border_style_set((GX_WIDGET *)a, b)
#define gx_widget_border_width_get(a, b)                         _gx_widget_border_width_get((GX_WIDGET *)a, b)
#define gx_widget_canvas_get(a, b)                               _gx_widget_canvas_get((GX_WIDGET *)a, b)
#define gx_widget_child_detect(a, b, c)                          _gx_widget_child_detect((GX_WIDGET *)a, b, c)
#define gx_widget_children_draw(a)                               _gx_widget_children_draw((GX_WIDGET *)a)
#define gx_widget_client_get(a, b, c)                            _gx_widget_client_get((GX_WIDGET *)a, b, c)
#define gx_widget_color_get(a, b, c)                             _gx_widget_color_get((GX_WIDGET *)a, b, c)
#define gx_widget_create(a, b, c, d, e, f)                       _gx_widget_create((GX_WIDGET *)a, b, (GX_WIDGET *)c, d, e, f)
#define gx_widget_created_test(a, b)                             _gx_widget_created_test((GX_WIDGET *)a, b)
#define gx_widget_delete(a)                                      _gx_widget_delete((GX_WIDGET *)a)
#define gx_widget_detach(a)                                      _gx_widget_detach((GX_WIDGET *)a)
#define gx_widget_draw(a)                                        _gx_widget_draw((GX_WIDGET *)a)
#define gx_widget_draw_set(a, b)                                 _gx_widget_draw_set((GX_WIDGET *)a, (void (*)(GX_WIDGET *))b)
#define gx_widget_event_generate(a, b, c)                        _gx_widget_event_generate((GX_WIDGET *)a, b, c)
#define gx_widget_event_process(a, b)                            _gx_widget_event_process((GX_WIDGET *)a, b)
#define gx_widget_event_process_set(a, b)                        _gx_widget_event_process_set((GX_WIDGET *)a, (UINT (*)(GX_WIDGET *, GX_EVENT *)) b)
#define gx_widget_event_to_parent(a, b)                          _gx_widget_event_to_parent((GX_WIDGET *)a, b)
#define gx_widget_fill_color_set(a, b, c)                        _gx_widget_fill_color_set((GX_WIDGET *)a, b, c)
#define gx_widget_find(a, b, c, d)                               _gx_widget_find((GX_WIDGET *)a, b, c, (GX_WIDGET **)d)
#define gx_widget_font_get(a, b, c)                              _gx_widget_font_get((GX_WIDGET *)a, b, c)
#define gx_widget_free(a)                                        _gx_widget_free((GX_WIDGET *)a)
#define gx_widget_front_move(a, b)                               _gx_widget_front_move((GX_WIDGET *)a, b)
#define gx_widget_focus_next(a)                                  _gx_widget_focus_next((GX_WIDGET *)a)
#define gx_widget_focus_previous(a)                              _gx_widget_focus_previous((GX_WIDGET *)a)
#define gx_widget_height_get(a, b)                               _gx_widget_height_get((GX_WIDGET *)a, b)
#define gx_widget_hide(a)                                        _gx_widget_hide((GX_WIDGET *)a)
#define gx_widget_pixelmap_get(a, b, c)                          _gx_widget_pixelmap_get((GX_WIDGET *)a, b, c)
#define gx_widget_resize(a, b)                                   _gx_widget_resize((GX_WIDGET *)a, b)
#define gx_widget_shift(a, b, c, d)                              _gx_widget_shift((GX_WIDGET *)a, b, c, d)
#define gx_widget_show(a)                                        _gx_widget_show((GX_WIDGET *)a)
#define gx_widget_status_add(a, b)                               _gx_widget_status_add((GX_WIDGET *)a, b)
#define gx_widget_status_get(a, b)                               _gx_widget_status_get((GX_WIDGET *)a, b)
#define gx_widget_status_remove(a, b)                            _gx_widget_status_remove((GX_WIDGET *)a, b)
#define gx_widget_status_test(a, b, c)                           _gx_widget_status_test((GX_WIDGET *)a, b, c)
#define gx_widget_style_add(a, b)                                _gx_widget_style_add((GX_WIDGET *)a, b)
#define gx_widget_style_get(a, b)                                _gx_widget_style_get((GX_WIDGET *)a, b)
#define gx_widget_style_remove(a, b)                             _gx_widget_style_remove((GX_WIDGET *)a, b)
#define gx_widget_style_set(a, b)                                _gx_widget_style_set((GX_WIDGET *)a, b)
#define gx_widget_text_blend(a, b, c, d, e, f, g)                _gx_widget_text_blend((GX_WIDGET *)a, b, c, d, e, f, g)
#define gx_widget_text_draw(a, b, c, d, e, f)                    _gx_widget_text_draw((GX_WIDGET *)a, b, c, d, e, f)
#define gx_widget_text_id_draw(a, b, c, d, e, f)                 _gx_widget_text_id_draw((GX_WIDGET *)a, b, c, d, e, f)
#define gx_widget_type_find(a, b, c)                             _gx_widget_type_find((GX_WIDGET *)a, b, (GX_WIDGET **)c)
#define gx_widget_width_get(a, b)                                _gx_widget_width_get((GX_WIDGET *)a, b)

#define gx_window_client_height_get                              _gx_window_client_height_get
#define gx_window_client_scroll                                  _gx_window_client_scroll
#define gx_window_client_width_get                               _gx_window_client_width_get
#define gx_window_close(a)                                       _gx_window_close((GX_WINDOW *) a)
#define gx_window_create(a, b, c, d, e, f)                       _gx_window_create(a, b, (GX_WIDGET *)c, d, e, f)
#define gx_window_draw                                           _gx_window_draw
#define gx_window_event_process                                  _gx_window_event_process
#define gx_window_execute                                        _gx_window_execute
#define gx_window_root_create                                    _gx_window_root_create
#define gx_window_root_delete                                    _gx_window_root_delete
#define gx_window_root_event_process                             _gx_window_root_event_process
#define gx_window_root_find(a, b)                                _gx_window_root_find((GX_WIDGET *)a, b)
#define gx_window_scroll_info_get                                _gx_window_scroll_info_get
#define gx_window_scrollbar_find                                 _gx_window_scrollbar_find
#define gx_window_wallpaper_get                                  _gx_window_wallpaper_get
#define gx_window_wallpaper_set(a, b, c)                         _gx_window_wallpaper_set((GX_WINDOW *)a, b, c)

/* Define the non-error checking function prototypes of the GUIX API.  These are the routines called when the application code is built with
   GX_DISABLE_ERROR_CHECKING defined.  */

UINT _gx_animation_canvas_define(GX_ANIMATION *animation, GX_VALUE width, GX_VALUE height,
                                 GX_DISPLAY *display, GX_COLOR *memory_area, ULONG memory_size);
UINT _gx_animation_create(GX_ANIMATION *animation);
UINT _gx_animation_delete(GX_ANIMATION *animation);
UINT _gx_animation_start(GX_ANIMATION *animation, GX_ANIMATION_INFO *info);
UINT _gx_animation_stop(GX_ANIMATION *animation);

UINT _gx_brush_default(GX_BRUSH *brush);
UINT _gx_brush_define(GX_BRUSH *brush, GX_COLOR line_color, GX_COLOR fill_color, UINT style);

UINT _gx_button_background_draw(GX_BUTTON *button);
UINT _gx_button_create(GX_BUTTON *button,
                       GX_CONST GX_CHAR *name,
                       GX_WIDGET *parent,
                       ULONG style, USHORT Id,
                       GX_CONST GX_RECTANGLE *size);
UINT _gx_button_deselect(GX_WIDGET *button);
UINT _gx_button_draw(GX_BUTTON *button);
UINT _gx_button_event_process(GX_BUTTON *button, GX_EVENT *event_ptr);
UINT _gx_button_select(GX_WIDGET *button);

UINT _gx_canvas_alpha_set(GX_CANVAS *canvas, GX_UBYTE alpha);
UINT _gx_canvas_block_move(GX_RECTANGLE *block, GX_VALUE x_shift, GX_VALUE y_shift, GX_RECTANGLE *dirty);
UINT _gx_canvas_create(GX_CANVAS *canvas, GX_CONST GX_CHAR *name, GX_DISPLAY *display,
                       UINT type, UINT width, UINT height, GX_COLOR *memory_area, ULONG memory_size);
UINT _gx_canvas_delete(GX_CANVAS *canvas);
UINT _gx_canvas_drawing_complete(GX_CANVAS *canvas, GX_BOOL Flush);
UINT _gx_canvas_drawing_initiate(GX_CANVAS *canvas, GX_WIDGET *who, GX_RECTANGLE *dirty_area);
UINT _gx_canvas_line_draw(GX_VALUE x_start, GX_VALUE y_start, GX_VALUE x_end, GX_VALUE y_end);
UINT _gx_canvas_offset_set(GX_CANVAS *canvas, GX_VALUE x, GX_VALUE y);
UINT _gx_canvas_pixel_draw(GX_POINT position);
UINT _gx_canvas_pixelmap_blend(GX_VALUE x_position, GX_VALUE y_position, GX_PIXELMAP *pixelmap, GX_UBYTE alpha);
UINT _gx_canvas_pixelmap_draw(GX_VALUE x_position, GX_VALUE y_position, GX_PIXELMAP *pixelmap);
UINT _gx_canvas_pixelmap_tile(GX_RECTANGLE *fill, GX_PIXELMAP *pixelmap);
UINT _gx_canvas_pixelmap_rotate(GX_VALUE x_position, GX_VALUE y_position, GX_PIXELMAP *pixelmap,
                                INT angle, INT rot_cx, INT rot_cy);
UINT _gx_canvas_polygon_draw(GX_POINT *point_array, INT number_of_points);
UINT _gx_canvas_circle_draw(INT xcenter, INT ycenter, UINT r);
UINT _gx_canvas_arc_draw(INT xcenter, INT ycenter, UINT r, INT start_angle, INT end_angle);
UINT _gx_canvas_pie_draw(INT xcenter, INT ycenter, UINT r, INT start_angle, INT end_angle);
UINT _gx_canvas_ellipse_draw(INT xcenter, INT ycenter, INT a, INT b);
UINT _gx_canvas_rectangle_draw(GX_RECTANGLE *rectangle);
UINT _gx_canvas_shift(GX_CANVAS *canvas, GX_VALUE x, GX_VALUE y);
UINT _gx_canvas_text_draw(GX_VALUE x_start, GX_VALUE y_start, GX_CHAR *string, INT length);

UINT _gx_checkbox_create(GX_CHECKBOX *checkbox,
                         GX_CONST GX_CHAR *name,
                         GX_WIDGET *parent,
                         GX_RESOURCE_ID text_id, ULONG style, USHORT checkbox_id,
                         GX_CONST GX_RECTANGLE *size);
UINT _gx_checkbox_draw(GX_CHECKBOX *checkbox);
UINT _gx_checkbox_event_process(GX_CHECKBOX *checkbox, GX_EVENT *event_ptr);
UINT _gx_checkbox_pixelmap_set(GX_CHECKBOX *button,  GX_RESOURCE_ID unchecked_id, GX_RESOURCE_ID checked_id,
                               GX_RESOURCE_ID unchecked_disabled_id, GX_RESOURCE_ID checked_disabled_id);
UINT _gx_checkbox_select(GX_CHECKBOX *checkbox);

UINT _gx_context_brush_default(GX_DRAW_CONTEXT *context);
UINT _gx_context_brush_define(GX_RESOURCE_ID line_color_id, GX_RESOURCE_ID fill_color_id, UINT style);
UINT _gx_context_brush_get(GX_BRUSH **return_brush);
UINT _gx_context_brush_pattern_set(ULONG pattern);
UINT _gx_context_brush_set(GX_BRUSH *brush);
UINT _gx_context_brush_style_set(UINT style);
UINT _gx_context_brush_width_set(UINT width);
UINT _gx_context_color_get(GX_RESOURCE_ID color_id, GX_COLOR *return_color);
UINT _gx_context_fill_color_set(GX_RESOURCE_ID fill_color_id);
UINT _gx_context_font_set(GX_RESOURCE_ID font_id);
UINT _gx_context_font_get(GX_RESOURCE_ID color_id, GX_FONT **return_font);
UINT _gx_context_line_color_set(GX_RESOURCE_ID line_color_id);
UINT _gx_context_pixelmap_set(GX_RESOURCE_ID pixelmap_id);
UINT _gx_context_pixelmap_get(GX_RESOURCE_ID pixelmap_id, GX_PIXELMAP **return_pixelmap);
UINT _gx_context_raw_brush_define(GX_COLOR line_color, GX_COLOR fill_color, UINT style);
UINT _gx_context_raw_fill_color_set(GX_COLOR line_color);
UINT _gx_context_raw_line_color_set(GX_COLOR line_color);

UINT  _gx_circular_gauge_angle_get(GX_CIRCULAR_GAUGE *circular_gauge, INT *angle);
UINT  _gx_circular_gauge_angle_set(GX_CIRCULAR_GAUGE *circular_gauge, INT angle);
UINT  _gx_circular_gauge_animation_set(GX_CIRCULAR_GAUGE *circular_gauge, INT steps, INT delay);
UINT  _gx_circular_gauge_background_draw(GX_CIRCULAR_GAUGE *gauge);
UINT  _gx_circular_gauge_create(GX_CIRCULAR_GAUGE *circular_gauge,
                                GX_CONST GX_CHAR *name,
                                GX_WIDGET *parent,
                                GX_CIRCULAR_GAUGE_INFO *circular_gauge_info,
                                GX_RESOURCE_ID background,
                                ULONG style,
                                USHORT circular_gauge_id,
                                GX_VALUE xpos, GX_VALUE ypos);
UINT  _gx_circular_gauge_draw(GX_CIRCULAR_GAUGE *circular_gauge);
UINT  _gx_circular_gauge_event_process(GX_CIRCULAR_GAUGE *circular_gauge, GX_EVENT *event_ptr);

UINT _gx_display_create(GX_DISPLAY *display, GX_CONST GX_CHAR *name, UINT (*display_driver_setup)(GX_DISPLAY *), GX_VALUE xres, GX_VALUE yres);
UINT _gx_display_delete(GX_DISPLAY *display, VOID (*display_driver_cleanup)(GX_DISPLAY *));
UINT _gx_display_color_table_set(GX_DISPLAY *display, GX_COLOR *color_table, INT number_of_colors);
UINT _gx_display_font_table_set(GX_DISPLAY *display, GX_FONT **font_table, UINT number_of_fonts);
UINT _gx_display_pixelmap_table_set(GX_DISPLAY *display, GX_PIXELMAP **pixelmap_table, UINT number_of_pixelmaps);

UINT _gx_drop_list_close(GX_DROP_LIST *drop_list);
UINT _gx_drop_list_create(GX_DROP_LIST *drop_list,
                          GX_CONST GX_CHAR *name,
                          GX_WIDGET *parent,
                          INT total_rows, INT open_height,
                          VOID (*callback)(GX_VERTICAL_LIST *, GX_WIDGET *, INT),
                          ULONG style, USHORT drop_list_id,
                          GX_CONST GX_RECTANGLE *size);
UINT _gx_drop_list_event_process(GX_DROP_LIST *list, GX_EVENT *event_ptr);
UINT _gx_drop_list_open(GX_DROP_LIST *list);
UINT _gx_drop_list_pixelmap_set(GX_DROP_LIST *drop_list, GX_RESOURCE_ID pixelmap_id);
UINT _gx_drop_list_popup_get(GX_DROP_LIST *box, GX_VERTICAL_LIST **return_list);

UINT _gx_horizontal_list_children_position(GX_HORIZONTAL_LIST *horizontal_list);
UINT _gx_horizontal_list_create(GX_HORIZONTAL_LIST *horizontal_list,
                                GX_CONST GX_CHAR *name,
                                GX_WIDGET *parent,
                                INT total_columns,
                                VOID (*callback)(GX_HORIZONTAL_LIST *, GX_WIDGET *, INT),
                                ULONG style, USHORT horizontal_list_id,
                                GX_CONST GX_RECTANGLE *size);
UINT _gx_horizontal_list_event_process(GX_HORIZONTAL_LIST *list, GX_EVENT *event_ptr);
UINT _gx_horizontal_list_selected_index_get(GX_HORIZONTAL_LIST *horizontal_list, INT *return_index);
UINT _gx_horizontal_list_selected_widget_get(GX_HORIZONTAL_LIST *horizontal_list, GX_WIDGET **return_list_entry);
UINT _gx_horizontal_list_selected_set(GX_HORIZONTAL_LIST *horizontal_list, INT index);
UINT _gx_horizontal_list_total_columns_set(GX_HORIZONTAL_LIST *horizontal_list, INT count);
UINT _gx_horizontal_list_page_index_set(GX_HORIZONTAL_LIST *list, INT index);

UINT _gx_horizontal_scrollbar_create(GX_SCROLLBAR *scrollbar, GX_CONST GX_CHAR *name,
                                     GX_WINDOW *parent, GX_SCROLLBAR_APPEARANCE *appearance, ULONG style);

UINT _gx_icon_background_draw(GX_ICON *icon);
UINT _gx_icon_button_create(GX_ICON_BUTTON *button,
                            GX_CONST GX_CHAR *name,
                            GX_WIDGET *parent,
                            GX_RESOURCE_ID icon_id,
                            ULONG style, USHORT icon_button_id,
                            GX_CONST GX_RECTANGLE *size);
UINT _gx_icon_button_pixelmap_set(GX_ICON_BUTTON *button, GX_RESOURCE_ID icon_id);
UINT _gx_icon_create(GX_ICON *icon,
                     GX_CONST GX_CHAR *name,
                     GX_WIDGET *parent,
                     GX_RESOURCE_ID pixelmap_id, ULONG style,
                     USHORT icon_id, GX_VALUE x, GX_VALUE y);
UINT _gx_icon_draw(GX_ICON *icon);
UINT _gx_icon_event_process(GX_ICON *icon, GX_EVENT *event_ptr);

UINT _gx_icon_pixelmap_set(GX_ICON *icon, GX_RESOURCE_ID normal_id, GX_RESOURCE_ID selected_id);

UINT _gx_image_reader_create(GX_IMAGE_READER *image_reader,
                             const GX_UBYTE *read_data,
                             INT read_data_size,
                             GX_UBYTE color_format,
                             GX_UBYTE mode);

UINT _gx_image_reader_palette_set(GX_IMAGE_READER *image_reader, GX_COLOR *pal, UINT palsize);
UINT _gx_image_reader_start(GX_IMAGE_READER *image_reader, GX_PIXELMAP *outmap);

UINT _gx_line_chart_axis_draw(GX_LINE_CHART *chart);
UINT _gx_line_chart_create(GX_LINE_CHART *chart,
                          GX_CONST GX_CHAR *name,
                          GX_WIDGET *parent,
                          GX_CONST GX_LINE_CHART_INFO *info,
                          ULONG style,
                          USHORT chart_id,
                          GX_CONST GX_RECTANGLE *size);
UINT _gx_line_chart_data_draw(GX_LINE_CHART *chart);
UINT _gx_line_chart_draw(GX_LINE_CHART *chart);
UINT _gx_line_chart_update(GX_LINE_CHART *chart, INT *data, INT data_count);
UINT _gx_line_chart_y_scale_calculate(GX_LINE_CHART *chart, INT *return_val);

UINT _gx_multi_line_text_button_create(GX_MULTI_LINE_TEXT_BUTTON *button, GX_CONST GX_CHAR *name,
                                       GX_WIDGET *parent, GX_RESOURCE_ID text_id, ULONG style, USHORT Id,
                                       GX_CONST GX_RECTANGLE *size);
UINT _gx_multi_line_text_button_draw(GX_MULTI_LINE_TEXT_BUTTON *button);
UINT _gx_multi_line_text_button_event_process(GX_MULTI_LINE_TEXT_BUTTON *button, GX_EVENT *event_ptr);
UINT _gx_multi_line_text_button_text_draw(GX_MULTI_LINE_TEXT_BUTTON *button);
UINT _gx_multi_line_text_button_text_id_set(GX_MULTI_LINE_TEXT_BUTTON *button, GX_RESOURCE_ID string_id);
UINT _gx_multi_line_text_button_text_set(GX_MULTI_LINE_TEXT_BUTTON *button, GX_CHAR *text);

UINT _gx_multi_line_text_input_create(GX_MULTI_LINE_TEXT_INPUT *text_input_ptr,
                                      GX_CONST GX_CHAR *name_ptr, GX_WIDGET *parent,
                                      GX_CHAR *input_buffer, UINT buffer_size,
                                      ULONG style, USHORT Id, GX_CONST GX_RECTANGLE *size);
UINT _gx_multi_line_text_input_style_remove(GX_MULTI_LINE_TEXT_INPUT *text_input_ptr, ULONG cursor_type);
UINT _gx_multi_line_text_input_style_add(GX_MULTI_LINE_TEXT_INPUT *text_input_ptr, ULONG cursor_type);
UINT _gx_multi_line_text_input_style_set(GX_MULTI_LINE_TEXT_INPUT *text_input_ptr, ULONG cursor_type);
UINT _gx_multi_line_text_input_buffer_get(GX_MULTI_LINE_TEXT_INPUT *text_input_ptr, GX_CHAR **buffer_address,
                                          UINT *content_size, UINT *buffer_size);
UINT _gx_multi_line_text_input_buffer_clear(GX_MULTI_LINE_TEXT_INPUT *text_input_ptr);

UINT _gx_multi_line_text_view_create(GX_MULTI_LINE_TEXT_VIEW *text_view_ptr,
                                     GX_CONST GX_CHAR *name_ptr, GX_WIDGET *parent,
                                     GX_RESOURCE_ID text_id, ULONG style, USHORT Id,
                                     GX_CONST GX_RECTANGLE *size);
UINT _gx_multi_line_text_view_event_process(GX_MULTI_LINE_TEXT_VIEW *view, GX_EVENT *event_ptr);
UINT _gx_multi_line_text_view_font_set(GX_MULTI_LINE_TEXT_VIEW *view, GX_RESOURCE_ID font_id);
UINT _gx_multi_line_text_view_scroll(GX_MULTI_LINE_TEXT_VIEW *view, GX_VALUE amount_to_scroll);
UINT _gx_multi_line_text_view_text_color_set(GX_MULTI_LINE_TEXT_VIEW *view, GX_RESOURCE_ID normal_id, GX_RESOURCE_ID selected_id);
UINT _gx_multi_line_text_view_text_id_set(GX_MULTI_LINE_TEXT_VIEW *text_view_ptr, GX_RESOURCE_ID text_id);
UINT _gx_multi_line_text_view_text_set(GX_MULTI_LINE_TEXT_VIEW *view, GX_CHAR *text);

UINT _gx_pixelmap_button_create(GX_PIXELMAP_BUTTON *button,
                                GX_CONST GX_CHAR *name,
                                GX_WIDGET *parent, GX_RESOURCE_ID normal_id,
                                GX_RESOURCE_ID selected_id, GX_RESOURCE_ID disabled_id,
                                ULONG style, USHORT pixelmap_button_id,
                                GX_CONST GX_RECTANGLE *size);
UINT _gx_pixelmap_button_draw(GX_PIXELMAP_BUTTON *button);
UINT _gx_pixelmap_button_event_process(GX_PIXELMAP_BUTTON *button, GX_EVENT *event_ptr);

UINT _gx_pixelmap_button_pixelmap_set(GX_PIXELMAP_BUTTON *button, GX_RESOURCE_ID normal_id, GX_RESOURCE_ID selected_id, GX_RESOURCE_ID disabled_id);

UINT _gx_pixelmap_prompt_create(GX_PIXELMAP_PROMPT *prompt,
                                GX_CONST GX_CHAR *name, GX_WIDGET *parent, GX_RESOURCE_ID text_id,
                                GX_RESOURCE_ID normal_fill_id, ULONG style, USHORT prompt_id,
                                GX_CONST GX_RECTANGLE *size);
UINT _gx_pixelmap_prompt_draw(GX_PIXELMAP_PROMPT *prompt);

UINT _gx_pixelmap_prompt_pixelmap_set(GX_PIXELMAP_PROMPT *prompt,
                                      GX_RESOURCE_ID normal_left_id,
                                      GX_RESOURCE_ID normal_fill_id,
                                      GX_RESOURCE_ID normal_right_id,
                                      GX_RESOURCE_ID selected_left_id,
                                      GX_RESOURCE_ID selected_fill_id,
                                      GX_RESOURCE_ID selected_right_id);

UINT _gx_pixelmap_slider_create(GX_PIXELMAP_SLIDER *slider,
                                GX_CONST GX_CHAR *name, GX_WIDGET *parent, GX_SLIDER_INFO *info,
                                GX_PIXELMAP_SLIDER_INFO *pixelmap_info,
                                ULONG style, USHORT pixelmap_slider_id,
                                GX_CONST GX_RECTANGLE *size);
UINT _gx_pixelmap_slider_draw(GX_PIXELMAP_SLIDER *slider);
UINT _gx_pixelmap_slider_event_process(GX_PIXELMAP_SLIDER *slider, GX_EVENT *event_ptr);
UINT _gx_pixelmap_slider_pixelmap_set(GX_PIXELMAP_SLIDER *slider, GX_PIXELMAP_SLIDER_INFO *info);

UINT _gx_progress_bar_create(GX_PROGRESS_BAR *progress_bar, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                             GX_PROGRESS_BAR_INFO *progress_bar_info, ULONG style,
                             USHORT progress_bar_id, GX_CONST GX_RECTANGLE *size);
UINT _gx_progress_bar_draw(GX_PROGRESS_BAR *progress_bar);
UINT _gx_progress_bar_event_process(GX_PROGRESS_BAR *progress_bar, GX_EVENT *event_ptr);
UINT _gx_progress_bar_font_set(GX_PROGRESS_BAR *progress_bar, GX_RESOURCE_ID font_id);
UINT _gx_progress_bar_info_set(GX_PROGRESS_BAR *progress_bar, GX_PROGRESS_BAR_INFO *info);
UINT _gx_progress_bar_pixelmap_set(GX_PROGRESS_BAR *progress_bar, GX_RESOURCE_ID pixelmap);
UINT _gx_progress_bar_range_set(GX_PROGRESS_BAR *progress_bar, INT min_value, INT max_value);
UINT _gx_progress_bar_text_color_set(GX_PROGRESS_BAR *progress_bar, GX_RESOURCE_ID normal_text_color_id,
                                     GX_RESOURCE_ID selected_text_color_id);
UINT _gx_progress_bar_value_set(GX_PROGRESS_BAR *progress_bar, INT new_value);

UINT _gx_prompt_create(GX_PROMPT *prompt, GX_CONST GX_CHAR *name,
                       GX_WIDGET *parent, GX_RESOURCE_ID text_id, ULONG style,
                       USHORT prompt_id, GX_CONST GX_RECTANGLE *size);
UINT _gx_prompt_draw(GX_PROMPT *prompt);
UINT _gx_prompt_font_set(GX_PROMPT *prompt, GX_RESOURCE_ID fontid);
UINT _gx_prompt_text_color_set(GX_PROMPT *prompt, GX_RESOURCE_ID normal_text_color_id, GX_RESOURCE_ID selected_text_color_id);
UINT _gx_prompt_text_draw(GX_PROMPT *prompt);
UINT _gx_prompt_text_get(GX_PROMPT *prompt, GX_CHAR **return_text);
UINT _gx_prompt_text_id_set(GX_PROMPT *prompt, GX_RESOURCE_ID string_id);
UINT _gx_prompt_text_set(GX_PROMPT *prompt, GX_CHAR *text);

UINT _gx_radial_progress_bar_anchor_set(GX_RADIAL_PROGRESS_BAR *progress_bar, GX_VALUE angle);
UINT _gx_radial_progress_bar_background_draw(GX_RADIAL_PROGRESS_BAR *progress_bar);
UINT _gx_radial_progress_bar_create(GX_RADIAL_PROGRESS_BAR *progress_bar,
                                    GX_CONST GX_CHAR *name,
                                    GX_WIDGET *parent,
                                    GX_RADIAL_PROGRESS_BAR_INFO *progress_bar_info,
                                    ULONG style,
                                    USHORT progress_bar_id);
UINT _gx_radial_progress_bar_draw(GX_RADIAL_PROGRESS_BAR *progress_bar);
UINT _gx_radial_progress_bar_event_process(GX_RADIAL_PROGRESS_BAR *progress_bar, GX_EVENT *event_ptr);
UINT _gx_radial_progress_bar_font_set(GX_RADIAL_PROGRESS_BAR *progress_bar, GX_RESOURCE_ID font_id);
UINT _gx_radial_progress_bar_info_set(GX_RADIAL_PROGRESS_BAR *bar, GX_RADIAL_PROGRESS_BAR_INFO *info);
UINT _gx_radial_progress_bar_text_color_set(GX_RADIAL_PROGRESS_BAR *progress_bar,
                                            GX_RESOURCE_ID normal_text_color_id,
                                            GX_RESOURCE_ID selected_text_color_id);
UINT _gx_radial_progress_bar_text_draw(GX_RADIAL_PROGRESS_BAR *bar);
UINT _gx_radial_progress_bar_value_set(GX_RADIAL_PROGRESS_BAR *progress_bar, GX_VALUE new_value);

UINT _gx_radio_button_create(GX_RADIO_BUTTON *button,
                             GX_CONST GX_CHAR *name, GX_WIDGET *parent, GX_RESOURCE_ID text_id,
                             ULONG style, USHORT radio_button_id, GX_CONST GX_RECTANGLE *size);
UINT _gx_radio_button_draw(GX_RADIO_BUTTON *button);
UINT _gx_radio_button_pixelmap_set(GX_RADIO_BUTTON *button, GX_RESOURCE_ID off_id, GX_RESOURCE_ID on_id,
                                   GX_RESOURCE_ID off_disabled_id, GX_RESOURCE_ID on_disabled_id);

UINT _gx_screen_stack_create(GX_SCREEN_STACK_CONTROL *control, GX_WIDGET **memory, INT size);
UINT _gx_screen_stack_push(GX_SCREEN_STACK_CONTROL *control, GX_WIDGET *screen, GX_WIDGET *new_screen);
UINT _gx_screen_stack_pop(GX_SCREEN_STACK_CONTROL *control);
UINT _gx_screen_stack_reset(GX_SCREEN_STACK_CONTROL *control);

UINT _gx_scroll_thumb_create(GX_SCROLL_THUMB *scroll_thumb, GX_SCROLLBAR *parent, ULONG style);
UINT _gx_scroll_thumb_draw(GX_SCROLL_THUMB *scroll_thumb);
UINT _gx_scroll_thumb_event_process(GX_SCROLL_THUMB *scroll_thumb, GX_EVENT *event_ptr);

UINT _gx_scrollbar_draw(GX_SCROLLBAR *scrollbar);
UINT _gx_scrollbar_event_process(GX_SCROLLBAR *scrollbar, GX_EVENT *event_ptr);
UINT _gx_scrollbar_limit_check(GX_SCROLLBAR *scrollbar);
UINT _gx_scrollbar_reset(GX_SCROLLBAR *scrollbar, GX_SCROLL_INFO *info);

UINT _gx_single_line_text_input_backspace(GX_SINGLE_LINE_TEXT_INPUT *text_input);
UINT _gx_single_line_text_input_buffer_clear(GX_SINGLE_LINE_TEXT_INPUT *text_input);
UINT _gx_single_line_text_input_buffer_get(GX_SINGLE_LINE_TEXT_INPUT *text_input_ptr, GX_CHAR **buffer_address,
                                           UINT *content_size, UINT *buffer_size);
UINT _gx_single_line_text_input_character_delete(GX_SINGLE_LINE_TEXT_INPUT *text_input);
UINT _gx_single_line_text_input_character_insert(GX_SINGLE_LINE_TEXT_INPUT *text_input, GX_UBYTE *str, UINT str_size);
UINT _gx_single_line_text_input_create(GX_SINGLE_LINE_TEXT_INPUT *text_input_ptr,
                                       GX_CONST GX_CHAR *name, GX_WIDGET *parent, GX_CHAR *input_buffer,
                                       UINT buffer_size, UINT style, USHORT text_input_id,
                                       GX_CONST GX_RECTANGLE *size);
UINT _gx_single_line_text_input_draw(GX_SINGLE_LINE_TEXT_INPUT *text_input);
UINT _gx_single_line_text_input_home(GX_SINGLE_LINE_TEXT_INPUT *text_input);
UINT _gx_single_line_text_input_end(GX_SINGLE_LINE_TEXT_INPUT *text_input);
UINT _gx_single_line_text_input_event_process(GX_SINGLE_LINE_TEXT_INPUT *text_input, GX_EVENT *event_ptr);
UINT _gx_single_line_text_input_left_arrow(GX_SINGLE_LINE_TEXT_INPUT *text_input);
UINT _gx_single_line_text_input_position_get(GX_SINGLE_LINE_TEXT_INPUT *text_input, UINT mouse_down_position);
UINT _gx_single_line_text_input_right_arrow(GX_SINGLE_LINE_TEXT_INPUT *text_input);
UINT _gx_single_line_text_input_style_add(GX_SINGLE_LINE_TEXT_INPUT *text_input_ptr, ULONG style);
UINT _gx_single_line_text_input_style_remove(GX_SINGLE_LINE_TEXT_INPUT *text_input_ptr, ULONG style);
UINT _gx_single_line_text_input_style_set(GX_SINGLE_LINE_TEXT_INPUT *text_input_ptr, ULONG style);

UINT _gx_slider_create(GX_SLIDER *slider, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                       INT tick_count, GX_SLIDER_INFO *slider_info, ULONG style, USHORT slider_id,
                       GX_CONST GX_RECTANGLE *size);
UINT _gx_slider_draw(GX_SLIDER *slider);
UINT _gx_slider_event_process(GX_SLIDER *slider, GX_EVENT *event_ptr);
UINT _gx_slider_info_set(GX_SLIDER *slider, GX_SLIDER_INFO *info);
UINT _gx_slider_needle_draw(GX_SLIDER *slider);
UINT _gx_slider_needle_position_get(GX_SLIDER *slider, GX_SLIDER_INFO *slider_info, GX_RECTANGLE *return_position);
UINT _gx_slider_tickmarks_draw(GX_SLIDER *slider);
UINT _gx_slider_travel_get(GX_SLIDER *widget, GX_SLIDER_INFO *info, INT *mintravel, INT *maxtravel);
UINT _gx_slider_value_calculate(GX_SLIDER *slider, GX_SLIDER_INFO *info, INT newpos);
UINT _gx_slider_value_set(GX_SLIDER *slider, GX_SLIDER_INFO *info, INT new_value);

UINT _gx_sprite_create(GX_SPRITE *sprite, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                       GX_SPRITE_FRAME *frame_list, USHORT framecount,
                       ULONG style, USHORT sprite_id, GX_CONST GX_RECTANGLE *size);
UINT _gx_sprite_current_frame_set(GX_SPRITE *sprite, USHORT frame);
UINT _gx_sprite_frame_list_set(GX_SPRITE *sprite, GX_SPRITE_FRAME *frame_list, USHORT frame_count);
UINT _gx_sprite_start(GX_SPRITE *sprite, USHORT start_frame);
UINT _gx_sprite_stop(GX_SPRITE *sprite);

UINT _gx_system_active_language_set(UINT language);
VOID _gx_system_canvas_refresh(VOID);
UINT _gx_system_dirty_mark(GX_WIDGET *widget);
UINT _gx_system_dirty_partial_add(GX_WIDGET *widget, GX_RECTANGLE *dirty_area);
UINT _gx_system_draw_context_get(GX_DRAW_CONTEXT **current_context);
UINT _gx_system_event_fold(GX_EVENT *in_event);
UINT _gx_system_event_send(GX_EVENT *in_event);
UINT _gx_system_focus_claim(GX_WIDGET *me);
UINT _gx_system_initialize(VOID);
UINT _gx_system_language_table_get(GX_CHAR ****language_table, UINT *languages_count, UINT *string_count);
UINT _gx_system_language_table_set(GX_CHAR ***language_table, UINT number_of_languages, UINT number_of_strings);
UINT _gx_system_memory_allocator_set(VOID *(*allocate)(ULONG size), VOID (*release)(VOID *));
UINT _gx_system_scroll_appearance_get(ULONG style, GX_SCROLLBAR_APPEARANCE *appearance);
UINT _gx_system_scroll_appearance_set(ULONG style, GX_SCROLLBAR_APPEARANCE *appearance);
UINT _gx_system_start(VOID);
UINT _gx_system_string_get(GX_RESOURCE_ID string_id, GX_CHAR **return_string);
UINT _gx_system_string_table_get(INT language, GX_CHAR ***get_table, UINT *get_size);
UINT _gx_system_string_width_get(GX_FONT *font, GX_CHAR *string, INT string_length, GX_VALUE *return_width);
UINT _gx_system_timer_start(GX_WIDGET *owner, UINT timer_id, UINT initial_ticks, UINT reschedule_ticks);
UINT _gx_system_timer_stop(GX_WIDGET *owner, UINT timer_id);
UINT _gx_system_version_string_get(GX_CHAR **return_string);
UINT _gx_system_widget_find(USHORT widget_id, INT search_level, GX_WIDGET **return_search_result);

UINT _gx_text_button_create(GX_TEXT_BUTTON *button, GX_CONST GX_CHAR *name,
                            GX_WIDGET *parent, GX_RESOURCE_ID text_id, ULONG style, USHORT Id,
                            GX_CONST GX_RECTANGLE *size);
UINT    _gx_text_button_draw(GX_TEXT_BUTTON *button);
UINT    _gx_text_button_font_set(GX_TEXT_BUTTON *button, GX_RESOURCE_ID font_id);
UINT    _gx_text_button_text_color_set(GX_TEXT_BUTTON *text_button, GX_RESOURCE_ID normal_text_color_id, GX_RESOURCE_ID selected_text_color_id);
UINT    _gx_text_button_text_draw(GX_TEXT_BUTTON *button);
UINT    _gx_text_button_text_get(GX_TEXT_BUTTON *button, GX_CHAR **return_text);
UINT    _gx_text_button_text_id_set(GX_TEXT_BUTTON *button, GX_RESOURCE_ID string_id);
UINT    _gx_text_button_text_set(GX_TEXT_BUTTON *button, GX_CHAR *text);

UINT    _gx_text_input_cursor_width_set(GX_TEXT_INPUT_CURSOR *cursor_input, GX_UBYTE width);
UINT    _gx_text_input_cursor_height_set(GX_TEXT_INPUT_CURSOR *cursor_input, GX_UBYTE height);
UINT    _gx_text_input_cursor_blink_interval_set(GX_TEXT_INPUT_CURSOR *cursor_input, GX_UBYTE blink_interval);

VOID    _gx_utility_ltoa(LONG value, GX_CHAR *return_buffer, UINT return_buffer_size);
INT     _gx_utility_math_acos(INT x);
INT     _gx_utility_math_asin(INT x);
INT     _gx_utility_math_cos(INT angle);
INT     _gx_utility_math_sin(INT angle);
UINT    _gx_utility_math_sqrt(UINT n);

VOID    _gx_utility_rectangle_center(GX_RECTANGLE *rectangle, GX_RECTANGLE *within);
VOID    _gx_utility_rectangle_center_find(GX_RECTANGLE *rectangle, GX_POINT *return_center);
GX_BOOL _gx_utility_rectangle_compare(GX_RECTANGLE *first_rectangle, GX_RECTANGLE *second_rectangle);
VOID    _gx_utility_rectangle_combine(GX_RECTANGLE *first_rectangle, GX_RECTANGLE *second_rectangle);
VOID    _gx_utility_rectangle_define(GX_RECTANGLE *rectangle, GX_VALUE left, GX_VALUE top, GX_VALUE right, GX_VALUE bottom);
VOID    _gx_utility_rectangle_resize(GX_RECTANGLE *rectangle, GX_VALUE adjust);
GX_BOOL _gx_utility_rectangle_overlap_detect(GX_RECTANGLE *first_rectangle, GX_RECTANGLE *second_rectangle, GX_RECTANGLE *return_overlap_area);
GX_BOOL _gx_utility_rectangle_point_detect(GX_RECTANGLE *rectangle, GX_POINT point);
VOID    _gx_utility_rectangle_shift(GX_RECTANGLE *rectangle, GX_VALUE x_shift, GX_VALUE y_shift);
UINT    _gx_utility_pixelmap_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, UINT *rot_cx, UINT *rot_cy);
UINT    _gx_utility_pixelmap_simple_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, UINT *rot_cx, UINT *rot_cy);
UINT    _gx_utility_pixelmap_resize(GX_PIXELMAP *src, GX_PIXELMAP *destination, INT width, INT height);

UINT    _gx_vertical_list_children_position(GX_VERTICAL_LIST *vertical_list);
UINT    _gx_vertical_list_create(GX_VERTICAL_LIST *vertical_list, GX_CONST GX_CHAR *name,  GX_WIDGET *parent,
                                 INT total_rows,
                                 VOID (*callback)(GX_VERTICAL_LIST *, GX_WIDGET *, INT),
                                 ULONG style, USHORT vertical_list_id,
                                 GX_CONST GX_RECTANGLE *size);
UINT _gx_vertical_list_event_process(GX_VERTICAL_LIST *list, GX_EVENT *event_ptr);
UINT _gx_vertical_list_total_rows_set(GX_VERTICAL_LIST *list, INT count);
UINT _gx_vertical_list_selected_index_get(GX_VERTICAL_LIST *vertical_list, INT *return_index);
UINT _gx_vertical_list_selected_widget_get(GX_VERTICAL_LIST *vertical_list, GX_WIDGET **return_list_entry);
UINT _gx_vertical_list_selected_set(GX_VERTICAL_LIST *vertical_list, INT index);
UINT _gx_vertical_scrollbar_create(GX_SCROLLBAR *scrollbar, GX_CONST GX_CHAR *name, GX_WINDOW *parent,
                                   GX_SCROLLBAR_APPEARANCE *settings, ULONG style);
UINT _gx_vertical_list_page_index_set(GX_VERTICAL_LIST *list, INT index);

UINT _gx_widget_allocate(GX_WIDGET **widget, ULONG memsize);
UINT _gx_widget_attach(GX_WIDGET *parent, GX_WIDGET *widget);
UINT _gx_widget_background_draw(GX_WIDGET *widget);
UINT _gx_widget_back_attach(GX_WIDGET *parent, GX_WIDGET *widget);
UINT _gx_widget_back_move(GX_WIDGET *widget, GX_BOOL *widget_moved);
UINT _gx_widget_block_move(GX_WIDGET *widget, GX_RECTANGLE *block, INT x_shift, INT y_shift);
UINT _gx_widget_border_draw(GX_WIDGET *widget, GX_COLOR border_color, GX_COLOR upper_color, GX_COLOR lower_color, GX_BOOL fill);
UINT _gx_widget_border_style_set(GX_WIDGET *widget, ULONG Style);
UINT _gx_widget_border_width_get(GX_WIDGET *widget, GX_VALUE *return_width);
UINT _gx_widget_canvas_get(GX_WIDGET *widget, GX_CANVAS **return_canvas);
UINT _gx_widget_child_detect(GX_WIDGET *parent, GX_WIDGET *child, GX_BOOL *return_detect);
UINT _gx_widget_children_draw(GX_WIDGET *widget);
UINT _gx_widget_client_get(GX_WIDGET *widget, GX_VALUE border_width, GX_RECTANGLE *return_size);
UINT _gx_widget_color_get(GX_WIDGET *widget, GX_RESOURCE_ID color_id, GX_COLOR *return_color);
UINT _gx_widget_create(GX_WIDGET *widget,
                       GX_CONST GX_CHAR *name,
                       GX_WIDGET *parent,
                       ULONG style, USHORT Id,
                       GX_CONST GX_RECTANGLE *size);
UINT _gx_widget_created_test(GX_WIDGET *widget, GX_BOOL *return_test);
UINT _gx_widget_delete(GX_WIDGET *widget);
UINT _gx_widget_detach(GX_WIDGET *widget);
UINT _gx_widget_draw(GX_WIDGET *widget);
UINT _gx_widget_draw_set(GX_WIDGET *widget, VOID (*draw_func)(GX_WIDGET *));
UINT _gx_widget_event_generate(GX_WIDGET *widget, USHORT event_type, LONG value);
UINT _gx_widget_event_process(GX_WIDGET *widget, GX_EVENT *event_ptr);
UINT _gx_widget_event_process_set(GX_WIDGET *widget, UINT (*event_processing_function)(GX_WIDGET *, GX_EVENT *));
UINT _gx_widget_event_to_parent(GX_WIDGET *widget, GX_EVENT *event_ptr);
UINT _gx_widget_fill_color_set(GX_WIDGET *widget, GX_RESOURCE_ID normal_color, GX_RESOURCE_ID selected_color);
UINT _gx_widget_find(GX_WIDGET *parent, USHORT widget_id, INT search_depth, GX_WIDGET **return_widget);
UINT _gx_widget_font_get(GX_WIDGET *widget, GX_RESOURCE_ID font_id, GX_FONT **return_font);
UINT _gx_widget_free(GX_WIDGET *widget);
UINT _gx_widget_front_move(GX_WIDGET *widget, GX_BOOL *widget_moved);
UINT _gx_widget_focus_next(GX_WIDGET *widget);
UINT _gx_widget_focus_previous(GX_WIDGET *widget);
UINT _gx_widget_height_get(GX_WIDGET *widget, GX_VALUE *return_height);
UINT _gx_widget_hide(GX_WIDGET *widget);
UINT _gx_widget_pixelmap_get(GX_WIDGET *widget, GX_RESOURCE_ID font_id, GX_PIXELMAP **return_pixelmap);

UINT _gx_widget_resize(GX_WIDGET *widget, GX_RECTANGLE *newsize);
UINT _gx_widget_shift(GX_WIDGET *widget, GX_VALUE x_shift, GX_VALUE y_shift, GX_BOOL mark_dirty);
UINT _gx_widget_show(GX_WIDGET *widget);
UINT _gx_widget_status_add(GX_WIDGET *widget, ULONG status);
UINT _gx_widget_status_get(GX_WIDGET *widget, ULONG *return_status);
UINT _gx_widget_status_remove(GX_WIDGET *widget, ULONG status);
UINT _gx_widget_status_test(GX_WIDGET *widget, ULONG status, GX_BOOL *return_test);
UINT _gx_widget_style_add(GX_WIDGET *widget, ULONG style);
UINT _gx_widget_style_get(GX_WIDGET *widget, ULONG *return_style);
UINT _gx_widget_style_remove(GX_WIDGET *widget, ULONG style);
UINT _gx_widget_style_set(GX_WIDGET *widget, ULONG style);

UINT _gx_widget_text_blend(GX_WIDGET *widget, UINT tColor, UINT font_id, GX_CHAR *string,
                           INT x_offset, INT y_offset, UCHAR alpha);

UINT _gx_widget_text_draw(GX_WIDGET *widget, UINT tColor, UINT font_id, GX_CHAR *string,
                          INT x_offset, INT y_offset);

UINT _gx_widget_text_id_draw(GX_WIDGET *widget, UINT tColor,
                             UINT font_id, UINT text_id,
                             INT x_offset, INT y_offset);

UINT _gx_widget_type_find(GX_WIDGET *parent, USHORT widget_type, GX_WIDGET **return_widget);
UINT _gx_widget_width_get(GX_WIDGET *widget, GX_VALUE *return_width);

UINT _gx_window_client_height_get(GX_WINDOW *window, GX_VALUE *return_height);
UINT _gx_window_client_scroll(GX_WINDOW *window, GX_VALUE x_scroll, GX_VALUE y_scroll);
UINT _gx_window_client_width_get(GX_WINDOW *window, GX_VALUE *return_width);
UINT _gx_window_close(GX_WINDOW *window);
UINT _gx_window_create(GX_WINDOW *window,
                       GX_CONST GX_CHAR *name,
                       GX_WIDGET *parent,
                       ULONG style, USHORT window_id,
                       GX_CONST GX_RECTANGLE *size);
UINT _gx_window_draw(GX_WINDOW *window);
UINT _gx_window_event_process(GX_WINDOW *window, GX_EVENT *event_ptr);
UINT _gx_window_execute(GX_WINDOW *window, ULONG *return_code); 
UINT _gx_window_root_create(GX_WINDOW_ROOT *root_window,
                            GX_CONST GX_CHAR *name,
                            GX_CANVAS *canvas, ULONG style, USHORT Id,
                            GX_CONST GX_RECTANGLE *size);
UINT _gx_window_root_delete(GX_WINDOW_ROOT *root_window);
UINT _gx_window_root_event_process(GX_WINDOW_ROOT *widget, GX_EVENT *event_ptr);
UINT _gx_window_root_find(GX_WIDGET *widget, GX_WINDOW_ROOT **return_root_window);
UINT _gx_window_scroll_info_get(GX_WINDOW *window, ULONG type, GX_SCROLL_INFO *return_scroll_info);
UINT _gx_window_scrollbar_find(GX_WINDOW *window, USHORT type, GX_SCROLLBAR **return_scrollbar);
UINT _gx_window_wallpaper_get(GX_WINDOW *window, GX_RESOURCE_ID *return_wallpaper_id);
UINT _gx_window_wallpaper_set(GX_WINDOW *window, GX_RESOURCE_ID wallpaper_id, GX_BOOL tile);

#else
#define gx_animation_canvas_define                               _gxe_animation_canvas_define
#define gx_animation_create                                      _gxe_animation_create
#define gx_animatin_delete                                       _gxe_animation_delete
#define gx_animation_start                                       _gxe_animation_start
#define gx_animation_stop                                        _gxe_animation_stop

#define gx_brush_default                                         _gxe_brush_default
#define gx_brush_define                                          _gxe_brush_define

#define gx_button_background_draw                                _gxe_button_background_draw
#define gx_button_create(a, b, c, d, e, f)                       _gxe_button_create(a, b, (GX_WIDGET *)c, d, e, f, sizeof(GX_BUTTON))
#define gx_button_deselect                                       _gxe_button_deselect
#define gx_button_draw                                           _gxe_button_draw
#define gx_button_event_process                                  _gxe_button_event_process
#define gx_button_select                                         _gxe_button_select

#define gx_canvas_alpha_set                                      _gxe_canvas_alpha_set
#define gx_canvas_block_move(a, b, c, d)                         _gxe_canvas_block_move(a, b, c, d)
#define gx_canvas_create(a, b, c, d, e, f, g, h)                 _gxe_canvas_create(a, b, c, d, e, f, g, h, sizeof(GX_CANVAS))
#define gx_canvas_delete                                         _gxe_canvas_delete
#define gx_canvas_drawing_complete                               _gxe_canvas_drawing_complete
#define gx_canvas_drawing_initiate(a, b, c)                      _gxe_canvas_drawing_initiate(a, (GX_WIDGET *)b, c)
#define gx_canvas_line_draw                                      _gxe_canvas_line_draw
#define gx_canvas_offset_set                                     _gxe_canvas_offset_set
#define gx_canvas_pixel_draw                                     _gxe_canvas_pixel_draw
#define gx_canvas_pixelmap_blend                                 _gxe_canvas_pixelmap_blend
#define gx_canvas_pixelmap_draw                                  _gxe_canvas_pixelmap_draw
#define gx_canvas_pixelmap_tile                                  _gxe_canvas_pixelmap_tile
#define gx_canvas_pixelmap_rotate                                _gxe_canvas_pixelmap_rotate
#define gx_canvas_polygon_draw                                   _gxe_canvas_polygon_draw
#define gx_canvas_circle_draw                                    _gxe_canvas_circle_draw
#define gx_canvas_arc_draw                                       _gxe_canvas_arc_draw
#define gx_canvas_ellipse_draw                                   _gxe_canvas_ellipse_draw
#define gx_canvas_pie_draw                                       _gxe_canvas_pie_draw
#define gx_canvas_rectangle_draw                                 _gxe_canvas_rectangle_draw
#define gx_canvas_shift                                          _gxe_canvas_shift
#define gx_canvas_text_draw                                      _gxe_canvas_text_draw

#define gx_checkbox_create(a, b, c, d, e, f, g)                  _gxe_checkbox_create(a, b, (GX_WIDGET *)c, d, e, f, g, sizeof(GX_CHECKBOX))
#define gx_checkbox_draw                                         _gxe_checkbox_draw
#define gx_checkbox_event_process                                _gxe_checkbox_event_process
#define gx_checkbox_pixelmap_set                                 _gxe_checkbox_pixelmap_set
#define gx_checkbox_select                                       _gxe_checkbox_select

#define gx_context_brush_default                                 _gxe_context_brush_default
#define gx_context_brush_define                                  _gxe_context_brush_define
#define gx_context_brush_get                                     _gxe_context_brush_get
#define gx_context_brush_pattern_set                             _gxe_context_brush_pattern_set
#define gx_context_brush_set                                     _gxe_context_brush_set
#define gx_context_brush_style_set                               _gxe_context_brush_style_set
#define gx_context_brush_width_set                               _gxe_context_brush_width_set
#define gx_context_color_get                                     _gxe_context_color_get
#define gx_context_fill_color_set                                _gxe_context_fill_color_set
#define gx_context_font_get                                      _gxe_context_font_get
#define gx_context_font_set                                      _gxe_context_font_set
#define gx_context_line_color_set                                _gxe_context_line_color_set
#define gx_context_pixelmap_get                                  _gxe_context_pixelmap_get
#define gx_context_pixelmap_set                                  _gxe_context_pixelmap_set
#define gx_context_raw_brush_define                              _gxe_context_raw_brush_define
#define gx_context_raw_fill_color_set                            _gxe_context_raw_fill_color_set
#define gx_context_raw_line_color_set                            _gxe_context_raw_line_color_set

#define gx_circular_gauge_angle_get                              _gxe_circular_gauge_angle_get
#define gx_circular_gauge_angle_set                              _gxe_circular_gauge_angle_set
#define gx_circular_gauge_animation_set                          _gxe_circular_gauge_animation_set
#define gx_circular_gauge_background_draw                        _gxe_circular_gauge_background_draw
#define gx_circular_gauge_create(a, b, c, d, e, f, g, h, i)      _gxe_circular_gauge_create((GX_CIRCULAR_GAUGE *) a, b, c, d, e, f, g, h, i, sizeof(GX_CIRCULAR_GAUGE))
#define gx_circular_gauge_draw                                   _gxe_circular_gauge_draw
#define gx_circular_gauge_event_process                          _gxe_circular_gauge_event_process

#define gx_display_create(a, b, c, d, e)                         _gxe_display_create(a, b, c, d, e, sizeof(GX_DISPLAY))
#define gx_display_delete                                        _gxe_display_delete
#define gx_display_color_table_set                               _gxe_display_color_table_set
#define gx_display_font_table_set                                _gxe_display_font_table_set
#define gx_display_pixelmap_table_set                            _gxe_display_pixelmap_table_set

#define gx_drop_list_close                                       _gxe_drop_list_close
#define gx_drop_list_create(a, b, c, d, e, f, g, h, i)           _gxe_drop_list_create((GX_DROP_LIST *)a, b, (GX_WIDGET *)c, d, e, f, g, h, i)
#define gx_drop_list_event_process                               _gxe_drop_list_event_process
#define gx_drop_list_open                                        _gxe_drop_list_open
#define gx_drop_list_pixelmap_set                                _gxe_drop_list_pixelmap_set
#define gx_drop_list_popup_get                                   _gxe_drop_list_popup_get

#define gx_horizontal_list_children_position                     _gxe_horizontal_list_children_position
#define gx_horizontal_list_create(a, b, c, d, e, f, g, h)        _gxe_horizontal_list_create(a, b, (GX_WIDGET *)c, d, e, f, g, h, sizeof(GX_HORIZONTAL_LIST))
#define gx_horizontal_list_event_process                         _gxe_horizontal_list_event_process
#define gx_horizontal_list_selected_index_get(a, b)              _gxe_horizontal_list_selected_index_get(a, b)
#define gx_horizontal_list_selected_widget_get(a, b)             _gxe_horizontal_list_selected_widget_get(a, (GX_WIDGET **)b)
#define gx_horizontal_list_selected_set(a, b)                    _gxe_horizontal_list_selected_set(a, b)
#define gx_horizontal_list_total_columns_set(a, b)               _gxe_horizontal_list_total_columns_set((GX_HORIZONTAL_LIST *)a, b)
#define gx_horizontal_list_page_index_set(a, b)                  _gxe_horizontal_list_page_index_set((GX_HORIZONTAL_LIST *)a, b)

#define gx_horizontal_scrollbar_create(a, b, c, d, e)            _gxe_horizontal_scrollbar_create(a, b, (GX_WINDOW *)c, d, e, sizeof(GX_SCROLLBAR))

#define gx_icon_background_draw                                  _gxe_icon_background_draw
#define gx_icon_button_create(a, b, c, d, e, f, g)               _gxe_icon_button_create(a, b, (GX_WIDGET *)c, d, e, f, g, sizeof(GX_ICON_BUTTON))
#define gx_icon_button_pixelmap_set                              _gxe_icon_button_pixelmap_set
#define gx_icon_create(a, b, c, d, e, f, g, h)                   _gxe_icon_create(a, b, (GX_WIDGET *)c, d, e, f, g, h, sizeof(GX_ICON))
#define gx_icon_draw                                             _gxe_icon_draw
#define gx_icon_event_process                                    _gxe_icon_event_process
#define gx_icon_pixelmap_set                                     _gxe_icon_pixelmap_set

#define gx_image_reader_create                                   _gxe_image_reader_create
#define gx_image_reader_palette_set                              _gxe_image_reader_palette_set
#define gx_image_reader_start                                    _gxe_image_reader_start

#define gx_line_chart_axis_draw                                  _gxe_line_chart_axis_draw
#define gx_line_chart_create(a, b, c, d, e, f, g)                _gxe_line_chart_create(a, b, (GX_WIDGET *) c, d, e, f, g, sizeof(GX_LINE_CHART))
#define gx_line_chart_data_draw                                  _gxe_line_chart_data_draw
#define gx_line_chart_draw                                       _gxe_line_chart_draw
#define gx_line_chart_update                                     _gxe_line_chart_update
#define gx_line_chart_y_scale_calculate                          _gxe_line_chart_y_scale_calculate

#define gx_multi_line_text_button_create(a, b, c, d, e, f, g)    _gxe_multi_line_text_button_create(a, b, (GX_WIDGET *)c, d, e, f, g, sizeof(GX_MULTI_LINE_TEXT_BUTTON))
#define gx_multi_line_text_button_draw                           _gxe_multi_line_text_button_draw
#define gx_multi_line_text_button_event_process                  _gxe_multi_line_text_button_event_process
#define gx_multi_line_text_button_text_draw                      _gxe_multi_line_text_button_text_draw
#define gx_multi_line_text_button_text_id_set                    _gxe_multi_line_text_button_text_id_set
#define gx_multi_line_text_button_text_set                       _gxe_multi_line_text_button_text_set

#define gx_multi_line_text_view_char_index_set                   _gxe_multi_line_text_view_char_index_set
#define gx_multi_line_text_input_create(a, b, c, d, e, f, g, h)  _gxe_multi_line_text_input_create(a, b, (GX_WIDGET *)c, d, e, f, g, h, sizeof(GX_MULTI_LINE_TEXT_INPUT))
#define gx_multi_line_text_input_style_remove                    _gxe_multi_line_text_input_style_remove
#define gx_multi_line_text_input_style_add                       _gxe_multi_line_text_input_style_add
#define gx_multi_line_text_input_style_set                       _gxe_multi_line_text_input_style_set
#define gx_multi_line_text_input_buffer_get                      _gxe_multi_line_text_input_buffer_get
#define gx_multi_line_text_input_buffer_clear                    _gxe_multi_line_text_input_buffer_clear

#define gx_multi_line_text_view_create(a, b, c, d, e, f, g)      _gxe_multi_line_text_view_create(a, b, (GX_WIDGET *)c, d, e, f, g, sizeof(GX_MULTI_LINE_TEXT_VIEW))
#define gx_multi_line_text_view_event_process                    _gxe_multi_line_text_view_event_process
#define gx_multi_line_text_view_font_set                         _gxe_multi_line_text_view_font_set
#define gx_multi_line_text_view_scroll                           _gxe_multi_line_text_view_scroll
#define gx_multi_line_text_view_text_color_set                   _gxe_multi_line_text_view_text_color_set
#define gx_multi_line_text_view_text_id_set                      _gxe_multi_line_text_view_text_id_set
#define gx_multi_line_text_view_text_set                         _gxe_multi_line_text_view_text_set

#define gx_pixelmap_button_create(a, b, c, d, e, f, g, h, i)     _gxe_pixelmap_button_create(a, b, (GX_WIDGET *)c, d, e, f, g, h, i, sizeof(GX_PIXELMAP_BUTTON))
#define gx_pixelmap_button_pixelmap_set                          _gxe_pixelmap_button_pixelmap_set
#define gx_pixelmap_button_draw                                  _gxe_pixelmap_button_draw
#define gx_pixelmap_button_event_process                         _gxe_pixelmap_button_event_process

#define gx_pixelmap_prompt_create(a, b, c, d, e, f, g, h)        _gxe_pixelmap_prompt_create(a, b, (GX_WIDGET *)c, d, e, f, g, h, sizeof(GX_PIXELMAP_PROMPT))
#define gx_pixelmap_prompt_draw                                  _gxe_pixelmap_prompt_draw
#define gx_pixelmap_prompt_pixelmap_set                          _gxe_pixelmap_prompt_pixelmap_set

#define gx_pixelmap_slider_create(a, b, c, d, e, f, g, h)        _gxe_pixelmap_slider_create(a, b, (GX_WIDGET *)c, d, e, f, g, h, sizeof(GX_PIXELMAP_SLIDER))
#define gx_pixelmap_slider_draw                                  _gxe_pixelmap_slider_draw
#define gx_pixelmap_slider_event_process                         _gxe_pixelmap_slider_event_process
#define gx_pixelmap_slider_pixelmap_set                          _gxe_pixelmap_slider_pixelmap_set

#define gx_progress_bar_create(a, b, c, d, e, f, g)              _gxe_progress_bar_create(a, b, (GX_WIDGET *)c, d, e, f, g, sizeof(GX_PROGRESS_BAR))
#define gx_progress_bar_draw                                     _gxe_progress_bar_draw
#define gx_progress_bar_event_process                            _gxe_progress_bar_event_process
#define gx_progress_bar_font_set                                 _gxe_progress_bar_font_set
#define gx_progress_bar_info_set(a, b)                           _gxe_progress_bar_info_set((GX_PROGRESS_BAR *)a, b);
#define gx_progress_bar_pixelmap_set                             _gxe_progress_bar_pixelmap_set
#define gx_progress_bar_range_set                                _gxe_progress_bar_range_set
#define gx_progress_bar_text_color_set                           _gxe_progress_bar_text_color_set
#define gx_progress_bar_value_set                                _gxe_progress_bar_value_set

#define gx_prompt_create(a, b, c, d, e, f, g)                    _gxe_prompt_create(a, b, (GX_WIDGET *)c, d, e, f, g, sizeof(GX_PROMPT))
#define gx_prompt_draw                                           _gxe_prompt_draw
#define gx_prompt_font_set                                       _gxe_prompt_font_set
#define gx_prompt_text_color_set                                 _gxe_prompt_text_color_set
#define gx_prompt_text_draw                                      _gxe_prompt_text_draw
#define gx_prompt_text_get                                       _gxe_prompt_text_get
#define gx_prompt_text_set                                       _gxe_prompt_text_set
#define gx_prompt_text_id_set                                    _gxe_prompt_text_id_set

#define gx_radial_progress_bar_anchor_set                        _gxe_radial_progress_bar_anchor_set
#define gx_radial_progress_bar_background_draw                   _gxe_radial_progress_bar_background_draw
#define gx_radial_progress_bar_create(a, b, c, d, e, f)          _gxe_radial_progress_bar_create(a, b, c, d, e, f, sizeof(GX_RADIAL_PROGRESS_BAR))
#define gx_radial_progress_bar_draw                              _gxe_radial_progress_bar_draw
#define gx_radial_progress_bar_event_process                     _gxe_radial_progress_bar_event_process
#define gx_radial_progress_bar_font_set                          _gxe_radial_progress_bar_font_set
#define gx_radial_progress_bar_info_set                          _gxe_radial_progress_bar_info_set
#define gx_radial_progress_bar_text_color_set                    _gxe_radial_progress_bar_text_color_set
#define gx_radial_progress_bar_text_draw                         _gxe_radial_progress_bar_text_draw
#define gx_radial_progress_bar_value_set                         _gxe_radial_progress_bar_value_set

#define gx_radio_button_create(a, b, c, d, e, f, g)              _gxe_radio_button_create(a, b, (GX_WIDGET *)c, d, e, f, g, sizeof(GX_RADIO_BUTTON))
#define gx_radio_button_draw                                     _gxe_radio_button_draw
#define gx_radio_button_pixelmap_set                             _gxe_radio_button_pixelmap_set

#define gx_screen_stack_create                                   _gxe_screen_stack_create
#define gx_screen_stack_push                                     _gxe_screen_stack_push
#define gx_screen_stack_pop                                      _gxe_screen_stack_pop
#define gx_screen_stack_reset                                    _gxe_screen_stack_reset

#define gx_scroll_thumb_create(a, b, c)                          _gxe_scroll_thumb_create(a, b, c, sizeof(GX_SCROLL_THUMB))
#define gx_scroll_thumb_draw                                     _gxe_scroll_thumb_draw
#define gx_scroll_thumb_event_process                            _gxe_scroll_thumb_event_process

#define gx_scrollbar_draw                                        _gxe_scrollbar_draw
#define gx_scrollbar_event_process                               _gxe_scrollbar_event_process
#define gx_scrollbar_limit_check                                 _gxe_scrollbar_limit_check
#define gx_scrollbar_reset                                       _gxe_scrollbar_reset

#define gx_single_line_text_input_backspace(a)                   _gxe_single_line_text_input_backspace((GX_SINGLE_LINE_TEXT_INPUT *)a)
#define gx_single_line_text_input_buffer_clear(a)                _gxe_single_line_text_input_buffer_clear((GX_SINGLE_LINE_TEXT_INPUT *)a);
#define gx_single_line_text_input_buffer_get(a, b, c, d)         _gxe_single_line_text_input_buffer_get((GX_SINGLE_LINE_TEXT_INPUT *)a, b, c, d);
#define gx_single_line_text_input_character_delete(a)            _gxe_single_line_text_input_character_delete(a)
#define gx_single_line_text_input_character_insert(a, b, c)      _gxe_single_line_text_input_character_insert(a, b, c)
#define gx_single_line_text_input_create(a, b, c, d, e, f, g, h) _gxe_single_line_text_input_create(a, b, (GX_WIDGET *)c, d, e, f, g, h, sizeof(GX_SINGLE_LINE_TEXT_INPUT))
#define gx_single_line_text_input_draw(a)                        _gxe_single_line_text_input_draw(a)
#define gx_single_line_text_input_end(a)                         _gxe_single_line_text_input_end((GX_SINGLE_LINE_TEXT_INPUT *)a)
#define gx_single_line_text_input_event_process(a, b)            _gxe_single_line_text_input_event_process((GX_SINGLE_LINE_TEXT_INPUT *)a, b)
#define gx_single_line_text_input_home(a)                        _gxe_single_line_text_input_home((GX_SINGLE_LINE_TEXT_INPUT *)a)
#define gx_single_line_text_input_left_arrow(a)                  _gxe_single_line_text_input_left_arrow((GX_SINGLE_LINE_TEXT_INPUT *)a)
#define gx_single_line_text_input_position_get(a, b)             _gxe_single_line_text_input_position_get(a, b)
#define gx_single_line_text_input_right_arrow(a)                 _gxe_single_line_text_input_right_arrow((GX_SINGLE_LINE_TEXT_INPUT *)a)
#define gx_single_line_text_input_style_add(a, b)                _gxe_single_line_text_input_style_add((GX_SINGLE_LINE_TEXT_INPUT *)a, b)
#define gx_single_line_text_input_style_remove(a, b)             _gxe_single_line_text_input_style_remove((GX_SINGLE_LINE_TEXT_INPUT *)a, b)
#define gx_single_line_text_input_style_set(a, b)                _gxe_single_line_text_input_style_set((GX_SINGLE_LINE_TEXT_INPUT *)a, b)

#define gx_slider_create(a, b, c, d, e, f, g, h)                 _gxe_slider_create(a, b, (GX_WIDGET *)c, d, e, f, g, h, sizeof(GX_SLIDER))
#define gx_slider_draw                                           _gxe_slider_draw
#define gx_slider_event_process                                  _gxe_slider_event_process
#define gx_slider_info_set                                       _gxe_slider_info_set
#define gx_slider_needle_draw                                    _gxe_slider_needle_draw
#define gx_slider_needle_position_get                            _gxe_slider_needle_position_get
#define gx_slider_tickmarks_draw                                 _gxe_slider_tickmarks_draw
#define gx_slider_travel_get                                     _gxe_slider_travel_get
#define gx_slider_value_calculate                                _gxe_slider_value_calculate
#define gx_slider_value_set                                      _gxe_slider_value_set

#define gx_sprite_create(a, b, c, d, e, f, g, h)                 _gxe_sprite_create(a, b, (GX_WIDGET *)c, d, e, f, g, h)
#define gx_sprite_current_frame_set                              _gxe_sprite_current_frame_set
#define gx_sprite_frame_list_set                                 _gxe_sprite_frame_list_set
#define gx_sprite_start(a, b)                                    _gxe_sprite_start(a, b)
#define gx_sprite_stop(a)                                        _gxe_sprite_stop(a)

#define gx_system_active_language_set                            _gxe_system_active_language_set
#define gx_system_canvas_refresh                                 _gx_system_canvas_refresh
#define gx_system_dirty_mark(a)                                  _gxe_system_dirty_mark((GX_WIDGET *)a);
#define gx_system_dirty_partial_add(a, b)                        _gxe_system_dirty_partial_add((GX_WIDGET *)a, b)
#define gx_system_draw_context_get                               _gxe_system_draw_context_get
#define gx_system_event_fold                                     _gxe_system_event_fold
#define gx_system_event_send                                     _gxe_system_event_send
#define gx_system_focus_claim(a)                                 _gxe_system_focus_claim((GX_WIDGET *)a)
#define gx_system_initialize                                     _gxe_system_initialize
#define gx_system_language_table_get                             _gxe_system_language_table_get
#define gx_system_language_table_set                             _gxe_system_language_table_set
#define gx_system_memory_allocator_set                           _gxe_system_memory_allocator_set
#define gx_system_scroll_appearance_get                          _gxe_system_scroll_appearance_get
#define gx_system_scroll_appearance_set                          _gxe_system_scroll_appearance_set
#define gx_system_start                                          _gxe_system_start
#define gx_system_string_get                                     _gxe_system_string_get
#define gx_system_string_table_get                               _gxe_system_string_table_get
#define gx_system_string_width_get                               _gxe_system_string_width_get
#define gx_system_timer_start(a, b, c, d)                        _gxe_system_timer_start((GX_WIDGET *)a, b, c, d)
#define gx_system_timer_stop(a, b)                               _gxe_system_timer_stop((GX_WIDGET *)a, b)
#define gx_system_version_string_get                             _gxe_system_version_string_get
#define gx_system_widget_find                                    _gxe_system_widget_find

#define gx_text_button_create(a, b, c, d, e, f, g)               _gxe_text_button_create(a, b, (GX_WIDGET *)c, d, e, f, g, sizeof(GX_TEXT_BUTTON))
#define gx_text_button_draw                                      _gxe_text_button_draw
#define gx_text_button_font_set                                  _gxe_text_button_font_set
#define gx_text_button_text_draw                                 _gxe_text_button_text_draw
#define gx_text_button_text_color_set                            _gxe_text_button_text_color_set
#define gx_text_button_text_get                                  _gxe_text_button_text_get
#define gx_text_button_text_id_set                               _gxe_text_button_text_id_set
#define gx_text_button_text_set                                  _gxe_text_button_text_set

#define gx_text_input_cursor_height_set(a, b)                    _gxe_text_input_cursor_height_set((GX_TEXT_INPUT_CURSOR *)a, b)
#define gx_text_input_cursor_width_set(a, b)                     _gxe_text_input_cursor_width_set((GX_TEXT_INPUT_CURSOR *)a, b)
#define gx_text_input_cursor_blink_interval_set(a, b)            _gxe_text_input_cursor_blink_interval_set((GX_TEXT_INPUT_CURSOR *)a, b)

#define gx_transition_window_create(a, b, c, d, e, f)            _gxe_transition_window_create(a, b, (GX_WIDGET *)c, d, e, f, sizeof(GX_WINDOW))

#define gx_utility_ltoa                                          _gxe_utility_ltoa
#define gx_utility_math_acos                                     _gxe_utility_math_acos
#define gx_utility_math_asin                                     _gxe_utility_math_asin
#define gx_utility_math_cos                                      _gx_utility_math_cos
#define gx_utility_math_sin                                      _gx_utility_math_sin
#define gx_utility_math_sqrt                                     _gx_utility_math_sqrt

#define gx_utility_rectangle_center                              _gxe_utility_rectangle_center
#define gx_utility_rectangle_center_find                         _gxe_utility_rectangle_center_find
#define gx_utility_rectangle_combine                             _gxe_utility_rectangle_combine
#define gx_utility_rectangle_compare                             _gxe_utility_rectangle_compare
#define gx_utility_rectangle_define                              _gxe_utility_rectangle_define
#define gx_utility_rectangle_resize                              _gxe_utility_rectangle_resize
#define gx_utility_rectangle_overlap_detect                      _gxe_utility_rectangle_overlap_detect
#define gx_utility_rectangle_point_detect                        _gxe_utility_rectangle_point_detect
#define gx_utility_rectangle_shift                               _gxe_utility_rectangle_shift
#define gx_utility_pixelmap_rotate                               _gxe_utility_pixelmap_rotate
#define gx_utility_pixelmap_simple_rotate                        _gx_utility_pixelmap_simple_rotate
#define gx_utility_pixelmap_resize                               _gxe_utility_pixelmap_resize

#define gx_vertical_list_children_position                       _gxe_vertical_list_children_position
#define gx_vertical_list_create(a, b, c, d, e, f, g, h)          _gxe_vertical_list_create(a, b, (GX_WIDGET *)c, d, e, f, g, h, sizeof(GX_VERTICAL_LIST))
#define gx_vertical_list_event_process                           _gxe_vertical_list_event_process
#define gx_vertical_list_selected_index_get(a, b)                _gxe_vertical_list_selected_index_get(a, b)
#define gx_vertical_list_selected_widget_get(a, b)               _gxe_vertical_list_selected_widget_get(a, (GX_WIDGET **)b)
#define gx_vertical_list_selected_set(a, b)                      _gxe_vertical_list_selected_set(a, b)
#define gx_vertical_list_total_rows_set(a, b)                    _gxe_vertical_list_total_rows_set((GX_VERTICAL_LIST *)a, b)
#define gx_vertical_list_page_index_set(a, b)                    _gxe_vertical_list_page_index_set((GX_VERTICAL_LIST *)a, b)

#define gx_vertical_scrollbar_create(a, b, c, d, e)              _gxe_vertical_scrollbar_create(a, b, (GX_WINDOW *)c, d, e, sizeof(GX_SCROLLBAR))

#define gx_widget_allocate(a, b)                                 _gxe_widget_allocate((GX_WIDGET **)a, b)
#define gx_widget_attach(a, b)                                   _gxe_widget_attach((GX_WIDGET *)a, (GX_WIDGET *)b)
#define gx_widget_back_attach(a, b)                              _gxe_widget_back_attach((GX_WIDGET *)a, (GX_WIDGET *)b)
#define gx_widget_back_move(a, b)                                _gxe_widget_back_move((GX_WIDGET *)a, b)
#define gx_widget_background_draw(a)                             _gxe_widget_background_draw((GX_WIDGET *)a)
#define gx_widget_block_move(a, b, c, d)                         _gxe_widget_block_move((GX_WIDGET *)a, b, c, d)
#define gx_widget_border_draw(a, b, c, d, e)                     _gxe_widget_border_draw((GX_WIDGET *)a, b, c, d, e)
#define gx_widget_border_style_set(a, b)                         _gxe_widget_border_style_set((GX_WIDGET *)a, b)
#define gx_widget_border_width_get(a, b)                         _gxe_widget_border_width_get((GX_WIDGET *)a, b)
#define gx_widget_canvas_get(a, b)                               _gxe_widget_canvas_get((GX_WIDGET *)a, b)
#define gx_widget_child_detect(a, b, c)                          _gxe_widget_child_detect((GX_WIDGET *)a, b, c)
#define gx_widget_children_draw(a)                               _gxe_widget_children_draw((GX_WIDGET *)a)
#define gx_widget_color_get(a, b, c)                             _gxe_widget_color_get((GX_WIDGET *)a, b, c)
#define gx_widget_client_get(a, b, c)                            _gxe_widget_client_get((GX_WIDGET *)a, b, c)
#define gx_widget_create(a, b, c, d, e, f)                       _gxe_widget_create((GX_WIDGET *)a, b, (GX_WIDGET *)c, d, e, f)
#define gx_widget_created_test(a, b)                             _gxe_widget_created_test((GX_WIDGET *)a, b)
#define gx_widget_delete(a)                                      _gxe_widget_delete((GX_WIDGET *)a)
#define gx_widget_detach(a)                                      _gxe_widget_detach((GX_WIDGET *)a)
#define gx_widget_draw(a)                                        _gxe_widget_draw((GX_WIDGET *)a)
#define gx_widget_draw_set(a, b)                                 _gxe_widget_draw_set((GX_WIDGET *)a, (void (*)(GX_WIDGET *))b)
#define gx_widget_event_generate(a, b, c)                        _gxe_widget_event_generate((GX_WIDGET *)a, b, c)
#define gx_widget_event_process(a, b)                            _gxe_widget_event_process((GX_WIDGET *)a, b)
#define gx_widget_event_process_set(a, b)                        _gxe_widget_event_process_set((GX_WIDGET *)a, (UINT (*)(GX_WIDGET *, GX_EVENT *)) b)
#define gx_widget_event_to_parent(a, b)                          _gxe_widget_event_to_parent((GX_WIDGET *)a, b)
#define gx_widget_fill_color_set(a, b, c)                        _gxe_widget_fill_color_set((GX_WIDGET *)a, b, c)
#define gx_widget_find(a, b, c, d)                               _gxe_widget_find((GX_WIDGET *)a, b, c, (GX_WIDGET **)d)
#define gx_widget_font_get(a, b, c)                              _gxe_widget_font_get((GX_WIDGET *)a, b, c)
#define gx_widget_free(a)                                        _gxe_widget_free((GX_WIDGET *)a)
#define gx_widget_front_move(a, b)                               _gxe_widget_front_move((GX_WIDGET *)a, b)
#define gx_widget_focus_next(a)                                  _gxe_widget_focus_next((GX_WIDGET *)a)
#define gx_widget_focus_previous(a)                              _gxe_widget_focus_previous((GX_WIDGET *)a)
#define gx_widget_height_get(a, b)                               _gxe_widget_height_get((GX_WIDGET *)a, b)
#define gx_widget_hide(a)                                        _gxe_widget_hide((GX_WIDGET *)a)
#define gx_widget_pixelmap_get(a, b, c)                          _gxe_widget_pixelmap_get((GX_WIDGET *)a, b, c)
#define gx_widget_resize(a, b)                                   _gxe_widget_resize((GX_WIDGET *)a, b)
#define gx_widget_shift(a, b, c, d)                              _gxe_widget_shift((GX_WIDGET *)a, b, c, d)
#define gx_widget_show(a)                                        _gxe_widget_show((GX_WIDGET *)a)
#define gx_widget_status_add(a, b)                               _gxe_widget_status_add((GX_WIDGET *)a, b)
#define gx_widget_status_get(a, b)                               _gxe_widget_status_get((GX_WIDGET *)a, b)
#define gx_widget_status_remove(a, b)                            _gxe_widget_status_remove((GX_WIDGET *)a, b)
#define gx_widget_status_test(a, b, c)                           _gxe_widget_status_test((GX_WIDGET *)a, b, c)
#define gx_widget_style_add(a, b)                                _gxe_widget_style_add((GX_WIDGET *)a, b)
#define gx_widget_style_get(a, b)                                _gxe_widget_style_get((GX_WIDGET *)a, b)
#define gx_widget_style_remove(a, b)                             _gxe_widget_style_remove((GX_WIDGET *)a, b)
#define gx_widget_style_set(a, b)                                _gxe_widget_style_set((GX_WIDGET *)a, b)
#define gx_widget_text_blend(a, b, c, d, e, f, g)                _gxe_widget_text_blend((GX_WIDGET *)a, b, c, d, e, f, g)
#define gx_widget_text_draw(a, b, c, d, e, f)                    _gxe_widget_text_draw((GX_WIDGET *)a, b, c, d, e, f)
#define gx_widget_text_id_draw(a, b, c, d, e, f)                 _gxe_widget_text_id_draw((GX_WIDGET *)a, b, c, d, e, f)
#define gx_widget_type_find(a, b, c)                             _gxe_widget_type_find((GX_WIDGET *)a, b, (GX_WIDGET **)c)
#define gx_widget_width_get(a, b)                                _gxe_widget_width_get((GX_WIDGET *)a, b)

#define gx_window_client_scroll                                  _gxe_window_client_scroll
#define gx_window_client_height_get                              _gxe_window_client_height_get
#define gx_window_client_width_get                               _gxe_window_client_width_get
#define gx_window_close                                          _gxe_window_close
#define gx_window_create(a, b, c, d, e, f)                       _gxe_window_create(a, b, (GX_WIDGET *)c, d, e, f, sizeof(GX_WINDOW))
#define gx_window_draw                                           _gxe_window_draw
#define gx_window_event_process                                  _gxe_window_event_process
#define gx_window_execute                                        _gxe_window_execute
#define gx_window_root_create                                    _gxe_window_root_create
#define gx_window_root_delete                                    _gxe_window_root_delete
#define gx_window_root_find                                      _gxe_window_root_find
#define gx_window_root_event_process                             _gxe_window_root_event_process
#define gx_window_scroll_info_get                                _gxe_window_scroll_info_get
#define gx_window_scrollbar_find                                 _gxe_window_scrollbar_find
#define gx_window_wallpaper_get                                  _gxe_window_wallpaper_get
#define gx_window_wallpaper_set(a, b, c)                         _gxe_window_wallpaper_set((GX_WINDOW *)a, b, c)

/* Define the error checking function prototypes of the GUIX API.  */

UINT _gxe_animation_canvas_define(GX_ANIMATION *animation, GX_VALUE width, GX_VALUE height,
                                  GX_DISPLAY *display, GX_COLOR *memory_area, ULONG memory_size);
UINT _gxe_animation_create(GX_ANIMATION *animation);
UINT _gxe_animation_delete(GX_ANIMATION *animation);
UINT _gxe_animation_start(GX_ANIMATION *animation, GX_ANIMATION_INFO *info);
UINT _gxe_animation_stop(GX_ANIMATION *animation);

UINT _gxe_brush_default(GX_BRUSH *brush);
UINT _gxe_brush_define(GX_BRUSH *brush, GX_COLOR line_color, GX_COLOR fill_color, UINT style);

UINT _gxe_button_background_draw(GX_BUTTON *button);
UINT _gxe_button_create(GX_BUTTON *button,
                        GX_CONST GX_CHAR *name,
                        GX_WIDGET *parent,
                        ULONG style, USHORT Id,
                        GX_CONST GX_RECTANGLE *size,
                        UINT button_control_block_size);
UINT _gxe_button_deselect(GX_BUTTON *button);
UINT _gxe_button_draw(GX_BUTTON *button);
UINT _gxe_button_event_process(GX_BUTTON *button, GX_EVENT *event_ptr);
UINT _gxe_button_select(GX_BUTTON *button);

UINT _gxe_canvas_alpha_set(GX_CANVAS *canvas, GX_UBYTE alpha);
UINT _gxe_canvas_block_move(GX_RECTANGLE *block, GX_VALUE x_shift, GX_VALUE y_shift, GX_RECTANGLE *dirty);
UINT _gxe_canvas_create(GX_CANVAS *canvas, GX_CONST GX_CHAR *name,
                        GX_DISPLAY *display, UINT type, UINT width, UINT height, GX_COLOR *memory_area,
                        ULONG memory_size, UINT canvas_control_block_size);
UINT _gxe_canvas_delete(GX_CANVAS *canvas);
UINT _gxe_canvas_drawing_complete(GX_CANVAS *canvas, GX_BOOL Flush);
UINT _gxe_canvas_drawing_initiate(GX_CANVAS *canvas, GX_WIDGET *who, GX_RECTANGLE *dirty_area);
UINT _gxe_canvas_line_draw(GX_VALUE x_start, GX_VALUE y_start, GX_VALUE x_end, GX_VALUE y_end);
UINT _gxe_canvas_offset_set(GX_CANVAS *canvas, GX_VALUE x, GX_VALUE y);
UINT _gxe_canvas_pixel_draw(GX_POINT position);
UINT _gxe_canvas_pixelmap_blend(GX_VALUE x_position, GX_VALUE y_position, GX_PIXELMAP *pixelmap, GX_UBYTE alpha);
UINT _gxe_canvas_pixelmap_draw(GX_VALUE x_position, GX_VALUE y_position, GX_PIXELMAP *pixelmap);
UINT _gxe_canvas_pixelmap_tile(GX_RECTANGLE *fill, GX_PIXELMAP *pixelmap);
UINT _gxe_canvas_pixelmap_rotate(GX_VALUE x_position, GX_VALUE y_position, GX_PIXELMAP *pixelmap,
                                INT angle, INT rot_cx, INT rot_cy);
UINT _gxe_canvas_polygon_draw(GX_POINT *point_array, INT number_of_points);
UINT _gxe_canvas_circle_draw(INT xcenter, INT ycenter, UINT r);
UINT _gxe_canvas_arc_draw(INT xcenter, INT ycenter, UINT r, INT start_angle, INT end_angle);
UINT _gxe_canvas_pie_draw(INT xcenter, INT ycenter, UINT r, INT start_angle, INT end_angle);
UINT _gxe_canvas_ellipse_draw(INT xcenter, INT ycenter, INT a, INT b);
UINT _gxe_canvas_rectangle_draw(GX_RECTANGLE *rectangle);
UINT _gxe_canvas_shift(GX_CANVAS *canvas, GX_VALUE x, GX_VALUE y);
UINT _gxe_canvas_text_draw(GX_VALUE x_start, GX_VALUE y_start, GX_CHAR *string, INT length);

UINT _gxe_checkbox_create(GX_CHECKBOX *checkbox, GX_CONST GX_CHAR *name,
                          GX_WIDGET *parent, GX_RESOURCE_ID text_id,
                          ULONG style, USHORT checkbox_id, GX_CONST GX_RECTANGLE *size,
                          UINT checkbox_control_block_size);
UINT _gxe_checkbox_draw(GX_CHECKBOX *checkbox);
UINT _gxe_checkbox_event_process(GX_CHECKBOX *checkbox, GX_EVENT *event_ptr);
UINT _gxe_checkbox_pixelmap_set(GX_CHECKBOX *button, GX_RESOURCE_ID unchecked_id, GX_RESOURCE_ID checked_id,
                                GX_RESOURCE_ID unchecked_disabled_id,  GX_RESOURCE_ID checked_disabled_id);
UINT _gxe_checkbox_select(GX_CHECKBOX *checkbox);

UINT _gxe_context_brush_default(GX_DRAW_CONTEXT *context);
UINT _gxe_context_brush_define(GX_RESOURCE_ID line_color_id, GX_RESOURCE_ID fill_color_id, UINT style);
UINT _gxe_context_brush_get(GX_BRUSH **return_brush);
UINT _gxe_context_brush_pattern_set(ULONG pattern);
UINT _gxe_context_brush_set(GX_BRUSH *brush);
UINT _gxe_context_brush_style_set(UINT style);
UINT _gxe_context_brush_width_set(UINT width);
UINT _gxe_context_color_get(GX_RESOURCE_ID color_id, GX_COLOR *return_color);
UINT _gxe_context_fill_color_set(GX_RESOURCE_ID fill_color_id);
UINT _gxe_context_font_get(GX_RESOURCE_ID font_id, GX_FONT **return_font);
UINT _gxe_context_font_set(GX_RESOURCE_ID font_id);
UINT _gxe_context_line_color_set(GX_RESOURCE_ID line_color_id);
UINT _gxe_context_pixelmap_get(GX_RESOURCE_ID resource_id, GX_PIXELMAP **return_pixelmap);
UINT _gxe_context_pixelmap_set(GX_RESOURCE_ID pixelmap_id);
UINT _gxe_context_raw_brush_define(GX_COLOR line_color, GX_COLOR fill_color, UINT style);
UINT _gxe_context_raw_fill_color_set(GX_COLOR line_color);
UINT _gxe_context_raw_line_color_set(GX_COLOR line_color);

UINT _gxe_circular_gauge_angle_get(GX_CIRCULAR_GAUGE *circular_gauge, INT *angle);
UINT _gxe_circular_gauge_angle_set(GX_CIRCULAR_GAUGE *circular_gauge, INT angle);
UINT _gxe_circular_gauge_animation_set(GX_CIRCULAR_GAUGE *circular_gauge, INT animation_steps);
UINT _gxe_circular_gauge_background_draw(GX_CIRCULAR_GAUGE *gauge);
UINT _gxe_circular_gauge_create(GX_CIRCULAR_GAUGE *circular_gauge,
                                GX_CONST GX_CHAR *name,
                                GX_WIDGET *parent,
                                GX_CIRCULAR_GAUGE_INFO *circular_gauge_info,
                                GX_RESOURCE_ID background,
                                ULONG style,
                                USHORT circular_gauge_id,
                                GX_VALUE xpos, GX_VALUE ypos, UINT checkbox_control_block_size);
UINT _gxe_circular_gauge_draw(GX_CIRCULAR_GAUGE *circular_gauge);
UINT _gxe_circular_gauge_event_process(GX_CIRCULAR_GAUGE *circular_gauge, GX_EVENT *event_ptr);

UINT _gxe_display_create(GX_DISPLAY *display, GX_CONST GX_CHAR *name, UINT (*display_driver_setup)(GX_DISPLAY *),
                         GX_VALUE width, GX_VALUE height, UINT display_control_block_size);
UINT _gxe_display_delete(GX_DISPLAY *display, VOID (*display_driver_cleanup)(GX_DISPLAY *));
UINT _gxe_display_color_table_set(GX_DISPLAY *display, GX_COLOR *color_table, INT number_of_colors);
UINT _gxe_display_font_table_set(GX_DISPLAY *display, GX_FONT **font_table, UINT number_of_fonts);
UINT _gxe_display_pixelmap_table_set(GX_DISPLAY *display, GX_PIXELMAP **pixelmap_table, UINT number_of_pixelmaps);

UINT _gxe_drop_list_close(GX_DROP_LIST *drop_list);
UINT _gxe_drop_list_create(GX_DROP_LIST *drop_list, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                           INT total_rows, INT open_height,
                           VOID (*callback)(GX_VERTICAL_LIST *, GX_WIDGET *, INT),
                           ULONG style, USHORT drop_list_id, GX_CONST GX_RECTANGLE *size);
UINT _gxe_drop_list_event_process(GX_DROP_LIST *list, GX_EVENT *event_ptr);
UINT _gxe_drop_list_open(GX_DROP_LIST *list);
UINT _gxe_drop_list_pixelmap_set(GX_DROP_LIST *drop_list, GX_RESOURCE_ID pixelmap_id);
UINT _gxe_drop_list_popup_get(GX_DROP_LIST *box, GX_VERTICAL_LIST **return_list);

UINT _gxe_horizontal_list_children_position(GX_HORIZONTAL_LIST *horizontal_list);
UINT _gxe_horizontal_list_create(GX_HORIZONTAL_LIST *horizontal_list, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                                 INT total_columns, VOID (*callback)(GX_HORIZONTAL_LIST *, GX_WIDGET *, INT),
                                 ULONG style, USHORT horizontal_list_id,
                                 GX_CONST GX_RECTANGLE *size, UINT horizontal_list_control_block_size);
UINT _gxe_horizontal_list_event_process(GX_HORIZONTAL_LIST *list, GX_EVENT *event_ptr);

UINT _gxe_horizontal_list_selected_index_get(GX_HORIZONTAL_LIST *horizontal_list, INT *return_index);
UINT _gxe_horizontal_list_selected_widget_get(GX_HORIZONTAL_LIST *horizontal_list, GX_WIDGET **return_list_entry);
UINT _gxe_horizontal_list_selected_set(GX_HORIZONTAL_LIST *horizontal_list, INT index);
UINT _gxe_horizontal_list_total_columns_set(GX_HORIZONTAL_LIST *horizontal_list, INT count);
UINT _gxe_horizontal_list_page_index_set(GX_HORIZONTAL_LIST *horizontal_list, INT list_entry);

UINT _gxe_horizontal_scrollbar_create(GX_SCROLLBAR *scrollbar, GX_CONST GX_CHAR *name, GX_WINDOW *parent,
                                      GX_SCROLLBAR_APPEARANCE *appearance, ULONG style, UINT scrollbar_control_block_size);

UINT _gxe_icon_background_draw(GX_ICON *icon);
UINT _gxe_icon_button_create(GX_ICON_BUTTON *button, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                             GX_RESOURCE_ID icon_id, ULONG style, USHORT icon_button_id,
                             GX_CONST GX_RECTANGLE *size, UINT button_control_block_size);
UINT _gxe_icon_button_pixelmap_set(GX_ICON_BUTTON *button, GX_RESOURCE_ID icon_id);
UINT _gxe_icon_create(GX_ICON *icon, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                      GX_RESOURCE_ID pixelmap_id, ULONG style, USHORT icon_id,
                      GX_VALUE x, GX_VALUE y, UINT icon_control_block_size);
UINT _gxe_icon_draw(GX_ICON *icon);
UINT _gxe_icon_event_process(GX_ICON *icon, GX_EVENT *event_ptr);
UINT _gxe_icon_pixelmap_set(GX_ICON *icon, GX_RESOURCE_ID normal_id, GX_RESOURCE_ID selected_id);

UINT _gxe_image_reader_create(GX_IMAGE_READER *image_reader,
                              const GX_UBYTE *read_data,
                              INT read_data_size,
                              GX_UBYTE color_format,
                              GX_UBYTE mode);
UINT _gxe_image_reader_palette_set(GX_IMAGE_READER *image_reader, GX_COLOR *pal, UINT palsize);
UINT _gxe_image_reader_start(GX_IMAGE_READER *image_reader, GX_PIXELMAP *outmap);

UINT _gxe_line_chart_axis_draw(GX_LINE_CHART *chart);
UINT _gxe_line_chart_create(GX_LINE_CHART *chart,
                          GX_CONST GX_CHAR *name,
                          GX_WIDGET *parent,
                          GX_CONST GX_LINE_CHART_INFO *info,
                          ULONG style,
                          USHORT chart_id,
                          GX_CONST GX_RECTANGLE *size,
                          UINT chart_control_block_size);

UINT _gxe_line_chart_data_draw(GX_LINE_CHART *chart);
UINT _gxe_line_chart_draw(GX_LINE_CHART *chart);
UINT _gxe_line_chart_update(GX_LINE_CHART *chart, INT *data, INT data_count);
UINT _gxe_line_chart_y_scale_calculate(GX_LINE_CHART *chart, INT *return_val);

UINT _gxe_multi_line_text_button_create(GX_MULTI_LINE_TEXT_BUTTON *button, GX_CONST GX_CHAR *name,
                                        GX_WIDGET *parent, GX_RESOURCE_ID text_id, ULONG style, USHORT Id,
                                        GX_CONST GX_RECTANGLE *size, UINT control_block_size);
UINT _gxe_multi_line_text_button_draw(GX_MULTI_LINE_TEXT_BUTTON *button);
UINT _gxe_multi_line_text_button_event_process(GX_MULTI_LINE_TEXT_BUTTON *button, GX_EVENT *event_ptr);
UINT _gxe_multi_line_text_button_text_draw(GX_MULTI_LINE_TEXT_BUTTON *button);

UINT _gxe_multi_line_text_input_create(GX_MULTI_LINE_TEXT_INPUT *text_input_ptr, GX_CONST GX_CHAR *name_ptr,
                                       GX_WIDGET *parent, GX_CHAR *input_buffer, UINT buffer_size,
                                       ULONG style, USHORT Id, GX_CONST GX_RECTANGLE *size,
                                       UINT text_input_control_block_size);
UINT _gxe_multi_line_text_input_style_add(GX_MULTI_LINE_TEXT_INPUT *text_input_ptr, ULONG cursor_type);
UINT _gxe_multi_line_text_input_style_remove(GX_MULTI_LINE_TEXT_INPUT *text_input_ptr, ULONG cursor_type);
UINT _gxe_multi_line_text_input_style_set(GX_MULTI_LINE_TEXT_INPUT *text_input_ptr, ULONG cursor_type);
UINT _gxe_multi_line_text_input_buffer_get(GX_MULTI_LINE_TEXT_INPUT *text_input_ptr, GX_CHAR **buffer_address,
                                           UINT *content_size, UINT *buffer_size);
UINT _gxe_multi_line_text_input_buffer_clear(GX_MULTI_LINE_TEXT_INPUT *text_input_ptr);

UINT _gxe_multi_line_text_view_create(GX_MULTI_LINE_TEXT_VIEW *text_view_ptr, GX_CONST GX_CHAR *name_ptr,
                                      GX_WIDGET *parent, GX_RESOURCE_ID text_id,
                                      ULONG style, USHORT Id, GX_CONST GX_RECTANGLE *size,
                                      UINT text_view_control_block_size);
UINT _gxe_multi_line_text_view_event_process(GX_MULTI_LINE_TEXT_VIEW *view, GX_EVENT *event_ptr);
UINT _gxe_multi_line_text_view_font_set(GX_MULTI_LINE_TEXT_VIEW *view, GX_RESOURCE_ID font_id);
UINT _gxe_multi_line_text_view_scroll(GX_MULTI_LINE_TEXT_VIEW *view, GX_VALUE amount_to_scroll);
UINT _gxe_multi_line_text_view_text_set(GX_MULTI_LINE_TEXT_VIEW *view, GX_CHAR *text);
UINT _gxe_multi_line_text_view_text_id_set(GX_MULTI_LINE_TEXT_VIEW *text_view_ptr, GX_RESOURCE_ID text_id);
UINT _gxe_multi_line_text_view_text_color_set(GX_MULTI_LINE_TEXT_VIEW *view, GX_RESOURCE_ID normal_id, GX_RESOURCE_ID selected_id);

UINT _gxe_pixelmap_button_create(GX_PIXELMAP_BUTTON *button, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                                 GX_RESOURCE_ID normal_id, GX_RESOURCE_ID selected_id,
                                 GX_RESOURCE_ID disabled_id, ULONG style, USHORT pixelmap_button_id,
                                 GX_CONST GX_RECTANGLE *size, UINT button_control_block_size);
UINT _gxe_pixelmap_button_draw(GX_PIXELMAP_BUTTON *button);
UINT _gxe_pixelmap_button_event_process(GX_PIXELMAP_BUTTON *button, GX_EVENT *event_ptr);
UINT _gxe_pixelmap_button_pixelmap_set(GX_PIXELMAP_BUTTON *button, GX_RESOURCE_ID normal_id, GX_RESOURCE_ID selected_id,
                                       GX_RESOURCE_ID disabled_id);

UINT _gxe_pixelmap_prompt_create(GX_PIXELMAP_PROMPT *prompt, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                                 GX_RESOURCE_ID text_id, GX_RESOURCE_ID normal_pixelmap_id,
                                 ULONG style, USHORT prompt_id, GX_CONST GX_RECTANGLE *size,
                                 UINT pixelmap_prompt_control_block_size);
UINT _gxe_pixelmap_prompt_draw(GX_PIXELMAP_PROMPT *prompt);
UINT _gxe_pixelmap_prompt_pixelmap_set(GX_PIXELMAP_PROMPT *prompt,
                                       GX_RESOURCE_ID normal_left_id,
                                       GX_RESOURCE_ID normal_fill_id,
                                       GX_RESOURCE_ID normal_right_id,
                                       GX_RESOURCE_ID selected_left_id,
                                       GX_RESOURCE_ID selected_fill_id,
                                       GX_RESOURCE_ID selected_right_id);

UINT _gxe_pixelmap_slider_create(GX_PIXELMAP_SLIDER *slider, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                                 GX_SLIDER_INFO *info,
                                 GX_PIXELMAP_SLIDER_INFO *pixelmap_info,
                                 ULONG style, USHORT pixelmap_slider_id, GX_CONST GX_RECTANGLE *size,
                                 UINT pixelmap_slider_control_block_size);
UINT _gxe_pixelmap_slider_draw(GX_PIXELMAP_SLIDER *slider);
UINT _gxe_pixelmap_slider_event_process(GX_PIXELMAP_SLIDER *slider, GX_EVENT *event_prt);
UINT _gxe_pixelmap_slider_pixelmap_set(GX_PIXELMAP_SLIDER *slider, GX_PIXELMAP_SLIDER_INFO *info);

UINT _gxe_progress_bar_create(GX_PROGRESS_BAR *progress_bar, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                              GX_PROGRESS_BAR_INFO *progress_bar_info, ULONG style,
                              USHORT progress_bar_id, GX_CONST GX_RECTANGLE *size,
                              UINT progress_bar_control_block_size);
UINT _gxe_progress_bar_draw(GX_PROGRESS_BAR *progress_bar);
UINT _gxe_progress_bar_event_process(GX_PROGRESS_BAR *progress_bar, GX_EVENT *event_ptr);
UINT _gxe_progress_bar_font_set(GX_PROGRESS_BAR *progress_bar, GX_RESOURCE_ID font_id);
UINT _gxe_progress_bar_info_set(GX_PROGRESS_BAR *progress_bar, GX_PROGRESS_BAR_INFO *info);
UINT _gxe_progress_bar_pixelmap_set(GX_PROGRESS_BAR *progress_bar, GX_RESOURCE_ID pixelmap);
UINT _gxe_progress_bar_range_set(GX_PROGRESS_BAR *progress_bar, INT min_value, INT max_value);
UINT _gxe_progress_bar_text_color_set(GX_PROGRESS_BAR *progress_bar, GX_RESOURCE_ID normal_text_color_id,
                                      GX_RESOURCE_ID selected_text_color_id);
UINT _gxe_progress_bar_value_set(GX_PROGRESS_BAR *progress_bar, INT new_value);

UINT _gxe_prompt_create(GX_PROMPT *prompt, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                        GX_RESOURCE_ID text_id, ULONG style, USHORT prompt_id,
                        GX_CONST GX_RECTANGLE *size, UINT prompt_control_block_size);
UINT _gxe_prompt_draw(GX_PROMPT *prompt);
UINT _gxe_prompt_font_set(GX_PROMPT *prompt, GX_RESOURCE_ID fontid);
UINT _gxe_prompt_text_draw(GX_PROMPT *prompt);
UINT _gxe_prompt_text_get(GX_PROMPT *prompt, GX_CHAR **return_text);
UINT _gxe_prompt_text_set(GX_PROMPT *prompt, GX_CHAR *text);
UINT _gxe_prompt_text_id_set(GX_PROMPT *prompt, GX_RESOURCE_ID string_id);
UINT _gxe_prompt_text_color_set(GX_PROMPT *prompt, GX_RESOURCE_ID normal_text_color_id, GX_RESOURCE_ID selected_text_color_id);

UINT _gxe_radial_progress_bar_anchor_set(GX_RADIAL_PROGRESS_BAR *progress_bar, GX_VALUE angle);
UINT _gxe_radial_progress_bar_background_draw(GX_RADIAL_PROGRESS_BAR *progress_bar);
UINT _gxe_radial_progress_bar_create(GX_RADIAL_PROGRESS_BAR *progress_bar,
                                     GX_CONST GX_CHAR *name,
                                     GX_WIDGET *parent,
                                     GX_RADIAL_PROGRESS_BAR_INFO *progress_bar_info,
                                     ULONG style,
                                     USHORT progress_bar_id,
                                     UINT progress_bar_control_block_size);
UINT _gxe_radial_progress_bar_draw(GX_RADIAL_PROGRESS_BAR *progress_bar);
UINT _gxe_radial_progress_bar_event_process(GX_RADIAL_PROGRESS_BAR *progress_bar, GX_EVENT *event_ptr);
UINT _gxe_radial_progress_bar_font_set(GX_RADIAL_PROGRESS_BAR *progress_bar, GX_RESOURCE_ID font_id);
UINT _gxe_radial_progress_bar_info_set(GX_RADIAL_PROGRESS_BAR *bar, GX_RADIAL_PROGRESS_BAR_INFO *info);
UINT _gxe_radial_progress_bar_text_color_set(GX_RADIAL_PROGRESS_BAR *progress_bar,
                                             GX_RESOURCE_ID normal_text_color_id,
                                             GX_RESOURCE_ID selected_text_color_id);
UINT _gxe_radial_progress_bar_text_draw(GX_RADIAL_PROGRESS_BAR *bar);
UINT _gxe_radial_progress_bar_value_set(GX_RADIAL_PROGRESS_BAR *progress_bar, GX_VALUE new_value);

UINT _gxe_radio_button_create(GX_RADIO_BUTTON *button, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                              GX_RESOURCE_ID text_id, ULONG style, USHORT radio_button_id,
                              GX_CONST GX_RECTANGLE *size, UINT radio_button_control_block_size);
UINT _gxe_radio_button_draw(GX_RADIO_BUTTON *button);
UINT _gxe_radio_button_pixelmap_set(GX_RADIO_BUTTON *button, GX_RESOURCE_ID unchecked_id, GX_RESOURCE_ID checked_id,
                                    GX_RESOURCE_ID unchecked_disabled_id, GX_RESOURCE_ID checked_disabled_id);

UINT _gxe_screen_stack_create(GX_SCREEN_STACK_CONTROL *control, GX_WIDGET **memory, INT size);
UINT _gxe_screen_stack_push(GX_SCREEN_STACK_CONTROL *control, GX_WIDGET *screen, GX_WIDGET *new_screen);
UINT _gxe_screen_stack_pop(GX_SCREEN_STACK_CONTROL *control);
UINT _gxe_screen_stack_reset(GX_SCREEN_STACK_CONTROL *control);

UINT _gxe_scrollbar_draw(GX_SCROLLBAR *scrollbar);
UINT _gxe_scrollbar_event_process(GX_SCROLLBAR *scrollbar, GX_EVENT *event_ptr);
UINT _gxe_scrollbar_limit_check(GX_SCROLLBAR *scrollbar);
UINT _gxe_scrollbar_reset(GX_SCROLLBAR *scrollbar, GX_SCROLL_INFO *info);

UINT _gxe_scroll_thumb_create(GX_SCROLL_THUMB *scroll_thumb, GX_SCROLLBAR *parent, ULONG style, UINT scroll_thumb_control_block_size);
UINT _gxe_scroll_thumb_draw(GX_SCROLL_THUMB *scroll_thumb);
UINT _gxe_scroll_thumb_event_process(GX_SCROLL_THUMB *scroll_thumb, GX_EVENT *event_ptr);

UINT _gxe_single_line_text_input_backspace(GX_SINGLE_LINE_TEXT_INPUT *text_input);
UINT _gxe_single_line_text_input_buffer_clear(GX_SINGLE_LINE_TEXT_INPUT *text_input);
UINT _gxe_single_line_text_input_buffer_get(GX_SINGLE_LINE_TEXT_INPUT *text_input_ptr, GX_CHAR **buffer_address,
                                            UINT *content_size, UINT *buffer_size);
UINT _gxe_single_line_text_input_character_delete(GX_SINGLE_LINE_TEXT_INPUT *text_input);
UINT _gxe_single_line_text_input_character_insert(GX_SINGLE_LINE_TEXT_INPUT *text_input, GX_UBYTE *str, UINT str_size);
UINT _gxe_single_line_text_input_create(GX_SINGLE_LINE_TEXT_INPUT *text_input_ptr,
                                        GX_CONST GX_CHAR *name, GX_WIDGET *parent, GX_CHAR *input_buffer,
                                        UINT buffer_size, UINT style, USHORT text_input_id,
                                        GX_CONST GX_RECTANGLE *size,
                                        UINT text_input_control_block_size);
UINT _gxe_single_line_text_input_draw(GX_SINGLE_LINE_TEXT_INPUT *text_input);
UINT _gxe_single_line_text_input_home(GX_SINGLE_LINE_TEXT_INPUT *text_input);
UINT _gxe_single_line_text_input_end(GX_SINGLE_LINE_TEXT_INPUT *text_input);
UINT _gxe_single_line_text_input_event_process(GX_SINGLE_LINE_TEXT_INPUT *text_input, GX_EVENT *event_ptr);
UINT _gxe_single_line_text_input_left_arrow(GX_SINGLE_LINE_TEXT_INPUT *text_input);
UINT _gxe_single_line_text_input_position_get(GX_SINGLE_LINE_TEXT_INPUT *text_input, UINT mouse_down_position);
UINT _gxe_single_line_text_input_right_arrow(GX_SINGLE_LINE_TEXT_INPUT *text_input);
UINT _gxe_single_line_text_input_style_add(GX_SINGLE_LINE_TEXT_INPUT *text_input_ptr, ULONG style);
UINT _gxe_single_line_text_input_style_remove(GX_SINGLE_LINE_TEXT_INPUT *text_input_ptr, ULONG style);
UINT _gxe_single_line_text_input_style_set(GX_SINGLE_LINE_TEXT_INPUT *text_input_ptr, ULONG style);

UINT _gxe_slider_create(GX_SLIDER *slider, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                        INT tick_count, GX_SLIDER_INFO *slider_info, ULONG style, USHORT slider_id,
                        GX_CONST GX_RECTANGLE *size, UINT slider_control_block_size);
UINT _gxe_slider_draw(GX_SLIDER *widget);
UINT _gxe_slider_event_process(GX_SLIDER *widget, GX_EVENT *event_ptr);
UINT _gxe_slider_info_set(GX_SLIDER *slider, GX_SLIDER_INFO *info);
UINT _gxe_slider_needle_draw(GX_SLIDER *slider);
UINT _gxe_slider_needle_position_get(GX_SLIDER *slider, GX_SLIDER_INFO *slider_info, GX_RECTANGLE *return_position);
UINT _gxe_slider_travel_get(GX_SLIDER *widget, GX_SLIDER_INFO *info, INT *mintravel, INT *maxtravel);
UINT _gxe_slider_tickmarks_draw(GX_SLIDER *slider);
UINT _gxe_slider_value_calculate(GX_SLIDER *slider, GX_SLIDER_INFO *info, INT new_position);
UINT _gxe_slider_value_set(GX_SLIDER *slider, GX_SLIDER_INFO *info, INT new_value);

UINT _gxe_sprite_create(GX_SPRITE *sprite, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                        GX_SPRITE_FRAME *frame_list, USHORT framecount,
                        ULONG style, USHORT sprite_id, GX_CONST GX_RECTANGLE *size);
UINT _gxe_sprite_current_frame_set(GX_SPRITE *sprite, USHORT frame);
UINT _gxe_sprite_frame_list_set(GX_SPRITE *sprite, GX_SPRITE_FRAME *frame_list, USHORT frame_count);
UINT _gxe_sprite_start(GX_SPRITE *sprite, USHORT start_frame);
UINT _gxe_sprite_stop(GX_SPRITE *sprite);

UINT _gxe_system_active_language_set(UINT language);
VOID _gx_system_canvas_refresh(VOID);
UINT _gxe_system_dirty_mark(GX_WIDGET *widget);
UINT _gxe_system_dirty_partial_add(GX_WIDGET *widget, GX_RECTANGLE *dirty_area);
UINT _gxe_system_draw_context_get(GX_DRAW_CONTEXT **current_context);
UINT _gxe_system_event_fold(GX_EVENT *event_ptr);
UINT _gxe_system_event_send(GX_EVENT *event_ptr);
UINT _gxe_system_focus_claim(GX_WIDGET *widget);
UINT _gxe_system_initialize(VOID);
UINT _gxe_system_language_table_get(GX_CHAR ****language_table, UINT *language_count, UINT *string_count);
UINT _gxe_system_language_table_set(GX_CHAR ***language_table, UINT number_of_languages, UINT number_of_strings);
UINT _gxe_system_memory_allocator_set(VOID *(*allocate)(ULONG size), VOID (*release)(VOID *));
UINT _gxe_system_scroll_appearance_get(ULONG style, GX_SCROLLBAR_APPEARANCE *return_appearance);
UINT _gxe_system_scroll_appearance_set(ULONG style, GX_SCROLLBAR_APPEARANCE *appearance);
UINT _gxe_system_start(VOID);
UINT _gxe_system_string_get(GX_RESOURCE_ID string_id, GX_CHAR **return_string);
UINT _gxe_system_string_table_get(INT language, GX_CHAR ***get_table, UINT *get_size);
UINT _gxe_system_string_width_get(GX_FONT *font, GX_CHAR *string, INT string_length, GX_VALUE *return_width);
UINT _gxe_system_timer_start(GX_WIDGET *owner, UINT timer_id, UINT initial_ticks, UINT reschedule_ticks);
UINT _gxe_system_timer_stop(GX_WIDGET *owner, UINT timer_id);
UINT _gxe_system_version_string_get(GX_CHAR **return_string);
UINT _gxe_system_widget_find(USHORT widget_id, INT search_level, GX_WIDGET **return_search_result);

UINT _gxe_text_button_create(GX_TEXT_BUTTON *button, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                             GX_RESOURCE_ID text_id, ULONG style, USHORT Id,
                             GX_CONST GX_RECTANGLE *size, UINT text_button_control_block_size);
UINT    _gxe_text_button_draw(GX_TEXT_BUTTON *button);
UINT    _gxe_text_button_font_set(GX_TEXT_BUTTON *button, GX_RESOURCE_ID font_id);
UINT    _gxe_text_button_text_color_set(GX_TEXT_BUTTON *_text_button, GX_RESOURCE_ID normal_text_color_id, GX_RESOURCE_ID selected_text_color_id);
UINT    _gxe_text_button_text_draw(GX_TEXT_BUTTON *button);
UINT    _gxe_text_button_text_get(GX_TEXT_BUTTON *button, GX_CHAR **return_text);
UINT    _gxe_text_button_text_id_set(GX_TEXT_BUTTON *button, GX_RESOURCE_ID string_id);
UINT    _gxe_text_button_text_set(GX_TEXT_BUTTON *button, GX_CHAR *text);

UINT    _gxe_text_input_cursor_width_set(GX_TEXT_INPUT_CURSOR *cursor_input, GX_UBYTE width);
UINT    _gxe_text_input_cursor_height_set(GX_TEXT_INPUT_CURSOR *cursor_input, GX_UBYTE height);
UINT    _gxe_text_input_cursor_blink_interval_set(GX_TEXT_INPUT_CURSOR *cursor_input, GX_UBYTE blink_interval);

VOID    _gxe_utility_ltoa(LONG value, GX_CHAR *return_buffer, UINT return_buffer_size);
INT     _gxe_utility_math_acos(INT x);
INT     _gxe_utility_math_asin(INT x);
INT     _gx_utility_math_cos(INT angle);
INT     _gx_utility_math_sin(INT angle);
UINT    _gx_utility_math_sqrt(UINT n);

VOID    _gxe_utility_rectangle_center(GX_RECTANGLE *rectangle, GX_RECTANGLE *within);
VOID    _gxe_utility_rectangle_center_find(GX_RECTANGLE *rectangle, GX_POINT *return_center);
VOID    _gxe_utility_rectangle_combine(GX_RECTANGLE *first_rectangle, GX_RECTANGLE *second_rectangle);
GX_BOOL _gxe_utility_rectangle_compare(GX_RECTANGLE *first_rectangle, GX_RECTANGLE *second_rectangle);
VOID    _gxe_utility_rectangle_define(GX_RECTANGLE *rectangle, GX_VALUE left, GX_VALUE top, GX_VALUE right, GX_VALUE bottom);
VOID    _gxe_utility_rectangle_resize(GX_RECTANGLE *rectangle, GX_VALUE adjust);
GX_BOOL _gxe_utility_rectangle_overlap_detect(GX_RECTANGLE *first_rectangle, GX_RECTANGLE *second_rectangle, GX_RECTANGLE *return_overlap_area);
GX_BOOL _gxe_utility_rectangle_point_detect(GX_RECTANGLE *rectangle, GX_POINT point);
VOID    _gxe_utility_rectangle_shift(GX_RECTANGLE *rectangle, GX_VALUE x_shift, GX_VALUE y_shift);
UINT    _gxe_utility_pixelmap_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, UINT *rot_cx, UINT *rot_cy);
UINT    _gx_utility_pixelmap_simple_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, UINT *rot_cx, UINT *rot_cy);
UINT    _gxe_utility_pixelmap_resize(GX_PIXELMAP *src, GX_PIXELMAP *destination, INT width, INT height);

UINT    _gxe_vertical_list_children_position(GX_VERTICAL_LIST *vertical_list);
UINT    _gxe_vertical_list_create(GX_VERTICAL_LIST *vertical_list, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                                  INT total_rows,
                                  VOID (*callback)(GX_VERTICAL_LIST *, GX_WIDGET *, INT),
                                  ULONG style, USHORT vertical_list_id, GX_CONST GX_RECTANGLE *size,
                                  UINT vertical_list_control_block_size);
UINT _gxe_vertical_list_event_process(GX_VERTICAL_LIST *list, GX_EVENT *event_ptr);
UINT _gxe_vertical_list_selected_index_get(GX_VERTICAL_LIST *vertical_list, INT *return_index);
UINT _gxe_vertical_list_selected_widget_get(GX_VERTICAL_LIST *vertical_list, GX_WIDGET **return_list_entry);
UINT _gxe_vertical_list_selected_set(GX_VERTICAL_LIST *vertical_list, INT index);
UINT _gxe_vertical_list_total_rows_set(GX_VERTICAL_LIST *list, INT count);
UINT _gxe_vertical_list_page_index_set(GX_VERTICAL_LIST *list, INT index);

UINT _gxe_vertical_scrollbar_create(GX_SCROLLBAR *scrollbar, GX_CONST GX_CHAR *name, GX_WINDOW *parent,
                                    GX_SCROLLBAR_APPEARANCE *settings,
                                    ULONG style, UINT scrollbar_control_blcok_size);

UINT _gxe_widget_allocate(GX_WIDGET **widget, ULONG memsize);
UINT _gxe_widget_attach(GX_WIDGET *parent, GX_WIDGET *widget);
UINT _gxe_widget_back_attach(GX_WIDGET *parent, GX_WIDGET *widget);
UINT _gxe_widget_background_draw(GX_WIDGET *a);
UINT _gxe_widget_back_move(GX_WIDGET *widget, GX_BOOL *widget_moved);
UINT _gxe_widget_block_move(GX_WIDGET *widget, GX_RECTANGLE *block, INT x_shift, INT y_shift);
UINT _gxe_widget_border_draw(GX_WIDGET *widget, GX_COLOR border_color, GX_COLOR upper_color, GX_COLOR lower_color, GX_BOOL fill);
UINT _gxe_widget_border_style_set(GX_WIDGET *widget, ULONG Style);
UINT _gxe_widget_border_width_get(GX_WIDGET *widget, GX_VALUE *return_width);
UINT _gxe_widget_canvas_get(GX_WIDGET *widget, GX_CANVAS **return_canvas);
UINT _gxe_widget_color_get(GX_WIDGET *widget, GX_RESOURCE_ID color_id, GX_COLOR *return_color);
UINT _gxe_widget_child_detect(GX_WIDGET *parent, GX_WIDGET *child, GX_BOOL *return_detect);
UINT _gxe_widget_children_draw(GX_WIDGET *widget);
UINT _gxe_widget_client_get(GX_WIDGET *widget, GX_VALUE border_width, GX_RECTANGLE *return_size);
UINT _gxe_widget_create(GX_WIDGET *widget,
                        GX_CONST GX_CHAR *name,
                        GX_WIDGET *parent,
                        ULONG style, USHORT Id,
                        GX_CONST GX_RECTANGLE *size);
UINT _gxe_widget_created_test(GX_WIDGET *widget, GX_BOOL *return_test);
UINT _gxe_widget_detach(GX_WIDGET *widget);
UINT _gxe_widget_delete(GX_WIDGET *widget);
UINT _gxe_widget_draw(GX_WIDGET *widget);
UINT _gxe_widget_draw_set(GX_WIDGET *widget, VOID (*draw_func)(GX_WIDGET *));
UINT _gxe_widget_event_generate(GX_WIDGET *widget, USHORT event_type, LONG value);
UINT _gxe_widget_event_process(GX_WIDGET *widget, GX_EVENT *event_ptr);
UINT _gxe_widget_event_process_set(GX_WIDGET *widget, UINT (*event_processing_function)(GX_WIDGET *, GX_EVENT *));
UINT _gxe_widget_event_to_parent(GX_WIDGET *widget, GX_EVENT *event_ptr);
UINT _gxe_widget_fill_color_set(GX_WIDGET *widget, GX_RESOURCE_ID normal_color, GX_RESOURCE_ID selected_color);
UINT _gxe_widget_find(GX_WIDGET *parent, USHORT widget_id, INT search_depth, GX_WIDGET **return_widget);
UINT _gxe_widget_font_get(GX_WIDGET *widget, GX_RESOURCE_ID font_id, GX_FONT **return_font);
UINT _gxe_widget_free(GX_WIDGET *widget);
UINT _gxe_widget_front_move(GX_WIDGET *widget, GX_BOOL *widget_moved);
UINT _gxe_widget_focus_next(GX_WIDGET *widget);
UINT _gxe_widget_focus_previous(GX_WIDGET *widget);
UINT _gxe_widget_height_get(GX_WIDGET *widget, GX_VALUE *return_height);
UINT _gxe_widget_hide(GX_WIDGET *widget);
UINT _gxe_widget_pixelmap_get(GX_WIDGET *widget, GX_RESOURCE_ID pixelmap_id, GX_PIXELMAP **return_pixelmap);
UINT _gxe_widget_resize(GX_WIDGET *widget, GX_RECTANGLE *newsize);
UINT _gxe_widget_shift(GX_WIDGET *widget, GX_VALUE x_shift, GX_VALUE y_shift, GX_BOOL mark_dirty);
UINT _gxe_widget_show(GX_WIDGET *widget);
UINT _gxe_widget_status_add(GX_WIDGET *widget, ULONG status);
UINT _gxe_widget_status_get(GX_WIDGET *widget, ULONG *return_status);
UINT _gxe_widget_status_remove(GX_WIDGET *widget, ULONG status);
UINT _gxe_widget_status_test(GX_WIDGET *widget, ULONG status, GX_BOOL *return_test);
UINT _gxe_widget_style_add(GX_WIDGET *widget, ULONG style);
UINT _gxe_widget_style_get(GX_WIDGET *widget, ULONG *return_style);
UINT _gxe_widget_style_remove(GX_WIDGET *widget, ULONG style);
UINT _gxe_widget_style_set(GX_WIDGET *widget, ULONG style);

UINT _gxe_widget_text_blend(GX_WIDGET *widget, UINT tColor, UINT font_id, GX_CHAR *string,
                           INT x_offset, INT y_offset, UCHAR alpha);


UINT _gxe_widget_text_draw(GX_WIDGET *widget, UINT tColor, UINT font_id, GX_CHAR *string,
                           INT x_offset, INT y_offset);

UINT _gxe_widget_text_id_draw(GX_WIDGET *widget, UINT tColor,
                              UINT font_id, UINT text_id,
                              INT x_offset, INT y_offset);

UINT _gxe_widget_type_find(GX_WIDGET *parent, USHORT widget_type, GX_WIDGET **return_widget);
UINT _gxe_widget_width_get(GX_WIDGET *widget, GX_VALUE *return_width);

UINT _gxe_window_client_height_get(GX_WINDOW *window, GX_VALUE *return_height);
UINT _gxe_window_client_scroll(GX_WINDOW *window, GX_VALUE xscroll, GX_VALUE yscroll);
UINT _gxe_window_client_width_get(GX_WINDOW *window, GX_VALUE *return_width);
UINT _gxe_window_close(GX_WINDOW *window);
UINT _gxe_window_create(GX_WINDOW *window, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                        ULONG style, USHORT Id, GX_CONST GX_RECTANGLE *size, UINT window_control_block_size);
UINT _gxe_window_draw(GX_WINDOW *window);
UINT _gxe_window_event_process(GX_WINDOW *window, GX_EVENT *event_ptr);
UINT _gxe_window_execute(GX_WINDOW *window, ULONG *return_ptr);
UINT _gxe_window_root_create(GX_WINDOW_ROOT *root_window, GX_CONST GX_CHAR *name, GX_CANVAS *canvas,
                             ULONG style, USHORT Id, GX_CONST GX_RECTANGLE *size);
UINT _gxe_window_root_delete(GX_WINDOW_ROOT *root_window);
UINT _gxe_window_root_find(GX_WIDGET *widget, GX_WINDOW_ROOT **return_root_window);
UINT _gxe_window_root_event_process(GX_WINDOW_ROOT *root, GX_EVENT *event_ptr);
UINT _gxe_window_scroll_info_get(GX_WINDOW *window, ULONG type, GX_SCROLL_INFO *return_scroll_info);
UINT _gxe_window_scrollbar_find(GX_WINDOW *window, USHORT type, GX_SCROLLBAR **return_scrollbar);
UINT _gxe_window_wallpaper_get(GX_WINDOW *window, GX_RESOURCE_ID *return_wallpaper_id);
UINT _gxe_window_wallpaper_set(GX_WINDOW *window, GX_RESOURCE_ID wallpaper_id, GX_BOOL tile);

#endif

#endif

/* Determine if a C++ compiler is being used.  If so, complete the standard
   C conditional started above.  */
#ifdef __cplusplus
}
#endif

#ifdef GX_MULTITHREAD_SUPPORT

#define GX_ENTER_CRITICAL _gx_system_lock();
#define GX_EXIT_CRITICAL  _gx_system_unlock();

#else

#define GX_ENTER_CRITICAL
#define GX_EXIT_CRITICAL

#endif

#ifndef GX_THREADX_BINDING
/* pull in custom rtos porting header */
#include <string.h>
#include <stdlib.h>
#include "gx_system_rtos_bind.h"
#endif

#endif

