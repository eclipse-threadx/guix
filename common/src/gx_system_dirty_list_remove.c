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
#include "gx_widget.h"
#include "gx_utility.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_system_dirty_list_remove                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function removes a widget from the dirty list.                 */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    remove                                Widget to be removed          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_child_detect               Detect a child widget         */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_system_dirty_list_remove          Remove widget from dirty list */
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
VOID  _gx_system_dirty_list_remove(GX_WIDGET *remove)
{
GX_CANVAS     *canvas;
GX_DIRTY_AREA *dirty_entry;
UINT           index;

    /* pick up pointer to canvas */

    canvas = _gx_system_canvas_created_list;

    while (canvas)
    {
        /* Setup pointer to dirty list.  */

        dirty_entry = canvas -> gx_canvas_dirty_list;

        /* Check to see if widget already has an entry.  */
        for (index = 0; index < canvas -> gx_canvas_dirty_count; index++)
        {
            /* Is the same widget is present. */
            if (dirty_entry -> gx_dirty_area_widget)
            {
                if (dirty_entry -> gx_dirty_area_widget == remove)
                {
                    dirty_entry -> gx_dirty_area_widget = GX_NULL;
                }
                /* No need to test for the dirty list entry being a child of the widget being deleted,
                   since child widgets are always deleted before the parent.
                */
            }
            dirty_entry++;
        }
        canvas = canvas -> gx_canvas_created_next;
    }
}

