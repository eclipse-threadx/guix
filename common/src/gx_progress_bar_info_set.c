/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation 
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
/**   Slider Management (Slider)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE

/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_widget.h"
#include "gx_slider.h"
#include "gx_progress_bar.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_progress_bar_info_set                           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reset the information structure of a GX_PROGRESS_BAR  */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    slider                                Widget control block          */
/*    info                                  Slider info block             */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
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
UINT _gx_progress_bar_info_set(GX_PROGRESS_BAR *bar, GX_PROGRESS_BAR_INFO *info)
{
    /* copy the new info */
    bar -> gx_progress_bar_info = *info;

    if (bar -> gx_widget_status & GX_STATUS_VISIBLE)
    {
        _gx_system_dirty_mark((GX_WIDGET *)bar);
    }

    return(GX_SUCCESS);
}

