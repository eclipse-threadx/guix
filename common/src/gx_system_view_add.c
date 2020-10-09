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
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
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

