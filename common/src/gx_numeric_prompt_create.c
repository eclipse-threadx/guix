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
/**   Numeric Prompt Management (Prompt)                                  */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_prompt.h"
#include "gx_numeric_prompt.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_numeric_prompt_create                           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates a numeric prompt, which is a special type of  */
/*    widget.                                                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    prompt                                Numeric prompt control block  */
/*    name                                  Name of numeric prompt        */
/*    parent                                Parent widget control block   */
/*    text_id                               Resource string id            */
/*    style                                 Style of numeric prompt       */
/*    prompt_id                             Application-defined ID of     */
/*                                            Numeric prompt.             */
/*    size                                  Numeric prompt size           */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_prompt_create                     Create a prompt               */
/*    _gx_widget_link                       Link a widget to its parent   */
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
UINT  _gx_numeric_prompt_create(GX_NUMERIC_PROMPT *prompt, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
    GX_RESOURCE_ID text_id, ULONG style, USHORT prompt_id, GX_CONST GX_RECTANGLE *size)
{

    /* Call the prompt create function.  */
    _gx_prompt_create((GX_PROMPT *)prompt, name, GX_NULL, text_id, style, prompt_id, size);

    /* Populate the rest of numeric prompt control block - overriding as necessary.  */
    prompt -> gx_widget_type = GX_TYPE_NUMERIC_PROMPT;
    prompt -> gx_numeric_prompt_format_function = _gx_numeric_prompt_format;

    /* Determine if a parent widget was provided.  */
    if (parent)
    {
        _gx_widget_link(parent, (GX_WIDGET *)prompt);
    }

    return(GX_SUCCESS);
}

