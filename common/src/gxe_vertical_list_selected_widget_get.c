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
/**   Vertical List (List)                                                */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_window.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_vertical_list_selected_widget_get              PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in the vertical list selected get   */
/*    function call.                                                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    vertical_list                         Vertical list control block   */
/*    return_list_entry                     Destination for return list   */
/*                                            entry widget                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_vertical_list_selected_get        Actual vertical list selected */
/*                                            get function                */
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
UINT  _gxe_vertical_list_selected_widget_get(GX_VERTICAL_LIST *vertical_list,
                                             GX_WIDGET **return_list_entry)
{
UINT status;

    /* Check for invalid input pointers.  */
    if ((vertical_list == GX_NULL) || (return_list_entry == GX_NULL))
    {
        return GX_PTR_ERROR;
    }

    /* Check for invalid widget. */
    if (vertical_list -> gx_widget_type == 0)
    {
        return GX_INVALID_WIDGET;
    }

    /* Call the actual vertical list selected get function.  */
    status = _gx_vertical_list_selected_widget_get(vertical_list, return_list_entry);

    /* Return completion status.  */
    return status;
}

