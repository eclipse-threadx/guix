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
/*    _gx_system_views_update                             PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function calculates the viewports that are owned by each       */
/*    top level window attached to a root.                                */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    root                                  Root window that needs to     */
/*                                            have it's children updated  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    children of the root window have view list updated.                 */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_all_views_free            Return all views to free list  */
/*    _gx_system_root_view_add             Add a view to root window      */
/*                                                                        */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_system_canvas_refresh            Refresh the canvas             */
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

VOID _gx_system_views_update(GX_WINDOW_ROOT *root)
{
GX_RECTANGLE *baserect = &root -> gx_widget_size;

    _gx_system_all_views_free(root);

    /* the root window has to check for child windows that are on top */
    _gx_system_root_view_add(root, baserect);

    /* clear the update flag */
    root -> gx_window_root_views_changed = GX_FALSE;
}

