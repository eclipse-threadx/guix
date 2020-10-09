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
/*    _gx_system_view_fold                                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function combines adjcent views.                               */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    win                                   Window to add view to         */
/*    view                                  Rectangle defining new view   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    GX_TRUE | GX_FALSE                                                  */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_system_view_add                   Link a view to window's view  */
/*                                            list                        */
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
GX_BOOL _gx_system_view_fold(GX_WINDOW *win, GX_RECTANGLE *view)
{
GX_VIEW *oldview = win -> gx_window_views;

    while (oldview)
    {
        /* check for two views stacked vertically:*/

        if ((oldview -> gx_view_rectangle.gx_rectangle_left == view -> gx_rectangle_left) &&
            (oldview -> gx_view_rectangle.gx_rectangle_right == view -> gx_rectangle_right))
        {
            if (oldview -> gx_view_rectangle.gx_rectangle_bottom == (view -> gx_rectangle_top - 1))
            {
                /* New view is right below existing view, expand bottom of
                   existing: */
                oldview -> gx_view_rectangle.gx_rectangle_bottom = view -> gx_rectangle_bottom;
                return GX_TRUE;
            }

            if (oldview -> gx_view_rectangle.gx_rectangle_top == (view -> gx_rectangle_bottom + 1))
            {
                /* new view is right above existing view, just expand existing
                   view top: */
                oldview -> gx_view_rectangle.gx_rectangle_top = view -> gx_rectangle_top;
                return GX_TRUE;
            }
        }
        else
        {
            /* check for views right beside each other horizontally: */
            if (oldview -> gx_view_rectangle.gx_rectangle_top == view -> gx_rectangle_top &&
                oldview -> gx_view_rectangle.gx_rectangle_bottom == view -> gx_rectangle_bottom)
            {
                if (oldview -> gx_view_rectangle.gx_rectangle_right == (view -> gx_rectangle_left - 1))
                {
                    /* new view is to the right of existing view, expand right side of
                       existing:*/
                    oldview -> gx_view_rectangle.gx_rectangle_right = view -> gx_rectangle_right;
                    return GX_TRUE;
                }

                if (oldview -> gx_view_rectangle.gx_rectangle_left == (view -> gx_rectangle_right + 1))
                {
                    /* new view is to the left of existing view, expand left side of
                       existing view: */
                    oldview -> gx_view_rectangle.gx_rectangle_left = view -> gx_rectangle_left;
                    return GX_TRUE;
                }
            }
        }
        oldview = oldview -> gx_view_next;
    }
    return GX_FALSE;
}

