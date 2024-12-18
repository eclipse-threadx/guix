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
/**   Text Input Management (Single Line Text Input)                      */
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
/*    _gx_single_line_text_input_cut                      PORTABLE C      */
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
/*     input                                Single-line text input widget */
/*                                            control block               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_single_line_text_input_copy      Copy selected text             */
/*    _gx_single_line_text_input_backspace Delete selected text in the    */
/*                                           left of the cursor           */
/*    _gx_single_line_text_input_character_delete                         */
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
UINT _gx_single_line_text_input_cut(GX_SINGLE_LINE_TEXT_INPUT *input)
{
    _gx_single_line_text_input_copy(input);

    if (input -> gx_single_line_text_input_start_mark != input -> gx_single_line_text_input_end_mark)
    {
        if (input -> gx_single_line_text_input_end_mark > input -> gx_single_line_text_input_start_mark)
        {
            _gx_single_line_text_input_backspace(input);
        }
        else
        {
            _gx_single_line_text_input_character_delete(input);
        }
    }

    return GX_SUCCESS;
}

