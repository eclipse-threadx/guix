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
/**   Screen Stack Management (Screen Stack)                              */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_screen_stack.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_screen_stack_push                               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function detaches screen from its parent, and pushes the       */
/*    old screen pointer and the parent pointer onto the screen stack.    */
/*    The new screen pointer is then attached to the parent.              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    control                               Pointer of stack entry        */
/*    screen                                Screen pointer to push        */
/*    new_screen                            Pointer of new screen         */
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
UINT  _gx_screen_stack_push(GX_SCREEN_STACK_CONTROL *control, GX_WIDGET *screen, GX_WIDGET *new_screen)
{
INT top;    
    control -> gx_screen_stack_control_top++;
    top = control -> gx_screen_stack_control_top;

    if (top <= control -> gx_screen_stack_control_max - 1)
    {
        top <<= 1;

        /* Push the screen and its parent to screen stack.  */
        control -> gx_screen_stack_control_memory[top] = screen;
        control -> gx_screen_stack_control_memory[top + 1] = screen -> gx_widget_parent;

        /* Attach the new screen to the parent.  */
        _gx_widget_attach(screen -> gx_widget_parent, new_screen);

        /* Dettached the old screen from its parent.  */
        _gx_widget_detach(screen);


        return(GX_SUCCESS);
    }
    else
    {
        control -> gx_screen_stack_control_top--;
        return(GX_FAILURE);
    }
}

