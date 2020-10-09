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
/**   Button Management (Button)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_button.h"
#include "gx_system.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_button_select                                   PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function selects one button, invalidating and eventing as      */
/*    necessary.                                                          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    button                                Button control block          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_front_move                 Move widget to the front      */
/*    _gx_button_siblings_deselect          Deselect all the siblings     */
/*    _gx_widget_event_generate             Generate events for widget    */
/*    _gx_system_timer_start                Allocate a free timer and     */
/*                                          activates it                  */
/*    _gx_system_dirty_mark                 Mark the widget dirty         */
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
UINT  _gx_button_select(GX_BUTTON *button)
{
GX_WIDGET *widget = (GX_WIDGET *)button;

    if (button -> gx_widget_style & GX_STYLE_ENABLED)
    {
        if (!(button -> gx_widget_style & GX_STYLE_BUTTON_PUSHED))
        {
            /* Set style for pen down.  */
            button -> gx_widget_style |= GX_STYLE_BUTTON_PUSHED;

            if (widget -> gx_widget_status & GX_STATUS_ACCEPTS_FOCUS)
            {
                _gx_widget_front_move(widget, NULL);
            }

            if (button -> gx_widget_style & GX_STYLE_BUTTON_RADIO)
            {
                _gx_button_siblings_deselect(button);
                _gx_widget_event_generate(widget, GX_EVENT_RADIO_SELECT, 0);
            }
            if (button -> gx_widget_style & GX_STYLE_BUTTON_EVENT_ON_PUSH)
            {
                _gx_widget_event_generate(widget, GX_EVENT_CLICKED, 0);
            }
            else
            {
                if (button -> gx_widget_style & GX_STYLE_BUTTON_TOGGLE)
                {
                    _gx_widget_event_generate(widget, GX_EVENT_TOGGLE_ON, 0);
                }
            }

            if (button -> gx_widget_style & GX_STYLE_BUTTON_REPEAT)
            {
                _gx_system_timer_start(widget, GX_BUTTON_TIMER, GX_REPEAT_BUTTON_INITIAL_TICS, GX_REPEAT_BUTTON_REPEAT_TICS);
            }

            /* Mark as dirty.  */
            _gx_system_dirty_mark(widget);
        }
        else
        {
            if (button -> gx_widget_style & GX_STYLE_BUTTON_TOGGLE)
            {
                button -> gx_widget_status |= GX_STATUS_TOGGLE_UNLOCK;
            }
        }
    }
        return(GX_SUCCESS);

}

