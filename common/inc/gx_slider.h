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
/**   Prompt Management (Slider)                                          */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/
/*                                                                        */
/*  COMPONENT DEFINITION                                   RELEASE        */
/*                                                                        */
/*    gx_slider.h                                         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file defines the GUIX slide widget,                            */
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

#ifndef GX_SLIDER_H
#define GX_SLIDER_H


/* Define prompt management function prototypes.  */

#define GX_SLIDER_NEEDLE_WIDTH 5

UINT _gx_slider_create(GX_SLIDER *slider, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                       INT tick_count, GX_SLIDER_INFO *slider_info,
                       ULONG style, USHORT slider_id, GX_CONST GX_RECTANGLE *size);

VOID _gx_slider_draw(GX_SLIDER *slider);
UINT _gx_slider_event_process(GX_SLIDER *slider, GX_EVENT *event_ptr);
UINT _gx_slider_info_set(GX_SLIDER *slider, GX_SLIDER_INFO *info);
VOID _gx_slider_needle_draw(GX_SLIDER *slider);
UINT _gx_slider_needle_position_get(GX_SLIDER *slider, GX_SLIDER_INFO *slider_info, GX_RECTANGLE *return_position);
VOID _gx_slider_tickmarks_draw(GX_SLIDER *slider);
UINT _gx_slider_travel_get(GX_SLIDER *slider, GX_SLIDER_INFO *info, INT *return_min_travel, INT *return_max_travel);
UINT _gx_slider_value_calculate(GX_SLIDER *slider, GX_SLIDER_INFO *info, INT newpos);
UINT _gx_slider_value_set(GX_SLIDER *slider, GX_SLIDER_INFO *info, INT new_value);

UINT _gx_pixelmap_slider_create(GX_PIXELMAP_SLIDER *slider, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                                GX_SLIDER_INFO *info,
                                GX_PIXELMAP_SLIDER_INFO *pixelmap_info, ULONG style, USHORT pixelmap_slider_id,
                                GX_CONST GX_RECTANGLE *size);

VOID _gx_pixelmap_slider_draw(GX_PIXELMAP_SLIDER *slider);
UINT _gx_pixelmap_slider_event_process(GX_PIXELMAP_SLIDER *slider, GX_EVENT *event_ptr);
UINT _gx_pixelmap_slider_pixelmap_set(GX_PIXELMAP_SLIDER *slider, GX_PIXELMAP_SLIDER_INFO *info);

UINT _gxe_slider_create(GX_SLIDER *slider, GX_CONST GX_CHAR *name, GX_WIDGET *parent, INT tick_count,
                        GX_SLIDER_INFO *slider_info, ULONG style, USHORT slider_id, GX_CONST GX_RECTANGLE *size,
                        UINT slider_control_block_size);
UINT _gxe_slider_event_process(GX_SLIDER *slider, GX_EVENT *event_ptr);
UINT _gxe_slider_travel_get(GX_SLIDER *slider, GX_SLIDER_INFO *info, INT *return_min_travel, INT *return_max_travel);
UINT _gxe_slider_needle_position_get(GX_SLIDER *slider, GX_SLIDER_INFO *slider_info, GX_RECTANGLE *return_position);
UINT _gxe_slider_value_calculate(GX_SLIDER *slider, GX_SLIDER_INFO *info, INT new_position);
UINT _gxe_slider_value_set(GX_SLIDER *slider, GX_SLIDER_INFO *info, INT new_value);

UINT _gxe_pixelmap_slider_create(GX_PIXELMAP_SLIDER *slider, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                                 GX_SLIDER_INFO *info, GX_PIXELMAP_SLIDER_INFO *pixelmap_info,
                                 ULONG style, USHORT pixelmap_slider_id,
                                 GX_CONST GX_RECTANGLE *size, UINT pixelmap_slider_controlb_block_size);
UINT _gxe_pixelmap_slider_event_process(GX_PIXELMAP_SLIDER *slider, GX_EVENT *event_ptr);
UINT _gxe_slider_info_set(GX_SLIDER *slider, GX_SLIDER_INFO *info);
UINT _gxe_pixelmap_slider_pixelmap_set(GX_PIXELMAP_SLIDER *slider, GX_PIXELMAP_SLIDER_INFO *info);
VOID _gx_pixelmap_slider_pixelmap_update(GX_PIXELMAP_SLIDER *slider);

#endif

