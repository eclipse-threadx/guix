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
/*    _gx_system_input_release                            PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Release previously captured input events                            */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    owner                                 Widget that releases input    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Return status                 */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _GX_ENTER_CRITICAL                    Enter critical section        */
/*    _gx_widget_status_remove              Remove a flag from the widget */
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

UINT _gx_system_input_release(GX_WIDGET *owner)
{
GX_WIDGET **stacktop;
GX_WIDGET **stackptr;
UINT        status = GX_PTR_ERROR;

    GX_ENTER_CRITICAL
    if (_gx_system_capture_count > 0)
    {
        if (owner -> gx_widget_status & GX_STATUS_OWNS_INPUT)
        {
            stacktop = _gx_system_input_capture_stack + _gx_system_capture_count - 1;
            stackptr = _gx_system_input_capture_stack;

            while (stackptr <= stacktop)
            {
                if (*stackptr == owner)
                {
                    _gx_widget_status_remove(owner, GX_STATUS_OWNS_INPUT);
                    _gx_system_capture_count--;
                    *stackptr = GX_NULL;
                    status = GX_SUCCESS;
                    break;
                }
                stackptr++;
            }

            if (status == GX_SUCCESS)
            {
                /* collapse the stack if this entry was in the middle */
                if (stackptr < stacktop)
                {
                    while (stackptr < stacktop)
                    {
                        *stackptr = *(stackptr + 1);
                        stackptr++;
                    }
                }
                if (_gx_system_capture_count > 0)
                {
                    stacktop--;
                    _gx_system_input_owner = *stacktop;
                }
                else
                {
                    _gx_system_input_owner = GX_NULL;
                }
            }
        }
        else
        {
            status = GX_NO_CHANGE;
        }
    }
    else
    {
        status = GX_NO_CHANGE;
    }

    GX_EXIT_CRITICAL
    return status;
}

