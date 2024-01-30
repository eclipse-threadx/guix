/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation 
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
/*    _gx_system_pen_speed_init                           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service initiates system pen speed.                            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    current                               Current pen position          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    tx_timer_get                          Get ThreadX timer tick        */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_system_event_send                 Send GUIX system event        */
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
VOID _gx_system_pen_speed_init(GX_POINT *current)
{
#ifdef GX_THREADX_BINDING
    _gx_system_last_clock = tx_time_get();
#else
    _gx_system_last_clock = GX_SYSTEM_TIME_GET;
#endif
    _gx_system_last_pen_point = *current;
    _gx_system_pen_speed_x = _gx_system_pen_speed_y = 0;
}

