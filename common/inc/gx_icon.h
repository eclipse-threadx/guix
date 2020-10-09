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
/**   Icon Management (Icon)                                              */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/
/*                                                                        */
/*  COMPONENT DEFINITION                                   RELEASE        */
/*                                                                        */
/*    gx_icon.h                                           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file defines the GUIX icon management component,               */
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

#ifndef GX_ICON_H
#define GX_ICON_H


/* Define button management function prototypes.  */

VOID _gx_icon_background_draw(GX_ICON *icon);
UINT _gx_icon_create(GX_ICON *icon, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                     GX_RESOURCE_ID pixelmap_id,
                     ULONG style, USHORT icon_id, GX_VALUE x, GX_VALUE y);
VOID _gx_icon_draw(GX_ICON *icon);
UINT _gx_icon_event_process(GX_ICON *icon, GX_EVENT *event_ptr);

UINT _gx_icon_pixelmap_set(GX_ICON *icon, GX_RESOURCE_ID normal_id, GX_RESOURCE_ID selected_id);
VOID _gx_icon_pixelmap_update(GX_ICON *icon);

/* Define error checking shells for API services.  These are only referenced by the
   application.  */


UINT _gxe_icon_create(GX_ICON *icon, GX_CONST GX_CHAR *name, GX_WIDGET *parent, GX_RESOURCE_ID pixelmap_id,
                      ULONG style, USHORT icon_id, GX_VALUE x, GX_VALUE y, UINT icon_control_block_size);
UINT _gxe_icon_event_process(GX_ICON *button, GX_EVENT *event_ptr);
UINT _gxe_icon_pixelmap_set(GX_ICON *icon, GX_RESOURCE_ID normal_id, GX_RESOURCE_ID selected_id);

#endif

