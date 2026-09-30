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
/*    _gxe_binres_language_table_load                     PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in binres language table load       */
/*    function.                                                           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    root_address                          Root address of binary        */
/*                                            resource data               */
/*    returned_language_table               Pointer to loaded language    */
/*                                           table                        */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_binres_language_table_load        The actual binres language    */
/*                                            table load function         */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/**************************************************************************/
#ifdef GX_BINARY_RESOURCE_SUPPORT
#ifdef GX_ENABLE_DEPRECATED_BINRES_API
#ifdef GX_ENABLE_DEPRECATED_STRING_API
UINT _gxe_binres_language_table_load(GX_UBYTE *root_address, GX_UBYTE ****returned_language_table)
{
UINT status;

    if ((root_address == GX_NULL) || (returned_language_table == GX_NULL))
    {
        return GX_PTR_ERROR;
    }

    if ((_gx_system_memory_allocator == GX_NULL) ||
        (_gx_system_memory_free == GX_NULL))
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    status = _gx_binres_language_table_load(root_address, returned_language_table);

    /* Return completion status code. */
    return(status);
}
#endif
#endif
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_binres_language_table_load_ext                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in binres language table load       */
/*    function.                                                           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    root_address                          Root address of binary        */
/*                                            resource data               */
/*    returned_language_table               Pointer to loaded language    */
/*                                           table                        */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_binres_language_table_load_ext    The actual binres language    */
/*                                            table load function         */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/**************************************************************************/
#ifdef GX_BINARY_RESOURCE_SUPPORT
#ifdef GX_ENABLE_DEPRECATED_BINRES_API
UINT _gxe_binres_language_table_load_ext(GX_UBYTE *root_address, GX_STRING ***returned_language_table)
{
UINT status;

    if ((root_address == GX_NULL) || (returned_language_table == GX_NULL))
    {
        return GX_PTR_ERROR;
    }

    if ((_gx_system_memory_allocator == GX_NULL) ||
        (_gx_system_memory_free == GX_NULL))
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    status = _gx_binres_language_table_load_ext(root_address, returned_language_table);

    /* Return completion status code. */
    return(status);
}
#endif
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_binres_language_table_load_ext2                PORTABLE C      */
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
/*    _gx_binres_language_table_load_ext2                 */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/**************************************************************************/
#ifdef GX_BINARY_RESOURCE_SUPPORT
UINT _gxe_binres_language_table_load_ext2(GX_UBYTE *root_address, ULONG root_size, GX_STRING ***returned_language_table)
{
UINT  status;

    if ((root_address == GX_NULL) || (returned_language_table == GX_NULL))
    {
        return GX_PTR_ERROR;
    }

    if ((_gx_system_memory_allocator == GX_NULL) ||
        (_gx_system_memory_free == GX_NULL))
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    if (root_size == 0)
    {
        return GX_INVALID_SIZE;
    }

    status = _gx_binres_language_table_load_ext2(root_address, root_size, returned_language_table);

    /* Return completion status code. */
    return(status);
}
#endif
