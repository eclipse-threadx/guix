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
/*                                                           6.2.1        */
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
/*  07-29-2022     Kenneth Maxwell          Modified comment(s),          */
/*                                            optimized logic and ensured */
/*                                            released stack entries are  */
/*                                            reset to NULL,              */
/*                                            resulting in version 6.1.12 */
/*  03-08-2023     Ting Zhu                 Modified comment(s),          */
/*                                            improved logic,             */
/*                                            resulting in version 6.2.1  */
/*                                                                        */
/**************************************************************************/

UINT _gx_system_input_release(GX_WIDGET *owner)
{
UINT        status = GX_PTR_ERROR;
GX_WIDGET **stack;
INT         current;
INT         top;

    GX_ENTER_CRITICAL
    if (_gx_system_capture_count > 0)
    {
        if (owner -> gx_widget_status & GX_STATUS_OWNS_INPUT)
        {
            stack = _gx_system_input_capture_stack;
            top = _gx_system_capture_count - 1;
            current = 0;

            while (current <= top)
            {
                if (stack[current] == owner)
                {
                    _gx_widget_status_remove(owner, GX_STATUS_OWNS_INPUT);
                    _gx_system_capture_count--;
                    status = GX_SUCCESS;
                    break;
                }
                current++;
            }

            if (status == GX_SUCCESS)
            {
                /* collapse the stack if this entry was in the middle */
                while (current < top)
                {
                    stack[current] = stack[current + 1];
                    current++;
                }
                stack[current] = GX_NULL;

                if (top > 0)
                {
                    _gx_system_input_owner = stack[top - 1];
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

