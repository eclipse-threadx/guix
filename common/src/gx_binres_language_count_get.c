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
/*    _gx_binres_language_count_get                       PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function retrives language count of specified binary resource. */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    root_address                          Root address of binary        */
/*                                            resource data               */
/*    header                                Returned Language count       */
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
/*    Application Code                                                    */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/**************************************************************************/
#ifdef GX_BINARY_RESOURCE_SUPPORT
UINT _gx_binres_language_count_get_ext(GX_UBYTE *root_address, ULONG root_size, GX_VALUE *put_count)
{
UINT                status = GX_SUCCESS;
GX_BINRES_DATA_INFO info;
GX_RESOURCE_HEADER  header;
GX_STRING_HEADER    string_header;

    memset(&info, 0, sizeof(GX_BINRES_DATA_INFO));

    info.gx_binres_root_address = root_address;
    info.gx_binres_root_size = root_size;

    /* Read Resource header. */
    info.gx_binres_read_offset = 0;
    status = _gx_binres_resource_header_load(&info, &header);

    if (status != GX_SUCCESS)
    {
        return status;
    }

    /* Skip theme info.  */
    info.gx_binres_read_offset += header.gx_resource_header_theme_data_size;

    if (header.gx_resource_header_magic_number != GX_MAGIC_NUMBER)
    {
        return GX_INVALID_FORMAT;
    }

    /* Read string header. */
    status = _gx_binres_string_header_load(&info, &string_header);

    if (status != GX_SUCCESS)
    {
        return status;
    }

    if (string_header.gx_string_header_magic_number != GX_MAGIC_NUMBER)
    {
        status = GX_INVALID_FORMAT;
    }
    else
    {
        *put_count = (GX_VALUE)(string_header.gx_string_header_language_count);
    }

    return status;
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_binres_language_count_get                       PORTABLE C      */
/*                                                           6.5.1        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Eclipse ThreadX contributors                                        */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function is the form that takes no resource length. It derives  */
/*    one from the size the resource declares and defers to language_coun */
/*                                                                        */
/*    A truncated resource is rejected, because its declared size still    */
/*    describes the whole of it. A resource built to mislead is not: the   */
/*    same attacker chose that size. Callers that can supply the real      */
/*    length should use the _ext form, which bounds every read by it.      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    root_address                          Resource address              */
/*    put_count                             Destination for the count     */
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
UINT _gx_binres_language_count_get(GX_UBYTE *root_address, GX_VALUE *put_count)
{
ULONG root_size;
UINT  status;

    status = _gx_binres_declared_size_get(root_address, &root_size);

    if (status != GX_SUCCESS)
    {
        return status;
    }

    return _gx_binres_language_count_get_ext(root_address, root_size, put_count);
}
#endif
#endif
