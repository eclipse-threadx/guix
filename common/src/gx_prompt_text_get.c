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
/*    _gx_prompt_text_get                                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION (deprecated)                                              */
/*                                                                        */
/*    This service gets the text of a prompt widget.                      */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    prompt                                Pointer to prompt widget      */
/*                                            control block               */
/*    return_text                           Pointer to destination for    */
/*                                            the text                    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_string_get                 Obtain the string             */
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
UINT _gx_prompt_text_get(GX_PROMPT *prompt, GX_CONST GX_CHAR **return_text)
{
UINT      status;
GX_STRING string;

    string.gx_string_ptr = GX_NULL;
    string.gx_string_length = 0;

    status = _gx_prompt_text_get_ext(prompt, &string);

    if (status == GX_SUCCESS)
    {
        *return_text = string.gx_string_ptr;
    }

    return(status);
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_prompt_text_get_ext                             PORTABLE C      */
/*                                                           6.1.10       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service gets the text of a prompt widget.                      */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    prompt                                Pointer to prompt widget      */
/*                                            control block               */
/*    return_text                           Pointer to destination for    */
/*                                            the text                    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_string_get_ext             Obtain the string             */
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
/*                                            added logic to retrieve     */
/*                                            dynamic bidi text,          */
/*                                            resulting in version 6.1    */
/*  01-31-2022     Ting Zhu                 Modified comment(s),          */
/*                                            updated with new bidi text  */
/*                                            reorder function call,      */
/*                                            resulting in version 6.1.10 */
/*                                                                        */
/**************************************************************************/
UINT _gx_prompt_text_get_ext(GX_PROMPT *prompt, GX_STRING *return_string)
{
UINT status = GX_SUCCESS;

#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
GX_BIDI_TEXT_INFO           text_info;
GX_BIDI_RESOLVED_TEXT_INFO *resolved_info;
GX_CANVAS                  *canvas;
GX_DISPLAY                 *display;
#endif

    if (prompt -> gx_prompt_text_id)
    {
        status = _gx_widget_string_get_ext((GX_WIDGET *)prompt, prompt -> gx_prompt_text_id, return_string);
    }
    else
    {
        _gx_system_private_string_get(&prompt -> gx_prompt_string, return_string, prompt -> gx_widget_style);
    }

#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
    if (_gx_system_bidi_text_enabled)
    {
        if (prompt -> gx_prompt_bidi_resolved_text_info)
        {
            *return_string = *prompt -> gx_prompt_bidi_resolved_text_info -> gx_bidi_resolved_text_info_text;
        }
        else
        {
            text_info.gx_bidi_text_info_text = *return_string;
            text_info.gx_bidi_text_info_font = GX_NULL;
            text_info.gx_bidi_text_info_display_width = -1;
            GX_UTILITY_TEXT_DIRECTION_GET(text_info.gx_bidi_text_info_direction, prompt, canvas, display);

            if (_gx_utility_bidi_paragraph_reorder_ext(&text_info, &resolved_info) == GX_SUCCESS)
            {
                prompt -> gx_prompt_bidi_resolved_text_info = resolved_info;
                *return_string = *resolved_info -> gx_bidi_resolved_text_info_text;
            }
        }
    }
#endif

    return(status);
}

