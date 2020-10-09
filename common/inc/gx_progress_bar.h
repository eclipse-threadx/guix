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
/**   Progress Bar Management (Progress Bar)                              */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/
/*                                                                        */
/*  COMPONENT DEFINITION                                   RELEASE        */
/*                                                                        */
/*    gx_progress_bar.h                                   PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file defines the GUIX progress bar widget,                     */
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

#ifndef GX_PROGRESS_BAR_H
#define GX_PROGRESS_BAR_H


/* Define progress bar management function prototypes.  */

#define GX_SEGMENT_BORDER_INTERVAL 1
#define GX_SEGMENT_INTERVAL        1

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
VOID _gx_progress_bar_text_draw(GX_PROGRESS_BAR *progress_bar);
UINT _gx_progress_bar_value_set(GX_PROGRESS_BAR *progress_bar, INT new_value);

UINT _gxe_progress_bar_create(GX_PROGRESS_BAR *progress_bar, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                              GX_PROGRESS_BAR_INFO *progress_bar_info, ULONG style,
                              USHORT progress_bar_id, GX_CONST GX_RECTANGLE *size,
                              UINT progress_bar_control_block_size);

/* Define error checking shells for API services.  These are only referenced by the
   application.  */

UINT _gxe_progress_bar_event_process(GX_PROGRESS_BAR *progress_bar, GX_EVENT *event_ptr);
UINT _gxe_progress_bar_font_set(GX_PROGRESS_BAR *progress_bar, GX_RESOURCE_ID font_id);
UINT _gxe_progress_bar_info_set(GX_PROGRESS_BAR *progress_bar, GX_PROGRESS_BAR_INFO *info);
UINT _gxe_progress_bar_pixelmap_set(GX_PROGRESS_BAR *progress_bar, GX_RESOURCE_ID pixelmap);
UINT _gxe_progress_bar_range_set(GX_PROGRESS_BAR *progress_bar, INT min_value, INT max_value);
UINT _gxe_progress_bar_text_color_set(GX_PROGRESS_BAR *progress_bar, GX_RESOURCE_ID normal_text_color_id,
                                      GX_RESOURCE_ID selected_text_color_id, GX_RESOURCE_ID disabled_text_color_id);
UINT _gxe_progress_bar_value_set(GX_PROGRESS_BAR *progress_bar, INT new_value);

#endif

