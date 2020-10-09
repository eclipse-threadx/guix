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
#include "gx_button.h"
#include "gx_system.h"
#include "gx_widget.h"
#include "gx_context.h"
#include "gx_canvas.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_monochrome_driver_disabled_button_line_draw     PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function draws a line over button text to indicate the         */
/*    button is disabled. Used only for monochrome format.                */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    button                                Button control block          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_context_raw_line_color_set      Set raw line color              */
/*    _gx_context_color_get               Get color associated with the   */
/*                                           supplied color id            */
/*    _gx_context_brush_width_set         Set brush width                 */
/*    _gx_canvas_line_draw                Draw a line                     */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_text_button_draw                                                */
/*    _gx_multi_line_text_button_draw                                     */
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
VOID  _gx_monochrome_driver_disabled_button_line_draw(GX_BUTTON *button)
{
GX_CANVAS *canvas;
INT        display_format;
GX_VALUE   start;
GX_VALUE   end;
GX_VALUE   yval;
GX_COLOR   line_color;


    canvas = _gx_system_current_draw_context -> gx_draw_context_canvas;
    display_format = canvas -> gx_canvas_display -> gx_display_color_format;

    if (display_format != GX_COLOR_FORMAT_MONOCHROME)
    {
        return;
    }

    /* Get fill color; */
    _gx_context_color_get(button -> gx_widget_normal_fill_color, &line_color);

    /* Set line color. */
    line_color = ~line_color;

    _gx_context_raw_line_color_set(line_color);

    _gx_context_brush_width_set(2);


    start = (GX_VALUE)(button -> gx_widget_size.gx_rectangle_left + 5);
    end = (GX_VALUE)(button -> gx_widget_size.gx_rectangle_right - 5);
    yval = (GX_VALUE)((button -> gx_widget_size.gx_rectangle_top + button -> gx_widget_size.gx_rectangle_bottom) >> 1);

    _gx_canvas_line_draw((GX_VALUE)start, (GX_VALUE)yval, (GX_VALUE)end, (GX_VALUE)yval);
}

