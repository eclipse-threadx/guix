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
/**   Menu Management (Menu)                                              */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/
/*                                                                        */
/*  COMPONENT DEFINITION                                   RELEASE        */
/*                                                                        */
/*    gx_menu.h                                           PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file defines the GUIX menu component, including all data types */
/*    and external references.  It is assumed that gx_api.h and gx_port.h */
/*    have already been included.                                         */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  12-31-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            declare menu event process, */
/*                                            resulting in version 6.1.3  */
/*                                                                        */
/**************************************************************************/

#ifndef GX_MENU_H
#define GX_MENU_H

/* Define menu management function prototypes.  */

#define GX_MENU_ANIMATION_OPEN  0x01
#define GX_MENU_ANIMATION_CLOSE 0x02

#define GX_MENU_OPEN_TIMER      0xf0
#define GX_MENU_CLOSE_TIMER     0xf1

UINT _gx_accordion_menu_create(GX_ACCORDION_MENU *accordion, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                               ULONG style, USHORT accordion_menu_id, GX_CONST GX_RECTANGLE *size);
VOID _gx_accordion_menu_draw(GX_ACCORDION_MENU *menu);
UINT _gx_accordion_menu_event_process(GX_ACCORDION_MENU *accordion, GX_EVENT *event_ptr);
UINT _gx_accordion_menu_position(GX_ACCORDION_MENU *accordion);

UINT _gx_menu_create(GX_MENU *menu, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                     GX_RESOURCE_ID text_id, GX_RESOURCE_ID fill_id,
                     ULONG style, USHORT menu_id, GX_CONST GX_RECTANGLE *size);
VOID _gx_menu_draw(GX_MENU *menu);
UINT _gx_menu_event_process(GX_MENU *menu, GX_EVENT *event_ptr);
UINT _gx_menu_insert(GX_MENU *menu, GX_WIDGET *widget);
UINT _gx_menu_one_level_position(GX_MENU *menu, GX_VALUE indentation);
UINT _gx_menu_position(GX_MENU *menu, GX_VALUE indentation);
UINT _gx_menu_remove(GX_MENU *menu, GX_WIDGET *widget);
VOID _gx_menu_text_draw(GX_MENU *menu);
UINT _gx_menu_text_offset_set(GX_MENU *menu, GX_VALUE x_offset, GX_VALUE y_offset);

/* error checking versions of menu API functions.  */
UINT _gxe_accordion_menu_create(GX_ACCORDION_MENU *accordion, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                                ULONG style, USHORT accordion_menu_id, GX_CONST GX_RECTANGLE *size, UINT control_block_size);
UINT _gxe_accordion_menu_event_process(GX_ACCORDION_MENU *accordion, GX_EVENT *event_ptr);
UINT _gxe_accordion_menu_position(GX_ACCORDION_MENU *accordion);

UINT _gxe_menu_create(GX_MENU *menu, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                      GX_RESOURCE_ID text_id, GX_RESOURCE_ID fill_id,
                      ULONG style, USHORT menu_id, GX_CONST GX_RECTANGLE *size, UINT control_block_size);
UINT _gxe_menu_event_process(GX_MENU *menu, GX_EVENT *event_ptr);
UINT _gxe_menu_insert(GX_MENU *menu, GX_WIDGET *widget);
UINT _gxe_menu_remove(GX_MENU *menu, GX_WIDGET *widget);
UINT _gxe_menu_text_offset_set(GX_MENU *menu, GX_VALUE x_offset, GX_VALUE y_offset);

#endif

