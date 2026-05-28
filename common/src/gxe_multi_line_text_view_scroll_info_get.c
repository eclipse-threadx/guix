/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation
 * Copyright (c) 2026 Eclipse ThreadX contributors
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
/**   Multi Line Text View Managment (Multi Line Text View)               */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_multi_line_text_view.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_multi_line_text_view_scroll_info_get           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in the multi line text view scroll  */
/*    info get function call.                                             */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_view                            Multi line text view widget    */
/*                                           control block                */
/*    style                                 Style                         */
/*    return_scroll_info                   Pointer to destination for     */
/*                                           scroll info                  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_multi_line_text_view_text_set     Actual multi line text view   */
/*                                            text set fucntion           */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/**************************************************************************/
UINT _gxe_multi_line_text_view_scroll_info_get(GX_MULTI_LINE_TEXT_VIEW *text_view, ULONG style, GX_SCROLL_INFO *return_scroll_info)
{
UINT status;

    if ((text_view == GX_NULL) || (return_scroll_info == GX_NULL))
    {
        return(GX_PTR_ERROR);
    }

    status = _gx_multi_line_text_view_scroll_info_get(text_view, style, return_scroll_info);

    return(status);
}

