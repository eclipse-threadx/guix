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
/**   Single Line Text Input Management (Single Line Text Input)          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_single_line_text_input.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_single_line_text_input_text_color_set           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service sets the text color of a single line text input widget.*/
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    input                                 Single-line text input widget */
/*                                            control block               */
/*    normal_text_color_id                  Resource ID of the normal     */
/*                                            text color                  */
/*    selected_text_color_id                Resource ID of the selected   */
/*                                            text color                  */
/*    disabled_text_color_id                Resource ID of the disabled   */
/*                                            text color                  */
/*    readonly_text_color_id                Resource ID of the read only  */
/*                                            text color                  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
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
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
UINT  _gx_single_line_text_input_text_color_set(GX_SINGLE_LINE_TEXT_INPUT *input,
                                                GX_RESOURCE_ID normal_text_color_id,
                                                GX_RESOURCE_ID selected_text_color_id,
                                                GX_RESOURCE_ID disabled_text_color_id,
                                                GX_RESOURCE_ID readonly_text_color_id)
{

    input -> gx_prompt_normal_text_color = normal_text_color_id;
    input -> gx_prompt_selected_text_color = selected_text_color_id;
    input -> gx_prompt_disabled_text_color = disabled_text_color_id;
    input -> gx_single_line_text_input_readonly_text_color = readonly_text_color_id;

    if (input -> gx_widget_status & GX_STATUS_VISIBLE)
    {
        _gx_system_dirty_mark((GX_WIDGET *)input);
    }

    return(GX_SUCCESS);
}

