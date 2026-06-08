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
/*    _gxe_system_widget_find                             PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in system widget search call.       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    widget_id                            Widget ID to search for        */
/*    search_level                         Specifies the depth of the     */
/*                                           search                       */
/*    return_search_result                 Pointer to destination for     */
/*                                           widget found                 */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                               Completion status              */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_widget_find               Actual widget find function    */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Application Code                                               */
/*                                                                        */
/**************************************************************************/
UINT _gxe_system_widget_find(USHORT widget_id, INT search_level, GX_WIDGET **return_search_result)
{
UINT status;

    /* Check for invalid pointer.  */
    if (return_search_result == GX_NULL)
    {
        return(GX_PTR_ERROR);
    }

    /* Call actual system widget search.  */
    status = _gx_system_widget_find(widget_id, search_level, return_search_result);

    /* Return status.  */
    return status;
}

