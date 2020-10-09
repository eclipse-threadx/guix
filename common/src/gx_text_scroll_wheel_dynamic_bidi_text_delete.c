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
/**   Text Scroll Wheel Management (Scroll Wheel)                         */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_scroll_wheel.h"
#include "gx_utility.h"
#include "gx_system.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_text_scroll_wheel_dynamic_bidi_text_delete      PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function releases dynamic bi-directional text of text scroll   */
/*    widget.                                                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    wheel                                 Scroll wheel control block    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_bidi_resolved_text_info_delete                          */
/*                                          Release resolved bidi text    */
/*    _gx_system_memory_free                Release memory                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  09-30-2020     Kenneth Maxwell          Initial Version 6.1           */
/*                                                                        */
/**************************************************************************/
#ifdef GX_DYNAMIC_BIDI_TEXT_SUPPORT
UINT  _gx_text_scroll_wheel_dynamic_bidi_text_delete(GX_TEXT_SCROLL_WHEEL *wheel)
{
UINT status = GX_SUCCESS;
INT  row;

    if (wheel -> gx_text_scroll_wheel_bidi_resolved_text_info)
    {
        for (row = 0; row < wheel -> gx_scroll_wheel_total_rows; row++)
        {
            if(wheel->gx_text_scroll_wheel_bidi_resolved_text_info[row])
            {
                status = _gx_utility_bidi_resolved_text_info_delete(&wheel -> gx_text_scroll_wheel_bidi_resolved_text_info[row]);

                if(status != GX_SUCCESS)
                {
                    return status;
                }
            }
        }

        if (!_gx_system_memory_free)
        {
            return GX_SYSTEM_MEMORY_ERROR;
        }

        _gx_system_memory_free(wheel -> gx_text_scroll_wheel_bidi_resolved_text_info);
        wheel -> gx_text_scroll_wheel_bidi_resolved_text_info = NULL;
    }

    /* Return completion status.  */
    return status;
}
#endif

