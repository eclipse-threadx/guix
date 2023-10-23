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
/*    _gxe_checkbox_pixelmap_set                          PORTABLE C      */
/*                                                           6.3.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in the checkbox pixlemap set        */
/*    routine.                                                            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    button                                Button control block          */
/*    unchecked_id                          Resource ID of the pixelmap   */
/*                                            used for unchecked box      */
/*    checked_id                            Resource ID of the pixelmap   */
/*                                            used for checked box        */
/*    unchecked_disabled_id                 Resource ID of the pixelmap   */
/*                                            used for unchecked disabled */
/*                                             box                        */
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
/*    _gx_checkbox_pixelmap_set             Actual function sets the      */
/*                                            pixelmap                    */
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
/*  10-31-2023     Ting Zhu                 Modified comment(s),          */
/*                                            added invalid widget check, */
/*                                            resulting in version 6.3.0  */
/*                                                                        */
/**************************************************************************/
UINT  _gxe_checkbox_pixelmap_set(GX_CHECKBOX *checkbox,
                                 GX_RESOURCE_ID unchecked_id,
                                 GX_RESOURCE_ID checked_id,
                                 GX_RESOURCE_ID unchecked_disabled_id,
                                 GX_RESOURCE_ID checked_disabled_id)
{
UINT status;

    /* Check for the invalid input pointers.  */
    if (checkbox == GX_NULL)
    {
        return(GX_PTR_ERROR);
    }

    /* Check for the invalid widget.  */
    if (checkbox -> gx_widget_type == 0)
    {
        return(GX_INVALID_WIDGET);
    }

    status = _gx_checkbox_pixelmap_set(checkbox, unchecked_id, checked_id,
                                       unchecked_disabled_id, checked_disabled_id);

    return(status);
}

