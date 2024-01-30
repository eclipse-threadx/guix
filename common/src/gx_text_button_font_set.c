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
/**   Text Button Management (Button)                                     */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_button.h"



/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_text_button_font_set                            PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function assigns a font to the specified button.               */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    button                                Button control block          */
/*    font_id                               Resource ID for the font      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_dirty_mark                 <ark this prompt as dirty     */
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
UINT  _gx_text_button_font_set(GX_TEXT_BUTTON *button, GX_RESOURCE_ID font_id)
{

    button -> gx_text_button_font_id = font_id;

    if (button -> gx_widget_status & GX_STATUS_VISIBLE)
    {
        _gx_system_dirty_mark((GX_WIDGET *)button);
    }

    return(GX_SUCCESS);
}

