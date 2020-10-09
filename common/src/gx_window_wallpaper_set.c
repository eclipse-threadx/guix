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
/**   Window Management (Window)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_window.h"
#include "gx_system.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_window_wallpaper_set                            PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service sets the wallpaper for the specified window.           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    window                                Pointer to window             */
/*    wallpaper_id                          Resource ID of wallpaper      */
/*    tile                                  Wallpaper is tiled if GX_TRUE,*/
/*                                          otherwise wallpaper is not    */
/*                                          tiled                         */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_dirty_mark                 Mark this text as dirty       */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
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
UINT _gx_window_wallpaper_set(GX_WINDOW *window, GX_RESOURCE_ID wallpaper_id, GX_BOOL tile)
{
    /* assign the resource id */
    window -> gx_window_wallpaper = wallpaper_id;

    /* check for centered or tiled */
    if (tile)
    {
        window -> gx_widget_style |= GX_STYLE_TILE_WALLPAPER;
    }
    else
    {
        window -> gx_widget_style &= (ULONG)(~GX_STYLE_TILE_WALLPAPER);
    }

    /* if I am already visible then I need to mark as dirty */
    if (window -> gx_widget_status & GX_STATUS_VISIBLE)
    {
        _gx_system_dirty_mark((GX_WIDGET *)window);
    }

    return(GX_SUCCESS);
}

