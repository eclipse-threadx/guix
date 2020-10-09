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
/*    _gx_utility_ltoa                                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service converts a long integer value into an ASCII string.    */
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
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
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
UINT _gx_utility_ltoa(LONG value, GX_CHAR *return_buffer, UINT return_buffer_size)
{
LONG     temp_val;
GX_CHAR  temp_buf[20];
GX_CHAR *mirror;
GX_CHAR *putchar;
INT      quotient;
INT      sign = 0;

    temp_val = value;

    if (value < 0)
    {
        sign = -1;
        temp_val = -value;
    }

    /* build the string in our temp_buf. The string will
       be in reverse (mirrored) order, we will fix that
       when we copy into the output buffer
     */

    mirror = temp_buf;

    do
    {
        quotient = temp_val % 10;
        temp_val = temp_val / 10;
        quotient = quotient | 0x30;
        *mirror++ = (GX_CHAR)quotient;
    } while (temp_val != 0);

    /* put a negative sign at the end if needed */

    if (sign != 0)
    {
        *mirror++ = '-';
    }

    /* now copy the result back into the caller's buffer,
       being careful not to exceed the buffer size
     */

    mirror--;
    putchar = return_buffer;

    while (mirror >= temp_buf && return_buffer_size > 1)
    {
        *putchar++ = *mirror--;
        return_buffer_size--;
    }

    /* termniate the return string */
    *putchar = 0;
    return GX_SUCCESS;
}

