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
/**   Multi Line Text Input Management (Multi Line Text Input)            */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_multi_line_text_input.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_input_buffer_get                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function retrieves buffer infomation of a multi-line text input*/
/*    widget.                                                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_input                            Multi-line text input widget  */
/*                                            control block               */
/*    buffer_address                        The address of the input      */
/*                                            buffer                      */
/*    content_size                          The byte count of the input   */
/*                                            data                        */
/*    buffer_size                           The size of the input buffer. */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
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
UINT _gx_multi_line_text_input_buffer_get(GX_MULTI_LINE_TEXT_INPUT *text_input, GX_CHAR **buffer_address,
                                          UINT *content_size, UINT *buffer_size)
{
    if (buffer_address)
    {
        *buffer_address = (GX_CHAR *)text_input->gx_multi_line_text_view_text.gx_string_ptr;
    }

    if (content_size)
    {
        *content_size = text_input->gx_multi_line_text_view_text.gx_string_length;
    }

    if (buffer_size)
    {
        *buffer_size = text_input->gx_multi_line_text_input_buffer_size;
    }
    return GX_SUCCESS;
}

