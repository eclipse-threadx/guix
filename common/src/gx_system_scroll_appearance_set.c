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
/*    _gx_system_scroll_appearance_set                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service sets the default scrollbar appearance.                 */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    style                                 Scrollbar style               */
/*                                            GX_SCROLLBAR_HORIZONTAL or  */
/*                                            GX_SCROLLBAR_VERTICAL       */
/*    appearance                            Pointer to appearance         */
/*                                            structure initialized with  */
/*                                            various scrollbar           */
/*                                            appearance attributes.      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                  Completion status             */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/**************************************************************************/
UINT _gx_system_scroll_appearance_set(ULONG style, GX_SCROLLBAR_APPEARANCE *appearance)
{
    if (style & GX_SCROLLBAR_HORIZONTAL)
    {
        _gx_system_horizontal_scrollbar_style = style;
        _gx_system_horizontal_scrollbar_appearance = *appearance;
    }
    else
    {
        _gx_system_vertical_scrollbar_style = style;
        _gx_system_vertical_scrollbar_appearance = *appearance;
    }

    return(GX_SUCCESS);
}

