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
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
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

