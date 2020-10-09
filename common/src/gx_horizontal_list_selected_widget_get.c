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
/**   Horizontal List (List)                                              */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_window.h"
#include "gx_system.h"
#include "gx_scrollbar.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_horizontal_list_selected_widget_get             PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service gets the list entry at the current list index.         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    horizontal_list                       Horizontal list widget        */
/*                                            control block               */
/*    return_list_entry                     Destination for return list   */
/*                                            entry widget                */
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
UINT  _gx_horizontal_list_selected_widget_get(GX_HORIZONTAL_LIST *horizontal_list,
                                              GX_WIDGET **return_list_entry)
{
INT        page_index;
GX_WIDGET *child = _gx_widget_first_client_child_get((GX_WIDGET *)horizontal_list);

    if (horizontal_list -> gx_horizontal_list_selected < horizontal_list -> gx_horizontal_list_top_index)
    {
        page_index = horizontal_list -> gx_horizontal_list_total_columns - horizontal_list -> gx_horizontal_list_top_index;
        page_index += horizontal_list -> gx_horizontal_list_selected;
    }
    else
    {
        page_index = horizontal_list -> gx_horizontal_list_selected - horizontal_list -> gx_horizontal_list_top_index;
    }

    while (child && page_index > 0)
    {
        child = _gx_widget_next_client_child_get(child);
        page_index--;
    }

    *return_list_entry = child;

    if (child)
    {
        return GX_SUCCESS;
    }
    return GX_FAILURE;
}

