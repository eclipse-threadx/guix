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
/**   Button Management (Button)                                          */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/
/*                                                                        */
/*  COMPONENT DEFINITION                                   RELEASE        */
/*                                                                        */
/*    gx_button.h                                         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file defines the GUIX button management component,             */
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
/*                                                                        */
/**************************************************************************/

#ifndef GX_BUTTON_H
#define GX_BUTTON_H


/* Define button management function prototypes.  */

UINT _gx_button_create(GX_BUTTON *button, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                       ULONG style, USHORT Id, GX_CONST GX_RECTANGLE *size);
VOID _gx_button_background_draw(GX_BUTTON *button);
UINT _gx_button_deselect(GX_BUTTON *button, GX_BOOL generate_event);
VOID _gx_button_draw(GX_BUTTON *button);
UINT _gx_button_event_process(GX_BUTTON *button, GX_EVENT *event_ptr);
VOID _gx_button_siblings_deselect(GX_BUTTON *button);
UINT _gx_button_select(GX_BUTTON *button);

UINT _gx_checkbox_create(GX_CHECKBOX *checkbox, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                         GX_RESOURCE_ID text_id, ULONG style, USHORT checkbox_id,
                         GX_CONST GX_RECTANGLE *size);
VOID _gx_checkbox_draw(GX_CHECKBOX *checkbox);
UINT _gx_checkbox_event_process(GX_CHECKBOX *checkbox, GX_EVENT *event_ptr);
UINT _gx_checkbox_pixelmap_set(GX_CHECKBOX *button, GX_RESOURCE_ID unchecked_id, GX_RESOURCE_ID checked_id,
                               GX_RESOURCE_ID unchecked_disabled_id, GX_RESOURCE_ID checked_disabled_id);
UINT _gx_checkbox_select(GX_CHECKBOX *checkbox);

UINT _gx_icon_button_create(GX_ICON_BUTTON *button, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                            GX_RESOURCE_ID icon_id,
                            ULONG style, USHORT icon_button_id, GX_CONST GX_RECTANGLE *size);
VOID _gx_icon_button_draw(GX_ICON_BUTTON *button);
UINT _gx_icon_button_pixelmap_set(GX_ICON_BUTTON *button, GX_RESOURCE_ID icon_id);


UINT _gx_multi_line_text_button_create(GX_MULTI_LINE_TEXT_BUTTON *button, GX_CONST GX_CHAR *name,
                                       GX_WIDGET *parent, GX_RESOURCE_ID text_id, ULONG style, USHORT Id,
                                       GX_CONST GX_RECTANGLE *size);

VOID _gx_multi_line_text_button_draw(GX_MULTI_LINE_TEXT_BUTTON *button);
UINT _gx_multi_line_text_button_event_process(GX_MULTI_LINE_TEXT_BUTTON *button, GX_EVENT *event_ptr);
VOID _gx_multi_line_text_button_line_pointers_set(GX_MULTI_LINE_TEXT_BUTTON *button);
VOID _gx_multi_line_text_button_text_draw(GX_MULTI_LINE_TEXT_BUTTON *button);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gx_multi_line_text_button_text_set(GX_MULTI_LINE_TEXT_BUTTON *button, GX_CONST GX_CHAR *text);
#endif
UINT _gx_multi_line_text_button_text_set_ext(GX_MULTI_LINE_TEXT_BUTTON *button, GX_CONST GX_STRING *text);

UINT _gx_multi_line_text_button_text_id_set(GX_MULTI_LINE_TEXT_BUTTON *button, GX_RESOURCE_ID string_id);

UINT _gx_pixelmap_button_create(GX_PIXELMAP_BUTTON *button, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                                GX_RESOURCE_ID normal_id, GX_RESOURCE_ID selected_id, GX_RESOURCE_ID disabled_id,
                                ULONG style, USHORT pixelmap_button_id,
                                GX_CONST GX_RECTANGLE *size);
VOID _gx_pixelmap_button_draw(GX_PIXELMAP_BUTTON *button);
UINT _gx_pixelmap_button_event_process(GX_PIXELMAP_BUTTON *button, GX_EVENT *event_ptr);

UINT _gx_pixelmap_button_pixelmap_set(GX_PIXELMAP_BUTTON *button, GX_RESOURCE_ID normal_id, GX_RESOURCE_ID selected_id,
                                      GX_RESOURCE_ID disabled_id);
VOID _gx_pixelmap_button_transparent_detect(GX_PIXELMAP_BUTTON *button);

UINT _gx_radio_button_create(GX_RADIO_BUTTON *button, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                             GX_RESOURCE_ID text_id, ULONG style, USHORT radio_button_id,
                             GX_CONST GX_RECTANGLE *size);
VOID _gx_radio_button_draw(GX_RADIO_BUTTON *button);
UINT _gx_radio_button_pixelmap_set(GX_RADIO_BUTTON *button, GX_RESOURCE_ID off_id, GX_RESOURCE_ID on_id,
                                   GX_RESOURCE_ID off_disabled_id, GX_RESOURCE_ID on_disabled_id);

UINT _gx_text_button_create(GX_TEXT_BUTTON *button, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                            GX_RESOURCE_ID text_id, ULONG style, USHORT Id,
                            GX_CONST GX_RECTANGLE *size);
VOID _gx_text_button_draw(GX_TEXT_BUTTON *button);
UINT _gx_text_button_event_process(GX_TEXT_BUTTON *button, GX_EVENT *event_ptr);
VOID _gx_text_button_text_draw(GX_TEXT_BUTTON *button);
UINT _gx_text_button_text_color_set(GX_TEXT_BUTTON *text_button,
                                    GX_RESOURCE_ID normal_text_color_id,
                                    GX_RESOURCE_ID selected_text_color_id,
                                    GX_RESOURCE_ID disabled_text_color_id);
UINT _gx_text_button_font_set(GX_TEXT_BUTTON *button, GX_RESOURCE_ID font_id);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gx_text_button_text_get(GX_TEXT_BUTTON *button, GX_CONST GX_CHAR **return_text);
UINT _gx_text_button_text_set(GX_TEXT_BUTTON *button, GX_CONST GX_CHAR *text);
#endif
UINT _gx_text_button_text_get_ext(GX_TEXT_BUTTON *button, GX_STRING *return_text);
UINT _gx_text_button_text_set_ext(GX_TEXT_BUTTON *button, GX_CONST GX_STRING *text);
UINT _gx_text_button_text_id_set(GX_TEXT_BUTTON *button, GX_RESOURCE_ID string_id);

VOID _gx_monochrome_driver_disabled_button_line_draw(GX_BUTTON *button);

/* Define error checking shells for API services.  These are only referenced by the
   application.  */

UINT _gxe_button_create(GX_BUTTON *button, GX_CONST GX_CHAR *name, GX_WIDGET *parent, ULONG style,
                        USHORT Id, GX_CONST GX_RECTANGLE *size, UINT button_control_block_size);
UINT _gxe_button_deselect(GX_BUTTON *button, GX_BOOL gen_event);
UINT _gxe_button_event_process(GX_BUTTON *button, GX_EVENT *event_ptr);
UINT _gxe_button_select(GX_BUTTON *button);

UINT _gxe_checkbox_create(GX_CHECKBOX *checkbox, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                          GX_RESOURCE_ID text_id, ULONG style,
                          USHORT checkbox_id, GX_CONST GX_RECTANGLE *size, UINT checkbox_control_block_size);
UINT _gxe_checkbox_event_process(GX_CHECKBOX *checkbox, GX_EVENT *event_ptr);
UINT _gxe_checkbox_pixelmap_set(GX_CHECKBOX *checkbox,
                                GX_RESOURCE_ID unchecked_id,
                                GX_RESOURCE_ID checked_id,
                                GX_RESOURCE_ID unchecked_disabled_id,
                                GX_RESOURCE_ID checked_disabled_id);
UINT _gxe_checkbox_select(GX_CHECKBOX *checkbox);

UINT _gxe_icon_button_create(GX_ICON_BUTTON *button, GX_CONST GX_CHAR *name, GX_WIDGET *parent, GX_RESOURCE_ID icon_id,
                             ULONG style, USHORT icon_button_id, GX_CONST GX_RECTANGLE *size, UINT button_control_block_size);
UINT _gxe_icon_button_pixelmap_set(GX_ICON_BUTTON *button, GX_RESOURCE_ID pixelmap_id);


UINT _gxe_multi_line_text_button_create(GX_MULTI_LINE_TEXT_BUTTON *button, GX_CONST GX_CHAR *name,
                                        GX_WIDGET *parent, GX_RESOURCE_ID text_id, ULONG style, USHORT Id,
                                        GX_CONST GX_RECTANGLE *size, UINT text_button_control_block);

UINT _gxe_multi_line_text_button_event_process(GX_MULTI_LINE_TEXT_BUTTON *button, GX_EVENT *event_ptr);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gxe_multi_line_text_button_text_set(GX_MULTI_LINE_TEXT_BUTTON *button, GX_CONST GX_CHAR *text);
#endif
UINT _gxe_multi_line_text_button_text_set_ext(GX_MULTI_LINE_TEXT_BUTTON *button, GX_CONST GX_STRING *text);
UINT _gxe_multi_line_text_button_text_id_set(GX_MULTI_LINE_TEXT_BUTTON *button, GX_RESOURCE_ID string_id);

UINT _gxe_pixelmap_button_event_process(GX_PIXELMAP_BUTTON *button, GX_EVENT *event_ptr);
UINT _gxe_pixelmap_button_create(GX_PIXELMAP_BUTTON *button, GX_CONST GX_CHAR *name, GX_WIDGET *parent, GX_RESOURCE_ID normal_id,
                                 GX_RESOURCE_ID selected_id, GX_RESOURCE_ID disabled_id, ULONG style, USHORT pixelmap_button_id,
                                 GX_CONST GX_RECTANGLE *size, UINT button_control_block_size);
UINT _gxe_pixelmap_button_pixelmap_set(GX_PIXELMAP_BUTTON *button, GX_RESOURCE_ID normal_id, GX_RESOURCE_ID selected_id,
                                       GX_RESOURCE_ID disabled_id);

UINT _gxe_radio_button_create(GX_RADIO_BUTTON *button, GX_CONST GX_CHAR *name, GX_WIDGET *parent, GX_RESOURCE_ID text_id, ULONG style,
                              USHORT radio_button_id, GX_CONST GX_RECTANGLE *size, UINT radio_button_control_block_size);
UINT _gxe_radio_button_pixelmap_set(GX_RADIO_BUTTON *button, GX_RESOURCE_ID off_id, GX_RESOURCE_ID on_id, GX_RESOURCE_ID off_disabled_id,
                                    GX_RESOURCE_ID on_disabled_id);

UINT _gxe_text_button_create(GX_TEXT_BUTTON *button, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                             GX_RESOURCE_ID text_id, ULONG style, USHORT Id,
                             GX_CONST GX_RECTANGLE *size, UINT text_button_control_block);
UINT _gxe_text_button_event_process(GX_TEXT_BUTTON *button, GX_EVENT *event_ptr);
UINT _gxe_text_button_font_set(GX_TEXT_BUTTON *button, GX_RESOURCE_ID font_id);
UINT _gxe_text_button_text_color_set(GX_TEXT_BUTTON *text_button,
                                     GX_RESOURCE_ID normal_text_color_id,
                                     GX_RESOURCE_ID selected_text_color_id,
                                     GX_RESOURCE_ID disabled_text_color_id);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gxe_text_button_text_get(GX_TEXT_BUTTON *button, GX_CONST GX_CHAR **return_text);
UINT _gxe_text_button_text_set(GX_TEXT_BUTTON *button, GX_CONST GX_CHAR *text);
#endif
UINT _gxe_text_button_text_get_ext(GX_TEXT_BUTTON *button, GX_STRING *return_text);
UINT _gxe_text_button_text_set_ext(GX_TEXT_BUTTON *button, GX_CONST GX_STRING *text);
UINT _gxe_text_button_text_id_set(GX_TEXT_BUTTON *button, GX_RESOURCE_ID string_id);

#endif

