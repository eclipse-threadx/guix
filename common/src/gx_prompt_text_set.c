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
/**   Prompt Management (Prompt)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_display.h"
#include "gx_widget.h"
#include "gx_prompt.h"
#include "gx_utility.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_prompt_text_set                                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION (deprecated)                                              */
/*                                                                        */
/*    This service sets the text of a prompt widget.                      */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    prompt                                Pointer to prompt widget      */
/*                                            control block               */
/*    text                                  Pointer to text               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_prompt_text_set_ext               New text set API              */
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
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT  _gx_prompt_text_set(GX_PROMPT *prompt, GX_CONST GX_CHAR *text)
{
UINT      status = GX_SUCCESS;
UINT      length = 0;
GX_STRING string;

    if (text)
    {
        status = _gx_utility_string_length_check(text, &length, GX_MAX_STRING_LENGTH);
    }

    if (status == GX_SUCCESS)
    {
        string.gx_string_ptr = text;
        string.gx_string_length = length;
        status = _gx_prompt_text_set_ext(prompt, &string);
    }

    return(status);
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_prompt_text_set_ext                             PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service sets the text of a prompt widget.                      */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    prompt                                Pointer to prompt widget      */
/*                                            control block               */
/*    text                                  Pointer to text               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_dirty_mark                 Mark this prompt as dirty     */
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
UINT  _gx_prompt_text_set_ext(GX_PROMPT *prompt, GX_CONST GX_STRING *string)
{
UINT status = GX_SUCCESS;

    prompt -> gx_prompt_text_id = 0;

    if (prompt -> gx_widget_style & GX_STYLE_TEXT_COPY)
    {
        status = _gx_system_private_string_copy(&prompt -> gx_prompt_string, string);
    }
    else
    {
        if (string)
        {
            prompt -> gx_prompt_string = *string;
        }
        else
        {
            prompt -> gx_prompt_string.gx_string_ptr = GX_NULL;
            prompt -> gx_prompt_string.gx_string_length = 0;
        }
    }

#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
    if (prompt -> gx_prompt_bidi_resolved_text_info)
    {
        _gx_utility_bidi_resolved_text_info_delete(&prompt -> gx_prompt_bidi_resolved_text_info);
    }
#endif

    if (prompt -> gx_widget_status & GX_STATUS_VISIBLE)
    {
        _gx_system_dirty_mark((GX_WIDGET *)prompt);
    }

    return(status);
}

