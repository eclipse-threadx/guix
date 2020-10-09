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
#include "gx_button.h"
#include "gx_system.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_pixelmap_button_pixelmap_set                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function sets a  pixelmap to the pixelmap button.              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    button                                Button control block          */
/*    normal_id                             normal state pixelmap id      */
/*    selected_id                           selected state pixelmap id    */
/*    disabled_id                           disabled state pixelmap id    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_pixelmap_button_transparent_detect                              */
/*                                          Detect the transparency       */
/*    _gx_system_dirty_mark                 Mark the widget as dirty      */
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
UINT  _gx_pixelmap_button_pixelmap_set(GX_PIXELMAP_BUTTON *button,
                                       GX_RESOURCE_ID normal_id,
                                       GX_RESOURCE_ID selected_id,
                                       GX_RESOURCE_ID disabled_id)
{
    button -> gx_pixelmap_button_normal_id = normal_id;
    button -> gx_pixelmap_button_selected_id = selected_id;
    button -> gx_pixelmap_button_disabled_id = disabled_id;

    /* check to see if any of my pixelmaps have transparency, and if so mark
       myself as being transparent.  */

    if (button -> gx_widget_status & GX_STATUS_VISIBLE)
    {
        _gx_pixelmap_button_transparent_detect(button);

        /* Mark widget as needing to be re-painted.  */
        _gx_system_dirty_mark((GX_WIDGET *)button);
    }
    return(GX_SUCCESS);
}

