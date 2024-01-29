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
/**   Rich Text View Management (Rich Text View)                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_rich_text_view.h"

/**************************************************************************/
/*                                                                        */
/*  COMPONENT DEFINITION                                   RELEASE        */
/*                                                                        */
/*    _gx_rich_text_view_context_save                     PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function saves current status of rich text context stack.      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    stack_type                            Stack type                    */
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
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  09-30-2020     Kenneth Maxwell          Initial Version 6.1           */
/*                                                                        */
/**************************************************************************/
UINT  _gx_rich_text_view_context_save()
{
GX_UBYTE top = _gx_system_rich_text_context_stack.gx_rich_text_context_stack_top;

    if (top)
    {
        /* Save system stack to temp stack. */
        memcpy(&_gx_system_rich_text_context_stack_save.gx_rich_text_context_stack, /* Use case of memcpy is verified. */
               &_gx_system_rich_text_context_stack.gx_rich_text_context_stack, sizeof(GX_RICH_TEXT_CONTEXT) * top);
    }

    _gx_system_rich_text_context_stack_save.gx_rich_text_context_stack_top = top;

    return GX_SUCCESS;
}

