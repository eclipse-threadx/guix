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
#include "gx_display.h"
#include "gx_context.h"
#include "gx_widget.h"
#include "gx_slider.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_slider_draw                                     PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service draws the specified slider.                            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    slider                                Widget control block          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_color_get                  Get the color by resource ID  */
/*    _gx_widget_border_draw                Draw border                   */
/*    _gx_slider_tickmarks_draw             Draw tickmarks                */
/*    _gx_slider_needle_draw                Draw the slider needle        */
/*    _gx_widget_children_draw              Draw children widgets         */
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
VOID  _gx_slider_draw(GX_SLIDER *slider)
{
GX_RESOURCE_ID fill;

    if (slider -> gx_widget_style & GX_STYLE_ENABLED)
    {
        if (slider -> gx_widget_style & GX_STYLE_DRAW_SELECTED)
        {
            fill = slider -> gx_widget_selected_fill_color;
        }
        else
        {
            fill = slider -> gx_widget_normal_fill_color;
        }
    }
    else
    {
        fill = slider -> gx_widget_disabled_fill_color;
    }

    _gx_widget_border_draw((GX_WIDGET *)slider, GX_COLOR_ID_BUTTON_BORDER, fill, fill, GX_TRUE);

    /* draw the scale */

    if (slider -> gx_widget_style & GX_STYLE_SHOW_TICKMARKS)
    {
        _gx_slider_tickmarks_draw((GX_SLIDER *)slider);
    }

    if (slider -> gx_widget_style & GX_STYLE_SHOW_NEEDLE)
    {
        _gx_slider_needle_draw((GX_SLIDER *)slider);
    }

    /* Draw children widgets of prompt widget.  */
    _gx_widget_children_draw((GX_WIDGET *)slider);
}

