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
/*    _gxe_widget_child_detect                            PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in the widget child detect function */
/*    call.                                                               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    parent                                Pointer to parent widget      */
/*    child                                 Pointer to child widget       */
/*    return_detect                         Pointer to destination for    */
/*                                          detection                     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_child_detect               Actual widget child detect    */
/*                                            function                    */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX application code                                               */
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
UINT _gxe_widget_child_detect(GX_WIDGET *parent, GX_WIDGET *child, GX_BOOL *return_detect)
{
UINT status;

    /* Check for invalid input pointers.  */
    if ((parent == GX_NULL) || (child == GX_NULL) || (return_detect == GX_NULL))
    {
        return(GX_PTR_ERROR);
    }

    /* Check for invalid parent and child widget.  */
    if ((parent -> gx_widget_type == 0) ||
        (child -> gx_widget_type == 0))
    {
        return(GX_INVALID_WIDGET);
    }

    /* Call actual widget child detect function.  */
    status = _gx_widget_child_detect(parent, child, return_detect);

    /* Return completion status.  */
    return(status);
}

