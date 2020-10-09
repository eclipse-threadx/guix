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
/**   Button Management (Button)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_button.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_pixelmap_button_transparent_detect              PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function detects if any of the pixelmaps are transparent.      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    button                                Button control block          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_transparent_pixelmap_detect                              */
/*                                          Detect the transparency       */
/*    _gx_widget_status_add                 Add the status flag           */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_pixelmap_event_process                                          */
/*    _gx_pixelmap_button_pixelmap_set      Assigns pixelmap to button    */
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
VOID _gx_pixelmap_button_transparent_detect(GX_PIXELMAP_BUTTON *button)
{
GX_WIDGET *widget = (GX_WIDGET *)button;

    if (!(widget -> gx_widget_status & GX_STATUS_TRANSPARENT))
    {
        if (_gx_widget_transparent_pixelmap_detect(widget, button -> gx_pixelmap_button_normal_id) ||
            _gx_widget_transparent_pixelmap_detect(widget, button -> gx_pixelmap_button_selected_id) ||
            _gx_widget_transparent_pixelmap_detect(widget, button -> gx_pixelmap_button_disabled_id))
        {
            _gx_widget_status_add((GX_WIDGET *)button, GX_STATUS_TRANSPARENT);
        }
    }
}

