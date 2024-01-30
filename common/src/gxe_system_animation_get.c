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

/* Bring in externs for caller checking code.  */
GX_CALLER_CHECKING_EXTERNS

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_system_animation_get                           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in gx_system_animaton_get call      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    animation                             Pointer to recieve pointer    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*   _gx_system_animation_get               Actual animation get call     */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Application                                                    */
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
#if (GX_ANIMATION_POOL_SIZE > 0)

UINT  _gxe_system_animation_get(GX_ANIMATION **animation)
{
UINT status;

    /* Check for invalid caller.  */
    GX_INIT_AND_THREADS_CALLER_CHECKING

    /* Check for invalid pointer.  */
    if (animation == GX_NULL)
    {
        return(GX_PTR_ERROR);
    }

    /* Call actual animation get  */
    status = _gx_system_animation_get(animation);

    /* Return status.  */
    return(status);
}
#endif
