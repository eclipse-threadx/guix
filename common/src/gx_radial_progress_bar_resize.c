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
/**   Progress Bar Management (Radial Progress Bar)                       */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_radial_progress_bar.h"
#include "gx_widget.h"
#include "gx_utility.h"
#include "gx_system.h"
#include "gx_canvas.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_radial_prpgress_bar_resize                      PORTABLE C      */
/*                                                           6.1.7        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function updates the center and radius of radial progress bar  */
/*    according to size.                                                  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    radial_progress                       Radial progess bar control    */
/*                                            block                       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_dirty_mark                 Mark the widget dirty         */
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
/*  06-02-2021     Ting Zhu                 Modified comment(s),          */
/*                                            fixed compile error when    */
/*                                            GX_BRUSH_ALPHA_SUPPORT is   */
/*                                            not defined,                */
/*                                            resulting in version 6.1.7  */
/*                                                                        */
/**************************************************************************/

UINT _gx_radial_progress_bar_resize(GX_RADIAL_PROGRESS_BAR *radial_progress)
{
GX_RADIAL_PROGRESS_BAR_INFO *info;
GX_VALUE                     new_xcenter;
GX_VALUE                     new_ycenter;
GX_VALUE                     new_radius;
INT                          width;
#if defined(GX_BRUSH_ALPHA_SUPPORT)
INT                          height;
#endif
GX_RECTANGLE                *size;

    info = &radial_progress -> gx_radial_progress_bar_info;
    width = info -> gx_radial_progress_bar_info_selected_brush_width;

    if (width < info -> gx_radial_progress_bar_info_normal_brush_width)
    {
        width = info -> gx_radial_progress_bar_info_normal_brush_width;
    }

    width = (GX_VALUE)((width + 1) >> 1);

    size = &radial_progress -> gx_widget_size;

    new_xcenter = (GX_VALUE)((size -> gx_rectangle_left + size -> gx_rectangle_right) >> 1);
    new_ycenter = (GX_VALUE)((size -> gx_rectangle_top + size -> gx_rectangle_bottom) >> 1);
    new_radius = (GX_VALUE)(new_xcenter - size -> gx_rectangle_left);
    new_radius = (GX_VALUE)(new_radius - width);

    info -> gx_radial_progress_bar_info_xcenter = new_xcenter;
    info -> gx_radial_progress_bar_info_ycenter = new_ycenter;

    if (new_radius > 0)
    {
        info -> gx_radial_progress_bar_info_radius = new_radius;
    }

#if defined(GX_BRUSH_ALPHA_SUPPORT)
    width = size -> gx_rectangle_right - size -> gx_rectangle_left + 1;
    height = size -> gx_rectangle_bottom - size -> gx_rectangle_top + 1;

    if (radial_progress -> gx_radial_progress_bar_canvas.gx_canvas_memory &&
        (width != radial_progress -> gx_radial_progress_bar_canvas.gx_canvas_x_resolution ||
         height != radial_progress -> gx_radial_progress_bar_canvas.gx_canvas_y_resolution) &&
        _gx_system_memory_free)
    {
        _gx_system_memory_free((VOID *)radial_progress -> gx_radial_progress_bar_canvas.gx_canvas_memory);
        _gx_canvas_delete(&radial_progress -> gx_radial_progress_bar_canvas);
    }
#endif

    if (radial_progress -> gx_widget_status & GX_STATUS_VISIBLE)
    {
        _gx_system_dirty_mark((GX_WIDGET *)radial_progress);
    }

    return GX_SUCCESS;
}

