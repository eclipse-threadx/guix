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
/**   Utility (Utility)                                                   */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE

/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_canvas.h"
#include "gx_system.h"
#include "gx_utility.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_gradient_delete                         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function deletes a previously created gradient.                */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    gradient                              Pointer to GX_GRADIENT        */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*  CALLED BY                                                             */
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
UINT _gx_utility_gradient_delete(GX_GRADIENT *gradient)
{
const GX_UBYTE *data_ptr = gradient -> gx_gradient_pixelmap.gx_pixelmap_data;
GX_GRADIENT    *search;
GX_BOOL         in_use = GX_FALSE;

    GX_ENTER_CRITICAL

    if (data_ptr)
    {
        /* check to see if any other gradients are using same pixelmap data,
           if not, then time to delete pixelmap data
         */
        search = _gx_system_gradient_list;

        while (search)
        {
            if (search != gradient &&
                search -> gx_gradient_pixelmap.gx_pixelmap_data == data_ptr)
            {
                in_use = GX_TRUE;
                break;
            }
            search = search -> gx_gradient_next;
        }

        /* unlink this gradient from gradient list */

        if (gradient -> gx_gradient_next)
        {
            gradient -> gx_gradient_next -> gx_gradient_previous = gradient -> gx_gradient_previous;
        }
        if (gradient -> gx_gradient_previous)
        {
            gradient -> gx_gradient_previous -> gx_gradient_next = gradient -> gx_gradient_next;
        }
        else
        {
            _gx_system_gradient_list = gradient -> gx_gradient_next;
        }
    }

    GX_EXIT_CRITICAL

    if (data_ptr && !in_use && _gx_system_memory_free)
    {
        _gx_system_memory_free((void *)data_ptr);
    }

    memset(gradient, 0, sizeof(GX_GRADIENT));

    return GX_SUCCESS;
}

