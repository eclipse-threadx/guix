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
/**   Multi Line Text View Management (Multi Line Text)                   */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_window.h"
#include "gx_multi_line_text_view.h"

GX_CALLER_CHECKING_EXTERNS

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_multi_line_text_view_create                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in the multi line text view create  */
/*    function.                                                           */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_view_ptr                         Multi Line view control block */
/*    name                                  Wigdget name                  */
/*    parent                                Parent widget control block   */
/*    input_buffer                          User-supplied input buffer    */
/*    style                                 The style of the widget border*/
/*    text_view_id                          The ID number of the widget   */
/*    size                                  Parent widget control block   */
/*    text_view_control_block_size          Size of the multi line text   */
/*                                            view control_block          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_multi_line_text_view_create       Actual multi line text view   */
/*                                            create function             */
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
UINT    _gxe_multi_line_text_view_create(GX_MULTI_LINE_TEXT_VIEW *text_view_ptr,
                                         GX_CONST GX_CHAR *name_ptr,
                                         GX_WIDGET *parent,
                                         GX_RESOURCE_ID text_id,
                                         ULONG style,
                                         USHORT Id,
                                         GX_CONST GX_RECTANGLE *size,
                                         UINT text_view_control_block_size)
{
UINT status;

    /* Check for invalid caller of this function.  */
    GX_INIT_AND_THREADS_CALLER_CHECKING

    /* Check for invalid pointer.  */
    if ((text_view_ptr == GX_NULL) || (size == GX_NULL))
    {
        return(GX_PTR_ERROR);
    }

    if (text_view_control_block_size != sizeof(GX_MULTI_LINE_TEXT_VIEW))
    {
        return(GX_INVALID_SIZE);
    }

    /* Check for widget already created.  */
    if (text_view_ptr -> gx_widget_type != 0)
    {
        return(GX_ALREADY_CREATED);
    }

    /* Call actual multi line text input create function.  */
    status = _gx_multi_line_text_view_create(text_view_ptr, name_ptr, parent, text_id, style, Id, size);

    /* Return the error status from window create.  */
    return(status);
}

