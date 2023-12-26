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
/**************************************************************************/
/**************************************************************************/
/*                                                                        */
/*  APPLICATION INTERFACE DEFINITION                       RELEASE        */
/*                                                                        */
/*    gx_api.h                                            PORTABLE C      */
/*                                                           6.4.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
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
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            added line break status,    */
/*                                            updated product constants,  */
/*                                            modified controls blocks,   */
/*                                            added new APIs,             */
/*                                            resulting in version 6.1    */
/*  12-31-2020     Kenneth Maxwell          Modified comment(s), added    */
/*                                            display rotation support,   */
/*                                            declare new APIs,           */
/*                                            defined new status,         */
/*                                            resulting in version 6.1.3  */
/*  03-02-2021     Kenneth Maxwell          Modified comment(s),          */
/*                                            change scroll_wheel style   */
/*                                            flag to status flag,        */
/*                                            renamed                     */
/*                                            GX_STATUS_TRACKING_START to */
/*                                            GX_STATUS_TRACKING_PEN,     */
/*                                            added rotation angle        */
/*                                            definitions, changed        */
/*                                            pixelmap rotation flag      */
/*                                            definitions,                */
/*                                            resulting in version 6.1.5  */
/*  06-02-2021     Ting Zhu                 Modified comment(s),          */
/*                                            added new widget type       */
/*                                            GX_GENERIC_SCROLL_WHEEL,    */
/*                                            added animation delete API, */
/*                                            added generic Dave2D        */
/*                                            graphics accelerator        */
/*                                            support,                    */
/*                                            resulting in version 6.1.7  */
/*  08-02-2021     Ting Zhu                 Modified comment(s), added    */
/*                                            GX_TICKS_SECOND definition  */
/*                                            resulting in version 6.1.8  */
/*  10-15-2021     Ting Zhu                 Modified comment(s),          */
/*                                            updated patch number,       */
/*                                            resulting in version 6.1.9  */
/*  01-31-2022     Kenneth Maxwell          Modified comment(s),          */
/*                                            removed errant semicolons,  */
/*                                            added new member to struct  */
/*                                            GX_RADIAL_SLIDER_INFO,      */
/*                                            added new member to         */
/*                                            vertical and horizontal     */
/*                                            list control blocks,        */
/*                                            resulting in version 6.1.10 */
/*  04-25-2022     Ting Zhu                 Modified comment(s),          */
/*                                            added new member to struct  */
/*                                            GX_DISPLAY_LAYER_SERVICES,  */
/*                                            added new animation flag    */
/*                                            GX_ANIMATION_BLOCK_MOVE,    */
/*                                            resulting in version 6.1.11 */
/*  07-29-2022     Kenneth Maxwell          Modified comment(s),          */
/*                                            added new style             */
/*                                            GX_STYLE_REPEAT_SELECT,     */
/*                                            resulting in version 6.1.12 */
/*  10-31-2022     Kenneth Maxwell          Modified comment(s),          */
/*                                            removed unused style flag   */
/*                                            GX_SCROLLBAR_BACKGROUND_    */
/*                                            TILE,                       */
/*                                            resulting in version 6.2.0  */
/*  03-08-2023     Ting Zhu                 Modified comment(s),          */
/*                                            updated patch version,      */
/*                                            resulting in version 6.2.1  */
/*  10-31-2023     Ting Zhu                 Modified comment(s),          */
/*                                            added new APIs for loading  */
/*                                            pixelmap and font from      */
/*                                            standalone binary,          */
/*                                            removed unused defines,     */
/*                                            added partial canvas buffer */
/*                                            support,                    */
/*                                            resulting in version 6.3.0  */
/*  12-31-2023     Ting Zhu                 Modified comment(s),          */
/*                                            updated patch version,      */
/*                                            resulting in version 6.4.0  */
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

#define AZURE_RTOS_GUIX
#define GUIX_MAJOR_VERSION 6
#define GUIX_MINOR_VERSION 4
#define GUIX_PATCH_VERSION 0

/* The following symbols are defined for backward compatibility reasons.*/
#define __PRODUCT_GUIX__
#define __GUIX_MAJOR_VERSION GUIX_MAJOR_VERSION
#define __GUIX_MINOR_VERSION GUIX_MINOR_VERSION

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

/* Fixed point math data type */
typedef signed long GX_FIXED_VAL;

typedef char GX_CHAR;

/* Define the String type. */
typedef struct GX_STRING_STRUCT
{
    GX_CONST GX_CHAR  *gx_string_ptr;     /* Pointer to string. */
    UINT               gx_string_length;  /* String length in bytes. */
} GX_STRING;

/* Define constants for GUIX.  */

/* Define configuration-specific constants. These constants can be redefined externally.  */

/* Default 20ms GUIX system timer.  */
#ifndef GX_SYSTEM_TIMER_MS
#define GX_SYSTEM_TIMER_MS        20
#endif

#ifndef GX_DISABLE_DEPRECATED_STRING_API
#define GX_ENABLE_DEPRECATED_STRING_API
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

#ifndef GX_TICKS_SECOND
#define GX_TICKS_SECOND           (1000 / GX_SYSTEM_TIMER_MS)
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

#ifndef GX_DISABLE_BINARY_RESOURCE_SUPPORT
#define GX_BINARY_RESOURCE_SUPPORT
#endif

#ifndef GX_DISABLE_BRUSH_ALPHA_SUPPORT
#define GX_BRUSH_ALPHA_SUPPORT
#endif

#ifndef GX_DISABLE_EXPO_ELASTIC_EASING_SUPPORT
#define GX_EXPO_ELASTIC_EASING_SUPPORT
#endif

#ifndef GX_REPEAT_BUTTON_INITIAL_TICS
#define GX_REPEAT_BUTTON_INITIAL_TICS       10
#endif

#ifndef GX_REPEAT_BUTTON_REPEAT_TICS
#define GX_REPEAT_BUTTON_REPEAT_TICS        3
#endif

/* define size of the GUIX event queue */
#ifndef GX_MAX_QUEUE_EVENTS
#define GX_MAX_QUEUE_EVENTS                 48
#endif

#ifndef GX_MAX_DIRTY_AREAS
#define GX_MAX_DIRTY_AREAS                  64
#endif

/* define depth of drawing context stack */
#ifndef GX_MAX_CONTEXT_NESTING
#define GX_MAX_CONTEXT_NESTING              8
#endif

/* define depth of input capture stack */

#ifndef GX_MAX_INPUT_CAPTURE_NESTING
#define GX_MAX_INPUT_CAPTURE_NESTING        4
#endif

#ifndef GX_SYSTEM_THREAD_PRIORITY
#define GX_SYSTEM_THREAD_PRIORITY           16
#endif

#ifndef GX_SYSTEM_THREAD_TIMESLICE
#define GX_SYSTEM_THREAD_TIMESLICE          10
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

/* Define rich text context stack size. */
#ifndef GX_MAX_RICH_TEXT_CONTEXT_NESTING
#define GX_MAX_RICH_TEXT_CONTEXT_NESTING    10
#endif

/* maximum number of polygon edges */
#ifndef GX_POLYGON_MAX_EDGE_NUM
#define GX_POLYGON_MAX_EDGE_NUM             10
#endif

#ifndef GX_NUMERIC_SCROLL_WHEEL_STRING_BUFFER_SIZE
#define GX_NUMERIC_SCROLL_WHEEL_STRING_BUFFER_SIZE 16
#endif

#ifndef GX_DEFAULT_CIRCULAR_GAUGE_ANIMATION_DELAY
#define GX_DEFAULT_CIRCULAR_GAUGE_ANIMATION_DELAY 5
#endif

/* define the largest string needed by a NUMERIC prompt widget */
#ifndef GX_NUMERIC_PROMPT_BUFFER_SIZE
#define GX_NUMERIC_PROMPT_BUFFER_SIZE        16
#endif

/* define size of the GX_ANIMATION pool, i.e. max sumultaneous animations */
#ifndef GX_ANIMATION_POOL_SIZE
#define GX_ANIMATION_POOL_SIZE 6
#endif

/* define maximum string length. */
#ifndef GX_MAX_STRING_LENGTH
#define GX_MAX_STRING_LENGTH 102400
#endif

#ifndef GX_PALETTE_MODE_AA_TEXT_COLORS
#define GX_PALETTE_MODE_AA_TEXT_COLORS       8
#endif

#ifndef GX_PARAMETER_NOT_USED
#define GX_PARAMETER_NOT_USED(p) ((void) (p))
#endif /* GX_PARAMETER_NOT_USED */

/* defines for backwards compatiblity */
#ifndef GX_RENESAS_DAVE2D_DRAW
#ifdef  GX_USE_SYNERGY_DRW
#define GX_RENESAS_DAVE2D_DRAW
#endif
#endif

#ifndef GX_RENESAS_DAVE2D_FONT_SUPPORT
#ifdef  GX_SYNERGY_FONT_FORMAT_SUPPORT
#define GX_RENESAS_DAVE2D_FONT_SUPPORT
#endif
#endif
/* end of defines for backwards compatiblity */

#define GX_MAX_PIXELMAP_RESOLUTION 0x3FFF

/* Define screen rotation types. */
#define GX_SCREEN_ROTATION_NONE   0
#define GX_SCREEN_ROTATION_CW    90
#define GX_SCREEN_ROTATION_CCW  270
#define GX_SCREEN_ROTATION_FLIP 180

/* API input parameters and general constants.  */

#define GX_TRUE                             1
#define GX_FALSE                            0
#define GX_NULL                             0
#define GX_ID_NONE                          0
#define GX_MAGIC_NUMBER                     0x4758U /* GX */
#define GX_RESOURCE_TYPE_BINRES_NORMAL      0x4758U /* GX */
#define GX_RESOURCE_TYPE_BINRES_STANDALONE  0x4753U /* GS */

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
#define GX_INVALID_VALUE                    0x22
#define GX_INVALID_DISPLAY_SIZE             0x23
#define GX_INVALID_FORMAT                   0x24
#define GX_INVALID_APPEARANCE               0x25
#define GX_INVALID_STATUS                   0x26
#define GX_INPUT_CAPTURE_NESTING_EXCEEDED   0x27
#define GX_NOT_SUPPORTED                    0x28
#define GX_INVALID_MEMORY_SIZE              0x29
#define GX_SYSTEM_MEMORY_ERROR              0x30
#define GX_OUT_OF_ANIMATIONS                0x31
#define GX_INVALID_ANIMATION                0x32
#define GX_INVALID_RESOURCE_ID              0x33
#define GX_INVALID_STRING_LENGTH            0x34
#define GX_MATH_OVERFLOW                    0x35
#define GX_RICH_STYLE_NESTING_EXEEDED       0x36
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
#define GX_COLOR_FORMAT_4BIT_GRAY           5                   /* 4 bit-per-pixel grayscale                */
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

#define GX_COLOR_ID_BTN_BORDER             10
#define GX_COLOR_ID_BUTTON_BORDER          GX_COLOR_ID_BTN_BORDER
#define GX_COLOR_ID_BTN_UPPER              11
#define GX_COLOR_ID_BUTTON_UPPER           GX_COLOR_ID_BTN_UPPER
#define GX_COLOR_ID_BTN_LOWER              12
#define GX_COLOR_ID_BUTTON_LOWER           GX_COLOR_ID_BTN_LOWER
#define GX_COLOR_ID_BTN_TEXT               13
#define GX_COLOR_ID_BUTTON_TEXT            GX_COLOR_ID_BTN_TEXT

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

/* Text widget color resource ids */
#define GX_COLOR_ID_DISABLED_TEXT           25
#define GX_COLOR_ID_DISABLED_FILL           26
#define GX_COLOR_ID_READONLY_TEXT           27
#define GX_COLOR_ID_READONLY_FILL           28

#define GX_MAX_DEFAULT_COLORS               29
#define GX_FIRST_USER_COLOR                 GX_MAX_DEFAULT_COLORS

/* Default font Resource IDs.  */

#define GX_FONT_ID_DEFAULT                  0
#define GX_FONT_ID_SYSTEM                   GX_FONT_ID_DEFAULT
#define GX_FONT_ID_BUTTON                   1
#define GX_FONT_ID_PROMPT                   2
#define GX_FONT_ID_TEXT_INPUT               3
#define GX_DEFAULT_FONT_COUNT               4
#define GX_FIRST_USER_FONT                  GX_DEFAULT_FONT_COUNT

/* Default PIXELMAP Resource IDs. */

#define GX_PIXELMAP_NULL                    0
#define GX_PIXELMAP_RADIO_ON_ID             1
#define GX_PIXELMAP_ID_RADIO_ON             GX_PIXELMAP_RADIO_ON_ID
#define GX_PIXELMAP_RADIO_OFF_ID            2
#define GX_PIXELMAP_ID_RADIO_OFF            GX_PIXELMAP_RADIO_OFF_ID
#define GX_PIXELMAP_CHECKBOX_ON_ID          3
#define GX_PIXELMAP_ID_CHECKBOX_ON          GX_PIXELMAP_CHECKBOX_ON_ID
#define GX_PIXELMAP_CHECKBOX_OFF_ID         4
#define GX_PIXELMAP_ID_CHECKBOX_OFF         GX_PIXELMAP_CHECKBOX_OFF_ID
#define GX_DEFAULT_PIXELMAP_COUNT           5
#define GX_FIRST_USER_PIXELMAP              GX_DEFAULT_PIXELMAP_COUNT

/* Predefined a:r:g:b color values, these follow the standard 16 VGA color names.  */

#define GX_COLOR_BLACK                      0xff000000UL
#define GX_COLOR_RED                        0xffb80000UL
#define GX_COLOR_GREEN                      0xff00bc00UL
#define GX_COLOR_BROWN                      0xffb8bc00UL
#define GX_COLOR_BLUE                       0xff0000b8UL
#define GX_COLOR_MAGENTA                    0xffb800b8UL
#define GX_COLOR_CYAN                       0xff00bcb8UL
#define GX_COLOR_LIGHTGRAY                  0xffc0c0c0UL
#define GX_COLOR_DARKGRAY                   0xff808080UL
#define GX_COLOR_LIGHTRED                   0xffff0000UL
#define GX_COLOR_LIGHTGREEN                 0xff00ff00UL
#define GX_COLOR_YELLOW                     0xffffff00UL
#define GX_COLOR_LIGHTBLUE                  0xff0000ffUL
#define GX_COLOR_LIGHTMAGENTA               0xffff00ffUL
#define GX_COLOR_LIGHTCYAN                  0xff00ffffUL
#define GX_COLOR_WHITE                      0xffffffffUL

/* Define events. The the event ranges are:

   0x00000000 to 0x000000ff   256 pre-defined system event types
   0x00000100 to 0x00ffffff   Signals from child widgets
   0x01000000 to 0x3fffffff   Unused
   0x40000000 to 0xffffffff   Application defined

 */
#define GX_FIRST_SIGNAL                     0x100
#define GX_LAST_SIGNAL                      0x00ffffffUL
#define GX_SIGNAL_EVENT_MASK                0xff


/* Define the pre-defined Widget event types.  */

#define GX_EVENT_TERMINATE                  1
#define GX_EVENT_REDRAW                     2
#define GX_EVENT_SHOW                       3
#define GX_EVENT_HIDE                       4
#define GX_EVENT_RESIZED                    5
#define GX_EVENT_SLIDE                      6
#define GX_EVENT_FOCUS_GAINED               7
#define GX_EVENT_FOCUS_LOST                 8
#define GX_EVENT_HORIZONTAL_SCROLL          9
#define GX_EVENT_VERTICAL_SCROLL            10
#define GX_EVENT_TIMER                      11
#define GX_EVENT_PEN_DOWN                   12
#define GX_EVENT_PEN_UP                     13
#define GX_EVENT_PEN_MOVE                   14
#define GX_EVENT_PEN_DRAG                   15
#define GX_EVENT_KEY_DOWN                   16
#define GX_EVENT_KEY_UP                     17
#define GX_EVENT_CLOSE                      18
#define GX_EVENT_DELETE                     19
#define GX_EVENT_SLIDER_VALUE               20
#define GX_EVENT_TOGGLE_ON                  21
#define GX_EVENT_TOGGLE_OFF                 22
#define GX_EVENT_RADIO_SELECT               23
#define GX_EVENT_RADIO_DESELECT             24
#define GX_EVENT_CLICKED                    25
#define GX_EVENT_LIST_SELECT                26
#define GX_EVENT_VERTICAL_FLICK             27
#define GX_EVENT_HORIZONTAL_FLICK           28

#define GX_EVENT_PARENT_SIZED               30
#define GX_EVENT_CLOSE_POPUP                31
#define GX_EVENT_ZOOM_IN                    32
#define GX_EVENT_ZOOM_OUT                   33
#define GX_EVENT_LANGUAGE_CHANGE            34
#define GX_EVENT_RESOURCE_CHANGE            35
#define GX_EVENT_ANIMATION_COMPLETE         36
#define GX_EVENT_SPRITE_COMPLETE            37
#define GX_EVENT_TEXT_EDITED                40
#define GX_EVENT_ANIMATION_START            41

#define GX_EVENT_FOCUS_NEXT                 42
#define GX_EVENT_FOCUS_PREVIOUS             43
#define GX_EVENT_FOCUS_GAIN_NOTIFY          44
#define GX_EVENT_SELECT                     45
#define GX_EVENT_DESELECT                   46
#define GX_EVENT_PROGRESS_VALUE             47
#define GX_EVENT_TOUCH_CALIBRATION_COMPLETE 48
#define GX_EVENT_INPUT_RELEASE              49
#define GX_EVENT_TREE_SELECT                50
#define GX_EVENT_STYLE_CHANGED              51
#define GX_EVENT_CLIENT_UPDATED             52
#define GX_EVENT_CUT                        53
#define GX_EVENT_COPY                       54
#define GX_EVENT_PASTE                      55
#define GX_EVENT_MARK_NEXT                  56
#define GX_EVENT_MARK_PREVIOUS              57
#define GX_EVENT_MARK_UP                    58
#define GX_EVENT_MARK_DOWN                  59
#define GX_EVENT_MARK_END                   60
#define GX_EVENT_MARK_HOME                  61
#define GX_EVENT_DYNAMIC_BIDI_TEXT_ENABLE   62
#define GX_EVENT_DYNAMIC_BIDI_TEXT_DISABLE  63

/* Define the pre-defined Widgets ID.        */
#define ID_DROP_LIST_BUTTON                 65535

/* Define the value of the first application defined event type.  */

#define GX_FIRST_USER_EVENT                 0x40000000UL

/* The following name has been deprecated, please use GX_FIRST_USER_EVENT */

#define GX_FIRST_APP_EVENT                  GX_FIRST_USER_EVENT


/* Define logical key values.  */

#define GX_KEY_PAGE_UP                      0x1b01U
#define GX_KEY_PAGE_DOWN                    0x1b02U
#define GX_KEY_END                          0x1b03U
#define GX_KEY_HOME                         0x1b04U
#define GX_KEY_LEFT_ARROW                   0x1b05U
#define GX_KEY_UP_ARROW                     0x1b06U
#define GX_KEY_RIGHT_ARROW                  0x1b07U
#define GX_KEY_DOWN_ARROW                   0x1b08U
#define GX_KEY_NEXT                         0x1b09U
#define GX_KEY_PREVIOUS                     0x1b0aU
#define GX_KEY_CAPS_LOCK                    0x1b10U
#define GX_KEY_SHIFT                        0x1b11U
#define GX_KEY_CONTROL                      0x1b12U
#define GX_KEY_NUMLOCK                      0x1b13U
#define GX_KEY_SEPARATOR                    0x1b14U

#define GX_KEY_BACKSPACE                    0x08
#define GX_KEY_SPACE                        0x20
#define GX_KEY_DELETE                       0x7f
#define GX_KEY_CARRIAGE_RETURN              0x0d
#define GX_KEY_LINE_FEED                    0x0a
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
#define GX_TYPE_RADIAL_SLIDER               26

#define GX_TYPE_PROMPT                      30
#define GX_TYPE_NUMERIC_PROMPT              31
#define GX_TYPE_PIXELMAP_PROMPT             32
#define GX_TYPE_NUMERIC_PIXELMAP_PROMPT     33

#define GX_TYPE_SINGLE_LINE_TEXT_INPUT      64
#define GX_TYPE_PIXELMAP_TEXT_INPUT         65
#define GX_TYPE_DROP_LIST                   70

#define GX_TYPE_MENU_LIST                   75
#define GX_TYPE_MENU                        76
#define GX_TYPE_ACCORDION_MENU              77

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
#define GX_TYPE_SCROLL_WHEEL                139
#define GX_TYPE_TEXT_SCROLL_WHEEL           140
#define GX_TYPE_STRING_SCROLL_WHEEL         141
#define GX_TYPE_NUMERIC_SCROLL_WHEEL        142
#define GX_TYPE_TREE_VIEW                   143
#define GX_TYPE_RICH_TEXT_VIEW              144
#define GX_TYPE_GENERIC_SCROLL_WHEEL        145

/* A pseudo-type, used by Studio code generator */ 
#define GX_TYPE_TEMPLATE                    200

/* Applications can define their own window types */
#define GX_FIRST_USER_WINDOW_TYPE           256


/* Define Widget status flags.  */

#define GX_STATUS_VISIBLE                   0x00000001UL
#define GX_STATUS_SELECTABLE                0x00000002UL
#define GX_STATUS_ACCEPTS_INPUT             0x00000004UL
#define GX_STATUS_HIDDEN                    0x00000008UL

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

/* define cursor status flags */
#define GX_STATUS_CURSOR_SHOW               0x00010000UL
#define GX_STATUS_CURSOR_DRAW               0x00020000UL
#define GX_STATUS_MARK_PREVIOUS             0x00040000UL
#define GX_STATUS_MARK_NEXT                 0x00080000UL

/* Define radial slider status flags */
#define GX_STATUS_ANIMATION_NONE            0x00010000UL
#define GX_STATUS_TRACKING_PEN              0x00020000UL

#define GX_STATUS_MODAL                     0x00100000UL

#define GX_STATUS_DYNAMIC_BUFFER            0x01000000UL
#define GX_STATUS_LINE_BREAK_PROCESSED      0x02000000UL
#define GX_STATUS_RESIZE_NOTIFY             0x04000000UL
#define GX_STATUS_STUDIO_CREATED            0x08000000UL

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
#define GX_STYLE_USE_LOCAL_ALPHA            0x01000000UL

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
#define GX_STYLE_REPEAT_SELECT              0x00000080UL


/* Define Icon, Pixelmap button and Icon button alignment flags.  */

#define GX_STYLE_HALIGN_CENTER              0x00010000UL
#define GX_STYLE_HALIGN_LEFT                0x00020000UL
#define GX_STYLE_HALIGN_RIGHT               0x00040000UL
#define GX_STYLE_VALIGN_CENTER              0x00080000UL
#define GX_STYLE_VALIGN_TOP                 0x00100000UL
#define GX_STYLE_VALIGN_BOTTOM              0x00200000UL

#define GX_PIXELMAP_HALIGN_MASK             0x00070000UL
#define GX_PIXELMAP_VALIGN_MASK             0x00380000UL


/* Define Slider style flags.  */

#define GX_STYLE_SHOW_NEEDLE                0x00000200UL
#define GX_STYLE_SHOW_TICKMARKS             0x00000400UL
#define GX_STYLE_SLIDER_VERTICAL            0x00000800UL

/* Define Pixelmap Slider flags.  */

#define GX_STYLE_TILE_BACKGROUND            0x00001000UL

/* Define Progress Bar style flags.  */

#define GX_STYLE_PROGRESS_PERCENT           0x00000010UL
#define GX_STYLE_PROGRESS_TEXT_DRAW         0x00000020UL
#define GX_STYLE_PROGRESS_VERTICAL          0x00000040UL

#define GX_STYLE_PROGRESS_SEGMENTED_FILL    0x00000100UL

/* Define Radial Progress Bar style flags.  */
#define GX_STYLE_RADIAL_PROGRESS_ALIAS      0x00000200UL
#define GX_STYLE_RADIAL_PROGRESS_ROUND      0x00000400UL
#define GX_STYLE_RADIAL_PROGRESS_NO_BACKTRACK 0x00000800UL

/* Define Text alignment styles.  */

#define GX_STYLE_TEXT_LEFT                  0x00001000UL
#define GX_STYLE_TEXT_RIGHT                 0x00002000UL
#define GX_STYLE_TEXT_CENTER                0x00004000UL
#define GX_STYLE_TEXT_ALIGNMENT_MASK        0x00007000UL
#define GX_STYLE_TEXT_COPY                  0x00008000UL

/* define cursor style flags */
#define GX_STYLE_CURSOR_BLINK               0x00000040UL
#define GX_STYLE_CURSOR_ALWAYS              0x00000080UL

/* text input style */
#define GX_STYLE_TEXT_INPUT_NOTIFY_ALL      0x00000100UL
#define GX_STYLE_TEXT_INPUT_READONLY        0x00000200UL

/* Define Window styles.  */
#define GX_STYLE_TILE_WALLPAPER             0x00040000UL

/* Define circular gauge styles.  */
#define GX_STYLE_CIRCULAR_GAUGE_CCW         0x00000010UL

/* Define menu styles. */
#define GX_STYLE_MENU_EXPANDED              0x00000010UL

/* Define tree view styles. */
#define GX_STYLE_TREE_VIEW_SHOW_ROOT_LINES  0x00000010UL

/* Define Scroll Bar styles.  */

#define GX_SCROLLBAR_RELATIVE_THUMB         0x00020000UL
#define GX_SCROLLBAR_END_BUTTONS            0x00040000UL
#define GX_SCROLLBAR_VERTICAL               0x01000000UL
#define GX_SCROLLBAR_HORIZONTAL             0x02000000UL


/* Define text scroll wheel styles*/
#define GX_STYLE_TEXT_SCROLL_WHEEL_ROUND    0x00000100UL

/* Define animation status.  */
#define GX_ANIMATION_IDLE                   0x01U
#define GX_ANIMATION_ACTIVE                 0x02U
#define GX_ANIMATION_SLIDE_TRACKING         0x03U
#define GX_ANIMATION_SLIDE_LANDING          0x04U

/* Define pre-defined animation type flags.  */

#define GX_ANIMATION_NONE                   0x0000U
#define GX_ANIMATION_TRANSLATE              0x0001U
#define GX_ANIMATION_SCREEN_DRAG            0x0002U
#define GX_ANIMATION_BLOCK_MOVE             0x0004U

/* flags that can be used in combination with screen drag animation */
#define GX_ANIMATION_WRAP                   0x0100U
#define GX_ANIMATION_HORIZONTAL             0x0200U
#define GX_ANIMATION_VERTICAL               0x0400U

/* flag that can be used in combination with translate animation */
#define GX_ANIMATION_PUSH_STACK             0x4000U
#define GX_ANIMATION_DETACH                 0x8000U
#define GX_ANIMATION_EASING_FUNC_MASK       0x10f0U
#define GX_ANIMATION_BACK_EASE_IN           0x0010U
#define GX_ANIMATION_BACK_EASE_OUT          0x0020U
#define GX_ANIMATION_BACK_EASE_IN_OUT       0x0030U
#define GX_ANIMATION_BOUNCE_EASE_IN         0x0040U
#define GX_ANIMATION_BOUNCE_EASE_OUT        0x0050U
#define GX_ANIMATION_BOUNCE_EASE_IN_OUT     0x0060U
#define GX_ANIMATION_CIRC_EASE_IN           0x0070U
#define GX_ANIMATION_CIRC_EASE_OUT          0x0080U
#define GX_ANIMATION_CIRC_EASE_IN_OUT       0x0090U
#define GX_ANIMATION_CUBIC_EASE_IN          0x00a0U
#define GX_ANIMATION_CUBIC_EASE_OUT         0x00b0U
#define GX_ANIMATION_CUBIC_EASE_IN_OUT      0x00c0U
#define GX_ANIMATION_ELASTIC_EASE_IN        0x00d0U
#define GX_ANIMATION_ELASTIC_EASE_OUT       0x00e0U
#define GX_ANIMATION_ELASTIC_EASE_IN_OUT    0x00f0U
#define GX_ANIMATION_EXPO_EASE_IN           0x1000U
#define GX_ANIMATION_EXPO_EASE_OUT          0x1010U
#define GX_ANIMATION_EXPO_EASE_IN_OUT       0x1020U
#define GX_ANIMATION_QUAD_EASE_IN           0x1030U
#define GX_ANIMATION_QUAD_EASE_OUT          0x1040U
#define GX_ANIMATION_QUAD_EASE_IN_OUT       0x1050U
#define GX_ANIMATION_QUART_EASE_IN          0x1060U
#define GX_ANIMATION_QUART_EASE_OUT         0x1070U
#define GX_ANIMATION_QUART_EASE_IN_OUT      0x1080U
#define GX_ANIMATION_QUINT_EASE_IN          0x1090U
#define GX_ANIMATION_QUINT_EASE_OUT         0x10a0U
#define GX_ANIMATION_QUINT_EASE_IN_OUT      0x10b0U
#define GX_ANIMATION_SINE_EASE_IN           0x10c0U
#define GX_ANIMATION_SINE_EASE_OUT          0x10d0U
#define GX_ANIMATION_SINE_EASE_IN_OUT       0x10e0U

/* Sprite background operations */

#define GX_SPRITE_BACKGROUND_NO_ACTION      0x00     /* do nothing between frames            */
#define GX_SPRITE_BACKGROUND_SOLID_FILL     0x01     /* draw background with solid color     */
#define GX_SPRITE_BACKGROUND_RESTORE        0x02     /* draw background with first image     */


/* Define Font information.  */

#define GX_FONT_ALIASED                     0x01

/* Define Brush attributes.  */

#define GX_BRUSH_OUTLINE                    0x0000U            /* for rectangle, text, polygon              */
#define GX_BRUSH_SOLID_FILL                 0x0001U            /* For rectangle, text, polygon              */
#define GX_BRUSH_PIXELMAP_FILL              0x0002U            /* For rectangle, polygon                    */
#define GX_BRUSH_ALIAS                      0x0004U            /* For lines                                 */
#define GX_BRUSH_UNDERLINE                  0x0008U            /* Underlined text                           */
#define GX_BRUSH_ROUND                      0x0010U            /* For lines                                 */


/* Define Canvas information.  */

#define GX_CANVAS_SIMPLE                    0x01                /* Canvas is memory-only canvas (no display) */
#define GX_CANVAS_MANAGED                   0x02                /* canvas is composited to frame buffer      */
#define GX_CANVAS_VISIBLE                   0x04                /* Canvas is visible                         */
#define GX_CANVAS_MODIFIED                  0x08
#define GX_CANVAS_COMPOSITE                 0x20                /* Canvas is a buffer for creating composite*/
#define GX_CANVAS_PARTIAL_FRAME_BUFFER      0x40

/* Define common combinations.  */

#define GX_CANVAS_MANAGED_VISIBLE           (GX_CANVAS_MANAGED | GX_CANVAS_VISIBLE)
#define GX_ALPHA_VALUE_OPAQUE               255


/* Define pre-defined Widget Resource IDs.  */

#define GX_ID_VERTICAL_SCROLL               0xff00U
#define GX_ID_HORIZONTAL_SCROLL             0xff01U
#define GX_ID_SCROLL_UP_LEFT                0xff02U
#define GX_ID_SCROLL_DOWN_RIGHT             0xff03U
#define GX_ID_SCROLL_THUMB                  0xff04U
#define GX_ID_DROP_BUTTON                   0xff05U

/* pre-defined timer IDs */
#define GX_BUTTON_TIMER                     0xff00U
#define GX_CIRCULAR_GAUGE_TIMER             0xff01U
#define GX_RADIAL_SLIDER_TIMER              0xff02U

/* Define the echo mode of Single-line Text input.  */

#define GX_STAR_ECHO                        0x01
#define GX_ECHO                             0x02

/* Define image type.  */
#define GX_IMAGE_TYPE_BMP                   0x01
#define GX_IMAGE_TYPE_GIF                   0x02
#define GX_IMAGE_TYPE_PNG                   0x03
#define GX_IMAGE_TYPE_JPG                   0x04

/* Define action types for Studio generated screen flow */

#define   GX_ACTION_TYPE_ATTACH                 1
#define   GX_ACTION_TYPE_DETACH                 2
#define   GX_ACTION_TYPE_TOGGLE                 3
#define   GX_ACTION_TYPE_SHOW                   4
#define   GX_ACTION_TYPE_HIDE                   5
#define   GX_ACTION_TYPE_ANIMATION              6
#define   GX_ACTION_TYPE_WINDOW_EXECUTE         7
#define   GX_ACTION_TYPE_WINDOW_EXECUTE_STOP    8
#define   GX_ACTION_TYPE_SCREEN_STACK_PUSH      9
#define   GX_ACTION_TYPE_SCREEN_STACK_POP      10
#define   GX_ACTION_TYPE_SCREEN_STACK_RESET    11

/* Gradient Styles */
#define GX_GRADIENT_TYPE_VERTICAL   0x01
#define GX_GRADIENT_TYPE_ALPHA      0x02
#define GX_GRADIENT_TYPE_MIRROR     0x04

/* Mouse status flags */
#define GX_MOUSE_ENABLED                    0x01
#define GX_MOUSE_VISIBLE                    0x02


/* Text input cursor flags */
#define GX_CURSOR_USE_CUSTOM_HEIGHT         0x01

/* Text render stlyles */
#if defined(GX_THAI_GLYPH_SHAPING_SUPPORT)
#define GX_TEXT_RENDER_THAI_GLYPH_SHAPING   0x01
#endif

/* Language direction flags. */
#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
#define GX_LANGUAGE_DIRECTION_LTR           0x01
#define GX_LANGUAGE_DIRECTION_RTL           0x02
#endif

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

#if defined(GX_UTF8_SUPPORT)
#if defined(GX_EXTENDED_UNICODE_SUPPORT)
/* support 32 bit character codes */
typedef ULONG GX_CHAR_CODE;
#define GX_MAX_GLYPH_CODE 0x10ffff
#else
/* support 16 bit character codes */
typedef USHORT GX_CHAR_CODE;
#define GX_MAX_GLYPH_CODE 0xffff
#endif
#else
/* support 8 bit character codes */
typedef GX_UBYTE GX_CHAR_CODE;
#define GX_MAX_GLYPH_CODE 0xff
#endif

#ifndef GX_STRLEN
#define GX_STRLEN(a)                        strlen((char *)a)
#endif

#ifndef GX_STRCPY
#define GX_STRCPY(a, b)                     strcpy(a, b)
#endif

#ifndef GX_STRCAT
#define GX_STRCAT(a, b)                     strcat(a, b)
#endif

#define GX_SIGNAL(_a, _b)                   (((_a) << 8) | (_b))
#define IS_SIGNAL(_a)                       (_a >= GX_FIRST_SIGNAL && _a <= GX_LAST_SIGNAL)
#define GX_SWAP_VALS(_a, _b)                {_a ^= _b; _b ^= _a; _a ^= _b; }
#define GX_SEARCH_DEPTH_INFINITE            65535

#if defined(GX_MOUSE_SUPPORT)
#if !defined(GX_MOUSE_MAX_RESOLUTION)
#define GX_MOUSE_MAX_RESOLUTION 32
#endif
#endif 

#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
#define GX_PROMPT_BIDI_RESOLVED_TEXT_INFO               GX_BIDI_RESOLVED_TEXT_INFO *gx_prompt_bidi_resolved_text_info;
#define GX_TEXT_BUTTON_BIDI_RESOLVED_TEXT_INFO          GX_BIDI_RESOLVED_TEXT_INFO *gx_text_button_bidi_resolved_text_info;
#define GX_MULTI_LINE_TEXT_VIEW_BIDI_RESOLVED_TEXT_INFO GX_BIDI_RESOLVED_TEXT_INFO *gx_multi_line_text_view_bidi_resolved_text_info;
#define GX_TEXT_SCROLL_WHEEL_BIDI_RESOLVED_TEXT_INFO    GX_BIDI_RESOLVED_TEXT_INFO **gx_text_scroll_wheel_bidi_resolved_text_info;
#else
#define GX_PROMPT_BIDI_RESOLVED_TEXT_INFO
#define GX_TEXT_BUTTON_BIDI_RESOLVED_TEXT_INFO
#define GX_MULTI_LINE_TEXT_VIEW_BIDI_RESOLVED_TEXT_INFO
#define GX_TEXT_SCROLL_WHEEL_BIDI_RESOLVED_TEXT_INFO
#endif

#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define GX_STRING_SCROLL_WHEEL_STRING_LIST_DEPRECATED GX_CONST GX_CHAR **gx_string_scroll_wheel_string_list_deprecated;
#define GX_TEXT_SCROLL_WHEEL_TEXT_GET_DEPRECATED      GX_CONST GX_CHAR* (*gx_text_scroll_wheel_text_get_deprecated)(struct GX_TEXT_SCROLL_WHEEL_STRUCT*, INT);
#else
#define GX_STRING_SCROLL_WHEEL_STRING_LIST_DEPRECATED
#define GX_TEXT_SCROLL_WHEEL_TEXT_GET_DEPRECATED
#endif

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

/* Define macro for GX_GLYPH_STRUCT members. */
#define GX_GLYPH_MEMBERS_DECLARE                                                                    \
    GX_CONST GX_UBYTE *gx_glyph_map;          /* Pointer to glyph data                           */ \
    SHORT              gx_glyph_ascent;       /* Glyph ascent above baseline                     */ \
    SHORT              gx_glyph_descent;      /* Glyph descent below baseline                    */ \
    GX_UBYTE           gx_glyph_advance;      /* Glyph advance                                   */ \
    GX_BYTE            gx_glyph_leading;      /* Pen x-pre-advance                               */ \
    GX_UBYTE           gx_glyph_width;                                                              \
    GX_UBYTE           gx_glyph_height;                                                             

/* Define the Glyph type.  */
typedef struct GX_GLYPH_STRUCT
{
    GX_GLYPH_MEMBERS_DECLARE
} GX_GLYPH;

#if defined (GX_FONT_KERNING_SUPPORT)
/* Define the Kerning Glyph type.  */
typedef struct GX_KERNING_GLYPH_STRUCT
{
    GX_GLYPH_MEMBERS_DECLARE                                                   
    GX_CONST GX_UBYTE *gx_kerning_table;                   /* Kerning table */ 
} GX_KERNING_GLYPH;
#endif

/* Define the Compressed Glyph type.  */
typedef struct GX_COMPRESSED_GLYPH_STRUCT
{
    GX_GLYPH_MEMBERS_DECLARE                                                     
    USHORT             gx_glyph_map_size;                       /* Glyph size */
} GX_COMPRESSED_GLYPH;

/* Define the Font type.  */
typedef struct GX_FONT_STRUCT
{
    GX_UBYTE                     gx_font_format;                /* Format flags, defined below              */
    GX_UBYTE                     gx_font_prespace;              /* Line spacing above, pixels               */
    GX_UBYTE                     gx_font_postspace;             /* Line spacing below, pixels               */
    GX_UBYTE                     gx_font_line_height;           /* Font height, excluding spacing           */
    GX_UBYTE                     gx_font_baseline;              /* Font baseline                            */
    GX_CHAR_CODE                 gx_font_first_glyph;           /* First glyph on this page                 */
    GX_CHAR_CODE                 gx_font_last_glyph;            /* Last glyph on this page                  */
    union {
        GX_CONST GX_GLYPH            *gx_font_normal_glyphs;        /* Array of GX_GLYPH structures             */
        GX_CONST GX_COMPRESSED_GLYPH *gx_font_compressed_glyphs;    /* Array of GX_COMPRESSED_GLYPH structures  */
        #if defined (GX_FONT_KERNING_SUPPORT)
        GX_CONST GX_KERNING_GLYPH    *gx_font_kerning_glyphs;       /* Array of GX_KERNING_GLYPH structures     */
        #endif
    } gx_font_glyphs;
    GX_CONST struct GX_FONT_STRUCT *gx_font_next_page;         /* For multiple page fonts (Unicode)        */
} GX_FONT;

#define GX_FONT_FORMAT_BPP_MASK   0x03
#define GX_FONT_FORMAT_1BPP       0x00
#define GX_FONT_FORMAT_2BPP       0x01
#define GX_FONT_FORMAT_4BPP       0x02
#define GX_FONT_FORMAT_8BPP       0x03

#define GX_FONT_FORMAT_COMPRESSED     0x10
#define GX_FONT_FORMAT_FREETYPE       0x20
#define GX_FONT_FORMAT_KERNING        0x40
#define GX_FONT_FORMAT_REVERSED_ORDER 0x80  /* For 4bpp font, half bytes order reversed. For 1bpp font, bits order reversed with least signigicant bit in left. */
#define GX_FONT_FORMAT_ROTATED_90     0x04
#define GX_FONT_FORMAT_ROTATED_270    0x08

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
#define GX_PIXELMAP_DYNAMICALLY_ALLOCATED  0x20                 /* Pixelmap is dynamically allocated        */

#define GX_PIXELMAP_ROTATED_CW     0x40
#define GX_PIXELMAP_ROTATED_CCW    0x80

/* Deprecated definitions, provided only for backward compatibility */
#define GX_PIXELMAP_ROTATED_90  GX_PIXELMAP_ROTATED_CW
#define GX_PIXELMAP_ROTATED_270 GX_PIXELMAP_ROTATED_CCW

#define PIXELMAP_IS_TRANSPARENT(a) (a -> gx_pixelmap_flags & (GX_PIXELMAP_TRANSPARENT | GX_PIXELMAP_ALPHA))

#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
typedef struct GX_BIDI_TEXT_INFO_STRUCT
{
    GX_STRING gx_bidi_text_info_text;
    GX_FONT  *gx_bidi_text_info_font;
    GX_VALUE  gx_bidi_text_info_display_width;
    GX_BYTE   gx_bidi_text_info_direction;
} GX_BIDI_TEXT_INFO;

typedef struct GX_BIDI_RESOLVED_TEXT_INFO_STRUCT
{
    GX_STRING                                *gx_bidi_resolved_text_info_text;
    UINT                                      gx_bidi_resolved_text_info_total_lines;
    struct GX_BIDI_RESOLVED_TEXT_INFO_STRUCT *gx_bidi_resolved_text_info_next;
} GX_BIDI_RESOLVED_TEXT_INFO;
#endif

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



/* forward reference */
struct GX_WIDGET_STRUCT;
struct GX_TEXT_SCROLL_WHEEL_STRUCT;

/* Define Dirty Area type.  */
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
    INT      gx_scroll_value;
    INT      gx_scroll_minimum;
    INT      gx_scroll_maximum;
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
    GX_VALUE gx_slider_info_needle_width;
    GX_VALUE gx_slider_info_needle_height;
    GX_VALUE gx_slider_info_needle_inset;
    GX_VALUE gx_slider_info_needle_hotspot_offset;
} GX_SLIDER_INFO;


typedef struct GX_PIXELMAP_SLIDER_INFO_STRUCT
{
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
    GX_RESOURCE_ID gx_progress_bar_disabled_text_color;
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
    GX_RESOURCE_ID gx_radial_progress_bar_info_disabled_text_color;
    GX_VALUE       gx_radial_progress_bar_info_normal_brush_width;     /* width of lower track.  */
    GX_VALUE       gx_radial_progress_bar_info_selected_brush_width;   /* width of upper track.  */
    GX_RESOURCE_ID gx_radial_progress_bar_info_normal_brush_color;     /* resource ID of color for lower track.  */
    GX_RESOURCE_ID gx_radial_progress_bar_info_selected_brush_color;   /* resource ID of color for upper track.  */
    
} GX_RADIAL_PROGRESS_BAR_INFO;

/* Define radial slider information structure, this structure is used to
   define the appearance of the radial slider widget.  */
typedef struct GX_RADIAL_SLIDER_INFO_STRUCT
{
    GX_VALUE       gx_radial_slider_info_xcenter;
    GX_VALUE       gx_radial_slider_info_ycenter;
    USHORT         gx_radial_slider_info_radius;
    USHORT         gx_radial_slider_info_track_width;
    GX_VALUE       gx_radial_slider_info_needle_offset;
    GX_VALUE       gx_radial_slider_info_current_angle;
    GX_VALUE       gx_radial_slider_info_min_angle;
    GX_VALUE       gx_radial_slider_info_max_angle;
    GX_VALUE      *gx_radial_slider_info_angle_list;
    USHORT         gx_radial_slider_info_list_count;
    GX_RESOURCE_ID gx_radial_slider_info_background_pixelmap;
    GX_RESOURCE_ID gx_radial_slider_info_needle_pixelmap;
}GX_RADIAL_SLIDER_INFO;

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

/* GX_GRADIENT structure. This structure is used to support
   runtime gradient pixelmap creation  */
typedef struct GX_GRADIENT_STRUCT {
    struct GX_GRADIENT_STRUCT *gx_gradient_next;
    struct GX_GRADIENT_STRUCT *gx_gradient_previous;
    UCHAR    gx_gradient_type;
    UCHAR    gx_gradient_alpha_start;
    UCHAR    gx_gradient_alpha_end;
    GX_FIXED_VAL  gx_gradient_alpha_step;
    GX_FIXED_VAL  gx_gradient_alpha_current;
    GX_PIXELMAP   gx_gradient_pixelmap;
} GX_GRADIENT;


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
#if defined GX_ENABLE_CANVAS_PARTIAL_FRAME_BUFFER
    GX_VALUE                  gx_draw_context_offset_x;
    GX_VALUE                  gx_draw_context_offset_y;
#endif
} GX_DRAW_CONTEXT;

/* Define fill map info type.  */
typedef struct GX_FILL_PIXELMAP_INFO_STRUCT
{
    GX_PIXELMAP *pixelmap;
    GX_BOOL      draw;
    GX_UBYTE    *current_pixel_ptr;
    GX_UBYTE    *current_aux_ptr;
    GX_UBYTE     mask;
    INT          x_offset;
    INT          y_offset;
}GX_FILL_PIXELMAP_INFO;

/* Display layer services structure type. These services may be provided by display drivers
   support multiple hardware graphics layers. */


typedef struct GX_DISPLAY_LAYER_SERVICES_STRUCT
{
    UINT (*gx_display_layer_initialize)(INT layer, struct GX_CANVAS_STRUCT *canvas);
    VOID (*gx_display_layer_show)(INT layer);
    VOID (*gx_display_layer_hide)(INT layer);
    VOID (*gx_display_layer_alpha_set)(INT layer, GX_UBYTE alpha);
    VOID (*gx_display_layer_offset_set)(INT layer, GX_VALUE xoffset, GX_VALUE yoffset);
    VOID (*gx_display_layer_active_display_area_set)(INT layer, GX_RECTANGLE *size);
} GX_DISPLAY_LAYER_SERVICES;

#if defined(GX_MOUSE_SUPPORT)
typedef struct GX_MOUSE_CURSOR_INFO_STRUCT
{
    GX_RESOURCE_ID            gx_mouse_cursor_image_id;
    GX_VALUE                  gx_mouse_cursor_hotspot_x;
    GX_VALUE                  gx_mouse_cursor_hotspot_y;
} GX_MOUSE_CURSOR_INFO;

typedef struct GX_DISPLAY_MOUSE_STRUCT
{
    GX_MOUSE_CURSOR_INFO     *gx_mouse_cursor_info;
    struct GX_CANVAS_STRUCT  *gx_mouse_canvas;
    GX_POINT                  gx_mouse_position;
    GX_UBYTE                  gx_mouse_status;

    #if !defined(GX_HARDWARE_MOUSE_SUPPORT)
    GX_RECTANGLE              gx_mouse_rect;
    GX_UBYTE                 *gx_mouse_capture_memory;
    #endif
} GX_DISPLAY_MOUSE;
#endif

/* Define the display control block structure.  */
typedef struct GX_DISPLAY_STRUCT
{
    /* Define the display ID and name. The display ID is used simply to see if the
       control block is valid.  */
    ULONG                     gx_display_id;                    /* Control block ID GX_DISPLAY_ID           */
    ULONG                     gx_display_handle;                /* used to identify unique display instance */
    GX_CONST GX_CHAR         *gx_display_name;                  /* Pointer to display's name                */

    struct GX_DISPLAY_STRUCT *gx_display_created_next;          /* Pointer to next control block            */
    struct GX_DISPLAY_STRUCT *gx_display_created_previous;      /* Pointer to previous control block        */

    GX_COLOR                 *gx_display_color_table;           /* color ID to native value mapping table   */
    GX_PIXELMAP             **gx_display_pixelmap_table;        /* pixelmap ID to GX_PIXELMAP mapping table */
    GX_FONT                 **gx_display_font_table;            /* font ID to GX_FONT mapping table         */
    GX_COLOR                 *gx_display_palette;               /* only used for 8-bpp palette mode driver  */

#if defined(GX_ENABLE_DEPRECATED_STRING_API)
    GX_CONST GX_CHAR       ***gx_display_language_table_deprecated;
#endif
    GX_CONST GX_STRING      **gx_display_language_table;        /* Define the language table.               */
#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
    GX_CONST GX_UBYTE        *gx_display_language_direction_table; /* Define the langauge direction table.  */
#endif
    UINT                      gx_display_color_table_size;
    UINT                      gx_display_pixelmap_table_size;
    UINT                      gx_display_font_table_size;
    UINT                      gx_display_string_table_size;
    UINT                      gx_display_palette_size;          /* only used for 8-bpp palette mode driver */

    GX_UBYTE                  gx_display_color_format;
    GX_UBYTE                  gx_display_active_language;       /* Define the active language.              */
    GX_UBYTE                  gx_display_language_table_size;
    GX_UBYTE                  gx_display_driver_ready;
    USHORT                    gx_display_rotation_angle;

    GX_VALUE                  gx_display_width;
    GX_VALUE                  gx_display_height;

    VOID                     *gx_display_driver_data;
    VOID                     *gx_display_accelerator;           /* graphics accelerator handle/instance */

    GX_DISPLAY_LAYER_SERVICES *gx_display_layer_services;       /* optional additional hardware graphics layer services */

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
    VOID                     (*gx_display_driver_horizontal_pixelmap_line_draw)(GX_DRAW_CONTEXT *context, INT xstart, INT xend, INT y, GX_FILL_PIXELMAP_INFO *info);
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
                                                                  GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph);

    /* Define driver function pointer for drawing aliased 4bit glyph (may be NULL).  */
    VOID                     (*gx_display_driver_4bit_glyph_draw)(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area,
                                                                  GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph);

    /* Define driver function pointer for drawing 1bit (monochrome) glyph.  */
    VOID                     (*gx_display_driver_1bit_glyph_draw)(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area,
                                                                  GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph);

    /* Define driver function pointer for drawing aliased 8bit compressed glyph (may be NULL).  */
    VOID                     (*gx_display_driver_8bit_compressed_glyph_draw)(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area,
                                                                             GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph);

    /* Define driver function pointer for drawing aliased 4bit compressed glyph (may be NULL).  */
    VOID                     (*gx_display_driver_4bit_compressed_glyph_draw)(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area,
                                                                             GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph);

    /* Define driver function pointer for drawing 1bit (monochrome) compressed glyph.  */
    VOID                     (*gx_display_driver_1bit_compressed_glyph_draw)(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area,
                                                                             GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph);

    VOID                     (*gx_display_driver_callback_assign)(UINT(*wait_func)(VOID *), VOID *);

#if defined(GX_ARC_DRAWING_SUPPORT)

    /* Define driver function pointer for drawing circle.  */
    VOID                     (*gx_display_driver_circle_draw)(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r);

    /* Define driver function pointer for drawing anti-aliased circle.  */
    VOID                     (*gx_display_driver_anti_aliased_circle_draw)(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r);

    /* Define driver function pointer for drawing circle with wide outlines.  */
    VOID                     (*gx_display_driver_wide_circle_draw)(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r);

    /* Define driver function pointer for drawing anti-aliased circle with wide outlines.  */
    VOID                     (*gx_display_driver_anti_aliased_wide_circle_draw)(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r);

    /* Define driver function pointer for filling circle shape.  */
    VOID                     (*gx_display_driver_circle_fill)(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r);


    /* Define driver function pointer for drawing circle arc. */
    VOID                     (*gx_display_driver_arc_draw)(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r, INT start_angle, INT end_angle);

    /* Define driver function pointer for drawing anti-aliased circle arc.  */
    VOID                     (*gx_display_driver_anti_aliased_arc_draw)(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r, INT start_angle, INT end_angle);

    /* Define driver function pointer for drawing circle arc with wide outlines. */
    VOID                     (*gx_display_driver_wide_arc_draw)(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r, INT start_angle, INT end_angle);

    /* Define driver function pointer for drawing anti-aliased circle arc with wide outlines. */
    VOID                     (*gx_display_driver_anti_aliased_wide_arc_draw)(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r, INT start_angle, INT end_angle);

    /* Define driver function pointer for filling circle arc shape.  */
    VOID                     (*gx_display_driver_arc_fill)(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r, INT start_angle, INT end_angle);

    /* Define driver function pointer for filling pie shape.  */
    VOID                     (*gx_display_driver_pie_fill)(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r, INT start_angle, INT end_angle);

    /* Define driver function pointer for drawing ellipse.  */
    VOID                     (*gx_display_driver_ellipse_draw)(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, INT a, INT b);

    /* Define driver function pointer for drawing anti-aliased ellipse.  */
    VOID                     (*gx_display_driver_anti_aliased_ellipse_draw)(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, INT a, INT b);

    /* Define driver function pointer for drawing ellipse with wide outlines.  */
    VOID                     (*gx_display_driver_wide_ellipse_draw)(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, INT a, INT b);

    /* Define driver function pointer for drawing anti-aliased ellipse with wide outlines.  */
    VOID                     (*gx_display_driver_anti_aliased_wide_ellipse_draw)(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, INT a, INT b);

    /* Define driver function pointer for drawing a filled ellipse.  */
    VOID                     (*gx_display_driver_ellipse_fill)(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, INT a, INT b);
#endif

#if defined (GX_MOUSE_SUPPORT)
    GX_DISPLAY_MOUSE        gx_display_mouse;
    /* Define driver function pointer for setting pixelmap for mouse. */
    VOID                    (*gx_display_mouse_define)(struct GX_DISPLAY_STRUCT *display, struct GX_CANVAS_STRUCT *canvas, GX_MOUSE_CURSOR_INFO *info);
    VOID                    (*gx_display_mouse_position_set)(struct GX_DISPLAY_STRUCT *display,  GX_POINT *pos);
    VOID                    (*gx_display_mouse_enable)(struct GX_DISPLAY_STRUCT *display, GX_BOOL enable);
#if !defined(GX_HARDWARE_MOUSE_SUPPORT)
    VOID                    (*gx_display_mouse_capture)(struct GX_DISPLAY_STRUCT *display);
    VOID                    (*gx_display_mouse_restore)(struct GX_DISPLAY_STRUCT *display);
    VOID                    (*gx_display_mouse_draw)(struct GX_DISPLAY_STRUCT *display);
#endif
#endif

} GX_DISPLAY;


/* Define Canvas type.  */

typedef struct GX_CANVAS_STRUCT
{
    /* Define the canvas ID and name. The canvas ID is used simply to see if the
       control block is valid.  */
    ULONG               gx_canvas_id;                            /* Control block ID                         */
    GX_CONST GX_CHAR   *gx_canvas_name;                          /* Pointer to canvas name                   */

    /* Define pointers to the next and previous canvases in the created list.  */
    struct GX_CANVAS_STRUCT *gx_canvas_created_next,
                            *gx_canvas_created_previous;

    /* Define the canvas memory pointer.  */
    GX_COLOR *gx_canvas_memory;
#ifdef GX_ENABLE_CANVAS_PARTIAL_FRAME_BUFFER
    GX_VALUE  gx_canvas_memory_width;
    GX_VALUE  gx_canvas_memory_height;
    GX_VALUE  gx_canvas_memory_offset_x;
    GX_VALUE  gx_canvas_memory_offset_y;
#endif

    /* padded canvas memory buffer, only needed for Win32 */
    #ifdef GX_TARGET_WIN32
    GX_COLOR *gx_canvas_padded_memory;
    #endif

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
    GX_BYTE       gx_canvas_hardware_layer;
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
    GX_UBYTE gx_text_input_cursor_flags;

    /* Cursor position information, managed by the cursor control code.*/
    GX_POINT gx_text_input_cursor_pos;
} GX_TEXT_INPUT_CURSOR;

/* Define pen configuration structure. */
typedef struct GX_PEN_CONFIGURATION_STRUCT
{
    GX_FIXED_VAL gx_pen_configuration_min_drag_dist;
    UINT         gx_pen_configuration_max_pen_speed_ticks;
} GX_PEN_CONFIGURATION;

#define GX_RESOURCE_HEADER_SIZE      20
#define GX_THEME_HEADER_SIZE         114
#define GX_COLOR_HEADER_SIZE         8
#define GX_PALETTE_HEADER_SIZE       8
#define GX_FONT_HEADER_SIZE          16
#define GX_PAGE_HEADER_SIZE          21
#define GX_GLYPH_HEADER_SIZE         22
#define GX_KERNING_GLYPH_HEADER_SIZE 24
#define GX_PIXELMAP_HEADER_SIZE      32
#define GX_STRING_HEADER_SIZE        10
#define GX_LANGUAGE_HEADER_SIZE      72
#define GX_LANGUAGE_HEADER_NAME_SIZE 64

/* Degine binary resource header type. */
typedef struct GX_RESOURCE_HEADER_STRUCT{
    USHORT gx_resource_header_magic_number;
    USHORT gx_resource_header_version;
    USHORT gx_resource_header_theme_count;
    USHORT gx_resource_header_language_count;
    ULONG  gx_resource_header_theme_data_size;
    ULONG  gx_resource_header_string_data_size;
    ULONG  gx_resource_header_data_size;
} GX_RESOURCE_HEADER;

/* Define binary theme header type. */
typedef struct GX_THEME_HEADER_STRUCT{
    USHORT                  gx_theme_header_magic_number;
    USHORT                  gx_theme_header_index;
    USHORT                  gx_theme_header_color_count;
    USHORT                  gx_theme_header_palette_count;
    USHORT                  gx_theme_header_font_count;
    USHORT                  gx_theme_header_pixelmap_count;
    GX_SCROLLBAR_APPEARANCE gx_theme_header_vscroll_appearance;
    GX_SCROLLBAR_APPEARANCE gx_theme_header_hscroll_appearance;
    ULONG                   gx_theme_header_vscroll_style;
    ULONG                   gx_theme_header_hscroll_style;
    ULONG                   gx_theme_header_color_data_size;
    ULONG                   gx_theme_header_palette_data_size;
    ULONG                   gx_theme_header_font_data_size;
    ULONG                   gx_theme_header_pixelmap_data_size;
    ULONG                   gx_theme_header_data_size;
} GX_THEME_HEADER;

/* Define binary color header type. */
typedef struct GX_COLOR_HEADER_STRUCT{
    USHORT gx_color_header_magic_number;
    USHORT gx_color_header_color_count;
    ULONG  gx_color_header_data_size;
}GX_COLOR_HEADER;

/* Define binary palette header type.  */
typedef struct GX_PALETTE_HEADER_STRUCT{
    USHORT gx_palette_header_magic_number;
    USHORT gx_palette_header_color_count;
    ULONG  gx_palette_header_data_size;
}GX_PALETTE_HEADER;

/* Define font header type. */
typedef struct GX_FONT_HEADER_STRUCT{
    USHORT    gx_font_header_magic_number;
    USHORT    gx_font_header_index;
    USHORT    gx_font_header_page_count;
    GX_UBYTE  gx_font_header_deault;
    GX_UBYTE  gx_font_header_bits;
    ULONG     gx_font_header_data_size;
    ULONG     gx_font_header_data_offset;
}GX_FONT_HEADER;

/* Define font page header type. */
typedef struct GX_PAGE_HEADER_STRUCT{
    USHORT        gx_page_header_magic_number;
    USHORT        gx_page_header_index;
    GX_UBYTE      gx_page_header_format;
    GX_UBYTE      gx_page_header_prespace;
    GX_UBYTE      gx_page_header_postspace;
    GX_UBYTE      gx_page_header_line_height;
    GX_UBYTE      gx_page_header_baseline;
    GX_CHAR_CODE  gx_page_header_first_glyph;
    GX_CHAR_CODE  gx_page_header_last_glyph;
    ULONG         gx_page_header_data_size;
}GX_PAGE_HEADER;

/* define glyph header type members. */
#define GX_GLYPH_HEADER_MEMBERS_DECLARE                                              \
    USHORT    gx_glyph_header_magic_number;                                          \
    USHORT    gx_glyph_header_map_size;                                              \
    ULONG     gx_glyph_header_map_offset;                                            \
    USHORT    gx_glyph_header_index;                                                 \
    SHORT     gx_glyph_header_ascent;                                                \
    SHORT     gx_glyph_header_descent;                                               \
    GX_UBYTE  gx_glyph_header_advance;                                               \
    GX_BYTE   gx_glyph_header_leading;                                               \
    GX_UBYTE  gx_glyph_header_width;                                                 \
    GX_UBYTE  gx_glyph_header_height;

/* Used for Guix after version 5.3.4(include). */
typedef struct GX_GLYPH_HEADER_STRUCT {
    GX_GLYPH_HEADER_MEMBERS_DECLARE
    ULONG     gx_glyph_header_data_size;
    USHORT    gx_glyph_header_kerning_table_size;
}GX_KERNING_GLYPH_HEADER;

/* define glyph header type. */
/* Used for Guix berfore Kerning. */
typedef struct GX_GLYPH_HEADER_BEFORE_KERNING_STRUCT {
    GX_GLYPH_HEADER_MEMBERS_DECLARE
    ULONG     gx_glyph_header_data_size;
}GX_GLYPH_HEADER;

/* Define pixelmap header type. */
typedef struct GX_PIXELMAP_HEADER_STRUCT{
    USHORT      gx_pixelmap_header_magic_number;
    USHORT      gx_pixelmap_header_index;
    GX_UBYTE    gx_pixelmap_header_version_major;
    GX_UBYTE    gx_pixelmap_header_version_minor;
    GX_UBYTE    gx_pixelmap_header_flags;
    GX_UBYTE    gx_pixelmap_header_format;
    ULONG       gx_pixelmap_header_map_size;
    ULONG       gx_pixelmap_header_aux_data_size;
    GX_COLOR    gx_pixelmap_header_transparent_color;
    USHORT      gx_pixelmap_header_width;
    USHORT      gx_pixelmap_header_height;
    ULONG       gx_pixelmap_header_data_size;
    ULONG       gx_pixelmap_header_data_offset;
}GX_PIXELMAP_HEADER;

/* Define string header type. */
typedef struct GX_STRING_HEADER_STRUCT{
    USHORT gx_string_header_magic_number;
    USHORT gx_string_header_language_count;
    USHORT gx_string_header_string_count;
    ULONG  gx_string_header_data_size;
}GX_STRING_HEADER;

/* Define language header type. */
typedef struct GX_LANGUAGE_HEADER_STRUCT{
    USHORT gx_language_header_magic_number;
    USHORT gx_language_header_index;
    UCHAR  gx_language_header_name[GX_LANGUAGE_HEADER_NAME_SIZE];
    ULONG  gx_language_header_data_size;
}GX_LANGUAGE_HEADER;

/* Define rich tet view fonts structure. */
typedef struct GX_RICH_TEXT_FONTS_STRUCT{
    GX_RESOURCE_ID gx_rich_text_fonts_normal_id;
    GX_RESOURCE_ID gx_rich_text_fonts_bold_id;
    GX_RESOURCE_ID gx_rich_text_fonts_italic_id;
    GX_RESOURCE_ID gx_rich_text_fonts_bold_italic_id;
}GX_RICH_TEXT_FONTS;

/* Define rich text flags. */
#define GX_RICH_TEXT_BOLD         0x01
#define GX_RICH_TEXT_ITALIC       0x02
#define GX_RICH_TEXT_UNDERLINE    0x04
#define GX_RICH_TEXT_LEFT         0x00
#define GX_RICH_TEXT_CENTER       0x10
#define GX_RICH_TEXT_RIGHT        0x20
#define GX_RICH_TEXT_ALIGN_MASK   0x30

/* Define rich text view draw style structure. */
typedef struct GX_RICH_TEXT_FORMAT_STRUCT
{
    GX_RESOURCE_ID gx_rich_text_color;
    GX_RESOURCE_ID gx_rich_text_highlight_color;
    GX_RESOURCE_ID gx_rich_text_font_id;
    GX_UBYTE       gx_rich_text_flags;
}GX_RICH_TEXT_FORMAT;

/* Define rich text context structure. */
typedef struct GX_RICH_TEXT_CONTEXT_STRUCT
{
    GX_RICH_TEXT_FORMAT gx_rich_text_context_format;
    GX_CONST GX_STRING *gx_rich_text_context_tag;
}GX_RICH_TEXT_CONTEXT;

/* Define rich text context stack structure. */
typedef struct GX_RICH_TEXT_CONTEXT_STACK_STRUCT
{
    GX_RICH_TEXT_CONTEXT gx_rich_text_context_stack[GX_MAX_RICH_TEXT_CONTEXT_NESTING];
    GX_UBYTE             gx_rich_text_context_stack_top;
} GX_RICH_TEXT_CONTEXT_STACK;

/* Define macro for GX_WIDGET members.  */

#if defined(GX_WIDGET_USER_DATA)
#define GX_USER_DATA_FIELD INT gx_widget_user_data;
#else
#define GX_USER_DATA_FIELD
#endif

/* Define macro for GX_WIDGET and GX_RADIAL_PROGRESS_BAR members.  */
#if defined(GX_BRUSH_ALPHA_SUPPORT)
#define GX_WIDGET_ALPHA_FIELD GX_UBYTE gx_widget_alpha;
#define GX_RADIAL_PROGRESS_BAR_CANVAS_FIELD GX_CANVAS gx_radial_progress_bar_canvas;
#else
#define GX_WIDGET_ALPHA_FIELD
#define GX_RADIAL_PROGRESS_BAR_CANVAS_FIELD
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
    UINT                   (*gx_widget_event_process_function)(struct GX_WIDGET_STRUCT *, GX_EVENT *);       \
    VOID                   (*gx_widget_draw_function)(struct GX_WIDGET_STRUCT *);                            \
    ULONG                    gx_widget_status;                                                               \
    ULONG                    gx_widget_style;                                                                \
    GX_RESOURCE_ID           gx_widget_normal_fill_color;                                                    \
    GX_RESOURCE_ID           gx_widget_selected_fill_color;                                                  \
    GX_RESOURCE_ID           gx_widget_disabled_fill_color;                                                  \
    USHORT                   gx_widget_type;                                                                 \
    USHORT                   gx_widget_id;                                                                   \
    GX_USER_DATA_FIELD                                                                                       \
    GX_WIDGET_ALPHA_FIELD


/* Define macro for GX_PROMPT, based on GX_WIDGET.  */

#define GX_PROMPT_MEMBERS_DECLARE                                                           \
    GX_WIDGET_MEMBERS_DECLARE                                                               \
    GX_STRING         gx_prompt_string;                                                     \
    VOID            (*gx_prompt_text_get_function)(struct GX_PROMPT_STRUCT *, GX_STRING *); \
    GX_RESOURCE_ID    gx_prompt_text_id;                                                    \
    GX_RESOURCE_ID    gx_prompt_font_id;                                                    \
    GX_RESOURCE_ID    gx_prompt_normal_text_color;                                          \
    GX_RESOURCE_ID    gx_prompt_selected_text_color;                                        \
    GX_RESOURCE_ID    gx_prompt_disabled_text_color;                                        \
    GX_PROMPT_BIDI_RESOLVED_TEXT_INFO

#define GX_NUMERIC_PROMPT_MEMBERS_DECLARE                                              \
    GX_PROMPT_MEMBERS_DECLARE                                                          \
    GX_CHAR gx_numeric_prompt_buffer[GX_NUMERIC_PROMPT_BUFFER_SIZE];                   \
    VOID(*gx_numeric_prompt_format_function)(struct GX_NUMERIC_PROMPT_STRUCT *, INT);

/* Define macro for PIXELMAP_PROMPT, based on GX_PROMPT.   */

#define GX_PIXELMAP_PROMPT_MEMBERS_DECLARE                  \
    GX_PROMPT_MEMBERS_DECLARE                               \
    GX_RESOURCE_ID gx_normal_left_pixelmap_id;              \
    GX_RESOURCE_ID gx_normal_right_pixelmap_id;             \
    GX_RESOURCE_ID gx_normal_fill_pixelmap_id;              \
    GX_RESOURCE_ID gx_selected_left_pixelmap_id;            \
    GX_RESOURCE_ID gx_selected_right_pixelmap_id;           \
    GX_RESOURCE_ID gx_selected_fill_pixelmap_id;

/* Defube macro for NUMERIC_PIXEMAP_PROMPT, based on GX_PIXELMAP_PROMPT. */
#define GX_NUMERIC_PIXELMAP_PROMPT_MEMBERS_DECLARE                            \
    GX_PIXELMAP_PROMPT_MEMBERS_DECLARE                                        \
    GX_CHAR gx_numeric_pixelmap_prompt_buffer[GX_NUMERIC_PROMPT_BUFFER_SIZE]; \
    VOID(*gx_numeric_pixelmap_prompt_format_function)(struct GX_NUMERIC_PIXELMAP_PROMPT_STRUCT *, INT);

/* Define macro for GX_SINGLE_LINE_TEXT_INPUT, based on GX_PROMPT.  */

#define GX_SINGLE_LINE_TEXT_INPUT_MEMBERS_DECLARE                       \
    GX_PROMPT_MEMBERS_DECLARE                                           \
    GX_CHAR             *gx_single_line_text_input_buffer;              \
    UINT                 gx_single_line_text_input_buffer_size;         \
    UINT                 gx_single_line_text_input_string_size;         \
    GX_VALUE             gx_single_line_text_input_xoffset;             \
    GX_VALUE             gx_single_line_text_input_yoffset;             \
    UINT                 gx_single_line_text_input_insert_pos;          \
    UINT                 gx_single_line_text_input_was_modified;        \
    GX_TEXT_INPUT_CURSOR gx_single_line_text_input_cursor_instance;     \
    GX_RESOURCE_ID       gx_single_line_text_input_readonly_text_color; \
    GX_RESOURCE_ID       gx_single_line_text_input_readonly_fill_color; \
    UINT                 gx_single_line_text_input_start_mark;          \
    UINT                 gx_single_line_text_input_end_mark;            \


/* Define macro for GX_ICON, based on GX_WIDGET.  */

#define GX_ICON_MEMBERS_DECLARE                  \
    GX_WIDGET_MEMBERS_DECLARE                    \
    GX_RESOURCE_ID gx_icon_normal_pixelmap;      \
    GX_RESOURCE_ID gx_icon_selected_pixelmap;

/* Define macro for GX_SPRITE, based on GX_WIDGET. */

#define GX_SPRITE_MEMBERS_DECLARE           \
    GX_WIDGET_MEMBERS_DECLARE               \
    GX_SPRITE_FRAME * gx_sprite_frame_list; \
    USHORT gx_sprite_frame_count;           \
    USHORT gx_sprite_current_frame;         \
    UCHAR  gx_sprite_run_state;


/* Define macro for GX_BUTTON, based on GX_WIDGET.  */

#define GX_BUTTON_MEMBERS_DECLARE                         \
    GX_WIDGET_MEMBERS_DECLARE                             \
    VOID (*gx_button_select_handler)(GX_WIDGET *button);  \
    VOID (*gx_button_deselect_handler)(GX_WIDGET *button, GX_BOOL gen_event);


/* Define macro for GX_TEXT_BUTTON, based on GX_BUTTON.  */

#define GX_TEXT_BUTTON_MEMBERS_DECLARE                     \
    GX_BUTTON_MEMBERS_DECLARE                              \
    GX_RESOURCE_ID     gx_text_button_text_id;             \
    GX_RESOURCE_ID     gx_text_button_font_id;             \
    GX_STRING          gx_text_button_string;              \
    GX_RESOURCE_ID     gx_text_button_normal_text_color;   \
    GX_RESOURCE_ID     gx_text_button_selected_text_color; \
    GX_RESOURCE_ID     gx_text_button_disabled_text_color; \
    GX_TEXT_BUTTON_BIDI_RESOLVED_TEXT_INFO

/* Define macro for GX_MULTI_LINE_TEXT_BUTTON, based on GX_TEXT_BUTTON.  */

#define GX_MULTI_LINE_TEXT_BUTTON_MEMBERS_DECLARE                                   \
    GX_TEXT_BUTTON_MEMBERS_DECLARE                                                  \
    INT      gx_multi_line_text_button_line_count;                                  \
    GX_STRING gx_multi_line_text_button_lines[GX_MULTI_LINE_TEXT_BUTTON_MAX_LINES];

/* Define macro for GX_RADIO_BUTTON, based on GX_TEXT_BUTTON.  */

#define GX_RADIO_BUTTON_MEMBERS_DECLARE                          \
    GX_TEXT_BUTTON_MEMBERS_DECLARE                               \
    GX_RESOURCE_ID gx_radio_button_off_pixelmap_id;              \
    GX_RESOURCE_ID gx_radio_button_on_pixelmap_id;               \
    GX_RESOURCE_ID gx_radio_button_off_disabled_pixelmap_id;     \
    GX_RESOURCE_ID gx_radio_button_on_disabled_pixelmap_id;


/* Define macro for GX_CHECKBOX, based on GX_TEXT_BUTTON.  */

#define GX_CHECKBOX_MEMBERS_DECLARE                                \
    GX_TEXT_BUTTON_MEMBERS_DECLARE                                 \
    GX_RESOURCE_ID gx_checkbox_unchecked_pixelmap_id;              \
    GX_RESOURCE_ID gx_checkbox_checked_pixelmap_id;                \
    GX_RESOURCE_ID gx_checkbox_unchecked_disabled_pixelmap_id;     \
    GX_RESOURCE_ID gx_checkbox_checked_disabled_pixelmap_id;

/* Define macro for GX_ICON_BUTTON, based ON GX_BUTTON.  */

#define GX_ICON_BUTTON_MEMBERS_DECLARE \
    GX_BUTTON_MEMBERS_DECLARE          \
    GX_RESOURCE_ID  gx_icon_button_icon_id;

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
    GX_CANVAS *gx_window_root_canvas;                    \
    GX_BOOL gx_window_root_views_changed;

/* Define macro for GX_SCROLL_THUMB, based on GX_BUTTON.  */
#define GX_SCROLL_THUMB_MEMBERS_DECLARE                       \
    GX_BUTTON_MEMBERS_DECLARE                                 \
    GX_RESOURCE_ID gx_scroll_thumb_pixelmap;                  \
    GX_RESOURCE_ID gx_scroll_thumb_border_color;              \
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
    GX_SLIDER_INFO gx_slider_info;              \
    INT            gx_slider_tick_count;        \
    GX_RESOURCE_ID gx_slider_tick_color;


/* Define macro for GX_PIXELMAP_SLIDER, based on GX_WIDGET.  */
#define GX_PIXELMAP_SLIDER_MEMBERS_DECLARE               \
    GX_SLIDER_MEMBERS_DECLARE                            \
    GX_PIXELMAP_SLIDER_INFO gx_pixelmap_slider_pixelmap_info;


/* Define macro for GX_VERTICAL_LIST, based on GX_WINDOW.  */
#define GX_VERTICAL_LIST_MEMBERS_DECLARE                                                                                      \
    GX_WINDOW_MEMBERS_DECLARE                                                                                                 \
    VOID    (*gx_vertical_list_callback)(struct GX_VERTICAL_LIST_STRUCT *, GX_WIDGET *widget, INT index);                     \
    INT      gx_vertical_list_selected;                                                                                       \
    INT      gx_vertical_list_pen_index;                                                                                      \
    INT      gx_vertical_list_top_index;                                                                                      \
    INT      gx_vertical_list_total_rows;                                                                                     \
    GX_VALUE gx_vertical_list_seperation;                                                                                     \
    GX_VALUE gx_vertical_list_visible_rows;                                                                                   \
    GX_VALUE gx_vertical_list_child_count;                                                                                    \
    GX_VALUE gx_vertical_list_child_height;                                                                                   \
    GX_VALUE gx_vertical_list_snap_back_distance;                                                                             \
    GX_WIDGET *gx_vertical_list_idle_child_list;

/* Define macro for GX_VERTICAL_LIST, based on GX_WINDOW.  */
#define GX_HORIZONTAL_LIST_MEMBERS_DECLARE                                                                                        \
    GX_WINDOW_MEMBERS_DECLARE                                                                                                     \
    VOID    (*gx_horizontal_list_callback)(struct GX_HORIZONTAL_LIST_STRUCT *, GX_WIDGET *widget, INT index);                     \
    INT      gx_horizontal_list_selected;                                                                                         \
    INT      gx_horizontal_list_pen_index;                                                                                        \
    INT      gx_horizontal_list_top_index;                                                                                        \
    INT      gx_horizontal_list_total_columns;                                                                                    \
    GX_VALUE gx_horizontal_list_seperation;                                                                                       \
    GX_VALUE gx_horizontal_list_visible_columns;                                                                                  \
    GX_VALUE gx_horizontal_list_child_count;                                                                                      \
    GX_VALUE gx_horizontal_list_child_width;                                                                                      \
    GX_VALUE gx_horizontal_list_snap_back_distance;                                                                               \
    GX_WIDGET *gx_horizontal_list_idle_child_list;

#define GX_POPUP_LIST_MEMBERS_DECLARE           \
    GX_VERTICAL_LIST gx_popup_list_list;        \
    GX_WIDGET *gx_popup_list_owner;

/* Define macro for GX_DROP_LIST, based on GX_WIDGET  */
#define GX_DROP_LIST_MEMBERS_DECLARE                \
    GX_WIDGET_MEMBERS_DECLARE                       \
    GX_POPUP_LIST  gx_drop_list_popup;              \
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
    GX_RESOURCE_ID gx_multi_line_text_view_disabled_text_color;  \
    UINT           gx_multi_line_text_view_text_total_rows;      \
    UINT           gx_multi_line_text_view_text_visible_rows;    \
    INT            gx_multi_line_text_view_text_scroll_shift;    \
    GX_STRING      gx_multi_line_text_view_text;                 \
    GX_UBYTE       gx_multi_line_text_view_whitespace;           \
    GX_BYTE        gx_multi_line_text_view_line_space;           \
    GX_BOOL        gx_multi_line_text_view_line_index_old;       \
    GX_UBYTE       gx_multi_line_text_view_cache_size;           \
    UINT           gx_multi_line_text_view_first_cache_line;     \
    UINT           gx_multi_line_text_view_line_index[GX_MULTI_LINE_INDEX_CACHE_SIZE]; \
    GX_MULTI_LINE_TEXT_VIEW_BIDI_RESOLVED_TEXT_INFO

/* Define macro for GX_RICH_TEXT_VIEW, based on GX_MULTI_LINE_TEXT_VIEW.  */

#define GX_RICH_TEXT_VIEW_MEMBERS_DECLARE                        \
    GX_MULTI_LINE_TEXT_VIEW_MEMBERS_DECLARE                      \
    GX_RICH_TEXT_FONTS      gx_rich_text_view_fonts;             \
    ULONG                   gx_rich_text_view_text_total_height;

/* GX_MULTI_LINE_TEXT_INPUT, based on GX_MULTI_LINE_TEXT_VIEW */
#define GX_MULTI_LINE_TEXT_INPUT_MEMBERS_DECLARE                          \
    GX_MULTI_LINE_TEXT_VIEW_MEMBERS_DECLARE                               \
    UINT                 gx_multi_line_text_input_buffer_size;            \
    UINT                 gx_multi_line_text_input_text_insert_position;   \
    UINT                 gx_multi_line_text_input_text_was_modified;      \
    UINT                 gx_multi_line_text_input_text_cursor_line;       \
    GX_TEXT_INPUT_CURSOR gx_multi_line_text_input_cursor_instance;        \
    GX_RESOURCE_ID       gx_multi_line_text_input_readonly_text_color;    \
    GX_RESOURCE_ID       gx_multi_line_text_input_readonly_fill_color;    \
    UINT                 gx_multi_line_text_input_start_mark;             \
    UINT                 gx_multi_line_text_input_end_mark;               \
    GX_UBYTE             gx_multi_line_text_input_new_line_character[3];  \
    GX_UBYTE             gx_multi_line_text_input_new_line_character_size;

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
    GX_RADIAL_PROGRESS_BAR_INFO gx_radial_progress_bar_info;              \
    GX_RADIAL_PROGRESS_BAR_CANVAS_FIELD

/* Define macro of GX_RADIAL_SLIDER members. */
#define GX_RADIAL_SLIDER_MEMBERS_DECLARE                                \
    GX_WIDGET_MEMBERS_DECLARE                                           \
    GX_RADIAL_SLIDER_INFO       gx_radial_slider_info;                  \
    GX_VALUE                    gx_radial_slider_start_angle;           \
    GX_VALUE                    gx_radial_slider_target_angle;          \
    GX_POINT                    gx_radial_slider_pendown_point;         \
    USHORT                      gx_radial_slider_animation_total_steps; \
    USHORT                      gx_radial_slider_animation_step;        \
    USHORT                      gx_radial_slider_animation_delay;       \
    USHORT                      gx_radial_slider_animation_style;       \
    VOID                      (*gx_radial_slider_animation_update_callback)(struct GX_RADIAL_SLIDER_STRUCT *slider);

/* Define macro for GX_LINE_CHART members. */
#define GX_LINE_CHART_MEMBERS_DECLARE      \
    GX_WINDOW_MEMBERS_DECLARE              \
    GX_LINE_CHART_INFO gx_line_chart_info;

/* Define macro for GX_SCROLL_WHEEL members */
#define GX_SCROLL_WHEEL_MEMBERS_DECLARE                            \
    GX_WINDOW_MEMBERS_DECLARE                                      \
    UINT            (*gx_scroll_wheel_scroll)(struct GX_SCROLL_WHEEL_STRUCT *wheel, GX_VALUE amount); \
    GX_BOOL         (*gx_scroll_wheel_wrap_style_check)(struct GX_SCROLL_WHEEL_STRUCT *wheel); \
    INT               gx_scroll_wheel_selected_row;                \
    INT               gx_scroll_wheel_total_rows;                  \
    GX_RESOURCE_ID    gx_scroll_wheel_selected_background;         \
    GX_GRADIENT       gx_scroll_wheel_gradient;                    \
    GX_VALUE          gx_scroll_wheel_selected_yshift;             \
    GX_VALUE          gx_scroll_wheel_animation_speed;             \
    GX_VALUE          gx_scroll_wheel_animation_end_speed;         \
    GX_VALUE          gx_scroll_wheel_animation_steps;             \
    GX_VALUE          gx_scroll_wheel_animation_delay;             \
    GX_VALUE          gx_scroll_wheel_animation_max_steps;         \
    GX_FIXED_VAL      gx_scroll_wheel_animation_start_speed_rate;  \
    GX_FIXED_VAL      gx_scroll_wheel_animation_end_speed_rate;    \
    GX_VALUE          gx_scroll_wheel_row_height;                  \
    GX_VALUE          gx_scroll_wheel_shift_error;

/* Define macro for GX_GENERIC_SCROLL_WHEEL members. */
#define GX_GENERIC_SCROLL_WHEEL_MEMBERS_DECLARE                                                                      \
    GX_SCROLL_WHEEL_MEMBERS_DECLARE                                                                                  \
    VOID (*gx_generic_scroll_wheel_callback)(struct GX_GENERIC_SCROLL_WHEEL_STRUCT *, GX_WIDGET *widget, INT index); \
    INT    gx_generic_scroll_wheel_top_index;                                                                        \
    INT    gx_generic_scroll_wheel_child_count;                                                                      \
    INT    gx_generic_scroll_wheel_visible_rows;

/* Define macro for GX_TEXT_SCROLL_WHEEL members. */
#define GX_TEXT_SCROLL_WHEEL_MEMBERS_DECLARE                                                                   \
    GX_SCROLL_WHEEL_MEMBERS_DECLARE                                                                            \
    GX_RESOURCE_ID      gx_text_scroll_wheel_normal_font;                                                      \
    GX_RESOURCE_ID      gx_text_scroll_wheel_selected_font;                                                    \
    GX_RESOURCE_ID      gx_text_scroll_wheel_normal_text_color;                                                \
    GX_RESOURCE_ID      gx_text_scroll_wheel_selected_text_color;                                              \
    GX_RESOURCE_ID      gx_text_scroll_wheel_disabled_text_color;                                              \
    UINT              (*gx_text_scroll_wheel_text_get)(struct GX_TEXT_SCROLL_WHEEL_STRUCT *, INT, GX_STRING *);\
    GX_TEXT_SCROLL_WHEEL_TEXT_GET_DEPRECATED                                                                   \
    GX_TEXT_SCROLL_WHEEL_BIDI_RESOLVED_TEXT_INFO

/* Define macro for GX_STRING_SCROLL_WHEEL members. */
#define GX_STRING_SCROLL_WHEEL_MEMBERS_DECLARE                               \
    GX_TEXT_SCROLL_WHEEL_MEMBERS_DECLARE                                     \
    GX_CONST GX_STRING       *gx_string_scroll_wheel_string_list;            \
    USHORT                    gx_string_scroll_wheel_string_list_buffer_size;\
    GX_CONST GX_RESOURCE_ID  *gx_string_scroll_wheel_string_id_list;         \
    GX_STRING_SCROLL_WHEEL_STRING_LIST_DEPRECATED

/* Define macro for GX_RANGE_SCROLL_WHEEL members. */
#define GX_NUMERIC_SCROLL_WHEEL_MEMBERS_DECLARE               \
    GX_TEXT_SCROLL_WHEEL_MEMBERS_DECLARE                      \
    INT gx_numeric_scroll_wheel_start_val;                    \
    INT gx_numeric_scroll_wheel_end_val;                      \
    GX_CHAR gx_numeric_scroll_wheel_string_buffer[GX_NUMERIC_SCROLL_WHEEL_STRING_BUFFER_SIZE];

#define GX_MENU_LIST_MEMBERS_DECLARE               \
    GX_WIDGET_MEMBERS_DECLARE                      \
    GX_WIDGET                 *gx_menu_list_owner;

#define GX_MENU_MEMBERS_DECLARE                          \
    GX_PIXELMAP_PROMPT_MEMBERS_DECLARE                   \
    GX_MENU_LIST               gx_menu_list;             \
    GX_VALUE                   gx_menu_list_total_count; \
    GX_VALUE                   gx_menu_text_x_offset;    \
    GX_VALUE                   gx_menu_text_y_offset;

#define GX_ACCORDION_MENU_MEMBERS_DECLARE          \
    GX_WIDGET_MEMBERS_DECLARE                      \
    GX_WIDGET *gx_accordion_menu_collapse_item;    \
    GX_WIDGET *gx_accordion_menu_expand_item;      \
    GX_UBYTE   gx_accordion_menu_animation_status;

#define GX_TREE_VIEW_MEMBERS_DECLARE                  \
    GX_WINDOW_MEMBERS_DECLARE                         \
    GX_RESOURCE_ID gx_tree_view_collapse_pixelmap_id; \
    GX_RESOURCE_ID gx_tree_view_expand_pixelmap_id;   \
    GX_RESOURCE_ID gx_tree_view_root_line_color;      \
    GX_VALUE       gx_tree_view_indentation;          \
    GX_VALUE       gx_tree_view_x_shift;              \
    GX_VALUE       gx_tree_view_y_shift;              \
    GX_VALUE       gx_tree_view_tree_width;           \
    GX_VALUE       gx_tree_view_tree_height;          \
    GX_WIDGET     *gx_tree_view_selected;

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

/* Define Numeric Prompt type.  */

typedef struct GX_NUMERIC_PROMPT_STRUCT
{
    GX_NUMERIC_PROMPT_MEMBERS_DECLARE
}GX_NUMERIC_PROMPT;

/* Define Pixelmap type.  */

typedef struct GX_PIXELMAP_PROMPT_STRUCT
{
    GX_PIXELMAP_PROMPT_MEMBERS_DECLARE
} GX_PIXELMAP_PROMPT;

/* Define Numeric Pixelmap Prompt type.  */
typedef struct GX_NUMERIC_PIXELMAP_PROMPT_STRUCT
{
    GX_NUMERIC_PIXELMAP_PROMPT_MEMBERS_DECLARE
}GX_NUMERIC_PIXELMAP_PROMPT;

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

/* Define Rich Text View type.  */

typedef struct GX_RICH_TEXT_VIEW_STRUCT
{
    GX_RICH_TEXT_VIEW_MEMBERS_DECLARE
} GX_RICH_TEXT_VIEW;

/*Define GX_MULTI_LINE_TEXT_INFO type. */
typedef struct GX_MULTI_LINE_TEXT_INFO_STRUCT
{
    USHORT   gx_text_display_width;
    USHORT   gx_text_display_number;
} GX_MULTI_LINE_TEXT_INFO;

typedef struct GX_RICH_TEXT_INOT_STRUCT
{
    GX_COLOR gx_rich_text_info_foreground_color;
    GX_COLOR gx_rich_text_info_background_color;
    USHORT   gx_rich_text_info_style;
    ULONG    gx_rich_text_info_start_index;
    ULONG    gx_rich_text_info_end_index;
}GX_RICH_TEXT_INFO;

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

/* Define Radial Slider type. */
typedef struct GX_RADIAL_SLIDER_STRUCT
{
    GX_RADIAL_SLIDER_MEMBERS_DECLARE
} GX_RADIAL_SLIDER;

/* Define GX_LINE_CHART control block type */
typedef struct GX_LINE_CHART_STRUCT
{
    GX_LINE_CHART_MEMBERS_DECLARE
} GX_LINE_CHART;

/* Define GX_SCROLL_WHEEL control block type */
typedef struct GX_SCROLL_WHEEL_STRUCT
{
    GX_SCROLL_WHEEL_MEMBERS_DECLARE
} GX_SCROLL_WHEEL;

/* Define GX_GENERIC_SCROLL_WHEEL control block type. */
typedef struct GX_GENERIC_SCROLL_WHEEL_STRUCT
{
    GX_GENERIC_SCROLL_WHEEL_MEMBERS_DECLARE
} GX_GENERIC_SCROLL_WHEEL;

/* Define GX_TEXT_SCROLL_WHEEL_BASE control block type */
typedef struct GX_TEXT_SCROLL_WHEEL_STRUCT
{
    GX_TEXT_SCROLL_WHEEL_MEMBERS_DECLARE
} GX_TEXT_SCROLL_WHEEL;

/* Define GX_STRING_SCROLL_WHEEL control block type */
typedef struct GX_STRING_SCROLL_WHEEL_STRUCT
{
    GX_STRING_SCROLL_WHEEL_MEMBERS_DECLARE
} GX_STRING_SCROLL_WHEEL;

/* Define GX_NUMERIC_SCROLL_WHEEL control block type */
typedef struct GX_NUMERIC_SCROLL_WHEEL_STRUCT
{
    GX_NUMERIC_SCROLL_WHEEL_MEMBERS_DECLARE
} GX_NUMERIC_SCROLL_WHEEL;


typedef struct GX_MENU_LIST_STRUCT
{
    GX_MENU_LIST_MEMBERS_DECLARE
}GX_MENU_LIST;

typedef struct GX_MENU_STRUCT
{
    GX_MENU_MEMBERS_DECLARE
}GX_MENU;

typedef struct GX_ACCORDION_MENU_STRUCT
{
    GX_ACCORDION_MENU_MEMBERS_DECLARE
}GX_ACCORDION_MENU;

typedef struct GX_TREE_VIEW_STRUCT
{
    GX_TREE_VIEW_MEMBERS_DECLARE
}GX_TREE_VIEW;

/* Animation information structure. This structure is used to define the
   parameters of a screen transition animation.  */

typedef struct GX_ANIMATION_INFO_STRUCT
{
    GX_WIDGET  *gx_animation_target;
    GX_WIDGET  *gx_animation_parent;
    GX_WIDGET **gx_animation_slide_screen_list;
    USHORT      gx_animation_style;
    USHORT      gx_animation_id;
    USHORT      gx_animation_start_delay;
    USHORT      gx_animation_frame_interval;
    GX_POINT    gx_animation_start_position;
    GX_POINT    gx_animation_end_position;
    GX_UBYTE    gx_animation_start_alpha;
    GX_UBYTE    gx_animation_end_alpha;
    GX_UBYTE    gx_animation_steps;
} GX_ANIMATION_INFO;

/* Animation control block */
typedef struct GX_ANIMATION_STRUCT
{
    GX_ANIMATION_INFO           gx_animation_info;
    GX_CANVAS                  *gx_animation_canvas;
    struct GX_ANIMATION_STRUCT *gx_animation_next;
    UINT                      (*gx_animation_original_event_process_function)(struct GX_WIDGET_STRUCT *, GX_EVENT *);

    GX_VALUE                    gx_animation_slide_target_index_1;
    GX_VALUE                    gx_animation_slide_target_index_2;
    GX_VALUE                    gx_animation_slide_tracking_start_pos;
    GX_VALUE                    gx_animation_slide_tracking_current_pos;
    USHORT                      gx_animation_landing_speed;
    USHORT                      gx_animation_timer;
    USHORT                      gx_animation_slide_screen_list_size;
    GX_UBYTE                    gx_animation_status;
    GX_UBYTE                    gx_animation_slide_direction;
    GX_UBYTE                    gx_animation_system_allocated;
    GX_UBYTE                    gx_animation_total_steps;
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
    UINT              (*gx_image_reader_pixel_write)(struct GX_IMAGE_READER_STRUCT *image_reader, GX_PIXEL *pixel);
    UINT              (*gx_image_reader_pixel_read)(struct GX_IMAGE_READER_STRUCT *image_reader, INT index, GX_PIXEL *pixel);
    GX_CONST GX_UBYTE  *gx_image_reader_source_data;
    UINT                gx_image_reader_source_data_size;
    GX_UBYTE           *gx_image_reader_getdata;
    GX_UBYTE           *gx_image_reader_getauxdata;
    GX_UBYTE           *gx_image_reader_putdata;
    GX_UBYTE           *gx_image_reader_putauxdata;
    GX_UBYTE           *gx_image_reader_putdatarow;
    GX_UBYTE           *gx_image_reader_putauxdatarow;
    GX_UBYTE            gx_image_reader_color_format;
    GX_UBYTE            gx_image_reader_mode;
    GX_UBYTE            gx_image_reader_image_type;
    GX_UBYTE            gx_image_reader_putdata_mask;
    GX_UBYTE            gx_image_reader_putauxdata_mask;
    GX_UBYTE            gx_image_reader_mono_shreshold;
    GX_COLOR           *gx_image_reader_palette;
    UINT                gx_image_reader_palette_size;
    UINT                gx_image_reader_input_stride;
    GX_BYTE             gx_image_reader_putdatarow_stride;
    GX_BYTE             gx_image_reader_putauxdatarow_stride;
    GX_BOOL             gx_image_reader_size_testing;
    GX_COLOR           *gx_image_reader_png_trans;
    GX_COLOR           *gx_image_reader_png_palette;
    UINT                gx_image_reader_png_palette_size;
    UINT                gx_image_reader_image_width;
    UINT                gx_image_reader_image_height;
} GX_IMAGE_READER;

#define GX_IMAGE_READER_MODE_NONE       0x00
#define GX_IMAGE_READER_MODE_COMPRESS   0x01
#define GX_IMAGE_READER_MODE_ALPHA      0x02
#define GX_IMAGE_READER_MODE_DITHER     0x04
#define GX_IMAGE_READER_MODE_ROTATE_CW  0x10
#define GX_IMAGE_READER_MODE_ROTATE_CCW 0x20

/* Define Screen stack control block */
typedef struct GX_SCREEN_STACK_CONTROL_STRUCT
{
    GX_WIDGET  **gx_screen_stack_control_memory;
    INT          gx_screen_stack_control_top;
    INT          gx_screen_stack_control_max;
} GX_SCREEN_STACK_CONTROL;

/* define resistive touch screen calibration params */
typedef struct TOUCH_CAL_MATRIX_STRUCT {
    INT  An;     /* A = An/Divider */
    INT  Bn;     /* B = Bn/Divider */
    INT  Cn;     /* C = Cn/Divider */
    INT  Dn;     /* D = Dn/Divider */
    INT  En;     /* E = En/Divider */
    INT  Fn;     /* F = Fn/Divider */
    INT  Divider ;
} TOUCH_CALIBRATION_MATRIX;

/* touch driver info specified by application. This information is required
   to run the touch screen calibration */
typedef struct GX_RESISTIVE_TOUCH_INFO_STRUCT {
    GX_RESOURCE_ID  gx_touch_info_string_id_no_touch;
    GX_RESOURCE_ID  gx_touch_info_string_id_touch_1;
    GX_RESOURCE_ID  gx_touch_info_string_id_touch_2;
    GX_RESOURCE_ID  gx_touch_info_string_id_touch_3;
    GX_MULTI_LINE_TEXT_VIEW *gx_touch_info_text;
    GX_ICON         *gx_touch_info_target;
} GX_RESISTIVE_TOUCH_INFO;

/* touch driver control block */
typedef struct GX_RESISTIVE_TOUCH_STRUCT {
    GX_RESISTIVE_TOUCH_INFO   gx_resistive_touch_info;
    GX_BOOL                   gx_resistive_touch_calibrated;
    USHORT                    gx_resistive_touch_min_drag_delta;
    USHORT                    gx_resistive_touch_sample_size;
    USHORT                    gx_resistive_touch_max_retries;
    USHORT                    gx_resistive_touch_stability_limit;
    USHORT                    gx_resistive_touch_x_range;
    USHORT                    gx_resistive_touch_y_range;
    USHORT                    gx_resistive_touch_current_touch_state;
    USHORT                    gx_resistive_touch_last_touch_state;
    GX_POINT                 *gx_resistive_touch_sample_ram;
    GX_POINT                  gx_resistive_touch_current_touch_coord;
    GX_POINT                  gx_resistive_touch_last_touch_coord;
    GX_BOOL                 (*gx_resistive_touch_pen_down_detect)(VOID);
    USHORT                  (*gx_resistive_touch_sample_read)(GX_VALUE axis);
    TOUCH_CALIBRATION_MATRIX  gx_resistive_touch_calibration_matrix;
} GX_RESISTIVE_TOUCH;

/* pen down or up status, returned to generic driver */
#define GX_TOUCH_STATE_TOUCHED  1
#define GX_TOUCH_STATE_RELEASED 2
#define GX_TOUCH_X_AXIS         1
#define GX_TOUCH_Y_AXIS         2

/* GUIX fixed point math helpers */

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

#define gx_accordion_menu_create(a, b, c, d, e, f)               _gx_accordion_menu_create(a, b, c, d, e, f)
#define gx_accordion_menu_draw                                   _gx_accordion_menu_draw
#define gx_accordion_menu_event_process                          _gx_accordion_menu_event_process
#define gx_accordion_menu_position(a)                            _gx_accordion_menu_position(a)

#define gx_animation_canvas_define                               _gx_animation_canvas_define
#define gx_animation_create                                      _gx_animation_create
#define gx_animation_delete                                      _gx_animation_delete
#define gx_animation_drag_disable                                _gx_animation_drag_disable
#define gx_animation_drag_enable                                 _gx_animation_drag_enable
#define gx_animation_landing_speed_set                           _gx_animation_landing_speed_set
#define gx_animation_start                                       _gx_animation_start
#define gx_animation_stop                                        _gx_animation_stop

#define gx_binres_language_count_get                             _gx_binres_language_count_get
#define gx_binres_language_info_load                             _gx_binres_language_info_load
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define gx_binres_language_table_load                            _gx_binres_language_table_load
#endif
#define gx_binres_language_table_load_ext                        _gx_binres_language_table_load_ext
#define gx_binres_theme_load                                     _gx_binres_theme_load
#define gx_binres_pixelmap_load                                  _gx_binres_pixelmap_load
#define gx_binres_font_load                                      _gx_binres_font_load

#define gx_brush_default                                         _gx_brush_default
#define gx_brush_define                                          _gx_brush_define

#define gx_button_background_draw                                _gx_button_background_draw
#define gx_button_create(a, b, c, d, e, f)                       _gx_button_create(a, b, (GX_WIDGET *)c, d, e, f)
#define gx_button_deselect                                       _gx_button_deselect
#define gx_button_draw                                           _gx_button_draw
#define gx_button_event_process                                  _gx_button_event_process
#define gx_button_select                                         _gx_button_select

#define gx_canvas_alpha_set                                      _gx_canvas_alpha_set
#define gx_canvas_arc_draw                                       _gx_canvas_arc_draw
#define gx_canvas_block_move(a, b, c, d)                         _gx_canvas_block_move(a, b, c, d)
#define gx_canvas_circle_draw                                    _gx_canvas_circle_draw
#define gx_canvas_create                                         _gx_canvas_create
#define gx_canvas_delete                                         _gx_canvas_delete
#define gx_canvas_drawing_complete                               _gx_canvas_drawing_complete
#define gx_canvas_drawing_initiate(a, b, c)                      _gx_canvas_drawing_initiate(a, (GX_WIDGET *)b, c)
#define gx_canvas_ellipse_draw                                   _gx_canvas_ellipse_draw
#define gx_canvas_hardware_layer_bind                            _gx_canvas_hardware_layer_bind
#define gx_canvas_hide                                           _gx_canvas_hide
#define gx_canvas_line_draw                                      _gx_canvas_line_draw
#define gx_canvas_memory_define                                  _gx_canvas_memory_define

#if defined (GX_MOUSE_SUPPORT)
#define gx_canvas_mouse_define                                   _gx_canvas_mouse_define
#define gx_canvas_mouse_hide                                     _gx_canvas_mouse_hide
#define gx_canvas_mouse_show                                     _gx_canvas_mouse_show
#endif
#define gx_canvas_offset_set                                     _gx_canvas_offset_set
#define gx_canvas_pixel_draw                                     _gx_canvas_pixel_draw
#define gx_canvas_pixelmap_blend                                 _gx_canvas_pixelmap_blend
#define gx_canvas_pixelmap_draw                                  _gx_canvas_pixelmap_draw
#define gx_canvas_pixelmap_get                                   _gx_canvas_pixelmap_get
#define gx_canvas_pixelmap_rotate                                _gx_canvas_pixelmap_rotate
#define gx_canvas_pixelmap_tile                                  _gx_canvas_pixelmap_tile
#define gx_canvas_polygon_draw                                   _gx_canvas_polygon_draw
#define gx_canvas_pie_draw                                       _gx_canvas_pie_draw
#define gx_canvas_rectangle_draw                                 _gx_canvas_rectangle_draw
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define gx_canvas_rotated_text_draw                              _gx_canvas_rotated_text_draw
#endif
#define gx_canvas_rotated_text_draw_ext                          _gx_canvas_rotated_text_draw_ext
#define gx_canvas_shift                                          _gx_canvas_shift
#define gx_canvas_show                                           _gx_canvas_show
#define gx_canvas_aligned_text_draw                              _gx_canvas_aligned_text_draw
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define gx_canvas_text_draw                                      _gx_canvas_text_draw
#endif
#define gx_canvas_text_draw_ext                                  _gx_canvas_text_draw_ext

#define gx_checkbox_create(a, b, c, d, e, f, g)                  _gx_checkbox_create(a, b, (GX_WIDGET *)c, d, e, f, g)
#define gx_checkbox_draw                                         _gx_checkbox_draw
#define gx_checkbox_event_process                                _gx_checkbox_event_process
#define gx_checkbox_pixelmap_set                                 _gx_checkbox_pixelmap_set
#define gx_checkbox_select                                       _gx_checkbox_select

#define gx_circular_gauge_angle_get                              _gx_circular_gauge_angle_get
#define gx_circular_gauge_angle_set                              _gx_circular_gauge_angle_set
#define gx_circular_gauge_animation_set                          _gx_circular_gauge_animation_set
#define gx_circular_gauge_background_draw                        _gx_circular_gauge_background_draw
#define gx_circular_gauge_create(a, b, c, d, e, f, g, h, i)      _gx_circular_gauge_create((GX_CIRCULAR_GAUGE *) a, b, c, d, e, f, g, h, i)
#define gx_circular_gauge_draw                                   _gx_circular_gauge_draw
#define gx_circular_gauge_event_process                          _gx_circular_gauge_event_process

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
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define gx_context_string_get                                    _gx_context_string_get
#endif
#define gx_context_string_get_ext                                _gx_context_string_get_ext

#define gx_display_active_language_set                           _gx_display_active_language_set
#define gx_display_color_set                                     _gx_display_color_set
#define gx_display_color_table_set                               _gx_display_color_table_set
#define gx_display_create                                        _gx_display_create
#define gx_display_delete                                        _gx_display_delete
#define gx_display_font_table_set                                _gx_display_font_table_set
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define gx_display_language_table_get                            _gx_display_language_table_get
#define gx_display_language_table_set                            _gx_display_language_table_set
#endif
#define gx_display_language_table_get_ext                        _gx_display_language_table_get_ext
#define gx_display_language_table_set_ext                        _gx_display_language_table_set_ext
#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
#define gx_display_language_direction_table_set                  _gx_display_language_direction_table_set
#endif
#define gx_display_pixelmap_table_set                            _gx_display_pixelmap_table_set
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define gx_display_string_get                                    _gx_display_string_get
#define gx_display_string_table_get                              _gx_display_string_table_get
#endif
#define gx_display_string_get_ext                                _gx_display_string_get_ext
#define gx_display_string_table_get_ext                          _gx_display_string_table_get_ext
#define gx_display_theme_install                                 _gx_display_theme_install

#define gx_drop_list_close                                       _gx_drop_list_close
#define gx_drop_list_create(a, b, c, d, e, f, g, h, i)           _gx_drop_list_create(a, b, (GX_WIDGET *)c, d, e, f, g, h, i)
#define gx_drop_list_event_process                               _gx_drop_list_event_process
#define gx_drop_list_open                                        _gx_drop_list_open
#define gx_drop_list_pixelmap_set                                _gx_drop_list_pixelmap_set
#define gx_drop_list_popup_get                                   _gx_drop_list_popup_get

#define gx_generic_scroll_wheel_children_position                _gx_generic_scroll_wheel_children_position
#define gx_generic_scroll_wheel_create                           _gx_generic_scroll_wheel_create
#define gx_generic_scroll_wheel_draw                             _gx_generic_scroll_wheel_draw
#define gx_generic_scroll_wheel_event_process                    _gx_generic_scroll_wheel_event_process
#define gx_generic_scroll_wheel_row_height_set                   _gx_generic_scroll_wheel_row_height_set
#define gx_generic_scroll_wheel_total_rows_set                   _gx_generic_scroll_wheel_total_rows_set

#define gx_horizontal_list_children_position                     _gx_horizontal_list_children_position
#define gx_horizontal_list_create(a, b, c, d, e, f, g, h)        _gx_horizontal_list_create(a, b, (GX_WIDGET *)c, d, e, f, g, h)
#define gx_horizontal_list_event_process                         _gx_horizontal_list_event_process
#define gx_horizontal_list_page_index_set(a, b)                  _gx_horizontal_list_page_index_set((GX_HORIZONTAL_LIST *)a, b)
#define gx_horizontal_list_selected_index_get(a, b)              _gx_horizontal_list_selected_index_get(a, b)
#define gx_horizontal_list_selected_set(a, b)                    _gx_horizontal_list_selected_set(a, b)
#define gx_horizontal_list_selected_widget_get(a, b)             _gx_horizontal_list_selected_widget_get(a, (GX_WIDGET **)b)
#define gx_horizontal_list_total_columns_set(a, b)               _gx_horizontal_list_total_columns_set((GX_HORIZONTAL_LIST *)a, b)

#define gx_horizontal_scrollbar_create(a, b, c, d, e)            _gx_horizontal_scrollbar_create(a, b, (GX_WINDOW *)c, d, e)

#define gx_icon_background_draw                                  _gx_icon_background_draw
#define gx_icon_button_create(a, b, c, d, e, f, g)               _gx_icon_button_create(a, b, (GX_WIDGET *)c, d, e, f, g)
#define gx_icon_button_draw                                      _gx_icon_button_draw
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

#define gx_menu_create(a, b, c, d, e, f, g, h)                   _gx_menu_create((GX_MENU *)a, b, (GX_WIDGET *)c, d, e, f, g, h)
#define gx_menu_draw                                             _gx_menu_draw
#define gx_menu_event_process                                    _gx_menu_event_process
#define gx_menu_insert                                           _gx_menu_insert
#define gx_menu_remove                                           _gx_menu_remove
#define gx_menu_remove                                           _gx_menu_remove
#define gx_menu_text_draw                                        _gx_menu_text_draw
#define gx_menu_text_offset_set                                  _gx_menu_text_offset_set

#define gx_multi_line_text_button_create(a, b, c, d, e, f, g)    _gx_multi_line_text_button_create(a, b, (GX_WIDGET *)c, d, e, f, g)
#define gx_multi_line_text_button_draw                           _gx_multi_line_text_button_draw
#define gx_multi_line_text_button_event_process                  _gx_multi_line_text_button_event_process
#define gx_multi_line_text_button_text_draw                      _gx_multi_line_text_button_text_draw
#define gx_multi_line_text_button_text_id_set                    _gx_multi_line_text_button_text_id_set
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define gx_multi_line_text_button_text_set                       _gx_multi_line_text_button_text_set
#endif
#define gx_multi_line_text_button_text_set_ext                   _gx_multi_line_text_button_text_set_ext

#define gx_multi_line_text_input_backspace                       _gx_multi_line_text_input_backspace
#define gx_multi_line_text_input_buffer_clear                    _gx_multi_line_text_input_buffer_clear
#define gx_multi_line_text_input_buffer_get                      _gx_multi_line_text_input_buffer_get
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define gx_multi_line_text_input_char_insert                     _gx_multi_line_text_input_char_insert
#endif
#define gx_multi_line_text_input_char_insert_ext                 _gx_multi_line_text_input_char_insert_ext
#define gx_multi_line_text_input_create(a, b, c, d, e, f, g, h)  _gx_multi_line_text_input_create(a, b, (GX_WIDGET *)c, d, e, f, g, h)
#define gx_multi_line_text_input_cursor_pos_get                  _gx_multi_line_text_input_cursor_pos_get
#define gx_multi_line_text_input_delete                          _gx_multi_line_text_input_delete
#define gx_multi_line_text_input_down_arrow                      _gx_multi_line_text_input_down_arrow
#define gx_multi_line_text_input_end                             _gx_multi_line_text_input_end
#define gx_multi_line_text_input_event_process                   _gx_multi_line_text_input_event_process
#define gx_multi_line_text_input_fill_color_set                  _gx_multi_line_text_input_fill_color_set
#define gx_multi_line_text_input_home                            _gx_multi_line_text_input_home
#define gx_multi_line_text_input_left_arrow                      _gx_multi_line_text_input_left_arrow
#define gx_multi_line_text_input_right_arrow                     _gx_multi_line_text_input_right_arrow
#define gx_multi_line_text_input_style_add                       _gx_multi_line_text_input_style_add
#define gx_multi_line_text_input_style_remove                    _gx_multi_line_text_input_style_remove
#define gx_multi_line_text_input_style_set                       _gx_multi_line_text_input_style_set
#define gx_multi_line_text_input_text_color_set                  _gx_multi_line_text_input_text_color_set
#define gx_multi_line_text_input_text_select                     _gx_multi_line_text_input_text_select
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define gx_multi_line_text_input_text_set                        _gx_multi_line_text_input_text_set
#endif
#define gx_multi_line_text_input_text_set_ext                    _gx_multi_line_text_input_text_set_ext
#define gx_multi_line_text_input_up_arrow                        _gx_multi_line_text_input_up_arrow

#define gx_multi_line_text_view_create(a, b, c, d, e, f, g)      _gx_multi_line_text_view_create(a, b, (GX_WIDGET *)c, d, e, f, g)
#define gx_multi_line_text_view_draw                             _gx_multi_line_text_view_draw
#define gx_multi_line_text_view_event_process                    _gx_multi_line_text_view_event_process
#define gx_multi_line_text_view_font_set                         _gx_multi_line_text_view_font_set
#define gx_multi_line_text_view_line_space_set                   _gx_multi_line_text_view_line_space_set
#define gx_multi_line_text_view_scroll_info_get                  _gx_multi_line_text_view_scroll_info_get
#if defined(GUIX_5_4_0_COMPATIBILITY)
#define gx_multi_line_text_view_text_color_set(a, b, c)          _gx_multi_line_text_view_text_color_set((GX_MUILTI_LINE_TEXT_VIEW *)a, b, c, b)
#else
#define gx_multi_line_text_view_text_color_set                   _gx_multi_line_text_view_text_color_set
#endif
#define gx_multi_line_text_view_text_id_set                      _gx_multi_line_text_view_text_id_set
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define gx_multi_line_text_view_text_set                         _gx_multi_line_text_view_text_set
#endif
#define gx_multi_line_text_view_text_set_ext                     _gx_multi_line_text_view_text_set_ext
#define gx_multi_line_text_view_whitespace_set                   _gx_multi_line_text_view_whitespace_set

#define gx_rich_text_view_create(a, b, c, d, e, f, g, h)         _gx_rich_text_view_create(a, b, (GX_WIDGET *)c, d, e, f, g, h)
#define gx_rich_text_view_draw                                   _gx_rich_text_view_draw
#define gx_rich_text_view_text_draw                              _gx_rich_text_view_text_draw
#define gx_rich_text_view_fonts_set                              _gx_rich_text_view_fonts_set

#define gx_numeric_pixelmap_prompt_create                        _gx_numeric_pixelmap_prompt_create
#define gx_numeric_pixelmap_prompt_format_function_set           _gx_numeric_pixelmap_prompt_format_function_set
#define gx_numeric_pixelmap_prompt_value_set                     _gx_numeric_pixelmap_prompt_value_set

#define gx_numeric_prompt_create                                 _gx_numeric_prompt_create
#define gx_numeric_prompt_format_function_set                    _gx_numeric_prompt_format_function_set
#define gx_numeric_prompt_value_set                              _gx_numeric_prompt_value_set

#define gx_numeric_scroll_wheel_create(a, b, c, d, e, f, g, h)   _gx_numeric_scroll_wheel_create(a, b, (GX_WIDGET *) c, d, e, f, g, h)
#define gx_numeric_scroll_wheel_range_set                        _gx_numeric_scroll_wheel_range_set

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

#define gx_progress_bar_background_draw                          _gx_progress_bar_background_draw
#define gx_progress_bar_create(a, b, c, d, e, f, g)              _gx_progress_bar_create(a, b, (GX_WIDGET *)c, d, e, f, g)
#define gx_progress_bar_draw                                     _gx_progress_bar_draw
#define gx_progress_bar_event_process                            _gx_progress_bar_event_process
#define gx_progress_bar_font_set                                 _gx_progress_bar_font_set
#define gx_progress_bar_info_set(a, b)                           _gx_progress_bar_info_set((GX_PROGRESS_BAR *)a, b)
#define gx_progress_bar_pixelmap_set                             _gx_progress_bar_pixelmap_set
#define gx_progress_bar_range_set                                _gx_progress_bar_range_set
#if defined(GUIX_5_4_0_COMPATIBILITY)
#define gx_progress_bar_text_color_set(a, b, c)                  _gx_progress_bar_text_color_set((GX_PROGRESS_BAR *)a, b, c, b)
#else
#define gx_progress_bar_text_color_set                           _gx_progress_bar_text_color_set
#endif
#define gx_progress_bar_text_draw                                _gx_progress_bar_text_draw
#define gx_progress_bar_value_set                                _gx_progress_bar_value_set

#define gx_prompt_create(a, b, c, d, e, f, g)                    _gx_prompt_create(a, b, (GX_WIDGET *)c, d, e, f, g)
#define gx_prompt_event_process                                  _gx_prompt_event_process
#define gx_prompt_draw                                           _gx_prompt_draw
#define gx_prompt_font_set                                       _gx_prompt_font_set

#if defined(GUIX_5_4_0_COMPATIBILITY)
#define gx_prompt_text_color_set(a, b, c)                        _gx_prompt_text_color_set((GX_PROGRESS_BAR *)a, b, c, b)
#else
#define gx_prompt_text_color_set                                 _gx_prompt_text_color_set
#endif
#define gx_prompt_text_draw                                      _gx_prompt_text_draw
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define gx_prompt_text_get                                       _gx_prompt_text_get
#endif
#define gx_prompt_text_get_ext                                   _gx_prompt_text_get_ext
#define gx_prompt_text_id_set                                    _gx_prompt_text_id_set
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define gx_prompt_text_set                                       _gx_prompt_text_set
#endif
#define gx_prompt_text_set_ext                                   _gx_prompt_text_set_ext

#define gx_radial_progress_bar_anchor_set                        _gx_radial_progress_bar_anchor_set
#define gx_radial_progress_bar_background_draw                   _gx_radial_progress_bar_background_draw
#define gx_radial_progress_bar_create                            _gx_radial_progress_bar_create
#define gx_radial_progress_bar_draw                              _gx_radial_progress_bar_draw
#define gx_radial_progress_bar_event_process                     _gx_radial_progress_bar_event_process
#define gx_radial_progress_bar_font_set                          _gx_radial_progress_bar_font_set
#define gx_radial_progress_bar_info_set                          _gx_radial_progress_bar_info_set
#if defined(GUIX_5_4_0_COMPATIBILITY)
#define gx_radial_progress_bar_text_color_set(a, b, c)           _gx_radial_progress_bar_text_color_set((GX_RADIAL_PROGRESS_BAR *)a, b, c, b)
#else
#define gx_radial_progress_bar_text_color_set                    _gx_radial_progress_bar_text_color_set
#endif
#define gx_radial_progress_bar_text_draw                         _gx_radial_progress_bar_text_draw
#define gx_radial_progress_bar_value_set                         _gx_radial_progress_bar_value_set

#define gx_radial_slider_anchor_angles_set                       _gx_radial_slider_anchor_anglees_set
#define gx_radial_slider_animation_set                           _gx_radial_slider_animation_set
#define gx_radial_slider_animation_start                         _gx_radial_slider_animation_start
#define gx_radial_slider_create                                  _gx_radial_slider_create
#define gx_radial_slider_draw                                    _gx_radial_slider_draw
#define gx_radial_slider_event_process                           _gx_radial_slider_event_process
#define gx_radial_slider_info_get                                _gx_radial_slider_info_get
#define gx_radial_slider_info_set                                _gx_radial_slider_info_set
#define gx_radial_slider_pixelmap_set                            _gx_radial_slider_pixelmap_set
#define gx_radial_slider_angle_set                               _gx_radial_slider_angle_set

#define gx_radio_button_create(a, b, c, d, e, f, g)              _gx_radio_button_create(a, b, (GX_WIDGET *)c, d, e, f, g)
#define gx_radio_button_draw                                     _gx_radio_button_draw
#define gx_radio_button_pixelmap_set                             _gx_radio_button_pixelmap_set

#define gx_screen_stack_create                                   _gx_screen_stack_create
#define gx_screen_stack_pop                                      _gx_screen_stack_pop
#define gx_screen_stack_push                                     _gx_screen_stack_push
#define gx_screen_stack_reset                                    _gx_screen_stack_reset

#define gx_scroll_thumb_create                                   _gx_scroll_thumb_create
#define gx_scroll_thumb_draw                                     _gx_scroll_thumb_draw
#define gx_scroll_thumb_event_process                            _gx_scroll_thumb_event_process

#define gx_scrollbar_draw                                        _gx_scrollbar_draw
#define gx_scrollbar_event_process                               _gx_scrollbar_event_process
#define gx_scrollbar_limit_check                                 _gx_scrollbar_limit_check
#define gx_scrollbar_reset                                       _gx_scrollbar_reset
#define gx_scrollbar_value_set                                   _gx_scrollbar_value_set

#define gx_scroll_wheel_create                                   _gx_scroll_wheel_create
#define gx_scroll_wheel_event_process                            _gx_scroll_wheel_event_process
#define gx_scroll_wheel_gradient_alpha_set(a, b, c)              _gx_scroll_wheel_gradient_alpha_set((GX_SCROLL_WHEEL *) a, b, c)
#define gx_scroll_wheel_row_height_set(a, b)                     _gx_scroll_wheel_row_height_set((GX_SCROLL_WHEEL *) a, b)
#define gx_scroll_wheel_selected_background_set(a, b)            _gx_scroll_wheel_selected_background_set((GX_SCROLL_WHEEL *) a, b)
#define gx_scroll_wheel_selected_get(a, b)                       _gx_scroll_wheel_selected_get((GX_SCROLL_WHEEL *) a, b)
#define gx_scroll_wheel_selected_set(a, b)                       _gx_scroll_wheel_selected_set((GX_SCROLL_WHEEL *) a, b)
#define gx_scroll_wheel_speed_set(a, b, c, d, e)                 _gx_scroll_wheel_speed_set((GX_SCROLL_WHEEL *) a, b, c, d, e)
#define gx_scroll_wheel_total_rows_set(a, b)                     _gx_scroll_wheel_total_rows_set((GX_SCROLL_WHEEL *) a, b)

#define gx_single_line_text_input_backspace(a)                   _gx_single_line_text_input_backspace((GX_SINGLE_LINE_TEXT_INPUT *)a)
#define gx_single_line_text_input_buffer_clear(a)                _gx_single_line_text_input_buffer_clear((GX_SINGLE_LINE_TEXT_INPUT *)a)
#define gx_single_line_text_input_buffer_get(a, b, c, d)         _gx_single_line_text_input_buffer_get((GX_SINGLE_LINE_TEXT_INPUT *)a, b, c, d)
#define gx_single_line_text_input_character_delete(a)            _gx_single_line_text_input_character_delete(a)
#define gx_single_line_text_input_character_insert(a, b, c)      _gx_single_line_text_input_character_insert(a, b, c)
#define gx_single_line_text_input_create(a, b, c, d, e, f, g, h) _gx_single_line_text_input_create(a, b, (GX_WIDGET *)c, d, e, f, g, h)
#define gx_single_line_text_input_draw(a)                        _gx_single_line_text_input_draw(a)
#define gx_single_line_text_input_draw_position_get(a, b, c)     _gx_single_line_text_input_draw_position_get(a, b, c)
#define gx_single_line_text_input_end(a)                         _gx_single_line_text_input_end((GX_SINGLE_LINE_TEXT_INPUT *)a)
#define gx_single_line_text_input_event_process(a, b)            _gx_single_line_text_input_event_process((GX_SINGLE_LINE_TEXT_INPUT *)a, b)
#define gx_single_line_text_input_fill_color_set(a, b, c, d, e)  _gx_single_line_text_input_fill_color_set((GX_SINGLE_LINE_TEXT_INPUT *)a, b, c, d, e)
#define gx_single_line_text_input_home(a)                        _gx_single_line_text_input_home((GX_SINGLE_LINE_TEXT_INPUT *)a)
#define gx_single_line_text_input_left_arrow(a)                  _gx_single_line_text_input_left_arrow((GX_SINGLE_LINE_TEXT_INPUT *)a)
#define gx_single_line_text_input_position_get(a, b)             _gx_single_line_text_input_position_get(a, b)
#define gx_single_line_text_input_right_arrow(a)                 _gx_single_line_text_input_right_arrow((GX_SINGLE_LINE_TEXT_INPUT *)a)
#define gx_single_line_text_input_style_add(a, b)                _gx_single_line_text_input_style_add((GX_SINGLE_LINE_TEXT_INPUT *)a, b);
#define gx_single_line_text_input_style_remove(a, b)             _gx_single_line_text_input_style_remove((GX_SINGLE_LINE_TEXT_INPUT *)a, b)
#define gx_single_line_text_input_style_set(a, b)                _gx_single_line_text_input_style_set((GX_SINGLE_LINE_TEXT_INPUT *)a, b)
#define gx_single_line_text_input_text_color_set(a, b, c, d, e)  _gx_single_line_text_input_text_color_set((GX_SINGLE_LINE_TEXT_INPUT *)a, b, c, d, e)
#define gx_single_line_text_input_text_select                    _gx_single_line_text_input_text_select
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define gx_single_line_text_input_text_set                       _gx_single_line_text_input_text_set
#endif
#define gx_single_line_text_input_text_set_ext                   _gx_single_line_text_input_text_set_ext

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

#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define gx_string_scroll_wheel_create(a, b, c, d, e, f, g, h)    _gx_string_scroll_wheel_create(a, b, c, d, e, f, g, h)
#define gx_string_scroll_wheel_string_list_set                   _gx_string_scroll_wheel_string_list_set
#endif
#define gx_string_scroll_wheel_create_ext(a, b, c, d, e, f, g, h) _gx_string_scroll_wheel_create_ext(a, b, c, d, e, f, g, h)
#define gx_string_scroll_wheel_event_process                     _gx_string_scroll_wheel_event_process
#define gx_string_scroll_wheel_string_id_list_set                _gx_string_scroll_wheel_string_id_list_set
#define gx_string_scroll_wheel_string_list_set_ext               _gx_string_scroll_wheel_string_list_set_ext

#define gx_system_active_language_set                            _gx_system_active_language_set

#if (GX_ANIMATION_POOL_SIZE > 0)
#define gx_system_animation_get                                  _gx_system_animation_get
#define gx_system_animation_free                                 _gx_system_animation_free
#endif

#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
#define gx_system_bidi_text_enable                               _gx_system_bidi_text_enable
#define gx_system_bidi_text_disable                              _gx_system_bidi_text_disable
#endif

#if defined(GX_THAI_GLYPH_SHAPING_SUPPORT)
#define gx_system_text_render_style_set                          _gx_system_text_render_style_set
#endif

#define gx_system_canvas_refresh                                 _gx_system_canvas_refresh
#define gx_system_dirty_mark(a)                                  _gx_system_dirty_mark((GX_WIDGET *)a);
#define gx_system_dirty_partial_add(a, b)                        _gx_system_dirty_partial_add((GX_WIDGET *)a, b)
#define gx_system_draw_context_get                               _gx_system_draw_context_get
#define gx_system_event_fold                                     _gx_system_event_fold
#define gx_system_event_send                                     _gx_system_event_send
#define gx_system_focus_claim(a)                                 _gx_system_focus_claim((GX_WIDGET *)a)
#define gx_system_initialize                                     _gx_system_initialize
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define gx_system_language_table_get                             _gx_system_language_table_get
#define gx_system_language_table_set                             _gx_system_language_table_set
#endif
#define gx_system_memory_allocator_set                           _gx_system_memory_allocator_set
#define gx_system_pen_configure                                  _gx_system_pen_configure
#define gx_system_screen_stack_create                            _gx_system_screen_stack_create
#define gx_system_screen_stack_get                               _gx_system_screen_stack_get
#define gx_system_screen_stack_pop                               _gx_system_screen_stack_pop
#define gx_system_screen_stack_push                              _gx_system_screen_stack_push
#define gx_system_screen_stack_reset                             _gx_system_screen_stack_reset
#define gx_system_scroll_appearance_get                          _gx_system_scroll_appearance_get
#define gx_system_scroll_appearance_set                          _gx_system_scroll_appearance_set
#define gx_system_start                                          _gx_system_start
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define gx_system_string_get                                     _gx_system_string_get
#define gx_system_string_table_get                               _gx_system_string_table_get
#define gx_system_string_width_get                               _gx_system_string_width_get
#endif
#define gx_system_string_width_get_ext                           _gx_system_string_width_get_ext

#define gx_system_timer_start(a, b, c, d)                        _gx_system_timer_start((GX_WIDGET *)a, b, c, d)
#define gx_system_timer_stop(a, b)                               _gx_system_timer_stop((GX_WIDGET *)a, b)
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define gx_system_version_string_get                             _gx_system_version_string_get
#endif
#define gx_system_version_string_get_ext                         _gx_system_version_string_get_ext
#define gx_system_widget_find                                    _gx_system_widget_find

#define gx_text_button_create(a, b, c, d, e, f, g)               _gx_text_button_create(a, b, (GX_WIDGET *)c, d, e, f, g)
#define gx_text_button_draw                                      _gx_text_button_draw
#define gx_text_button_event_process                             _gx_text_button_event_process
#define gx_text_button_font_set                                  _gx_text_button_font_set
#if defined(GUIX_5_4_0_COMPATIBILITY)
#define gx_text_button_text_color_set(a, b, c)                   _gx_text_button_text_color_set((GX_TEXT_BUTTON *)a, b, c)
#else
#define gx_text_button_text_color_set                            _gx_text_button_text_color_set
#endif
#define gx_text_button_text_draw                                 _gx_text_button_text_draw
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define gx_text_button_text_get                                  _gx_text_button_text_get
#endif
#define gx_text_button_text_get_ext                              _gx_text_button_text_get_ext
#define gx_text_button_text_id_set                               _gx_text_button_text_id_set
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define gx_text_button_text_set                                  _gx_text_button_text_set
#endif
#define gx_text_button_text_set_ext                              _gx_text_button_text_set_ext

#define gx_text_input_cursor_blink_interval_set                  _gx_text_input_cursor_blink_interval_set
#define gx_text_input_cursor_height_set                          _gx_text_input_cursor_height_set
#define gx_text_input_cursor_width_set                           _gx_text_input_cursor_width_set

#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define gx_text_scroll_wheel_callback_set(a, b)                  _gx_text_scroll_wheel_callback_set((GX_TEXT_SCROLL_WHEEL *)a, (GX_CONST GX_CHAR *(*)(GX_TEXT_SCROLL_WHEEL *, INT)) b)
#endif
#define gx_text_scroll_wheel_callback_set_ext(a, b)              _gx_text_scroll_wheel_callback_set_ext((GX_TEXT_SCROLL_WHEEL *)a, (UINT(*)(GX_TEXT_SCROLL_WHEEL *, INT, GX_STRING *)) b)
#define gx_text_scroll_wheel_font_set(a, b, c)                   _gx_text_scroll_wheel_font_set((GX_TEXT_SCROLL_WHEEL *) a, b, c)
#if defined(GUIX_5_4_0_COMPATIBILITY)
#define gx_text_scroll_wheel_text_color_set(a, b, c)             _gx_text_scroll_wheel_text_color_set((GX_TEXT_SCROLL_WHEEL *) a, b, c, b)
#else
#define gx_text_scroll_wheel_text_color_set(a, b, c, d)          _gx_text_scroll_wheel_text_color_set((GX_TEXT_SCROLL_WHEEL *) a, b, c, d)
#endif
#define gx_text_scroll_wheel_create                              _gx_text_scroll_wheel_create
#define gx_text_scroll_wheel_draw                                _gx_text_scroll_wheel_draw
#define gx_text_scroll_wheel_event_process                       _gx_text_scroll_wheel_event_process

#define gx_tree_view_create(a, b, c, d, e, f)                    _gx_tree_view_create((GX_TREE_VIEW *)a, b, (GX_WIDGET *)c, d, e, f)
#define gx_tree_view_draw                                        _gx_tree_view_draw
#define gx_tree_view_event_process                               _gx_tree_view_event_process
#define gx_tree_view_indentation_set                             _gx_tree_view_indentation_set
#define gx_tree_view_position                                    _gx_tree_view_position
#define gx_tree_view_root_line_color_set                         _gx_tree_view_root_line_color_set
#define gx_tree_view_root_pixelmap_set                           _gx_tree_view_root_pixelmap_set
#define gx_tree_view_selected_get                                _gx_tree_view_selected_get
#define gx_tree_view_selected_set                                _gx_tree_view_selected_set

#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
#define gx_utility_bidi_paragraph_reorder                        _gx_utility_bidi_paragraph_reorder
#define gx_utility_bidi_paragraph_reorder_ext                    _gx_utility_bidi_paragraph_reorder_ext
#define gx_utility_bidi_resolved_text_info_delete                _gx_utility_bidi_resolved_text_info_delete
#endif
#define gx_utility_canvas_to_bmp                                 _gx_utility_canvas_to_bmp
#define gx_utility_gradient_create                               _gx_utility_gradient_create
#define gx_utility_gradient_delete                               _gx_utility_gradient_delete
#define gx_utility_circle_point_get                              _gx_utility_circle_point_get
#define gx_utility_ltoa                                          _gx_utility_ltoa
#if defined(GUIX_5_4_0_COMPATIBILITY)
#define gx_utility_math_acos                                     _gx_utility_math_acos_5_4_0
#define gx_utility_math_asin                                     _gx_utility_math_asin_5_4_0
#define gx_utility_math_cos                                      _gx_utility_math_cos_5_4_0
#define gx_utility_math_sin                                      _gx_utility_math_sin_5_4_0
#else
#define gx_utility_math_acos                                     _gx_utility_math_acos
#define gx_utility_math_asin                                     _gx_utility_math_asin
#define gx_utility_math_cos                                      _gx_utility_math_cos
#define gx_utility_math_sin                                      _gx_utility_math_sin
#endif
#define gx_utility_math_sqrt                                     _gx_utility_math_sqrt
#define gx_utility_pixelmap_resize                               _gx_utility_pixelmap_resize
#define gx_utility_pixelmap_rotate                               _gx_utility_pixelmap_rotate
#define gx_utility_pixelmap_simple_rotate                        _gx_utility_pixelmap_simple_rotate
#define gx_utility_rectangle_center                              _gx_utility_rectangle_center
#define gx_utility_rectangle_center_find                         _gx_utility_rectangle_center_find
#define gx_utility_rectangle_combine                             _gx_utility_rectangle_combine
#define gx_utility_rectangle_compare                             _gx_utility_rectangle_compare
#define gx_utility_rectangle_define                              _gx_utility_rectangle_define
#define gx_utility_rectangle_resize                              _gx_utility_rectangle_resize
#define gx_utility_rectangle_overlap_detect                      _gx_utility_rectangle_overlap_detect
#define gx_utility_rectangle_point_detect                        _gx_utility_rectangle_point_detect
#define gx_utility_rectangle_shift                               _gx_utility_rectangle_shift
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define gx_utility_string_to_alphamap                            _gx_utility_string_to_alphamap
#endif
#define gx_utility_string_to_alphamap_ext                        _gx_utility_string_to_alphamap_ext

#define gx_vertical_list_children_position                       _gx_vertical_list_children_position
#define gx_vertical_list_create(a, b, c, d, e, f, g, h)          _gx_vertical_list_create(a, b, (GX_WIDGET *)c, d, e, f, g, h)
#define gx_vertical_list_event_process                           _gx_vertical_list_event_process
#define gx_vertical_list_page_index_set(a, b)                    _gx_vertical_list_page_index_set((GX_VERTICAL_LIST *)a, b)
#define gx_vertical_list_selected_index_get(a, b)                _gx_vertical_list_selected_index_get(a, b)
#define gx_vertical_list_selected_set(a, b)                      _gx_vertical_list_selected_set(a, b)
#define gx_vertical_list_selected_widget_get(a, b)               _gx_vertical_list_selected_widget_get(a, (GX_WIDGET **)b)
#define gx_vertical_list_total_rows_set(a, b)                    _gx_vertical_list_total_rows_set((GX_VERTICAL_LIST *)a, b)

#define gx_vertical_scrollbar_create(a, b, c, d, e)              _gx_vertical_scrollbar_create(a, b, (GX_WINDOW *)c, d, e)

#define gx_widget_allocate(a, b)                                 _gx_widget_allocate((GX_WIDGET **)a, b)
#define gx_widget_attach(a, b)                                   _gx_widget_attach((GX_WIDGET *)a, (GX_WIDGET *)b)
#define gx_widget_back_attach(a, b)                              _gx_widget_back_attach((GX_WIDGET *)a, (GX_WIDGET *)b)
#define gx_widget_back_move(a, b)                                _gx_widget_back_move((GX_WIDGET *)a, b)
#define gx_widget_background_draw(a)                             _gx_widget_background_draw((GX_WIDGET *)a)
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
#if defined(GUIX_5_4_0_COMPATIBILITY)
#define gx_widget_fill_color_set(a, b, c)                        _gx_widget_fill_color_set((GX_WIDGET *)a, b, c, b)
#else
#define gx_widget_fill_color_set(a, b, c, d)                     _gx_widget_fill_color_set((GX_WIDGET *)a, b, c, d)
#endif
#define gx_widget_find(a, b, c, d)                               _gx_widget_find((GX_WIDGET *)a, b, c, (GX_WIDGET **)d)
#define gx_widget_first_child_get(a, b)                          _gx_widget_first_child_get((GX_WIDGET *) a, (GX_WIDGET **) b)
#define gx_widget_focus_next(a)                                  _gx_widget_focus_next((GX_WIDGET *)a)
#define gx_widget_focus_previous(a)                              _gx_widget_focus_previous((GX_WIDGET *)a)
#define gx_widget_font_get(a, b, c)                              _gx_widget_font_get((GX_WIDGET *)a, b, c)
#define gx_widget_free(a)                                        _gx_widget_free((GX_WIDGET *)a)
#define gx_widget_front_move(a, b)                               _gx_widget_front_move((GX_WIDGET *)a, b)
#define gx_widget_height_get(a, b)                               _gx_widget_height_get((GX_WIDGET *)a, b)
#define gx_widget_hide(a)                                        _gx_widget_hide((GX_WIDGET *)a)
#define gx_widget_last_child_get(a, b)                           _gx_widget_last_child_get((GX_WIDGET *) a, (GX_WIDGET **) b)
#define gx_widget_next_sibling_get(a, b)                         _gx_widget_next_sibling_get((GX_WIDGET *) a, (GX_WIDGET **) b)
#define gx_widget_parent_get(a, b)                               _gx_widget_parent_get((GX_WIDGET *) a, (GX_WIDGET **) b)
#define gx_widget_pixelmap_get(a, b, c)                          _gx_widget_pixelmap_get((GX_WIDGET *)a, b, c)
#define gx_widget_previous_sibling_get(a, b)                     _gx_widget_previous_sibling_get((GX_WIDGET *) a, (GX_WIDGET **) b)
#define gx_widget_resize(a, b)                                   _gx_widget_resize((GX_WIDGET *)a, b)
#define gx_widget_shift(a, b, c, d)                              _gx_widget_shift((GX_WIDGET *)a, b, c, d)
#define gx_widget_show(a)                                        _gx_widget_show((GX_WIDGET *)a)
#define gx_widget_status_add(a, b)                               _gx_widget_status_add((GX_WIDGET *)a, b)
#define gx_widget_status_get(a, b)                               _gx_widget_status_get((GX_WIDGET *)a, b)
#define gx_widget_status_remove(a, b)                            _gx_widget_status_remove((GX_WIDGET *)a, b)
#define gx_widget_status_test(a, b, c)                           _gx_widget_status_test((GX_WIDGET *)a, b, c)
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define gx_widget_string_get(a, b, c)                            _gx_widget_string_get((GX_WIDGET *)a, b, c)
#endif
#define gx_widget_string_get_ext(a, b, c)                        _gx_widget_string_get_ext((GX_WIDGET *)a, b, c)
#define gx_widget_style_add(a, b)                                _gx_widget_style_add((GX_WIDGET *)a, b)
#define gx_widget_style_get(a, b)                                _gx_widget_style_get((GX_WIDGET *)a, b)
#define gx_widget_style_remove(a, b)                             _gx_widget_style_remove((GX_WIDGET *)a, b)
#define gx_widget_style_set(a, b)                                _gx_widget_style_set((GX_WIDGET *)a, b)
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define gx_widget_text_blend(a, b, c, d, e, f, g)                _gx_widget_text_blend((GX_WIDGET *)a, b, c, d, e, f, g)
#define gx_widget_text_draw(a, b, c, d, e, f)                    _gx_widget_text_draw((GX_WIDGET *)a, b, c, d, e, f)
#endif
#define gx_widget_text_blend_ext(a, b, c, d, e, f, g)            _gx_widget_text_blend_ext((GX_WIDGET *)a, b, c, d, e, f, g)
#define gx_widget_text_draw_ext(a, b, c, d, e, f)                _gx_widget_text_draw_ext((GX_WIDGET *)a, b, c, d, e, f)
#define gx_widget_text_id_draw(a, b, c, d, e, f)                 _gx_widget_text_id_draw((GX_WIDGET *)a, b, c, d, e, f)
#define gx_widget_top_visible_child_find(a, b)                   _gx_widget_top_visible_child_find((GX_WIDGET *) a, (GX_WIDGET **) b)
#define gx_widget_type_find(a, b, c)                             _gx_widget_type_find((GX_WIDGET *)a, b, (GX_WIDGET **)c)
#define gx_widget_width_get(a, b)                                _gx_widget_width_get((GX_WIDGET *)a, b)

#define gx_window_background_draw                                _gx_window_background_draw
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

UINT _gx_accordion_menu_create(GX_ACCORDION_MENU *accordion, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                               ULONG style, USHORT accordion_menu_id, GX_CONST GX_RECTANGLE *size);
VOID _gx_accordion_menu_draw(GX_ACCORDION_MENU *menu);
UINT _gx_accordion_menu_event_process(GX_ACCORDION_MENU *accordion, GX_EVENT *event_ptr);
UINT _gx_accordion_menu_position(GX_ACCORDION_MENU *accordion);

UINT _gx_animation_canvas_define(GX_ANIMATION *animation, GX_CANVAS *canvas);
UINT _gx_animation_create(GX_ANIMATION *animation);
UINT _gx_animation_delete(GX_ANIMATION *target, GX_WIDGET *parent);
UINT _gx_animation_drag_disable(GX_ANIMATION *animation, GX_WIDGET *widget);
UINT _gx_animation_drag_enable(GX_ANIMATION *animation, GX_WIDGET *widget, GX_ANIMATION_INFO *info);
UINT _gx_animation_landing_speed_set(GX_ANIMATION *animation, USHORT shift_per_step);
UINT _gx_animation_start(GX_ANIMATION *animation, GX_ANIMATION_INFO *info);
UINT _gx_animation_stop(GX_ANIMATION *animation);

UINT _gx_binres_language_count_get(GX_UBYTE *root_address, GX_VALUE *put_count);
UINT _gx_binres_language_info_load(GX_UBYTE *root_address, GX_LANGUAGE_HEADER *put_info);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gx_binres_language_table_load(GX_UBYTE *root_address, GX_UBYTE ****returned_language_table);
#endif
UINT _gx_binres_language_table_load_ext(GX_UBYTE *root_address, GX_STRING ***returned_language_table);
UINT _gx_binres_theme_load(GX_UBYTE *root_address, INT theme_id, GX_THEME **returned_theme);
UINT _gx_binres_pixelmap_load(GX_UBYTE *root_address, UINT map_index, GX_PIXELMAP *pixelmap);
UINT _gx_binres_font_load(GX_UBYTE *root_address, UINT font_index, GX_UBYTE *buffer, ULONG *buffer_size);

UINT _gx_brush_default(GX_BRUSH *brush);
UINT _gx_brush_define(GX_BRUSH *brush, GX_COLOR line_color, GX_COLOR fill_color, UINT style);

VOID _gx_button_background_draw(GX_BUTTON *button);
UINT _gx_button_create(GX_BUTTON *button,
                       GX_CONST GX_CHAR *name,
                       GX_WIDGET *parent,
                       ULONG style, USHORT Id,
                       GX_CONST GX_RECTANGLE *size);
UINT _gx_button_deselect(GX_BUTTON *button, GX_BOOL gen_event);
VOID _gx_button_draw(GX_BUTTON *button);
UINT _gx_button_event_process(GX_BUTTON *button, GX_EVENT *event_ptr);
UINT _gx_button_select(GX_BUTTON *button);

UINT _gx_canvas_alpha_set(GX_CANVAS *canvas, GX_UBYTE alpha);
UINT _gx_canvas_arc_draw(INT xcenter, INT ycenter, UINT r, INT start_angle, INT end_angle);
UINT _gx_canvas_block_move(GX_RECTANGLE *block, GX_VALUE x_shift, GX_VALUE y_shift, GX_RECTANGLE *dirty);
UINT _gx_canvas_circle_draw(INT xcenter, INT ycenter, UINT r);
UINT _gx_canvas_create(GX_CANVAS *canvas, GX_CONST GX_CHAR *name, GX_DISPLAY *display,
                       UINT type, UINT width, UINT height, GX_COLOR *memory_area, ULONG memory_size);
UINT _gx_canvas_delete(GX_CANVAS *canvas);
UINT _gx_canvas_drawing_complete(GX_CANVAS *canvas, GX_BOOL Flush);
UINT _gx_canvas_drawing_initiate(GX_CANVAS *canvas, GX_WIDGET *who, GX_RECTANGLE *dirty_area);
UINT _gx_canvas_ellipse_draw(INT xcenter, INT ycenter, INT a, INT b);
UINT _gx_canvas_hardware_layer_bind(GX_CANVAS *canvas, INT layer);
UINT _gx_canvas_hide(GX_CANVAS *canvas);
UINT _gx_canvas_line_draw(GX_VALUE x_start, GX_VALUE y_start, GX_VALUE x_end, GX_VALUE y_end);
UINT _gx_canvas_memory_define(GX_CANVAS *canvas, GX_COLOR *memory, ULONG memsize);

#if defined(GX_MOUSE_SUPPORT)
UINT _gx_canvas_mouse_define(GX_CANVAS *canvas, GX_MOUSE_CURSOR_INFO *info);
UINT _gx_canvas_mouse_hide(GX_CANVAS *canvas);
UINT _gx_canvas_mouse_show(GX_CANVAS *canvas);
#endif
UINT _gx_canvas_offset_set(GX_CANVAS *canvas, GX_VALUE x, GX_VALUE y);
UINT _gx_canvas_pie_draw(INT xcenter, INT ycenter, UINT r, INT start_angle, INT end_angle);
UINT _gx_canvas_pixel_draw(GX_POINT position);
UINT _gx_canvas_pixelmap_blend(GX_VALUE x_position, GX_VALUE y_position, GX_PIXELMAP *pixelmap, GX_UBYTE alpha);
UINT _gx_canvas_pixelmap_draw(GX_VALUE x_position, GX_VALUE y_position, GX_PIXELMAP *pixelmap);
UINT _gx_canvas_pixelmap_get(GX_PIXELMAP *pixelmap);
UINT _gx_canvas_pixelmap_rotate(GX_VALUE x_position, GX_VALUE y_position, GX_PIXELMAP *pixelmap,
                                INT angle, INT rot_cx, INT rot_cy);
UINT _gx_canvas_pixelmap_tile(GX_RECTANGLE *fill, GX_PIXELMAP *pixelmap);
UINT _gx_canvas_polygon_draw(GX_POINT *point_array, INT number_of_points);
UINT _gx_canvas_rectangle_draw(GX_RECTANGLE *rectangle);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gx_canvas_rotated_text_draw(GX_CONST GX_CHAR *text, GX_VALUE xcenter, GX_VALUE ycenter, INT angle);
#endif
UINT _gx_canvas_rotated_text_draw_ext(GX_CONST GX_STRING *text, GX_VALUE xcenter, GX_VALUE ycenter, INT angle);
UINT _gx_canvas_shift(GX_CANVAS *canvas, GX_VALUE x, GX_VALUE y);
UINT _gx_canvas_show(GX_CANVAS *canvas);

UINT _gx_canvas_aligned_text_draw(GX_CONST GX_STRING *string, GX_RECTANGLE *rectangle, ULONG alignment);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gx_canvas_text_draw(GX_VALUE x_start, GX_VALUE y_start, GX_CONST GX_CHAR *string, INT length);
#endif
UINT _gx_canvas_text_draw_ext(GX_VALUE x_start, GX_VALUE y_start, GX_CONST GX_STRING *string);

UINT _gx_checkbox_create(GX_CHECKBOX *checkbox,
                         GX_CONST GX_CHAR *name,
                         GX_WIDGET *parent,
                         GX_RESOURCE_ID text_id, ULONG style, USHORT checkbox_id,
                         GX_CONST GX_RECTANGLE *size);
VOID _gx_checkbox_draw(GX_CHECKBOX *checkbox);
UINT _gx_checkbox_event_process(GX_CHECKBOX *checkbox, GX_EVENT *event_ptr);
UINT _gx_checkbox_pixelmap_set(GX_CHECKBOX *button,  GX_RESOURCE_ID unchecked_id, GX_RESOURCE_ID checked_id,
                               GX_RESOURCE_ID unchecked_disabled_id, GX_RESOURCE_ID checked_disabled_id);
UINT _gx_checkbox_select(GX_CHECKBOX *checkbox);

UINT _gx_circular_gauge_angle_get(GX_CIRCULAR_GAUGE *circular_gauge, INT *angle);
UINT _gx_circular_gauge_angle_set(GX_CIRCULAR_GAUGE *circular_gauge, INT angle);
UINT _gx_circular_gauge_animation_set(GX_CIRCULAR_GAUGE *circular_gauge, INT steps, INT delay);
VOID _gx_circular_gauge_background_draw(GX_CIRCULAR_GAUGE *gauge);
UINT _gx_circular_gauge_create(GX_CIRCULAR_GAUGE *circular_gauge,
                               GX_CONST GX_CHAR *name,
                               GX_WIDGET *parent,
                               GX_CIRCULAR_GAUGE_INFO *circular_gauge_info,
                               GX_RESOURCE_ID background,
                               ULONG style,
                               USHORT circular_gauge_id,
                               GX_VALUE xpos, GX_VALUE ypos);
VOID _gx_circular_gauge_draw(GX_CIRCULAR_GAUGE *circular_gauge);
UINT _gx_circular_gauge_event_process(GX_CIRCULAR_GAUGE *circular_gauge, GX_EVENT *event_ptr);

UINT _gx_context_brush_default(GX_DRAW_CONTEXT *context);
UINT _gx_context_brush_define(GX_RESOURCE_ID line_color_id, GX_RESOURCE_ID fill_color_id, UINT style);
UINT _gx_context_brush_get(GX_BRUSH **return_brush);
UINT _gx_context_brush_pattern_set(ULONG pattern);
UINT _gx_context_brush_set(GX_BRUSH *brush);
UINT _gx_context_brush_style_set(UINT style);
UINT _gx_context_brush_width_set(UINT width);
UINT _gx_context_color_get(GX_RESOURCE_ID color_id, GX_COLOR *return_color);
UINT _gx_context_fill_color_set(GX_RESOURCE_ID fill_color_id);
UINT _gx_context_font_get(GX_RESOURCE_ID color_id, GX_FONT **return_font);
UINT _gx_context_font_set(GX_RESOURCE_ID font_id);
UINT _gx_context_line_color_set(GX_RESOURCE_ID line_color_id);
UINT _gx_context_pixelmap_get(GX_RESOURCE_ID pixelmap_id, GX_PIXELMAP **return_pixelmap);
UINT _gx_context_pixelmap_set(GX_RESOURCE_ID pixelmap_id);
UINT _gx_context_raw_brush_define(GX_COLOR line_color, GX_COLOR fill_color, UINT style);
UINT _gx_context_raw_fill_color_set(GX_COLOR line_color);
UINT _gx_context_raw_line_color_set(GX_COLOR line_color);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gx_context_string_get(GX_RESOURCE_ID string_id, GX_CONST GX_CHAR **return_string);
#endif
UINT _gx_context_string_get_ext(GX_RESOURCE_ID string_id, GX_STRING *return_string);

UINT _gx_display_active_language_set(GX_DISPLAY *display, GX_UBYTE language);
UINT _gx_display_color_set(GX_DISPLAY *display, GX_RESOURCE_ID resource_id, GX_COLOR new_color);
UINT _gx_display_color_table_set(GX_DISPLAY *display, GX_COLOR *color_table, INT number_of_colors);
UINT _gx_display_create(GX_DISPLAY *display, GX_CONST GX_CHAR *name, UINT (*display_driver_setup)(GX_DISPLAY *), GX_VALUE xres, GX_VALUE yres);
UINT _gx_display_delete(GX_DISPLAY *display, VOID (*display_driver_cleanup)(GX_DISPLAY *));
UINT _gx_display_font_table_set(GX_DISPLAY *display, GX_FONT **font_table, UINT number_of_fonts);

#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gx_display_language_table_get(GX_DISPLAY *display, GX_CHAR ****table, GX_UBYTE *language_count, UINT *string_count);
UINT _gx_display_language_table_set(GX_DISPLAY *display, GX_CHAR ***table, GX_UBYTE num_languages, UINT number_of_strings);
#endif
UINT _gx_display_language_table_get_ext(GX_DISPLAY *display, GX_STRING ***table, GX_UBYTE *language_count, UINT *string_count);
UINT _gx_display_language_table_set_ext(GX_DISPLAY *display, GX_CONST GX_STRING **table, GX_UBYTE num_languages, UINT number_of_strings);
#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
UINT _gx_display_language_direction_table_set(GX_DISPLAY *display, GX_CONST GX_UBYTE *language_direction_table, GX_UBYTE num_languages);
#endif
UINT _gx_display_pixelmap_table_set(GX_DISPLAY *display, GX_PIXELMAP **pixelmap_table, UINT number_of_pixelmaps);

#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gx_display_string_get(GX_DISPLAY *display, GX_RESOURCE_ID string_id, GX_CONST GX_CHAR **return_string);
UINT _gx_display_string_table_get(GX_DISPLAY *display, GX_UBYTE language, GX_CHAR ***table, UINT *size);
#endif
UINT _gx_display_string_get_ext(GX_DISPLAY *display, GX_RESOURCE_ID string_id, GX_STRING *return_string);
UINT _gx_display_string_table_get_ext(GX_DISPLAY *display, GX_UBYTE language, GX_STRING **table, UINT *size);

UINT _gx_display_theme_install(GX_DISPLAY *display, GX_CONST GX_THEME *theme_ptr);

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

UINT _gx_generic_scroll_wheel_create(GX_GENERIC_SCROLL_WHEEL *wheel,
                                     GX_CONST GX_CHAR *name,
                                     GX_WIDGET *parent,
                                     INT total_rows,
                                     VOID (*callback)(GX_GENERIC_SCROLL_WHEEL *, GX_WIDGET *, INT),
                                     ULONG style,
                                     USHORT id,
                                     GX_CONST GX_RECTANGLE *size);
UINT _gx_generic_scroll_wheel_children_position(GX_GENERIC_SCROLL_WHEEL *wheel);
UINT _gx_generic_scroll_wheel_event_process(GX_GENERIC_SCROLL_WHEEL *wheel, GX_EVENT *event_ptr);
UINT _gx_generic_scroll_wheel_row_height_set(GX_GENERIC_SCROLL_WHEEL *wheel, GX_VALUE row_height);
UINT _gx_generic_scroll_wheel_total_rows_set(GX_GENERIC_SCROLL_WHEEL *wheel, INT count);

UINT _gx_horizontal_list_children_position(GX_HORIZONTAL_LIST *horizontal_list);
UINT _gx_horizontal_list_create(GX_HORIZONTAL_LIST *horizontal_list,
                                GX_CONST GX_CHAR *name,
                                GX_WIDGET *parent,
                                INT total_columns,
                                VOID (*callback)(GX_HORIZONTAL_LIST *, GX_WIDGET *, INT),
                                ULONG style, USHORT horizontal_list_id,
                                GX_CONST GX_RECTANGLE *size);
UINT _gx_horizontal_list_event_process(GX_HORIZONTAL_LIST *list, GX_EVENT *event_ptr);
UINT _gx_horizontal_list_page_index_set(GX_HORIZONTAL_LIST *list, INT index);
UINT _gx_horizontal_list_selected_index_get(GX_HORIZONTAL_LIST *horizontal_list, INT *return_index);
UINT _gx_horizontal_list_selected_set(GX_HORIZONTAL_LIST *horizontal_list, INT index);
UINT _gx_horizontal_list_selected_widget_get(GX_HORIZONTAL_LIST *horizontal_list, GX_WIDGET **return_list_entry);
UINT _gx_horizontal_list_total_columns_set(GX_HORIZONTAL_LIST *horizontal_list, INT count);

UINT _gx_horizontal_scrollbar_create(GX_SCROLLBAR *scrollbar, GX_CONST GX_CHAR *name,
                                     GX_WINDOW *parent, GX_SCROLLBAR_APPEARANCE *appearance, ULONG style);

VOID _gx_icon_background_draw(GX_ICON *icon);
UINT _gx_icon_button_create(GX_ICON_BUTTON *button,
                            GX_CONST GX_CHAR *name,
                            GX_WIDGET *parent,
                            GX_RESOURCE_ID icon_id,
                            ULONG style, USHORT icon_button_id,
                            GX_CONST GX_RECTANGLE *size);
VOID _gx_icon_button_draw(GX_ICON_BUTTON *button);
UINT _gx_icon_button_pixelmap_set(GX_ICON_BUTTON *button, GX_RESOURCE_ID icon_id);
UINT _gx_icon_create(GX_ICON *icon,
                     GX_CONST GX_CHAR *name,
                     GX_WIDGET *parent,
                     GX_RESOURCE_ID pixelmap_id, ULONG style,
                     USHORT icon_id, GX_VALUE x, GX_VALUE y);
VOID _gx_icon_draw(GX_ICON *icon);
UINT _gx_icon_event_process(GX_ICON *icon, GX_EVENT *event_ptr);
UINT _gx_icon_pixelmap_set(GX_ICON *icon, GX_RESOURCE_ID normal_id, GX_RESOURCE_ID selected_id);

UINT _gx_image_reader_create(GX_IMAGE_READER *image_reader,
                             GX_CONST GX_UBYTE *read_data,
                             INT read_data_size,
                             GX_UBYTE color_format,
                             GX_UBYTE mode);
UINT _gx_image_reader_palette_set(GX_IMAGE_READER *image_reader, GX_COLOR *pal, UINT palsize);
UINT _gx_image_reader_start(GX_IMAGE_READER *image_reader, GX_PIXELMAP *outmap);

VOID _gx_line_chart_axis_draw(GX_LINE_CHART *chart);
UINT _gx_line_chart_create(GX_LINE_CHART *chart,
                           GX_CONST GX_CHAR *name,
                           GX_WIDGET *parent,
                           GX_CONST GX_LINE_CHART_INFO *info,
                           ULONG style,
                           USHORT chart_id,
                           GX_CONST GX_RECTANGLE *size);
VOID _gx_line_chart_data_draw(GX_LINE_CHART *chart);
VOID _gx_line_chart_draw(GX_LINE_CHART *chart);
UINT _gx_line_chart_update(GX_LINE_CHART *chart, INT *data, INT data_count);
UINT _gx_line_chart_y_scale_calculate(GX_LINE_CHART *chart, INT *return_val);

UINT _gx_menu_create(GX_MENU *menu, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                     GX_RESOURCE_ID text_id, GX_RESOURCE_ID fill_id,
                     ULONG style, USHORT menu_id, GX_CONST GX_RECTANGLE *size);
VOID _gx_menu_draw(GX_MENU *menu);
UINT _gx_menu_event_process(GX_MENU *menu, GX_EVENT *event_ptr);
UINT _gx_menu_insert(GX_MENU *menu, GX_WIDGET *widget);
UINT _gx_menu_remove(GX_MENU *menu, GX_WIDGET *widget);
UINT _gx_menu_remove(GX_MENU *menu, GX_WIDGET *widget);
VOID _gx_menu_text_draw(GX_MENU *menu);
UINT _gx_menu_text_offset_set(GX_MENU *menu, GX_VALUE x_offset, GX_VALUE y_offset);

UINT _gx_multi_line_text_button_create(GX_MULTI_LINE_TEXT_BUTTON *button, GX_CONST GX_CHAR *name,
                                       GX_WIDGET *parent, GX_RESOURCE_ID text_id, ULONG style, USHORT Id,
                                       GX_CONST GX_RECTANGLE *size);
VOID _gx_multi_line_text_button_draw(GX_MULTI_LINE_TEXT_BUTTON *button);
UINT _gx_multi_line_text_button_event_process(GX_MULTI_LINE_TEXT_BUTTON *button, GX_EVENT *event_ptr);
VOID _gx_multi_line_text_button_text_draw(GX_MULTI_LINE_TEXT_BUTTON *button);
UINT _gx_multi_line_text_button_text_id_set(GX_MULTI_LINE_TEXT_BUTTON *button, GX_RESOURCE_ID string_id);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gx_multi_line_text_button_text_set(GX_MULTI_LINE_TEXT_BUTTON *button, GX_CONST GX_CHAR *text);
#endif
UINT _gx_multi_line_text_button_text_set_ext(GX_MULTI_LINE_TEXT_BUTTON *button, GX_CONST GX_STRING text);

UINT _gx_multi_line_text_input_backspace(GX_MULTI_LINE_TEXT_INPUT *text_input);
UINT _gx_multi_line_text_input_buffer_clear(GX_MULTI_LINE_TEXT_INPUT *text_input_ptr);
UINT _gx_multi_line_text_input_buffer_get(GX_MULTI_LINE_TEXT_INPUT *text_input_ptr, GX_CHAR **buffer_address,
                                          UINT *content_size, UINT *buffer_size);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gx_multi_line_text_input_char_insert(GX_MULTI_LINE_TEXT_INPUT *text_input, GX_UBYTE *str, UINT str_size);
#endif
UINT _gx_multi_line_text_input_char_insert_ext(GX_MULTI_LINE_TEXT_INPUT *text_input, GX_CONST GX_STRING *str);
UINT _gx_multi_line_text_input_create(GX_MULTI_LINE_TEXT_INPUT *text_input_ptr,
                                      GX_CONST GX_CHAR *name_ptr, GX_WIDGET *parent,
                                      GX_CHAR *input_buffer, UINT buffer_size,
                                      ULONG style, USHORT Id, GX_CONST GX_RECTANGLE *size);
UINT _gx_multi_line_text_input_cursor_pos_get(GX_MULTI_LINE_TEXT_INPUT *input, GX_POINT *cursor_pos);
UINT _gx_multi_line_text_input_delete(GX_MULTI_LINE_TEXT_INPUT *text_input);
UINT _gx_multi_line_text_input_down_arrow(GX_MULTI_LINE_TEXT_INPUT *text_input);
UINT _gx_multi_line_text_input_end(GX_MULTI_LINE_TEXT_INPUT *text_input);
UINT _gx_multi_line_text_input_event_process(GX_MULTI_LINE_TEXT_INPUT *input, GX_EVENT *event_ptr);
UINT _gx_multi_line_text_input_style_add(GX_MULTI_LINE_TEXT_INPUT *text_input_ptr, ULONG cursor_type);
UINT _gx_multi_line_text_input_style_remove(GX_MULTI_LINE_TEXT_INPUT *text_input_ptr, ULONG cursor_type);
UINT _gx_multi_line_text_input_style_set(GX_MULTI_LINE_TEXT_INPUT *text_input_ptr, ULONG cursor_type);
UINT _gx_multi_line_text_input_fill_color_set(GX_MULTI_LINE_TEXT_INPUT *view,
                                              GX_RESOURCE_ID normal_id,
                                              GX_RESOURCE_ID selected_id,
                                              GX_RESOURCE_ID disabled_id,
                                              GX_RESOURCE_ID readonly_id);
UINT _gx_multi_line_text_input_home(GX_MULTI_LINE_TEXT_INPUT *text_input);
UINT _gx_multi_line_text_input_left_arrow(GX_MULTI_LINE_TEXT_INPUT *text_input);
UINT _gx_multi_line_text_input_right_arrow(GX_MULTI_LINE_TEXT_INPUT *text_input);
UINT _gx_multi_line_text_input_text_color_set(GX_MULTI_LINE_TEXT_INPUT *view,
                                              GX_RESOURCE_ID normal_id,
                                              GX_RESOURCE_ID selected_id,
                                              GX_RESOURCE_ID disabled_id,
                                              GX_RESOURCE_ID readonly_id);
UINT _gx_multi_line_text_input_text_select(GX_MULTI_LINE_TEXT_INPUT *input, UINT start_index, UINT end_index);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gx_multi_line_text_input_text_set(GX_MULTI_LINE_TEXT_INPUT *text_input, GX_CONST GX_CHAR *text);
#endif
UINT _gx_multi_line_text_input_text_set_ext(GX_MULTI_LINE_TEXT_INPUT *text_input, GX_CONST GX_STRING *text);
UINT _gx_multi_line_text_input_up_arrow(GX_MULTI_LINE_TEXT_INPUT *text_input);

UINT _gx_multi_line_text_view_create(GX_MULTI_LINE_TEXT_VIEW *text_view_ptr,
                                     GX_CONST GX_CHAR *name_ptr, GX_WIDGET *parent,
                                     GX_RESOURCE_ID text_id, ULONG style, USHORT Id,
                                     GX_CONST GX_RECTANGLE *size);
VOID _gx_multi_line_text_view_draw(GX_MULTI_LINE_TEXT_VIEW *view);
UINT _gx_multi_line_text_view_event_process(GX_MULTI_LINE_TEXT_VIEW *view, GX_EVENT *event_ptr);
UINT _gx_multi_line_text_view_font_set(GX_MULTI_LINE_TEXT_VIEW *view, GX_RESOURCE_ID font_id);
UINT _gx_multi_line_text_view_line_space_set(GX_MULTI_LINE_TEXT_VIEW *view, GX_BYTE line_space);
UINT _gx_multi_line_text_view_scroll_info_get(GX_MULTI_LINE_TEXT_VIEW *view, ULONG style, GX_SCROLL_INFO *return_scroll_info);
UINT _gx_multi_line_text_view_text_color_set(GX_MULTI_LINE_TEXT_VIEW *view, GX_RESOURCE_ID normal_id, GX_RESOURCE_ID selected_id, GX_RESOURCE_ID disabled_id);
UINT _gx_multi_line_text_view_text_id_set(GX_MULTI_LINE_TEXT_VIEW *text_view_ptr, GX_RESOURCE_ID text_id);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gx_multi_line_text_view_text_set(GX_MULTI_LINE_TEXT_VIEW *view, GX_CONST GX_CHAR *text);
#endif
UINT _gx_multi_line_text_view_text_set_ext(GX_MULTI_LINE_TEXT_VIEW *view, GX_CONST GX_STRING *text);
UINT _gx_multi_line_text_view_whitespace_set(GX_MULTI_LINE_TEXT_VIEW *view, GX_UBYTE whitespace);

UINT _gx_rich_text_view_create(GX_RICH_TEXT_VIEW *rich_view,
                               GX_CONST GX_CHAR *name,
                               GX_WIDGET *parent,
                               GX_RESOURCE_ID text_id,
                               GX_RICH_TEXT_FONTS *fonts,
                               ULONG style,
                               USHORT id,
                               GX_CONST GX_RECTANGLE *size);
VOID _gx_rich_text_view_draw(GX_RICH_TEXT_VIEW *text_view);
VOID _gx_rich_text_view_text_draw(GX_RICH_TEXT_VIEW *text_view);
UINT _gx_rich_text_view_fonts_set(GX_RICH_TEXT_VIEW *rich_view, GX_RICH_TEXT_FONTS *fonts);

UINT _gx_numeric_pixelmap_prompt_create(GX_NUMERIC_PIXELMAP_PROMPT *prompt,
                                        GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                                        GX_RESOURCE_ID text_id, GX_RESOURCE_ID fill_id,
                                        ULONG style, USHORT pixelmap_prompt_id,
                                        GX_CONST GX_RECTANGLE *size);
UINT _gx_numeric_pixelmap_prompt_format_function_set(GX_NUMERIC_PIXELMAP_PROMPT *prompt, VOID (*format_func)(GX_NUMERIC_PIXELMAP_PROMPT *, INT));
UINT _gx_numeric_pixelmap_prompt_value_set(GX_NUMERIC_PIXELMAP_PROMPT *prompt, INT value);

UINT _gx_numeric_prompt_create(GX_NUMERIC_PROMPT *prompt, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                               GX_RESOURCE_ID text_id, ULONG style, USHORT prompt_id, GX_CONST GX_RECTANGLE *size);
UINT _gx_numeric_prompt_format_function_set(GX_NUMERIC_PROMPT *prompt, VOID (*format_func)(GX_NUMERIC_PROMPT *, INT));
UINT _gx_numeric_prompt_value_set(GX_NUMERIC_PROMPT *prompt, INT value);

UINT _gx_numeric_scroll_wheel_create(GX_NUMERIC_SCROLL_WHEEL *wheel, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                                     INT start_val, INT end_val,
                                     ULONG style, USHORT Id, GX_CONST GX_RECTANGLE *size);
UINT _gx_numeric_scroll_wheel_range_set(GX_NUMERIC_SCROLL_WHEEL *wheel, INT start_val, INT end_val);

UINT _gx_pixelmap_button_create(GX_PIXELMAP_BUTTON *button,
                                GX_CONST GX_CHAR *name,
                                GX_WIDGET *parent, GX_RESOURCE_ID normal_id,
                                GX_RESOURCE_ID selected_id, GX_RESOURCE_ID disabled_id,
                                ULONG style, USHORT pixelmap_button_id,
                                GX_CONST GX_RECTANGLE *size);
VOID _gx_pixelmap_button_draw(GX_PIXELMAP_BUTTON *button);
UINT _gx_pixelmap_button_event_process(GX_PIXELMAP_BUTTON *button, GX_EVENT *event_ptr);
UINT _gx_pixelmap_button_pixelmap_set(GX_PIXELMAP_BUTTON *button, GX_RESOURCE_ID normal_id, GX_RESOURCE_ID selected_id, GX_RESOURCE_ID disabled_id);

UINT _gx_pixelmap_prompt_create(GX_PIXELMAP_PROMPT *prompt,
                                GX_CONST GX_CHAR *name, GX_WIDGET *parent, GX_RESOURCE_ID text_id,
                                GX_RESOURCE_ID normal_fill_id, ULONG style, USHORT prompt_id,
                                GX_CONST GX_RECTANGLE *size);
VOID _gx_pixelmap_prompt_draw(GX_PIXELMAP_PROMPT *prompt);
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
VOID _gx_pixelmap_slider_draw(GX_PIXELMAP_SLIDER *slider);
UINT _gx_pixelmap_slider_event_process(GX_PIXELMAP_SLIDER *slider, GX_EVENT *event_ptr);
UINT _gx_pixelmap_slider_pixelmap_set(GX_PIXELMAP_SLIDER *slider, GX_PIXELMAP_SLIDER_INFO *info);

VOID _gx_progress_bar_background_draw(GX_PROGRESS_BAR *progress_bar);
UINT _gx_progress_bar_create(GX_PROGRESS_BAR *progress_bar, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                             GX_PROGRESS_BAR_INFO *progress_bar_info, ULONG style,
                             USHORT progress_bar_id, GX_CONST GX_RECTANGLE *size);
VOID _gx_progress_bar_draw(GX_PROGRESS_BAR *progress_bar);
UINT _gx_progress_bar_event_process(GX_PROGRESS_BAR *progress_bar, GX_EVENT *event_ptr);
UINT _gx_progress_bar_font_set(GX_PROGRESS_BAR *progress_bar, GX_RESOURCE_ID font_id);
UINT _gx_progress_bar_info_set(GX_PROGRESS_BAR *progress_bar, GX_PROGRESS_BAR_INFO *info);
UINT _gx_progress_bar_pixelmap_set(GX_PROGRESS_BAR *progress_bar, GX_RESOURCE_ID pixelmap);
UINT _gx_progress_bar_range_set(GX_PROGRESS_BAR *progress_bar, INT min_value, INT max_value);
UINT _gx_progress_bar_text_color_set(GX_PROGRESS_BAR *progress_bar, GX_RESOURCE_ID normal_text_color_id,
                                     GX_RESOURCE_ID selected_text_color_id, GX_RESOURCE_ID disabled_text_color_id);
UINT _gx_progress_bar_value_set(GX_PROGRESS_BAR *progress_bar, INT new_value);

UINT _gx_prompt_create(GX_PROMPT *prompt, GX_CONST GX_CHAR *name,
                       GX_WIDGET *parent, GX_RESOURCE_ID text_id, ULONG style,
                       USHORT prompt_id, GX_CONST GX_RECTANGLE *size);
UINT _gx_prompt_event_process(GX_PROMPT *prompt, GX_EVENT *event_ptr);
VOID _gx_prompt_draw(GX_PROMPT *prompt);
UINT _gx_prompt_font_set(GX_PROMPT *prompt, GX_RESOURCE_ID fontid);
UINT _gx_prompt_text_color_set(GX_PROMPT *prompt,
                               GX_RESOURCE_ID normal_text_color_id,
                               GX_RESOURCE_ID selected_text_color_id,
                               GX_RESOURCE_ID disabled_text_color_id);
VOID _gx_prompt_text_draw(GX_PROMPT *prompt);

#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gx_prompt_text_get(GX_PROMPT *prompt, GX_CONST GX_CHAR **return_text);
#endif
UINT _gx_prompt_text_get_ext(GX_PROMPT *prompt, GX_STRING *return_text);
UINT _gx_prompt_text_id_set(GX_PROMPT *prompt, GX_RESOURCE_ID string_id);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gx_prompt_text_set(GX_PROMPT *prompt, GX_CONST GX_CHAR *text);
#endif
UINT _gx_prompt_text_set_ext(GX_PROMPT *prompt, GX_CONST GX_STRING *text);

UINT _gx_radial_progress_bar_anchor_set(GX_RADIAL_PROGRESS_BAR *progress_bar, GX_VALUE angle);
VOID _gx_radial_progress_bar_background_draw(GX_RADIAL_PROGRESS_BAR *progress_bar);
UINT _gx_radial_progress_bar_create(GX_RADIAL_PROGRESS_BAR *progress_bar,
                                    GX_CONST GX_CHAR *name,
                                    GX_WIDGET *parent,
                                    GX_RADIAL_PROGRESS_BAR_INFO *progress_bar_info,
                                    ULONG style,
                                    USHORT progress_bar_id);
VOID _gx_radial_progress_bar_draw(GX_RADIAL_PROGRESS_BAR *progress_bar);
UINT _gx_radial_progress_bar_event_process(GX_RADIAL_PROGRESS_BAR *progress_bar, GX_EVENT *event_ptr);
UINT _gx_radial_progress_bar_font_set(GX_RADIAL_PROGRESS_BAR *progress_bar, GX_RESOURCE_ID font_id);
UINT _gx_radial_progress_bar_info_set(GX_RADIAL_PROGRESS_BAR *bar, GX_RADIAL_PROGRESS_BAR_INFO *info);
UINT _gx_radial_progress_bar_text_color_set(GX_RADIAL_PROGRESS_BAR *progress_bar,
                                            GX_RESOURCE_ID normal_text_color_id,
                                            GX_RESOURCE_ID selected_text_color_id,
                                            GX_RESOURCE_ID disabled_text_color_id);
VOID _gx_radial_progress_bar_text_draw(GX_RADIAL_PROGRESS_BAR *bar);
UINT _gx_radial_progress_bar_value_set(GX_RADIAL_PROGRESS_BAR *progress_bar, GX_VALUE new_value);

UINT _gx_radial_slider_anchor_angles_set(GX_RADIAL_SLIDER *slider, GX_VALUE *anchor_angles, USHORT anchor_count);
UINT _gx_radial_slider_animation_set(GX_RADIAL_SLIDER *slider, USHORT steps, USHORT delay, USHORT animation_style,
                                     VOID (*animation_update_callback)(GX_RADIAL_SLIDER *slider));
UINT _gx_radial_slider_animation_start(GX_RADIAL_SLIDER *slider, GX_VALUE new_value);
UINT _gx_radial_slider_create(GX_RADIAL_SLIDER *slider, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                              GX_RADIAL_SLIDER_INFO *info, ULONG style, USHORT slider_id, GX_CONST GX_RECTANGLE *size);
VOID _gx_radial_slider_draw(GX_RADIAL_SLIDER *slider);
UINT _gx_radial_slider_event_process(GX_RADIAL_SLIDER *slider, GX_EVENT *event_ptr);
UINT _gx_radial_slider_info_get(GX_RADIAL_SLIDER *slider, GX_RADIAL_SLIDER_INFO **info);
UINT _gx_radial_slider_info_set(GX_RADIAL_SLIDER *slider,  GX_RADIAL_SLIDER_INFO *info);
UINT _gx_radial_slider_pixelmap_set(GX_RADIAL_SLIDER *slider, GX_RESOURCE_ID background_pixelmap, GX_RESOURCE_ID needle_pixelmap);
UINT _gx_radial_slider_angle_set(GX_RADIAL_SLIDER *slider, GX_VALUE new_angle);

UINT _gx_radio_button_create(GX_RADIO_BUTTON *button,
                             GX_CONST GX_CHAR *name, GX_WIDGET *parent, GX_RESOURCE_ID text_id,
                             ULONG style, USHORT radio_button_id, GX_CONST GX_RECTANGLE *size);
VOID _gx_radio_button_draw(GX_RADIO_BUTTON *button);
UINT _gx_radio_button_pixelmap_set(GX_RADIO_BUTTON *button, GX_RESOURCE_ID off_id, GX_RESOURCE_ID on_id,
                                   GX_RESOURCE_ID off_disabled_id, GX_RESOURCE_ID on_disabled_id);

UINT _gx_screen_stack_create(GX_SCREEN_STACK_CONTROL *control, GX_WIDGET **memory, INT size);
UINT _gx_screen_stack_pop(GX_SCREEN_STACK_CONTROL *control);
UINT _gx_screen_stack_push(GX_SCREEN_STACK_CONTROL *control, GX_WIDGET *screen, GX_WIDGET *new_screen);
UINT _gx_screen_stack_reset(GX_SCREEN_STACK_CONTROL *control);

UINT _gx_scroll_thumb_create(GX_SCROLL_THUMB *scroll_thumb, GX_SCROLLBAR *parent, ULONG style);
VOID _gx_scroll_thumb_draw(GX_SCROLL_THUMB *scroll_thumb);
UINT _gx_scroll_thumb_event_process(GX_SCROLL_THUMB *scroll_thumb, GX_EVENT *event_ptr);

VOID _gx_scrollbar_draw(GX_SCROLLBAR *scrollbar);
UINT _gx_scrollbar_event_process(GX_SCROLLBAR *scrollbar, GX_EVENT *event_ptr);
UINT _gx_scrollbar_limit_check(GX_SCROLLBAR *scrollbar);
UINT _gx_scrollbar_reset(GX_SCROLLBAR *scrollbar, GX_SCROLL_INFO *info);
UINT _gx_scrollbar_value_set(GX_SCROLLBAR *scrollbar, INT value);

UINT _gx_scroll_wheel_create(GX_SCROLL_WHEEL *wheel, GX_CONST GX_CHAR *name, GX_WIDGET *parent, INT total_rows,
                             ULONG style, USHORT Id, GX_CONST GX_RECTANGLE *size);
UINT _gx_scroll_wheel_event_process(GX_SCROLL_WHEEL *wheel, GX_EVENT *event_ptr);
UINT _gx_scroll_wheel_gradient_alpha_set(GX_SCROLL_WHEEL *wheel, GX_UBYTE start_alpha, GX_UBYTE end_alpha);
UINT _gx_scroll_wheel_row_height_set(GX_SCROLL_WHEEL *wheel, GX_VALUE row_height);
UINT _gx_scroll_wheel_selected_background_set(GX_SCROLL_WHEEL *wheel, GX_RESOURCE_ID selected_bg);
UINT _gx_scroll_wheel_selected_get(GX_SCROLL_WHEEL *wheel, INT *row);
UINT _gx_scroll_wheel_selected_set(GX_SCROLL_WHEEL *wheel, INT row);
UINT _gx_scroll_wheel_speed_set(GX_SCROLL_WHEEL *wheel, GX_FIXED_VAL start_speed_rate, GX_FIXED_VAL end_speed_rate, GX_VALUE max_steps, GX_VALUE delay);
UINT _gx_scroll_wheel_total_rows_set(GX_SCROLL_WHEEL *wheel, INT total_rows);

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
VOID _gx_single_line_text_input_draw(GX_SINGLE_LINE_TEXT_INPUT *text_input);
UINT _gx_single_line_text_input_draw_position_get(GX_SINGLE_LINE_TEXT_INPUT *input, GX_VALUE *xpos, GX_VALUE *ypos);
UINT _gx_single_line_text_input_end(GX_SINGLE_LINE_TEXT_INPUT *text_input);
UINT _gx_single_line_text_input_event_process(GX_SINGLE_LINE_TEXT_INPUT *text_input, GX_EVENT *event_ptr);
UINT _gx_single_line_text_input_fill_color_set(GX_SINGLE_LINE_TEXT_INPUT *input,
                                               GX_RESOURCE_ID normal_fill_color_id,
                                               GX_RESOURCE_ID selected_fill_color_id,
                                               GX_RESOURCE_ID disabled_fill_color_id,
                                               GX_RESOURCE_ID readonly_fill_color_id);
UINT _gx_single_line_text_input_home(GX_SINGLE_LINE_TEXT_INPUT *text_input);
UINT _gx_single_line_text_input_left_arrow(GX_SINGLE_LINE_TEXT_INPUT *text_input);
UINT _gx_single_line_text_input_position_get(GX_SINGLE_LINE_TEXT_INPUT *text_input, INT pixel_position);
UINT _gx_single_line_text_input_right_arrow(GX_SINGLE_LINE_TEXT_INPUT *text_input);
UINT _gx_single_line_text_input_style_add(GX_SINGLE_LINE_TEXT_INPUT *text_input_ptr, ULONG style);
UINT _gx_single_line_text_input_style_remove(GX_SINGLE_LINE_TEXT_INPUT *text_input_ptr, ULONG style);
UINT _gx_single_line_text_input_style_set(GX_SINGLE_LINE_TEXT_INPUT *text_input_ptr, ULONG style);
UINT _gx_single_line_text_input_text_color_set(GX_SINGLE_LINE_TEXT_INPUT *input,
                                               GX_RESOURCE_ID normal_text_color_id,
                                               GX_RESOURCE_ID selected_text_color_id,
                                               GX_RESOURCE_ID disabled_text_color_id,
                                               GX_RESOURCE_ID readonly_text_color_id);
UINT _gx_single_line_text_input_text_select(GX_SINGLE_LINE_TEXT_INPUT *input, UINT start_index, UINT end_index);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gx_single_line_text_input_text_set(GX_SINGLE_LINE_TEXT_INPUT *input, GX_CONST GX_CHAR *text);
#endif
UINT _gx_single_line_text_input_text_set_ext(GX_SINGLE_LINE_TEXT_INPUT *input, GX_CONST GX_STRING *text);

UINT _gx_slider_create(GX_SLIDER *slider, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                       INT tick_count, GX_SLIDER_INFO *slider_info, ULONG style, USHORT slider_id,
                       GX_CONST GX_RECTANGLE *size);
VOID _gx_slider_draw(GX_SLIDER *slider);
UINT _gx_slider_event_process(GX_SLIDER *slider, GX_EVENT *event_ptr);
UINT _gx_slider_info_set(GX_SLIDER *slider, GX_SLIDER_INFO *info);
VOID _gx_slider_needle_draw(GX_SLIDER *slider);
UINT _gx_slider_needle_position_get(GX_SLIDER *slider, GX_SLIDER_INFO *slider_info, GX_RECTANGLE *return_position);
VOID _gx_slider_tickmarks_draw(GX_SLIDER *slider);
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

#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gx_string_scroll_wheel_create(GX_STRING_SCROLL_WHEEL *wheel, GX_CONST GX_CHAR *name, GX_WIDGET *parent, INT total_rows,
                                    GX_CONST GX_CHAR **string_list,
                                    ULONG style, USHORT Id, GX_CONST GX_RECTANGLE *size);
#endif
UINT _gx_string_scroll_wheel_create_ext(GX_STRING_SCROLL_WHEEL *wheel, GX_CONST GX_CHAR *name, GX_WIDGET *parent, INT total_rows,
                                        GX_CONST GX_STRING *string_list,
                                        ULONG style, USHORT Id, GX_CONST GX_RECTANGLE *size);
UINT _gx_string_scroll_wheel_event_process(GX_STRING_SCROLL_WHEEL *wheel, GX_EVENT *event_ptr);
UINT _gx_string_scroll_wheel_string_id_list_set(GX_STRING_SCROLL_WHEEL *wheel,
                                                GX_CONST GX_RESOURCE_ID *string_id_list,
                                                INT id_count);

#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gx_string_scroll_wheel_string_list_set(GX_STRING_SCROLL_WHEEL *wheel,
                                             GX_CONST GX_CHAR **string_list,
                                             INT string_count);
#endif
UINT _gx_string_scroll_wheel_string_list_set_ext(GX_STRING_SCROLL_WHEEL *wheel,
                                                 GX_CONST GX_STRING *string_list,
                                                 INT string_count);

UINT _gx_system_active_language_set(GX_UBYTE language);

#if (GX_ANIMATION_POOL_SIZE > 0)
UINT _gx_system_animation_get(GX_ANIMATION **free_return);
UINT _gx_system_animation_free(GX_ANIMATION *free_return);
#endif

#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
UINT _gx_system_bidi_text_enable(VOID);
UINT _gx_system_bidi_text_disable(VOID);
#endif

#if defined(GX_THAI_GLYPH_SHAPING_SUPPORT)
UINT _gx_system_text_render_style_set(GX_UBYTE style);
#endif

UINT _gx_system_canvas_refresh(VOID);
UINT _gx_system_dirty_mark(GX_WIDGET *widget);
UINT _gx_system_dirty_partial_add(GX_WIDGET *widget, GX_RECTANGLE *dirty_area);
UINT _gx_system_draw_context_get(GX_DRAW_CONTEXT **current_context);
UINT _gx_system_event_fold(GX_EVENT *in_event);
UINT _gx_system_event_send(GX_EVENT *in_event);
UINT _gx_system_focus_claim(GX_WIDGET *me);
UINT _gx_system_initialize(VOID);

#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gx_system_language_table_get(GX_CHAR ****language_table, GX_UBYTE *languages_count, UINT *string_count);
UINT _gx_system_language_table_set(GX_CHAR ***language_table, GX_UBYTE number_of_languages, UINT number_of_strings);
#endif

UINT _gx_system_memory_allocator_set(VOID *(*allocate)(ULONG size), VOID (*release)(VOID *));
UINT _gx_system_pen_configure(GX_PEN_CONFIGURATION *pen_configuration);
UINT _gx_system_screen_stack_create(GX_WIDGET **memory, INT size);
UINT _gx_system_screen_stack_pop(VOID);
UINT _gx_system_screen_stack_push(GX_WIDGET *screen);
UINT _gx_system_screen_stack_get(GX_WIDGET **popped_parent, GX_WIDGET **popped_screen);
UINT _gx_system_screen_stack_reset(VOID);
UINT _gx_system_scroll_appearance_get(ULONG style, GX_SCROLLBAR_APPEARANCE *appearance);
UINT _gx_system_scroll_appearance_set(ULONG style, GX_SCROLLBAR_APPEARANCE *appearance);
UINT _gx_system_start(VOID);

#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gx_system_string_get(GX_RESOURCE_ID string_id, GX_CONST GX_CHAR **return_string);
UINT _gx_system_string_table_get(GX_UBYTE language, GX_CHAR ***get_table, UINT *get_size);
UINT _gx_system_string_width_get(GX_CONST GX_FONT *font, GX_CONST GX_CHAR *string, INT string_length, GX_VALUE *return_width);
#endif
UINT _gx_system_string_width_get_ext(GX_CONST GX_FONT *font, GX_CONST GX_STRING *string, GX_VALUE *return_width);
UINT _gx_system_timer_start(GX_WIDGET *owner, UINT timer_id, UINT initial_ticks, UINT reschedule_ticks);
UINT _gx_system_timer_stop(GX_WIDGET *owner, UINT timer_id);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gx_system_version_string_get(GX_CHAR **return_string);
#endif
UINT _gx_system_version_string_get_ext(GX_STRING *return_string);
UINT _gx_system_widget_find(USHORT widget_id, INT search_level, GX_WIDGET **return_search_result);

UINT _gx_text_button_create(GX_TEXT_BUTTON *button, GX_CONST GX_CHAR *name,
                            GX_WIDGET *parent, GX_RESOURCE_ID text_id, ULONG style, USHORT Id,
                            GX_CONST GX_RECTANGLE *size);
VOID _gx_text_button_draw(GX_TEXT_BUTTON *button);
UINT _gx_text_button_event_process(GX_TEXT_BUTTON *button, GX_EVENT *event_ptr);
UINT _gx_text_button_font_set(GX_TEXT_BUTTON *button, GX_RESOURCE_ID font_id);
UINT _gx_text_button_text_color_set(GX_TEXT_BUTTON *text_button,
                                    GX_RESOURCE_ID normal_text_color_id,
                                    GX_RESOURCE_ID selected_text_color_id,
                                    GX_RESOURCE_ID disabled_text_color_id);
VOID _gx_text_button_text_draw(GX_TEXT_BUTTON *button);
UINT _gx_text_button_text_get(GX_TEXT_BUTTON *button, GX_CONST GX_CHAR **return_text);
UINT _gx_text_button_text_id_set(GX_TEXT_BUTTON *button, GX_RESOURCE_ID string_id);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gx_text_button_text_set(GX_TEXT_BUTTON *button, GX_CONST GX_CHAR *text);
#endif
UINT _gx_text_button_text_set_ext(GX_TEXT_BUTTON *button, GX_CONST GX_STRING *text);

UINT _gx_text_input_cursor_blink_interval_set(GX_TEXT_INPUT_CURSOR *cursor_input, GX_UBYTE blink_interval);
UINT _gx_text_input_cursor_height_set(GX_TEXT_INPUT_CURSOR *cursor_input, GX_UBYTE height);
UINT _gx_text_input_cursor_width_set(GX_TEXT_INPUT_CURSOR *cursor_input, GX_UBYTE width);

#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gx_text_scroll_wheel_callback_set(GX_TEXT_SCROLL_WHEEL * wheel, GX_CONST GX_CHAR * (*callback)(GX_TEXT_SCROLL_WHEEL *, INT));
#endif
UINT _gx_text_scroll_wheel_callback_set_ext(GX_TEXT_SCROLL_WHEEL *wheel, UINT (*callback)(GX_TEXT_SCROLL_WHEEL *, INT, GX_STRING *));
UINT _gx_text_scroll_wheel_create(GX_TEXT_SCROLL_WHEEL *wheel, GX_CONST GX_CHAR *name, GX_WIDGET *parent, INT total_rows,
                                  ULONG style, USHORT Id, GX_CONST GX_RECTANGLE *size);
VOID _gx_text_scroll_wheel_draw(GX_TEXT_SCROLL_WHEEL *wheel);
UINT _gx_text_scroll_wheel_event_process(GX_TEXT_SCROLL_WHEEL *wheel, GX_EVENT *event_ptr);
UINT _gx_text_scroll_wheel_font_set(GX_TEXT_SCROLL_WHEEL *wheel, GX_RESOURCE_ID normal_font, GX_RESOURCE_ID selected_font);
UINT _gx_text_scroll_wheel_text_color_set(GX_TEXT_SCROLL_WHEEL *wheel, GX_RESOURCE_ID normal_text_color,
                                          GX_RESOURCE_ID selected_text_color, GX_RESOURCE_ID disabled_text_color);

UINT _gx_tree_view_create(GX_TREE_VIEW *tree, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                          ULONG style, USHORT tree_view_id, GX_CONST GX_RECTANGLE *size);
VOID _gx_tree_view_draw(GX_TREE_VIEW *tree);
UINT _gx_tree_view_event_process(GX_TREE_VIEW *tree, GX_EVENT *event_ptr);
UINT _gx_tree_view_indentation_set(GX_TREE_VIEW *tree, GX_VALUE indentation);
UINT _gx_tree_view_position(GX_TREE_VIEW *tree);
UINT _gx_tree_view_root_line_color_set(GX_TREE_VIEW *tree, GX_RESOURCE_ID color);
UINT _gx_tree_view_root_pixelmap_set(GX_TREE_VIEW *tree, GX_RESOURCE_ID expand_map_id, GX_RESOURCE_ID collapse_map_id);
UINT _gx_tree_view_selected_get(GX_TREE_VIEW *tree, GX_WIDGET **selected);
UINT _gx_tree_view_selected_set(GX_TREE_VIEW *tree, GX_WIDGET *selected);

#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
UINT    _gx_utility_bidi_paragraph_reorder(GX_BIDI_TEXT_INFO *input_info, GX_BIDI_RESOLVED_TEXT_INFO **resolved_info_head);
UINT    _gx_utility_bidi_paragraph_reorder_ext(GX_BIDI_TEXT_INFO *input_info, GX_BIDI_RESOLVED_TEXT_INFO **resolved_info_head);
UINT    _gx_utility_bidi_resolved_text_info_delete(GX_BIDI_RESOLVED_TEXT_INFO **resolved_info_head);
#endif
UINT    _gx_utility_canvas_to_bmp(GX_CANVAS *canvas, GX_RECTANGLE *rect, UINT(*write_data)(GX_UBYTE *byte_data, UINT data_count));
UINT    _gx_utility_gradient_create(GX_GRADIENT *gradient, GX_VALUE width, GX_VALUE height, UCHAR type, GX_UBYTE start_alpha, GX_UBYTE end_alpha);
UINT    _gx_utility_gradient_delete(GX_GRADIENT *gradient);

UINT    _gx_utility_circle_point_get(INT xcenter, INT ycenter, UINT r, INT angle, GX_POINT *point);
UINT    _gx_utility_ltoa(LONG value, GX_CHAR *return_buffer, UINT return_buffer_size);
#if defined(GUIX_5_4_0_COMPATIBILITY)
INT     _gx_utility_math_acos_5_4_0(INT x);
INT     _gx_utility_math_asin_5_4_0(INT x);
INT     _gx_utility_math_cos_5_4_0(INT angle);
INT     _gx_utility_math_sin_5_4_0(INT angle);
#else
INT     _gx_utility_math_acos(GX_FIXED_VAL x);
INT     _gx_utility_math_asin(GX_FIXED_VAL x);
GX_FIXED_VAL _gx_utility_math_cos(GX_FIXED_VAL angle);
GX_FIXED_VAL _gx_utility_math_sin(GX_FIXED_VAL angle);
#endif
UINT    _gx_utility_math_sqrt(UINT n);
UINT    _gx_utility_pixelmap_resize(GX_PIXELMAP *src, GX_PIXELMAP *destination, INT width, INT height);
UINT    _gx_utility_pixelmap_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy);
UINT    _gx_utility_pixelmap_simple_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy);
UINT    _gx_utility_rectangle_center(GX_RECTANGLE *rectangle, GX_RECTANGLE *within);
UINT    _gx_utility_rectangle_center_find(GX_RECTANGLE *rectangle, GX_POINT *return_center);
UINT    _gx_utility_rectangle_combine(GX_RECTANGLE *first_rectangle, GX_RECTANGLE *second_rectangle);
GX_BOOL _gx_utility_rectangle_compare(GX_RECTANGLE *first_rectangle, GX_RECTANGLE *second_rectangle);
UINT    _gx_utility_rectangle_define(GX_RECTANGLE *rectangle, GX_VALUE left, GX_VALUE top, GX_VALUE right, GX_VALUE bottom);
GX_BOOL _gx_utility_rectangle_overlap_detect(GX_RECTANGLE *first_rectangle, GX_RECTANGLE *second_rectangle, GX_RECTANGLE *return_overlap_area);
GX_BOOL _gx_utility_rectangle_point_detect(GX_RECTANGLE *rectangle, GX_POINT point);
UINT    _gx_utility_rectangle_resize(GX_RECTANGLE *rectangle, GX_VALUE adjust);
UINT    _gx_utility_rectangle_shift(GX_RECTANGLE *rectangle, GX_VALUE x_shift, GX_VALUE y_shift);

#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT    _gx_utility_string_to_alphamap(GX_CONST GX_CHAR *text, GX_CONST GX_FONT *font, GX_PIXELMAP *textmap);
#endif
UINT    _gx_utility_string_to_alphamap_ext(GX_CONST GX_STRING *text, GX_CONST GX_FONT *font, GX_PIXELMAP *textmap);

UINT _gx_vertical_list_children_position(GX_VERTICAL_LIST *vertical_list);
UINT _gx_vertical_list_create(GX_VERTICAL_LIST *vertical_list, GX_CONST GX_CHAR *name,  GX_WIDGET *parent,
                              INT total_rows,
                              VOID (*callback)(GX_VERTICAL_LIST *, GX_WIDGET *, INT),
                              ULONG style, USHORT vertical_list_id,
                              GX_CONST GX_RECTANGLE *size);
UINT _gx_vertical_list_event_process(GX_VERTICAL_LIST *list, GX_EVENT *event_ptr);
UINT _gx_vertical_list_page_index_set(GX_VERTICAL_LIST *list, INT index);
UINT _gx_vertical_list_selected_index_get(GX_VERTICAL_LIST *vertical_list, INT *return_index);
UINT _gx_vertical_list_selected_set(GX_VERTICAL_LIST *vertical_list, INT index);
UINT _gx_vertical_list_selected_widget_get(GX_VERTICAL_LIST *vertical_list, GX_WIDGET **return_list_entry);
UINT _gx_vertical_list_total_rows_set(GX_VERTICAL_LIST *list, INT count);

UINT _gx_vertical_scrollbar_create(GX_SCROLLBAR *scrollbar, GX_CONST GX_CHAR *name, GX_WINDOW *parent,
                                   GX_SCROLLBAR_APPEARANCE *settings, ULONG style);

UINT _gx_widget_allocate(GX_WIDGET **widget, ULONG memsize);
UINT _gx_widget_attach(GX_WIDGET *parent, GX_WIDGET *widget);
UINT _gx_widget_back_attach(GX_WIDGET *parent, GX_WIDGET *widget);
UINT _gx_widget_back_move(GX_WIDGET *widget, GX_BOOL *widget_moved);
VOID _gx_widget_background_draw(GX_WIDGET *widget);
UINT _gx_widget_block_move(GX_WIDGET *widget, GX_RECTANGLE *block, INT x_shift, INT y_shift);
VOID _gx_widget_border_draw(GX_WIDGET *widget, GX_RESOURCE_ID border_color, GX_RESOURCE_ID upper_color, GX_COLOR lower_color, GX_BOOL fill);
UINT _gx_widget_border_style_set(GX_WIDGET *widget, ULONG Style);
UINT _gx_widget_border_width_get(GX_WIDGET *widget, GX_VALUE *return_width);
UINT _gx_widget_canvas_get(GX_WIDGET *widget, GX_CANVAS **return_canvas);
UINT _gx_widget_child_detect(GX_WIDGET *parent, GX_WIDGET *child, GX_BOOL *return_detect);
VOID _gx_widget_children_draw(GX_WIDGET *widget);
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
VOID _gx_widget_draw(GX_WIDGET *widget);
UINT _gx_widget_draw_set(GX_WIDGET *widget, VOID (*draw_func)(GX_WIDGET *));
UINT _gx_widget_event_generate(GX_WIDGET *widget, USHORT event_type, LONG value);
UINT _gx_widget_event_process(GX_WIDGET *widget, GX_EVENT *event_ptr);
UINT _gx_widget_event_process_set(GX_WIDGET *widget, UINT (*event_processing_function)(GX_WIDGET *, GX_EVENT *));
UINT _gx_widget_event_to_parent(GX_WIDGET *widget, GX_EVENT *event_ptr);
UINT _gx_widget_fill_color_set(GX_WIDGET *widget, GX_RESOURCE_ID normal_color, GX_RESOURCE_ID selected_color, GX_RESOURCE_ID disabled_color);
UINT _gx_widget_find(GX_WIDGET *parent, USHORT widget_id, INT search_depth, GX_WIDGET **return_widget);
UINT _gx_widget_first_child_get(GX_WIDGET *parent, GX_WIDGET **child_return);
UINT _gx_widget_font_get(GX_WIDGET *widget, GX_RESOURCE_ID font_id, GX_FONT **return_font);
UINT _gx_widget_free(GX_WIDGET *widget);
UINT _gx_widget_focus_next(GX_WIDGET *widget);
UINT _gx_widget_focus_previous(GX_WIDGET *widget);
UINT _gx_widget_front_move(GX_WIDGET *widget, GX_BOOL *widget_moved);
UINT _gx_widget_height_get(GX_WIDGET *widget, GX_VALUE *return_height);
UINT _gx_widget_hide(GX_WIDGET *widget);
UINT _gx_widget_last_child_get(GX_WIDGET *parent, GX_WIDGET **child_return);
UINT _gx_widget_next_sibling_get(GX_WIDGET *current, GX_WIDGET **sibling_return);
UINT _gx_widget_parent_get(GX_WIDGET *current, GX_WIDGET **parent_return);
UINT _gx_widget_pixelmap_get(GX_WIDGET *widget, GX_RESOURCE_ID font_id, GX_PIXELMAP **return_pixelmap);
UINT _gx_widget_previous_sibling_get(GX_WIDGET *current, GX_WIDGET **sibling_return);
UINT _gx_widget_resize(GX_WIDGET *widget, GX_RECTANGLE *newsize);
UINT _gx_widget_shift(GX_WIDGET *widget, GX_VALUE x_shift, GX_VALUE y_shift, GX_BOOL mark_dirty);
UINT _gx_widget_show(GX_WIDGET *widget);
UINT _gx_widget_status_add(GX_WIDGET *widget, ULONG status);
UINT _gx_widget_status_get(GX_WIDGET *widget, ULONG *return_status);
UINT _gx_widget_status_remove(GX_WIDGET *widget, ULONG status);
UINT _gx_widget_status_test(GX_WIDGET *widget, ULONG status, GX_BOOL *return_test);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gx_widget_string_get(GX_WIDGET *widget, GX_RESOURCE_ID string_id, GX_CONST GX_CHAR **return_string);
#endif
UINT _gx_widget_string_get_ext(GX_WIDGET *widget, GX_RESOURCE_ID string_id, GX_STRING *return_string);
UINT _gx_widget_style_add(GX_WIDGET *widget, ULONG style);
UINT _gx_widget_style_get(GX_WIDGET *widget, ULONG *return_style);
UINT _gx_widget_style_remove(GX_WIDGET *widget, ULONG style);
UINT _gx_widget_style_set(GX_WIDGET *widget, ULONG style);

#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gx_widget_text_blend(GX_WIDGET *widget, UINT tColor, UINT font_id, GX_CONST GX_CHAR *string,
                           INT x_offset, INT y_offset, UCHAR alpha);

VOID _gx_widget_text_draw(GX_WIDGET *widget, UINT tColor, UINT font_id, GX_CONST GX_CHAR *string,
                          INT x_offset, INT y_offset);
#endif
UINT _gx_widget_text_blend_ext(GX_WIDGET *widget, UINT tColor, UINT font_id, GX_CONST GX_STRING *string,
                               INT x_offset, INT y_offset, UCHAR alpha);
VOID _gx_widget_text_draw_ext(GX_WIDGET *widget, UINT tColor, UINT font_id, GX_CONST GX_STRING *string,
                              INT x_offset, INT y_offset);
VOID _gx_widget_text_id_draw(GX_WIDGET *widget, UINT tColor,
                             UINT font_id, UINT text_id,
                             INT x_offset, INT y_offset);
UINT _gx_widget_top_visible_child_find(GX_WIDGET *parent, GX_WIDGET **child_return);
UINT _gx_widget_type_find(GX_WIDGET *parent, USHORT widget_type, GX_WIDGET **return_widget);
UINT _gx_widget_width_get(GX_WIDGET *widget, GX_VALUE *return_width);

VOID _gx_window_background_draw(GX_WINDOW *window);
UINT _gx_window_client_height_get(GX_WINDOW *window, GX_VALUE *return_height);
UINT _gx_window_client_scroll(GX_WINDOW *window, GX_VALUE x_scroll, GX_VALUE y_scroll);
UINT _gx_window_client_width_get(GX_WINDOW *window, GX_VALUE *return_width);
UINT _gx_window_close(GX_WINDOW *window);
UINT _gx_window_create(GX_WINDOW *window,
                       GX_CONST GX_CHAR *name,
                       GX_WIDGET *parent,
                       ULONG style, USHORT window_id,
                       GX_CONST GX_RECTANGLE *size);
VOID _gx_window_draw(GX_WINDOW *window);
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

#define gx_accordion_menu_create(a, b, c, d, e, f)               _gxe_accordion_menu_create(a, b, c, d, e, f, sizeof(GX_ACCORDION_MENU))
#define gx_accordion_menu_draw                                   _gx_accordion_menu_draw
#define gx_accordion_menu_event_process                          _gxe_accordion_menu_event_process
#define gx_accordion_menu_position                               _gxe_accordion_menu_position

#define gx_animation_canvas_define                               _gxe_animation_canvas_define
#define gx_animation_create                                      _gxe_animation_create
#define gx_animation_delete                                      _gxe_animation_delete
#define gx_animation_drag_disable                                _gxe_animation_drag_disable
#define gx_animation_drag_enable                                 _gxe_animation_drag_enable
#define gx_animation_landing_speed_set                           _gxe_animation_landing_speed_set
#define gx_animation_start                                       _gxe_animation_start
#define gx_animation_stop                                        _gxe_animation_stop

#define gx_binres_language_count_get                             _gxe_binres_language_count_get
#define gx_binres_language_info_load                             _gxe_binres_language_info_load
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define gx_binres_language_table_load                            _gxe_binres_language_table_load
#endif
#define gx_binres_language_table_load_ext                        _gxe_binres_language_table_load_ext
#define gx_binres_theme_load                                     _gxe_binres_theme_load
#define gx_binres_pixelmap_load                                  _gxe_binres_pixelmap_load
#define gx_binres_font_load                                      _gxe_binres_font_load

#define gx_brush_default                                         _gxe_brush_default
#define gx_brush_define                                          _gxe_brush_define

#define gx_button_background_draw                                _gx_button_background_draw
#define gx_button_create(a, b, c, d, e, f)                       _gxe_button_create(a, b, (GX_WIDGET *)c, d, e, f, sizeof(GX_BUTTON))
#define gx_button_deselect                                       _gxe_button_deselect
#define gx_button_draw                                           _gx_button_draw
#define gx_button_event_process                                  _gxe_button_event_process
#define gx_button_select                                         _gxe_button_select

#define gx_canvas_alpha_set                                      _gxe_canvas_alpha_set
#define gx_canvas_arc_draw                                       _gxe_canvas_arc_draw
#define gx_canvas_block_move(a, b, c, d)                         _gxe_canvas_block_move(a, b, c, d)
#define gx_canvas_circle_draw                                    _gxe_canvas_circle_draw
#define gx_canvas_create(a, b, c, d, e, f, g, h)                 _gxe_canvas_create(a, b, c, d, e, f, g, h, sizeof(GX_CANVAS))
#define gx_canvas_delete                                         _gxe_canvas_delete
#define gx_canvas_drawing_complete                               _gxe_canvas_drawing_complete
#define gx_canvas_drawing_initiate(a, b, c)                      _gxe_canvas_drawing_initiate(a, (GX_WIDGET *)b, c)
#define gx_canvas_ellipse_draw                                   _gxe_canvas_ellipse_draw
#define gx_canvas_hardware_layer_bind                            _gxe_canvas_hardware_layer_bind
#define gx_canvas_hide                                           _gxe_canvas_hide
#define gx_canvas_line_draw                                      _gxe_canvas_line_draw
#define gx_canvas_memory_define                                  _gxe_canvas_memory_define

#if defined (GX_MOUSE_SUPPORT)
#define gx_canvas_mouse_define                                   _gxe_canvas_mouse_define
#define gx_canvas_mouse_hide                                     _gxe_canvas_mouse_hide
#define gx_canvas_mouse_show                                     _gxe_canvas_mouse_show
#endif
#define gx_canvas_offset_set                                     _gxe_canvas_offset_set
#define gx_canvas_pie_draw                                       _gxe_canvas_pie_draw
#define gx_canvas_pixel_draw                                     _gxe_canvas_pixel_draw
#define gx_canvas_pixelmap_blend                                 _gxe_canvas_pixelmap_blend
#define gx_canvas_pixelmap_draw                                  _gxe_canvas_pixelmap_draw
#define gx_canvas_pixelmap_get                                   _gxe_canvas_pixelmap_get
#define gx_canvas_pixelmap_rotate                                _gxe_canvas_pixelmap_rotate
#define gx_canvas_pixelmap_tile                                  _gxe_canvas_pixelmap_tile
#define gx_canvas_polygon_draw                                   _gxe_canvas_polygon_draw
#define gx_canvas_rectangle_draw                                 _gxe_canvas_rectangle_draw
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define gx_canvas_rotated_text_draw                              _gxe_canvas_rotated_text_draw
#endif
#define gx_canvas_rotated_text_draw_ext                          _gxe_canvas_rotated_text_draw_ext
#define gx_canvas_shift                                          _gxe_canvas_shift
#define gx_canvas_show                                           _gxe_canvas_show
#define gx_canvas_aligned_text_draw                              _gxe_canvas_aligned_text_draw
#if defined (GX_ENABLE_DEPRECATED_STRING_API)
#define gx_canvas_text_draw                                      _gxe_canvas_text_draw
#endif
#define gx_canvas_text_draw_ext                                  _gxe_canvas_text_draw_ext

#define gx_checkbox_create(a, b, c, d, e, f, g)                  _gxe_checkbox_create(a, b, (GX_WIDGET *)c, d, e, f, g, sizeof(GX_CHECKBOX))
#define gx_checkbox_draw                                         _gx_checkbox_draw
#define gx_checkbox_event_process                                _gxe_checkbox_event_process
#define gx_checkbox_pixelmap_set                                 _gxe_checkbox_pixelmap_set
#define gx_checkbox_select                                       _gxe_checkbox_select

#define gx_circular_gauge_angle_get                              _gxe_circular_gauge_angle_get
#define gx_circular_gauge_angle_set                              _gxe_circular_gauge_angle_set
#define gx_circular_gauge_animation_set                          _gxe_circular_gauge_animation_set
#define gx_circular_gauge_background_draw                        _gx_circular_gauge_background_draw
#define gx_circular_gauge_create(a, b, c, d, e, f, g, h, i)      _gxe_circular_gauge_create((GX_CIRCULAR_GAUGE *) a, b, c, d, e, f, g, h, i, sizeof(GX_CIRCULAR_GAUGE))
#define gx_circular_gauge_draw                                   _gx_circular_gauge_draw
#define gx_circular_gauge_event_process                          _gxe_circular_gauge_event_process

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

#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define gx_context_string_get                                    _gxe_context_string_get
#endif
#define gx_context_string_get_ext                                _gxe_context_string_get_ext

#define gx_display_active_language_set                           _gxe_display_active_language_set
#define gx_display_color_set                                     _gxe_display_color_set
#define gx_display_color_table_set                               _gxe_display_color_table_set
#define gx_display_create(a, b, c, d, e)                         _gxe_display_create(a, b, c, d, e, sizeof(GX_DISPLAY))
#define gx_display_delete                                        _gxe_display_delete
#define gx_display_font_table_set                                _gxe_display_font_table_set
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define gx_display_language_table_get                            _gxe_display_language_table_get
#define gx_display_language_table_set                            _gxe_display_language_table_set
#endif
#define gx_display_language_table_get_ext                        _gxe_display_language_table_get_ext
#define gx_display_language_table_set_ext                        _gxe_display_language_table_set_ext
#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
#define gx_display_language_direction_table_set                  _gxe_display_language_direction_table_set
#endif
#define gx_display_pixelmap_table_set                            _gxe_display_pixelmap_table_set
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define gx_display_string_get                                    _gxe_display_string_get
#define gx_display_string_table_get                              _gxe_display_string_table_get
#endif
#define gx_display_string_get_ext                                _gxe_display_string_get_ext
#define gx_display_string_table_get_ext                          _gxe_display_string_table_get_ext
#define gx_display_theme_install                                 _gxe_display_theme_install

#define gx_drop_list_close                                       _gxe_drop_list_close
#define gx_drop_list_create(a, b, c, d, e, f, g, h, i)           _gxe_drop_list_create((GX_DROP_LIST *)a, b, (GX_WIDGET *)c, d, e, f, g, h, i, sizeof(GX_DROP_LIST))
#define gx_drop_list_event_process                               _gxe_drop_list_event_process
#define gx_drop_list_open                                        _gxe_drop_list_open
#define gx_drop_list_pixelmap_set                                _gxe_drop_list_pixelmap_set
#define gx_drop_list_popup_get                                   _gxe_drop_list_popup_get

#define gx_generic_scroll_wheel_children_position                _gxe_generic_scroll_wheel_children_position
#define gx_generic_scroll_wheel_create(a, b, c, d, e, f, g, h)   _gxe_generic_scroll_wheel_create(a, b, c, d, e, f, g, h, sizeof(GX_GENERIC_SCROLL_WHEEL))
#define gx_generic_scroll_wheel_draw                             _gxe_generic_scroll_wheel_draw
#define gx_generic_scroll_wheel_event_process                    _gxe_generic_scroll_wheel_event_process
#define gx_generic_scroll_wheel_row_height_set                   _gxe_generic_scroll_wheel_row_height_set
#define gx_generic_scroll_wheel_total_rows_set                   _gxe_generic_scroll_wheel_total_rows_set

#define gx_horizontal_list_children_position                     _gxe_horizontal_list_children_position
#define gx_horizontal_list_create(a, b, c, d, e, f, g, h)        _gxe_horizontal_list_create(a, b, (GX_WIDGET *)c, d, e, f, g, h, sizeof(GX_HORIZONTAL_LIST))
#define gx_horizontal_list_event_process                         _gxe_horizontal_list_event_process
#define gx_horizontal_list_page_index_set(a, b)                  _gxe_horizontal_list_page_index_set((GX_HORIZONTAL_LIST *)a, b)
#define gx_horizontal_list_selected_index_get(a, b)              _gxe_horizontal_list_selected_index_get(a, b)
#define gx_horizontal_list_selected_set(a, b)                    _gxe_horizontal_list_selected_set(a, b)
#define gx_horizontal_list_selected_widget_get(a, b)             _gxe_horizontal_list_selected_widget_get(a, (GX_WIDGET **)b)
#define gx_horizontal_list_total_columns_set(a, b)               _gxe_horizontal_list_total_columns_set((GX_HORIZONTAL_LIST *)a, b)

#define gx_horizontal_scrollbar_create(a, b, c, d, e)            _gxe_horizontal_scrollbar_create(a, b, (GX_WINDOW *)c, d, e, sizeof(GX_SCROLLBAR))

#define gx_icon_background_draw                                  _gx_icon_background_draw
#define gx_icon_button_create(a, b, c, d, e, f, g)               _gxe_icon_button_create(a, b, (GX_WIDGET *)c, d, e, f, g, sizeof(GX_ICON_BUTTON))
#define gx_icon_button_draw                                      _gx_icon_button_draw
#define gx_icon_button_pixelmap_set                              _gxe_icon_button_pixelmap_set
#define gx_icon_create(a, b, c, d, e, f, g, h)                   _gxe_icon_create(a, b, (GX_WIDGET *)c, d, e, f, g, h, sizeof(GX_ICON))
#define gx_icon_draw                                             _gx_icon_draw
#define gx_icon_event_process                                    _gxe_icon_event_process
#define gx_icon_pixelmap_set                                     _gxe_icon_pixelmap_set

#define gx_image_reader_create                                   _gxe_image_reader_create
#define gx_image_reader_palette_set                              _gxe_image_reader_palette_set
#define gx_image_reader_start                                    _gxe_image_reader_start

#define gx_line_chart_axis_draw                                  _gx_line_chart_axis_draw
#define gx_line_chart_create(a, b, c, d, e, f, g)                _gxe_line_chart_create(a, b, (GX_WIDGET *) c, d, e, f, g, sizeof(GX_LINE_CHART))
#define gx_line_chart_data_draw                                  _gx_line_chart_data_draw
#define gx_line_chart_draw                                       _gx_line_chart_draw
#define gx_line_chart_update                                     _gxe_line_chart_update
#define gx_line_chart_y_scale_calculate                          _gxe_line_chart_y_scale_calculate

#define gx_menu_create(a, b, c, d, e, f, g, h)                   _gxe_menu_create((GX_MENU *)a, b, (GX_WIDGET *)c, d, e, f, g, h, sizeof(GX_MENU))
#define gx_menu_draw                                             _gx_menu_draw
#define gx_menu_event_process                                    _gxe_menu_event_process
#define gx_menu_insert                                           _gxe_menu_insert
#define gx_menu_remove                                           _gxe_menu_remove
#define gx_menu_text_draw                                        _gx_menu_text_draw
#define gx_menu_text_offset_set                                  _gxe_menu_text_offset_set

#define gx_multi_line_text_button_create(a, b, c, d, e, f, g)    _gxe_multi_line_text_button_create(a, b, (GX_WIDGET *)c, d, e, f, g, sizeof(GX_MULTI_LINE_TEXT_BUTTON))
#define gx_multi_line_text_button_draw                           _gx_multi_line_text_button_draw
#define gx_multi_line_text_button_event_process                  _gxe_multi_line_text_button_event_process
#define gx_multi_line_text_button_text_draw                      _gx_multi_line_text_button_text_draw
#define gx_multi_line_text_button_text_id_set                    _gxe_multi_line_text_button_text_id_set
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define gx_multi_line_text_button_text_set                       _gxe_multi_line_text_button_text_set
#endif
#define gx_multi_line_text_button_text_set_ext                   _gxe_multi_line_text_button_text_set_ext

#define gx_multi_line_text_input_backspace                       _gxe_multi_line_text_input_backspace
#define gx_multi_line_text_input_buffer_clear                    _gxe_multi_line_text_input_buffer_clear
#define gx_multi_line_text_input_buffer_get                      _gxe_multi_line_text_input_buffer_get
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define gx_multi_line_text_input_char_insert                     _gxe_multi_line_text_input_char_insert
#endif
#define gx_multi_line_text_input_char_insert_ext                 _gxe_multi_line_text_input_char_insert_ext
#define gx_multi_line_text_input_create(a, b, c, d, e, f, g, h)  _gxe_multi_line_text_input_create(a, b, (GX_WIDGET *)c, d, e, f, g, h, sizeof(GX_MULTI_LINE_TEXT_INPUT))
#define gx_multi_line_text_input_cursor_pos_get                  _gxe_multi_line_text_input_cursor_pos_get
#define gx_multi_line_text_input_delete                          _gxe_multi_line_text_input_delete
#define gx_multi_line_text_input_down_arrow                      _gxe_multi_line_text_input_down_arrow
#define gx_multi_line_text_input_end                             _gxe_multi_line_text_input_end
#define gx_multi_line_text_input_event_process                   _gxe_multi_line_text_input_event_process
#define gx_multi_line_text_input_fill_color_set                  _gxe_multi_line_text_input_fill_color_set
#define gx_multi_line_text_input_home                            _gxe_multi_line_text_input_home
#define gx_multi_line_text_input_left_arrow                      _gxe_multi_line_text_input_left_arrow
#define gx_multi_line_text_input_right_arrow                     _gxe_multi_line_text_input_right_arrow
#define gx_multi_line_text_input_style_add                       _gxe_multi_line_text_input_style_add
#define gx_multi_line_text_input_style_remove                    _gxe_multi_line_text_input_style_remove
#define gx_multi_line_text_input_style_set                       _gxe_multi_line_text_input_style_set
#define gx_multi_line_text_input_text_color_set                  _gxe_multi_line_text_input_text_color_set
#define gx_multi_line_text_input_text_select                     _gxe_multi_line_text_input_text_select
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define gx_multi_line_text_input_text_set                        _gxe_multi_line_text_input_text_set
#endif
#define gx_multi_line_text_input_text_set_ext                    _gxe_multi_line_text_input_text_set_ext
#define gx_multi_line_text_input_up_arrow                        _gxe_multi_line_text_input_up_arrow

#define gx_multi_line_text_view_create(a, b, c, d, e, f, g)      _gxe_multi_line_text_view_create(a, b, (GX_WIDGET *)c, d, e, f, g, sizeof(GX_MULTI_LINE_TEXT_VIEW))
#define gx_multi_line_text_view_draw                             _gx_multi_line_text_view_draw
#define gx_multi_line_text_view_event_process                    _gxe_multi_line_text_view_event_process
#define gx_multi_line_text_view_font_set                         _gxe_multi_line_text_view_font_set
#define gx_multi_line_text_view_line_space_set                   _gxe_multi_line_text_view_line_space_set
#define gx_multi_line_text_view_scroll_info_get                  _gxe_multi_line_text_view_scroll_info_get
#if defined(GUIX_5_4_0_COMPATIBILITY)
#define gx_multi_line_text_view_text_color_set(a, b, c)          _gxe_multi_line_text_view_text_color_set((GX_MULTI_LINE_TEXT_VIEW *)a, b, c, b)
#else
#define gx_multi_line_text_view_text_color_set                   _gxe_multi_line_text_view_text_color_set
#endif
#define gx_multi_line_text_view_text_id_set                      _gxe_multi_line_text_view_text_id_set
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define gx_multi_line_text_view_text_set                         _gxe_multi_line_text_view_text_set
#endif
#define gx_multi_line_text_view_text_set_ext                     _gxe_multi_line_text_view_text_set_ext
#define gx_multi_line_text_view_whitespace_set                   _gxe_multi_line_text_view_whitespace_set

#define gx_rich_text_view_create(a, b, c, d, e, f, g, h)         _gxe_rich_text_view_create(a, b, (GX_WIDGET *)c, d, e, f, g, h, sizeof(GX_RICH_TEXT_VIEW))
#define gx_rich_text_view_draw                                   _gx_rich_text_view_draw
#define gx_rich_text_view_text_draw                              _gx_rich_text_view_text_draw
#define gx_rich_text_view_fonts_set                              _gxe_rich_text_view_fonts_set

#define gx_numeric_pixelmap_prompt_create(a, b, c, d, e, f, g, h) _gxe_numeric_pixelmap_prompt_create(a, b, (GX_WIDGET *)c, d, e, f, g, h, sizeof(GX_NUMERIC_PIXELMAP_PROMPT))
#define gx_numeric_pixelmap_prompt_format_function_set           _gxe_numeric_pixelmap_prompt_format_function_set
#define gx_numeric_pixelmap_prompt_value_set                     _gxe_numeric_pixelmap_prompt_value_set

#define gx_numeric_prompt_create(a, b, c, d, e, f, g)            _gxe_numeric_prompt_create(a, b, (GX_WIDGET *)c, d, e, f, g, sizeof(GX_NUMERIC_PROMPT))
#define gx_numeric_prompt_format_function_set                    _gxe_numeric_prompt_format_function_set
#define gx_numeric_prompt_value_set                              _gxe_numeric_prompt_value_set

#define gx_numeric_scroll_wheel_create(a, b, c, d, e, f, g, h)   _gxe_numeric_scroll_wheel_create(a, b, (GX_WIDGET *) c, d, e, f, g, h, sizeof(GX_NUMERIC_SCROLL_WHEEL))
#define gx_numeric_scroll_wheel_range_set                        _gxe_numeric_scroll_wheel_range_set

#define gx_pixelmap_button_create(a, b, c, d, e, f, g, h, i)     _gxe_pixelmap_button_create(a, b, (GX_WIDGET *)c, d, e, f, g, h, i, sizeof(GX_PIXELMAP_BUTTON))
#define gx_pixelmap_button_draw                                  _gx_pixelmap_button_draw
#define gx_pixelmap_button_event_process                         _gxe_pixelmap_button_event_process
#define gx_pixelmap_button_pixelmap_set                          _gxe_pixelmap_button_pixelmap_set

#define gx_pixelmap_prompt_create(a, b, c, d, e, f, g, h)        _gxe_pixelmap_prompt_create(a, b, (GX_WIDGET *)c, d, e, f, g, h, sizeof(GX_PIXELMAP_PROMPT))
#define gx_pixelmap_prompt_draw                                  _gx_pixelmap_prompt_draw
#define gx_pixelmap_prompt_pixelmap_set                          _gxe_pixelmap_prompt_pixelmap_set

#define gx_pixelmap_slider_create(a, b, c, d, e, f, g, h)        _gxe_pixelmap_slider_create(a, b, (GX_WIDGET *)c, d, e, f, g, h, sizeof(GX_PIXELMAP_SLIDER))
#define gx_pixelmap_slider_draw                                  _gx_pixelmap_slider_draw
#define gx_pixelmap_slider_event_process                         _gxe_pixelmap_slider_event_process
#define gx_pixelmap_slider_pixelmap_set                          _gxe_pixelmap_slider_pixelmap_set

#define gx_progress_bar_background_draw                          _gx_progress_bar_background_draw
#define gx_progress_bar_create(a, b, c, d, e, f, g)              _gxe_progress_bar_create(a, b, (GX_WIDGET *)c, d, e, f, g, sizeof(GX_PROGRESS_BAR))
#define gx_progress_bar_draw                                     _gx_progress_bar_draw
#define gx_progress_bar_event_process                            _gxe_progress_bar_event_process
#define gx_progress_bar_font_set                                 _gxe_progress_bar_font_set
#define gx_progress_bar_info_set(a, b)                           _gxe_progress_bar_info_set((GX_PROGRESS_BAR *)a, b);
#define gx_progress_bar_pixelmap_set                             _gxe_progress_bar_pixelmap_set
#define gx_progress_bar_range_set                                _gxe_progress_bar_range_set
#if defined(GUIX_5_4_0_COMPATIBILITY)
#define gx_progress_bar_text_color_set(a, b, c)                  _gxe_progress_bar_text_color_set((GX_PROGRESS_BAR *)a, b, c, b)
#else
#define gx_progress_bar_text_color_set                           _gxe_progress_bar_text_color_set
#endif
#define gx_progress_bar_text_draw                                _gx_progress_bar_text_draw
#define gx_progress_bar_value_set                                _gxe_progress_bar_value_set

#define gx_prompt_create(a, b, c, d, e, f, g)                    _gxe_prompt_create(a, b, (GX_WIDGET *)c, d, e, f, g, sizeof(GX_PROMPT))
#define gx_prompt_event_process                                  _gxe_prompt_event_process
#define gx_prompt_draw                                           _gx_prompt_draw
#define gx_prompt_font_set                                       _gxe_prompt_font_set
#if defined(GUIX_5_4_0_COMPATIBILITY)
#define gx_prompt_text_color_set(a, b, c)                        _gxe_prompt_text_color_set((GX_PROMPT *)a, b, c, b)
#else
#define gx_prompt_text_color_set                                 _gxe_prompt_text_color_set
#endif
#define gx_prompt_text_draw                                      _gx_prompt_text_draw
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define gx_prompt_text_get                                       _gxe_prompt_text_get
#endif
#define gx_prompt_text_get_ext                                   _gxe_prompt_text_get_ext
#define gx_prompt_text_id_set                                    _gxe_prompt_text_id_set
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define gx_prompt_text_set                                       _gxe_prompt_text_set
#endif
#define gx_prompt_text_set_ext                                   _gxe_prompt_text_set_ext

#define gx_radial_progress_bar_anchor_set                        _gxe_radial_progress_bar_anchor_set
#define gx_radial_progress_bar_background_draw                   _gx_radial_progress_bar_background_draw
#define gx_radial_progress_bar_create(a, b, c, d, e, f)          _gxe_radial_progress_bar_create(a, b, c, d, e, f, sizeof(GX_RADIAL_PROGRESS_BAR))
#define gx_radial_progress_bar_draw                              _gx_radial_progress_bar_draw
#define gx_radial_progress_bar_event_process                     _gxe_radial_progress_bar_event_process
#define gx_radial_progress_bar_font_set                          _gxe_radial_progress_bar_font_set
#define gx_radial_progress_bar_info_set                          _gxe_radial_progress_bar_info_set
#if defined(GUIX_5_4_0_COMPATIBILITY)
#define gx_radial_progress_bar_text_color_set(a, b, c)           _gxe_radial_progress_bar_text_color_set((GX_RADIAL_PROGRESS_BAR *)a, b, c, b)
#else
#define gx_radial_progress_bar_text_color_set                    _gxe_radial_progress_bar_text_color_set
#endif
#define gx_radial_progress_bar_text_draw                         _gx_radial_progress_bar_text_draw
#define gx_radial_progress_bar_value_set                         _gxe_radial_progress_bar_value_set

#define gx_radial_slider_anchor_angles_set                       _gxe_radial_slider_anchor_angles_set
#define gx_radial_slider_animation_set                           _gxe_radial_slider_animation_set
#define gx_radial_slider_animation_start                         _gxe_radial_slider_animation_start
#define gx_radial_slider_create(a, b, c, d, e, f, g)             _gxe_radial_slider_create((GX_RADIAL_SLIDER *)a, b, c, d, e, f, g, sizeof(GX_RADIAL_SLIDER))
#define gx_radial_slider_draw                                    _gx_radial_slider_draw
#define gx_radial_slider_event_process                           _gxe_radial_slider_event_process
#define gx_radial_slider_info_get                                _gxe_radial_slider_info_get
#define gx_radial_slider_info_set                                _gxe_radial_slider_info_set
#define gx_radial_slider_pixelmap_set                            _gxe_radial_slider_pixelmap_set
#define gx_radial_slider_angle_set                               _gxe_radial_slider_angle_set

#define gx_radio_button_create(a, b, c, d, e, f, g)              _gxe_radio_button_create(a, b, (GX_WIDGET *)c, d, e, f, g, sizeof(GX_RADIO_BUTTON))
#define gx_radio_button_draw                                     _gx_radio_button_draw
#define gx_radio_button_pixelmap_set                             _gxe_radio_button_pixelmap_set

#define gx_screen_stack_create                                   _gxe_screen_stack_create
#define gx_screen_stack_pop                                      _gxe_screen_stack_pop
#define gx_screen_stack_push                                     _gxe_screen_stack_push
#define gx_screen_stack_reset                                    _gxe_screen_stack_reset

#define gx_scroll_thumb_create(a, b, c)                          _gxe_scroll_thumb_create(a, b, c, sizeof(GX_SCROLL_THUMB))
#define gx_scroll_thumb_draw                                     _gx_scroll_thumb_draw
#define gx_scroll_thumb_event_process                            _gxe_scroll_thumb_event_process

#define gx_scrollbar_draw                                        _gx_scrollbar_draw
#define gx_scrollbar_event_process                               _gxe_scrollbar_event_process
#define gx_scrollbar_limit_check                                 _gxe_scrollbar_limit_check
#define gx_scrollbar_reset                                       _gxe_scrollbar_reset
#define gx_scrollbar_value_set                                   _gxe_scrollbar_value_set

#define gx_scroll_wheel_create(a, b, c, d, e, f, g)              _gxe_scroll_wheel_create(a, b, c, d, e, f, g, sizeof(GX_SCROLL_WHEEL))
#define gx_scroll_wheel_event_process                            _gxe_scroll_wheel_event_process
#define gx_scroll_wheel_gradient_alpha_set(a, b, c)              _gxe_scroll_wheel_gradient_alpha_set((GX_SCROLL_WHEEL *) a, b, c)
#define gx_scroll_wheel_row_height_set(a, b)                     _gxe_scroll_wheel_row_height_set((GX_SCROLL_WHEEL *) a, b)
#define gx_scroll_wheel_selected_background_set(a, b)            _gxe_scroll_wheel_selected_background_set((GX_SCROLL_WHEEL *) a, b)
#define gx_scroll_wheel_selected_get(a, b)                       _gxe_scroll_wheel_selected_get((GX_SCROLL_WHEEL *) a, b)
#define gx_scroll_wheel_selected_set(a, b)                       _gxe_scroll_wheel_selected_set((GX_SCROLL_WHEEL *) a, b)
#define gx_scroll_wheel_speed_set(a, b, c, d, e)                 _gxe_scroll_wheel_speed_set((GX_SCROLL_WHEEL *) a, b, c, d, e)
#define gx_scroll_wheel_total_rows_set(a, b)                     _gxe_scroll_wheel_total_rows_set((GX_SCROLL_WHEEL *) a, b)

#define gx_single_line_text_input_backspace(a)                   _gxe_single_line_text_input_backspace((GX_SINGLE_LINE_TEXT_INPUT *)a)
#define gx_single_line_text_input_buffer_clear(a)                _gxe_single_line_text_input_buffer_clear((GX_SINGLE_LINE_TEXT_INPUT *)a)
#define gx_single_line_text_input_buffer_get(a, b, c, d)         _gxe_single_line_text_input_buffer_get((GX_SINGLE_LINE_TEXT_INPUT *)a, b, c, d)
#define gx_single_line_text_input_character_delete(a)            _gxe_single_line_text_input_character_delete(a)
#define gx_single_line_text_input_character_insert(a, b, c)      _gxe_single_line_text_input_character_insert(a, b, c)
#define gx_single_line_text_input_create(a, b, c, d, e, f, g, h) _gxe_single_line_text_input_create(a, b, (GX_WIDGET *)c, d, e, f, g, h, sizeof(GX_SINGLE_LINE_TEXT_INPUT))
#define gx_single_line_text_input_draw(a)                        _gx_single_line_text_input_draw(a)
#define gx_single_line_text_input_draw_position_get(a, b, c)     _gxe_single_line_text_input_draw_position_get(a, b, c)
#define gx_single_line_text_input_end(a)                         _gxe_single_line_text_input_end((GX_SINGLE_LINE_TEXT_INPUT *)a)
#define gx_single_line_text_input_event_process(a, b)            _gxe_single_line_text_input_event_process((GX_SINGLE_LINE_TEXT_INPUT *)a, b)
#define gx_single_line_text_input_fill_color_set(a, b, c, d, e)  _gxe_single_line_text_input_fill_color_set((GX_SINGLE_LINE_TEXT_INPUT *)a, b, c, d, e)
#define gx_single_line_text_input_home(a)                        _gxe_single_line_text_input_home((GX_SINGLE_LINE_TEXT_INPUT *)a)
#define gx_single_line_text_input_left_arrow(a)                  _gxe_single_line_text_input_left_arrow((GX_SINGLE_LINE_TEXT_INPUT *)a)
#define gx_single_line_text_input_position_get(a, b)             _gxe_single_line_text_input_position_get(a, b)
#define gx_single_line_text_input_right_arrow(a)                 _gxe_single_line_text_input_right_arrow((GX_SINGLE_LINE_TEXT_INPUT *)a)
#define gx_single_line_text_input_style_add(a, b)                _gxe_single_line_text_input_style_add((GX_SINGLE_LINE_TEXT_INPUT *)a, b)
#define gx_single_line_text_input_style_remove(a, b)             _gxe_single_line_text_input_style_remove((GX_SINGLE_LINE_TEXT_INPUT *)a, b)
#define gx_single_line_text_input_style_set(a, b)                _gxe_single_line_text_input_style_set((GX_SINGLE_LINE_TEXT_INPUT *)a, b)
#define gx_single_line_text_input_text_color_set(a, b, c, d, e)  _gxe_single_line_text_input_text_color_set((GX_SINGLE_LINE_TEXT_INPUT *)a, b, c, d, e)
#define gx_single_line_text_input_text_select                    _gxe_single_line_text_input_text_select
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define gx_single_line_text_input_text_set                       _gxe_single_line_text_input_text_set
#endif
#define gx_single_line_text_input_text_set_ext                   _gxe_single_line_text_input_text_set_ext

#define gx_slider_create(a, b, c, d, e, f, g, h)                 _gxe_slider_create(a, b, (GX_WIDGET *)c, d, e, f, g, h, sizeof(GX_SLIDER))
#define gx_slider_draw                                           _gx_slider_draw
#define gx_slider_event_process                                  _gxe_slider_event_process
#define gx_slider_info_set                                       _gxe_slider_info_set
#define gx_slider_needle_draw                                    _gx_slider_needle_draw
#define gx_slider_needle_position_get                            _gxe_slider_needle_position_get
#define gx_slider_tickmarks_draw                                 _gx_slider_tickmarks_draw
#define gx_slider_travel_get                                     _gxe_slider_travel_get
#define gx_slider_value_calculate                                _gxe_slider_value_calculate
#define gx_slider_value_set                                      _gxe_slider_value_set

#define gx_sprite_create(a, b, c, d, e, f, g, h)                 _gxe_sprite_create(a, b, (GX_WIDGET *)c, d, e, f, g, h, sizeof(GX_SPRITE))
#define gx_sprite_current_frame_set                              _gxe_sprite_current_frame_set
#define gx_sprite_frame_list_set                                 _gxe_sprite_frame_list_set
#define gx_sprite_start(a, b)                                    _gxe_sprite_start(a, b)
#define gx_sprite_stop(a)                                        _gxe_sprite_stop(a)

#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define gx_string_scroll_wheel_create(a, b, c, d, e, f, g, h)    _gxe_string_scroll_wheel_create(a, b, c, d, e, f, g, h, sizeof(GX_STRING_SCROLL_WHEEL))
#define gx_string_scroll_wheel_string_list_set                   _gxe_string_scroll_wheel_string_list_set
#endif
#define gx_string_scroll_wheel_create_ext(a, b, c, d, e, f, g, h) _gxe_string_scroll_wheel_create_ext(a, b, c, d, e, f, g, h, sizeof(GX_STRING_SCROLL_WHEEL))
#define gx_string_scroll_wheel_event_process                     _gxe_string_scroll_wheel_event_process
#define gx_string_scroll_wheel_string_id_list_set                _gxe_string_scroll_wheel_string_id_list_set
#define gx_string_scroll_wheel_string_list_set_ext               _gxe_string_scroll_wheel_string_list_set_ext

#define gx_system_active_language_set                            _gxe_system_active_language_set

#if (GX_ANIMATION_POOL_SIZE > 0)
#define gx_system_animation_get                                  _gxe_system_animation_get
#define gx_system_animation_free                                 _gxe_system_animation_free
#endif

#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
#define gx_system_bidi_text_enable                               _gx_system_bidi_text_enable
#define gx_system_bidi_text_disable                              _gx_system_bidi_text_disable
#endif

#if defined(GX_THAI_GLYPH_SHAPING_SUPPORT)
#define gx_system_text_render_style_set                          _gx_system_text_render_style_set
#endif

#define gx_system_canvas_refresh                                 _gxe_system_canvas_refresh
#define gx_system_dirty_mark(a)                                  _gxe_system_dirty_mark((GX_WIDGET *)a);
#define gx_system_dirty_partial_add(a, b)                        _gxe_system_dirty_partial_add((GX_WIDGET *)a, b)
#define gx_system_draw_context_get                               _gxe_system_draw_context_get
#define gx_system_event_fold                                     _gxe_system_event_fold
#define gx_system_event_send                                     _gxe_system_event_send
#define gx_system_focus_claim(a)                                 _gxe_system_focus_claim((GX_WIDGET *)a)
#define gx_system_initialize                                     _gxe_system_initialize
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define gx_system_language_table_get                             _gxe_system_language_table_get
#define gx_system_language_table_set                             _gxe_system_language_table_set
#endif
#define gx_system_memory_allocator_set                           _gxe_system_memory_allocator_set
#define gx_system_pen_configure                                  _gxe_system_pen_configure
#define gx_system_screen_stack_create(a, b)                      _gxe_system_screen_stack_create(a, b, sizeof(GX_SCREEN_STACK_CONTROL))
#define gx_system_screen_stack_get                               _gxe_system_screen_stack_get
#define gx_system_screen_stack_pop                               _gxe_system_screen_stack_pop
#define gx_system_screen_stack_push                              _gxe_system_screen_stack_push
#define gx_system_screen_stack_reset                             _gx_system_screen_stack_reset
#define gx_system_scroll_appearance_get                          _gxe_system_scroll_appearance_get
#define gx_system_scroll_appearance_set                          _gxe_system_scroll_appearance_set
#define gx_system_start                                          _gxe_system_start
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define gx_system_string_get                                     _gxe_system_string_get
#define gx_system_string_table_get                               _gxe_system_string_table_get
#define gx_system_string_width_get                               _gxe_system_string_width_get
#endif
#define gx_system_string_width_get_ext                           _gxe_system_string_width_get_ext
#define gx_system_timer_start(a, b, c, d)                        _gxe_system_timer_start((GX_WIDGET *)a, b, c, d)
#define gx_system_timer_stop(a, b)                               _gxe_system_timer_stop((GX_WIDGET *)a, b)
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define gx_system_version_string_get                             _gxe_system_version_string_get
#endif
#define gx_system_version_string_get_ext                         _gxe_system_version_string_get_ext
#define gx_system_widget_find                                    _gxe_system_widget_find

#define gx_text_button_create(a, b, c, d, e, f, g)               _gxe_text_button_create(a, b, (GX_WIDGET *)c, d, e, f, g, sizeof(GX_TEXT_BUTTON))
#define gx_text_button_draw                                      _gx_text_button_draw
#define gx_text_button_event_process                             _gxe_text_button_event_process
#define gx_text_button_font_set                                  _gxe_text_button_font_set
#if defined(GUIX_5_4_0_COMPATIBILITY)
#define gx_text_button_text_color_set(a, b, c)                   _gxe_text_button_text_color_set((GX_TEXT_BUTTON *)a, b, c, b)
#else
#define gx_text_button_text_color_set                            _gxe_text_button_text_color_set
#endif
#define gx_text_button_text_draw                                 _gx_text_button_text_draw
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define gx_text_button_text_get                                  _gxe_text_button_text_get
#endif
#define gx_text_button_text_get_ext                              _gxe_text_button_text_get_ext
#define gx_text_button_text_id_set                               _gxe_text_button_text_id_set
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define gx_text_button_text_set                                  _gxe_text_button_text_set
#endif
#define gx_text_button_text_set_ext                              _gxe_text_button_text_set_ext

#define gx_text_input_cursor_blink_interval_set(a, b)            _gxe_text_input_cursor_blink_interval_set((GX_TEXT_INPUT_CURSOR *)a, b)
#define gx_text_input_cursor_height_set(a, b)                    _gxe_text_input_cursor_height_set((GX_TEXT_INPUT_CURSOR *)a, b)
#define gx_text_input_cursor_width_set(a, b)                     _gxe_text_input_cursor_width_set((GX_TEXT_INPUT_CURSOR *)a, b)

#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define gx_text_scroll_wheel_callback_set(a, b)                  _gxe_text_scroll_wheel_callback_set((GX_TEXT_SCROLL_WHEEL *)a, (GX_CONST GX_CHAR *(*)(GX_TEXT_SCROLL_WHEEL *, INT)) b)
#endif
#define gx_text_scroll_wheel_callback_set_ext(a, b)              _gxe_text_scroll_wheel_callback_set_ext((GX_TEXT_SCROLL_WHEEL *)a, (UINT(*)(GX_TEXT_SCROLL_WHEEL *, INT, GX_STRING *)) b)
#define gx_text_scroll_wheel_font_set(a, b, c)                   _gxe_text_scroll_wheel_font_set((GX_TEXT_SCROLL_WHEEL *) a, b, c)
#if defined(GUIX_5_4_0_COMPATIBILITY)
#define gx_text_scroll_wheel_text_color_set(a, b, c)             _gxe_text_scroll_wheel_text_color_set((GX_TEXT_SCROLL_WHEEL *) a, b, c, b)
#else
#define gx_text_scroll_wheel_text_color_set(a, b, c, d)          _gxe_text_scroll_wheel_text_color_set((GX_TEXT_SCROLL_WHEEL *) a, b, c, d)
#endif
#define gx_text_scroll_wheel_create(a, b, c, d, e, f, g)         _gxe_text_scroll_wheel_create(a, b, c, d, e, f, g, sizeof(GX_TEXT_SCROLL_WHEEL))
#define gx_text_scroll_wheel_draw                                _gx_text_scroll_wheel_draw
#define gx_text_scroll_wheel_event_process                       _gxe_text_scroll_wheel_event_process

#define gx_utility_gradient_create                               _gxe_utility_gradient_create
#define gx_utility_gradient_delete                               _gxe_utility_gradient_delete
#define gx_tree_view_create(a, b, c, d, e, f)                    _gxe_tree_view_create((GX_TREE_VIEW *)a, b, (GX_WIDGET *)c, d, e, f, sizeof(GX_TREE_VIEW))
#define gx_tree_view_draw                                        _gx_tree_view_draw
#define gx_tree_view_event_process                               _gxe_tree_view_event_process
#define gx_tree_view_indentation_set                             _gxe_tree_view_indentation_set
#define gx_tree_view_position                                    _gxe_tree_view_position
#define gx_tree_view_root_line_color_set                         _gxe_tree_view_root_line_color_set
#define gx_tree_view_root_pixelmap_set                           _gxe_tree_view_root_pixelmap_set
#define gx_tree_view_selected_get                                _gxe_tree_view_selected_get
#define gx_tree_view_selected_set                                _gxe_tree_view_selected_set

#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
#define gx_utility_bidi_paragraph_reorder                        _gxe_utility_bidi_paragraph_reorder
#define gx_utility_bidi_paragraph_reorder_ext                    _gxe_utility_bidi_paragraph_reorder_ext
#define gx_utility_bidi_resolved_text_info_delete                _gxe_utility_bidi_resolved_text_info_delete
#endif
#define gx_utility_canvas_to_bmp                                 _gxe_utility_canvas_to_bmp
#define gx_utility_circle_point_get                              _gxe_utility_circle_point_get
#define gx_utility_ltoa                                          _gxe_utility_ltoa
#if defined(GUIX_5_4_0_COMPATIBILITY)
#define gx_utility_math_acos                                     _gx_utility_math_acos_5_4_0
#define gx_utility_math_asin                                     _gx_utility_math_asin_5_4_0
#define gx_utility_math_cos                                      _gx_utility_math_cos_5_4_0
#define gx_utility_math_sin                                      _gx_utility_math_sin_5_4_0
#else
#define gx_utility_math_acos                                     _gx_utility_math_acos
#define gx_utility_math_asin                                     _gx_utility_math_asin
#define gx_utility_math_cos                                      _gx_utility_math_cos
#define gx_utility_math_sin                                      _gx_utility_math_sin
#endif
#define gx_utility_math_sqrt                                     _gx_utility_math_sqrt
#define gx_utility_pixelmap_resize                               _gxe_utility_pixelmap_resize
#define gx_utility_pixelmap_rotate                               _gxe_utility_pixelmap_rotate
#define gx_utility_pixelmap_simple_rotate                        _gxe_utility_pixelmap_simple_rotate
#define gx_utility_rectangle_center                              _gxe_utility_rectangle_center
#define gx_utility_rectangle_center_find                         _gxe_utility_rectangle_center_find
#define gx_utility_rectangle_combine                             _gxe_utility_rectangle_combine
#define gx_utility_rectangle_compare                             _gxe_utility_rectangle_compare
#define gx_utility_rectangle_define                              _gxe_utility_rectangle_define
#define gx_utility_rectangle_overlap_detect                      _gxe_utility_rectangle_overlap_detect
#define gx_utility_rectangle_point_detect                        _gxe_utility_rectangle_point_detect
#define gx_utility_rectangle_resize                              _gxe_utility_rectangle_resize
#define gx_utility_rectangle_shift                               _gxe_utility_rectangle_shift

#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define gx_utility_string_to_alphamap                            _gxe_utility_string_to_alphamap
#endif
#define gx_utility_string_to_alphamap_ext                        _gxe_utility_string_to_alphamap_ext

#define gx_vertical_list_children_position                       _gxe_vertical_list_children_position
#define gx_vertical_list_create(a, b, c, d, e, f, g, h)          _gxe_vertical_list_create(a, b, (GX_WIDGET *)c, d, e, f, g, h, sizeof(GX_VERTICAL_LIST))
#define gx_vertical_list_event_process                           _gxe_vertical_list_event_process
#define gx_vertical_list_page_index_set(a, b)                    _gxe_vertical_list_page_index_set((GX_VERTICAL_LIST *)a, b)
#define gx_vertical_list_selected_index_get(a, b)                _gxe_vertical_list_selected_index_get(a, b)
#define gx_vertical_list_selected_set(a, b)                      _gxe_vertical_list_selected_set(a, b)
#define gx_vertical_list_selected_widget_get(a, b)               _gxe_vertical_list_selected_widget_get(a, (GX_WIDGET **)b)
#define gx_vertical_list_total_rows_set(a, b)                    _gxe_vertical_list_total_rows_set((GX_VERTICAL_LIST *)a, b)

#define gx_vertical_scrollbar_create(a, b, c, d, e)              _gxe_vertical_scrollbar_create(a, b, (GX_WINDOW *)c, d, e, sizeof(GX_SCROLLBAR))

#define gx_widget_allocate(a, b)                                 _gxe_widget_allocate((GX_WIDGET **)a, b)
#define gx_widget_attach(a, b)                                   _gxe_widget_attach((GX_WIDGET *)a, (GX_WIDGET *)b)
#define gx_widget_back_attach(a, b)                              _gxe_widget_back_attach((GX_WIDGET *)a, (GX_WIDGET *)b)
#define gx_widget_back_move(a, b)                                _gxe_widget_back_move((GX_WIDGET *)a, b)
#define gx_widget_background_draw(a)                             _gx_widget_background_draw((GX_WIDGET *)a)
#define gx_widget_block_move(a, b, c, d)                         _gxe_widget_block_move((GX_WIDGET *)a, b, c, d)
#define gx_widget_border_draw(a, b, c, d, e)                     _gx_widget_border_draw((GX_WIDGET *)a, b, c, d, e)
#define gx_widget_border_style_set(a, b)                         _gxe_widget_border_style_set((GX_WIDGET *)a, b)
#define gx_widget_border_width_get(a, b)                         _gxe_widget_border_width_get((GX_WIDGET *)a, b)
#define gx_widget_canvas_get(a, b)                               _gxe_widget_canvas_get((GX_WIDGET *)a, b)
#define gx_widget_child_detect(a, b, c)                          _gxe_widget_child_detect((GX_WIDGET *)a, b, c)
#define gx_widget_children_draw(a)                               _gx_widget_children_draw((GX_WIDGET *)a)
#define gx_widget_color_get(a, b, c)                             _gxe_widget_color_get((GX_WIDGET *)a, b, c)
#define gx_widget_client_get(a, b, c)                            _gxe_widget_client_get((GX_WIDGET *)a, b, c)
#define gx_widget_create(a, b, c, d, e, f)                       _gxe_widget_create((GX_WIDGET *)a, b, (GX_WIDGET *)c, d, e, f, sizeof(GX_WIDGET))
#define gx_widget_created_test(a, b)                             _gxe_widget_created_test((GX_WIDGET *)a, b)
#define gx_widget_delete(a)                                      _gxe_widget_delete((GX_WIDGET *)a)
#define gx_widget_detach(a)                                      _gxe_widget_detach((GX_WIDGET *)a)
#define gx_widget_draw(a)                                        _gx_widget_draw((GX_WIDGET *)a)
#define gx_widget_draw_set(a, b)                                 _gxe_widget_draw_set((GX_WIDGET *)a, (void (*)(GX_WIDGET *))b)
#define gx_widget_event_generate(a, b, c)                        _gxe_widget_event_generate((GX_WIDGET *)a, b, c)
#define gx_widget_event_process(a, b)                            _gxe_widget_event_process((GX_WIDGET *)a, b)
#define gx_widget_event_process_set(a, b)                        _gxe_widget_event_process_set((GX_WIDGET *)a, (UINT (*)(GX_WIDGET *, GX_EVENT *)) b)
#define gx_widget_event_to_parent(a, b)                          _gxe_widget_event_to_parent((GX_WIDGET *)a, b)
#if defined(GUIX_5_4_0_COMPATIBILITY)
#define gx_widget_fill_color_set(a, b, c)                        _gxe_widget_fill_color_set((GX_WIDGET *)a, b, c, b)
#else
#define gx_widget_fill_color_set(a, b, c, d)                     _gxe_widget_fill_color_set((GX_WIDGET *)a, b, c, d)
#endif
#define gx_widget_find(a, b, c, d)                               _gxe_widget_find((GX_WIDGET *)a, b, c, (GX_WIDGET **)d)
#define gx_widget_first_child_get(a, b)                          _gxe_widget_first_child_get((GX_WIDGET *) a, (GX_WIDGET **) b)
#define gx_widget_focus_next(a)                                  _gxe_widget_focus_next((GX_WIDGET *)a)
#define gx_widget_focus_previous(a)                              _gxe_widget_focus_previous((GX_WIDGET *)a)
#define gx_widget_font_get(a, b, c)                              _gxe_widget_font_get((GX_WIDGET *)a, b, c)
#define gx_widget_free(a)                                        _gxe_widget_free((GX_WIDGET *)a)
#define gx_widget_front_move(a, b)                               _gxe_widget_front_move((GX_WIDGET *)a, b)
#define gx_widget_height_get(a, b)                               _gxe_widget_height_get((GX_WIDGET *)a, b)
#define gx_widget_hide(a)                                        _gxe_widget_hide((GX_WIDGET *)a)
#define gx_widget_last_child_get(a, b)                           _gxe_widget_last_child_get((GX_WIDGET *) a, (GX_WIDGET **) b)
#define gx_widget_next_sibling_get(a, b)                         _gxe_widget_next_sibling_get((GX_WIDGET *) a, (GX_WIDGET **) b)
#define gx_widget_parent_get(a, b)                               _gxe_widget_parent_get((GX_WIDGET *) a, (GX_WIDGET **) b)
#define gx_widget_pixelmap_get(a, b, c)                          _gxe_widget_pixelmap_get((GX_WIDGET *)a, b, c)
#define gx_widget_previous_sibling_get(a, b)                     _gxe_widget_previous_sibling_get((GX_WIDGET *) a, (GX_WIDGET **) b)
#define gx_widget_resize(a, b)                                   _gxe_widget_resize((GX_WIDGET *)a, b)
#define gx_widget_shift(a, b, c, d)                              _gxe_widget_shift((GX_WIDGET *)a, b, c, d)
#define gx_widget_show(a)                                        _gxe_widget_show((GX_WIDGET *)a)
#define gx_widget_status_add(a, b)                               _gxe_widget_status_add((GX_WIDGET *)a, b)
#define gx_widget_status_get(a, b)                               _gxe_widget_status_get((GX_WIDGET *)a, b)
#define gx_widget_status_remove(a, b)                            _gxe_widget_status_remove((GX_WIDGET *)a, b)
#define gx_widget_status_test(a, b, c)                           _gxe_widget_status_test((GX_WIDGET *)a, b, c)
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define gx_widget_string_get(a, b, c)                            _gxe_widget_string_get((GX_WIDGET *)a, b, c)
#endif
#define gx_widget_string_get_ext(a, b, c)                        _gxe_widget_string_get_ext((GX_WIDGET *)a, b, c)
#define gx_widget_style_add(a, b)                                _gxe_widget_style_add((GX_WIDGET *)a, b)
#define gx_widget_style_get(a, b)                                _gxe_widget_style_get((GX_WIDGET *)a, b)
#define gx_widget_style_remove(a, b)                             _gxe_widget_style_remove((GX_WIDGET *)a, b)
#define gx_widget_style_set(a, b)                                _gxe_widget_style_set((GX_WIDGET *)a, b)
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
#define gx_widget_text_blend(a, b, c, d, e, f, g)                _gxe_widget_text_blend((GX_WIDGET *)a, b, c, d, e, f, g)
#define gx_widget_text_draw(a, b, c, d, e, f)                    _gx_widget_text_draw((GX_WIDGET *)a, b, c, d, e, f)
#endif
#define gx_widget_text_blend_ext(a, b, c, d, e, f, g)            _gxe_widget_text_blend_ext((GX_WIDGET *)a, b, c, d, e, f, g)
#define gx_widget_text_draw_ext(a, b, c, d, e, f)                _gx_widget_text_draw_ext((GX_WIDGET *)a, b, c, d, e, f)
#define gx_widget_text_id_draw(a, b, c, d, e, f)                 _gx_widget_text_id_draw((GX_WIDGET *)a, b, c, d, e, f)
#define gx_widget_top_visible_child_find(a, b)                   _gxe_widget_top_visible_child_find((GX_WIDGET *) a, (GX_WIDGET **) b)
#define gx_widget_type_find(a, b, c)                             _gxe_widget_type_find((GX_WIDGET *)a, b, (GX_WIDGET **)c)
#define gx_widget_width_get(a, b)                                _gxe_widget_width_get((GX_WIDGET *)a, b)

#define gx_window_background_draw                                _gx_window_background_draw
#define gx_window_client_height_get                              _gxe_window_client_height_get
#define gx_window_client_scroll                                  _gxe_window_client_scroll
#define gx_window_client_width_get                               _gxe_window_client_width_get
#define gx_window_close                                          _gxe_window_close
#define gx_window_create(a, b, c, d, e, f)                       _gxe_window_create(a, b, (GX_WIDGET *)c, d, e, f, sizeof(GX_WINDOW))
#define gx_window_draw                                           _gx_window_draw
#define gx_window_event_process                                  _gxe_window_event_process
#define gx_window_execute                                        _gxe_window_execute
#define gx_window_root_create(a, b, c, d, e, f)                  _gxe_window_root_create(a, b, c, d, e, f, sizeof(GX_WINDOW_ROOT))
#define gx_window_root_delete                                    _gxe_window_root_delete
#define gx_window_root_event_process                             _gxe_window_root_event_process
#define gx_window_root_find                                      _gxe_window_root_find
#define gx_window_scroll_info_get                                _gxe_window_scroll_info_get
#define gx_window_scrollbar_find                                 _gxe_window_scrollbar_find
#define gx_window_wallpaper_get                                  _gxe_window_wallpaper_get
#define gx_window_wallpaper_set(a, b, c)                         _gxe_window_wallpaper_set((GX_WINDOW *)a, b, c)

/* Define the error checking function prototypes of the GUIX API.  */
UINT _gxe_accordion_menu_create(GX_ACCORDION_MENU *accordion, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                                ULONG style, USHORT accordion_menu_id, GX_CONST GX_RECTANGLE *size, UINT control_block_size);
VOID _gx_accordion_menu_draw(GX_ACCORDION_MENU *menu);
UINT _gxe_accordion_menu_event_process(GX_ACCORDION_MENU *accordion, GX_EVENT *event_ptr);
UINT _gxe_accordion_menu_position(GX_ACCORDION_MENU *accordion);

UINT _gxe_animation_canvas_define(GX_ANIMATION *animation, GX_CANVAS *canvas);
UINT _gxe_animation_create(GX_ANIMATION *animation);
UINT _gxe_animation_delete(GX_ANIMATION *target, GX_WIDGET *parent);
UINT _gxe_animation_drag_disable(GX_ANIMATION *animation, GX_WIDGET *widget);
UINT _gxe_animation_drag_enable(GX_ANIMATION *animation, GX_WIDGET *widget, GX_ANIMATION_INFO *info);
UINT _gxe_animation_landing_speed_set(GX_ANIMATION *animation, USHORT shift_per_step);
UINT _gxe_animation_start(GX_ANIMATION *animation, GX_ANIMATION_INFO *info);
UINT _gxe_animation_stop(GX_ANIMATION *animation);

UINT _gxe_binres_language_count_get(GX_UBYTE *root_address, GX_VALUE *put_count);
UINT _gxe_binres_language_info_load(GX_UBYTE *root_address, GX_LANGUAGE_HEADER *put_info);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gxe_binres_language_table_load(GX_UBYTE *root_address, GX_UBYTE ****returned_language_table);
#endif
UINT _gxe_binres_language_table_load_ext(GX_UBYTE *root_address, GX_STRING ***returned_language_table);
UINT _gxe_binres_theme_load(GX_UBYTE *root_address, INT theme_id, GX_THEME **returned_theme);
UINT _gxe_binres_pixelmap_load(GX_UBYTE *root_address, UINT map_index, GX_PIXELMAP *pixelmap);
UINT _gxe_binres_font_load(GX_UBYTE *root_address, UINT font_index, GX_UBYTE *buffer, ULONG *buffer_size);

UINT _gxe_brush_default(GX_BRUSH *brush);
UINT _gxe_brush_define(GX_BRUSH *brush, GX_COLOR line_color, GX_COLOR fill_color, UINT style);

VOID _gx_button_background_draw(GX_BUTTON *button);
UINT _gxe_button_create(GX_BUTTON *button,
                        GX_CONST GX_CHAR *name,
                        GX_WIDGET *parent,
                        ULONG style, USHORT Id,
                        GX_CONST GX_RECTANGLE *size,
                        UINT button_control_block_size);
UINT _gxe_button_deselect(GX_BUTTON *button, GX_BOOL gen_event);
VOID _gx_button_draw(GX_BUTTON *button);
UINT _gxe_button_event_process(GX_BUTTON *button, GX_EVENT *event_ptr);
UINT _gxe_button_select(GX_BUTTON *button);

UINT _gxe_canvas_alpha_set(GX_CANVAS *canvas, GX_UBYTE alpha);
UINT _gxe_canvas_arc_draw(INT xcenter, INT ycenter, UINT r, INT start_angle, INT end_angle);
UINT _gxe_canvas_block_move(GX_RECTANGLE *block, GX_VALUE x_shift, GX_VALUE y_shift, GX_RECTANGLE *dirty);
UINT _gxe_canvas_circle_draw(INT xcenter, INT ycenter, UINT r);
UINT _gxe_canvas_create(GX_CANVAS *canvas, GX_CONST GX_CHAR *name,
                        GX_DISPLAY *display, UINT type, UINT width, UINT height, GX_COLOR *memory_area,
                        ULONG memory_size, UINT canvas_control_block_size);
UINT _gxe_canvas_delete(GX_CANVAS *canvas);
UINT _gxe_canvas_drawing_complete(GX_CANVAS *canvas, GX_BOOL Flush);
UINT _gxe_canvas_drawing_initiate(GX_CANVAS *canvas, GX_WIDGET *who, GX_RECTANGLE *dirty_area);
UINT _gxe_canvas_ellipse_draw(INT xcenter, INT ycenter, INT a, INT b);
UINT _gxe_canvas_hardware_layer_bind(GX_CANVAS *canvas, INT layer);
UINT _gxe_canvas_hide(GX_CANVAS *canvas);
UINT _gxe_canvas_line_draw(GX_VALUE x_start, GX_VALUE y_start, GX_VALUE x_end, GX_VALUE y_end);
UINT _gxe_canvas_memory_define(GX_CANVAS *canvas, GX_COLOR *memory, ULONG memsize);

#if defined(GX_MOUSE_SUPPORT)
UINT _gxe_canvas_mouse_define(GX_CANVAS *canvas, GX_MOUSE_CURSOR_INFO *info);
UINT _gxe_canvas_mouse_hide(GX_CANVAS *canvas);
UINT _gxe_canvas_mouse_show(GX_CANVAS *canvas);
#endif
UINT _gxe_canvas_offset_set(GX_CANVAS *canvas, GX_VALUE x, GX_VALUE y);
UINT _gxe_canvas_pie_draw(INT xcenter, INT ycenter, UINT r, INT start_angle, INT end_angle);
UINT _gxe_canvas_pixel_draw(GX_POINT position);
UINT _gxe_canvas_pixelmap_blend(GX_VALUE x_position, GX_VALUE y_position, GX_PIXELMAP *pixelmap, GX_UBYTE alpha);
UINT _gxe_canvas_pixelmap_draw(GX_VALUE x_position, GX_VALUE y_position, GX_PIXELMAP *pixelmap);
UINT _gxe_canvas_pixelmap_get(GX_PIXELMAP *pixelmap);
UINT _gxe_canvas_pixelmap_rotate(GX_VALUE x_position, GX_VALUE y_position, GX_PIXELMAP *pixelmap,
                                 INT angle, INT rot_cx, INT rot_cy);
UINT _gxe_canvas_pixelmap_tile(GX_RECTANGLE *fill, GX_PIXELMAP *pixelmap);
UINT _gxe_canvas_polygon_draw(GX_POINT *point_array, INT number_of_points);
UINT _gxe_canvas_rectangle_draw(GX_RECTANGLE *rectangle);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gxe_canvas_rotated_text_draw(GX_CONST GX_CHAR *text, GX_VALUE xcenter, GX_VALUE ycenter, INT angle);
#endif
UINT _gxe_canvas_rotated_text_draw_ext(GX_CONST GX_STRING *text, GX_VALUE xcenter, GX_VALUE ycenter, INT angle);
UINT _gxe_canvas_shift(GX_CANVAS *canvas, GX_VALUE x, GX_VALUE y);
UINT _gxe_canvas_show(GX_CANVAS *canvas);

UINT _gxe_canvas_aligned_text_draw(GX_CONST GX_STRING *string, GX_RECTANGLE *rectangle, ULONG alignment);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gxe_canvas_text_draw(GX_VALUE x_start, GX_VALUE y_start, GX_CONST GX_CHAR *string, INT length);
#endif
UINT _gxe_canvas_text_draw_ext(GX_VALUE x_start, GX_VALUE y_start, GX_CONST GX_STRING *string);

UINT _gxe_checkbox_create(GX_CHECKBOX *checkbox, GX_CONST GX_CHAR *name,
                          GX_WIDGET *parent, GX_RESOURCE_ID text_id,
                          ULONG style, USHORT checkbox_id, GX_CONST GX_RECTANGLE *size,
                          UINT checkbox_control_block_size);
VOID _gx_checkbox_draw(GX_CHECKBOX *checkbox);
UINT _gxe_checkbox_event_process(GX_CHECKBOX *checkbox, GX_EVENT *event_ptr);
UINT _gxe_checkbox_pixelmap_set(GX_CHECKBOX *button, GX_RESOURCE_ID unchecked_id, GX_RESOURCE_ID checked_id,
                                GX_RESOURCE_ID unchecked_disabled_id,  GX_RESOURCE_ID checked_disabled_id);
UINT _gxe_checkbox_select(GX_CHECKBOX *checkbox);

UINT _gxe_circular_gauge_angle_get(GX_CIRCULAR_GAUGE *circular_gauge, INT *angle);
UINT _gxe_circular_gauge_angle_set(GX_CIRCULAR_GAUGE *circular_gauge, INT angle);
UINT _gxe_circular_gauge_animation_set(GX_CIRCULAR_GAUGE *circular_gauge, INT animation_steps, INT delay);
VOID _gx_circular_gauge_background_draw(GX_CIRCULAR_GAUGE *gauge);
UINT _gxe_circular_gauge_create(GX_CIRCULAR_GAUGE *circular_gauge,
                                GX_CONST GX_CHAR *name,
                                GX_WIDGET *parent,
                                GX_CIRCULAR_GAUGE_INFO *circular_gauge_info,
                                GX_RESOURCE_ID background,
                                ULONG style,
                                USHORT circular_gauge_id,
                                GX_VALUE xpos, GX_VALUE ypos, UINT checkbox_control_block_size);
VOID _gx_circular_gauge_draw(GX_CIRCULAR_GAUGE *circular_gauge);
UINT _gxe_circular_gauge_event_process(GX_CIRCULAR_GAUGE *circular_gauge, GX_EVENT *event_ptr);

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
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gxe_context_string_get(GX_RESOURCE_ID string_id, GX_CONST GX_CHAR **return_string);
#endif
UINT _gxe_context_string_get_ext(GX_RESOURCE_ID string_id, GX_STRING *return_string);

UINT _gxe_display_active_language_set(GX_DISPLAY *display, GX_UBYTE language);
UINT _gxe_display_color_set(GX_DISPLAY *display, GX_RESOURCE_ID resource_id, GX_COLOR new_color);
UINT _gxe_display_color_table_set(GX_DISPLAY *display, GX_COLOR *color_table, INT number_of_colors);
UINT _gxe_display_create(GX_DISPLAY *display, GX_CONST GX_CHAR *name, UINT (*display_driver_setup)(GX_DISPLAY *),
                         GX_VALUE width, GX_VALUE height, UINT display_control_block_size);
UINT _gxe_display_delete(GX_DISPLAY *display, VOID (*display_driver_cleanup)(GX_DISPLAY *));
UINT _gxe_display_font_table_set(GX_DISPLAY *display, GX_FONT **font_table, UINT number_of_fonts);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gxe_display_language_table_get(GX_DISPLAY *display, GX_CHAR ****table, GX_UBYTE *language_count, UINT *string_count);
UINT _gxe_display_language_table_set(GX_DISPLAY *display, GX_CHAR ***table,  GX_UBYTE num_languages, UINT number_of_strings);
#endif
UINT _gxe_display_language_table_get_ext(GX_DISPLAY *display, GX_STRING ***table, GX_UBYTE *language_count, UINT *string_count);
UINT _gxe_display_language_table_set_ext(GX_DISPLAY *display, GX_CONST GX_STRING **table,  GX_UBYTE num_languages, UINT number_of_strings);
#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
UINT _gxe_display_language_direction_table_set(GX_DISPLAY *display, GX_CONST GX_UBYTE *language_direction_table, GX_UBYTE num_languages);
#endif
UINT _gxe_display_pixelmap_table_set(GX_DISPLAY *display, GX_PIXELMAP **pixelmap_table, UINT number_of_pixelmaps);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gxe_display_string_get(GX_DISPLAY *display, GX_RESOURCE_ID string_id, GX_CONST GX_CHAR **return_string);
UINT _gxe_display_string_table_get(GX_DISPLAY *display, GX_UBYTE language, GX_CHAR ***table, UINT *size);
#endif
UINT _gxe_display_string_get_ext(GX_DISPLAY *display, GX_RESOURCE_ID string_id, GX_STRING *return_string);
UINT _gxe_display_string_table_get_ext(GX_DISPLAY *display, GX_UBYTE language, GX_STRING **table, UINT *size);
UINT _gxe_display_theme_install(GX_DISPLAY *display, GX_CONST GX_THEME *theme_ptr);

UINT _gxe_drop_list_close(GX_DROP_LIST *drop_list);
UINT _gxe_drop_list_create(GX_DROP_LIST *drop_list, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                           INT total_rows, INT open_height,
                           VOID (*callback)(GX_VERTICAL_LIST *, GX_WIDGET *, INT),
                           ULONG style, USHORT drop_list_id, GX_CONST GX_RECTANGLE *size, UINT drop_list_control_block_size);
UINT _gxe_drop_list_event_process(GX_DROP_LIST *list, GX_EVENT *event_ptr);
UINT _gxe_drop_list_open(GX_DROP_LIST *list);
UINT _gxe_drop_list_pixelmap_set(GX_DROP_LIST *drop_list, GX_RESOURCE_ID pixelmap_id);
UINT _gxe_drop_list_popup_get(GX_DROP_LIST *box, GX_VERTICAL_LIST **return_list);

UINT _gxe_generic_scroll_wheel_create(GX_GENERIC_SCROLL_WHEEL *wheel,
                                      GX_CONST GX_CHAR *name,
                                      GX_WIDGET *parent,
                                      INT total_rows,
                                      VOID (*callback)(GX_GENERIC_SCROLL_WHEEL *, GX_WIDGET *, INT),
                                      ULONG style,
                                      USHORT id,
                                      GX_CONST GX_RECTANGLE *size,
                                      UINT control_block_size);
UINT _gxe_generic_scroll_wheel_children_position(GX_GENERIC_SCROLL_WHEEL *wheel);
UINT _gxe_generic_scroll_wheel_event_process(GX_GENERIC_SCROLL_WHEEL *wheel, GX_EVENT *event_ptr);
UINT _gxe_generic_scroll_wheel_row_height_set(GX_GENERIC_SCROLL_WHEEL *wheel, GX_VALUE row_height);
UINT _gxe_generic_scroll_wheel_total_rows_set(GX_GENERIC_SCROLL_WHEEL *wheel, INT count);

UINT _gxe_horizontal_list_children_position(GX_HORIZONTAL_LIST *horizontal_list);
UINT _gxe_horizontal_list_create(GX_HORIZONTAL_LIST *horizontal_list, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                                 INT total_columns, VOID (*callback)(GX_HORIZONTAL_LIST *, GX_WIDGET *, INT),
                                 ULONG style, USHORT horizontal_list_id,
                                 GX_CONST GX_RECTANGLE *size, UINT horizontal_list_control_block_size);
UINT _gxe_horizontal_list_event_process(GX_HORIZONTAL_LIST *list, GX_EVENT *event_ptr);
UINT _gxe_horizontal_list_page_index_set(GX_HORIZONTAL_LIST *horizontal_list, INT list_entry);
UINT _gxe_horizontal_list_selected_index_get(GX_HORIZONTAL_LIST *horizontal_list, INT *return_index);
UINT _gxe_horizontal_list_selected_set(GX_HORIZONTAL_LIST *horizontal_list, INT index);
UINT _gxe_horizontal_list_selected_widget_get(GX_HORIZONTAL_LIST *horizontal_list, GX_WIDGET **return_list_entry);
UINT _gxe_horizontal_list_total_columns_set(GX_HORIZONTAL_LIST *horizontal_list, INT count);

UINT _gxe_horizontal_scrollbar_create(GX_SCROLLBAR *scrollbar, GX_CONST GX_CHAR *name, GX_WINDOW *parent,
                                      GX_SCROLLBAR_APPEARANCE *appearance, ULONG style, UINT scrollbar_control_block_size);

VOID _gx_icon_background_draw(GX_ICON *icon);
UINT _gxe_icon_button_create(GX_ICON_BUTTON *button, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                             GX_RESOURCE_ID icon_id, ULONG style, USHORT icon_button_id,
                             GX_CONST GX_RECTANGLE *size, UINT button_control_block_size);
VOID _gx_icon_button_draw(GX_ICON_BUTTON *button);
UINT _gxe_icon_button_pixelmap_set(GX_ICON_BUTTON *button, GX_RESOURCE_ID icon_id);
UINT _gxe_icon_create(GX_ICON *icon, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                      GX_RESOURCE_ID pixelmap_id, ULONG style, USHORT icon_id,
                      GX_VALUE x, GX_VALUE y, UINT icon_control_block_size);
VOID _gx_icon_draw(GX_ICON *icon);
UINT _gxe_icon_event_process(GX_ICON *icon, GX_EVENT *event_ptr);
UINT _gxe_icon_pixelmap_set(GX_ICON *icon, GX_RESOURCE_ID normal_id, GX_RESOURCE_ID selected_id);

UINT _gxe_image_reader_create(GX_IMAGE_READER *image_reader,
                              GX_CONST GX_UBYTE *read_data,
                              INT read_data_size,
                              GX_UBYTE color_format,
                              GX_UBYTE mode);
UINT _gxe_image_reader_palette_set(GX_IMAGE_READER *image_reader, GX_COLOR *pal, UINT palsize);
UINT _gxe_image_reader_start(GX_IMAGE_READER *image_reader, GX_PIXELMAP *outmap);

VOID _gx_line_chart_axis_draw(GX_LINE_CHART *chart);
UINT _gxe_line_chart_create(GX_LINE_CHART *chart,
                            GX_CONST GX_CHAR *name,
                            GX_WIDGET *parent,
                            GX_CONST GX_LINE_CHART_INFO *info,
                            ULONG style,
                            USHORT chart_id,
                            GX_CONST GX_RECTANGLE *size,
                            UINT chart_control_block_size);
VOID _gx_line_chart_data_draw(GX_LINE_CHART *chart);
VOID _gx_line_chart_draw(GX_LINE_CHART *chart);
UINT _gxe_line_chart_update(GX_LINE_CHART *chart, INT *data, INT data_count);
UINT _gxe_line_chart_y_scale_calculate(GX_LINE_CHART *chart, INT *return_val);

UINT _gxe_menu_create(GX_MENU *menu, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                      GX_RESOURCE_ID text_id, GX_RESOURCE_ID fill_id,
                      ULONG style, USHORT menu_id, GX_CONST GX_RECTANGLE *size, UINT control_block_size);
VOID _gx_menu_draw(GX_MENU *menu);
UINT _gxe_menu_event_process(GX_MENU *menu, GX_EVENT *event_ptr);
UINT _gxe_menu_insert(GX_MENU *menu, GX_WIDGET *widget);
UINT _gxe_menu_remove(GX_MENU *menu, GX_WIDGET *widget);
VOID _gx_menu_text_draw(GX_MENU *menu);
UINT _gxe_menu_text_offset_set(GX_MENU *menu, GX_VALUE x_offset, GX_VALUE y_offset);

UINT _gxe_multi_line_text_button_create(GX_MULTI_LINE_TEXT_BUTTON *button, GX_CONST GX_CHAR *name,
                                        GX_WIDGET *parent, GX_RESOURCE_ID text_id, ULONG style, USHORT Id,
                                        GX_CONST GX_RECTANGLE *size, UINT control_block_size);
VOID _gx_multi_line_text_button_draw(GX_MULTI_LINE_TEXT_BUTTON *button);
UINT _gxe_multi_line_text_button_event_process(GX_MULTI_LINE_TEXT_BUTTON *button, GX_EVENT *event_ptr);
VOID _gx_multi_line_text_button_text_draw(GX_MULTI_LINE_TEXT_BUTTON *button);
UINT _gxe_multi_line_text_button_text_id_set(GX_MULTI_LINE_TEXT_BUTTON *button, GX_RESOURCE_ID string_id);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gxe_multi_line_text_button_text_set(GX_MULTI_LINE_TEXT_BUTTON *button, GX_CONST GX_CHAR *text);
#endif
UINT _gxe_multi_line_text_button_text_set_ext(GX_MULTI_LINE_TEXT_BUTTON *button, GX_CONST GX_STRING *text);

UINT _gxe_multi_line_text_input_backspace(GX_MULTI_LINE_TEXT_INPUT *text_input);
UINT _gxe_multi_line_text_input_buffer_clear(GX_MULTI_LINE_TEXT_INPUT *text_input_ptr);
UINT _gxe_multi_line_text_input_buffer_get(GX_MULTI_LINE_TEXT_INPUT *text_input_ptr, GX_CHAR **buffer_address,
                                           UINT *content_size, UINT *buffer_size);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gxe_multi_line_text_input_char_insert(GX_MULTI_LINE_TEXT_INPUT *text_input, GX_UBYTE *str, UINT str_size);
#endif
UINT _gxe_multi_line_text_input_char_insert_ext(GX_MULTI_LINE_TEXT_INPUT *text_input, GX_CONST GX_STRING *str);
UINT _gxe_multi_line_text_input_create(GX_MULTI_LINE_TEXT_INPUT *text_input_ptr, GX_CONST GX_CHAR *name_ptr,
                                       GX_WIDGET *parent, GX_CHAR *input_buffer, UINT buffer_size,
                                       ULONG style, USHORT Id, GX_CONST GX_RECTANGLE *size,
                                       UINT text_input_control_block_size);
UINT _gxe_multi_line_text_input_cursor_pos_get(GX_MULTI_LINE_TEXT_INPUT *input, GX_POINT *cursor_pos);
UINT _gxe_multi_line_text_input_delete(GX_MULTI_LINE_TEXT_INPUT *text_input);
UINT _gxe_multi_line_text_input_down_arrow(GX_MULTI_LINE_TEXT_INPUT *text_input);
UINT _gxe_multi_line_text_input_end(GX_MULTI_LINE_TEXT_INPUT *text_input);
UINT _gxe_multi_line_text_input_event_process(GX_MULTI_LINE_TEXT_INPUT *input, GX_EVENT *event_ptr);
UINT _gxe_multi_line_text_input_style_add(GX_MULTI_LINE_TEXT_INPUT *text_input_ptr, ULONG cursor_type);
UINT _gxe_multi_line_text_input_style_remove(GX_MULTI_LINE_TEXT_INPUT *text_input_ptr, ULONG cursor_type);
UINT _gxe_multi_line_text_input_style_set(GX_MULTI_LINE_TEXT_INPUT *text_input_ptr, ULONG cursor_type);
UINT _gxe_multi_line_text_input_fill_color_set(GX_MULTI_LINE_TEXT_INPUT *view,
                                               GX_RESOURCE_ID normal_color_id,
                                               GX_RESOURCE_ID selected_color_id,
                                               GX_RESOURCE_ID disabled_color_id,
                                               GX_RESOURCE_ID readonly_color_id);
UINT _gxe_multi_line_text_input_home(GX_MULTI_LINE_TEXT_INPUT *text_input);
UINT _gxe_multi_line_text_input_left_arrow(GX_MULTI_LINE_TEXT_INPUT *text_input);
UINT _gxe_multi_line_text_input_right_arrow(GX_MULTI_LINE_TEXT_INPUT *text_input);
UINT _gxe_multi_line_text_input_text_color_set(GX_MULTI_LINE_TEXT_INPUT *view,
                                               GX_RESOURCE_ID normal_color_id,
                                               GX_RESOURCE_ID selected_color_id,
                                               GX_RESOURCE_ID disabled_color_id,
                                               GX_RESOURCE_ID readonly_color_id);
UINT _gxe_multi_line_text_input_text_select(GX_MULTI_LINE_TEXT_INPUT *input, UINT start_index, UINT end_index);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gxe_multi_line_text_input_text_set(GX_MULTI_LINE_TEXT_INPUT *text_input, GX_CONST GX_CHAR *text);
#endif
UINT _gxe_multi_line_text_input_text_set_ext(GX_MULTI_LINE_TEXT_INPUT *text_input, GX_CONST GX_STRING *text);
UINT _gxe_multi_line_text_input_up_arrow(GX_MULTI_LINE_TEXT_INPUT *text_input);

UINT _gxe_multi_line_text_view_create(GX_MULTI_LINE_TEXT_VIEW *text_view_ptr, GX_CONST GX_CHAR *name_ptr,
                                      GX_WIDGET *parent, GX_RESOURCE_ID text_id,
                                      ULONG style, USHORT Id, GX_CONST GX_RECTANGLE *size,
                                      UINT text_view_control_block_size);
VOID _gx_multi_line_text_view_draw(GX_MULTI_LINE_TEXT_VIEW *view);
UINT _gxe_multi_line_text_view_event_process(GX_MULTI_LINE_TEXT_VIEW *view, GX_EVENT *event_ptr);
UINT _gxe_multi_line_text_view_font_set(GX_MULTI_LINE_TEXT_VIEW *view, GX_RESOURCE_ID font_id);
UINT _gxe_multi_line_text_view_line_space_set(GX_MULTI_LINE_TEXT_VIEW *view, GX_BYTE line_space);
UINT _gxe_multi_line_text_view_scroll_info_get(GX_MULTI_LINE_TEXT_VIEW *view, ULONG style, GX_SCROLL_INFO *return_scroll_info);
UINT _gxe_multi_line_text_view_text_color_set(GX_MULTI_LINE_TEXT_VIEW *view,
                                              GX_RESOURCE_ID normal_color_id,
                                              GX_RESOURCE_ID selected_color_id,
                                              GX_RESOURCE_ID disabled_color_id);
UINT _gxe_multi_line_text_view_text_id_set(GX_MULTI_LINE_TEXT_VIEW *text_view_ptr, GX_RESOURCE_ID text_id);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gxe_multi_line_text_view_text_set(GX_MULTI_LINE_TEXT_VIEW *view, GX_CONST GX_CHAR *text);
#endif
UINT _gxe_multi_line_text_view_text_set_ext(GX_MULTI_LINE_TEXT_VIEW *view, GX_CONST GX_STRING *text);
UINT _gxe_multi_line_text_view_whitespace_set(GX_MULTI_LINE_TEXT_VIEW *view, GX_UBYTE whitespace);

UINT _gxe_rich_text_view_create(GX_RICH_TEXT_VIEW *rich_view,
                                GX_CONST GX_CHAR *name,
                                GX_WIDGET *parent,
                                GX_RESOURCE_ID text_id,
                                GX_RICH_TEXT_FONTS *fonts,
                                ULONG style,
                                USHORT id,
                                GX_CONST GX_RECTANGLE *size,
                                UINT control_block_size);
VOID _gx_rich_text_view_draw(GX_RICH_TEXT_VIEW *text_view);
VOID _gx_rich_text_view_text_draw(GX_RICH_TEXT_VIEW *text_view);
UINT _gxe_rich_text_view_fonts_set(GX_RICH_TEXT_VIEW *rich_view, GX_RICH_TEXT_FONTS *fonts);

UINT _gxe_numeric_pixelmap_prompt_create(GX_NUMERIC_PIXELMAP_PROMPT *prompt,
                                         GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                                         GX_RESOURCE_ID text_id, GX_RESOURCE_ID fill_id,
                                         ULONG style, USHORT pixelmap_prompt_id,
                                         GX_CONST GX_RECTANGLE *size,
                                         UINT control_block_size);
UINT _gxe_numeric_pixelmap_prompt_format_function_set(GX_NUMERIC_PIXELMAP_PROMPT *prompt, VOID (*format_func)(GX_NUMERIC_PIXELMAP_PROMPT *, INT));
UINT _gxe_numeric_pixelmap_prompt_value_set(GX_NUMERIC_PIXELMAP_PROMPT *prompt, INT value);

UINT _gxe_numeric_prompt_create(GX_NUMERIC_PROMPT *prompt, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                                GX_RESOURCE_ID text_id, ULONG style, USHORT prompt_id, GX_CONST GX_RECTANGLE *size,
                                UINT control_block_size);
UINT _gxe_numeric_prompt_format_function_set(GX_NUMERIC_PROMPT *prompt, VOID (*format_func)(GX_NUMERIC_PROMPT *, INT));
UINT _gxe_numeric_prompt_value_set(GX_NUMERIC_PROMPT *prompt, INT value);

UINT _gxe_numeric_scroll_wheel_create(GX_NUMERIC_SCROLL_WHEEL *wheel, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                                      INT start_val, INT end_val,
                                      ULONG style, USHORT Id, GX_CONST GX_RECTANGLE *size,
                                      UINT numeric_scroll_control_block_size);
UINT _gxe_numeric_scroll_wheel_range_set(GX_NUMERIC_SCROLL_WHEEL *wheel, INT start_val, INT end_val);

UINT _gxe_pixelmap_button_create(GX_PIXELMAP_BUTTON *button, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                                 GX_RESOURCE_ID normal_id, GX_RESOURCE_ID selected_id,
                                 GX_RESOURCE_ID disabled_id, ULONG style, USHORT pixelmap_button_id,
                                 GX_CONST GX_RECTANGLE *size, UINT button_control_block_size);
VOID _gx_pixelmap_button_draw(GX_PIXELMAP_BUTTON *button);
UINT _gxe_pixelmap_button_event_process(GX_PIXELMAP_BUTTON *button, GX_EVENT *event_ptr);
UINT _gxe_pixelmap_button_pixelmap_set(GX_PIXELMAP_BUTTON *button, GX_RESOURCE_ID normal_id, GX_RESOURCE_ID selected_id,
                                       GX_RESOURCE_ID disabled_id);

UINT _gxe_pixelmap_prompt_create(GX_PIXELMAP_PROMPT *prompt, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                                 GX_RESOURCE_ID text_id, GX_RESOURCE_ID normal_pixelmap_id,
                                 ULONG style, USHORT prompt_id, GX_CONST GX_RECTANGLE *size,
                                 UINT pixelmap_prompt_control_block_size);
VOID _gx_pixelmap_prompt_draw(GX_PIXELMAP_PROMPT *prompt);
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
VOID _gx_pixelmap_slider_draw(GX_PIXELMAP_SLIDER *slider);
UINT _gxe_pixelmap_slider_event_process(GX_PIXELMAP_SLIDER *slider, GX_EVENT *event_prt);
UINT _gxe_pixelmap_slider_pixelmap_set(GX_PIXELMAP_SLIDER *slider, GX_PIXELMAP_SLIDER_INFO *info);

VOID _gx_progress_bar_background_draw(GX_PROGRESS_BAR *progress_bar);
UINT _gxe_progress_bar_create(GX_PROGRESS_BAR *progress_bar, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                              GX_PROGRESS_BAR_INFO *progress_bar_info, ULONG style,
                              USHORT progress_bar_id, GX_CONST GX_RECTANGLE *size,
                              UINT progress_bar_control_block_size);
VOID _gx_progress_bar_draw(GX_PROGRESS_BAR *progress_bar);
UINT _gxe_progress_bar_event_process(GX_PROGRESS_BAR *progress_bar, GX_EVENT *event_ptr);
UINT _gxe_progress_bar_font_set(GX_PROGRESS_BAR *progress_bar, GX_RESOURCE_ID font_id);
UINT _gxe_progress_bar_info_set(GX_PROGRESS_BAR *progress_bar, GX_PROGRESS_BAR_INFO *info);
UINT _gxe_progress_bar_pixelmap_set(GX_PROGRESS_BAR *progress_bar, GX_RESOURCE_ID pixelmap);
UINT _gxe_progress_bar_range_set(GX_PROGRESS_BAR *progress_bar, INT min_value, INT max_value);
UINT _gxe_progress_bar_text_color_set(GX_PROGRESS_BAR *progress_bar, GX_RESOURCE_ID normal_text_color_id,
                                      GX_RESOURCE_ID selected_text_color_id, GX_RESOURCE_ID disabled_text_color_id);
VOID _gx_progress_bar_text_draw(GX_PROGRESS_BAR *progress_bar);
UINT _gxe_progress_bar_value_set(GX_PROGRESS_BAR *progress_bar, INT new_value);

UINT _gxe_prompt_create(GX_PROMPT *prompt, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                        GX_RESOURCE_ID text_id, ULONG style, USHORT prompt_id,
                        GX_CONST GX_RECTANGLE *size, UINT prompt_control_block_size);
UINT _gxe_prompt_event_process(GX_PROMPT *prompt, GX_EVENT *event_ptr);
VOID _gx_prompt_draw(GX_PROMPT *prompt);
UINT _gxe_prompt_font_set(GX_PROMPT *prompt, GX_RESOURCE_ID fontid);
UINT _gxe_prompt_text_color_set(GX_PROMPT *prompt,
                                GX_RESOURCE_ID normal_text_color_id,
                                GX_RESOURCE_ID selected_text_color_id,
                                GX_RESOURCE_ID disabled_text_color_id);
VOID _gx_prompt_text_draw(GX_PROMPT *prompt);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gxe_prompt_text_get(GX_PROMPT *prompt, GX_CONST GX_CHAR **return_text);
#endif
UINT _gxe_prompt_text_get_ext(GX_PROMPT *prompt, GX_STRING *return_text);
UINT _gxe_prompt_text_id_set(GX_PROMPT *prompt, GX_RESOURCE_ID string_id);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gxe_prompt_text_set(GX_PROMPT *prompt, GX_CONST GX_CHAR *text);
#endif
UINT _gxe_prompt_text_set_ext(GX_PROMPT *prompt, GX_CONST GX_STRING *text);

UINT _gxe_radial_progress_bar_anchor_set(GX_RADIAL_PROGRESS_BAR *progress_bar, GX_VALUE angle);
VOID _gx_radial_progress_bar_background_draw(GX_RADIAL_PROGRESS_BAR *progress_bar);
UINT _gxe_radial_progress_bar_create(GX_RADIAL_PROGRESS_BAR *progress_bar,
                                     GX_CONST GX_CHAR *name,
                                     GX_WIDGET *parent,
                                     GX_RADIAL_PROGRESS_BAR_INFO *progress_bar_info,
                                     ULONG style,
                                     USHORT progress_bar_id,
                                     UINT progress_bar_control_block_size);
VOID _gx_radial_progress_bar_draw(GX_RADIAL_PROGRESS_BAR *progress_bar);
UINT _gxe_radial_progress_bar_event_process(GX_RADIAL_PROGRESS_BAR *progress_bar, GX_EVENT *event_ptr);
UINT _gxe_radial_progress_bar_font_set(GX_RADIAL_PROGRESS_BAR *progress_bar, GX_RESOURCE_ID font_id);
UINT _gxe_radial_progress_bar_info_set(GX_RADIAL_PROGRESS_BAR *bar, GX_RADIAL_PROGRESS_BAR_INFO *info);
UINT _gxe_radial_progress_bar_text_color_set(GX_RADIAL_PROGRESS_BAR *progress_bar,
                                             GX_RESOURCE_ID normal_text_color_id,
                                             GX_RESOURCE_ID selected_text_color_id,
                                             GX_RESOURCE_ID disabled_text_color_id);
VOID _gx_radial_progress_bar_text_draw(GX_RADIAL_PROGRESS_BAR *bar);
UINT _gxe_radial_progress_bar_value_set(GX_RADIAL_PROGRESS_BAR *progress_bar, GX_VALUE new_value);

UINT _gxe_radial_slider_anchor_angles_set(GX_RADIAL_SLIDER *slider, GX_VALUE *anchor_angles, USHORT anchor_count);
UINT _gxe_radial_slider_animation_set(GX_RADIAL_SLIDER *slider, USHORT steps, USHORT delay, USHORT animation_style,
                                      VOID (*animation_update_callback)(GX_RADIAL_SLIDER *slider));
UINT _gxe_radial_slider_animation_start(GX_RADIAL_SLIDER *slider, GX_VALUE new_value);
UINT _gxe_radial_slider_create(GX_RADIAL_SLIDER *slider, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                               GX_RADIAL_SLIDER_INFO *info, ULONG style, USHORT slider_id, GX_CONST GX_RECTANGLE *size, UINT control_block_size);
VOID _gx_radial_slider_draw(GX_RADIAL_SLIDER *slider);
UINT _gxe_radial_slider_event_process(GX_RADIAL_SLIDER *slider, GX_EVENT *event_ptr);
UINT _gxe_radial_slider_info_get(GX_RADIAL_SLIDER *slider, GX_RADIAL_SLIDER_INFO **info);
UINT _gxe_radial_slider_info_set(GX_RADIAL_SLIDER *slider,  GX_RADIAL_SLIDER_INFO *info);
UINT _gxe_radial_slider_pixelmap_set(GX_RADIAL_SLIDER *slider, GX_RESOURCE_ID background_pixelmap, GX_RESOURCE_ID needle_pixelmap);
UINT _gxe_radial_slider_angle_set(GX_RADIAL_SLIDER *slider, GX_VALUE new_angle);

UINT _gxe_radio_button_create(GX_RADIO_BUTTON *button, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                              GX_RESOURCE_ID text_id, ULONG style, USHORT radio_button_id,
                              GX_CONST GX_RECTANGLE *size, UINT radio_button_control_block_size);
VOID _gx_radio_button_draw(GX_RADIO_BUTTON *button);
UINT _gxe_radio_button_pixelmap_set(GX_RADIO_BUTTON *button, GX_RESOURCE_ID unchecked_id, GX_RESOURCE_ID checked_id,
                                    GX_RESOURCE_ID unchecked_disabled_id, GX_RESOURCE_ID checked_disabled_id);

UINT _gxe_screen_stack_create(GX_SCREEN_STACK_CONTROL *control, GX_WIDGET **memory, INT size);
UINT _gxe_screen_stack_pop(GX_SCREEN_STACK_CONTROL *control);
UINT _gxe_screen_stack_push(GX_SCREEN_STACK_CONTROL *control, GX_WIDGET *screen, GX_WIDGET *new_screen);
UINT _gxe_screen_stack_reset(GX_SCREEN_STACK_CONTROL *control);

UINT _gxe_scroll_thumb_create(GX_SCROLL_THUMB *scroll_thumb, GX_SCROLLBAR *parent, ULONG style, UINT scroll_thumb_control_block_size);
VOID _gx_scroll_thumb_draw(GX_SCROLL_THUMB *scroll_thumb);
UINT _gxe_scroll_thumb_event_process(GX_SCROLL_THUMB *scroll_thumb, GX_EVENT *event_ptr);

VOID _gx_scrollbar_draw(GX_SCROLLBAR *scrollbar);
UINT _gxe_scrollbar_event_process(GX_SCROLLBAR *scrollbar, GX_EVENT *event_ptr);
UINT _gxe_scrollbar_limit_check(GX_SCROLLBAR *scrollbar);
UINT _gxe_scrollbar_reset(GX_SCROLLBAR *scrollbar, GX_SCROLL_INFO *info);
UINT _gxe_scrollbar_value_set(GX_SCROLLBAR *scrollbar, INT value);

UINT _gxe_scroll_wheel_create(GX_SCROLL_WHEEL *wheel, GX_CONST GX_CHAR *name, GX_WIDGET *parent, INT total_rows,
                              ULONG style, USHORT Id, GX_CONST GX_RECTANGLE *size, UINT control_block_size);
UINT _gxe_scroll_wheel_event_process(GX_SCROLL_WHEEL *wheel, GX_EVENT *event_ptr);
UINT _gxe_scroll_wheel_gradient_alpha_set(GX_SCROLL_WHEEL *wheel, GX_UBYTE start_alpha, GX_UBYTE end_alpha);
UINT _gxe_scroll_wheel_row_height_set(GX_SCROLL_WHEEL *wheel, GX_VALUE row_height);
UINT _gxe_scroll_wheel_selected_background_set(GX_SCROLL_WHEEL *wheel, GX_RESOURCE_ID selected_bg);
UINT _gxe_scroll_wheel_selected_get(GX_SCROLL_WHEEL *wheel, INT *row);
UINT _gxe_scroll_wheel_selected_set(GX_SCROLL_WHEEL *wheel, INT row);
UINT _gxe_scroll_wheel_speed_set(GX_SCROLL_WHEEL *wheel, GX_FIXED_VAL start_speed_rate, GX_FIXED_VAL end_speed_rate, GX_VALUE max_steps, GX_VALUE delay);
UINT _gxe_scroll_wheel_total_rows_set(GX_SCROLL_WHEEL *wheel, INT total_rows);

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
VOID _gx_single_line_text_input_draw(GX_SINGLE_LINE_TEXT_INPUT *text_input);
UINT _gxe_single_line_text_input_draw_position_get(GX_SINGLE_LINE_TEXT_INPUT *input, GX_VALUE *xpos, GX_VALUE *ypos);
UINT _gxe_single_line_text_input_end(GX_SINGLE_LINE_TEXT_INPUT *text_input);
UINT _gxe_single_line_text_input_event_process(GX_SINGLE_LINE_TEXT_INPUT *text_input, GX_EVENT *event_ptr);
UINT _gxe_single_line_text_input_home(GX_SINGLE_LINE_TEXT_INPUT *text_input);
UINT _gxe_single_line_text_input_left_arrow(GX_SINGLE_LINE_TEXT_INPUT *text_input);
UINT _gxe_single_line_text_input_position_get(GX_SINGLE_LINE_TEXT_INPUT *text_input, INT pixel_position);
UINT _gxe_single_line_text_input_right_arrow(GX_SINGLE_LINE_TEXT_INPUT *text_input);
UINT _gxe_single_line_text_input_style_add(GX_SINGLE_LINE_TEXT_INPUT *text_input_ptr, ULONG style);
UINT _gxe_single_line_text_input_style_remove(GX_SINGLE_LINE_TEXT_INPUT *text_input_ptr, ULONG style);
UINT _gxe_single_line_text_input_style_set(GX_SINGLE_LINE_TEXT_INPUT *text_input_ptr, ULONG style);
UINT _gxe_single_line_text_input_fill_color_set(GX_SINGLE_LINE_TEXT_INPUT *input,
                                                GX_RESOURCE_ID normal_color_id,
                                                GX_RESOURCE_ID selected_color_id,
                                                GX_RESOURCE_ID disabled_color_id,
                                                GX_RESOURCE_ID readonly_color_id);
UINT _gxe_single_line_text_input_text_color_set(GX_SINGLE_LINE_TEXT_INPUT *input,
                                                GX_RESOURCE_ID normal_color_id,
                                                GX_RESOURCE_ID selected_color_id,
                                                GX_RESOURCE_ID disabled_color_id,
                                                GX_RESOURCE_ID readonly_color_id);
UINT _gxe_single_line_text_input_text_select(GX_SINGLE_LINE_TEXT_INPUT *input, UINT start_index, UINT end_index);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gxe_single_line_text_input_text_set(GX_SINGLE_LINE_TEXT_INPUT *input, GX_CONST GX_CHAR *text);
#endif
UINT _gxe_single_line_text_input_text_set_ext(GX_SINGLE_LINE_TEXT_INPUT *input, GX_CONST GX_STRING *text);

UINT _gxe_slider_create(GX_SLIDER *slider, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                        INT tick_count, GX_SLIDER_INFO *slider_info, ULONG style, USHORT slider_id,
                        GX_CONST GX_RECTANGLE *size, UINT slider_control_block_size);
VOID _gx_slider_draw(GX_SLIDER *widget);
UINT _gxe_slider_event_process(GX_SLIDER *widget, GX_EVENT *event_ptr);
UINT _gxe_slider_info_set(GX_SLIDER *slider, GX_SLIDER_INFO *info);
VOID _gx_slider_needle_draw(GX_SLIDER *slider);
UINT _gxe_slider_needle_position_get(GX_SLIDER *slider, GX_SLIDER_INFO *slider_info, GX_RECTANGLE *return_position);
VOID _gx_slider_tickmarks_draw(GX_SLIDER *slider);
UINT _gxe_slider_travel_get(GX_SLIDER *widget, GX_SLIDER_INFO *info, INT *mintravel, INT *maxtravel);
UINT _gxe_slider_value_calculate(GX_SLIDER *slider, GX_SLIDER_INFO *info, INT new_position);
UINT _gxe_slider_value_set(GX_SLIDER *slider, GX_SLIDER_INFO *info, INT new_value);

UINT _gxe_sprite_create(GX_SPRITE *sprite, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                        GX_SPRITE_FRAME *frame_list, USHORT framecount,
                        ULONG style, USHORT sprite_id, GX_CONST GX_RECTANGLE *size, UINT sprite_control_block_size);
UINT _gxe_sprite_current_frame_set(GX_SPRITE *sprite, USHORT frame);
UINT _gxe_sprite_frame_list_set(GX_SPRITE *sprite, GX_SPRITE_FRAME *frame_list, USHORT frame_count);
UINT _gxe_sprite_start(GX_SPRITE *sprite, USHORT start_frame);
UINT _gxe_sprite_stop(GX_SPRITE *sprite);

#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gxe_string_scroll_wheel_create(GX_STRING_SCROLL_WHEEL *wheel, GX_CONST GX_CHAR *name, GX_WIDGET *parent, INT value_count,
                                     GX_CONST GX_CHAR **string_list,
                                     ULONG style, USHORT Id, GX_CONST GX_RECTANGLE *size,
                                     UINT control_block_size);
#endif
UINT _gxe_string_scroll_wheel_create_ext(GX_STRING_SCROLL_WHEEL *wheel, GX_CONST GX_CHAR *name, GX_WIDGET *parent, INT total_rows,
                                         GX_CONST GX_STRING *string_list,
                                         ULONG style, USHORT Id, GX_CONST GX_RECTANGLE *size,
                                         UINT control_block_size);
UINT _gxe_string_scroll_wheel_event_process(GX_STRING_SCROLL_WHEEL *wheel, GX_EVENT *event_ptr);
UINT _gxe_string_scroll_wheel_string_id_list_set(GX_STRING_SCROLL_WHEEL *wheel,
                                                 GX_CONST GX_RESOURCE_ID *string_id_list,
                                                 INT id_count);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gxe_string_scroll_wheel_string_list_set(GX_STRING_SCROLL_WHEEL *wheel,
                                              GX_CONST GX_CHAR **string_list,
                                              INT string_count);
#endif
UINT _gxe_string_scroll_wheel_string_list_set_ext(GX_STRING_SCROLL_WHEEL *wheel,
                                                  GX_CONST GX_STRING *string_list,
                                                  INT string_count);

UINT _gxe_system_active_language_set(GX_UBYTE language);
UINT _gxe_system_animation_get(GX_ANIMATION **animation);
UINT _gxe_system_animation_free(GX_ANIMATION *animation);
#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
UINT _gx_system_bidi_text_enable(VOID);
UINT _gx_system_bidi_text_disable(VOID);
#endif

#if defined(GX_THAI_GLYPH_SHAPING_SUPPORT)
UINT _gx_system_text_render_style_set(GX_UBYTE style);
#endif
UINT _gxe_system_canvas_refresh(VOID);
UINT _gxe_system_dirty_mark(GX_WIDGET *widget);
UINT _gxe_system_dirty_partial_add(GX_WIDGET *widget, GX_RECTANGLE *dirty_area);
UINT _gxe_system_draw_context_get(GX_DRAW_CONTEXT **current_context);
UINT _gxe_system_event_fold(GX_EVENT *event_ptr);
UINT _gxe_system_event_send(GX_EVENT *event_ptr);
UINT _gxe_system_focus_claim(GX_WIDGET *widget);
UINT _gxe_system_initialize(VOID);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gxe_system_language_table_get(GX_CHAR ****language_table, GX_UBYTE *language_count, UINT *string_count);
UINT _gxe_system_language_table_set(GX_CHAR ***language_table, GX_UBYTE number_of_languages, UINT number_of_strings);
#endif
UINT _gxe_system_memory_allocator_set(VOID *(*allocate)(ULONG size), VOID (*release)(VOID *));
UINT _gxe_system_pen_configure(GX_PEN_CONFIGURATION *pen_configuration);
UINT _gxe_system_screen_stack_create(GX_WIDGET **memory, INT size, UINT control_block_size);
UINT _gxe_system_screen_stack_get(GX_WIDGET **popped_parent, GX_WIDGET **popped_screen);
UINT _gxe_system_screen_stack_pop(VOID);
UINT _gxe_system_screen_stack_push(GX_WIDGET *screen);
UINT _gx_system_screen_stack_get(GX_WIDGET **popped_parent, GX_WIDGET **popped_screen);
UINT _gx_system_screen_stack_reset(VOID);
UINT _gxe_system_scroll_appearance_get(ULONG style, GX_SCROLLBAR_APPEARANCE *return_appearance);
UINT _gxe_system_scroll_appearance_set(ULONG style, GX_SCROLLBAR_APPEARANCE *appearance);
UINT _gxe_system_start(VOID);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gxe_system_string_get(GX_RESOURCE_ID string_id, GX_CONST GX_CHAR **return_string);
UINT _gxe_system_string_table_get(GX_UBYTE language, GX_CHAR ***get_table, UINT *get_size);
UINT _gxe_system_string_width_get(GX_CONST GX_FONT *font, GX_CONST GX_CHAR *string, INT string_length, GX_VALUE *return_width);
#endif
UINT _gxe_system_string_width_get_ext(GX_CONST GX_FONT *font, GX_CONST GX_STRING *string, GX_VALUE *return_width);
UINT _gxe_system_timer_start(GX_WIDGET *owner, UINT timer_id, UINT initial_ticks, UINT reschedule_ticks);
UINT _gxe_system_timer_stop(GX_WIDGET *owner, UINT timer_id);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gxe_system_version_string_get(GX_CHAR **return_string);
#endif
UINT _gxe_system_version_string_get_ext(GX_STRING *return_string);
UINT _gxe_system_widget_find(USHORT widget_id, INT search_level, GX_WIDGET **return_search_result);

UINT _gxe_text_button_create(GX_TEXT_BUTTON *button, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                             GX_RESOURCE_ID text_id, ULONG style, USHORT Id,
                             GX_CONST GX_RECTANGLE *size, UINT text_button_control_block_size);
VOID _gx_text_button_draw(GX_TEXT_BUTTON *button);
UINT _gxe_text_button_event_process(GX_TEXT_BUTTON *button, GX_EVENT *event_ptr);
UINT _gxe_text_button_font_set(GX_TEXT_BUTTON *button, GX_RESOURCE_ID font_id);
UINT _gxe_text_button_text_color_set(GX_TEXT_BUTTON *_text_button,
                                     GX_RESOURCE_ID normal_text_color_id,
                                     GX_RESOURCE_ID selected_text_color_id,
                                     GX_RESOURCE_ID disabled_text_color_id);
VOID _gx_text_button_text_draw(GX_TEXT_BUTTON *button);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gxe_text_button_text_get(GX_TEXT_BUTTON *button, GX_CONST GX_CHAR **return_text);
#endif
UINT _gxe_text_button_text_get_ext(GX_TEXT_BUTTON *button, GX_STRING *return_text);
UINT _gxe_text_button_text_id_set(GX_TEXT_BUTTON *button, GX_RESOURCE_ID string_id);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gxe_text_button_text_set(GX_TEXT_BUTTON *button, GX_CONST GX_CHAR *text);
#endif
UINT _gxe_text_button_text_set_ext(GX_TEXT_BUTTON *button, GX_CONST GX_STRING *text);

UINT _gxe_text_input_cursor_blink_interval_set(GX_TEXT_INPUT_CURSOR *cursor_input, GX_UBYTE blink_interval);
UINT _gxe_text_input_cursor_height_set(GX_TEXT_INPUT_CURSOR *cursor_input, GX_UBYTE height);
UINT _gxe_text_input_cursor_width_set(GX_TEXT_INPUT_CURSOR *cursor_input, GX_UBYTE width);

#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gxe_text_scroll_wheel_callback_set(GX_TEXT_SCROLL_WHEEL * wheel, GX_CONST GX_CHAR * (*callback)(GX_TEXT_SCROLL_WHEEL *, INT));
#endif
UINT _gxe_text_scroll_wheel_callback_set_ext(GX_TEXT_SCROLL_WHEEL *wheel, UINT (*callback)(GX_TEXT_SCROLL_WHEEL *, INT, GX_STRING *));
UINT _gxe_text_scroll_wheel_font_set(GX_TEXT_SCROLL_WHEEL *wheel, GX_RESOURCE_ID normal_font, GX_RESOURCE_ID selected_font);
UINT _gxe_text_scroll_wheel_text_color_set(GX_TEXT_SCROLL_WHEEL *wheel, GX_RESOURCE_ID normal_text_color,
                                           GX_RESOURCE_ID selected_text_color, GX_RESOURCE_ID disabled_text_color);
UINT _gxe_text_scroll_wheel_create(GX_TEXT_SCROLL_WHEEL *wheel, GX_CONST GX_CHAR *name, GX_WIDGET *parent, INT value_count,
                                   ULONG style, USHORT Id, GX_CONST GX_RECTANGLE *size,
                                   UINT control_block_size);
UINT _gxe_text_scroll_wheel_event_process(GX_TEXT_SCROLL_WHEEL *wheel, GX_EVENT *event_ptr);
VOID _gx_text_scroll_wheel_draw(GX_TEXT_SCROLL_WHEEL *wheel);

UINT _gxe_tree_view_create(GX_TREE_VIEW *tree, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
    ULONG style, USHORT tree_id, GX_CONST GX_RECTANGLE *size, UINT control_block_size);
VOID _gx_tree_view_draw(GX_TREE_VIEW *tree);
UINT _gxe_tree_view_event_process(GX_TREE_VIEW *tree, GX_EVENT *event_ptr);
UINT _gxe_tree_view_indentation_set(GX_TREE_VIEW *tree, GX_VALUE indentation);
UINT _gxe_tree_view_position(GX_TREE_VIEW *tree);
UINT _gxe_tree_view_root_line_color_set(GX_TREE_VIEW *tree, GX_RESOURCE_ID color);
UINT _gxe_tree_view_root_pixelmap_set(GX_TREE_VIEW *tree, GX_RESOURCE_ID expand_map_id, GX_RESOURCE_ID collapse_map_id);
UINT _gxe_tree_view_selected_get(GX_TREE_VIEW *tree, GX_WIDGET **selected);
UINT _gxe_tree_view_selected_set(GX_TREE_VIEW *tree, GX_WIDGET *selected);

UINT    _gxe_utility_gradient_create(GX_GRADIENT *gradient, GX_VALUE width, GX_VALUE height, UCHAR type, UCHAR alpha_start, UCHAR alpha_end);
UINT    _gxe_utility_gradient_delete(GX_GRADIENT *gradient);
#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
UINT    _gxe_utility_bidi_paragraph_reorder(GX_BIDI_TEXT_INFO *input_info, GX_BIDI_RESOLVED_TEXT_INFO **resolved_info_head);
UINT    _gxe_utility_bidi_paragraph_reorder_ext(GX_BIDI_TEXT_INFO *input_info, GX_BIDI_RESOLVED_TEXT_INFO **resolved_info_head);
UINT    _gxe_utility_bidi_resolved_text_info_delete(GX_BIDI_RESOLVED_TEXT_INFO **resolved_info_head);
#endif
UINT    _gxe_utility_canvas_to_bmp(GX_CANVAS *canvas, GX_RECTANGLE *rect, UINT(*write_data)(GX_UBYTE *byte_data, UINT data_count));
UINT    _gxe_utility_circle_point_get(INT xcenter, INT ycenter, UINT r, INT angle, GX_POINT *point);
UINT    _gxe_utility_ltoa(LONG value, GX_CHAR *return_buffer, UINT return_buffer_size);
#if defined(GUIX_5_4_0_COMPATIBILITY)
INT     _gx_utility_math_acos_5_4_0(INT x);
INT     _gx_utility_math_asin_5_4_0(INT x);
INT     _gx_utility_math_cos_5_4_0(INT angle);
INT     _gx_utility_math_sin_5_4_0(INT angle);
#else
INT     _gx_utility_math_acos(GX_FIXED_VAL x);
INT     _gx_utility_math_asin(GX_FIXED_VAL x);
GX_FIXED_VAL _gx_utility_math_cos(GX_FIXED_VAL angle);
GX_FIXED_VAL _gx_utility_math_sin(GX_FIXED_VAL angle);
#endif
UINT    _gx_utility_math_sqrt(UINT n);

UINT    _gxe_utility_pixelmap_resize(GX_PIXELMAP *src, GX_PIXELMAP *destination, INT width, INT height);
UINT    _gxe_utility_pixelmap_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy);
UINT    _gxe_utility_pixelmap_simple_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy);

UINT    _gxe_utility_rectangle_center(GX_RECTANGLE *rectangle, GX_RECTANGLE *within);
UINT    _gxe_utility_rectangle_center_find(GX_RECTANGLE *rectangle, GX_POINT *return_center);
UINT    _gxe_utility_rectangle_combine(GX_RECTANGLE *first_rectangle, GX_RECTANGLE *second_rectangle);
GX_BOOL _gxe_utility_rectangle_compare(GX_RECTANGLE *first_rectangle, GX_RECTANGLE *second_rectangle);
UINT    _gxe_utility_rectangle_define(GX_RECTANGLE *rectangle, GX_VALUE left, GX_VALUE top, GX_VALUE right, GX_VALUE bottom);
GX_BOOL _gxe_utility_rectangle_overlap_detect(GX_RECTANGLE *first_rectangle, GX_RECTANGLE *second_rectangle, GX_RECTANGLE *return_overlap_area);
GX_BOOL _gxe_utility_rectangle_point_detect(GX_RECTANGLE *rectangle, GX_POINT point);
UINT    _gxe_utility_rectangle_resize(GX_RECTANGLE *rectangle, GX_VALUE adjust);
UINT    _gxe_utility_rectangle_shift(GX_RECTANGLE *rectangle, GX_VALUE x_shift, GX_VALUE y_shift);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT    _gxe_utility_string_to_alphamap(GX_CONST GX_CHAR *text, GX_CONST GX_FONT *font, GX_PIXELMAP *textmap);
#endif
UINT    _gxe_utility_string_to_alphamap_ext(GX_CONST GX_STRING *text, GX_CONST GX_FONT *font, GX_PIXELMAP *textmap);

UINT    _gxe_vertical_list_children_position(GX_VERTICAL_LIST *vertical_list);
UINT    _gxe_vertical_list_create(GX_VERTICAL_LIST *vertical_list, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                                  INT total_rows,
                                  VOID (*callback)(GX_VERTICAL_LIST *, GX_WIDGET *, INT),
                                  ULONG style, USHORT vertical_list_id, GX_CONST GX_RECTANGLE *size,
                                  UINT vertical_list_control_block_size);
UINT _gxe_vertical_list_event_process(GX_VERTICAL_LIST *list, GX_EVENT *event_ptr);
UINT _gxe_vertical_list_page_index_set(GX_VERTICAL_LIST *list, INT index);
UINT _gxe_vertical_list_selected_index_get(GX_VERTICAL_LIST *vertical_list, INT *return_index);
UINT _gxe_vertical_list_selected_set(GX_VERTICAL_LIST *vertical_list, INT index);
UINT _gxe_vertical_list_selected_widget_get(GX_VERTICAL_LIST *vertical_list, GX_WIDGET **return_list_entry);
UINT _gxe_vertical_list_total_rows_set(GX_VERTICAL_LIST *list, INT count);

UINT _gxe_vertical_scrollbar_create(GX_SCROLLBAR *scrollbar, GX_CONST GX_CHAR *name, GX_WINDOW *parent,
                                    GX_SCROLLBAR_APPEARANCE *settings,
                                    ULONG style, UINT scrollbar_control_blcok_size);

UINT _gxe_widget_allocate(GX_WIDGET **widget, ULONG memsize);
UINT _gxe_widget_attach(GX_WIDGET *parent, GX_WIDGET *widget);
UINT _gxe_widget_back_attach(GX_WIDGET *parent, GX_WIDGET *widget);
UINT _gxe_widget_back_move(GX_WIDGET *widget, GX_BOOL *widget_moved);
VOID _gx_widget_background_draw(GX_WIDGET *a);
UINT _gxe_widget_block_move(GX_WIDGET *widget, GX_RECTANGLE *block, INT x_shift, INT y_shift);
VOID _gx_widget_border_draw(GX_WIDGET *widget, GX_RESOURCE_ID border_color, GX_RESOURCE_ID upper_color, GX_RESOURCE_ID lower_color, GX_BOOL fill);
UINT _gxe_widget_border_style_set(GX_WIDGET *widget, ULONG Style);
UINT _gxe_widget_border_width_get(GX_WIDGET *widget, GX_VALUE *return_width);
UINT _gxe_widget_canvas_get(GX_WIDGET *widget, GX_CANVAS **return_canvas);
UINT _gxe_widget_color_get(GX_WIDGET *widget, GX_RESOURCE_ID color_id, GX_COLOR *return_color);
UINT _gxe_widget_child_detect(GX_WIDGET *parent, GX_WIDGET *child, GX_BOOL *return_detect);
VOID _gx_widget_children_draw(GX_WIDGET *widget);
UINT _gxe_widget_client_get(GX_WIDGET *widget, GX_VALUE border_width, GX_RECTANGLE *return_size);
UINT _gxe_widget_create(GX_WIDGET *widget,
                        GX_CONST GX_CHAR *name,
                        GX_WIDGET *parent,
                        ULONG style, USHORT Id,
                        GX_CONST GX_RECTANGLE *size,
                        UINT widget_block_size);
UINT _gxe_widget_created_test(GX_WIDGET *widget, GX_BOOL *return_test);
UINT _gxe_widget_delete(GX_WIDGET *widget);
UINT _gxe_widget_detach(GX_WIDGET *widget);
VOID _gx_widget_draw(GX_WIDGET *widget);
UINT _gxe_widget_draw_set(GX_WIDGET *widget, VOID (*draw_func)(GX_WIDGET *));
UINT _gxe_widget_event_generate(GX_WIDGET *widget, USHORT event_type, LONG value);
UINT _gxe_widget_event_process(GX_WIDGET *widget, GX_EVENT *event_ptr);
UINT _gxe_widget_event_process_set(GX_WIDGET *widget, UINT (*event_processing_function)(GX_WIDGET *, GX_EVENT *));
UINT _gxe_widget_event_to_parent(GX_WIDGET *widget, GX_EVENT *event_ptr);
UINT _gxe_widget_fill_color_set(GX_WIDGET *widget, GX_RESOURCE_ID normal_color, GX_RESOURCE_ID selected_color, GX_RESOURCE_ID disabled_color);
UINT _gxe_widget_find(GX_WIDGET *parent, USHORT widget_id, INT search_depth, GX_WIDGET **return_widget);
UINT _gxe_widget_first_child_get(GX_WIDGET *parent, GX_WIDGET **child_return);
UINT _gxe_widget_focus_next(GX_WIDGET *widget);
UINT _gxe_widget_focus_previous(GX_WIDGET *widget);
UINT _gxe_widget_font_get(GX_WIDGET *widget, GX_RESOURCE_ID font_id, GX_FONT **return_font);
UINT _gxe_widget_free(GX_WIDGET *widget);
UINT _gxe_widget_front_move(GX_WIDGET *widget, GX_BOOL *widget_moved);
UINT _gxe_widget_height_get(GX_WIDGET *widget, GX_VALUE *return_height);
UINT _gxe_widget_hide(GX_WIDGET *widget);
UINT _gxe_widget_last_child_get(GX_WIDGET *parent, GX_WIDGET **child_return);
UINT _gxe_widget_next_sibling_get(GX_WIDGET *current, GX_WIDGET **sibling_return);
UINT _gxe_widget_parent_get(GX_WIDGET *current, GX_WIDGET **parent_return);
UINT _gxe_widget_pixelmap_get(GX_WIDGET *widget, GX_RESOURCE_ID pixelmap_id, GX_PIXELMAP **return_pixelmap);
UINT _gxe_widget_previous_sibling_get(GX_WIDGET *current, GX_WIDGET **sibling_return);
UINT _gxe_widget_resize(GX_WIDGET *widget, GX_RECTANGLE *newsize);
UINT _gxe_widget_shift(GX_WIDGET *widget, GX_VALUE x_shift, GX_VALUE y_shift, GX_BOOL mark_dirty);
UINT _gxe_widget_show(GX_WIDGET *widget);
UINT _gxe_widget_status_add(GX_WIDGET *widget, ULONG status);
UINT _gxe_widget_status_get(GX_WIDGET *widget, ULONG *return_status);
UINT _gxe_widget_status_remove(GX_WIDGET *widget, ULONG status);
UINT _gxe_widget_status_test(GX_WIDGET *widget, ULONG status, GX_BOOL *return_test);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gxe_widget_string_get(GX_WIDGET *widget, GX_RESOURCE_ID string_id, GX_CONST GX_CHAR **return_string);
#endif
UINT _gxe_widget_string_get_ext(GX_WIDGET *widget, GX_RESOURCE_ID string_id, GX_STRING *return_string);
UINT _gxe_widget_style_add(GX_WIDGET *widget, ULONG style);
UINT _gxe_widget_style_get(GX_WIDGET *widget, ULONG *return_style);
UINT _gxe_widget_style_remove(GX_WIDGET *widget, ULONG style);
UINT _gxe_widget_style_set(GX_WIDGET *widget, ULONG style);

#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gxe_widget_text_blend(GX_WIDGET *widget, UINT tColor, UINT font_id, GX_CONST GX_CHAR *string,
                            INT x_offset, INT y_offset, UCHAR alpha);
VOID _gx_widget_text_draw(GX_WIDGET *widget, UINT tColor, UINT font_id, GX_CONST GX_CHAR *string,
                          INT x_offset, INT y_offset);
#endif
UINT _gxe_widget_text_blend_ext(GX_WIDGET *widget,  UINT tColor, UINT font_id, GX_CONST GX_STRING *string,
                                INT x_offset, INT y_offset, UCHAR alpha);

VOID _gx_widget_text_draw_ext(GX_WIDGET *widget, UINT tColor, UINT font_id, GX_CONST GX_STRING *string,
                              INT x_offset, INT y_offset);

VOID _gx_widget_text_id_draw(GX_WIDGET *widget, UINT tColor,
                             UINT font_id, UINT text_id,
                             INT x_offset, INT y_offset);

UINT _gxe_widget_top_visible_child_find(GX_WIDGET *parent, GX_WIDGET **child_return);
UINT _gxe_widget_type_find(GX_WIDGET *parent, USHORT widget_type, GX_WIDGET **return_widget);
UINT _gxe_widget_width_get(GX_WIDGET *widget, GX_VALUE *return_width);

VOID _gx_window_background_draw(GX_WINDOW *window);
UINT _gxe_window_client_height_get(GX_WINDOW *window, GX_VALUE *return_height);
UINT _gxe_window_client_scroll(GX_WINDOW *window, GX_VALUE xscroll, GX_VALUE yscroll);
UINT _gxe_window_client_width_get(GX_WINDOW *window, GX_VALUE *return_width);
UINT _gxe_window_close(GX_WINDOW *window);
UINT _gxe_window_create(GX_WINDOW *window, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                        ULONG style, USHORT Id, GX_CONST GX_RECTANGLE *size, UINT window_control_block_size);
VOID _gx_window_draw(GX_WINDOW *window);
UINT _gxe_window_event_process(GX_WINDOW *window, GX_EVENT *event_ptr);
UINT _gxe_window_execute(GX_WINDOW *window, ULONG *return_ptr);
UINT _gxe_window_root_create(GX_WINDOW_ROOT *root_window, GX_CONST GX_CHAR *name, GX_CANVAS *canvas,
                             ULONG style, USHORT Id, GX_CONST GX_RECTANGLE *size, UINT root_window_control_block_size);
UINT _gxe_window_root_delete(GX_WINDOW_ROOT *root_window);
UINT _gxe_window_root_event_process(GX_WINDOW_ROOT *root, GX_EVENT *event_ptr);
UINT _gxe_window_root_find(GX_WIDGET *widget, GX_WINDOW_ROOT **return_root_window);
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
#ifdef  GX_STANDALONE_BINDING
#include <string.h>
#include <stdlib.h>
#include "gx_system_standalone_bind.h"
#else
/* pull in custom rtos porting header */
#include <string.h>
#include <stdlib.h>
#include "gx_system_rtos_bind.h"
#endif
#endif

#endif

