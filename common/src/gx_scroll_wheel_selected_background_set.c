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
/**   Scroll Wheel Management (Scroll Wheel)                              */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_scroll_wheel.h"
#include "gx_system.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_scroll_wheel_selected_background_set            PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function sets background pixelmap id for the selected row of   */
/*    scroll wheel widget.                                                */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    wheel                                 Scroll wheel control block    */
/*    selected_bg                           Selected background to be set */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_dirty_mark                 Mark a widget as dirty        */
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
UINT _gx_scroll_wheel_selected_background_set(GX_SCROLL_WHEEL *wheel, GX_RESOURCE_ID selected_bg)
{
    wheel -> gx_scroll_wheel_selected_background = selected_bg;

    if (wheel -> gx_widget_status & GX_STATUS_VISIBLE)
    {
        _gx_system_dirty_mark((GX_WIDGET *)wheel);
    }

    return GX_SUCCESS;
}

