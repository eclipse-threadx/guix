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
#include "gx_utility.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_widget_delete_helper                            PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function safely destroys a widget.                             */
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
/*    _gx_widget_unlink                     Unlink a widget               */
/*    _gx_system_private_string_delete      Delete string copy            */
/*    _gx_system_dirty_list_remove          Trims the dirty list          */
/*    _gx_system_timer_stop                 Stop timers owned by widget   */
/*    _gx_widget_free                       Free memory owned by widget   */
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
/*                                            removed private string      */
/*                                            delete,                     */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
static VOID _gx_widget_delete_helper(GX_WIDGET *widget)
{
GX_EVENT delete_event;

    delete_event.gx_event_type = GX_EVENT_DELETE;
    delete_event.gx_event_target = widget;
    widget -> gx_widget_event_process_function(widget, &delete_event);

    _gx_widget_unlink(widget);

    /* now delete top-level widget */
    widget -> gx_widget_type = 0;

    /* this widget cannot have dirty list entries */
    _gx_system_dirty_list_remove(widget);

    /* this widget cannot have timers */
    _gx_system_timer_stop(widget, 0);

    /* this widget cannot have events */
    _gx_system_event_remove(widget);

    /* test for deleting focus widget */
    if (_gx_system_focus_owner == widget)
    {
        _gx_system_focus_owner = GX_NULL;
    }

    if (widget -> gx_widget_status & GX_STATUS_DYNAMICALLY_ALLOCATED)
    {
        _gx_widget_free(widget);
    }
    else
    {
        widget -> gx_widget_id = 0;
        widget -> gx_widget_status = 0;
    }
}


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_widget_delete                                   PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function delete a widget tree.                                 */
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
/*    _gx_system_lock                       Lock access to GUIX           */
/*    _gx_widget_delete_helper              Safely delete widget instance */
/*    _gx_widget_free                       Free memory owned by widget   */
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
UINT  _gx_widget_delete(GX_WIDGET *widget)
{
GX_WIDGET *child;

    /* lock access to GUIX */
    GX_ENTER_CRITICAL

    /* first delete widget children */
    while (widget -> gx_widget_first_child)
    {
        child = widget -> gx_widget_first_child;
        while (child -> gx_widget_first_child)
        {
            child = child -> gx_widget_first_child;
        }

        _gx_widget_delete_helper(child);
    }
    _gx_widget_delete_helper(widget);

    /* Release the protection.  */
    GX_EXIT_CRITICAL

    /* Return successful status.  */
    return(GX_SUCCESS);
}

