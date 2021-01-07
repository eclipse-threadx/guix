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
/**   Text Input Management (Single Line Text Input)                      */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_single_line_text_input.h"

/* Bring in externs for caller checking code.  */
GX_CALLER_CHECKING_EXTERNS

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_single_line_text_input_create                  PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in the single line text input       */
/*    create call.                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_input                        Single-line text input widget     */
/*                                        control block                   */
/*    name                              Name of text input widget         */
/*    parent                            Pointer to parent widget          */
/*    input_buffer                      Pointer to text input buffer      */
/*    buffer_size                       Size of text input buffer         */
/*    style                             Style of text input widget.       */
/*    text_input_id                     Application-defined ID for text   */
/*                                        input                           */
/*    size                              Dimensions of text input widget   */
/*    text_input_control_block_size     Size of the single line text      */
/*                                        input control block             */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                            Completion status                 */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_single_line_text_input_create                                   */
/*                                      Actual single line text input     */
/*                                        widget create call              */
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
/*  12-31-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            removed input buffer check, */
/*                                            resulting in version 6.1.3  */
/*                                                                        */
/**************************************************************************/

UINT  _gxe_single_line_text_input_create(GX_SINGLE_LINE_TEXT_INPUT *text_input,
                                         GX_CONST GX_CHAR *name,
                                         GX_WIDGET *parent,
                                         GX_CHAR *input_buffer,
                                         UINT buffer_size,
                                         UINT style,
                                         USHORT text_input_id,
                                         GX_CONST GX_RECTANGLE *size,
                                         UINT text_input_control_block_size)
{
UINT status;

    /* Check for invalid caller.  */
    GX_INIT_AND_THREADS_CALLER_CHECKING

    /* Check for invalid input pointers.  */
    if ((text_input == GX_NULL) || (size == GX_NULL))
    {
        return(GX_PTR_ERROR);
    }

    if (text_input_control_block_size != sizeof(GX_SINGLE_LINE_TEXT_INPUT))
    {
        return(GX_INVALID_SIZE);
    }

    /* Check for already created. */
    if (text_input -> gx_widget_type != 0)
    {
        return(GX_ALREADY_CREATED);
    }

    if (parent && (parent -> gx_widget_type == 0))
    {
        return(GX_INVALID_WIDGET);
    }

    /* Call actual single line text input create function.  */
    status = _gx_single_line_text_input_create(text_input, name, parent, input_buffer, buffer_size, style, text_input_id, size);

    /* Return completion status.  */
    return(status);
}

