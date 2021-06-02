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
#include "gx_button.h"
#include "gx_utility.h"
#include "gx_system.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_text_button_event_process                       PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function processes events for the specified text button.       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    button                                Pointer to text button        */
/*                                            control block               */
/*    event_ptr                             Incoming event to process     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_bidi_resolved_text_info_delete                          */
/*                                          Delete dynamic bidi text      */
/*    _gx_button_event_process              Default widget event process  */
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
/*  09-30-2020     Kenneth Maxwell          Initial Version 6.1           */
/*  06-02-2021     Kenneth Maxwell          Modified comment(s),          */
/*                                            eliminate fall-through,     */
/*                                            resulting in version 6.1.7  */
/*                                                                        */
/**************************************************************************/
UINT  _gx_text_button_event_process(GX_TEXT_BUTTON *button, GX_EVENT *event_ptr)
{

    /* Process relative to the type of event.  */
    switch (event_ptr -> gx_event_type)
    {
    case GX_EVENT_DELETE:
        if ((button -> gx_widget_style & GX_STYLE_TEXT_COPY) && button -> gx_text_button_string.gx_string_ptr)
        {
            if (!_gx_system_memory_free)
            {
                return GX_SYSTEM_MEMORY_ERROR;
            }
            _gx_system_memory_free((void *)button -> gx_text_button_string.gx_string_ptr);
            button -> gx_text_button_string.gx_string_ptr = GX_NULL;
            button -> gx_text_button_string.gx_string_length = 0;
        }
#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
        if (button -> gx_text_button_bidi_resolved_text_info)
        {
            _gx_utility_bidi_resolved_text_info_delete(&button -> gx_text_button_bidi_resolved_text_info);
        }
#endif
        break;

    default:

        /* Do nothing.  */
        break;
    }

    /* Return completion status.  */
    return _gx_button_event_process((GX_BUTTON*)button, event_ptr);
}

