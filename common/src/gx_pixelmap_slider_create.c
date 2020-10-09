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
/*    _gx_pixelmap_slider_create                          PORTABLE C      */
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
/*    name                                  Name of prompt                */
/*    parent                                Parent widget control block   */
/*    info                                  Pointer to a GX_SLIDER_INFO   */
/*    pixelmap_info                         Pointer to the pixelmap       */
/*                                            info block                  */
/*    style                                 Style of slider               */
/*    pixelmap_slider_id                    Application-defined ID of     */
/*                                            pixelmap slider             */
/*    size                                  Dimensions of pixelmap prompt */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_create                     Create the underlying widget  */
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

UINT  _gx_pixelmap_slider_create(GX_PIXELMAP_SLIDER *slider,
                                 GX_CONST GX_CHAR *name,
                                 GX_WIDGET *parent,
                                 GX_SLIDER_INFO *info,
                                 GX_PIXELMAP_SLIDER_INFO *pixelmap_info,
                                 ULONG style,
                                 USHORT pixelmap_slider_id,
                                 GX_CONST GX_RECTANGLE *size)
{

    /* Call the widget create function.  */
    _gx_widget_create((GX_WIDGET *)slider, name, GX_NULL, style, pixelmap_slider_id, size);

    /* Populate the rest of slider control block - overriding as necessary.  */
    slider -> gx_widget_type = GX_TYPE_PIXELMAP_SLIDER;

    slider -> gx_widget_draw_function = (VOID (*)(GX_WIDGET *))_gx_pixelmap_slider_draw;
    slider -> gx_widget_event_process_function = (UINT (*)(GX_WIDGET *, GX_EVENT *))_gx_pixelmap_slider_event_process;
    slider -> gx_slider_info = *info;
    slider -> gx_pixelmap_slider_pixelmap_info = *pixelmap_info;

    slider -> gx_slider_info.gx_slider_info_needle_height = 0;
    slider -> gx_slider_info.gx_slider_info_needle_width = 0;

    /* Determine if a parent widget was provided.  */
    if (parent)
    {
        _gx_widget_link(parent, (GX_WIDGET *)slider);
    }

    /* Return the status from prompt create.  */
    return(GX_SUCCESS);
}

