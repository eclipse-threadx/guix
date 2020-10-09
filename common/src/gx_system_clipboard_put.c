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
/**   System Management (System)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_system_clipboard_put                            PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service puts data to clipboard.                                */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    data                                  Pointer the copied data       */
/*    data_size                             The number of bytes to copy   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_memory_allocator           Application defined memory    */
/*                                            allocation function         */
/*    _gx_system_memory_free                Application defined memory    */
/*                                            free function               */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
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
UINT  _gx_system_clipboard_put(VOID *data, UINT data_size)
{

    if ((!_gx_system_memory_allocator) ||
        (!_gx_system_memory_free))
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    if (_gx_system_clipboard)
    {
        _gx_system_memory_free(_gx_system_clipboard);
    }

    _gx_system_clipboard = _gx_system_memory_allocator(data_size);

    /* Verify the memory allocation was successful. */
    if(_gx_system_clipboard == GX_NULL)
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    _gx_system_clipboard_size = data_size;

    memcpy(_gx_system_clipboard, data, data_size); /* Use case of memcpy is verified. */

    return(GX_SUCCESS);
}

