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
/**   Canvas Management (Canvas)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE

/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_utility.h"
#include "gx_canvas.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_alphamap_create                         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates an 8bpp alpha-map pixelmap.                   */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    width                                 width of desired alphamap     */
/*    height                                height of desired alphamap    */
/*    map                                   pointer to pixlmap structure  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_memory_allocator           Dynamically allocate memory   */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_utility_string_to_alphamap                                      */
/*    application software                                                */
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
UINT _gx_utility_alphamap_create(INT width, INT height, GX_PIXELMAP *map)
{
/* try to allocate memory for alphamap */
GX_UBYTE *memptr;
ULONG     memsize = (ULONG)(width * height);

    if (width > GX_MAX_PIXELMAP_RESOLUTION)
    {
        return GX_INVALID_WIDTH;
    }

    if (height > GX_MAX_PIXELMAP_RESOLUTION)
    {
        return GX_INVALID_HEIGHT;
    }

    if (!memsize || _gx_system_memory_allocator == GX_NULL)
    {
        return GX_FAILURE;
    }

    memptr = (GX_UBYTE *)(_gx_system_memory_allocator(memsize));

    if (memptr == GX_NULL)
    {
        map -> gx_pixelmap_data = GX_NULL;
        return GX_FAILURE;
    }

    map -> gx_pixelmap_aux_data = GX_NULL;
    map -> gx_pixelmap_aux_data_size = 0;
    map -> gx_pixelmap_data = memptr;
    map -> gx_pixelmap_data_size = memsize;
    map -> gx_pixelmap_flags = GX_PIXELMAP_TRANSPARENT;
    map -> gx_pixelmap_transparent_color = 0;
    map -> gx_pixelmap_format = GX_COLOR_FORMAT_8BIT_ALPHAMAP;
    map -> gx_pixelmap_height = (GX_VALUE)height;
    map -> gx_pixelmap_width = (GX_VALUE)width;
    map -> gx_pixelmap_version_major = map -> gx_pixelmap_version_minor = 0;
    memset(memptr, 0, (size_t)memsize);
    return GX_SUCCESS;
}

