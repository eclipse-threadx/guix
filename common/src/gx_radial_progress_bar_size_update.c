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

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_radial_prpgress_bar_size_update                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function calculate the size of the radial progress bar.        */
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
/*    _gx_widget_resize                     Resize the widget             */
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

UINT _gx_radial_progress_bar_size_update(GX_RADIAL_PROGRESS_BAR *radial_progress)
{
GX_RADIAL_PROGRESS_BAR_INFO *info;
GX_RECTANGLE                *size;
GX_RECTANGLE                 new_size;
GX_VALUE                     width;

    info = &radial_progress -> gx_radial_progress_bar_info;

    width = info -> gx_radial_progress_bar_info_selected_brush_width;

    if (width < info -> gx_radial_progress_bar_info_normal_brush_width)
    {
        width = info -> gx_radial_progress_bar_info_normal_brush_width;
    }

    width = (GX_VALUE)((width + 1) >> 1);
    /* Calculate new size.  */
    new_size.gx_rectangle_left = (GX_VALUE)(info -> gx_radial_progress_bar_info_xcenter - info -> gx_radial_progress_bar_info_radius - width);
    new_size.gx_rectangle_right = (GX_VALUE)(info -> gx_radial_progress_bar_info_xcenter + info -> gx_radial_progress_bar_info_radius + width);
    new_size.gx_rectangle_top = (GX_VALUE)(info -> gx_radial_progress_bar_info_ycenter - info -> gx_radial_progress_bar_info_radius - width);
    new_size.gx_rectangle_bottom = (GX_VALUE)(info -> gx_radial_progress_bar_info_ycenter + info -> gx_radial_progress_bar_info_radius + width);

    /* Retrieve old progress bar size.  */
    size = &radial_progress -> gx_widget_size;

    if ((new_size.gx_rectangle_left != size -> gx_rectangle_left) ||
        (new_size.gx_rectangle_right != size -> gx_rectangle_right) ||
        (new_size.gx_rectangle_top != size -> gx_rectangle_top))
    {
        /* Update radial progress bar size.  */
        _gx_widget_resize((GX_WIDGET *)radial_progress, &new_size);
    }

    return GX_SUCCESS;
}

