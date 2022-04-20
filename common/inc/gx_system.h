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
/**************************************************************************/


/**************************************************************************/
/*                                                                        */
/*  COMPONENT DEFINITION                                   RELEASE        */
/*                                                                        */
/*    gx_system.h                                         PORTABLE C      */
/*                                                           6.1.11       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file defines the GUIX system management component,             */
/*    including all data types and external references.  It is assumed    */
/*    that gx_api.h and gx_port.h have already been included.             */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            removed private string      */
/*                                            delete declaration,         */
/*                                            resulting in version 6.1    */
/*  12-31-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            added GX_DISABLE_THREADX_   */
/*                                            TIMER_SOURCE configuration, */
/*                                            resulting in version 6.1.3  */
/*  04-25-2022     Ting Zhu                 Modified comment(s),          */
/*                                            added new parameter to      */
/*                                            _gx_system_pen_flick_test,  */
/*                                            resulting in version 6.1.11 */
/*                                                                        */
/**************************************************************************/

#ifndef GX_SYSTEM_H
#define GX_SYSTEM_H


/* Define system management function prototypes.  */

UINT _gx_system_active_language_set(GX_UBYTE language);
VOID _gx_system_all_canvas_dirty(VOID);
VOID _gx_system_all_views_free(GX_WINDOW_ROOT *root);

#if (GX_ANIMATION_POOL_SIZE > 0)
UINT _gx_system_animation_get(GX_ANIMATION **animation);
UINT _gx_system_animation_free(GX_ANIMATION *animation);
#endif

#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
UINT _gx_system_bidi_text_enable(VOID);
UINT _gx_system_bidi_text_disable(VOID);
#endif

#if defined(GX_THAI_GLYPH_SHAPING_SUPPORT)
UINT _gx_system_text_render_style_set(GX_UBYTE style);
#endif

UINT     _gx_system_canvas_refresh(VOID);

UINT     _gx_system_clipboard_put(VOID *data, UINT data_size);
UINT     _gx_system_clipboard_get(VOID **data, UINT *data_size);

UINT     _gx_system_dirty_mark(GX_WIDGET *widget);
UINT     _gx_system_dirty_list_trim(GX_RECTANGLE *dirty_area, GX_WINDOW_ROOT *root);
VOID     _gx_system_dirty_list_remove(GX_WIDGET *remove);
UINT     _gx_system_dirty_partial_add(GX_WIDGET *widget, GX_RECTANGLE *dirty_area);
VOID     _gx_system_dirty_entry_shift(GX_WIDGET *widget, INT xShift, INT yShift);
UINT     _gx_system_draw_context_get(GX_DRAW_CONTEXT **current_context);

VOID     _gx_system_error_process(UINT error_code);
UINT     _gx_system_event_dispatch(GX_EVENT *new_event);
UINT     _gx_system_event_fold(GX_EVENT *in_event);
VOID     _gx_system_event_remove(GX_WIDGET *widget);
UINT     _gx_system_event_send(GX_EVENT *in_event);
UINT     _gx_system_focus_claim(GX_WIDGET *widget);
GX_VIEW *_gx_system_free_view_get(VOID);

UINT     _gx_system_initialize(VOID);

UINT     _gx_system_input_capture(GX_WIDGET *owner);
UINT     _gx_system_input_release(GX_WIDGET *owner);

#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gx_system_language_table_get(GX_CHAR ****language_table, GX_UBYTE *language_count, UINT *string_count);
UINT _gx_system_language_table_set(GX_CHAR ***language_table, GX_UBYTE number_of_langauges, UINT number_of_strings);
#endif

VOID _gx_system_lock(VOID);
UINT _gx_system_memory_allocator_set(VOID *(*allocate)(ULONG size), VOID (*release)(VOID *));

VOID _gx_system_unlock(VOID);
VOID _gx_system_input_lock(VOID);
VOID _gx_system_input_unlock(VOID);

VOID _gx_system_pen_speed_init(GX_POINT *current);
VOID _gx_system_pen_speed_update(GX_POINT *current);
VOID _gx_system_pen_flick_test(GX_WIDGET *target);
UINT _gx_system_pen_configure(GX_PEN_CONFIGURATION *pen_configuration);

UINT _gx_system_private_string_copy(GX_STRING *ptr_address, GX_CONST GX_STRING *text);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gx_system_private_string_list_copy(GX_CONST GX_CHAR ***ptr_address, GX_CONST GX_CHAR **string_list, INT string_count);
VOID _gx_system_private_string_list_get(GX_CONST GX_CHAR **input, GX_CONST GX_CHAR ***output, ULONG style);
#endif
UINT _gx_system_private_string_list_copy_ext(GX_STRING **ptr_address, USHORT *buffer_size, GX_CONST GX_STRING *string_list, INT string_count);
VOID _gx_system_private_string_get(GX_CONST GX_STRING *input, GX_STRING *output, ULONG style);

VOID _gx_system_root_view_add(GX_WINDOW_ROOT *root, GX_RECTANGLE *inrect);
UINT _gx_system_screen_stack_create(GX_WIDGET **memory, INT size);
UINT _gx_system_screen_stack_pop(VOID);
UINT _gx_system_screen_stack_push(GX_WIDGET *screen);
UINT _gx_system_screen_stack_get(GX_WIDGET **popped_parent, GX_WIDGET **popped_screen);
UINT _gx_system_screen_stack_reset(VOID);
UINT _gx_system_scroll_appearance_get(ULONG type,  GX_SCROLLBAR_APPEARANCE *save);
UINT _gx_system_scroll_appearance_set(ULONG style, GX_SCROLLBAR_APPEARANCE *appearance);
UINT _gx_system_start(VOID);

#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gx_system_string_get(GX_RESOURCE_ID string_id, GX_CONST GX_CHAR **return_string);
UINT _gx_system_string_table_get(GX_UBYTE langauge, GX_CHAR ***put_table, UINT *put_size);
UINT _gx_system_string_width_get(GX_CONST GX_FONT *font, GX_CONST GX_CHAR *string, INT string_length, GX_VALUE *return_width);
#endif

UINT            _gx_system_string_width_get_ext(GX_CONST GX_FONT *font, GX_CONST GX_STRING *string, GX_VALUE *return_width);

VOID            _gx_system_thread_entry(ULONG id);
VOID            _gx_system_timer_expiration(ULONG val);
UINT            _gx_system_timer_start(GX_WIDGET *owner, UINT timer_id, UINT initial_ticks, UINT reschedule_ticks);
UINT            _gx_system_timer_stop(GX_WIDGET *owner, UINT timer_id);
VOID            _gx_system_timer_update(ULONG ticks);
GX_WINDOW_ROOT *_gx_system_top_root_find(GX_EVENT *in_event);
GX_WIDGET      *_gx_system_top_widget_find(GX_WIDGET *root, GX_POINT test_point, ULONG status);

#ifdef GX_ENABLE_DEPRECATED_STRING_API
UINT    _gx_system_version_string_get(GX_CHAR **return_string);
#endif
UINT    _gx_system_version_string_get_ext(GX_STRING *return_string);

VOID    _gx_system_view_add(GX_WINDOW *win, GX_RECTANGLE *view);
GX_BOOL _gx_system_view_fold(GX_WINDOW *win, GX_RECTANGLE *view);
VOID    _gx_system_views_free(GX_VIEW *head);
VOID    _gx_system_view_split(GX_WINDOW *over, GX_WINDOW_ROOT *root, GX_RECTANGLE *original);
VOID    _gx_system_views_update(GX_WINDOW_ROOT *root);
UINT    _gx_system_widget_find(USHORT widget_id, INT search_level, GX_WIDGET **return_search_result);


/* Define error checking shells for API services.  These are only referenced by the
   application.  */

UINT _gxe_system_active_language_set(GX_UBYTE language);

#if (GX_ANIMATION_POOL_SIZE > 0)
UINT _gxe_system_animation_get(GX_ANIMATION **animation);
UINT _gxe_system_animation_free(GX_ANIMATION *animation);
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
UINT _gxe_system_language_table_set(GX_CHAR ***string_table, GX_UBYTE language_count, UINT number_of_strings);
#endif
UINT _gxe_system_memory_allocator_set(VOID *(*allocate)(ULONG size), VOID (*release)(VOID *));
UINT _gxe_system_pen_configure(GX_PEN_CONFIGURATION *pen_configuration);
UINT _gxe_system_screen_stack_create(GX_WIDGET **memory, INT size, UINT control_block_size);
UINT _gxe_system_screen_stack_get(GX_WIDGET **popped_parent, GX_WIDGET **popped_screen);
UINT _gxe_system_screen_stack_pop(VOID);
UINT _gxe_system_screen_stack_push(GX_WIDGET *screen);
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

/* System management component data declarations follow.  */

/* Determine if the initialization function of this component is including
   this file.  If so, make the data definitions really happen.  Otherwise,
   make them extern so other functions in the component can access them.  */

#ifdef GX_SYSTEM_INIT
#define SYSTEM_DECLARE
#else
#define SYSTEM_DECLARE           extern
#endif

#define GX_VERSION_STRING_LENGTH 16

/* Define string for holding system version information */
SYSTEM_DECLARE  GX_CHAR _gx_system_version_string[GX_VERSION_STRING_LENGTH];

/* internal structure for implementing gx_system_timer API. */
typedef struct GX_TIMER_STRUCT
{
    UINT                    gx_timer_initial_ticks;
    UINT                    gx_timer_reschedule_ticks;
    UINT                    gx_timer_id;
    GX_WIDGET              *gx_timer_owner;
    struct GX_TIMER_STRUCT *gx_timer_next;
} GX_TIMER;

/* Define touch configuration information. */
SYSTEM_DECLARE GX_PEN_CONFIGURATION _gx_system_pen_configuration;

/* Define the created display list head pointer.  */

SYSTEM_DECLARE  GX_DISPLAY *_gx_system_display_created_list;

/* Define the number of created displays.  */

SYSTEM_DECLARE  ULONG _gx_system_display_created_count;

/* Define the created canvas list head pointer.  */

SYSTEM_DECLARE  GX_CANVAS *_gx_system_canvas_created_list;

/* Define the number of created canvases.  */

SYSTEM_DECLARE  ULONG _gx_system_canvas_created_count;

/* Define the root window list head pointer.  */

SYSTEM_DECLARE  GX_WINDOW_ROOT *_gx_system_root_window_created_list;

/* define memory for holding nested drawing context */
SYSTEM_DECLARE  GX_DRAW_CONTEXT _gx_system_draw_context_stack[GX_MAX_CONTEXT_NESTING];

/* define pointer to the current active drawing context */
SYSTEM_DECLARE  GX_DRAW_CONTEXT *_gx_system_current_draw_context;

/* define pointer to the end of the draw context stack */
SYSTEM_DECLARE GX_DRAW_CONTEXT *_gx_system_draw_context_stack_end;

/* define storage for GX_TIMERs */

SYSTEM_DECLARE GX_TIMER _gx_system_timer_memory[GX_MAX_ACTIVE_TIMERS];

/* linked list of running timers */
SYSTEM_DECLARE GX_TIMER *_gx_system_active_timer_list;

/* linked list of free timers */
SYSTEM_DECLARE GX_TIMER *_gx_system_free_timer_list;

/* define scratchpad area for storing line end points
   used by wide line, circle, and arc functions */

/* Scratch area. */
SYSTEM_DECLARE INT _gx_system_scratchpad[GX_MAX_DISPLAY_HEIGHT * 2];

/* Define memory for holding GX_ANIMATION structure pool */
#if (GX_ANIMATION_POOL_SIZE > 0)
SYSTEM_DECLARE GX_ANIMATION  _gx_system_animation_pool[GX_ANIMATION_POOL_SIZE];
SYSTEM_DECLARE GX_ANIMATION *_gx_system_animation_free_list;
#endif

/* linked list of active animations */
SYSTEM_DECLARE GX_ANIMATION *_gx_system_animation_list;

/* linked list of created gradients */
SYSTEM_DECLARE GX_GRADIENT *_gx_system_gradient_list;

/* application defined memory allocation function */
SYSTEM_DECLARE VOID * (*_gx_system_memory_allocator)(ULONG);

/* application defined memory free function */
SYSTEM_DECLARE VOID (*_gx_system_memory_free)(VOID *);

/* storage for the system views */

SYSTEM_DECLARE GX_VIEW _gx_system_view_memory[GX_MAX_VIEWS];

/* linked list of free GX_VIEWS */
SYSTEM_DECLARE GX_VIEW *_gx_system_free_views;

/* Define pointer to widget that owns input focus */
SYSTEM_DECLARE  GX_WIDGET *_gx_system_focus_owner;

/* storage for default vertical scrollbar appearance */
SYSTEM_DECLARE ULONG                   _gx_system_vertical_scrollbar_style;
SYSTEM_DECLARE GX_SCROLLBAR_APPEARANCE _gx_system_vertical_scrollbar_appearance;

/* storage for default horizontal scrollbar appearance */
SYSTEM_DECLARE ULONG                   _gx_system_horizontal_scrollbar_style;
SYSTEM_DECLARE GX_SCROLLBAR_APPEARANCE _gx_system_horizontal_scrollbar_appearance;

/* stack of input owners */

SYSTEM_DECLARE  GX_WIDGET *_gx_system_input_capture_stack[GX_MAX_INPUT_CAPTURE_NESTING];
SYSTEM_DECLARE  GX_WIDGET *_gx_system_input_owner;
SYSTEM_DECLARE  INT        _gx_system_capture_count;

/* stack of rich text context. */
SYSTEM_DECLARE  GX_RICH_TEXT_CONTEXT_STACK  _gx_system_rich_text_context_stack;

/* temporary stack used to save rich text context stack. */
SYSTEM_DECLARE  GX_RICH_TEXT_CONTEXT_STACK  _gx_system_rich_text_context_stack_save;

#ifdef GX_THREADX_BINDING

/* guix system thread control block */
SYSTEM_DECLARE  TX_THREAD _gx_system_thread;

/* guix system thread stack */
SYSTEM_DECLARE GX_UBYTE _gx_system_thread_stack[GX_THREAD_STACK_SIZE];

/* Keep track of which thread has GUIX locked */

SYSTEM_DECLARE  TX_THREAD *_gx_system_lock_thread;

/* Keep track of which thread has block input */

SYSTEM_DECLARE  TX_THREAD *_gx_system_input_lock_thread;

/* Define the system event queue.  */

SYSTEM_DECLARE  TX_QUEUE _gx_system_event_queue;

/* Define the system protection mutex.  */

SYSTEM_DECLARE  TX_MUTEX _gx_system_protect;

/* Define the system input lock mutex. */

SYSTEM_DECLARE TX_MUTEX _gx_system_input_protect;
/* Define the system timer.  */

#ifndef GX_DISABLE_THREADX_TIMER_SOURCE
SYSTEM_DECLARE  TX_TIMER _gx_system_timer;
#endif

/* Define the memory area for the system event queue.  */

SYSTEM_DECLARE  GX_UBYTE _gx_system_event_queue_memory[GX_MAX_QUEUE_EVENTS * GX_EVENT_SIZE];

#define         GX_CURRENT_THREAD _tx_thread_identify()

#else

SYSTEM_DECLARE  VOID *_gx_system_lock_thread;

#endif  /* GX_THREADX_BINDING */


/* Define the system mutex lock count.  */

SYSTEM_DECLARE  UINT _gx_system_lock_nesting;

/* Define the system input lock count. */

SYSTEM_DECLARE UINT _gx_system_input_lock_nesting;

/* Define variables for tracking pen speed */
SYSTEM_DECLARE ULONG    _gx_system_last_clock;
SYSTEM_DECLARE LONG     _gx_system_clock_delta;
SYSTEM_DECLARE GX_POINT _gx_system_last_pen_point;
SYSTEM_DECLARE INT      _gx_system_pen_speed_x;
SYSTEM_DECLARE INT      _gx_system_pen_speed_y;

#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
/* Define text style. */
SYSTEM_DECLARE GX_BOOL _gx_system_bidi_text_enabled;
#endif

#if defined(GX_THAI_GLYPH_SHAPING_SUPPORT)
SYSTEM_DECLARE GX_UBYTE _gx_system_text_render_style;
#endif

/* Define the theme table */
SYSTEM_DECLARE GX_CONST GX_THEME **_gx_system_theme_table;

/* Define the theme table size.  */
SYSTEM_DECLARE  GX_RESOURCE_ID _gx_system_theme_table_size;

/* Define the active theme.  */
SYSTEM_DECLARE GX_RESOURCE_ID _gx_system_active_theme;

/* Define the storage for the last system error code.  */

SYSTEM_DECLARE  UINT _gx_system_last_error;

/* Define the number of system errors.  */

SYSTEM_DECLARE  ULONG                  _gx_system_error_count;

SYSTEM_DECLARE VOID                   *_gx_system_clipboard;
SYSTEM_DECLARE UINT                    _gx_system_clipboard_size;

SYSTEM_DECLARE GX_SCREEN_STACK_CONTROL _gx_system_screen_stack;

#endif

