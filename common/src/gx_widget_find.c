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
/**   Widget Management (Widget)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_widget_find                                     PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service searches for the specified widget.                     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    parent                                Pointer to parent widget to   */
/*                                          start search from             */
/*    widget_id                             Widget ID                     */
/*    search_depth                          how many generations to search*/
/*    return_widget                         Pointer to destination for    */
/*                                            found widget                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_find                       Find the specified widget     */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
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
UINT _gx_widget_find(GX_WIDGET *parent, USHORT widget_id, INT search_depth, GX_WIDGET **return_widget)
{
GX_WIDGET *child;

    *return_widget = GX_NULL;

    /* Is there a widget?  */
    if (parent)
    {
        child = parent -> gx_widget_first_child;

        while (child)
        {
            if (child -> gx_widget_id == widget_id)
            {
                *return_widget = child;
                return(GX_SUCCESS);
            }
            if (search_depth > 0)
            {
                if (child -> gx_widget_first_child)
                {
                    child = child -> gx_widget_first_child;
                    search_depth--;
                    continue;
                }
            }

            while ((child -> gx_widget_next == GX_NULL) && (child != parent))
            {
                search_depth++;
                child = child -> gx_widget_parent;
            }

            if (child == parent)
            {
                break;
            }

            child = child -> gx_widget_next;
        }
    }
    return(GX_NOT_FOUND);
}

