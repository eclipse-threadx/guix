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
/**   Slider Management (Slider)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE

/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_canvas.h"
#include "gx_context.h"
#include "gx_widget.h"
#include "gx_utility.h"
#include "gx_slider.h"



/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_slider_tickmarks_draw                           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service draws the slider tickmarks.                            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    slider                                Slider widget control block   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_slider_travel_get                 Get the slider travel         */
/*    _gx_context_line_color_set            Set the line color for the    */
/*                                            drawing context             */
/*    _gx_context_brush_width_set           Set the brush width for the   */
/*                                            drawing context             */
/*    _gx_canvas_line_draw                  Draw a line                   */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*    GUIX Internal Code                                                  */
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
VOID  _gx_slider_tickmarks_draw(GX_SLIDER *slider)
{
INT             mintravel;
INT             maxtravel;
INT             tickstart;
INT             tickend;
INT             space;
GX_FIXED_VAL    tickpos;
INT             tickcount;
GX_SLIDER_INFO *info;

    if (slider -> gx_slider_tick_count <= 1)
    {
        return;
    }

    /* make a border 1/8 of widget size around ticksmarks */
    if (slider -> gx_widget_style & GX_STYLE_SLIDER_VERTICAL)
    {
        mintravel = slider -> gx_widget_size.gx_rectangle_left;
        maxtravel = slider -> gx_widget_size.gx_rectangle_right;
        space = (maxtravel - mintravel) / 8;

        /* draw left in */
        tickend = (maxtravel + mintravel) / 2;
        tickend -= space;
        tickstart = mintravel + space;
    }
    else
    {
        mintravel = slider -> gx_widget_size.gx_rectangle_top;
        maxtravel = slider -> gx_widget_size.gx_rectangle_bottom;
        space = (maxtravel - mintravel) / 8;

        /* draw ticks from center down */
        tickend = maxtravel - space;
        tickstart = (maxtravel + mintravel) / 2;
        tickstart += space;
    }

    /* pick up pointer to slider info struct */
    info = &slider -> gx_slider_info;

    /* get the limits of the slider travel */
    _gx_slider_travel_get(slider, info, &mintravel, &maxtravel);

    tickcount = slider -> gx_slider_tick_count;
    space = GX_FIXED_VAL_MAKE(maxtravel - mintravel) / (tickcount - 1);

    /* set line color */
    _gx_context_line_color_set(slider -> gx_slider_tick_color);
    _gx_context_brush_width_set(1);
    tickpos = GX_FIXED_VAL_MAKE(mintravel);

    while (tickcount > 0)
    {
        mintravel = GX_FIXED_VAL_RND(tickpos);
        if (slider -> gx_widget_style & GX_STYLE_SLIDER_VERTICAL)
        {
            _gx_canvas_line_draw((GX_VALUE)tickstart, (GX_VALUE)mintravel, (GX_VALUE)tickend, (GX_VALUE)mintravel);
        }
        else
        {
            _gx_canvas_line_draw((GX_VALUE)mintravel, (GX_VALUE)tickstart, (GX_VALUE)mintravel, (GX_VALUE)tickend);
        }
        tickpos += space;
        tickcount--;
    }
}

