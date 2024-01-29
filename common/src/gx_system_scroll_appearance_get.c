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
/**   System Management (System)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_system_scroll_appearance_get                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service gets the scrollbar appearance.                         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    style                                 Scrollbar style               */
/*                                            GX_SCROLLBAR_HORIZONTAL or  */
/*                                            GX_SCROLLBAR_VERTICAL       */
/*    return_appearance                     Pointer to destination for    */
/*                                            appearance.                 */
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
/*    _gx_horizontal_scrollbar_create       Create the horizontal         */
/*                                            scrollbar widget            */
/*    _gx_vertical_scrollbar_create         Create the vertical           */
/*                                            scrollbar widget            */
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
UINT  _gx_system_scroll_appearance_get(ULONG style, GX_SCROLLBAR_APPEARANCE *return_appearance)
{
    if (style & GX_SCROLLBAR_HORIZONTAL)
    {
        *return_appearance = _gx_system_horizontal_scrollbar_appearance;
    }
    else
    {
        *return_appearance = _gx_system_vertical_scrollbar_appearance;
    }

    return(GX_SUCCESS);
}

