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
/**   Vertical List (Lists)                                               */
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
/*    _gx_vertical_list_selected_widget_get               PORTABLE C      */
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
/*    vertical_list                         Vertical list widget control  */
/*                                          block                         */
/*    return_list_entry                     Destination for return list   */
/*                                          entry widget                  */
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
UINT  _gx_vertical_list_selected_widget_get(GX_VERTICAL_LIST *vertical_list,
                                            GX_WIDGET **return_list_entry)
{
INT        page_index;
GX_WIDGET *child = _gx_widget_first_client_child_get((GX_WIDGET *)vertical_list);

    if (vertical_list -> gx_vertical_list_selected < vertical_list -> gx_vertical_list_top_index)
    {
        page_index = vertical_list -> gx_vertical_list_total_rows - vertical_list -> gx_vertical_list_top_index;
        page_index += vertical_list -> gx_vertical_list_selected;
    }
    else
    {
        page_index = vertical_list -> gx_vertical_list_selected - vertical_list -> gx_vertical_list_top_index;
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

