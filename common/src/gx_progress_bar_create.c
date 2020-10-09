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
#include "gx_widget.h"
#include "gx_progress_bar.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_progress_bar_create                             PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service creates a progress bar.                                */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    progress_bar                          Progress Bar control block    */
/*    name                                  Name of prompt                */
/*    parent                                Parent widget control block   */
/*    progress_bar_info                     Pointer to progress bar info  */
/*    style                                 Style of progress bar         */
/*    progress_bar_id                       Application-defined ID of     */
/*                                            progress bar                */
/*    size                                  Dimensions of progress bar    */
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
UINT  _gx_progress_bar_create(GX_PROGRESS_BAR *progress_bar,
                              GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                              GX_PROGRESS_BAR_INFO *progress_bar_info, ULONG style,
                              USHORT progress_bar_id,
                              GX_CONST GX_RECTANGLE *size)
{

    /* Call the widget create function.  */
    _gx_widget_create((GX_WIDGET *)progress_bar, name, GX_NULL, style, progress_bar_id, size);

    /* Populate the rest of progress bar control block - overriding as necessary.  */
    progress_bar -> gx_widget_type = GX_TYPE_PROGRESS_BAR;
    progress_bar -> gx_widget_draw_function = (VOID (*)(GX_WIDGET *))_gx_progress_bar_draw;
    progress_bar -> gx_widget_event_process_function = (UINT (*)(GX_WIDGET *, GX_EVENT *))_gx_progress_bar_event_process;

    if (progress_bar_info)
    {
        progress_bar -> gx_progress_bar_info = *progress_bar_info;
    }
    else
    {
        progress_bar -> gx_progress_bar_info.gx_progress_bar_info_max_val = 100;
        progress_bar -> gx_progress_bar_info.gx_progress_bar_info_min_val = 0;
        progress_bar -> gx_progress_bar_info.gx_progress_bar_info_current_val = 0;
        progress_bar -> gx_progress_bar_info.gx_progress_bar_font_id = GX_FONT_ID_DEFAULT;
        progress_bar -> gx_progress_bar_info.gx_progress_bar_normal_text_color = GX_COLOR_ID_TEXT;
        progress_bar -> gx_progress_bar_info.gx_progress_bar_selected_text_color = GX_COLOR_ID_SELECTED_TEXT;
        progress_bar -> gx_progress_bar_info.gx_progress_bar_disabled_text_color = GX_COLOR_ID_DISABLED_TEXT;
        progress_bar -> gx_progress_bar_info.gx_progress_bar_fill_pixelmap = GX_PIXELMAP_NULL;
    }

    /* Determine if a parent widget was provided.  */
    if (parent)
    {
        _gx_widget_link(parent, (GX_WIDGET *)progress_bar);
    }

    /* Return the completion status from widget create.  */
    return(GX_SUCCESS);
}

