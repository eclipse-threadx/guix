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
/**   System Management (System)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Locate GUIX system data in this file.  */

#define GX_SYSTEM_INIT


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_utility.h"
#include "gx_animation.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_system_initialize                               PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service initializes GUIX.                                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    memset                                Clear various data structures */
/*    _gx_system_error_process              Process system errors         */
/*    _gx_utility_ltoa                      Int to ASCII conversion       */
/*    _gx_utility_string_length_check       Test string length            */
/*    tx_timer_create                       Create ThreadX Timer          */
/*    tx_mutex_create                       Create system protection mutex*/
/*    tx_queue_create                       Create system event queue     */
/*    tx_queue_delete                       Delete system event queue     */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  12-31-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            added GX_DISABLE_THREADX_   */
/*                                            TIMER_SOURCE configuration, */
/*                                            resulting in version 6.1.3  */
/*                                                                        */
/**************************************************************************/
UINT  _gx_system_initialize(VOID)
{
#ifndef TX_DISABLE_ERROR_CHECKING
UINT status;
#endif
UINT index;
UINT length;

    /* Set linked list pointers to NULL.  */

    _gx_system_display_created_list =               GX_NULL;
    _gx_system_canvas_created_list =                GX_NULL;
    _gx_system_root_window_created_list =           GX_NULL;
    _gx_system_focus_owner =                        GX_NULL;

    /* Clear all the created counters.  */
    _gx_system_display_created_count =              0;
    _gx_system_canvas_created_count =               0;

    /* initialize the draw context stack */
    _gx_system_draw_context_stack_end = _gx_system_draw_context_stack;
    _gx_system_draw_context_stack_end += GX_MAX_CONTEXT_NESTING;
    _gx_system_current_draw_context = GX_NULL;

    /* initialize gradient list */
    _gx_system_gradient_list = GX_NULL;

    /* initialize the input capture stack */
    memset(_gx_system_input_capture_stack, 0, sizeof(GX_WIDGET *) * GX_MAX_INPUT_CAPTURE_NESTING);
    _gx_system_capture_count = 0;
    _gx_system_input_owner = GX_NULL;

    /* initialize rich text context stack. */
    _gx_system_rich_text_context_stack.gx_rich_text_context_stack_top = 0;
    _gx_system_rich_text_context_stack_save.gx_rich_text_context_stack_top = 0;

    /* initialize the lock count to 0 */
    _gx_system_lock_nesting =                       0;

    /* no thread has GUIX locked */

    _gx_system_lock_thread =                        GX_NULL;

    /* initialize default vertical scrollbar appearance */
    memset(&_gx_system_vertical_scrollbar_appearance, 0, sizeof(GX_SCROLLBAR_APPEARANCE));

    _gx_system_vertical_scrollbar_style = GX_SCROLLBAR_VERTICAL | GX_SCROLLBAR_RELATIVE_THUMB | GX_SCROLLBAR_END_BUTTONS;

    _gx_system_vertical_scrollbar_appearance.gx_scroll_width = 20;
    _gx_system_vertical_scrollbar_appearance.gx_scroll_thumb_width = 18;
    _gx_system_vertical_scrollbar_appearance.gx_scroll_thumb_color = GX_COLOR_ID_SCROLL_BUTTON;
    _gx_system_vertical_scrollbar_appearance.gx_scroll_thumb_border_color = GX_COLOR_ID_SCROLL_BUTTON;
    _gx_system_vertical_scrollbar_appearance.gx_scroll_button_color = GX_COLOR_ID_SCROLL_BUTTON;
    _gx_system_vertical_scrollbar_appearance.gx_scroll_thumb_travel_min = 20;
    _gx_system_vertical_scrollbar_appearance.gx_scroll_thumb_travel_max = 20;
    _gx_system_vertical_scrollbar_appearance.gx_scroll_thumb_border_style = GX_STYLE_BORDER_THIN;

    /* initialize default horizontal scrollbar appearance */
    _gx_system_horizontal_scrollbar_appearance = _gx_system_vertical_scrollbar_appearance;
    _gx_system_horizontal_scrollbar_style = GX_SCROLLBAR_HORIZONTAL | GX_SCROLLBAR_RELATIVE_THUMB | GX_SCROLLBAR_END_BUTTONS;

#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
    _gx_system_bidi_text_enabled = GX_FALSE;
#endif

#if defined(GX_THAI_GLYPH_SHAPING_SUPPORT)
    _gx_system_text_render_style = 0;
#endif

    /* initialize theme table information  */
    _gx_system_theme_table = GX_NULL;
    _gx_system_theme_table_size = 0;
    _gx_system_active_theme = 0;

    /* Initialize the version string */
    memset(_gx_system_version_string, 0, GX_VERSION_STRING_LENGTH);

    /* We are allocating 16 bytes for the version string. MAJOR_VERSION and MINOR_VERSION do not exceed 3 bytes each
       (version 999.999).  Therefore the code below will not exceed the 16 byte buffer. (8 + 3 + '.' + 3 + NULL). */
    length = sizeof("Version ") - 1;

    /* Copy "Version " to version string. */
    memcpy(_gx_system_version_string, "Version ", length); /* Use case of memcpy is verified. */

    index = length;

    /* Convert major version value to version string. */
    _gx_utility_ltoa(__GUIX_MAJOR_VERSION, _gx_system_version_string + index, GX_VERSION_STRING_LENGTH - index);

    /* Calculate major version string length. */
    _gx_utility_string_length_check(_gx_system_version_string + index, &length, GX_VERSION_STRING_LENGTH - index);

    index += length;
    _gx_system_version_string[index++] = '.';

    /* Convert minor version value to version string. */
    _gx_utility_ltoa(__GUIX_MINOR_VERSION, _gx_system_version_string + index, GX_VERSION_STRING_LENGTH - index);

    /* Initialize the system timers */

    memset(_gx_system_timer_memory, 0, sizeof(GX_TIMER) * GX_MAX_ACTIVE_TIMERS);
    _gx_system_free_timer_list = _gx_system_timer_memory;
    _gx_system_active_timer_list = GX_NULL;

    /* link all the timers in the timer free list */
    for (index = 0; index < GX_MAX_ACTIVE_TIMERS - 1; index++)
    {
        _gx_system_timer_memory[index].gx_timer_next = &_gx_system_timer_memory[index + 1];
    }

    /* initialize pen tracking variables */
    _gx_system_last_clock = 0;
    _gx_system_clock_delta = 0;
    _gx_system_last_pen_point.gx_point_x = 0;
    _gx_system_last_pen_point.gx_point_y = 0;
    _gx_system_pen_speed_x = 0;
    _gx_system_pen_speed_y = 0;

    /* initialize touch configuration information */
    _gx_system_pen_configuration.gx_pen_configuration_min_drag_dist = 80;
    _gx_system_pen_configuration.gx_pen_configuration_max_pen_speed_ticks = 10;

    /* Initialize free animation pool */
#if (GX_ANIMATION_POOL_SIZE > 0)
    /* link all of the animation blocks into free list */
    for (index = 0; index < GX_ANIMATION_POOL_SIZE - 1; index++)
    {
        _gx_animation_create(&_gx_system_animation_pool[index]);
        _gx_system_animation_pool[index].gx_animation_next = &_gx_system_animation_pool[index + 1];
    }
    _gx_system_animation_free_list = _gx_system_animation_pool;
#endif

    /* initialize the active animation list */
    _gx_system_animation_list = GX_NULL;

    /* initialize memory allocators */
    _gx_system_memory_allocator = GX_NULL;
    _gx_system_memory_free = GX_NULL;

    /* Initialize the viewport storage */

    _gx_system_free_views = _gx_system_view_memory;

    for (index = 0; index < GX_MAX_VIEWS - 1; index++)
    {
        _gx_system_free_views -> gx_view_next = (_gx_system_free_views + 1);
        _gx_system_free_views = _gx_system_free_views -> gx_view_next;
    }
    _gx_system_free_views -> gx_view_next = GX_NULL;
    _gx_system_free_views = _gx_system_view_memory;

    /* Initialize the system error information.  */
    _gx_system_last_error =                         GX_SUCCESS;
    _gx_system_error_count =                        0;

    _gx_system_clipboard = GX_NULL;
    _gx_system_clipboard_size = 0;

    /* Initiate system screen stack controler. */
    memset(&_gx_system_screen_stack, 0, sizeof(GX_SCREEN_STACK_CONTROL));

    /* Create the required ThreadX objects.  */

#ifdef GX_THREADX_BINDING
    /* First initialize all ThreadX objects to 0.  */
    memset(&_gx_system_event_queue, 0, sizeof(_gx_system_event_queue));
    memset(&_gx_system_protect, 0, sizeof(_gx_system_protect));

    /* Check the validity of the GX_EVENT_SIZE constant.  */
    if (GX_EVENT_SIZE < sizeof(GX_EVENT))
    {

        /* The constant GX_EVENT_SIZE is not large enough to hold a GX_EVENT type, which is a system
           error. GX_EVENT_SIZE must be redefined and the GUIX library must be rebuilt. */

        /* Call system error handler.  */
        _gx_system_error_process(GX_SYSTEM_EVENT_SIZE_MISMATCH);

        /* Return error!  */
        return(GX_SYSTEM_ERROR);
    }

#ifndef GX_DISABLE_THREADX_TIMER_SOURCE
    memset(&_gx_system_timer, 0, sizeof(_gx_system_timer));

    /* create the ThreadX timer that drives all GUIX timers */
    tx_timer_create(&_gx_system_timer, "guix timer", _gx_system_timer_expiration, 0,
                    GX_SYSTEM_TIMER_TICKS, GX_SYSTEM_TIMER_TICKS, TX_NO_ACTIVATE);
#endif

#ifdef TX_DISABLE_ERROR_CHECKING
    tx_queue_create(&_gx_system_event_queue, "GUIX System Event Queue", (GX_EVENT_SIZE / sizeof(ULONG)),
                    _gx_system_event_queue_memory, sizeof(_gx_system_event_queue_memory));
#else
    /* Create the system event queue.  */
    status = tx_queue_create(&_gx_system_event_queue, "GUIX System Event Queue", (GX_EVENT_SIZE / sizeof(ULONG)),
                             _gx_system_event_queue_memory, sizeof(_gx_system_event_queue_memory));

    /* Determine if the event queue creation was successful.  */
    if (status != TX_SUCCESS)
    {

        /* Queue create failed - call system error handler.  */
        _gx_system_error_process(GX_SYSTEM_QUEUE_CREATE_FAILED);

        /* Return error!  */
        return(GX_SYSTEM_ERROR);
    }
#endif

#ifdef TX_DISABLE_ERROR_CHECKING
    tx_mutex_create(&_gx_system_protect, "GUIX System Protection", TX_NO_INHERIT);
#else
    /* Create the system protection mutex.  */
    status =  tx_mutex_create(&_gx_system_protect, "GUIX System Protection", TX_NO_INHERIT);

    /* Determine if the mutex creation was successful.  */
    if (status != TX_SUCCESS)
    {

        /* Mutex create failed - call system error handler.  */
        _gx_system_error_process(GX_SYSTEM_MUTEX_CREATE_FAILED);

        /* Delete the event queue.  */
        tx_queue_delete(&_gx_system_event_queue);

        /* Return error!  */
        return(GX_SYSTEM_ERROR);
    }
#endif

    /* initialize GUIX thread here. It is started later by gx_system_start() */
    memset(&_gx_system_thread, 0, sizeof(_gx_system_thread));

#ifdef TX_DISABLE_ERROR_CHECKING
    tx_thread_create(&_gx_system_thread, "GUIX System Thread",
                     _gx_system_thread_entry, 0,
                     _gx_system_thread_stack, sizeof(_gx_system_thread_stack),
                     GX_SYSTEM_THREAD_PRIORITY, GX_SYSTEM_THREAD_PRIORITY,
                     GX_SYSTEM_THREAD_TIMESLICE, TX_DONT_START);
#else
    status =  tx_thread_create(&_gx_system_thread, "GUIX System Thread",
                               _gx_system_thread_entry, 0,
                               _gx_system_thread_stack, sizeof(_gx_system_thread_stack),
                               GX_SYSTEM_THREAD_PRIORITY, GX_SYSTEM_THREAD_PRIORITY,
                               GX_SYSTEM_THREAD_TIMESLICE, TX_DONT_START);

    if (status != TX_SUCCESS)
    {
        /* delete other resources */
        tx_queue_delete(&_gx_system_event_queue);

        /* Delete the mutex.  */
        tx_mutex_delete(&_gx_system_protect);

        /* thread create failed - call system error handler.  */
        _gx_system_error_process(GX_SYSTEM_THREAD_CREATE_FAILED);
        return(GX_SYSTEM_ERROR);
    }
#endif
#else
    GX_RTOS_BINDING_INITIALIZE;
#endif

    /* Return successful completion.  */
    return(GX_SUCCESS);
}

