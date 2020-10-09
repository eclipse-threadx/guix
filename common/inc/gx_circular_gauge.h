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
/**   Circular Gauge Management (Circular Gauge)                          */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/
/*                                                                        */
/*  COMPONENT DEFINITION                                   RELEASE        */
/*                                                                        */
/*    gx_circular_gauge.h                                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file defines the GUIX circular gauge widget,                   */
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

#ifndef GX_CIRCULAR_GAUGE_H
#define GX_CIRCULAR_GAUGE_H


/* Define prompt management function prototypes.  */

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
UINT _gx_circular_gauge_needle_rectangle_calculate(GX_CIRCULAR_GAUGE *gauge, INT angle, GX_RECTANGLE *rect);
UINT _gx_circular_gauge_needle_rotate(GX_CIRCULAR_GAUGE *circular_gauge);
UINT _gx_circular_gauge_needle_rotate_callback(VOID *gauge);
UINT _gx_circular_gauge_angle_increment_calculate(GX_CIRCULAR_GAUGE *circular_gauge);
UINT _gx_circular_gauge_needle_dirty_mark(GX_CIRCULAR_GAUGE *circular_gauge);


/* Define error checking shells for API services.  These are only referenced by the
   application.  */
UINT _gxe_circular_gauge_angle_get(GX_CIRCULAR_GAUGE *circular_gauge, INT *angle);
UINT _gxe_circular_gauge_angle_set(GX_CIRCULAR_GAUGE *circular_gauge, INT angle);
UINT _gxe_circular_gauge_animation_set(GX_CIRCULAR_GAUGE *circular_gauge, INT steps, INT delay);
UINT _gxe_circular_gauge_create(GX_CIRCULAR_GAUGE *circular_gauge,
                                GX_CONST GX_CHAR *name,
                                GX_WIDGET *parent,
                                GX_CIRCULAR_GAUGE_INFO *circular_gauge_info,
                                GX_RESOURCE_ID background,
                                ULONG style,
                                USHORT circular_gauge_id,
                                GX_VALUE xpos, GX_VALUE ypos, UINT checkbox_control_block_size);
UINT _gxe_circular_gauge_event_process(GX_CIRCULAR_GAUGE *circular_gauge, GX_EVENT *event_ptr);
#endif

