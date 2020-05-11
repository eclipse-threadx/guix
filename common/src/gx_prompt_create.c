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
#include "gx_widget.h"
#include "gx_prompt.h"
#include "gx_system.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_prompt_create                                   PORTABLE C      */
/*                                                           6.0          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates a prompt, which is a special type of          */
/*    widget.                                                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    prompt                                Prompt control block          */
/*    name                                  Name of prompt                */
/*    parent                                Parent widget control block   */
/*    text_id                               Resource string id            */
/*    style                                 Style of prompt               */
/*    prompt_id                             Application-defined ID of     */
/*                                            prompt.                     */
/*    size                                  Prompt size                   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_create                     Create the underlying widget  */
/*    _gx_widget_link                       Link the widget to its parent */
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
/*                                                                        */
/**************************************************************************/
UINT  _gx_prompt_create(GX_PROMPT *prompt, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                        GX_RESOURCE_ID text_id, ULONG style, USHORT prompt_id,
                        GX_CONST GX_RECTANGLE *size)
{

    /* Call the widget create function.  */
    _gx_widget_create((GX_WIDGET *)prompt, name, GX_NULL, style, prompt_id, size);

    /* Populate the rest of prompt control block - overriding as necessary.  */
    prompt -> gx_widget_type =                  GX_TYPE_PROMPT;
    prompt -> gx_prompt_text_id =               text_id;
    prompt -> gx_prompt_normal_text_color =     GX_COLOR_ID_TEXT;
    prompt -> gx_prompt_selected_text_color =   GX_COLOR_ID_SELECTED_TEXT;
    prompt -> gx_prompt_disabled_text_color =   GX_COLOR_ID_DISABLED_TEXT;
    prompt -> gx_prompt_string.gx_string_ptr =  GX_NULL;
    prompt -> gx_prompt_string.gx_string_length = 0;
    prompt -> gx_prompt_font_id =               GX_FONT_ID_PROMPT;
    prompt -> gx_widget_draw_function =         (VOID (*)(GX_WIDGET *))_gx_prompt_draw;
    prompt -> gx_prompt_text_get_function =     (VOID (*)(GX_PROMPT *, GX_STRING *))_gx_prompt_text_get_ext;

    /* Determine if a parent widget was provided.  */
    if (parent)
    {
        _gx_widget_link(parent, (GX_WIDGET *)prompt);
    }

    /* Return the status from widget create.  */
    return(GX_SUCCESS);
}

