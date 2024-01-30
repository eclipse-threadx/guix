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
/**   Context Management (Context)                                        */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_context.h"

/* Bring in externs for caller checking code.  */
GX_CALLER_CHECKING_EXTERNS

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_context_pixelmap_get                           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in context pixelmap set call.       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    pixelmap_id                           Pixelmap resource ID          */
/*    return_pixelmap                       Pointer to pixelmap           */
/*                                            destination pointer         */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_context_pixelmap_get              Actual context pixelmap get   */
/*                                            call                        */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*    GUIX default draw funtions                                          */
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
UINT _gxe_context_pixelmap_get(GX_RESOURCE_ID pixelmap_id, GX_PIXELMAP **return_pixelmap)
{
UINT             status;

    /* Check for invalid pointer.  */
    if (return_pixelmap == GX_NULL)
    {
        return(GX_PTR_ERROR);
    }

    if (_gx_system_current_draw_context == GX_NULL)
    {
        *return_pixelmap = GX_NULL;
        return GX_INVALID_CONTEXT;
    }

    /* Call actual pixelmap.  */
    status = _gx_context_pixelmap_get(pixelmap_id, return_pixelmap);

    /* Return status.  */
    return(status);
}

