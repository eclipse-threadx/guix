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
#include "gx_window.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_window_scrollbar_find                           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service finds the scrollbar for the specified window.          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    window                                Pointer to window             */
/*    type                                  GX_SCROLLBAR_HORIZONTAL or    */
/*                                            GX_SCROLLBAR_VERTICAL       */
/*    return_scrollbar                      Pointer to destination for    */
/*                                            scrollbar                   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
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
UINT _gx_window_scrollbar_find(GX_WINDOW *window, USHORT type, GX_SCROLLBAR **return_scrollbar)
{
GX_WIDGET *child = window -> gx_widget_first_child;

    while (child)
    {
        if ((child -> gx_widget_type == type) && (child -> gx_widget_status & GX_STATUS_NONCLIENT))
        {
            *return_scrollbar = (GX_SCROLLBAR *)child;
            return(GX_SUCCESS);
        }
        child = child -> gx_widget_next;
    }
    *return_scrollbar = GX_NULL;
    return(GX_NOT_FOUND);
}

