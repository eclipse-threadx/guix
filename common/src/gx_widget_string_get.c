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
#include "gx_display.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_widget_string_get                              PORTABLE C       */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION (deprecated)                                              */
/*                                                                        */
/*    This service gets the string associated with the supplied           */
/*      resource ID.                                                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    widget                                Called widget control block   */
/*    string_id                             Pixelmap resource ID          */
/*    return_string                         Pointer to string             */
/*                                            destination pointer         */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_display_string_get                Retrieve string from display  */
/*                                            function                    */
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
UINT  _gx_widget_string_get(GX_WIDGET *widget, GX_RESOURCE_ID resource_id, GX_CONST GX_CHAR **return_string)
{
UINT        status = GX_INVALID_CANVAS;

GX_CANVAS  *canvas;
GX_DISPLAY *display;

    if (widget -> gx_widget_status & GX_STATUS_VISIBLE)
    {
        _gx_widget_canvas_get(widget, &canvas);

        if (canvas)
        {
            display = canvas -> gx_canvas_display;
            status = _gx_display_string_get(display, resource_id, return_string);
        }
    }
    else
    {
        *return_string = GX_NULL;
    }
    return status;
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_widget_string_get_ext                          PORTABLE C       */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service gets the string associated with the supplied           */
/*      resource ID.                                                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    widget                                Called widget control block   */
/*    string_id                             Pixelmap resource ID          */
/*    return_string                         Pointer to string             */
/*                                            destination pointer         */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_display_string_get                Retrieve string from display  */
/*                                            function                    */
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
UINT  _gx_widget_string_get_ext(GX_WIDGET *widget, GX_RESOURCE_ID resource_id, GX_STRING *return_string)
{
UINT        status = GX_INVALID_CANVAS;

GX_CANVAS  *canvas;
GX_DISPLAY *display;

    if (widget -> gx_widget_status & GX_STATUS_VISIBLE)
    {
        _gx_widget_canvas_get(widget, &canvas);

        if (canvas)
        {
            display = canvas -> gx_canvas_display;
            status = _gx_display_string_get_ext(display, resource_id, return_string);
        }
    }
    else
    {
        return_string -> gx_string_ptr = GX_NULL;
        return_string -> gx_string_length = 0;
    }
    return status;
}

