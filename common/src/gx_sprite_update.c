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
/*    _gx_sprite_update                                   PORTABLE C      */
/*                                                           6.1.10       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service draws a sprite widget.                                 */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    sprite                                Pointer to the control block  */
/*                                            of the sprite widget        */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_event_generate             Generate an event             */
/*    _gx_system_timer_start                Allocate a free timer and     */
/*                                            activate it                 */
/*    _gx_system_dirty_mark                 Mark widget as dirty          */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_sprite_event_process              Event process routine for     */
/*                                            the sprite widget           */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  01-31-2022     Kenneth Maxwell          Modified comment(s),          */
/*                                            fix logic for restarting    */
/*                                            sprite timer,               */
/*                                            resulting in version 6.1.10 */
/*                                                                        */
/**************************************************************************/
VOID  _gx_sprite_update(GX_SPRITE *sprite)
{
GX_WIDGET       *widget = (GX_WIDGET *)sprite;
GX_SPRITE_FRAME *frame;
UINT             delayval;

    if (sprite->gx_sprite_run_state == GX_SPRITE_RUNNING)
    {
        if (sprite -> gx_sprite_frame_list)
        {
            if (sprite -> gx_sprite_current_frame + 1 >= sprite -> gx_sprite_frame_count)
            {
                if (sprite -> gx_widget_style & GX_STYLE_SPRITE_LOOP)
                {
                    sprite -> gx_sprite_current_frame = 0;
                }
                else
                {
                    sprite -> gx_sprite_run_state = GX_SPRITE_IDLE;
                    _gx_widget_event_generate(widget, GX_EVENT_SPRITE_COMPLETE, 0);
                    return;
                }
            }
            else
            {
                sprite -> gx_sprite_current_frame++;
            }

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
            _gx_system_dirty_mark(widget);
        }
        else
        {
            sprite -> gx_sprite_run_state = GX_SPRITE_IDLE;
        }
    }
}

