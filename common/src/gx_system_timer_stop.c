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
/**   System Management (System)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_system_timer_stop                               PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service stops the timer for the specified widget.              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    owner                                 Pointer to widget control     */
/*                                            block                       */
/*    timer_id                              ID of timer                   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_lock                       lock critical section         */
/*    _gx_system_unlock                     unlock critical section       */
/*    tx_timer_deactivate                   stop the ThreadX timer        */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Application                                                    */
/*    GUIX Internal Code                                                  */
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
UINT _gx_system_timer_stop(GX_WIDGET *owner, UINT timer_id)
{
UINT      status = GX_FAILURE;
GX_TIMER *next_timer;
GX_TIMER *previous_timer = GX_NULL;
GX_TIMER *search;

    GX_ENTER_CRITICAL

    /* find the timer in our active list */
    search = _gx_system_active_timer_list;

    while (search)
    {
        /* save point to next active timer */
        next_timer = search -> gx_timer_next;

        /* is this timer owned by caller? */
        if (search -> gx_timer_owner == owner)
        {
            /* is this the requested timer? */
            if (timer_id == 0 || search -> gx_timer_id == timer_id)
            {
                /* save status that we found at least one timer */
                status = GX_SUCCESS;

                /* If timer we found is first in active list, update list head */
                if (previous_timer == GX_NULL)
                {
                    _gx_system_active_timer_list = search -> gx_timer_next;
                }
                else
                {
                    previous_timer -> gx_timer_next = search -> gx_timer_next;
                }

                /* add this timer to head of free list */
                search -> gx_timer_next = _gx_system_free_timer_list;
                _gx_system_free_timer_list = search;

                /* if just looking for one timer, we are done */
                if (timer_id != 0)
                {
                    break;
                }
                else
                {
                    search = previous_timer;
                }
            }
        }

        /* advance search to next active timer */
        previous_timer = search;
        search = next_timer;

    }
    GX_EXIT_CRITICAL

    /* if our active list is NULL, then stop the tx_timer */

    if (_gx_system_active_timer_list == GX_NULL &&
        _gx_system_animation_list == GX_NULL)
    {
#ifdef GX_THREADX_BINDING
#ifndef GX_DISABLE_THREADX_TIMER_SOURCE
        tx_timer_deactivate(&_gx_system_timer);
#endif
#else
        GX_TIMER_STOP;
#endif
    }
    return status;
}

