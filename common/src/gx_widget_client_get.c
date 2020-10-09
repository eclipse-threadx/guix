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
/*    _gx_widget_client_get                               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function retrieves client area of a widget                     */
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
/*    _gx_widget_border_width_get           Get widget border width       */
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
UINT  _gx_widget_client_get(GX_WIDGET *widget, GX_VALUE width, GX_RECTANGLE *return_size)
{
GX_VALUE border_width;

    if (width < 0)
    {
        _gx_widget_border_width_get(widget, &border_width);
    }
    else
    {
        border_width = width;
    }
    /* Yes, return the widget's client rectangle.  */
    return_size -> gx_rectangle_top = (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_top + border_width);
    return_size -> gx_rectangle_bottom = (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_bottom - border_width);
    return_size -> gx_rectangle_left = (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_left + border_width);
    return_size -> gx_rectangle_right = (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_right - border_width);

    /* Return successful completion.  */
    return(GX_SUCCESS);
}

