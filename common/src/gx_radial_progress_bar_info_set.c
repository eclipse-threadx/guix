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
#include "gx_system.h"
#include "gx_widget.h"
#include "gx_radial_progress_bar.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_radial_progress_bar_info_set                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reset the information structure of a radial progress  */
/*      bar                                                               */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    bar                                   Radial proress bar control    */
/*                                            block                       */
/*    info                                  Radial progess bar info block */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_radial_progrss_bar_size_update    Update the size of the        */
/*                                            progress bar                */
/*    _gx_system_dirty_mark                 Mark the widget as dirty      */
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
UINT _gx_radial_progress_bar_info_set(GX_RADIAL_PROGRESS_BAR *bar, GX_RADIAL_PROGRESS_BAR_INFO *info)
{

    /* Copy the new info.  */
    bar -> gx_radial_progress_bar_info = *info;

    if (info -> gx_radial_progress_bar_info_anchor_val < 0)
    {
        bar -> gx_radial_progress_bar_info.gx_radial_progress_bar_info_anchor_val = 0;
    }
    else if (info -> gx_radial_progress_bar_info_anchor_val > 360)
    {
        bar -> gx_radial_progress_bar_info.gx_radial_progress_bar_info_anchor_val = 360;
    }

    /* Calculate radial progress bar.  */
    _gx_radial_progress_bar_size_update(bar);

    if (bar -> gx_widget_status & GX_STATUS_VISIBLE)
    {
        /* Mark the widget area as dirty.  */
        _gx_system_dirty_mark((GX_WIDGET *)bar);
    }

    return(GX_SUCCESS);
}

