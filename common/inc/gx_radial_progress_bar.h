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
/**   Progress Bar Management (Radial Progress Bar)                       */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/
/*                                                                        */
/*  COMPONENT DEFINITION                                   RELEASE        */
/*                                                                        */
/*    gx_radial_progress_bar.h                            PORTABLE C      */
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

#ifndef GX_RADIAL_PROGRESS_BAR_H
#define GX_RADIAL_PROGRESS_BAR_H


/* Define radial progress bar management function prototypes.  */

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
UINT _gx_radial_progress_bar_resize(GX_RADIAL_PROGRESS_BAR *radial_progress);
UINT _gx_radial_progress_bar_size_update(GX_RADIAL_PROGRESS_BAR *bar);
UINT _gx_radial_progress_bar_text_color_set(GX_RADIAL_PROGRESS_BAR *progress_bar,
                                            GX_RESOURCE_ID normal_text_color_id,
                                            GX_RESOURCE_ID selected_text_color_id,
                                            GX_RESOURCE_ID disabled_text_color_id);
VOID _gx_radial_progress_bar_text_draw(GX_RADIAL_PROGRESS_BAR *bar);
UINT _gx_radial_progress_bar_value_calculate(GX_RADIAL_PROGRESS_BAR *progress_bar, GX_POINT new_position);
UINT _gx_radial_progress_bar_value_set(GX_RADIAL_PROGRESS_BAR *progress_bar, GX_VALUE new_value);


/* Define error checking shells for API services.  These are only referenced by the
   application.  */

UINT _gxe_radial_progress_bar_anchor_set(GX_RADIAL_PROGRESS_BAR *progress_bar, GX_VALUE angle);
UINT _gxe_radial_progress_bar_create(GX_RADIAL_PROGRESS_BAR *progress_bar,
                                     GX_CONST GX_CHAR *name,
                                     GX_WIDGET *parent,
                                     GX_RADIAL_PROGRESS_BAR_INFO *progress_bar_info,
                                     ULONG style,
                                     USHORT progress_bar_id,
                                     UINT progress_bar_control_block_size);
UINT _gxe_radial_progress_bar_event_process(GX_RADIAL_PROGRESS_BAR *progress_bar, GX_EVENT *event_ptr);
UINT _gxe_radial_progress_bar_font_set(GX_RADIAL_PROGRESS_BAR *progress_bar, GX_RESOURCE_ID font_id);
UINT _gxe_radial_progress_bar_text_color_set(GX_RADIAL_PROGRESS_BAR *progress_bar,
                                             GX_RESOURCE_ID normal_text_color_id,
                                             GX_RESOURCE_ID selected_text_color_id,
                                             GX_RESOURCE_ID disabled_text_color_id);
UINT _gxe_radial_progress_bar_info_set(GX_RADIAL_PROGRESS_BAR *bar, GX_RADIAL_PROGRESS_BAR_INFO *info);
UINT _gxe_radial_progress_bar_value_set(GX_RADIAL_PROGRESS_BAR *progress_bar, GX_VALUE new_value);


#endif

