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
/**   Text Scroll Wheel Management (Scroll Wheel)                         */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_scroll_wheel.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_text_scroll_wheel_event_process                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function processes the comming events for a scroll wheel       */
/*    widget.                                                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    wheel                                 Text scroll wheel control     */
/*                                            block                       */
/*    event_ptr                             Event to be processed         */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_text_scroll_wheel_dynamic_bidi_text_delete                     */
/*                                          Release dynamic bidi text     */
/*    _gx_scroll_wheel_event_process        Default event process         */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  09-30-2020     Kenneth Maxwell          Initial Version 6.1           */
/*                                                                        */
/**************************************************************************/
UINT _gx_text_scroll_wheel_event_process(GX_TEXT_SCROLL_WHEEL *wheel, GX_EVENT *event_ptr)
{
    switch (event_ptr -> gx_event_type)
    {
#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
    case GX_EVENT_DELETE:
        _gx_text_scroll_wheel_dynamic_bidi_text_delete(wheel);
        return _gx_scroll_wheel_event_process((GX_SCROLL_WHEEL*)wheel, event_ptr);
#endif

    default:
        return _gx_scroll_wheel_event_process((GX_SCROLL_WHEEL *)wheel, event_ptr);
    }
}

