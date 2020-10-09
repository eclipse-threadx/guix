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
/**   Text Button Management (Button)                                     */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_button.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_button_event_process            PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates a multi-line text button, which is a special  */
/*    type of button (widget).                                            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    button                                Button control block          */
/*    name                                  Name of button                */
/*    parent                                Parent widget control block   */
/*    text_id                               text resource id              */
/*    style                                 Style of button               */
/*    size                                  Button size                   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_text_button_event_process                                       */
/*    _gx_multi_line_text_button_line_pointers_set                        */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    System and Application Code                                         */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            updated default call,       */
/*                                            added logic to handle new   */
/*                                            events,                     */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
UINT  _gx_multi_line_text_button_event_process(GX_MULTI_LINE_TEXT_BUTTON *button, GX_EVENT *event_ptr)
{
UINT status;

    /* Default status to success.  */
    status =  GX_SUCCESS;

    /* Process relative to the type of event.  */
    switch (event_ptr -> gx_event_type)
    {
    case GX_EVENT_SHOW:
    case GX_EVENT_LANGUAGE_CHANGE:
        status = _gx_text_button_event_process((GX_TEXT_BUTTON *)button, event_ptr);
        _gx_multi_line_text_button_line_pointers_set(button);
        break;

#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
    case GX_EVENT_DYNAMIC_BIDI_TEXT_ENABLE:
    case GX_EVENT_DYNAMIC_BIDI_TEXT_DISABLE:
        _gx_multi_line_text_button_line_pointers_set(button);
        break;
#endif

    default:

        /* Call the widget default processing.  */
        status = _gx_text_button_event_process((GX_TEXT_BUTTON *)button, event_ptr);
    }

    /* Return completion status.  */
    return(status);
}

