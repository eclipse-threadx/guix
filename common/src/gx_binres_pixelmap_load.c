/**************************************************************************/
/*                                                                        */
/*       Copyright (c) Microsoft Corporation. All rights reserved.        */
/*                                                                        */
/*       This software is licensed under the Microsoft Software License   */
/*       Terms for Microsoft Azure RTOS. Full text of the license can be  */
/*       found in the LICENSE file at https://aka.ms/AzureRTOS_EULA       */
/*       and in the root directory of this software.                      */
/*                                                                        */
/**************************************************************************/


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
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  10-31-2023     Ting Zhu                 Initial Version 6.3.0         */
/*                                                                        */
/**************************************************************************/
UINT _gx_binres_standalone_resource_seek(GX_BINRES_DATA_INFO *info, UINT res_index)
{
USHORT type;
ULONG  count;

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
        GX_BINRES_READ_ULONG(info -> gx_binres_read_offset, info -> gx_binres_root_address + info -> gx_binres_read_offset + sizeof(ULONG) * res_index);
    }

    return GX_SUCCESS;
}

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
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  10-31-2023     Ting Zhu                 Initial Version 6.3.0         */
/*                                                                        */
/**************************************************************************/
UINT _gx_binres_pixelmap_load(GX_UBYTE *root_address, UINT map_index, GX_PIXELMAP *pixelmap)
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
    info.gx_binres_buffer = (GX_UBYTE *)pixelmap;
    info.gx_binres_buffer_size = sizeof(GX_PIXELMAP);

    status = _gx_binres_standalone_resource_seek(&info, map_index);

    if (status == GX_SUCCESS)
    {
        status = _gx_binres_one_pixelmap_load(&info, GX_NULL, GX_NULL);
    }

    return status;
}

