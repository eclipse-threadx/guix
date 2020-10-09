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
/**   Button Management (Checkbox)                                        */
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
/*    _gx_checkbox_select                                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function deselects one button, invalidating and eventing as    */
/*    necessary.                                                          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    checkbox                              Checkbox control block        */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_event_generate             Generate event to notify      */
/*                                             parent widget              */
/*    _gx_system_dirty_mark                 Set the dirty flag            */
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
UINT  _gx_checkbox_select(GX_CHECKBOX *checkbox)
{
GX_WIDGET *widget = (GX_WIDGET *)checkbox;

    if (checkbox -> gx_widget_style & GX_STYLE_ENABLED)
    {
        if (!(checkbox -> gx_widget_style & GX_STYLE_BUTTON_PUSHED))
        {
            checkbox -> gx_widget_style |= GX_STYLE_BUTTON_PUSHED;
            _gx_widget_event_generate(widget, GX_EVENT_TOGGLE_ON, 0);
        }
        else
        {
            checkbox -> gx_widget_style &= ~GX_STYLE_BUTTON_PUSHED;
            _gx_widget_event_generate(widget, GX_EVENT_TOGGLE_OFF, 0);
        }

        /* Mark as dirty.  */
        _gx_system_dirty_mark(widget);

    }

    return(GX_SUCCESS);
}
