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
/*    _gx_binres_standalone_resource_seek                 PORTABLE C      */
/*                                                           6.3.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function locates the resource data in the binary data memory.  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    info                                  Binary resource control block */
/*    res_index                             The index of the resource to  */
/*                                            be located                  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/**************************************************************************/
#ifdef GX_BINARY_RESOURCE_SUPPORT
UINT _gx_binres_standalone_resource_seek(GX_BINRES_DATA_INFO *info, UINT res_index)
{
USHORT type;
ULONG  count;

    /* Type, version and resource count. */
    if (_gx_binres_range_check(info, info -> gx_binres_read_offset,
                               (sizeof(USHORT) * 2) + sizeof(ULONG)) != GX_SUCCESS)
    {
        return GX_INVALID_FORMAT;
    }

    GX_BINRES_READ_USHORT(type, info -> gx_binres_root_address + info -> gx_binres_read_offset);
    info -> gx_binres_read_offset += sizeof(USHORT);

    if (type != GX_RESOURCE_TYPE_BINRES_STANDALONE)
    {
        return GX_INVALID_FORMAT;
    }

    /* Skip 2 bytes version.  */
    info -> gx_binres_read_offset += sizeof(USHORT);

    GX_BINRES_READ_ULONG(count, info -> gx_binres_root_address + info -> gx_binres_read_offset);
    info -> gx_binres_read_offset += sizeof(ULONG);

    if ((USHORT)res_index >= count)
    {
        return GX_NOT_FOUND;
    }

    if (count > 1)
    {
        /* The offset table, and then the offset it yields, both have to address
           data inside the resource: the second is a file value that becomes the
           position every later read starts from. */
        if (_gx_binres_range_check(info, info -> gx_binres_read_offset + (sizeof(ULONG) * res_index),
                                   sizeof(ULONG)) != GX_SUCCESS)
        {
            return GX_INVALID_FORMAT;
        }

        GX_BINRES_READ_ULONG(info -> gx_binres_read_offset, info -> gx_binres_root_address + info -> gx_binres_read_offset + sizeof(ULONG) * res_index);

        if (_gx_binres_range_check(info, info -> gx_binres_read_offset, 1) != GX_SUCCESS)
        {
            return GX_INVALID_FORMAT;
        }
    }

    return GX_SUCCESS;
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_binres_pixelmap_load                            PORTABLE C      */
/*                                                           6.3.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service loads a pixelmap from a resource data memory.          */
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
/*    Status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_binres_standalone_resource_seek  Locate the resource data       */
/*    _gx_binres_one_pixelmap_load         Load one pixelmap              */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/**************************************************************************/
#ifdef GX_BINARY_RESOURCE_SUPPORT
UINT _gx_binres_pixelmap_load_ext(GX_UBYTE *root_address, ULONG root_size, UINT map_index, GX_PIXELMAP *pixelmap)
{
UINT                status = GX_SUCCESS;
GX_BINRES_DATA_INFO info;

    /* file format
     +--------+
     |        | <-- represents one bytes
     +--------+

     |+========+
     |        | <-- represents a variable number of bytes
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
    info.gx_binres_buffer = (GX_UBYTE *)pixelmap;
    info.gx_binres_buffer_size = sizeof(GX_PIXELMAP);

    status = _gx_binres_standalone_resource_seek(&info, map_index);

    if (status == GX_SUCCESS)
    {
        status = _gx_binres_one_pixelmap_load(&info, GX_NULL, GX_NULL);
    }

    return status;
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_binres_pixelmap_load                            PORTABLE C      */
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
/*    map_index                             Pixelmap index to load        */
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
UINT _gx_binres_pixelmap_load(GX_UBYTE *root_address, UINT map_index, GX_PIXELMAP *pixelmap)
{
    /* A standalone resource carries no total size, only a type, a count and the
       offsets of what it holds, so there is nothing here to derive an extent
       from. The reads are left unbounded, which is what this entry point has
       always done; _gx_binres_pixelmap_load_ext takes the length that bounds them. */
    return _gx_binres_pixelmap_load_ext(root_address, 0, map_index, pixelmap);
}
#endif
#endif
