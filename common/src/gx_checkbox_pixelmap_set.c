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
/**   Button Management (Checkbox)                                        */
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
/*    _gx_checkbox_pixelmap_set                           PORTABLE C      */
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
/*    checkbox                              Checkbox control block        */
/*    unchecked_id                          Resource ID of the pixelmap   */
/*                                            used for unchecked box      */
/*    checked_id                            Resource ID of the pixelmap   */
/*                                            used for checked box        */
/*    unchecked_disabled_id                 Resource ID of the pixelmap   */
/*                                            used for unchecked disabled */
/*                                            box                         */
/*    checked_disabled_id                   Resource ID of the pixelmap   */
/*                                            used for checked disabled   */
/*                                            box                         */
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
UINT  _gx_checkbox_pixelmap_set(GX_CHECKBOX *checkbox,
                                GX_RESOURCE_ID unchecked_id,
                                GX_RESOURCE_ID checked_id,
                                GX_RESOURCE_ID unchecked_disabled_id,
                                GX_RESOURCE_ID checked_disabled_id)
{
    if (unchecked_id)
    {
        checkbox -> gx_checkbox_unchecked_pixelmap_id = unchecked_id;
    }
    if (checked_id)
    {
        checkbox -> gx_checkbox_checked_pixelmap_id = checked_id;
    }
    if (unchecked_disabled_id)
    {
        checkbox -> gx_checkbox_unchecked_disabled_pixelmap_id = unchecked_disabled_id;
    }
    if (checked_disabled_id)
    {
        checkbox -> gx_checkbox_checked_disabled_pixelmap_id = checked_disabled_id;
    }

    if (checkbox -> gx_widget_status & GX_STATUS_VISIBLE)
    {
        /* Mark widget as needing to be re-painted.  */
        _gx_system_dirty_mark((GX_WIDGET *)checkbox);
    }
    return(GX_SUCCESS);
}

