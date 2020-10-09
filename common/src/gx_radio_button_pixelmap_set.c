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
#include "gx_system.h"
#include "gx_utility.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_radio_button_pixelmap_set                       PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates a pixelmap button, which is a special type of */
/*    button.                                                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    button                                Button control block          */
/*    off_id                                Resource ID of the pixelmap   */
/*                                            used for drawing button off */
/*    on_id                                 Resource ID of the pixelmap   */
/*                                            used for drawing button on  */
/*    off_disabled_id                       Resource ID of the pixelmap   */
/*                                            used for draw button off    */
/*                                            disabled                    */
/*    on_disabled_id                        Resource ID of the pixelmap   */
/*                                            used for drawing button on  */
/*                                            disabled                    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
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
UINT  _gx_radio_button_pixelmap_set(GX_RADIO_BUTTON *button,
                                    GX_RESOURCE_ID off_id,
                                    GX_RESOURCE_ID on_id,
                                    GX_RESOURCE_ID off_disabled_id,
                                    GX_RESOURCE_ID on_disabled_id)
{

    if (off_id)
    {
        button -> gx_radio_button_off_pixelmap_id = off_id;
    }
    if (on_id)
    {
        button -> gx_radio_button_on_pixelmap_id = on_id;
    }
    if (off_disabled_id)
    {
        button -> gx_radio_button_off_disabled_pixelmap_id = off_disabled_id;
    }
    if (on_disabled_id)
    {
        button -> gx_radio_button_on_disabled_pixelmap_id = on_disabled_id;
    }

    if (button -> gx_widget_status & GX_STATUS_VISIBLE)
    {
        /* Mark widget as needing to be re-painted.  */
        _gx_system_dirty_mark((GX_WIDGET *)button);
    }
    return(GX_SUCCESS);
}

