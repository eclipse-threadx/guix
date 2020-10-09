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
/*    _gx_icon_pixelmap_update                            PORTABLE C      */
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
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*    _gx_widget_pixelmap_get               Retrieve the pixelmap         */
/*                                                                        */
/*    _gx_utility_rectangle_define          Defines the icon area         */
/*    _gx_widget_resize                     Set up the icon size          */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_icon_event_process                                              */
/*    _gx_icon_pixelmap_set                                               */
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
VOID _gx_icon_pixelmap_update(GX_ICON *icon)
{
GX_RECTANGLE size;
GX_PIXELMAP *map;

    _gx_widget_pixelmap_get((GX_WIDGET *)icon, icon -> gx_icon_normal_pixelmap, &map);

    if (map)
    {
        _gx_utility_rectangle_define(&size,
                                     icon -> gx_widget_size.gx_rectangle_left,
                                     icon -> gx_widget_size.gx_rectangle_top,
                                     (GX_VALUE)(icon -> gx_widget_size.gx_rectangle_left + map -> gx_pixelmap_width - 1),
                                     (GX_VALUE)(icon -> gx_widget_size.gx_rectangle_top + map -> gx_pixelmap_height - 1));
        _gx_widget_resize((GX_WIDGET *)icon, &size);
    }
}

