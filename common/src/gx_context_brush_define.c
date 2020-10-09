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
/**   Context Management (Context)                                        */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_context.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_context_brush_define                            PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Set the brush of the current display context.                       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    line_color_id                         Resource ID of line color     */
/*    fill_color_id                         Resource ID of fill color     */
/*    style                                 Style of brush                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_context_color_get                 Retrieve context color        */
/*    _gx_brush_define                      Define the brush              */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_scrollbar_draw                                                  */
/*    _gx_slider_needle_draw                                              */
/*    _gx_widget_border_draw                                              */
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
UINT _gx_context_brush_define(GX_RESOURCE_ID line_color_id, GX_RESOURCE_ID fill_color_id, UINT style)
{
UINT             status;

GX_BRUSH        *brush;
GX_DRAW_CONTEXT *context = _gx_system_current_draw_context;

GX_COLOR         linecolor;
GX_COLOR         fillcolor;

    status = _gx_context_color_get(line_color_id, &linecolor);

    if (status == GX_SUCCESS)
    {
        status = _gx_context_color_get(fill_color_id, &fillcolor);

        if (status == GX_SUCCESS)
        {
            /* Set the brush.  */
            brush = &context -> gx_draw_context_brush;
            status = _gx_brush_define(brush, linecolor, fillcolor, style);
        }
    }

    /* Return status. */
    return status;
}

