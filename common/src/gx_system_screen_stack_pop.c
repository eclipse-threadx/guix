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
#include "gx_widget.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_system_screen_stack_pop                         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function removes the topmost entry from the system screen      */
/*    stack, and attaches the poped screen to its parent.                 */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    control                               Pointer of stack entry.       */
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
UINT  _gx_system_screen_stack_pop(VOID)
{
INT        top;
GX_WIDGET *screen;
GX_WIDGET *parent;

    top = _gx_system_screen_stack.gx_screen_stack_control_top;

    if (top >= 0)
    {
        top <<= 1;
        screen = _gx_system_screen_stack.gx_screen_stack_control_memory[top];
        parent = _gx_system_screen_stack.gx_screen_stack_control_memory[top + 1];

        if (parent)
        {
            /* Attach the popped screen to its parent. */
            _gx_widget_attach(parent, screen);
        }

        _gx_system_screen_stack.gx_screen_stack_control_top--;

        return(GX_SUCCESS);
    }
    else
    {
        return(GX_FAILURE);
    }
}

