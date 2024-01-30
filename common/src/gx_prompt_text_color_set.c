/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation 
 * 
 * This program and the accompanying materials are made available under the
 * terms of the MIT License which is available at
 * https://opensource.org/licenses/MIT.
 * 
 * SPDX-License-Identifier: MIT
 **************************************************************************/


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


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_prompt_text_color_set                           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service sets the text color of a prompt widget.                */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    prompt                                Pointer to prompt control     */
/*                                             block                      */
/*    normal_text_color_id                  Resource ID of the normal     */
/*                                             text color                 */
/*    selected_text_color_id                Resource ID of the selected   */
/*                                             text color                 */
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
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
UINT  _gx_prompt_text_color_set(GX_PROMPT *prompt,
                                GX_RESOURCE_ID normal_text_color_id,
                                GX_RESOURCE_ID selected_text_color_id,
                                GX_RESOURCE_ID disabled_text_color_id)
{

    prompt -> gx_prompt_normal_text_color = normal_text_color_id;
    prompt -> gx_prompt_selected_text_color = selected_text_color_id;
    prompt -> gx_prompt_disabled_text_color = disabled_text_color_id;

    if (prompt -> gx_widget_status & GX_STATUS_VISIBLE)
    {
        _gx_system_dirty_mark((GX_WIDGET *)prompt);
    }

    return(GX_SUCCESS);
}

