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
/**   Window Management (Window)                                          */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/
/*                                                                        */
/*  COMPONENT DEFINITION                                   RELEASE        */
/*                                                                        */
/*    gx_drop_list.h                                      PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file defines the GUIX window management component,             */
/*    including all data types and external references.  It is assumed    */
/*    that gx_api.h and gx_port.h have already been included.             */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/

#ifndef GX_DROP_LIST_H
#define GX_DROP_LIST_H

/* Define drop list management function prototypes. */
UINT _gx_drop_list_close(GX_DROP_LIST *drop_list);
UINT _gx_drop_list_create(GX_DROP_LIST *drop_list, GX_CONST GX_CHAR *name,
                          GX_WIDGET *parent, INT total_rows, INT open_height,
                          VOID (*callback)(GX_VERTICAL_LIST *, GX_WIDGET *, INT),
                          ULONG style, USHORT drop_list_id, GX_CONST GX_RECTANGLE *size);
VOID _gx_drop_list_draw(GX_DROP_LIST *box);
VOID _gx_drop_list_background_draw(GX_DROP_LIST *drop_list);
UINT _gx_drop_list_event_process(GX_DROP_LIST *widget, GX_EVENT *event_ptr);
UINT _gx_drop_list_open(GX_DROP_LIST *list);
UINT _gx_drop_list_pixelmap_set(GX_DROP_LIST *drop_list, GX_RESOURCE_ID pixelmap_id);
UINT _gx_drop_list_popup_get(GX_DROP_LIST *box, GX_VERTICAL_LIST **return_list);


UINT _gxe_drop_list_close(GX_DROP_LIST *drop_list);
UINT _gxe_drop_list_create(GX_DROP_LIST *drop_list, GX_CONST GX_CHAR *name,
                           GX_WIDGET *parent, INT total_rows, INT open_height,
                           VOID (*callback)(GX_VERTICAL_LIST *, GX_WIDGET *, INT),
                           ULONG style, USHORT drop_list_id, GX_CONST GX_RECTANGLE *size, UINT drop_list_control_block_size);
UINT _gxe_drop_list_event_process(GX_DROP_LIST *widget, GX_EVENT *event_ptr);
UINT _gxe_drop_list_open(GX_DROP_LIST *list);
UINT _gxe_drop_list_pixelmap_set(GX_DROP_LIST *drop_list, GX_RESOURCE_ID pixelmap_id);
UINT _gxe_drop_list_popup_get(GX_DROP_LIST *box, GX_VERTICAL_LIST **return_list);

#endif

