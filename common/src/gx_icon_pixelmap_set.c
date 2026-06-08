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
/**   Icon Management (Icon)                                              */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_utility.h"
#include "gx_prompt.h"
#include "gx_system.h"
#include "gx_icon.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_icon_pixelmap_set                               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Change the pixelmap associated with an icon after the icon has      */
/*    been created.                                                       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    icon                                  Pointer to icon control block */
/*    normal_pixelmap_id                    Normal state pixelamp         */
/*                                            resource ID                 */
/*    selected_pixelmap_id                  Selected state pixelmap       */
/*                                            resource ID                 */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_icon_pixelmap_update              Change the pixelmap           */
/*    _gx_system_dirty_mark                 Mark the widget dirty         */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/**************************************************************************/
UINT  _gx_icon_pixelmap_set(GX_ICON *icon,
                            GX_RESOURCE_ID normal_pixelmap_id,
                            GX_RESOURCE_ID selected_pixelmap_id)
{
    icon -> gx_icon_normal_pixelmap = normal_pixelmap_id;
    icon -> gx_icon_selected_pixelmap = selected_pixelmap_id;

    if (icon -> gx_widget_status & GX_STATUS_VISIBLE)
    {
        _gx_icon_pixelmap_update(icon);

        /* Mark widget area as dirty. */
        _gx_system_dirty_mark((GX_WIDGET *)icon);
    }

    return(GX_SUCCESS);
}

