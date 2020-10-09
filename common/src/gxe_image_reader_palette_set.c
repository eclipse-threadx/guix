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
/**   Image Reader Management (Image Reader)                              */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_image_reader.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_image_reader_palette_set                       PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in the image reader palette set     */
/*    function.                                                           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    image_reader                          Image reader control block.   */
/*    pal                                   Pointer to palette.           */
/*    palsize                               The size of pallete.          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                          Completion status                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*   _gx_image_reader_info_set        Actual image reader palette set     */
/*                                    call.                               */
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
#if defined(GX_SOFTWARE_DECODER_SUPPORT)
UINT _gxe_image_reader_palette_set(GX_IMAGE_READER *image_reader, GX_COLOR *pal, UINT palsize)
{
UINT status;

    /* Check for invalid input pointers.  */
    if ((image_reader == GX_NULL) || (pal == GX_NULL))
    {
        return(GX_PTR_ERROR);
    }

    /* Check for invalid input values.  */
    if (palsize == 0)
    {
        return(GX_INVALID_VALUE);
    }

    /* Call the actual image reader palette set function.  */
    status = _gx_image_reader_palette_set(image_reader, pal, palsize);

    /* Return completion status.  */
    return status;
}
#endif
