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
#include "gx_system.h"
#include "gx_utility.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_binres_resource_header_load                     PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reads resource header from a binary data buffer.      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    info                                  Binary read control block     */
/*    header                                Returned resource header      */
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
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            removed use of memcpy,      */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
#ifdef GX_BINARY_RESOURCE_SUPPORT
UINT _gx_binres_resource_header_load(GX_BINRES_DATA_INFO *info, GX_RESOURCE_HEADER *header)
{
    GX_BINRES_READ_USHORT(header -> gx_resource_header_magic_number, info -> gx_binres_root_address + info -> gx_binres_read_offset);
    info -> gx_binres_read_offset += sizeof(USHORT);

    GX_BINRES_READ_USHORT(header -> gx_resource_header_version, info -> gx_binres_root_address + info -> gx_binres_read_offset);
    info -> gx_binres_read_offset += sizeof(USHORT);

    GX_BINRES_READ_USHORT(header -> gx_resource_header_theme_count, info -> gx_binres_root_address + info -> gx_binres_read_offset);
    info -> gx_binres_read_offset += sizeof(USHORT);

    GX_BINRES_READ_USHORT(header -> gx_resource_header_language_count, info -> gx_binres_root_address + info -> gx_binres_read_offset);
    info -> gx_binres_read_offset += sizeof(USHORT);

    GX_BINRES_READ_ULONG(header -> gx_resource_header_theme_data_size, info -> gx_binres_root_address + info -> gx_binres_read_offset);
    info -> gx_binres_read_offset += sizeof(ULONG);

    GX_BINRES_READ_ULONG(header -> gx_resource_header_string_data_size, info -> gx_binres_root_address + info -> gx_binres_read_offset);
    info -> gx_binres_read_offset += sizeof(ULONG);

    GX_BINRES_READ_ULONG(header -> gx_resource_header_data_size, info -> gx_binres_root_address + info -> gx_binres_read_offset);
    info -> gx_binres_read_offset += sizeof(ULONG);

    return GX_SUCCESS;
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_binres_string_header_load                       PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reads string header from a binary data buffer.        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    info                                  Binary read control block     */
/*    header                                Returned string header        */
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
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            removed use of memcpy,      */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
#ifdef GX_BINARY_RESOURCE_SUPPORT
UINT _gx_binres_string_header_load(GX_BINRES_DATA_INFO *info, GX_STRING_HEADER *header)
{
    GX_BINRES_READ_USHORT(header -> gx_string_header_magic_number, info -> gx_binres_root_address + info -> gx_binres_read_offset);
    info -> gx_binres_read_offset += sizeof(USHORT);

    GX_BINRES_READ_USHORT(header -> gx_string_header_language_count, info -> gx_binres_root_address + info -> gx_binres_read_offset);
    info -> gx_binres_read_offset += sizeof(USHORT);

    GX_BINRES_READ_USHORT(header -> gx_string_header_string_count, info -> gx_binres_root_address + info -> gx_binres_read_offset);
    info -> gx_binres_read_offset += sizeof(USHORT);

    GX_BINRES_READ_ULONG(header -> gx_string_header_data_size, info -> gx_binres_root_address + info -> gx_binres_read_offset);
    info -> gx_binres_read_offset += sizeof(ULONG);

    return GX_SUCCESS;
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_binres_language_header_load                     PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reads language header from a binary data buffer.      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    info                                  Binary read control block     */
/*    header                                Returned language header      */
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
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            removed use of memcpy,      */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
#ifdef GX_BINARY_RESOURCE_SUPPORT
UINT _gx_binres_language_header_load(GX_BINRES_DATA_INFO *info, GX_LANGUAGE_HEADER *header)
{
    GX_BINRES_READ_USHORT(header -> gx_language_header_magic_number, info -> gx_binres_root_address + info -> gx_binres_read_offset);
    info -> gx_binres_read_offset += sizeof(USHORT);

    GX_BINRES_READ_USHORT(header -> gx_language_header_index, info -> gx_binres_root_address + info -> gx_binres_read_offset);
    info -> gx_binres_read_offset += sizeof(USHORT);
 
    memcpy(&header->gx_language_header_name, info->gx_binres_root_address + info->gx_binres_read_offset, sizeof(header->gx_language_header_name)); /* Use case of memcpy is verified. */
    info -> gx_binres_read_offset += sizeof(header -> gx_language_header_name);

    GX_BINRES_READ_ULONG(header -> gx_language_header_data_size, info -> gx_binres_root_address + info -> gx_binres_read_offset);
    info -> gx_binres_read_offset += sizeof(ULONG);

    return GX_SUCCESS;
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_binres_language_table_buffer_allocate           PORTABLE C      */
/*                                                           6.1.7        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function allocates needed memory buffer for loading language   */
/*    table.                                                              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    info                                  Binary resource control block */
/*    language_table_type_size              Size of language table type   */
/*    string_table_type_size                Size of string table type     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_memory_allocator           Application defined memory    */
/*                                            allocation function         */
/*    _gx_binres_resource_header_load       Read binary resource header   */
/*    _gx_binres_string_header_load         Read string data header       */
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
/*  06-02-2021     Kenneth Maxwell          Modified comment(s),          */
/*                                            test for langauge_count == 0*/
/*                                            resulting in version 6.1.7  */
/*                                                                        */
/**************************************************************************/
#ifdef GX_BINARY_RESOURCE_SUPPORT
static UINT _gx_binres_language_table_buffer_allocate(GX_BINRES_DATA_INFO *info, GX_UBYTE language_table_type_size, GX_UBYTE string_table_type_size)
{
GX_RESOURCE_HEADER res_header;
GX_STRING_HEADER   string_header;
USHORT             language_count;
USHORT             string_count;
UINT               language_table_size;
UINT               string_table_size;

    info -> gx_binres_read_offset = 0;

    /* Read resource header.  */
    _gx_binres_resource_header_load(info, &res_header);
    info -> gx_binres_read_offset += res_header.gx_resource_header_theme_data_size;

    if (res_header.gx_resource_header_magic_number != GX_MAGIC_NUMBER)
    {
        return GX_INVALID_FORMAT;
    }

    _gx_binres_string_header_load(info, &string_header);

    if (string_header.gx_string_header_magic_number != GX_MAGIC_NUMBER)
    {
        return GX_INVALID_FORMAT;
    }

    language_count = string_header.gx_string_header_language_count;

    if (language_count == 0)
    {
        return GX_INVALID_FORMAT;
    }
    string_count = string_header.gx_string_header_string_count;

    /* Calcualte memory size needed for string tables. */
    string_table_size = (UINT)(string_count * string_table_type_size);
    GX_UTILITY_MATH_UINT_MULT(string_table_size, language_count, string_table_size)

    /* Calculate memory size needed for language table. */
    language_table_size = (UINT)(language_table_type_size * language_count);

    /* Calculate memory size needed.  */
    GX_UTILITY_MATH_UINT_ADD(string_table_size, language_table_size, info -> gx_binres_buffer_size)

    info -> gx_binres_buffer = (GX_UBYTE *)_gx_system_memory_allocator(info -> gx_binres_buffer_size);

    if (!info -> gx_binres_buffer)
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    memset(info -> gx_binres_buffer, 0, info -> gx_binres_buffer_size);
    info -> gx_binres_buffer_index = 0;

    return GX_SUCCESS;
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_binres_language_table_load                      PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION (deprecated)                                              */
/*                                                                        */
/*    This service loads a language table from a binary data buffer.      */
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
/*    Status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_binres_language_table_buffer_allocate                           */
/*                                          Allocate needed buffer for    */
/*                                            loading language table      */
/*    _gx_binres_resource_header_load                                     */
/*                                          Read resource header          */
/*    _gx_binres_string_header_load         Read string data header       */
/*    _gx_binres_language_header_load       Read language data header     */
/*    _gx_system_memory_free                Application defined memory    */
/*                                            free function               */
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
/*                                            removed use of memcpy,      */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
#ifdef GX_BINARY_RESOURCE_SUPPORT
#ifdef GX_ENABLE_DEPRECATED_STRING_API
UINT _gx_binres_language_table_load(GX_UBYTE *root_address, GX_UBYTE ****returned_language_table)
{
UINT                status;
GX_BINRES_DATA_INFO info;
GX_RESOURCE_HEADER  header;
GX_STRING_HEADER    string_header;
GX_LANGUAGE_HEADER  language_header;
GX_UBYTE         ***language_table;
UINT                lang_index;
UINT                string_index;
USHORT              string_length;
GX_CHAR             get_char;

    memset(&info, 0, sizeof(GX_BINRES_DATA_INFO));

    info.gx_binres_root_address = root_address;

    /* Allocate memory that needed for language table.  */
    status = _gx_binres_language_table_buffer_allocate(&info, sizeof(GX_UBYTE * *), sizeof(GX_UBYTE *));

    /* Read Resource header. */
    info.gx_binres_read_offset = 0;
    _gx_binres_resource_header_load(&info, &header);

    /* Skip theme info.  */
    info.gx_binres_read_offset += header.gx_resource_header_theme_data_size;

    if (header.gx_resource_header_magic_number != GX_MAGIC_NUMBER)
    {
        return GX_INVALID_FORMAT;
    }

    language_table = GX_NULL;

    /* Read language table.  */
    if (status == GX_SUCCESS)
    {
        /* Read string header. */
        _gx_binres_string_header_load(&info, &string_header);

        language_table = (GX_UBYTE ***)(info.gx_binres_buffer + info.gx_binres_buffer_index);
        info.gx_binres_buffer_index += sizeof(GX_UBYTE * *) * string_header.gx_string_header_language_count;

        for (lang_index = 0; lang_index < string_header.gx_string_header_language_count; lang_index++)
        {
            /* Read language header.  */
            _gx_binres_language_header_load(&info, &language_header);

            if (language_header.gx_language_header_magic_number != GX_MAGIC_NUMBER)
            {
                status = GX_INVALID_FORMAT;
                break;
            }

            /* Read string table.  */
            language_table[lang_index] = (GX_UBYTE **)(info.gx_binres_buffer + info.gx_binres_buffer_index);
            info.gx_binres_buffer_index += sizeof(GX_UBYTE *) * string_header.gx_string_header_string_count;

            if (header.gx_resource_header_version >= GX_BINRES_VERSION_ADD_STRING_LENGTH)
            {
                for (string_index = 1; string_index < string_header.gx_string_header_string_count; string_index++)
                {
                    /* Read string length. */
                    GX_BINRES_READ_USHORT(string_length, info.gx_binres_root_address + info.gx_binres_read_offset);
                    info.gx_binres_read_offset += sizeof(USHORT);

                    if (string_length == 0)
                    {
                        language_table[lang_index][string_index] = GX_NULL;
                    }
                    else
                    {
                        language_table[lang_index][string_index] = (GX_UBYTE *)(info.gx_binres_root_address + info.gx_binres_read_offset);
                        info.gx_binres_read_offset += (UINT)(string_length + 1);
                    }
                }
            }
            else
            {
                string_index = 1;
                string_length = 0;
                while (string_index < string_header.gx_string_header_string_count)
                {
                    string_length++;

                    get_char = (GX_CHAR)info.gx_binres_root_address[info.gx_binres_read_offset];
                    info.gx_binres_read_offset++;

                    if (get_char == '\0')
                    {
                        if (string_length == 1)
                        {
                            language_table[lang_index][string_index] = GX_NULL;
                        }
                        else
                        {
                            language_table[lang_index][string_index] = (GX_UBYTE *)(info.gx_binres_root_address + info.gx_binres_read_offset - string_length);
                        }

                        string_length = 0;
                        string_index++;
                    }
                }
            }
        }
    }

    if (status == GX_SUCCESS)
    {
        *returned_language_table = language_table;
    }
    else
    {
        /* Free allocated memory when language loading failed. */
        if (info.gx_binres_buffer)
        {
            _gx_system_memory_free(info.gx_binres_buffer);
        }

        *returned_language_table = GX_NULL;
    }


    return status;
}
#endif
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_binres_language_table_load_ext                  PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service loads a language table from a binary data buffer.      */
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
/*    Status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_binres_language_table_buffer_allocate                           */
/*                                          Allocate needed buffer for    */
/*                                            loading language table      */
/*    _gx_binres_resource_header_load                                     */
/*                                          Read resource header          */
/*    _gx_binres_string_header_load         Read string data header       */
/*    _gx_binres_language_header_load       Read language data header     */
/*    _gx_system_memory_free                Application defined memory    */
/*                                            free function               */
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
#ifdef GX_BINARY_RESOURCE_SUPPORT
UINT _gx_binres_language_table_load_ext(GX_UBYTE *root_address, GX_STRING ***returned_language_table)
{
UINT                status;
GX_BINRES_DATA_INFO info;
GX_RESOURCE_HEADER  header;
GX_STRING_HEADER    string_header;
GX_LANGUAGE_HEADER  language_header;
GX_STRING         **language_table;
UINT                lang_index;
UINT                string_index;
USHORT              string_length;
GX_UBYTE           *get_data;

    memset(&info, 0, sizeof(GX_BINRES_DATA_INFO));

    info.gx_binres_root_address = root_address;

    /* Allocate memory that needed for language table.  */
    status = _gx_binres_language_table_buffer_allocate(&info, sizeof(GX_STRING *), sizeof(GX_STRING));

    /* Read Resource header. */
    info.gx_binres_read_offset = 0;
    _gx_binres_resource_header_load(&info, &header);

    /* Skip theme info.  */
    info.gx_binres_read_offset += header.gx_resource_header_theme_data_size;

    if (header.gx_resource_header_magic_number != GX_MAGIC_NUMBER)
    {
        return GX_INVALID_FORMAT;
    }

    language_table = GX_NULL;

    /* Read language table.  */
    if (status == GX_SUCCESS)
    {
        /* Read string header. */
        _gx_binres_string_header_load(&info, &string_header);

        language_table = (GX_STRING **)(info.gx_binres_buffer + info.gx_binres_buffer_index);
        info.gx_binres_buffer_index += sizeof(GX_STRING *) * string_header.gx_string_header_language_count;

        for (lang_index = 0; lang_index < string_header.gx_string_header_language_count; lang_index++)
        {
            /* Read language header.  */
            _gx_binres_language_header_load(&info, &language_header);

            if (language_header.gx_language_header_magic_number != GX_MAGIC_NUMBER)
            {
                status = GX_INVALID_FORMAT;
                break;
            }

            /* Read string table.  */
            language_table[lang_index] = (GX_STRING *)(info.gx_binres_buffer + info.gx_binres_buffer_index);
            info.gx_binres_buffer_index += sizeof(GX_STRING) * string_header.gx_string_header_string_count;

            for (string_index = 1; string_index < string_header.gx_string_header_string_count; string_index++)
            {
                /* Read string length. */
                get_data = info.gx_binres_root_address + info.gx_binres_read_offset;
                string_length = *(get_data + 1);
                string_length = (USHORT)(string_length << 8);
                string_length = (USHORT)(string_length | (*get_data));
                info.gx_binres_read_offset += sizeof(USHORT);

                if (string_length)
                {
                    language_table[lang_index][string_index].gx_string_ptr = (GX_CHAR *)(info.gx_binres_root_address + info.gx_binres_read_offset);
                }
                else
                {
                    language_table[lang_index][string_index].gx_string_ptr = GX_NULL;
                }

                language_table[lang_index][string_index].gx_string_length = string_length;
                info.gx_binres_read_offset += (UINT)(string_length + 1);
            }
        }
    }

    if (status == GX_SUCCESS)
    {
        *returned_language_table = language_table;
    }
    else
    {
        /* Free allocated memory when language loading failed. */
        if (info.gx_binres_buffer)
        {
            _gx_system_memory_free(info.gx_binres_buffer);
        }

        *returned_language_table = GX_NULL;
    }


    return status;
}
#endif

