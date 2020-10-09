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
/**   Utility Management (Utility)                                        */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_utility.h"
#include "gx_system.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_utility_string_to_alphamap                     PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks error in utility string to alphamap function.  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text                                  pointer to string             */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_string_to_alphamap        The actual utility string to  */
/*                                            alphamap function           */
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
UINT _gxe_utility_string_to_alphamap(GX_CONST GX_CHAR *text, GX_CONST GX_FONT *font, GX_PIXELMAP *textmap)
{
UINT status;

    if (text == GX_NULL || font == GX_NULL || textmap == GX_NULL)
    {
        return GX_PTR_ERROR;
    }

    if (!_gx_system_memory_allocator || !_gx_system_memory_free)
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    status = _gx_utility_string_to_alphamap(text, font, textmap);

    /* Return completion status code. */
    return(status);
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_utility_string_to_alphamap_ext                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks error in utility string to alphamap function.  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text                                  pointer to string             */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_string_to_alphamap_ext    The actual utility string to  */
/*                                            alphamap ext function       */
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
UINT    _gxe_utility_string_to_alphamap_ext(GX_CONST GX_STRING *text, GX_CONST GX_FONT *font, GX_PIXELMAP *textmap)
{
UINT status;
UINT text_length = 0;

    if ((text == GX_NULL) ||
        (text -> gx_string_ptr == GX_NULL) ||
        (font == GX_NULL) ||
        (textmap == GX_NULL))
    {
        return GX_PTR_ERROR;
    }

    if (!_gx_system_memory_allocator || !_gx_system_memory_free)
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    status = _gx_utility_string_length_check(text -> gx_string_ptr, &text_length, text -> gx_string_length);

    if (status != GX_SUCCESS)
    {
        return status;
    }

    if (text_length != text -> gx_string_length)
    {
        return GX_INVALID_STRING_LENGTH;
    }

    status = _gx_utility_string_to_alphamap_ext(text, font, textmap);

    /* Return completion status code. */
    return(status);
}

