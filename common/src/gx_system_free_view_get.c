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
#include "gx_utility.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_system_free_view_get                            PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Grabs a free GX_VIEW from the list of free views, updates free      */
/*    view list.                                                          */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Returns a pointer to a free GX_VIEW structure                       */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_error_process              Process a system error        */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_system_view_add                   Link a view to window's       */
/*                                            view list                   */
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

GX_VIEW *_gx_system_free_view_get(VOID)
{
GX_VIEW *retval = GX_NULL;

    if (_gx_system_free_views)
    {
        retval = _gx_system_free_views;
        _gx_system_free_views = _gx_system_free_views -> gx_view_next;
    }
    else
    {
        _gx_system_error_process(GX_SYSTEM_OUT_OF_VIEWS);
    }
    return retval;
}

