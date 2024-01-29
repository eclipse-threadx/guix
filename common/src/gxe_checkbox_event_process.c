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
/**   Button Management (checkbox)                                        */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_widget.h"
#include "gx_button.h"

GX_CALLER_CHECKING_EXTERNS

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_checkbox_event_process                         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in the checkbox event process       */
/*    function call.                                                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    checkbox                              Checkbox control block        */
/*    event_ptr                             Incoming event to process     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_checkbox_event_process            Actual checkbox event process */
/*                                            call                        */
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
UINT  _gxe_checkbox_event_process(GX_CHECKBOX *checkbox, GX_EVENT *event_ptr)
{
UINT       status;
GX_WIDGET *parent;

    /* Check for appropriate caller.  */
    GX_INIT_AND_THREADS_CALLER_CHECKING

    /* Check for the invalid input pointers.  */
    if ((checkbox == GX_NULL) || (checkbox -> gx_widget_parent == GX_NULL) || (event_ptr == GX_NULL))
    {
        return(GX_PTR_ERROR);
    }

    parent = checkbox -> gx_widget_parent;

    /* Check for the invalid widget.  */
    if ((parent -> gx_widget_type == 0) ||
        (checkbox -> gx_widget_type == 0))
    {
        return(GX_INVALID_WIDGET);
    }

    /* Call the actual checkbox event process function.  */
    status = _gx_checkbox_event_process(checkbox, event_ptr);

    /* Return completion status.  */
    return status;
}

