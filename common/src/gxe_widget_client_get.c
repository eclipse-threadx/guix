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
/**   Widget Management (Widget)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_widget_client_get                              PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in the widget client get function   */
/*    call.                                                               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    widget                                Widget control block          */
/*    border_width                          width of widget border        */
/*    return_size                           Return rectangle pointer      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_width_get                  Get widget width              */
/*    _gx_widget_height_get                 Get widget height             */
/*    _gx_widget_client_get                 Actual widget client get      */
/*                                            function                    */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*    GUIX draw functions                                                 */
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
UINT  _gxe_widget_client_get(GX_WIDGET *widget, GX_VALUE border_width, GX_RECTANGLE *return_size)
{
UINT     status;
GX_VALUE widget_width;
GX_VALUE widget_height;

    /* Check for invalid input pointers.  */
    if ((widget == GX_NULL) || (return_size == GX_NULL))
    {
        return(GX_PTR_ERROR);
    }

    /* Check for invalid widget.  */
    if (widget -> gx_widget_type == 0)
    {
        return(GX_INVALID_WIDGET);
    }

    /* Check for invalid widget border.  */
    _gx_widget_width_get(widget, &widget_width);
    _gx_widget_height_get(widget, &widget_height);
    if ((border_width > (widget_width / 2)) || (border_width > (widget_height / 2)))
    {
        return(GX_INVALID_VALUE);
    }

    /* Call actual widget client get function.  */
    status = _gx_widget_client_get(widget, border_width, return_size);

    /* Return completion status.  */
    return(status);
}

