/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation 
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
#include "gx_system.h"
#include "gx_utility.h"

/* Bring in externs for caller checking code.  */
GX_CALLER_CHECKING_EXTERNS

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_radio_button_pixelmap_set                      PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in the checkbox pixlemap set routine*/
/*    button.                                                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    button                                Button control block          */
/*    off_id                                Resource ID of the pixelmap   */
/*                                            used for draw button off    */
/*    on_id                                 Resource ID of the pixelmap   */
/*                                            used for checke  d box      */
/*    off_disabled_id                       Resource ID of the pixelmap   */
/*                                            used for unchecked disabled */
/*                                             box                        */
/*    on_disabled_id                        Resource ID of the pixelmap   */
/*                                            used for checked disabled   */
/*                                            box                         */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_radio_button_pixelmap_set         Acutal pixelmap set function  */
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
UINT  _gxe_radio_button_pixelmap_set(GX_RADIO_BUTTON *button,
                                     GX_RESOURCE_ID off_id,
                                     GX_RESOURCE_ID on_id,
                                     GX_RESOURCE_ID off_disabled_id,
                                     GX_RESOURCE_ID on_disabled_id)
{
UINT status;

    /* Check for appropriate caller.  */
    GX_INIT_AND_THREADS_CALLER_CHECKING

    /* Check for the invalid input pointers.  */
    if (button == GX_NULL)
    {
        return(GX_PTR_ERROR);
    }

    /* Check for invalid widget. */
    if (button->gx_widget_type == 0)
    {
        return(GX_INVALID_WIDGET);
    }

    status = _gx_radio_button_pixelmap_set(button, off_id, on_id,
                                           off_disabled_id, on_disabled_id);

    return(status);
}

