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
/*    _gx_string_scroll_wheel_string_id_list_set          PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function assigns a string id list for the string scroll wheel. */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    wheel                                 Scroll wheel control block    */
/*    string_id_list                        String id list to be set      */
/*    id_count                              The number of string ids      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_memory_free                Application defined memory    */
/*                                            free function               */
/*    _gx_scroll_wheel_total_rows_set       Set scroll wheel total rows   */
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
/*                                            added logic to delete       */
/*                                            dynamic bidi text,          */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
UINT _gx_string_scroll_wheel_string_id_list_set(GX_STRING_SCROLL_WHEEL *wheel,
                                                GX_CONST GX_RESOURCE_ID *string_id_list,
                                                INT id_count)
{
UINT status;

    if (wheel -> gx_widget_style & GX_STYLE_TEXT_COPY)
    {
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
        if (wheel -> gx_string_scroll_wheel_string_list_deprecated)
        {
            if (_gx_system_memory_free == GX_NULL)
            {
                return GX_SYSTEM_MEMORY_ERROR;
            }

            _gx_system_memory_free((void *)wheel -> gx_string_scroll_wheel_string_list_deprecated);
        }
#endif

        if (wheel -> gx_string_scroll_wheel_string_list)
        {
            if (_gx_system_memory_free == GX_NULL)
            {
                return GX_SYSTEM_MEMORY_ERROR;
            }

            _gx_system_memory_free((void *)wheel -> gx_string_scroll_wheel_string_list);
        }
    }

    wheel -> gx_string_scroll_wheel_string_id_list = string_id_list;
    wheel -> gx_string_scroll_wheel_string_list = GX_NULL;
    wheel -> gx_string_scroll_wheel_string_list_buffer_size = 0;
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
    wheel -> gx_string_scroll_wheel_string_list_deprecated = GX_NULL;
#endif

#ifdef GX_DYNAMIC_BIDI_TEXT_SUPPORT
    _gx_text_scroll_wheel_dynamic_bidi_text_delete((GX_TEXT_SCROLL_WHEEL *)wheel);
#endif  // GX_DYNAMIC_BIDI_TEXT_SUPPORT

    status = _gx_scroll_wheel_total_rows_set((GX_SCROLL_WHEEL *)wheel, id_count);

    return status;
}

