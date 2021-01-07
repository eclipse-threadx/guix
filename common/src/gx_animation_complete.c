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

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_system.h"
#include "gx_canvas.h"
#include "gx_animation.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_animation_complete_event_send                   PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to send an animation complete event.       */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    animation                             Pointer to animation control  */
/*                                            block                       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_event_send                 Send GUIX event               */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_animation_complete                                              */
/*    _gx_animation_slide_landing                                         */
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
VOID _gx_animation_complete_event_send(GX_ANIMATION *animation)
{
GX_EVENT complete_event;

    if (animation -> gx_animation_info.gx_animation_id)
    {
        if (animation -> gx_animation_info.gx_animation_target)
        {
            complete_event.gx_event_target = animation -> gx_animation_info.gx_animation_target;
        }
        else
        {
            complete_event.gx_event_target = animation -> gx_animation_info.gx_animation_parent;
        }

        /* send event to notify the animation has been completed */

        complete_event.gx_event_type = GX_EVENT_ANIMATION_COMPLETE;
        complete_event.gx_event_sender = animation -> gx_animation_info.gx_animation_id;
        _gx_system_event_send(&complete_event);
    }
}



/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_animation_complete                              PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Called internally when an animation sequence is finished.           */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    animation                             Pointer to window control     */
/*                                            block                       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_animation_stop                    Stop an animation             */
/*    _gx_widget_detach                     Detach a widget from its      */
/*                                            parent                      */
/*    _gx_widget_hide                       Hide a widget                 */
/*    _gx_widget_shift                      Shift a widget                */
/*    _gx_widget_attach                     Attach a widget to its parent */
/*    _gx_system_event_send                 Send GUIX event               */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_animation_update                                                */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  12-31-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            improved logic,             */
/*                                            resulting in version 6.1.3  */
/*                                                                        */
/**************************************************************************/

VOID _gx_animation_complete(GX_ANIMATION *animation)
{
GX_WIDGET *target;
GX_VALUE   xshift;
GX_VALUE   yshift;

    /* Remove animation from active list and assign idle status */
    _gx_animation_stop(animation);

    /* do final cleanup */
    target = animation -> gx_animation_info.gx_animation_target;

    if (animation -> gx_animation_canvas)
    {
        /* hide the animation root */
        if (target -> gx_widget_parent)
        {
            _gx_widget_hide(target -> gx_widget_parent);
        }

        /* Hide animation target. */
        _gx_widget_detach(target);

        if (animation -> gx_animation_info.gx_animation_style & GX_ANIMATION_PUSH_STACK)
        {
            /* Push animation target to system screen stack. */
            _gx_system_screen_stack_push(target);
        }
        else if (animation -> gx_animation_info.gx_animation_style & GX_ANIMATION_DETACH)
        {
            if (target -> gx_widget_status & GX_STATUS_STUDIO_CREATED)
            {
                _gx_widget_delete(target);
                animation -> gx_animation_info.gx_animation_target = GX_NULL;
            }
        }
        else
        {
            /* shift the target into final position */
            _gx_widget_shift(animation -> gx_animation_info.gx_animation_target,
                             animation -> gx_animation_info.gx_animation_end_position.gx_point_x,
                             animation -> gx_animation_info.gx_animation_end_position.gx_point_y, GX_FALSE);

            /* attach the widget to it's parent */
            _gx_widget_attach(animation -> gx_animation_info.gx_animation_parent, target);
        }
        _gx_canvas_hide(animation -> gx_animation_canvas);
        _gx_system_canvas_refresh();
    }
    else
    {
#if defined(GX_BRUSH_ALPHA_SUPPORT)

        if (animation -> gx_animation_info.gx_animation_start_alpha !=
            animation -> gx_animation_info.gx_animation_end_alpha)
        {
            animation -> gx_animation_info.gx_animation_target -> gx_widget_style &= ~GX_STYLE_USE_LOCAL_ALPHA;
            _gx_system_dirty_mark(animation -> gx_animation_info.gx_animation_target);
        }
#endif

        if (animation -> gx_animation_info.gx_animation_style & GX_ANIMATION_PUSH_STACK)
        {
            /* Push animation target to system screen stack. */
            _gx_system_screen_stack_push(target);
        }
        else if (animation -> gx_animation_info.gx_animation_style & GX_ANIMATION_DETACH)
        {
            _gx_widget_hide(target);

            /* if this target was created by the Studio generated code, then delete the widget here */
            if (target -> gx_widget_status & GX_STATUS_STUDIO_CREATED)
            {
                _gx_widget_delete(target);
                animation -> gx_animation_info.gx_animation_target = GX_NULL;
            }
        }
        else
        {
            /* test for shift translation */
            xshift = (GX_VALUE)(animation -> gx_animation_info.gx_animation_end_position.gx_point_x -
                                animation -> gx_animation_info.gx_animation_start_position.gx_point_x);

            yshift = (GX_VALUE)(animation -> gx_animation_info.gx_animation_end_position.gx_point_y -
                                animation -> gx_animation_info.gx_animation_start_position.gx_point_y);

            if (xshift || yshift)
            {
                xshift = (GX_VALUE)(animation -> gx_animation_info.gx_animation_end_position.gx_point_x -
                                    target -> gx_widget_size.gx_rectangle_left);

                yshift = (GX_VALUE)(animation -> gx_animation_info.gx_animation_end_position.gx_point_y -
                                    target -> gx_widget_size.gx_rectangle_top);

                /* shift the target into final position */
                _gx_widget_shift(target, xshift, yshift, GX_TRUE);
            }
        }
    }

    _gx_animation_complete_event_send(animation);

    /* If this animation came from the system pool, return it */
    if (animation -> gx_animation_system_allocated)
    {
        _gx_system_animation_free(animation);
    }
}

