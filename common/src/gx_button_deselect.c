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
/*    _gx_button_deselect                                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service deselects the specified button.                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    button                                Pointer to button control     */
/*                                            block                       */
/*    generate_event                        If GX_TRUE, the button will   */
/*                                            generate an event depending */
/*                                            on the button style.        */
/*                                          If GX_FALSE, the button will  */
/*                                            not generate any higher     */
/*                                            level event.                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_timer_stop                 Stop an active GUIX timer     */
/*    _gx_widget_event_generate             Generate event to notify      */
/*                                             parent widget              */
/*    _gx_system_dirty_mark                 Sets the dirty flag           */
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
UINT  _gx_button_deselect(GX_BUTTON *button, GX_BOOL generate_event)
{
GX_WIDGET *widget = (GX_WIDGET *)button;

    if (button -> gx_widget_style & GX_STYLE_BUTTON_PUSHED)
    {
        if (button -> gx_widget_style & GX_STYLE_BUTTON_TOGGLE)
        {
            if (!(button -> gx_widget_status & GX_STATUS_TOGGLE_UNLOCK))
            {
                return GX_SUCCESS;
            }
            button -> gx_widget_status &= ~GX_STATUS_TOGGLE_UNLOCK;
        }

        if (button -> gx_widget_style & GX_STYLE_BUTTON_REPEAT)
        {
            _gx_system_timer_stop(widget, GX_BUTTON_TIMER);
        }

        /* Remove style for pen down.  */
        button -> gx_widget_style &= ~GX_STYLE_BUTTON_PUSHED;
        
        if (generate_event)
        {
            if (button -> gx_widget_style & GX_STYLE_BUTTON_RADIO)
            {
                _gx_widget_event_generate(widget, GX_EVENT_RADIO_DESELECT, 0);
            }
            else
            {
                if (button -> gx_widget_style & GX_STYLE_BUTTON_TOGGLE)
                {
                    _gx_widget_event_generate(widget, GX_EVENT_TOGGLE_OFF, 0);
                }
                else
                {
                    if (!(button -> gx_widget_style & GX_STYLE_BUTTON_EVENT_ON_PUSH))
                    {
                        _gx_widget_event_generate(widget, GX_EVENT_CLICKED, 0);
                    }
                }
            }
        }
    }

    if (button -> gx_widget_status & GX_STATUS_VISIBLE)
    {
        /* Mark as dirty.  */
        _gx_system_dirty_mark(widget);
    }

    return(GX_SUCCESS);
}

