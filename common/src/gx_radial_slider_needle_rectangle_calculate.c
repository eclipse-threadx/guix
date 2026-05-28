/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation
 * Copyright (c) 2026 Eclipse ThreadX contributors
 *
 * This program and the accompanying materials are made available under the
 * terms of the MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 **************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** GUIX Component                                                        */
/**                                                                       */
/**   Radial Slider Management (Slider)                                   */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_utility.h"
#include "gx_radial_slider.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_radial_slider_needle_rectangle_calculate        PORTABLE C      */
/*                                                           6.1.10       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service calculates the bounding rectangle of the radial slider */
/*    needle.                                                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    slider                                Radial slider control block   */
/*    rectangle                             Rectangle to be retrieved     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_pixelmap_get               Retrieve pixelmap with        */
/*                                           specified resource id        */
/*    _gx_utility_rectangle_define          Define a rectangle            */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/**************************************************************************/
UINT _gx_radial_slider_needle_rectangle_calculate(GX_RADIAL_SLIDER *slider, GX_RECTANGLE *rectangle)
{
GX_PIXELMAP           *map;
GX_RADIAL_SLIDER_INFO *info = &slider -> gx_radial_slider_info;
INT                    xpos;
INT                    ypos;

    _gx_widget_pixelmap_get((GX_WIDGET *)slider, info -> gx_radial_slider_info_needle_pixelmap, &map);

    if (map)
    {
        xpos = _gx_utility_math_cos(GX_FIXED_VAL_MAKE(info -> gx_radial_slider_info_current_angle));
        ypos = _gx_utility_math_sin(GX_FIXED_VAL_MAKE(info -> gx_radial_slider_info_current_angle));

        /* calculate sin and cos value of current angle value. */
        xpos = GX_FIXED_VAL_RND((info -> gx_radial_slider_info_radius + info -> gx_radial_slider_info_needle_offset) * xpos);
        ypos = GX_FIXED_VAL_RND((info -> gx_radial_slider_info_radius + info -> gx_radial_slider_info_needle_offset) * ypos);

        /* calculate needle position. */
        xpos += info -> gx_radial_slider_info_xcenter;
        ypos = info -> gx_radial_slider_info_ycenter - ypos;

        xpos -= map -> gx_pixelmap_width >> 1;
        ypos -= map -> gx_pixelmap_height >> 1;

        xpos += slider -> gx_widget_size.gx_rectangle_left;
        ypos += slider -> gx_widget_size.gx_rectangle_top;

        /* define needle rectangle. */
        _gx_utility_rectangle_define(rectangle, (GX_VALUE)xpos, (GX_VALUE)ypos,
                                    (GX_VALUE)(xpos + map -> gx_pixelmap_width),
                                    (GX_VALUE)(ypos + map -> gx_pixelmap_height));
    }
    else
    {
        _gx_utility_rectangle_define(rectangle, 0, 0, 0, 0);
    }

    return GX_SUCCESS;
}

