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
/**   Text Input Management (Multi Line Text Input)                       */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_multi_line_text_input.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_input_cut                       PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function cuts the highlight text.                              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*     input                                Multi-line text input widget  */
/*                                            control block               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_multi_line_text_input_copy       Copy selected text             */
/*    _gx_multi_line_text_input_backspace  Delete selected text in the    */
/*                                           left of the cursor           */
/*    _gx_multi_line_text_input_character_delete                          */
/*                                         Delete selected text in the    */
/*                                           right of the cursor          */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
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
UINT _gx_multi_line_text_input_cut(GX_MULTI_LINE_TEXT_INPUT *input)
{

    if (input -> gx_multi_line_text_input_start_mark != input -> gx_multi_line_text_input_end_mark)
    {
        _gx_multi_line_text_input_copy(input);

        if (input -> gx_multi_line_text_input_end_mark > input -> gx_multi_line_text_input_start_mark)
        {
            _gx_multi_line_text_input_backspace(input);
        }
        else
        {
            _gx_multi_line_text_input_delete(input);
        }
    }

    return GX_SUCCESS;
}

