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
#include "gx_display.h"
#include "gx_utility.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_string_get                              PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION  (deprecated)                                             */
/*                                                                        */
/*    This service return the string associated with the specified string */
/*    ID.                                                                 */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    display                               Pointer to display instance   */
/*    string_id                             String resource ID            */
/*    return_string                         Pointer to return string      */
/*                                            pointer                     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*    _gx_context_string_get                                              */
/*    _gx_widget_string_get                 Drew the text onto the wdiget */
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
UINT  _gx_display_string_get(GX_DISPLAY *display, GX_RESOURCE_ID string_id, GX_CONST GX_CHAR **return_string)
{
UINT              status = GX_SUCCESS;
GX_RESOURCE_ID    string_table_size;
GX_CONST GX_CHAR *string_ptr = GX_NULL;
INT               language;

    /* Pickup size of string table.  */
    string_table_size =  display -> gx_display_string_table_size;

    /* Check to make sure the string ID is valid.  */
    if(string_id >= string_table_size)
    {
        status = GX_INVALID_RESOURCE_ID;
    }
    else if (string_id > 0)
    {
        /* Setup return string pointer associated with this ID.  */
        /* first try the active language */
        language = display -> gx_display_active_language;
        if (display -> gx_display_language_table)
        {
            string_ptr = display -> gx_display_language_table[language][string_id].gx_string_ptr;
            if (language && string_ptr == GX_NULL)
            {
                /* If active language string is NULL, return reference language string */
                string_ptr = display -> gx_display_language_table[0][string_id].gx_string_ptr;
            }
        }
        else
        {
            if (display -> gx_display_language_table_deprecated)
            {
                string_ptr = display -> gx_display_language_table_deprecated[language][string_id];
                if (language && string_ptr == GX_NULL)
                {
                    /* If active language string is NULL, return reference language string */
                    string_ptr = display -> gx_display_language_table_deprecated[0][string_id];
                }
            }
        }
    }

    *return_string = string_ptr;
    return status;
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_string_get_ext                          PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service return the string associated with the specified string */
/*    ID.                                                                 */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    display                               Pointer to display instance   */
/*    string_id                             String resource ID            */
/*    return_string                         Pointer to return string      */
/*                                            pointer                     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*    _gx_context_string_get                                              */
/*    _gx_widget_string_get                 Drew the text onto the wdiget */
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
UINT  _gx_display_string_get_ext(GX_DISPLAY *display, GX_RESOURCE_ID string_id, GX_STRING *return_string)
{
GX_RESOURCE_ID string_table_size;
GX_STRING      string;
INT            language;
UINT           status = GX_SUCCESS;

#if defined(GX_ENABLE_DEPRECATED_STRING_API)
GX_CONST GX_CHAR *old_string;
UINT              string_length;
#endif

    string.gx_string_ptr = GX_NULL;
    string.gx_string_length = 0;

#if defined(GX_ENABLE_DEPRECATED_STRING_API)
    /* If application is used the deprecated API to assign string table,
       convert to GX_STRING here
    */
    if (display -> gx_display_language_table_deprecated)
    {
        status = _gx_display_string_get(display, string_id, &old_string);
        if (status == GX_SUCCESS)
        {
            status = _gx_utility_string_length_check(old_string, &string_length, GX_MAX_STRING_LENGTH);
            if (status == GX_SUCCESS)
            {
                string.gx_string_length = string_length;
                string.gx_string_ptr = old_string;
            }
        }
        *return_string = string;
        return status;
    }
#endif
    
    /* Pickup size of string table.  */
    string_table_size =  display -> gx_display_string_table_size;

    /* Check to make sure the string ID is valid.  */
    if(string_id >= string_table_size)
    {
        status = GX_INVALID_RESOURCE_ID;
    }
    else if(string_id > 0)
    {
        if (display -> gx_display_language_table)
        {
            /* Setup return string pointer associated with this ID.  */
            /* first try the active language */
            language = display -> gx_display_active_language;
            string = display -> gx_display_language_table[language][string_id];

            if (language && string.gx_string_ptr == GX_NULL)
            {
                /* If active language string is NULL, return reference language string */
                string = display -> gx_display_language_table[0][string_id];
            }
        }
    }

    *return_string = string;
    return status;
}
