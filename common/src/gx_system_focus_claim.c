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


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_system_focus_claim                              PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service claims the focus for the specified widget.             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    widget                                Pointer to widget control     */
/*                                            block to claim focus        */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_error_process              Processes internal GUIX       */
/*                                            system errors               */
/*    _gx_widget_child_detect               Detect if one widget is child */
/*                                            of another.                 */
/*    [gx_widget_event_process_function]    Widget event process routine  */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Application                                                    */
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
UINT _gx_system_focus_claim(GX_WIDGET *widget)
{
GX_EVENT   newevent;
GX_WIDGET *parent;
GX_BOOL    detect_result;

    newevent.gx_event_target = GX_NULL;
    /* is this a visible widget? */
    if (widget -> gx_widget_status & GX_STATUS_VISIBLE)
    {
        while (!(widget -> gx_widget_status & GX_STATUS_ACCEPTS_FOCUS))
        {
            widget = widget -> gx_widget_parent;
            if (!widget)
            {
                /* this should not happened. Somehow we have a portion of
                   the widget tree that does not accept focus, including the
                   root window. Return an error.
                 */
                _gx_system_error_process(GX_PTR_ERROR);
                return(GX_PTR_ERROR);
            }
        }

        /* check to see if this widget already owns focus, if so
           just return
         */
        if (widget == _gx_system_focus_owner)
        {
            return(GX_NO_CHANGE);
        }

        /* first figure out who is losing focus */
        if (_gx_system_focus_owner)
        {
            parent = _gx_system_focus_owner;

            /* is the widget gaining focus a child of widget which currently has focus ? */
            _gx_widget_child_detect(parent, widget, &detect_result);

            if (detect_result)
            {
                /* yes, do not send focus lost */
                parent = GX_NULL;
            }
            else
            {
                while (parent -> gx_widget_parent)
                {
                    _gx_widget_child_detect(parent -> gx_widget_parent, widget, &detect_result);

                    /* does this object have same parent as object gaining focus ? */
                    if (detect_result)
                    {
                        /* yes, that's where we stop "losing focus" */
                        break;
                    }
                    /* move up one layer */
                    parent = parent -> gx_widget_parent;
                }
            }

            /* tell the top-most "non common ancestor" that he and his children are losing focus */
            if (parent)
            {
                newevent.gx_event_type = GX_EVENT_FOCUS_LOST;
                parent -> gx_widget_event_process_function(parent, &newevent);
            }
        }

        /* now tell the new owner he has focus */
        _gx_system_focus_owner = widget;
        newevent.gx_event_type = GX_EVENT_FOCUS_GAINED;
        widget -> gx_widget_event_process_function(widget, &newevent);
    }

    /* send event to add focus */
    return GX_SUCCESS;
}

