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
/**   Button Management (Button)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_button.h"

GX_CALLER_CHECKING_EXTERNS

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_pixelmap_button_create                         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in the pixelmap button create       */
/*                                         function.                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    button                                Button control block          */
/*    name                                  Name of button                */
/*    parent                                Parent widget control block   */
/*    normal_id                             Normal state pixelmap id      */
/*    selected_id                           Selected state pixelmap id    */
/*    disabled_id                           Disabled state pixelmap id    */
/*    style                                 Style of checkbox             */
/*    pixelmap_button_id                    Application-defined ID of     */
/*                                            the pixelmap button         */
/*    size                                  Button size                   */
/*    button_control_block_size             Size of the pixelmap button   */
/*                                            control block               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_pixelmap_button_create            the actual function           */
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
UINT  _gxe_pixelmap_button_create(GX_PIXELMAP_BUTTON *button,
                                  GX_CONST GX_CHAR *name,
                                  GX_WIDGET *parent,
                                  GX_RESOURCE_ID normal_id,
                                  GX_RESOURCE_ID selected_id,
                                  GX_RESOURCE_ID disabled_id,
                                  ULONG style,
                                  USHORT pixelmap_button_id,
                                  GX_CONST GX_RECTANGLE *size,
                                  UINT button_control_block_size)
{
UINT status;

    /* Check for invalid caller.  */
    GX_INIT_AND_THREADS_CALLER_CHECKING

    /* Check error for valid pointer.  */
    if ((button == GX_NULL) || (size == GX_NULL))
    {
        return(GX_PTR_ERROR);
    }

    /* Check for invalid control block size. */
    if (button_control_block_size != sizeof(GX_PIXELMAP_BUTTON))
    {
        return(GX_INVALID_SIZE);
    }

    /* Check error for valid widget.  */
    if (button -> gx_widget_type != 0)
    {
        return(GX_ALREADY_CREATED);
    }

    status = _gx_pixelmap_button_create(button, name, parent, normal_id, selected_id, disabled_id, style, pixelmap_button_id, size);
    return(status);
}

