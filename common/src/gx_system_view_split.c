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
/*    _gx_system_view_split                               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This splits a rectangle into over and under views                   */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    over                                  Pointer to window on top      */
/*    root                                  Pointer to root window        */
/*    original                              Rectangle to split into       */
/*                                            smaller chunks              */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    gx_system_root_view_add               Add viewport to root window   */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_system_root_view_add              Add view to a root window     */
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

VOID _gx_system_view_split(GX_WINDOW *over, GX_WINDOW_ROOT *root, GX_RECTANGLE *original)
{
GX_RECTANGLE split;
GX_RECTANGLE oversize = over -> gx_widget_size;

    /* check to see if any area on top is exposed */
    if (oversize.gx_rectangle_top > original -> gx_rectangle_top)
    {
        split.gx_rectangle_left = original -> gx_rectangle_left;
        split.gx_rectangle_right = original -> gx_rectangle_right;
        split.gx_rectangle_top = original -> gx_rectangle_top;
        split.gx_rectangle_bottom = (GX_VALUE)(oversize.gx_rectangle_top - 1);

        /* try to add this piece to the 'under' window's viewports */
        _gx_system_root_view_add(root, &split);
    }

    /* check to see if any area on bottom is exposed */
    if (oversize.gx_rectangle_bottom < original -> gx_rectangle_bottom)
    {
        split.gx_rectangle_left = original -> gx_rectangle_left;
        split.gx_rectangle_right = original -> gx_rectangle_right;
        split.gx_rectangle_top = (GX_VALUE)(oversize.gx_rectangle_bottom + 1);
        split.gx_rectangle_bottom = original -> gx_rectangle_bottom;

        /* try to add this piece to the 'under' window's viewports */
        _gx_system_root_view_add(root, &split);
    }

    /* check to see if any area on left is exposed */
    if (oversize.gx_rectangle_left > original -> gx_rectangle_left)
    {
        split.gx_rectangle_left = original -> gx_rectangle_left;
        split.gx_rectangle_right = (GX_VALUE)(oversize.gx_rectangle_left - 1);

        if (original -> gx_rectangle_top > oversize.gx_rectangle_top)
        {
            split.gx_rectangle_top = original -> gx_rectangle_top;
        }
        else
        {
            split.gx_rectangle_top = oversize.gx_rectangle_top;
        }

        if (original -> gx_rectangle_bottom < oversize.gx_rectangle_bottom)
        {
            split.gx_rectangle_bottom = original -> gx_rectangle_bottom;
        }
        else
        {
            split.gx_rectangle_bottom = oversize.gx_rectangle_bottom;
        }

        /* try to add this piece to the 'under' window's viewports */
        _gx_system_root_view_add(root, &split);
    }

    /* check to see if any area on right is exposed */
    if (oversize.gx_rectangle_right < original -> gx_rectangle_right)
    {
        split.gx_rectangle_left = (GX_VALUE)(oversize.gx_rectangle_right + 1);
        split.gx_rectangle_right = original -> gx_rectangle_right;

        if (original -> gx_rectangle_top > oversize.gx_rectangle_top)
        {
            split.gx_rectangle_top = original -> gx_rectangle_top;
        }
        else
        {
            split.gx_rectangle_top = oversize.gx_rectangle_top;
        }

        if (original -> gx_rectangle_bottom < oversize.gx_rectangle_bottom)
        {
            split.gx_rectangle_bottom = original -> gx_rectangle_bottom;
        }
        else
        {
            split.gx_rectangle_bottom = oversize.gx_rectangle_bottom;
        }
        /* try to add this piece to the 'under' window's viewports */
        _gx_system_root_view_add(root, &split);
    }
}

