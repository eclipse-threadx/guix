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
#include "gx_utility.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_system_view_add                                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Link a view to window's view list                                   */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    win                                   Window to add view to         */
/*    view                                  Rectangle defining new view   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*   _gx_system_view_fold                   Combine views                 */
/*   _gx_system_free_view_get               Get free view from storage    */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_system_root_view_add              Add a view to the root        */
/*                                            window                      */
/*                                                                        */
/**************************************************************************/
VOID _gx_system_view_add(GX_WINDOW *win, GX_RECTANGLE *view)
{
GX_VIEW *newview;

    if (_gx_system_view_fold(win, view))
    {
        return;
    }
    newview = _gx_system_free_view_get();

    if (newview)
    {
        newview -> gx_view_rectangle = *view;

        if (win -> gx_window_views)
        {
            newview -> gx_view_next = win -> gx_window_views;
        }
        else
        {
            newview -> gx_view_next = GX_NULL;
        }
        win -> gx_window_views = newview;
    }
}

