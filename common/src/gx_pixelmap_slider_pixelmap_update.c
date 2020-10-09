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
#include "gx_widget.h"
#include "gx_slider.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_pixelmap_slider_pixelmap_update                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates a pixelmap slider, which is a type of widget  */
/*    for displaying a user-adjustable value in graphical fashion.        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    slider                                Slider control block          */
/*    pixinfo                               Slider infomration block      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_pixelmap_get               Retrieve the pixelmap         */
/*    _gx_widget_status_add                 Add status flag               */
/*    _gx_system_dirty_mark                 Mark the widget as dirty      */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_pixelmap_slider_event_process     Event process routine for     */
/*                                            pixelmap slider widget      */
/*    _gx_pixelmap_slider_pixelmap_set      Assign pixelmap to the        */
/*                                            pixelmap slider widget      */
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
VOID _gx_pixelmap_slider_pixelmap_update(GX_PIXELMAP_SLIDER *slider)
{
GX_PIXELMAP_SLIDER_INFO *pixelmap_info = &slider -> gx_pixelmap_slider_pixelmap_info;
GX_WIDGET               *widget = (GX_WIDGET *)slider;
GX_PIXELMAP             *map;

    /* if any of my pixelmaps are transparent, give myself transparent status */

    if (pixelmap_info -> gx_pixelmap_slider_info_needle_pixelmap)
    {
        _gx_widget_pixelmap_get(widget, pixelmap_info -> gx_pixelmap_slider_info_needle_pixelmap, &map);

        if (map)
        {
            slider -> gx_slider_info.gx_slider_info_needle_height = map -> gx_pixelmap_height;
            slider -> gx_slider_info.gx_slider_info_needle_width = map -> gx_pixelmap_width;

            if (PIXELMAP_IS_TRANSPARENT(map))
            {
                _gx_widget_status_add((GX_WIDGET *)slider, GX_STATUS_TRANSPARENT);
            }
        }
    }

    if (pixelmap_info -> gx_pixelmap_slider_info_lower_background_pixelmap)
    {
        _gx_widget_pixelmap_get(widget, pixelmap_info -> gx_pixelmap_slider_info_lower_background_pixelmap, &map);

        if (map && PIXELMAP_IS_TRANSPARENT(map))
        {
            _gx_widget_status_add((GX_WIDGET *)slider, GX_STATUS_TRANSPARENT);
        }
    }
}

