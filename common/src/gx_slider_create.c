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
/*    _gx_slider_create                                   PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service creates a slider.                                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    slider                                Slider control block          */
/*    name                                  Name of prompt                */
/*    parent                                Parent widget control block   */
/*    tick_count                            Number of slider ticks        */
/*    slider_info                           Pointer to slider info        */
/*    style                                 Style of slider               */
/*    slider_id                             Application-defined ID of     */
/*                                            slider                      */
/*    size                                  Dimensions of slider          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    memset                                Set control block to zero     */
/*    _gx_widget_create                     Create the underlying widget  */
/*    _gx_widget_width_get                  Retrieve the width of the     */
/*                                            widget                      */
/*    _gx_widget_height_get                 Retrieve the height of the    */
/*                                            widget                      */
/*    _gx_widget_link                       Link the widget to its parent */
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
UINT  _gx_slider_create(GX_SLIDER *slider, GX_CONST GX_CHAR *name,
                        GX_WIDGET *parent, INT tick_count,
                        GX_SLIDER_INFO *slider_info, ULONG style,
                        USHORT slider_id, GX_CONST GX_RECTANGLE *size)
{

    /* Call the widget create function.  */
    _gx_widget_create((GX_WIDGET *)slider, name, GX_NULL, style, slider_id, size);

    /* Populate the rest of slider control block - overriding as necessary.  */
    slider -> gx_widget_type = GX_TYPE_SLIDER;

    slider -> gx_slider_info = *slider_info;
    if (slider -> gx_slider_info.gx_slider_info_needle_width <= 0)
    {
        if (style & GX_STYLE_SLIDER_VERTICAL)
        {
            _gx_widget_width_get((GX_WIDGET *)slider, &slider -> gx_slider_info.gx_slider_info_needle_width);
            slider -> gx_slider_info.gx_slider_info_needle_width /= 2;
        }
        else
        {
            slider -> gx_slider_info.gx_slider_info_needle_width =  GX_SLIDER_NEEDLE_WIDTH;
        }
    }
    if (slider -> gx_slider_info.gx_slider_info_needle_height <= 0)
    {
        if (style & GX_STYLE_SLIDER_VERTICAL)
        {
            slider -> gx_slider_info.gx_slider_info_needle_height =  GX_SLIDER_NEEDLE_WIDTH;
        }
        else
        {
            _gx_widget_height_get((GX_WIDGET *)slider, &slider -> gx_slider_info.gx_slider_info_needle_height);
            slider -> gx_slider_info.gx_slider_info_needle_height /= 2;
        }
    }
    slider -> gx_widget_normal_fill_color =      GX_COLOR_ID_BUTTON_UPPER;
    slider -> gx_widget_selected_fill_color =    GX_COLOR_ID_BUTTON_UPPER;
    slider -> gx_slider_tick_color =             GX_COLOR_ID_SLIDER_TICK;
    slider -> gx_slider_tick_count =             tick_count;
    slider -> gx_widget_draw_function = (VOID (*)(GX_WIDGET *))_gx_slider_draw;
    slider -> gx_widget_event_process_function = (UINT (*)(GX_WIDGET *, GX_EVENT *))_gx_slider_event_process;

    /* Determine if a parent widget was provided.  */
    if (parent)
    {
        _gx_widget_link(parent, (GX_WIDGET *)slider);
    }

    /* Return the status from widget create.  */
    return(GX_SUCCESS);
}

