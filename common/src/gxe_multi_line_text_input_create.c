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
/**   Multi Line Text Input Management (Multi Line Text)                  */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_multi_line_text_input.h"
#include "gx_multi_line_text_view.h"

/* Bring in externs for caller checking code.  */
GX_CALLER_CHECKING_EXTERNS

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_multi_line_text_input_create                   PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in the multi line text input        */
/*  create.                                                               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_input_ptr                        Multi Line input control block*/
/*    nampe_ptr                             Wigdget name                  */
/*    parent                                Parent widget control block   */
/*    size                                  Parent widget control block   */
/*    input_buffer                          User-supplied input buffer    */
/*    buffer_size                           Size of the user-supplied     */
/*                                            input buffer                */
/*    style                                 The style of the widget border*/
/*    Id                                    The ID number of the widget   */
/*    text_input_control_block_size         Size of the multi line text   */
/*                                            input control block         */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_multi_line_text_input_create                                    */
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
UINT  _gxe_multi_line_text_input_create(GX_MULTI_LINE_TEXT_INPUT *text_input_ptr,
                                        GX_CONST GX_CHAR *name_ptr,
                                        GX_WIDGET *parent,
                                        GX_CHAR *input_buffer,
                                        UINT buffer_size,
                                        ULONG style,
                                        USHORT Id,
                                        GX_CONST GX_RECTANGLE *size,
                                        UINT text_input_control_block_size)
{
UINT status;

    /* Check for invalid caller of this function.  */
    GX_INIT_AND_THREADS_CALLER_CHECKING

    /* Check for invalid pointer.  */
    if ((text_input_ptr == GX_NULL) || (size == GX_NULL))
    {
        return(GX_PTR_ERROR);
    }

    /* Check for invalid control block size. */
    if (text_input_control_block_size != sizeof(GX_MULTI_LINE_TEXT_INPUT))
    {
        return(GX_INVALID_SIZE);
    }

    /* Check for widget already created.  */
    if (text_input_ptr -> gx_widget_type != 0)
    {
        return(GX_ALREADY_CREATED);
    }

    /* Check for invalid parent widget.  */
    if (parent && (parent -> gx_widget_type == 0))
    {
        return(GX_INVALID_WIDGET);
    }

    /* Call actual multi line text input create function.  */
    status = _gx_multi_line_text_input_create(text_input_ptr, name_ptr, parent, input_buffer, buffer_size, style, Id, size);

    /* Return completion status.  */
    return(status);
}

