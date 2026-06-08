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
/*    _gx_system_event_send                               PORTABLE C      */
/*                                                           6.1.11       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service sends the specified event for processing.              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    event                                 Pointer to event              */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    GX_ABS                                Compute the absolute value    */
/*    tx_queue_send                         Send message through ThreadX  */
/*                                            queue                       */
/*    _gx_system_pen_speed_init             Initialize pen speed          */
/*    _gx_system_pen_speed_update           Update pen speed              */
/*    _gx_system_pen_flick_test             Check for pen flick event     */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Application                                                    */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/**************************************************************************/
UINT  _gx_system_event_send(GX_EVENT *in_event)
{
UINT       status = GX_SYSTEM_ERROR;
GX_BOOL    check_send_flick = GX_FALSE;
GX_WIDGET *target = GX_NULL;

    switch (in_event -> gx_event_type)
    {
    case GX_EVENT_PEN_DOWN:
        _gx_system_pen_speed_init(&in_event -> gx_event_payload.gx_event_pointdata);
        break;

    case GX_EVENT_PEN_DRAG:
        _gx_system_pen_speed_update(&in_event -> gx_event_payload.gx_event_pointdata);
        break;

    case GX_EVENT_PEN_UP:
        check_send_flick = GX_TRUE;

        if (_gx_system_capture_count > 0)
        {
            /* Get the widget that owns the system input.  */
            target = *_gx_system_input_capture_stack;
        }
        break;
    }

#ifdef GX_THREADX_BINDING
    if (tx_queue_send(&_gx_system_event_queue, in_event, TX_NO_WAIT) ==
        TX_SUCCESS)
    {

        status = GX_SUCCESS;
    }
#else
    status = GX_EVENT_PUSH(in_event);
#endif

    if (check_send_flick)
    {
        _gx_system_pen_flick_test(target);
    }
    return status;
}

