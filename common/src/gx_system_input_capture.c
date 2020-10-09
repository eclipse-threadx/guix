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
#include "gx_widget.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_system_input_capture                            PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Temporarily direct all input events to specified widget             */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    ownert                                Widget that wants to own      */
/*                                            input events                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    widget added to capture stack                                       */
/*    widget receives GX_STATUS_OWNS_INPUT                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _GX_ENTER_CRITICAL                    Enter critical section        */
/*    _gx_widget_status_add                 Add status flag to the widget */
/*    _GX_EXIT_CRITICAL                     Exit critical section         */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
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

UINT _gx_system_input_capture(GX_WIDGET *owner)
{
GX_WIDGET **stackptr;
UINT        status = GX_SUCCESS;

    GX_ENTER_CRITICAL
    if (_gx_system_capture_count < GX_MAX_INPUT_CAPTURE_NESTING)
    {
        if (!(owner -> gx_widget_status & GX_STATUS_OWNS_INPUT))
        {
            stackptr = _gx_system_input_capture_stack + _gx_system_capture_count;
            *stackptr = owner;
            _gx_widget_status_add(owner, GX_STATUS_OWNS_INPUT);
            _gx_system_capture_count++;
            _gx_system_input_owner = owner;
        }
        else
        {
            status = GX_NO_CHANGE;
        }
    }
    else
    {
        status = GX_INPUT_CAPTURE_NESTING_EXCEEDED;
    }

    GX_EXIT_CRITICAL
    return status;
}

