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
/**   Utility (Utility)                                                   */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_utility.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_utility_ltoa                                   PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in the utility ltoa function call.  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    value                                 Long integer value to convert */
/*    return_buffer                         Destination buffer for ASCII  */
/*                                            string                      */
/*    return_buffer_size                    Size of destination buffer    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_ltoa                                                    */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
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
UINT _gxe_utility_ltoa(LONG value, GX_CHAR *return_buffer, UINT return_buffer_size)
{
    /* Check for invalid input pointers.  */
    if (return_buffer == GX_NULL)
    {
        return GX_PTR_ERROR;
    }

    if (return_buffer_size == 0)
    {
        return GX_INVALID_SIZE;
    }

    /* Call actual utility ltoa function.  */
    return(_gx_utility_ltoa(value, return_buffer, return_buffer_size));
}

