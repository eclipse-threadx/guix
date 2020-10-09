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

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    gx_binres_language_info_load                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function retrives language header information.                 */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    root_address                          Root address of binary        */
/*                                            resource data               */
/*    header                                Returned Language header      */
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
#ifdef GX_BINARY_RESOURCE_SUPPORT
UINT _gx_binres_language_info_load(GX_UBYTE *root_address, GX_LANGUAGE_HEADER *put_info)
{
UINT                status = GX_SUCCESS;
GX_BINRES_DATA_INFO info;
GX_RESOURCE_HEADER  header;
GX_STRING_HEADER    string_header;
UINT                lang_index;

    memset(&info, 0, sizeof(GX_BINRES_DATA_INFO));

    info.gx_binres_root_address = root_address;

    /* Read Resource header. */
    info.gx_binres_read_offset = 0;
    _gx_binres_resource_header_load(&info, &header);

    /* Skip theme info.  */
    info.gx_binres_read_offset += header.gx_resource_header_theme_data_size;

    if (header.gx_resource_header_magic_number != GX_MAGIC_NUMBER)
    {
        return GX_INVALID_FORMAT;
    }

    /* Read string header. */
    _gx_binres_string_header_load(&info, &string_header);

    if (string_header.gx_string_header_magic_number != GX_MAGIC_NUMBER)
    {
        status = GX_INVALID_FORMAT;
    }
    else
    {
        for (lang_index = 0; lang_index < string_header.gx_string_header_language_count; lang_index++)
        {
            /* Read language header.  */
            _gx_binres_language_header_load(&info, put_info);

            if (put_info -> gx_language_header_magic_number != GX_MAGIC_NUMBER)
            {
                status = GX_INVALID_FORMAT;
                break;
            }
            info.gx_binres_read_offset += put_info -> gx_language_header_data_size;
            put_info++;
        }
    }

    return status;
}
#endif

