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
/**   Slider Management (Slider)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_widget.h"
#include "gx_slider.h"

/* Bring in externs for caller checking code.  */
GX_CALLER_CHECKING_EXTERNS

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_pixelmap_slider_create                         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for error in pixelmap slider create call.      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    slider                                Slider control block          */
/*    name                                  Name of prompt                */
/*    parent                                Parent widget control block   */
/*    info                                  Pointer to a GX_SLIDER_INFO   */
/*    pixelmap_info                         Pointer to the pixelmap       */
/*                                            info block                  */
/*    style                                 Style of the slider           */
/*    pixelmap_slider_id                    Application-defined ID of     */
/*                                            pixelmap slider             */
/*    size                                  Dimensions of pixelmap prompt */
/*    pixelmap_slider_control_block_size    Size of the pixelmap slider   */
/*                                            control block               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_pixelmap_slider_create            The actual function           */
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
UINT  _gxe_pixelmap_slider_create(GX_PIXELMAP_SLIDER *slider,
                                  GX_CONST GX_CHAR *name,
                                  GX_WIDGET *parent,
                                  GX_SLIDER_INFO *info,
                                  GX_PIXELMAP_SLIDER_INFO *pixelmap_info,
                                  ULONG style,
                                  USHORT pixelmap_slider_id,
                                  GX_CONST GX_RECTANGLE *size,
                                  UINT pixelmap_slider_control_block_size)
{

UINT status;

    /* Check for appropriate caller.  */
    GX_INIT_AND_THREADS_CALLER_CHECKING

    /* Check error for valid pointer. */
    if ((slider == GX_NULL) || (size == GX_NULL) ||
        (info == GX_NULL) || (pixelmap_info == GX_NULL))
    {
        return(GX_PTR_ERROR);
    }

    /* Check for invalid widget control block size. */
    if (pixelmap_slider_control_block_size != sizeof(GX_PIXELMAP_SLIDER))
    {
        return(GX_INVALID_SIZE);
    }

    /* Check for widget already created.  */
    if (slider -> gx_widget_type != 0)
    {
        return(GX_ALREADY_CREATED);
    }

    status = _gx_pixelmap_slider_create(slider, name, parent, info, pixelmap_info, style, pixelmap_slider_id, size);

    return(status);
}

