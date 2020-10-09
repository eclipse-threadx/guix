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
/**   Window Management (Window)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_window.h"
#include "gx_widget.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_window_root_delete                              PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function delets a previous-created root window                 */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    root_window                           Window control block          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    memset                                Set control block to zero     */
/*    _gx_system_all_views_free             Free up all view ports        */
/*    _gx_widget_delete                     Delete widget                 */
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
UINT  _gx_window_root_delete(GX_WINDOW_ROOT *root_window)
{
GX_WINDOW_ROOT *previous;

    _gx_system_all_views_free(root_window);

    if (_gx_system_root_window_created_list == root_window)
    {
        _gx_system_root_window_created_list = (GX_WINDOW_ROOT *)_gx_system_root_window_created_list -> gx_widget_next;
    }
    else
    {
        previous = _gx_system_root_window_created_list;

        while (previous)
        {
            if ((GX_WINDOW_ROOT *)previous -> gx_widget_next == root_window)
            {
                previous -> gx_widget_next = root_window -> gx_widget_next;
                break;
            }

            previous = (GX_WINDOW_ROOT *)previous -> gx_widget_next;
        }
    }

    _gx_widget_delete((GX_WIDGET *)root_window);

    /* Clear the root window control block.  */
    memset(root_window, 0, sizeof(GX_WINDOW_ROOT));

    return(GX_SUCCESS);
}

