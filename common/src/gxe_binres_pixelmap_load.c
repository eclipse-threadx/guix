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
/**   Binres Loader Management (Binres Loader)                            */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_binres_loader.h"
#include "gx_system.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_binres_pixelmap_load                           PORTABLE C      */
/*                                                           6.3.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in binres pixelmap load function.   */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    root_address                          Pointer to the binary data    */
/*                                            memory                      */
/*    map_index                             Resource index of the pixelmap*/
/*                                            to be loaded                */
/*    pixelmap                              Pointer to the returned       */
/*                                            pixelmap                    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_binres_pixelmap_load              The actual binres pixelmap    */
/*                                            load function               */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  10-31-2023     Ting Zhu                 Initial Version 6.3.0         */
/*                                                                        */
/**************************************************************************/
UINT _gxe_binres_pixelmap_load(GX_UBYTE *root_address, UINT map_index, GX_PIXELMAP *pixelmap)
{
    if (root_address == GX_NULL || pixelmap == GX_NULL)
    {
        return GX_PTR_ERROR;
    }

    return _gx_binres_pixelmap_load(root_address, map_index, pixelmap);
}

