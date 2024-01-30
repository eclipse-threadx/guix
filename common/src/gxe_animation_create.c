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
/**   Window Management (Window)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_animation.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_animation_create                               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in the animation create call.       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    animation                             Pointer to window control     */
/*                                            block                       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_animation_create                  Actual animation create call. */
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
UINT  _gxe_animation_create(GX_ANIMATION *animation)
{
UINT status;

    /* Check for invalid input pointers.  */
    if (animation == GX_NULL)
    {
        return(GX_PTR_ERROR);
    }

    if (animation -> gx_animation_status != 0)
    {
        return GX_ALREADY_CREATED;
    }

    /* Call actual window create function.  */
    status = _gx_animation_create(animation);

    /* Return completion status.  */
    return(status);
}

