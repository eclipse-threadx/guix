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

// Portions of this file were generated with AI assistance.


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
/*    _gxe_binres_font_load                               PORTABLE C      */
/*                                                           6.3.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in binres font load function.       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    root_address                          Pointer to the binary data    */
/*                                            memory                      */
/*    font_index                            Resource index of the font    */
/*                                            to be loaded                */
/*    buffer                                Pointer to the buffer to      */
/*                                            store the loaded font       */
/*    buffer_size                           Size of the buffer. It will   */
/*                                            be overwritten with the     */
/*                                            required buffer size if the */
/*                                            input buffer size is        */
/*                                            insufficient                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_binres_font_load                  The actual binres font load   */
/*                                            function                    */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/**************************************************************************/
#ifdef GX_BINARY_RESOURCE_SUPPORT
#ifdef GX_ENABLE_DEPRECATED_BINRES_API
UINT _gxe_binres_font_load(GX_UBYTE *root_address, UINT font_index, GX_UBYTE *buffer, ULONG *buffer_size)
{
    if (root_address == GX_NULL || buffer == GX_NULL || buffer_size == GX_NULL)
    {
        return GX_PTR_ERROR;
    }

    return _gx_binres_font_load(root_address, font_index, buffer, buffer_size);
}
#endif
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_binres_font_load_ext                           PORTABLE C      */
/*                                                           6.5.1        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Eclipse ThreadX contributors                                        */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in the resource load with a length.  */
/*                                                                        */
/*    A length of zero is rejected rather than treated as unknown: a       */
/*    caller reaching this entry point is supplying one, and accepting     */
/*    zero would silently leave every read unbounded.                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    root_address                          Resource address              */
/*    root_size                             Extent of the resource        */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_binres_font_load_ext                            */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/**************************************************************************/
#ifdef GX_BINARY_RESOURCE_SUPPORT
UINT _gxe_binres_font_load_ext(GX_UBYTE *root_address, ULONG root_size, UINT font_index, GX_UBYTE *buffer, ULONG *buffer_size)
{
UINT  status;

    if ((root_address == GX_NULL) || (buffer_size == GX_NULL))
    {
        return GX_PTR_ERROR;
    }

    if (root_size == 0)
    {
        return GX_INVALID_SIZE;
    }

    status = _gx_binres_font_load_ext(root_address, root_size, font_index, buffer, buffer_size);

    /* Return completion status code. */
    return(status);
}
#endif
