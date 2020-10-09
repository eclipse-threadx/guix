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
/*    _gx_widget_font_get                                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service gets the font associated with the specified            */
/*    resource ID.                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    widget                                Widget control block pointer  */
/*    resource_id                           Resource ID of font           */
/*    return_font                           Pointer to destination for    */
/*                                            font pointer                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_canvas_get                 Get widget canvas pointer     */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*    GUIX default drawing functions                                      */
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
UINT  _gx_widget_font_get(GX_WIDGET *widget, GX_RESOURCE_ID resource_id, GX_FONT **return_font)
{
GX_FONT       *font = GX_NULL;
UINT           status = GX_INVALID_CANVAS;

GX_CANVAS     *canvas;
GX_DISPLAY    *display;
GX_RESOURCE_ID font_table_size;

    if (widget -> gx_widget_status & GX_STATUS_VISIBLE)
    {
        _gx_widget_canvas_get(widget, &canvas);

        if (canvas)
        {
            display = canvas -> gx_canvas_display;

            /* Pickup the font table size.  */
            font_table_size =  display -> gx_display_font_table_size;

            /* Determine if the ID is within range.  */
            if (resource_id < font_table_size)
            {
                /* Yes, the ID is within range. Perform a table lookup and return the
                   font poiner.  */
                font = display -> gx_display_font_table[resource_id];
                status = GX_SUCCESS;
            }
            else
            {
                status = GX_INVALID_RESOURCE_ID;
            }
        }
    }
    *return_font = font;
    return(status);
}

