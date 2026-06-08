/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation
 * Copyright (c) 2026 Eclipse ThreadX contributors
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

