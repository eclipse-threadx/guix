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
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  10-31-2023     Ting Zhu                 Initial Version 6.3.0         */
/*                                                                        */
/**************************************************************************/
UINT _gx_binres_font_load(GX_UBYTE *root_address, UINT font_index, GX_UBYTE *buffer, ULONG *buffer_size)
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

