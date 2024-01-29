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
/*    _gx_utility_string_compare                          PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function compares two strings character by character.          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    string_1                              String to be compared         */
/*    string_2                              Another string to be compared */
/*    count                                 number of bytes to compare    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    [GX_TRUE | GX_FALSE]                  [equal | not equal]           */
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
/*  09-30-2020     Kenneth Maxwell          Initial Version 6.1           */
/*                                                                        */
/**************************************************************************/
GX_BOOL _gx_utility_string_compare(GX_CONST GX_STRING *string_1, GX_CONST GX_STRING *string_2, UINT count)
{
UINT index = 0;

    if ((string_1 -> gx_string_length < count) || (string_2 -> gx_string_length < count))
    {
        /* Not equal.*/
        return GX_FALSE;
    }

    while (index < count)
    {
        if (string_1 -> gx_string_ptr[index] != string_2 -> gx_string_ptr[index])
        {
            /* Not equal. */
            return GX_FALSE;
        }

        index++;
    }

    /* Equal. */
    return GX_TRUE;
}

