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
/**   Widget Management (Widget)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_widget_previous_sibling_get                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in the                              */
/*    gx_widget_previous_sibling_get function call.                       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    current                               Pointer to current widget to  */
/*                                            start search from           */
/*    return_widget                         Pointer to destination for    */
/*                                            found widget                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_previous_sibling_get       Actual                        */
/*                                            widget_previous_sibling_get */
/*                                            function.                   */
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
UINT _gxe_widget_previous_sibling_get(GX_WIDGET *current, GX_WIDGET **return_widget)
{
UINT status;

    /* Check for invalid input pointers.  */
    if ((current == GX_NULL) || (return_widget == GX_NULL))
    {
        return(GX_PTR_ERROR);
    }

    /* Check for invalid widget.  */
    if (current -> gx_widget_type == 0)
    {
        return(GX_INVALID_WIDGET);
    }

    /* Call actual widget find function.  */
    status = _gx_widget_previous_sibling_get(current, return_widget);

    /* Return successful completion.  */
    return(status);
}

