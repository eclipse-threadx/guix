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
/**   Scroll Wheel Management (Generic Scroll Wheel)                      */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_scroll_wheel.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_generic_scroll_wheel_event_process              PORTABLE C      */
/*                                                           6.1.7        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service processes an event for the generic scroll wheel.       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    wheel                                 Scroll wheel control block    */
/*    event_ptr                             Pointer to event to process   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_generic_scroll_wheel_children_position                          */
/*                                          Position the children for     */
/*                                            the scroll wheel children   */
/*    _gx_scroll_wheel_event_process        Process events for the        */
/*                                            scroll wheel widget         */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  06-02-2021        Ting Zhu              Initial Version 6.1.7         */
/*                                                                        */
/**************************************************************************/
UINT  _gx_generic_scroll_wheel_event_process(GX_GENERIC_SCROLL_WHEEL *wheel, GX_EVENT *event_ptr)
{
UINT status;

    switch (event_ptr -> gx_event_type)
    {
    case GX_EVENT_SHOW:

        /* Show the children before attempting to position them, because child
           widgets do not know their size until shown.  */

        status = _gx_scroll_wheel_event_process((GX_SCROLL_WHEEL *)wheel, event_ptr);

        /* Position the child widgets.  */
        if (!wheel -> gx_generic_scroll_wheel_child_count)
        {
            _gx_generic_scroll_wheel_children_position(wheel);
        }
        break;

    default:
        status = _gx_scroll_wheel_event_process((GX_SCROLL_WHEEL *)wheel, event_ptr);
        break;
    }

    return status;
}

