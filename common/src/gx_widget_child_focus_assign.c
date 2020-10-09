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
#include "gx_system.h"
#include "gx_widget.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_widget_child_focus_assign                       PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function processes events for the specified widget.            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    widget                                Widget control block          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    gx_system_focus_claim()                                             */
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
VOID _gx_widget_child_focus_assign(GX_WIDGET *parent)
{
GX_WIDGET *winner = GX_NULL;
GX_WIDGET *test = parent -> gx_widget_first_child;

    /* does any child have default focus status? */
    while (test)
    {
        if (test -> gx_widget_status & GX_STATUS_DEFAULT_FOCUS)
        {
            winner = test;

            if (winner -> gx_widget_status & GX_STATUS_NAV_PARENT)
            {
                break;
            }
            else
            {
                /* keep drilling down to children of children */
                test = winner -> gx_widget_first_child;
            }
        }
        else
        {
            test = test -> gx_widget_next;
        }
    }

    if (winner)
    {
        if (!(winner -> gx_widget_status & GX_STATUS_HAS_FOCUS) ||
            winner != _gx_system_focus_owner)
        {
            _gx_system_focus_claim(winner);
        }
        return;
    }

    /* nothing has default focus flag, so just try to give focus to first child that accepts focus */
    test = parent -> gx_widget_first_child;

    while (test)
    {
        if ((test -> gx_widget_status & GX_STATUS_ACCEPTS_FOCUS) &&
            !(test -> gx_widget_status & GX_STATUS_NONCLIENT))
        {
            winner = test;

            if (winner -> gx_widget_status & GX_STATUS_NAV_PARENT)
            {
                break;
            }
            else
            {
                test = winner -> gx_widget_first_child;
            }
        }
        else
        {
            test = test -> gx_widget_next;
        }
    }

    if (winner && !(winner -> gx_widget_status & GX_STATUS_HAS_FOCUS))
    {
        _gx_system_focus_claim(winner);
    }
}

