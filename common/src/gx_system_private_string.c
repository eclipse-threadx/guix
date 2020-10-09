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
/**   Prompt Management (Prompt)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */
#include "gx_api.h"
#include "gx_system.h"
#include "gx_utility.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_system_private_string_copy                      PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service makes a private copy of text assigned to a widget.     */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    string                                Address of widget's text      */
/*                                            pointer                     */
/*    text                                  Pointer to string             */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_string_length_check       Test string length            */
/*    _gx_system_memory_allocator           Allocate dynamic memory       */
/*    _gx_system_memory_free                Deallocate dynamic memory     */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX internal code                                                  */
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
UINT _gx_system_private_string_copy(GX_STRING *string, GX_CONST GX_STRING *text)
{
GX_UBYTE *byte;
USHORT    current_buffer_size;
USHORT    needed_buffer_size;

    if (_gx_system_memory_allocator == GX_NULL)
    {
        string -> gx_string_ptr = GX_NULL;
        string -> gx_string_length = 0;
        return GX_SYSTEM_MEMORY_ERROR;
    }

    /* are we assigning a non-NULL string? */
    if (text && text -> gx_string_ptr)
    {
        GX_UTILITY_USHORT_CHECK(text -> gx_string_length)
        GX_UTILITY_MATH_USHORT_ADD((USHORT)text -> gx_string_length, 3, needed_buffer_size)
    }
    else
    {
        needed_buffer_size = 0;
    }

    if (needed_buffer_size)
    {
        /* check to see if we have already allocated sufficient memory */
        if (string -> gx_string_ptr)
        {
            byte = (GX_UBYTE *)string -> gx_string_ptr;
            current_buffer_size = *byte++;
            current_buffer_size = (USHORT)(current_buffer_size << 8);
            current_buffer_size = (USHORT)(current_buffer_size | (*byte++));

            if (current_buffer_size < needed_buffer_size)
            {
                /* if our buffer is not large enough, free the current buffer */
                _gx_system_memory_free((void *)string -> gx_string_ptr);

                /* and attempt to allocate a new buffer */
                string -> gx_string_ptr = (GX_CHAR *)_gx_system_memory_allocator(needed_buffer_size);

                /* if buffer allocation succeeded, update our buffer size and copy the string */
                if (string -> gx_string_ptr)
                {
                    byte = (GX_UBYTE *)string -> gx_string_ptr;
                    *byte++ = (GX_UBYTE)(needed_buffer_size >> 8);
                    *byte++ = (GX_UBYTE)(needed_buffer_size & 0xff);

                    memcpy(byte, text -> gx_string_ptr, text -> gx_string_length + 1); /* Use case of memcpy is verified. */
                    string -> gx_string_length = text -> gx_string_length;
                }
                else
                {
                    string -> gx_string_length = 0;
                }
            }
            else
            {
                /* the existing buffer is large enough, just copy the string without updating buffer size */
                memcpy(byte, text -> gx_string_ptr, text -> gx_string_length + 1); /* Use case of memcpy is verified. */
                string -> gx_string_length = text -> gx_string_length;
            }
        }
        else
        {
            /* the current buffer is NULL, attempt to allocate a new buffer */
            string -> gx_string_ptr = (GX_CHAR *)_gx_system_memory_allocator(needed_buffer_size);

            /* if buffer allocation succeeded, update our buffer size and copy the string */
            if (string -> gx_string_ptr)
            {
                byte = (GX_UBYTE *)string -> gx_string_ptr;
                *byte++ = (GX_UBYTE)(needed_buffer_size >> 8);
                *byte++ = (GX_UBYTE)(needed_buffer_size & 0xff);

                memcpy(byte, text -> gx_string_ptr, text -> gx_string_length + 1); /* Use case of memcpy is verified. */
                string -> gx_string_length = text -> gx_string_length;
            }
            else
            {
                string -> gx_string_length = 0;
            }
        }
    }
    else
    {
        /* here if the new text is GX_NULL, free our buffer if one has been allocated */
        if (string -> gx_string_ptr)
        {
            _gx_system_memory_free((void *)string -> gx_string_ptr);
            string -> gx_string_ptr = GX_NULL;
        }

        string -> gx_string_length = 0;
        return GX_SUCCESS;
    }
    if (string -> gx_string_ptr)
    {
        return(GX_SUCCESS);
    }
    return GX_SYSTEM_MEMORY_ERROR;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_system_private_string_list_copy                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service makes a private copy of string list assigned to a      */
/*    widget.                                                             */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    ptr_address                           Address of widget's string    */
/*                                            list pointer                */
/*    string_list                           Pointer to string list        */
/*    string_count                          Number of string in list      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_string_length_check       Test string length            */
/*    _gx_system_memory_allocator           Allocate dynamic memory       */
/*    _gx_system_memory_free                Deallocate dynamic memory     */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX internal code                                                  */
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
UINT _gx_system_private_string_list_copy(GX_CONST GX_CHAR ***ptr_address, GX_CONST GX_CHAR **string_list, INT string_count)
{
UINT      status;
GX_UBYTE *byte;
USHORT    current_buffer_size;
USHORT    needed_buffer_size;
INT       index;
GX_CHAR **list_ptr;
GX_CHAR  *string_ptr;
UINT      length;

    list_ptr = (GX_CHAR **)*ptr_address;

    if (_gx_system_memory_allocator == GX_NULL)
    {
        *ptr_address = GX_NULL;
        return GX_SYSTEM_MEMORY_ERROR;
    }

    /* are we assigning a non-NULL string array? */
    if (string_list)
    {
        /* calculate size of needed buffer. buffer size for holding each string pointer +
                                            2 byte size +
                                            buffer size of each string. */
        GX_UTILITY_USHORT_CHECK(string_count)
        GX_UTILITY_MATH_USHORT_MULT((USHORT)string_count, sizeof(GX_CHAR *), needed_buffer_size)

        /* As string_count is an integer, the maximum needed_buffer_size is 65532,
           so that needed_buffer_size can not be overflow. */
        needed_buffer_size = (USHORT)(needed_buffer_size + 2);

        for (index = 0; index < string_count; index++)
        {
            if (string_list[index])
            {
                status = _gx_utility_string_length_check(string_list[index], &length, GX_MAX_STRING_LENGTH);

                if (status != GX_SUCCESS)
                {
                    return status;
                }

                GX_UTILITY_USHORT_CHECK(length)
                GX_UTILITY_MATH_USHORT_ADD(needed_buffer_size, (USHORT)length, needed_buffer_size)
                GX_UTILITY_MATH_USHORT_ADD(needed_buffer_size, 1, needed_buffer_size)
            }
        }
    }
    else
    {
        needed_buffer_size = 0;
    }

    if (needed_buffer_size)
    {
        /* check to see if we have already allocated sufficient memory */
        if (list_ptr)
        {
            byte = (GX_UBYTE *)list_ptr;
            current_buffer_size = *byte++;
            current_buffer_size = (USHORT)(current_buffer_size << 8);
            current_buffer_size = (USHORT)(current_buffer_size | (*byte++));

            if (current_buffer_size < needed_buffer_size)
            {
                /* if our buffer is not large enough, free the current buffer */
                _gx_system_memory_free((void *)list_ptr);

                /* and attempt to allocate a new buffer */
                list_ptr = (GX_CHAR **)_gx_system_memory_allocator(needed_buffer_size);
            }
        }
        else
        {
            /* the current buffer is NULL, attempt to allocate a new buffer */
            list_ptr = (GX_CHAR **)_gx_system_memory_allocator(needed_buffer_size);
        }

        if (list_ptr)
        {
            *ptr_address = (GX_CONST GX_CHAR **)list_ptr;
            byte = (GX_UBYTE *)list_ptr;
            *byte++ = (GX_UBYTE)(needed_buffer_size >> 8);
            *byte++ = (GX_UBYTE)(needed_buffer_size & 0xff);

            list_ptr = (GX_CHAR **)byte;
            string_ptr = (GX_CHAR *)(byte + ((INT)sizeof(GX_CHAR *)) * string_count);

            for (index = 0; index < string_count; index++)
            {
                list_ptr[index] = string_ptr;

                if (string_list[index])
                {
                    /* Get string length. */
                    _gx_utility_string_length_check(string_list[index], &length, GX_MAX_STRING_LENGTH);

                    memcpy((VOID *)string_ptr, string_list[index], length + 1); /* Use case of memcpy is verified. */
                }
                else
                {
                    *string_ptr = '\0';
                    length = 0;
                }

                string_ptr += length + 1;
            }
        }
    }
    else
    {
        /* here if the new text is GX_NULL, free our buffer if one has been allocated */
        if (list_ptr)
        {
            _gx_system_memory_free((void *)list_ptr);
            *ptr_address = GX_NULL;
        }
        return GX_SUCCESS;
    }
    if (list_ptr)
    {
        return(GX_SUCCESS);
    }
    return GX_SYSTEM_MEMORY_ERROR;
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_system_private_string_list_copy_ext             PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service makes a private copy of string list assigned to a      */
/*    widget.                                                             */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    ptr_address                           Address of widget's string    */
/*                                            list pointer                */
/*    buffer_size                           String list buffer size       */
/*    string_list                           Pointer to string list        */
/*    string_count                          Number of string in list      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_memory_allocator           Allocate dynamic memory       */
/*    _gx_system_memory_free                Deallocate dynamic memory     */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX internal code                                                  */
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
UINT _gx_system_private_string_list_copy_ext(GX_STRING **ptr_address, USHORT *buffer_size, GX_CONST GX_STRING *string_list, INT string_count)
{
USHORT            current_buffer_size;
USHORT            needed_buffer_size;
INT               index;
GX_STRING        *list_ptr;
GX_CONST GX_CHAR *string_ptr;
UINT              length;

    list_ptr = *ptr_address;

    if (_gx_system_memory_allocator == GX_NULL)
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    /* are we assigning a non-NULL string array? */
    if (string_list)
    {
        /* calculate size of needed buffer. buffer size for holding each string pointer +
                                            buffer size of each string. */
        needed_buffer_size = (USHORT)(((INT)sizeof(GX_STRING)) * string_count);
        for (index = 0; index < string_count; index++)
        {
            if (string_list[index].gx_string_ptr)
            {
                GX_UTILITY_USHORT_CHECK(string_list[index].gx_string_length)
                GX_UTILITY_MATH_USHORT_ADD(needed_buffer_size, (USHORT)string_list[index].gx_string_length, needed_buffer_size)
                GX_UTILITY_MATH_USHORT_ADD(needed_buffer_size, 1, needed_buffer_size)
            }
        }
    }
    else
    {
        needed_buffer_size = 0;
    }

    if (needed_buffer_size)
    {
        /* check to see if we have already allocated sufficient memory */
        if (list_ptr)
        {
            current_buffer_size = *buffer_size;

            if (current_buffer_size < needed_buffer_size)
            {
                /* if our buffer is not large enough, free the current buffer */
                _gx_system_memory_free((void *)list_ptr);

                /* and attempt to allocate a new buffer */
                list_ptr = (GX_STRING *)_gx_system_memory_allocator(needed_buffer_size);
                *buffer_size = needed_buffer_size;
            }
        }
        else
        {
            /* the current buffer is NULL, attempt to allocate a new buffer */
            list_ptr = (GX_STRING *)_gx_system_memory_allocator(needed_buffer_size);
            *buffer_size = needed_buffer_size;
        }

        if (list_ptr)
        {
            *ptr_address = list_ptr;

            string_ptr = (GX_CONST GX_CHAR *)(list_ptr + string_count);

            for (index = 0; index < string_count; index++)
            {
                length = string_list[index].gx_string_length;

                list_ptr[index].gx_string_ptr = string_ptr;
                list_ptr[index].gx_string_length = length;

                if (string_list[index].gx_string_ptr)
                {
                    memcpy((VOID *)string_ptr, string_list[index].gx_string_ptr, length + 1); /* Use case of memcpy is verified. */
                    string_ptr += length + 1;
                }
            }
        }
    }
    else
    {
        /* here if the new text is GX_NULL, free our buffer if one has been allocated */
        if (list_ptr)
        {
            _gx_system_memory_free((void *)list_ptr);
            *ptr_address = GX_NULL;
            *buffer_size = 0;
        }
        return GX_SUCCESS;
    }
    if (list_ptr)
    {
        return(GX_SUCCESS);
    }

    return GX_SYSTEM_MEMORY_ERROR;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_system_private_string_get                       PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service returns the string pointer in a dynamically copied     */
/*    string buffer.                                                      */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    buffer                                string buffer address         */
/*    style                                 calling widget style          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_checkbox_draw                                                   */
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
VOID _gx_system_private_string_get(GX_CONST GX_STRING *input, GX_STRING *output, ULONG style)
{
GX_UBYTE *temp;

    output -> gx_string_ptr = GX_NULL;
    output -> gx_string_length  = 0;

    if (input -> gx_string_ptr == GX_NULL)
    {
        return;
    }

    if (style & GX_STYLE_TEXT_COPY)
    {
        /* If the string has been dynamically copied, then the first two bytes of
           the string buffer hold the buffer size. Skip those bytes and return
           pointer to the actual string
         */
        temp = (GX_UBYTE *)input -> gx_string_ptr;
        temp += 2;
        output -> gx_string_ptr = (GX_CHAR *)temp;
        output -> gx_string_length = input -> gx_string_length;
    }
    else
    {
        /* No dynamic copy, the buffer is the string */
        output -> gx_string_ptr = input -> gx_string_ptr;
        output -> gx_string_length = input -> gx_string_length;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_system_private_string_list_get                  PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service returns the string pointer in a dynamically copied     */
/*    string buffer.                                                      */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    buffer                                string buffer address         */
/*    style                                 calling widget style          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_checkbox_draw                                                   */
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
VOID _gx_system_private_string_list_get(GX_CONST GX_CHAR **input, GX_CONST GX_CHAR ***output, ULONG style)
{
GX_UBYTE *temp;

    if (style & GX_STYLE_TEXT_COPY)
    {
        /* If the string has been dynamically copied, then the first two bytes of
            the string buffer hold the buffer size. Skip those bytes and return
            pointer to the actual string
         */
        temp = (GX_UBYTE *)input;
        temp += 2;

        *output = (GX_CONST GX_CHAR **)temp;
    }
    else
    {
        *output = input;
    }
}
#endif

