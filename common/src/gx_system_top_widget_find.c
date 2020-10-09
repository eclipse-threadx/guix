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
/*    _gx_system_top_widget_find                          PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function finds the top widget that contains the test point.    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    root                                  Widget pointer                */
/*    test_point                            Point to search for           */
/*    status                                Widget status to match        */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    GX_WIDGET                             Pointer to found widget       */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_rectangle_point_detect    Detect point in rectangle     */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_system_event_dispatch             Dispatch GUIX events          */
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
GX_WIDGET  *_gx_system_top_widget_find(GX_WIDGET *root, GX_POINT test_point, ULONG status)
{

GX_WIDGET *test;
GX_WIDGET *winner = GX_NULL;

    /* Setup test widget pointer to the last child.  */
    test =  root -> gx_widget_last_child;

    /* Loop through the widgets in reverse order.  */
    while (test)
    {
        if (test -> gx_widget_status & GX_STATUS_VISIBLE) 
        {
            /* Is the point in this widget?  */
            if (_gx_utility_rectangle_point_detect(&test -> gx_widget_clip, test_point))
            {
                /* Yes, pickup the last child of this widget.  */
                root =  test;
                test =  root -> gx_widget_last_child;

                if (status)
                {
                    if (root -> gx_widget_status & status)
                    {
                        winner = root;
                    }
                }
                else
                {
                    winner = root;
                }
                continue;
            }
        }

        /* Move to the previous widget.  */
        test = test -> gx_widget_previous;
    }

    /* Return the root widget.  */
    return(winner);
}

