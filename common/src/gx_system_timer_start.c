/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation
 * Copyright (c) 2026 Eclipse ThreadX contributors
 *
 * This program and the accompanying materials are made available under the
 * terms of the MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 **************************************************************************/


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
/*    _gx_system_timer_start                              PORTABLE C      */
/*                                                           6.1.7        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service starts a timer for the specified widget.               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    owner                                 Pointer to widget control     */
/*                                            block                       */
/*    timer_id                              ID of timer                   */
/*    initial ticks                         Initial expiration ticks      */
/*    reschedule_ticks                      Periodic expiration ticks     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*   tx_timer_activate                      start the ThreadX timer       */
/*   GX_ENTER_CRITICAL                      lock system mutex             */
/*   GX_EXIT_CRITICAL                       unlock system mutex           */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Application                                                    */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/**************************************************************************/
UINT  _gx_system_timer_start(GX_WIDGET *owner, UINT timer_id, UINT initial_ticks, UINT reschedule_ticks)
{
GX_TIMER *found;

#ifdef GX_THREADX_BINDING
#ifndef GX_DISABLE_THREADX_TIMER_SOURCE
UINT tx_timer_active;
#endif
#endif

    /* check for bad widget pointer */
    if (!owner)
    {
        return GX_PTR_ERROR;
    }

    /* lock entering critical section */
    GX_ENTER_CRITICAL

    /* check for already having this timer */
    found = _gx_system_active_timer_list;

    while (found)
    {
        if (found -> gx_timer_id == timer_id &&
            found -> gx_timer_owner == owner)
        {
            /* yes, this timer is already running */
            /* reset the existing timer value and return */
            found -> gx_timer_initial_ticks = initial_ticks;
            found -> gx_timer_reschedule_ticks = reschedule_ticks;
            GX_EXIT_CRITICAL
            return GX_SUCCESS;
        }
        found = found -> gx_timer_next;
    }

    /* check for having timer available */
    if (!_gx_system_free_timer_list)
    {
        /* no timers available, unlock and return error */
        GX_EXIT_CRITICAL
        return GX_OUT_OF_TIMERS;
    }

    /* we have a free timer, remove from free list */
    found = _gx_system_free_timer_list;
    _gx_system_free_timer_list = found -> gx_timer_next;

    /* initialize new timer */
    found -> gx_timer_initial_ticks = initial_ticks;
    found -> gx_timer_reschedule_ticks = reschedule_ticks;
    found -> gx_timer_id = timer_id;
    found -> gx_timer_owner = owner;

    /* link this timer at the head of the active list */
    found -> gx_timer_next = _gx_system_active_timer_list;
    _gx_system_active_timer_list = found;

#ifdef GX_THREADX_BINDING
#ifndef GX_DISABLE_THREADX_TIMER_SOURCE
    /* if the low-level timer is not active, start it */
    tx_timer_info_get(&_gx_system_timer, (CHAR **)TX_NULL, &tx_timer_active,
                      (ULONG *)TX_NULL, (ULONG *)TX_NULL, (TX_TIMER **)TX_NULL);

    if (!tx_timer_active)
    {
        tx_timer_activate(&_gx_system_timer);
    }
#endif
#else
    GX_TIMER_START;
#endif

    /* release our lock */
    GX_EXIT_CRITICAL
    return GX_SUCCESS;
}

