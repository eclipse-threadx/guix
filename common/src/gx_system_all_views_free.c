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
/*    _gx_system_all_views_free                           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function releaes the views attached to the given root window   */
/*    and it's chilren back to the free list.                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    root                                  Root window that needs to     */
/*                                            have it's views released    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    views owned by this root and children are returned to free list     */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*     _gx_system_views_free                Free views for one window     */
/*     _gx_system_error_process             Process error code            */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
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
VOID _gx_system_all_views_free(GX_WINDOW_ROOT *root)
{
#ifndef GX_DISABLE_ERROR_CHECKING
int viewcount;
#endif

GX_VIEW   *test;
GX_WIDGET *child;
GX_WINDOW *win;

    /* pick up pointer to first child window */
    child = root -> gx_widget_first_child;

    /* free each of the top-level window's views */
    while (child)
    {
        if (child -> gx_widget_type >= GX_TYPE_WINDOW)
        {
            win = (GX_WINDOW *)child;
            test = win -> gx_window_views;

            /* does this window have any views? */

            if (test)
            {
                _gx_system_views_free(test);
                win -> gx_window_views = GX_NULL;
            }
        }
        child = (child -> gx_widget_next);
    }


    /* lastly, free the root window's views */

    test = root -> gx_window_views;

    if (test)
    {
        _gx_system_views_free(test);
        root -> gx_window_views = GX_NULL;
    }

#ifndef GX_DISABLE_ERROR_CHECKING
    test = _gx_system_free_views;

    viewcount = 0;

    while (test)
    {
        viewcount++;
        test = test -> gx_view_next;
    }

    if (viewcount != GX_MAX_VIEWS)
    {
        _gx_system_error_process(GX_SYSTEM_OUT_OF_VIEWS);
    }
#endif
}

