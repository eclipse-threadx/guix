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
/**   Sprite Management (Sprite)                                          */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/
/*                                                                        */
/*  COMPONENT DEFINITION                                   RELEASE        */
/*                                                                        */
/*    gx_sprite.h                                         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file defines the GUIX sprite component, including all data     */
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
/*                                                                        */
/**************************************************************************/

#ifndef GX_SPRITE_H
#define GX_SPRITE_H

#define GX_SPRITE_IDLE    0x0001
#define GX_SPRITE_RUNNING 0x0002
#define GX_SPRITE_TIMER   0x1000

/* Define animation management function prototypes.  */
UINT _gx_sprite_create(GX_SPRITE *sprite, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                       GX_SPRITE_FRAME *frame_list, USHORT frame_count,
                       ULONG style, USHORT sprite_id, GX_CONST GX_RECTANGLE *size);

UINT _gx_sprite_start(GX_SPRITE *sprite, USHORT start_frame);
UINT _gx_sprite_stop(GX_SPRITE *sprite);
VOID _gx_sprite_update(GX_SPRITE *sprite);
VOID _gx_sprite_complete(GX_SPRITE *sprite);
UINT _gx_sprite_current_frame_set(GX_SPRITE *sprite, USHORT frame);
VOID _gx_sprite_draw(GX_SPRITE *sprite);
UINT _gx_sprite_event_process(GX_SPRITE *sprite, GX_EVENT *event_ptr);
UINT _gx_sprite_frame_list_set(GX_SPRITE *sprite, GX_SPRITE_FRAME *frame_list, USHORT frame_count);

/* error checking versions of the animation API functions */
UINT _gxe_sprite_create(GX_SPRITE *sprite, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                        GX_SPRITE_FRAME *frame_list, USHORT frame_count,
                        ULONG style, USHORT sprite_id, GX_CONST GX_RECTANGLE *size, UINT sprite_control_block_size);
UINT _gxe_sprite_current_frame_set(GX_SPRITE *sprite, USHORT frame);
UINT _gxe_sprite_frame_list_set(GX_SPRITE *sprite, GX_SPRITE_FRAME *frame_list, USHORT frame_count);
UINT _gxe_sprite_start(GX_SPRITE *sprite, USHORT frame);
UINT _gxe_sprite_stop(GX_SPRITE *sprite);


#endif

