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
/*    _gx_animation_stop                                  PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function stops an animation sequence.                          */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    animation                             Pointer to animation control  */
/*                                            block                       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*   tx_timer_deactivate                    Deactivate a timer            */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_animation_complete                                              */
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
/*                                            added GX_DISABLE_THREADX_   */
/*                                            TIMER_SOURCE configuration, */
/*                                            resulting in version 6.1.3  */
/*                                                                        */
/**************************************************************************/
UINT _gx_animation_stop(GX_ANIMATION *animation)
{
UINT          status = GX_SUCCESS;
GX_ANIMATION *previous;

    /* assign IDLE status to this animation structure */
    animation -> gx_animation_status = GX_ANIMATION_IDLE;

    /* Remove the animation from the active list */
    GX_ENTER_CRITICAL

    if (_gx_system_animation_list == animation)
    {
        /* animation is first in active list */
        _gx_system_animation_list = _gx_system_animation_list -> gx_animation_next;

        /* if there are no other active animations or timers, stop the system timer */
        if ((_gx_system_active_timer_list == NULL) && (_gx_system_animation_list == NULL))
        {
#ifdef GX_THREADX_BINDING
#ifndef GX_DISABLE_THREADX_TIMER_SOURCE
            tx_timer_deactivate(&_gx_system_timer);
#endif
#else
            GX_TIMER_STOP;
#endif
        }
    }
    else
    {
        /* the animation is not the first, find the previous and 
           adjust linked list
        */
        previous = _gx_system_animation_list;
        while (previous)
        {
            if (previous -> gx_animation_next == animation)
            {
                previous -> gx_animation_next = animation -> gx_animation_next;
                break;
            }
            previous = previous -> gx_animation_next;
        }
    }
    GX_EXIT_CRITICAL
    return(status);
}

