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
/**   Context Management (Context)                                        */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_context.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_context_pixelmap_get                            PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service gets the pixelmap associated with the supplied         */
/*      resource ID.                                                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    pixelmap_id                           Pixelmap resource ID          */
/*    return_pixelmap                       Pointer to pixelmap           */
/*                                            destination pointer         */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_checkbox_draw                                                   */
/*    _gx_context_pixelmap_set                                            */
/*    _gx_icon_button_draw                                                */
/*    _gx_icon_draw                                                       */
/*    _gx_pixelmap_button_draw                                            */
/*    _gx_pixelmap_prompt_draw                                            */
/*    _gx_pixelmap_slider_draw                                            */
/*    _gx_radio_button_draw                                               */
/*    _gx_scroll_thumb_draw                                               */
/*    _gx_scrollbar_draw                                                  */
/*    _gx_scrollbar_thumb_position_calculate                              */
/*    _gx_window_draw                                                     */
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
UINT  _gx_context_pixelmap_get(GX_RESOURCE_ID resource_id, GX_PIXELMAP **return_pixelmap)
{
GX_DRAW_CONTEXT *context = _gx_system_current_draw_context;
GX_DISPLAY      *display;

    display = context -> gx_draw_context_display;

    /* Determine if the ID is within range.  */
    if (resource_id < display -> gx_display_pixelmap_table_size)
    {

        /* Yes, the ID is within range. Perform a table lookup and return the
            pixelmap pointer.  */
        *return_pixelmap = display -> gx_display_pixelmap_table[resource_id];
        return(GX_SUCCESS);
    }
    else
    {
        *return_pixelmap = NULL;
        return(GX_INVALID_RESOURCE_ID);
    }
}

