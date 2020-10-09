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
#include "gx_utility.h"
#include "gx_system.h"
#include "gx_widget.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_system_widget_find                              PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service searches for the specified widget ID.                  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    widget_id                            Widget ID to search for        */
/*    search_level                         Specifies the depth of the     */
/*                                           search                       */
/*    return_search_result                 Pointer to destination for     */
/*                                           widget found                 */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                               Completion status              */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_find                      Search throught its children   */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Application Code                                               */
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
UINT _gx_system_widget_find(USHORT widget_id, INT search_level, GX_WIDGET **return_search_result)
{

GX_WIDGET *test;

    /* start at first root window */
    test = (GX_WIDGET *)_gx_system_root_window_created_list;

    while (test)
    {
        /* check to see if they are looking for a root window */
        if (test -> gx_widget_id == widget_id)
        {
            *return_search_result = test;
            return GX_SUCCESS;
        }
        /* check the children of this root window */
        if (test -> gx_widget_first_child)
        {
            /* yes, check to see if the requested Id is a child */
            if (_gx_widget_find(test, widget_id, search_level, return_search_result) == GX_SUCCESS)
            {
                return GX_SUCCESS;
            }
        }
        /* advance to next entry in list */
        test = test -> gx_widget_next;
    }
    return GX_NOT_FOUND;
}

