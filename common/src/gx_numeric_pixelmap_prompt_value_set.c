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
/**   Numeric Pixelmap Prompt Management (Prompt)                         */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_numeric_pixelmap_prompt.h"
#include "gx_system.h"
#include "gx_utility.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_numeric_pixelmap_prompt_value_set               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function sets an integer for a numeric pixelmap prompt widget. */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    prompt                                Numeric pixelmap prompt       */
/*                                            control block               */
/*    value                                 Prompt value to be set        */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    [gx_numeric_pixelmap_prompt_format_function]                        */
/*                                          Value format callback         */
/*    _gx_system_dirty_mark                 Mark a widget as dirty        */
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
UINT _gx_numeric_pixelmap_prompt_value_set(GX_NUMERIC_PIXELMAP_PROMPT *prompt, INT value)
{
UINT status = GX_SUCCESS;
UINT length = 0;

    /* Format int value to string. */
    prompt -> gx_prompt_string.gx_string_ptr = prompt -> gx_numeric_pixelmap_prompt_buffer;
    prompt -> gx_numeric_pixelmap_prompt_format_function(prompt, value);
    status = _gx_utility_string_length_check(prompt->gx_prompt_string.gx_string_ptr, &length, GX_NUMERIC_PROMPT_BUFFER_SIZE - 1);
    if (status != GX_SUCCESS)
    {
        return status;
    }
 
    prompt -> gx_prompt_string.gx_string_length = length;

    prompt -> gx_prompt_text_id = 0;

#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
    if (prompt -> gx_prompt_bidi_resolved_text_info)
    {
        _gx_utility_bidi_resolved_text_info_delete(&prompt->gx_prompt_bidi_resolved_text_info);
    }
#endif

    if (prompt -> gx_widget_status & GX_STATUS_VISIBLE)
    {
        /* Mark widget as dirty. */
        status = _gx_system_dirty_mark((GX_WIDGET *)prompt);
    }

    return status;
}

