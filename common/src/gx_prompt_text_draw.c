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
#include "gx_context.h"
#include "gx_widget.h"
#include "gx_prompt.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_prompt_text_draw                                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function draws the specified prompt with text.                 */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    prompt                                Prompt control block          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    [gx_prompt_text_get_function]         Prompt-specified text get     */
/*                                            function                    */
/*    _gx_widget_text_draw                  Draw the text on the widget   */
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
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
VOID  _gx_prompt_text_draw(GX_PROMPT *prompt)
{
GX_WIDGET     *widget = (GX_WIDGET *)prompt;
GX_RESOURCE_ID color;
GX_STRING      pText;

    /* Draw text.  */

    if (widget -> gx_widget_style & GX_STYLE_ENABLED)
    {
        if ((widget -> gx_widget_style & GX_STYLE_DRAW_SELECTED) &&
            prompt -> gx_prompt_selected_text_color != 0)
        {
            color = prompt -> gx_prompt_selected_text_color;
        }
        else
        {
            color = prompt -> gx_prompt_normal_text_color;
        }
    }
    else
    {
        color = prompt -> gx_prompt_disabled_text_color;
    }

    memset((GX_STRING *)&pText, 0, sizeof(GX_STRING));
    prompt -> gx_prompt_text_get_function(prompt, &pText);

    if (pText.gx_string_ptr)
    {
        _gx_widget_text_draw_ext((GX_WIDGET *)prompt, color,
                                 prompt -> gx_prompt_font_id, &pText, 0, 0);
    }
}

