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
/**   Radial Slider Management (Slider)                                   */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_context.h"
#include "gx_canvas.h"
#include "gx_radial_slider.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_radial_slider_draw                              PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function draws a radial slider widget.                         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    slider                                Radial slider control block   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_draw                       Default widget draw           */
/*    _gx_context_pixelmap_get              Retrieve pixelmap with        */
/*                                            specified pixelmap id       */
/*    _gx_canvas_pixelmap_draw              Draw a pixelmap to canvas     */
/*    _gx_widget_children_draw              Draw widget children          */
/*    _gx_radial_slider_needle_rectangle_get                              */
/*                                          Retrive needle bounding       */
/*                                            rectangle                   */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
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
VOID  _gx_radial_slider_draw(GX_RADIAL_SLIDER *slider)
{
GX_PIXELMAP *map = GX_NULL;
GX_RECTANGLE size;

    /* Call default widget draw. */
    _gx_widget_background_draw((GX_WIDGET *)slider);

    _gx_context_pixelmap_get(slider -> gx_radial_slider_info.gx_radial_slider_info_background_pixelmap, &map);

    _gx_widget_context_fill_set((GX_WIDGET *)slider);

    if (map)
    {
        /* Draw background pixelmap. */
        _gx_canvas_pixelmap_draw(slider -> gx_widget_size.gx_rectangle_left, slider -> gx_widget_size.gx_rectangle_top, map);
    }

    /* Pick needle pixelmap. */
    _gx_context_pixelmap_get(slider -> gx_radial_slider_info.gx_radial_slider_info_needle_pixelmap, &map);

    if (map)
    {
        /* Calcualte needle bounding rectangle. */
        _gx_radial_slider_needle_rectangle_calculate(slider, &size);

        /* Draw needle. */
        _gx_canvas_pixelmap_draw(size.gx_rectangle_left, size.gx_rectangle_top, map);
    }

    /* Draw children widgets. */
    _gx_widget_children_draw((GX_WIDGET *)slider);
}

