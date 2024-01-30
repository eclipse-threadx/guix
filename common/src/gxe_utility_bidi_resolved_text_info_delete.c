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
/**   Utility (Utility)                                                   */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_utility.h"

GX_CALLER_CHECKING_EXTERNS

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_utility_bidi_resolved_text_info_delete         PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in the utility bidi resolved text   */
/*    info delete function call.                                          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    reordered_text                        The head pointer of reordered */
/*                                            bidi text information link  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_bidi_resolved_text_info_delete                          */
/*                                          Actual utility bidi resolved  */
/*                                            text into delete function   */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  12-31-2020     Kenneth Maxwell          Initial Version 6.1.3         */
/*                                                                        */
/**************************************************************************/
#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
UINT _gxe_utility_bidi_resolved_text_info_delete(GX_BIDI_RESOLVED_TEXT_INFO **resolved_info_head)
{
UINT status;

    /* Check for invalid input pointers.  */
    if (resolved_info_head == GX_NULL)
    {
        return GX_PTR_ERROR;
    }

    /* Call the actual utility pixelmap resize function.  */
    status = _gx_utility_bidi_resolved_text_info_delete(resolved_info_head);

    return status;
}
#endif

