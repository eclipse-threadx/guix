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
/**   Radial Slider Management (Slider)                                   */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/
/*                                                                        */
/*  COMPONENT DEFINITION                                   RELEASE        */
/*                                                                        */
/*    gx_radial_slider.h                                  PORTABLE C      */
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

#ifndef GX_RADIAL_SLIDER_H
#define GX_RADIAL_SLIDER_H

/* Define radial slider management function prototypes.  */
UINT _gx_radial_slider_anchor_angle_calculate(GX_RADIAL_SLIDER *slider, GX_VALUE *target_value);
UINT _gx_radial_slider_anchor_angles_set(GX_RADIAL_SLIDER *slider, GX_VALUE *anchor_values, USHORT anchor_count);
UINT _gx_radial_slider_animation_set(GX_RADIAL_SLIDER *slider, USHORT steps, USHORT delay, USHORT animation_style,
                                     VOID (*animation_update_callback)(GX_RADIAL_SLIDER *slider));
UINT _gx_radial_slider_animation_start(GX_RADIAL_SLIDER *slider, GX_VALUE new_value);
UINT _gx_radial_slider_animation_update(GX_RADIAL_SLIDER *slider);
UINT _gx_radial_slider_create(GX_RADIAL_SLIDER *slider, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                              GX_RADIAL_SLIDER_INFO *info, ULONG style, USHORT slider_id, GX_CONST GX_RECTANGLE *size);
VOID _gx_radial_slider_draw(GX_RADIAL_SLIDER *slider);
UINT _gx_radial_slider_event_process(GX_RADIAL_SLIDER *slider, GX_EVENT *event_ptr);
UINT _gx_radial_slider_info_get(GX_RADIAL_SLIDER *slider, GX_RADIAL_SLIDER_INFO **info);
UINT _gx_radial_slider_info_set(GX_RADIAL_SLIDER *slider, GX_RADIAL_SLIDER_INFO *info);
UINT _gx_radial_slider_needle_rectangle_calculate(GX_RADIAL_SLIDER *slider, GX_RECTANGLE *rectangle);
UINT _gx_radial_slider_pixelmap_set(GX_RADIAL_SLIDER *slider, GX_RESOURCE_ID background_pixelmap, GX_RESOURCE_ID needle_pixelmap);
UINT _gx_radial_slider_angle_calculate(GX_RADIAL_SLIDER *slider, GX_POINT point, GX_VALUE *return_value);
UINT _gx_radial_slider_angle_set(GX_RADIAL_SLIDER *slider, GX_VALUE new_value);

/* Define error checking shells for API services.  These are only referenced by the
   application.  */
UINT _gxe_radial_slider_anchor_angles_set(GX_RADIAL_SLIDER *slider, GX_VALUE *anchor_values, USHORT anchor_count);
UINT _gxe_radial_slider_animation_set(GX_RADIAL_SLIDER *slider, USHORT steps, USHORT delay, USHORT animation_style,
                                      VOID (*animation_update_callback)(GX_RADIAL_SLIDER *slider));
UINT _gxe_radial_slider_animation_start(GX_RADIAL_SLIDER *slider, GX_VALUE new_value);
UINT _gxe_radial_slider_create(GX_RADIAL_SLIDER *slider, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                               GX_RADIAL_SLIDER_INFO *info, ULONG style, USHORT slider_id,
                               GX_CONST GX_RECTANGLE *size, UINT control_block_size);
UINT _gxe_radial_slider_event_process(GX_RADIAL_SLIDER *slider, GX_EVENT *event_ptr);
UINT _gxe_radial_slider_info_get(GX_RADIAL_SLIDER *slider, GX_RADIAL_SLIDER_INFO **info);
UINT _gxe_radial_slider_info_set(GX_RADIAL_SLIDER *slider, GX_RADIAL_SLIDER_INFO *info);
UINT _gxe_radial_slider_pixelmap_set(GX_RADIAL_SLIDER *slider, GX_RESOURCE_ID background_pixelmap, GX_RESOURCE_ID needle_pixelmap);
UINT _gxe_radial_slider_angle_set(GX_RADIAL_SLIDER *slider, GX_VALUE new_value);

#endif

