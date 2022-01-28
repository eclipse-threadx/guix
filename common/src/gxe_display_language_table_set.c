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
/**   Display Management (Display)                                        */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_display.h"
#include "gx_utility.h"

/* Bring in externs for caller checking code.  */
GX_CALLER_CHECKING_EXTERNS

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_display_language_table_set                     PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in display language table set call. */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    display                               Pointer to display instance.  */
/*    language_table                        The language table to be set  */
/*    num_languages                         Number of languages in the    */
/*                                            table                       */
/*    number_of_strings                     Number of strings in each     */
/*                                            language                    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_display_language_table_set        Actual display language table */
/*                                            set call                    */
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
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT  _gxe_display_language_table_set(GX_DISPLAY *display, GX_CHAR ***language_table,
                                      GX_UBYTE number_of_languages, UINT number_of_strings)
{
UINT status;

    /* Check for invalid caller.  */
    GX_INIT_AND_THREADS_CALLER_CHECKING

    if (display == GX_NULL)
    {
        /* Check for null pointer*/
        return(GX_PTR_ERROR);
    }

    /* Allow string table to be NULL if number_of_strings is zero. */
    if (number_of_strings && language_table == GX_NULL)
    {
        /* Return error.  */
        return(GX_PTR_ERROR);
    }

    /* Call actual system string table set.  */
    status = _gx_display_language_table_set(display, language_table, number_of_languages, number_of_strings);

    /* Return status.  */
    return(status);
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_display_language_table_set_ext                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in display language table set call. */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    display                               Pointer to display instance.  */
/*    table                                 The language table to be set  */
/*    num_languages                         Number of languages in the    */
/*                                            table                       */
/*    number_of_strings                     Number of strings in each     */
/*                                            language                    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_display_language_table_set_ext    Actual display language table */
/*                                            set ext call                */
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
UINT _gxe_display_language_table_set_ext(GX_DISPLAY *display, GX_CONST GX_STRING **table, GX_UBYTE num_languages, UINT number_of_strings)
{
UINT                status;
UINT                language;
UINT                string_id;
GX_CONST GX_STRING *string_table;
GX_CONST GX_STRING *string;
UINT                string_length;

    /* Check for invalid caller.  */
    GX_INIT_AND_THREADS_CALLER_CHECKING

    if (display == GX_NULL)
    {
        /* Check for null pointer*/
        return(GX_PTR_ERROR);
    }

    /* Allow string table to be NULL if number_of_strings is zero. */
    if (number_of_strings)
    {
        if (table == GX_NULL)
        {
            /* Return error.  */
            return(GX_PTR_ERROR);
        }

        for (language = 0; language < num_languages; language++)
        {
            string_table = table[language];
            for (string_id = 0; string_id < number_of_strings; string_id++)
            {
                string = &string_table[string_id];

                if (string -> gx_string_ptr)
                {
                    status = _gx_utility_string_length_check(string -> gx_string_ptr, &string_length, string -> gx_string_length);

                    if (status != GX_SUCCESS)
                    {
                        return status;
                    }
                }
                else
                {
                    string_length = 0;
                }

                if (string_length != string -> gx_string_length)
                {
                    return GX_INVALID_STRING_LENGTH;
                }
            }
        }
    }

    /* Call actual system string table set.  */
    status = _gx_display_language_table_set_ext(display, table, num_languages, number_of_strings);

    /* Return status.  */
    return(status);
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_display_language_direction_table_set           PORTABLE C      */
/*                                                           6.1.10       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in display language direction table */
/*    set call.                                                           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    display                               Pointer to display instance.  */
/*    language_direction_table              The language direction table  */
/*                                            to be set                   */
/*    num_languages                         Number of languages in the    */
/*                                            table                       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_display_language_direciton_table_set                            */
/*                                          Actual display language table */
/*                                            set ext call                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  01-31-2022     Ting Zhu                 Initial Version 6.1.10        */
/*                                                                        */
/**************************************************************************/
#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
UINT _gxe_display_language_direction_table_set(GX_DISPLAY *display, GX_CONST GX_UBYTE *language_direction_table, GX_UBYTE num_languages)
{
UINT status;

    /* Check for invalid caller.  */
    GX_INIT_AND_THREADS_CALLER_CHECKING

    if (display == GX_NULL)
    {
        /* Check for null pointer*/
        return(GX_PTR_ERROR);
    }

    /* Call actual system string table set.  */
    status = _gx_display_language_direction_table_set(display, language_direction_table, num_languages);

    /* Return status.  */
    return(status);
}
#endif
