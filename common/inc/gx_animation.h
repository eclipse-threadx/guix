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
/**   Animation Management (Animation)                                    */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/
/*                                                                        */
/*  COMPONENT DEFINITION                                   RELEASE        */
/*                                                                        */
/*    gx_animation.h                                      PORTABLE C      */
/*                                                           6.1.7        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file defines the GUIX animation component, including all data  */
/*    types and external references.  It is assumed that gx_api.h and     */
/*    gx_port.h have already been included.                               */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  06-02-2021     Ting Zhu                 Modified comment(s),          */
/*                                            added animation delete API, */
/*                                            resulting in version 6.1.7  */
/*                                                                        */
/**************************************************************************/

#ifndef GX_ANIMATION_H
#define GX_ANIMATION_H

#define GX_ANIMATION_SLIDE_LEFT       0x0001
#define GX_ANIMATION_SLIDE_RIGHT      0x0002
#define GX_ANIMATION_SLIDE_UP         0x0040
#define GX_ANIMATION_SLIDE_DOWN       0x0080

#define GX_ANIMATION_SLIDE_TIMER      1000
#define GX_ANIMATION_MIN_SLIDING_DIST 5

/* Define animation management function prototypes.  */

UINT _gx_animation_create(GX_ANIMATION *animation);
UINT _gx_animation_canvas_define(GX_ANIMATION *animation, GX_CANVAS *canvas);
VOID _gx_animation_complete_event_send(GX_ANIMATION *animation);
UINT _gx_animation_delete(GX_ANIMATION *target, GX_WIDGET *parent);
UINT _gx_animation_start(GX_ANIMATION *animation, GX_ANIMATION_INFO *info);
UINT _gx_animation_stop(GX_ANIMATION *animation);
VOID _gx_animation_update(VOID);
VOID _gx_animation_complete(GX_ANIMATION *animation);

UINT _gx_animation_drag_disable(GX_ANIMATION *animation, GX_WIDGET *widget);
UINT _gx_animation_drag_enable(GX_ANIMATION *animation, GX_WIDGET *widget, GX_ANIMATION_INFO *info);
UINT _gx_animation_drag_event_process(GX_WIDGET *widget, GX_EVENT *event_ptr);
UINT _gx_animation_drag_tracking(GX_ANIMATION *animation, GX_POINT penpos);
UINT _gx_animation_drag_tracking_start(GX_ANIMATION *animation, GX_POINT penpos);
UINT _gx_animation_slide_landing(GX_ANIMATION *animation);
UINT _gx_animation_landing_speed_set(GX_ANIMATION *animation, USHORT shift_per_step);
UINT _gx_animation_slide_landing_start(GX_ANIMATION *animation);

/* error checking versions of the animation API functions */
UINT _gxe_animation_create(GX_ANIMATION *animation);
UINT _gxe_animation_canvas_define(GX_ANIMATION *animation, GX_CANVAS *canvas);
UINT _gxe_animation_delete(GX_ANIMATION *target, GX_WIDGET *parent);
UINT _gxe_animation_drag_disable(GX_ANIMATION *animation, GX_WIDGET *widget);
UINT _gxe_animation_drag_enable(GX_ANIMATION *animation, GX_WIDGET *widget, GX_ANIMATION_INFO *info);
UINT _gxe_animation_start(GX_ANIMATION *animation, GX_ANIMATION_INFO *info);
UINT _gxe_animation_stop(GX_ANIMATION *animation);

UINT _gxe_animation_landing_speed_set(GX_ANIMATION *animation, USHORT shift_per_step);

#endif

