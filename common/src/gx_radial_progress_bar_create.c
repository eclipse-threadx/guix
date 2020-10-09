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
#include "gx_widget.h"
#include "gx_utility.h"
#include "gx_radial_progress_bar.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_radial_progress_bar_create                      PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service creates a radial progress bar.                         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    progress_bar                          Progress Bar control block    */
/*    name                                  Name of radial progress bar   */
/*    parent                                Pointer to parent widget      */
/*    info                                  Pointer to radial progress    */
/*                                            bar info.                   */
/*    style                                 Style of radial progress bar  */
/*    id                                    Application-defined ID of     */
/*                                            progress bar                */
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
UINT  _gx_radial_progress_bar_create(GX_RADIAL_PROGRESS_BAR *progress_bar,
                                     GX_CONST GX_CHAR *name,
                                     GX_WIDGET *parent,
                                     GX_RADIAL_PROGRESS_BAR_INFO *info,
                                     ULONG style,
                                     USHORT id)
{

GX_RECTANGLE size;

    _gx_utility_rectangle_define(&size, 0, 0, 0, 0);

    /* Call the widget create function.  */
    _gx_widget_create((GX_WIDGET *)progress_bar, name, GX_NULL, style, id, &size);

    /* Populate the rest of radial progress bar control block - overriding as necessary.  */
    progress_bar -> gx_radial_progress_bar_info = *info;
    progress_bar -> gx_widget_type = GX_TYPE_RADIAL_PROGRESS_BAR;
    progress_bar -> gx_widget_draw_function = (VOID (*)(GX_WIDGET *))_gx_radial_progress_bar_draw;
    progress_bar -> gx_widget_event_process_function = (UINT (*)(GX_WIDGET *, GX_EVENT *))_gx_radial_progress_bar_event_process;

#if defined(GX_BRUSH_ALPHA_SUPPORT)
    memset(&progress_bar -> gx_radial_progress_bar_canvas, 0, sizeof(GX_CANVAS));
#endif

    _gx_radial_progress_bar_size_update(progress_bar);

    /* we want this widget to be notified on resize/shift */
    progress_bar -> gx_widget_status |= GX_STATUS_RESIZE_NOTIFY;

    /* Determine if a parent widget was provided.  */
    if (parent)
    {
        _gx_widget_link(parent, (GX_WIDGET *)progress_bar);
    }

    /* Return the completion status from widget create.  */
    return(GX_SUCCESS);
}

