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
#include "gx_utility.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_binres_font_load                                PORTABLE C      */
/*                                                           6.3.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service loads a font from a resource data memory.              */
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
/*    Status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_binres_standalone_resource_seek  Locate the resource data       */
/*    _gx_binres_font_buffer_size_get      Get the required font buffer   */
/*                                            size                        */
/*    _gx_binres_one_font_load             Load one font                  */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/**************************************************************************/
#ifdef GX_BINARY_RESOURCE_SUPPORT
UINT _gx_binres_font_load_ext(GX_UBYTE *root_address, ULONG root_size, UINT font_index, GX_UBYTE *buffer, ULONG *buffer_size)
{
UINT                status = GX_SUCCESS;
GX_BINRES_DATA_INFO info;
UINT                required_size;

    /* file format
     +--------+
     |        | <-- represents one bytes
     +--------+

     |+========+
     |         | <-- represents a variable number of bytes
     |+========+

     |+--------+--------+--------+--------+
     |    magic number  | resource count  |
     |+--------+--------+--------+--------+
     |+--------+--------+--------+--------+
     |         resource offset            |
     |+--------+--------+--------+--------+
     |+--------+--------+--------+--------+
     |              ...                   |
     |+--------+--------+--------+--------+
     |+===================================+
     |         resource data              |
     |+===================================+
     */

    memset(&info, 0, sizeof(GX_BINRES_DATA_INFO));

    info.gx_binres_root_address = (GX_UBYTE *)root_address;
    info.gx_binres_root_size = root_size;
    info.gx_binres_buffer = (GX_UBYTE *)buffer;
    info.gx_binres_buffer_size = *buffer_size;

    status = _gx_binres_standalone_resource_seek(&info, font_index);

    if (status != GX_SUCCESS)
    {
        return status;
    }

    status = _gx_binres_font_buffer_size_get(&info, &required_size, GX_TRUE);

    if (status != GX_SUCCESS)
    {
        return status;
    }

    if (required_size > *buffer_size)
    {
        *buffer_size = required_size;
        return GX_INVALID_MEMORY_SIZE;
    }

    status = _gx_binres_one_font_load(&info, GX_NULL);

    return status;
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_binres_font_load                                PORTABLE C      */
/*                                                           6.5.1        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Eclipse ThreadX contributors                                        */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function is the form that takes no resource length.             */
/*                                                                        */
/*                                                                        */
/*    A standalone resource declares no total size, so nothing can be      */
/*    derived here and the reads stay unbounded, as they have always been. */
/*    Callers that know the length should use the _ext form, which bounds  */
/*    every read by it.                                                    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    root_address                          Resource address              */
/*    font_index                            Font index to load            */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_binres_declared_size_get          Derive the resource extent    */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/**************************************************************************/
#ifdef GX_BINARY_RESOURCE_SUPPORT
#ifdef GX_ENABLE_DEPRECATED_BINRES_API
UINT _gx_binres_font_load(GX_UBYTE *root_address, UINT font_index, GX_UBYTE *buffer, ULONG *buffer_size)
{
    /* A standalone resource carries no total size, only a type, a count and the
       offsets of what it holds, so there is nothing here to derive an extent
       from. The reads are left unbounded, which is what this entry point has
       always done; _gx_binres_font_load_ext takes the length that bounds them. */
    return _gx_binres_font_load_ext(root_address, 0, font_index, buffer, buffer_size);
}
#endif
#endif
