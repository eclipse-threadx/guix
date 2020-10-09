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
/**   Progress Bar Management (Progress Bar)                              */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE

/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_utility.h"
#include "gx_progress_bar.h"
#include "gx_widget.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_progress_bar_value_set                          PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service sets the progress bar value.                           */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    progress_bar                          Progress Bar control block    */
/*    new_value                             New progress bar value        */
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
UINT _gx_progress_bar_value_set(GX_PROGRESS_BAR *progress_bar, INT new_value)
{
    if (new_value > progress_bar -> gx_progress_bar_info.gx_progress_bar_info_max_val)
    {
        new_value = progress_bar -> gx_progress_bar_info.gx_progress_bar_info_max_val;
    }
    else if (new_value < progress_bar -> gx_progress_bar_info.gx_progress_bar_info_min_val)
    {
        new_value = progress_bar -> gx_progress_bar_info.gx_progress_bar_info_min_val;
    }

    if (new_value != progress_bar -> gx_progress_bar_info.gx_progress_bar_info_current_val)
    {
        /* Assign progress bar value */
        progress_bar -> gx_progress_bar_info.gx_progress_bar_info_current_val = new_value;

        /* Mark as dirty so that I get redrawn */
        if (progress_bar -> gx_widget_status & GX_STATUS_VISIBLE)
        {
            _gx_system_dirty_mark((GX_WIDGET *)progress_bar);
        }

        /* notify my parent of my new value */
        _gx_widget_event_generate((GX_WIDGET *)progress_bar, GX_EVENT_PROGRESS_VALUE, new_value);
    }
    return(GX_SUCCESS);
}

