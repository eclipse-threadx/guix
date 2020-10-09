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
/*    _gx_sprite_start                                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service starts the sprite widget from a given frame number.    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    sprite                                Pointer to sprite widget      */
/*                                           control block                */
/*    frame_number                          The frame number to start     */
/*                                            with                        */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_timer_stop                 Stop an active GUIX timer     */
/*    _gx_system_timer_start                Start a GUIX timer            */
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
UINT  _gx_sprite_start(GX_SPRITE *sprite, USHORT frame_number)
{
GX_WIDGET       *widget = (GX_WIDGET *)sprite;
GX_SPRITE_FRAME *frame;
UINT             delayval;

    if (sprite -> gx_sprite_run_state == GX_SPRITE_RUNNING)
    {
        _gx_system_timer_stop(widget, GX_SPRITE_TIMER);
        sprite -> gx_sprite_run_state = GX_SPRITE_IDLE;
    }

    if (sprite -> gx_sprite_frame_list &&
        (widget -> gx_widget_status & GX_STATUS_VISIBLE))
    {
        if (frame_number < sprite -> gx_sprite_frame_count)
        {
            sprite -> gx_sprite_current_frame = frame_number;

            frame = &sprite -> gx_sprite_frame_list[sprite -> gx_sprite_current_frame];
            if (frame -> gx_sprite_frame_delay > 0)
            {
                delayval = frame -> gx_sprite_frame_delay;
            }
            else
            {
                delayval = 1;
            }
            _gx_system_timer_start(widget, GX_SPRITE_TIMER, delayval, 0);
            sprite -> gx_sprite_run_state = GX_SPRITE_RUNNING;
            _gx_system_dirty_mark(widget);
            return(GX_SUCCESS);
        }
    }
    return(GX_FAILURE);
}

