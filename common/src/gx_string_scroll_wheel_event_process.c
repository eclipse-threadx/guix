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
/**   String Scroll Wheel Management (Scroll Wheel)                       */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_scroll_wheel.h"
#include "gx_system.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_string_scroll_wheel_event_process               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function processes the comming events for a string scroll      */
/*    wheel widget.                                                       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    wheel                                 Text scroll wheel control     */
/*                                            block                       */
/*    event_ptr                             Event to be processed         */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_memory_free                Memory free function          */
/*    _gx_scroll_wheel_event_process        Default event process         */
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
/*                                                                        */
/**************************************************************************/
UINT _gx_string_scroll_wheel_event_process(GX_STRING_SCROLL_WHEEL *wheel, GX_EVENT *event_ptr)
{

    switch (event_ptr -> gx_event_type)
    {
    case GX_EVENT_DELETE:
        if (wheel -> gx_widget_style & GX_STYLE_TEXT_COPY)
        {
            if (wheel -> gx_string_scroll_wheel_string_list)
            {
                if (!_gx_system_memory_free)
                {
                    return GX_SYSTEM_MEMORY_ERROR;
                }
                _gx_system_memory_free((void *)wheel -> gx_string_scroll_wheel_string_list);
                wheel -> gx_string_scroll_wheel_string_list = GX_NULL;
            }
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
            if (wheel -> gx_string_scroll_wheel_string_list_deprecated)
            {
                if (!_gx_system_memory_free)
                {
                    return GX_SYSTEM_MEMORY_ERROR;
                }

                _gx_system_memory_free((void *)wheel -> gx_string_scroll_wheel_string_list_deprecated);
                wheel -> gx_string_scroll_wheel_string_list_deprecated = GX_NULL;
            }
#endif
            wheel -> gx_string_scroll_wheel_string_list_buffer_size = 0;
        }
        break;

    default:
        /* Do nothing. */
        break;
    }

    return _gx_text_scroll_wheel_event_process((GX_TEXT_SCROLL_WHEEL*)wheel, event_ptr);;
}

