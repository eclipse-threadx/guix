/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation
 * Copyright (c) 2026 Eclipse ThreadX contributors
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
/**   Window Management (Window)                                          */
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
/*    _gxe_window_scrollbar_find                          PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in the window scrollbar find        */
/*    function call.                                                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    window                                Pointer to window             */
/*    type                                  GX_SCROLLBAR_HORIZONTAL or    */
/*                                            GX_SCROLLBAR_VERTICAL       */
/*    return_scrollbar                      Pointer to destination for    */
/*                                            scrollbar                   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_window_scrollbar_find             Actual window scrollbar find  */
/*                                            function                    */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/**************************************************************************/
UINT _gxe_window_scrollbar_find(GX_WINDOW *window, USHORT type, GX_SCROLLBAR **return_scrollbar)
{
UINT status;

    /* Check for invalid input pointers.  */
    if ((window == GX_NULL) || (return_scrollbar == GX_NULL))
    {
        return(GX_PTR_ERROR);
    }

    /* Check for invalid widget.  */
    if (window -> gx_widget_type == 0)
    {
        return(GX_INVALID_WIDGET);
    }

    /* Check for invalid type.  */
    if ((type != GX_TYPE_HORIZONTAL_SCROLL) && (type != GX_TYPE_VERTICAL_SCROLL))
    {
        return(GX_INVALID_TYPE);
    }

    /* Call actual window scrollbar find function.  */
    status = _gx_window_scrollbar_find(window, type, return_scrollbar);

    /* Return successful completion.  */
    return(status);
}

