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
/**   Button Management (text_button)                                     */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_button.h"

GX_CALLER_CHECKING_EXTERNS

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_text_button_text_color_set                     PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in the text button text color set   */
/*    function call.                                                      */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_button                           Button control block          */
/*    normal_text_color_id                  Resource ID of normal text    */
/*    selected_text_color_id                Resource ID of selected text  */
/*    disabled_text_color_id                Resource ID of disabled text  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_text_button_text_color_set        Actual text button color      */
/*                                            set function                */
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
UINT  _gxe_text_button_text_color_set(GX_TEXT_BUTTON *text_button,
                                      GX_RESOURCE_ID normal_text_color_id,
                                      GX_RESOURCE_ID selected_text_color_id,
                                      GX_RESOURCE_ID disabled_text_color_id)
{

    /* Check for appropriate caller.  */
    GX_INIT_AND_THREADS_CALLER_CHECKING

    /* Check for invalid input pointers.  */
    if (text_button == GX_NULL)
    {
        return(GX_PTR_ERROR);
    }

    /* Check for valid widget.  */
    if (text_button -> gx_widget_type == 0)
    {
        return GX_INVALID_WIDGET;
    }

    /* Call the actual text button color set function.  */
    _gx_text_button_text_color_set(text_button, normal_text_color_id, selected_text_color_id, disabled_text_color_id);

    /* Return completion status.  */
    return GX_SUCCESS;
}

