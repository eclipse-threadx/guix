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
/**************************************************************************/
/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_binres_range_check                              PORTABLE C      */
/*                                                           6.5.1        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Eclipse ThreadX contributors                                        */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks that length bytes at the given offset lie       */
/*    inside the resource.                                                 */
/*                                                                        */
/*    The counts and offsets that drive every read come from the resource  */
/*    itself, so a resource declaring more data than it holds would        */
/*    otherwise be read past its end. An extent of zero means the caller   */
/*    supplied no length, and nothing can be checked.                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    info                                  Binary resource data info     */
/*    offset                                Offset the read starts at     */
/*    length                                Number of bytes to be read    */
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
/*    GUIX Internal Code                                                  */
/*                                                                        */
/**************************************************************************/
#ifdef GX_BINARY_RESOURCE_SUPPORT
UINT _gx_binres_range_check(GX_BINRES_DATA_INFO *info, ULONG offset, ULONG length)
{
    if (info -> gx_binres_root_size == 0)
    {
        return GX_SUCCESS;
    }

    /* Written as a subtraction so that the sum cannot wrap past the extent. */
    if ((offset > info -> gx_binres_root_size) ||
        (length > (info -> gx_binres_root_size - offset)))
    {
        return GX_INVALID_FORMAT;
    }

    return GX_SUCCESS;
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_binres_declared_size_get                        PORTABLE C      */
/*                                                           6.5.1        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Eclipse ThreadX contributors                                        */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function derives the extent of a resource from the size its     */
/*    own header declares.                                                */
/*                                                                        */
/*    It is what the entry points that take no length fall back on. A      */
/*    resource that is merely truncated is caught, because the declared    */
/*    size still describes the whole of it. A resource built to mislead    */
/*    is not, because the same attacker chose that size: only a length     */
/*    from the caller bounds that, which is what the _ext entry points     */
/*    take. The header itself is read before anything is known, so the     */
/*    caller must supply at least that much.                              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    root_address                          Resource address              */
/*    returned_size                         Destination for the extent    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_binres_resource_header_load       Read resource header          */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/**************************************************************************/
#ifdef GX_BINARY_RESOURCE_SUPPORT
UINT _gx_binres_declared_size_get(GX_UBYTE *root_address, ULONG *returned_size)
{
GX_BINRES_DATA_INFO info;
GX_RESOURCE_HEADER  header;

    memset(&info, 0, sizeof(GX_BINRES_DATA_INFO));
    info.gx_binres_root_address = root_address;

    /* The extent is not known yet, so the header read cannot be refused. */
    (VOID)_gx_binres_resource_header_load(&info, &header);

    if (header.gx_resource_header_magic_number != GX_MAGIC_NUMBER)
    {
        return GX_INVALID_FORMAT;
    }

    if (header.gx_resource_header_data_size == 0)
    {
        /* A resource that declares no size says nothing about its extent, and
           older ones leave the field empty. Reporting the extent as unknown
           keeps those loading, rather than rejecting every read past the
           header. */
        *returned_size = 0;

        return GX_SUCCESS;
    }

    if (header.gx_resource_header_data_size > (~(ULONG)0 - GX_RESOURCE_HEADER_SIZE))
    {
        return GX_MATH_OVERFLOW;
    }

    *returned_size = header.gx_resource_header_data_size + GX_RESOURCE_HEADER_SIZE;

    return GX_SUCCESS;
}
#endif

#ifdef GX_BINARY_RESOURCE_SUPPORT
UINT _gx_binres_resource_header_load(GX_BINRES_DATA_INFO *info, GX_RESOURCE_HEADER *header)
{
    /* The resource declares what follows, so the bytes this header occupies are
       checked to be present before any of them is read. */
    if (_gx_binres_range_check(info, info -> gx_binres_read_offset, GX_RESOURCE_HEADER_SIZE) != GX_SUCCESS)
    {
        return GX_INVALID_FORMAT;
    }

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
/**************************************************************************/
#ifdef GX_BINARY_RESOURCE_SUPPORT
UINT _gx_binres_string_header_load(GX_BINRES_DATA_INFO *info, GX_STRING_HEADER *header)
{
    /* The resource declares what follows, so the bytes this header occupies are
       checked to be present before any of them is read. */
    if (_gx_binres_range_check(info, info -> gx_binres_read_offset, GX_STRING_HEADER_SIZE) != GX_SUCCESS)
    {
        return GX_INVALID_FORMAT;
    }

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
/**************************************************************************/
#ifdef GX_BINARY_RESOURCE_SUPPORT
UINT _gx_binres_language_header_load(GX_BINRES_DATA_INFO *info, GX_LANGUAGE_HEADER *header)
{
    /* The resource declares what follows, so the bytes this header occupies are
       checked to be present before any of them is read. */
    if (_gx_binres_range_check(info, info -> gx_binres_read_offset, GX_LANGUAGE_HEADER_SIZE) != GX_SUCCESS)
    {
        return GX_INVALID_FORMAT;
    }

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
UINT               status;

    info -> gx_binres_read_offset = 0;

    /* Read resource header.  */
    status = _gx_binres_resource_header_load(info, &res_header);

    if (status != GX_SUCCESS)
    {
        return status;
    }
    info -> gx_binres_read_offset += res_header.gx_resource_header_theme_data_size;

    if (res_header.gx_resource_header_magic_number != GX_MAGIC_NUMBER)
    {
        return GX_INVALID_FORMAT;
    }

    status = _gx_binres_string_header_load(info, &string_header);

    if (status != GX_SUCCESS)
    {
        return status;
    }

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
/*    _gx_binres_declared_size_get          Get declared resource size    */
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
/**************************************************************************/
#ifdef GX_BINARY_RESOURCE_SUPPORT
#ifdef GX_ENABLE_DEPRECATED_BINRES_API
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

    /* No length is given, so the resource is bounded by the size it declares. */
    status = _gx_binres_declared_size_get(root_address, &info.gx_binres_root_size);

    if (status != GX_SUCCESS)
    {
        return status;
    }

    /* Allocate memory that needed for language table.  */
    status = _gx_binres_language_table_buffer_allocate(&info, sizeof(GX_UBYTE * *), sizeof(GX_UBYTE *));

    if (status != GX_SUCCESS)
    {
        return status;
    }

    /* Read Resource header. The allocation has already read and checked
       both headers, so reading them again cannot fail. */
    info.gx_binres_read_offset = 0;
    (VOID)_gx_binres_resource_header_load(&info, &header);

    /* Skip theme info.  */
    info.gx_binres_read_offset += header.gx_resource_header_theme_data_size;

    language_table = GX_NULL;

    /* Read language table.  */
    if (status == GX_SUCCESS)
    {
        /* Read string header. */
        (VOID)_gx_binres_string_header_load(&info, &string_header);

        language_table = (GX_UBYTE ***)(info.gx_binres_buffer + info.gx_binres_buffer_index);
        info.gx_binres_buffer_index += sizeof(GX_UBYTE * *) * string_header.gx_string_header_language_count;

        for (lang_index = 0; lang_index < string_header.gx_string_header_language_count; lang_index++)
        {
            /* Read language header.  */
            status = _gx_binres_language_header_load(&info, &language_header);

            if (status != GX_SUCCESS)
            {
                break;
            }

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
                    if (_gx_binres_range_check(&info, info.gx_binres_read_offset, sizeof(USHORT)) != GX_SUCCESS)
                    {
                        status = GX_INVALID_FORMAT;
                        break;
                    }

                    GX_BINRES_READ_USHORT(string_length, info.gx_binres_root_address + info.gx_binres_read_offset);
                    info.gx_binres_read_offset += sizeof(USHORT);

                    if (string_length == 0)
                    {
                        language_table[lang_index][string_index] = GX_NULL;
                    }
                    else
                    {
                        /* The string and its terminator are addressed by a pointer
                           that is kept and read later, so they have to lie inside
                           the resource. */
                        if (_gx_binres_range_check(&info, info.gx_binres_read_offset,
                                                   (ULONG)string_length + 1) != GX_SUCCESS)
                        {
                            status = GX_INVALID_FORMAT;
                            break;
                        }

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

                    /* This format carries no length, so the scan runs to a
                       terminator and the resource is what stops it. */
                    if (_gx_binres_range_check(&info, info.gx_binres_read_offset, 1) != GX_SUCCESS)
                    {
                        status = GX_INVALID_FORMAT;
                        break;
                    }

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

            if (status != GX_SUCCESS)
            {
                break;
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
/**************************************************************************/
#ifdef GX_BINARY_RESOURCE_SUPPORT
UINT _gx_binres_language_table_load_ext2(GX_UBYTE *root_address, ULONG root_size, GX_STRING ***returned_language_table)
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
    info.gx_binres_root_size = root_size;

    /* Allocate memory that needed for language table.  */
    status = _gx_binres_language_table_buffer_allocate(&info, sizeof(GX_STRING *), sizeof(GX_STRING));

    if (status != GX_SUCCESS)
    {
        return status;
    }

    /* Read Resource header. The allocation has already read and checked
       both headers, so reading them again cannot fail. */
    info.gx_binres_read_offset = 0;
    (VOID)_gx_binres_resource_header_load(&info, &header);

    /* Skip theme info.  */
    info.gx_binres_read_offset += header.gx_resource_header_theme_data_size;

    language_table = GX_NULL;

    /* Read language table.  */
    if (status == GX_SUCCESS)
    {
        /* Read string header. */
        (VOID)_gx_binres_string_header_load(&info, &string_header);

        language_table = (GX_STRING **)(info.gx_binres_buffer + info.gx_binres_buffer_index);
        info.gx_binres_buffer_index += sizeof(GX_STRING *) * string_header.gx_string_header_language_count;

        for (lang_index = 0; lang_index < string_header.gx_string_header_language_count; lang_index++)
        {
            /* Read language header.  */
            status = _gx_binres_language_header_load(&info, &language_header);

            if (status != GX_SUCCESS)
            {
                break;
            }

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
                if (_gx_binres_range_check(&info, info.gx_binres_read_offset, sizeof(USHORT)) != GX_SUCCESS)
                {
                    status = GX_INVALID_FORMAT;
                    break;
                }

                get_data = info.gx_binres_root_address + info.gx_binres_read_offset;
                string_length = *(get_data + 1);
                string_length = (USHORT)(string_length << 8);
                string_length = (USHORT)(string_length | (*get_data));
                info.gx_binres_read_offset += sizeof(USHORT);

                /* The string and its terminator are addressed by a pointer that
                   is kept and read later, so they have to lie inside the
                   resource. */
                if (_gx_binres_range_check(&info, info.gx_binres_read_offset,
                                           (ULONG)string_length + 1) != GX_SUCCESS)
                {
                    status = GX_INVALID_FORMAT;
                    break;
                }

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

            if (status != GX_SUCCESS)
            {
                break;
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

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_binres_language_table_load_ext                  PORTABLE C      */
/*                                                           6.5.1        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Eclipse ThreadX contributors                                        */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function is the form that takes no resource length. It derives  */
/*    one from the size the resource declares and defers to language_tabl */
/*                                                                        */
/*    A truncated resource is rejected, because its declared size still    */
/*    describes the whole of it. A resource built to mislead is not: the   */
/*    same attacker chose that size. Callers that can supply the real      */
/*    length should use the _ext form, which bounds every read by it.      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    root_address                          Resource address              */
/*    returned_language_table               Destination for the table     */
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
UINT _gx_binres_language_table_load_ext(GX_UBYTE *root_address, GX_STRING ***returned_language_table)
{
ULONG root_size;
UINT  status;

    status = _gx_binres_declared_size_get(root_address, &root_size);

    if (status != GX_SUCCESS)
    {
        return status;
    }

    return _gx_binres_language_table_load_ext2(root_address, root_size, returned_language_table);
}
#endif
#endif
