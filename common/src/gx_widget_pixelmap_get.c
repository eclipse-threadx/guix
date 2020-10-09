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
/**   System Management (System)                                          */
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
/*    _gx_widget_pixelmap_get                            PORTABLE C       */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service gets the pixelmap associated with the supplied         */
/*      resource ID.                                                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    widget                                Called widget control block   */
/*    pixelmap_id                           Pixelmap resource ID          */
/*    return_pixelmap                       Pointer to pixelmap           */
/*                                            destination pointer         */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_pixelmap_get               Actual widget pixelmap get    */
/*                                            function                    */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*    _gx_drop_list_pixelmap_set            Assign pixelmap to drop list  */
/*    _gx_icon_pixelmap_update              Update pixelmap in an icon    */
/*    _gx_pixelmap_slider_pixelmap_update   Update pixelmap in slider     */
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
UINT  _gx_widget_pixelmap_get(GX_WIDGET *widget, GX_RESOURCE_ID resource_id, GX_PIXELMAP **return_pixelmap)
{
GX_PIXELMAP *map = GX_NULL;
UINT         status = GX_INVALID_CANVAS;

GX_CANVAS   *canvas;
GX_DISPLAY  *display;

    if (widget -> gx_widget_status & GX_STATUS_VISIBLE)
    {
        _gx_widget_canvas_get(widget, &canvas);

        if (canvas)
        {
            display = canvas -> gx_canvas_display;

            /* Determine if the ID is within range.  */
            if (resource_id < display -> gx_display_pixelmap_table_size)
            {
                /* Yes, the ID is within range. Perform a table lookup and return the
                   pixelmap pointer.  */
                map = display -> gx_display_pixelmap_table[resource_id];
                status = GX_SUCCESS;
            }
            else
            {
                status = GX_INVALID_RESOURCE_ID;
            }
        }
    }

    *return_pixelmap = map;
    return status;
}

