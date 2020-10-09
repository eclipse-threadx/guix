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
/*    _gx_system_screen_stack_get                         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function removes the topmost entry from the system screen      */
/*    stack, and returns popped parent and screen pointers to the caller. */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    popped_parent                         Retrieved stack top parent    */
/*    popped_screen                         Retrieved stack top screen    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
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
UINT  _gx_system_screen_stack_get(GX_WIDGET **popped_parent, GX_WIDGET **popped_screen)
{
UINT       status;
INT        top;
GX_WIDGET *screen = GX_NULL;
GX_WIDGET *parent = GX_NULL;

    top = _gx_system_screen_stack.gx_screen_stack_control_top;

    if (top >= 0)
    {
        top <<= 1;
        screen = _gx_system_screen_stack.gx_screen_stack_control_memory[top];
        parent = _gx_system_screen_stack.gx_screen_stack_control_memory[top + 1];

        _gx_system_screen_stack.gx_screen_stack_control_top--;

        status = GX_SUCCESS;
    }
    else
    {
        status = GX_FAILURE;
    }

    if (popped_parent)
    {
        (*popped_parent) = parent;
    }

    if (popped_screen)
    {
        (*popped_screen) = screen;
    }

    return status;
}

