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
/**   PIXELMAP Management (PIXELMAP)                                      */
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
/*    _gx_widget_transparent_pixelmap_detect              PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function detects whether or not a pixelmap contains            */
/*    transparency.                                                       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    pixelmap_id                           Pixelmap ID                   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    GX_TRUE or GX_FALSE                                                 */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_canvas_get                 Retrieve canvas               */
/*                                                                        */
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
GX_BOOL _gx_widget_transparent_pixelmap_detect(GX_WIDGET *widget, GX_RESOURCE_ID pixelmap_id)
{
GX_PIXELMAP *map;
GX_CANVAS   *canvas;
GX_DISPLAY  *display;

    if (widget -> gx_widget_status & GX_STATUS_VISIBLE)
    {
        _gx_widget_canvas_get(widget, &canvas);

        if (canvas)
        {
            display = canvas -> gx_canvas_display;

            if (pixelmap_id < display -> gx_display_pixelmap_table_size)
            {
                map = display -> gx_display_pixelmap_table[pixelmap_id];

                if (map)
                {
                    if (map -> gx_pixelmap_flags & (GX_PIXELMAP_TRANSPARENT | GX_PIXELMAP_ALPHA))
                    {
                        return GX_TRUE;
                    }
                }
            }
        }
    }
    return GX_FALSE;
}

