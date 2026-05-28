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
/**   Scroll Management (Scroll)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE

/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_widget.h"
#include "gx_scrollbar.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_scrollbar_limit_check                           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Check the scrollbar assigned value and make sure it is within the   */
/*    range limits.                                                       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    scrollbar                             Scrollbar control block       */
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
/*                                                                        */
/**************************************************************************/
UINT  _gx_scrollbar_limit_check(GX_SCROLLBAR *scrollbar)
{

    if (scrollbar -> gx_scrollbar_info.gx_scroll_value >
        ((scrollbar -> gx_scrollbar_info.gx_scroll_maximum - scrollbar -> gx_scrollbar_info.gx_scroll_visible) + 1))
    {
        scrollbar -> gx_scrollbar_info.gx_scroll_value =
            (scrollbar -> gx_scrollbar_info.gx_scroll_maximum - scrollbar -> gx_scrollbar_info.gx_scroll_visible) + 1;
    }

    if (scrollbar -> gx_scrollbar_info.gx_scroll_value < scrollbar -> gx_scrollbar_info.gx_scroll_minimum)
    {
        scrollbar -> gx_scrollbar_info.gx_scroll_value = scrollbar -> gx_scrollbar_info.gx_scroll_minimum;
    }

    return(GX_SUCCESS);
}

