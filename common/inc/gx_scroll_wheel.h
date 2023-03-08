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
/**   Scroll Wheel Management (Scroll Wheel)                              */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/
/*                                                                        */
/*  COMPONENT DEFINITION                                   RELEASE        */
/*                                                                        */
/*    gx_scroll_wheel.h                                   PORTABLE C      */
/*                                                           6.2.1        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file defines the GUIX scroll wheel management component,       */
/*    including all data types and external references.  It is assumed    */
/*    that gx_api.h and gx_port.h have already been included.             */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            added new prototypes,       */
/*                                            resulting in version 6.1    */
/*  06-02-2021     Ting Zhu                 Modified comment(s),          */
/*                                            added prototypes for        */
/*                                            generic scroll wheel,       */
/*                                            resulting in version 6.1.7  */
/*                                                                        */
/*  10-31-2022     Kenneth Maxwell          Modified comment(s),          */
/*                                            changed prototype for       */
/*                                            generic_scroll_wheel_scroll,*/
/*                                            resulting in version 6.2.0  */
/*  03-08-2023     Ting Zhu                 Modified comment(s),          */
/*                                            changed return type,        */
/*                                            resulting in version 6.2.1  */
/*                                                                        */
/**************************************************************************/

#ifndef GX_SCROLL_WHEEL_H
#define GX_SCROLL_WHEEL_H


#define GX_ANIMATION_TIMER 0x1100

/* Define scroll wheel management function prototypes.  */

UINT    _gx_numeric_scroll_wheel_create(GX_NUMERIC_SCROLL_WHEEL *wheel, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                                        INT start_val, INT end_val,
                                        ULONG style, USHORT Id, GX_CONST GX_RECTANGLE *size);
UINT    _gx_numeric_scroll_wheel_range_set(GX_NUMERIC_SCROLL_WHEEL *wheel, INT start_val, INT end_val);
UINT    _gx_numeric_scroll_wheel_text_get(GX_NUMERIC_SCROLL_WHEEL *wheel, INT row, GX_STRING *);

UINT    _gx_scroll_wheel_create(GX_SCROLL_WHEEL *wheel, GX_CONST GX_CHAR *name, GX_WIDGET *parent, INT total_rows,
                                ULONG style, USHORT Id, GX_CONST GX_RECTANGLE *size);
UINT    _gx_scroll_wheel_event_process(GX_SCROLL_WHEEL *wheel, GX_EVENT *event_ptr);
VOID    _gx_scroll_wheel_gradient_create(GX_SCROLL_WHEEL *wheel);
UINT    _gx_scroll_wheel_gradient_alpha_set(GX_SCROLL_WHEEL *wheel, GX_UBYTE start_alpha, GX_UBYTE end_alpha);
UINT    _gx_scroll_wheel_row_height_set(GX_SCROLL_WHEEL *wheel, GX_VALUE row_height);
UINT    _gx_scroll_wheel_selected_row_calculate(GX_SCROLL_WHEEL *wheel);
UINT    _gx_scroll_wheel_scroll(GX_SCROLL_WHEEL *wheel, GX_VALUE shift);
UINT    _gx_scroll_wheel_selected_background_set(GX_SCROLL_WHEEL *wheel, GX_RESOURCE_ID selected_bg);
UINT    _gx_scroll_wheel_selected_get(GX_SCROLL_WHEEL *wheel, INT *row);
UINT    _gx_scroll_wheel_selected_set(GX_SCROLL_WHEEL *wheel, INT row);
UINT    _gx_scroll_wheel_speed_set(GX_SCROLL_WHEEL *wheel, GX_FIXED_VAL start_speed_rate, GX_FIXED_VAL end_speed_rate,
                                   GX_VALUE max_steps, GX_VALUE delay);
UINT    _gx_scroll_wheel_total_rows_set(GX_SCROLL_WHEEL *wheel, INT total_rows);
GX_BOOL _gx_scroll_wheel_wrap_style_check(GX_SCROLL_WHEEL *wheel);

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
UINT _gx_string_scroll_wheel_text_get(GX_STRING_SCROLL_WHEEL *wheel, INT row, GX_STRING *string);

/* Define text scroll wheel management function prototypes.  */
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gx_text_scroll_wheel_callback_set(GX_TEXT_SCROLL_WHEEL * wheel, GX_CONST GX_CHAR * (*callback)(GX_TEXT_SCROLL_WHEEL *, INT));
#endif
#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
UINT _gx_text_scroll_wheel_dynamic_bidi_text_delete(GX_TEXT_SCROLL_WHEEL *wheel);
#endif
UINT _gx_text_scroll_wheel_event_process(GX_TEXT_SCROLL_WHEEL *wheel, GX_EVENT *event_ptr);
UINT _gx_text_scroll_wheel_callback_set_ext(GX_TEXT_SCROLL_WHEEL *wheel, UINT (*callback)(GX_TEXT_SCROLL_WHEEL *, INT, GX_STRING *));
UINT _gx_text_scroll_wheel_create(GX_TEXT_SCROLL_WHEEL *wheel, GX_CONST GX_CHAR *name, GX_WIDGET *parent, INT total_rows,
                                  ULONG style, USHORT Id, GX_CONST GX_RECTANGLE *size);
VOID _gx_text_scroll_wheel_draw(GX_TEXT_SCROLL_WHEEL *wheel);
UINT _gx_text_scroll_wheel_font_set(GX_TEXT_SCROLL_WHEEL *wheel, GX_RESOURCE_ID normal_font, GX_RESOURCE_ID selected_font);
UINT _gx_text_scroll_wheel_text_color_set(GX_TEXT_SCROLL_WHEEL *wheel,
                                          GX_RESOURCE_ID normal_text_color,
                                          GX_RESOURCE_ID selected_text_color,
                                          GX_RESOURCE_ID disabled_text_color);


/* Define generic scroll wheel management function prototypes.  */
UINT    _gx_generic_scroll_wheel_create(GX_GENERIC_SCROLL_WHEEL *wheel,
                                       GX_CONST GX_CHAR *name,
                                       GX_WIDGET *parent,
                                       INT total_rows,
                                       VOID (*callback)(GX_GENERIC_SCROLL_WHEEL *, GX_WIDGET *, INT),
                                       ULONG style,
                                       USHORT id,
                                       GX_CONST GX_RECTANGLE *size);
UINT    _gx_generic_scroll_wheel_children_position(GX_GENERIC_SCROLL_WHEEL *wheel);
VOID    _gx_generic_scroll_wheel_down_wrap(GX_GENERIC_SCROLL_WHEEL *wheel);
VOID    _gx_generic_scroll_wheel_draw(GX_GENERIC_SCROLL_WHEEL *wheel);
UINT    _gx_generic_scroll_wheel_event_process(GX_GENERIC_SCROLL_WHEEL *wheel, GX_EVENT *event_ptr);
UINT    _gx_generic_scroll_wheel_row_height_set(GX_GENERIC_SCROLL_WHEEL *wheel, GX_VALUE row_height);
UINT    _gx_generic_scroll_wheel_scroll(GX_GENERIC_SCROLL_WHEEL *wheel, GX_VALUE shift);
UINT    _gx_generic_scroll_wheel_total_rows_set(GX_GENERIC_SCROLL_WHEEL *wheel, INT count);
VOID    _gx_generic_scroll_wheel_up_wrap(GX_GENERIC_SCROLL_WHEEL *wheel);
GX_BOOL _gx_generic_scroll_wheel_wrap_style_check(GX_GENERIC_SCROLL_WHEEL *wheel);

/* Define error checking shells for API services.  These are only referenced by the
   application.  */

UINT _gxe_numeric_scroll_wheel_create(GX_NUMERIC_SCROLL_WHEEL *wheel, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                                      INT start_val, INT end_val,
                                      ULONG style, USHORT Id, GX_CONST GX_RECTANGLE *size, UINT control_block_size);
UINT _gxe_numeric_scroll_wheel_range_set(GX_NUMERIC_SCROLL_WHEEL *wheel, INT start_val, INT end_val);

UINT _gxe_scroll_wheel_create(GX_SCROLL_WHEEL *wheel, GX_CONST GX_CHAR *name, GX_WIDGET *parent, INT total_rows,
                              ULONG style, USHORT Id, GX_CONST GX_RECTANGLE *size, UINT control_block_size);
UINT _gxe_scroll_wheel_event_process(GX_SCROLL_WHEEL *wheel, GX_EVENT *event_ptr);
UINT _gxe_scroll_wheel_gradient_alpha_set(GX_SCROLL_WHEEL *wheel, GX_UBYTE start_alpha, GX_UBYTE end_alpha);
UINT _gxe_scroll_wheel_row_height_set(GX_SCROLL_WHEEL *wheel, GX_VALUE row_height);
UINT _gxe_scroll_wheel_selected_background_set(GX_SCROLL_WHEEL *wheel, GX_RESOURCE_ID selected_bg);
UINT _gxe_scroll_wheel_selected_get(GX_SCROLL_WHEEL *wheel, INT *row);
UINT _gxe_scroll_wheel_selected_set(GX_SCROLL_WHEEL *wheel, INT row);
UINT _gxe_scroll_wheel_speed_set(GX_SCROLL_WHEEL *wheel, GX_FIXED_VAL start_speed_rate, GX_FIXED_VAL end_speed_rate,
                                 GX_VALUE max_steps, GX_VALUE delay);
UINT _gxe_scroll_wheel_total_rows_set(GX_SCROLL_WHEEL *wheel, INT total_rows);


#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gxe_string_scroll_wheel_create(GX_STRING_SCROLL_WHEEL *wheel, GX_CONST GX_CHAR *name, GX_WIDGET *parent, INT total_rows,
                                     GX_CONST GX_CHAR **string_list,
                                     ULONG style, USHORT Id, GX_CONST GX_RECTANGLE *size, UINT control_block_size);
#endif
UINT _gxe_string_scroll_wheel_create_ext(GX_STRING_SCROLL_WHEEL *wheel, GX_CONST GX_CHAR *name, GX_WIDGET *parent, INT total_rows,
                                         GX_CONST GX_STRING *string_list,
                                         ULONG style, USHORT Id, GX_CONST GX_RECTANGLE *size, UINT control_block_size);
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

#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gxe_text_scroll_wheel_callback_set(GX_TEXT_SCROLL_WHEEL * wheel, GX_CONST GX_CHAR * (*callback)(GX_TEXT_SCROLL_WHEEL *, INT));
#endif
UINT _gxe_text_scroll_wheel_callback_set_ext(GX_TEXT_SCROLL_WHEEL *wheel, UINT (*callback)(GX_TEXT_SCROLL_WHEEL *, INT, GX_STRING *));
UINT _gxe_text_scroll_wheel_create(GX_TEXT_SCROLL_WHEEL *wheel, GX_CONST GX_CHAR *name, GX_WIDGET *parent, INT total_rows,
                                   ULONG style, USHORT Id, GX_CONST GX_RECTANGLE *size, UINT control_block_size);
UINT _gxe_text_scroll_wheel_event_process(GX_TEXT_SCROLL_WHEEL *wheel, GX_EVENT *event_ptr);
UINT _gxe_text_scroll_wheel_font_set(GX_TEXT_SCROLL_WHEEL *wheel, GX_RESOURCE_ID normal_font, GX_RESOURCE_ID selected_font);
UINT _gxe_text_scroll_wheel_text_color_set(GX_TEXT_SCROLL_WHEEL *wheel,
                                           GX_RESOURCE_ID normal_text_color,
                                           GX_RESOURCE_ID selected_text_color,
                                           GX_RESOURCE_ID disabled_text_color);

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

#endif

