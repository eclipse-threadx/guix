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
#include "gx_pixelmap_prompt.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_pixelmap_prompt_create                          PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates a pixelmap prompt, which is a special type of */
/*    widget.                                                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    prompt                                Prompt control block          */
/*    name                                  Name of prompt                */
/*    parent                                Parent widget control block   */
/*    text_id                               Resource string id            */
/*    fill_id                               pixelmap id for fill area     */
/*    style                                 Style of checkbox             */
/*    pixelmap_prompt_id                    Application-defined ID of     */
/*                                            pixelmap prompt             */
/*    size                                  Prompt size                   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_prompt_create                     Create the underlying prompt  */
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
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
UINT  _gx_pixelmap_prompt_create(GX_PIXELMAP_PROMPT *prompt,
                                 GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                                 GX_RESOURCE_ID text_id, GX_RESOURCE_ID fill_id,
                                 ULONG style, USHORT pixelmap_prompt_id,
                                 GX_CONST GX_RECTANGLE *size)

{

    /* Call the base prompt create function.  */
    _gx_prompt_create((GX_PROMPT *)prompt, name, GX_NULL, text_id, style, pixelmap_prompt_id, size);

    /* Populate the rest of prompt control block - overriding as necessary.  */
    prompt -> gx_widget_type = GX_TYPE_PIXELMAP_PROMPT;
    prompt -> gx_normal_fill_pixelmap_id = fill_id;
    prompt -> gx_normal_left_pixelmap_id = 0;
    prompt -> gx_normal_right_pixelmap_id = 0;

    prompt -> gx_selected_fill_pixelmap_id = 0;
    prompt -> gx_selected_left_pixelmap_id = 0;
    prompt -> gx_selected_right_pixelmap_id = 0;

    prompt -> gx_widget_draw_function =  (VOID (*)(GX_WIDGET *))_gx_pixelmap_prompt_draw;

    /* Determine if a parent widget was provided.  */
    if (parent)
    {
        _gx_widget_link(parent, (GX_WIDGET *)prompt);
    }

    /* Return the status from prompt create.  */
    return(GX_SUCCESS);
}

