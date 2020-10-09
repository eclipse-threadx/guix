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
/*    _gx_widget_color_get                                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service gets the color associated with the supplied            */
/*      resource ID from the system color table.                          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    widget                                caller identify               */
/*    color_id                              Resource ID of color          */
/*    return_color                          Pointer to destination for    */
/*                                            color                       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_canvas_get                 Get canvas pointer            */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*    GUIX default draw funtions                                          */
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
UINT _gx_widget_color_get(GX_WIDGET *widget, GX_RESOURCE_ID color_id, GX_COLOR *return_color)
{
UINT        status = GX_INVALID_CANVAS;
GX_COLOR    colorval = 0;
GX_CANVAS  *canvas;
GX_DISPLAY *display;

    if (widget -> gx_widget_status & GX_STATUS_VISIBLE)
    {
        _gx_widget_canvas_get(widget, &canvas);

        if (canvas)
        {
            display = canvas -> gx_canvas_display;

            /* Determine if the ID is within range.  */
            if (color_id < display -> gx_display_color_table_size)
            {
                /* Yes, the ID is within range. Perform a table lookup and return the
                   color.  */
                colorval = display -> gx_display_color_table[color_id];
                status = GX_SUCCESS;
            }
            else
            {
                status = GX_INVALID_RESOURCE_ID;
            }
        }
    }
    *return_color = colorval;
    return(status);
}

