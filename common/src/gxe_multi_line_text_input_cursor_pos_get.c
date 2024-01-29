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
/**   Multi Line Text Input Managment (Multi Line Text Input)             */
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
/*    _gxe_multi_line_text_input_cursor_pos_get           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in the multi line text input cursor */
/*    pos get function call.                                              */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    input                                 Multi line text input widget  */
/*                                            control block               */
/*    cursor_pos                            Retrieved cursor position     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_multi_line_text_input_cursor_pos_get                            */
/*                                          Actual multi line text input  */
/*                                            cursor pos get function     */
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
UINT  _gxe_multi_line_text_input_cursor_pos_get(GX_MULTI_LINE_TEXT_INPUT *input, GX_POINT *cursor_pos)
{
UINT status;

    /* Check for invalid pointer. */
    if ((input == GX_NULL) || (cursor_pos == GX_NULL))
    {
        return(GX_PTR_ERROR);
    }

    /* Check for invalid widget. */
    if (input -> gx_widget_type == 0)
    {
        return(GX_INVALID_WIDGET);
    }

    status = _gx_multi_line_text_input_cursor_pos_get(input, cursor_pos);

    return(status);
}

