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

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_display.h"
#include "gx_context.h"
#include "gx_canvas.h"
#include "gx_widget.h"
#include "gx_sprite.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_sprite_frame_list_set                           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service assigns the frame list to the sprite widget.           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    sprite                                Pointer to sprite widget      */
/*                                            control block               */
/*    frame_list                            The frame list to be assigned */
/*    frame_count                           Number of frames in the frame */
/*                                            list                        */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_sprite_stop                       Stop a sprite widget          */
/*    _gx_system_dirty_mark                 Mark the widget as dirty      */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
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
UINT  _gx_sprite_frame_list_set(GX_SPRITE *sprite, GX_SPRITE_FRAME *frame_list, USHORT frame_count)
{
GX_WIDGET *widget = (GX_WIDGET *)sprite;

    if (sprite -> gx_sprite_run_state == GX_SPRITE_RUNNING)
    {
        _gx_sprite_stop(sprite);
    }
    sprite -> gx_sprite_current_frame = 0;
    sprite -> gx_sprite_frame_count = frame_count;
    sprite -> gx_sprite_frame_list = frame_list;

    if (sprite -> gx_widget_status & GX_STATUS_VISIBLE)
    {
        _gx_system_dirty_mark(widget);
    }
    return(GX_SUCCESS);
}

