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
/**   Drop List Management (List)                                         */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_window.h"
#include "gx_drop_list.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_drop_list_pixelmap_set                          PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Assign a pixelmap to the displayed on a drop list.                  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    drop_list                             Drop list control block       */
/*    id                                    Resource ID to the pixelmap   */
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
UINT _gx_drop_list_pixelmap_set(GX_DROP_LIST *drop_list, GX_RESOURCE_ID id)
{

    drop_list -> gx_drop_list_pixelmap = id;

    if (drop_list -> gx_widget_status & GX_STATUS_VISIBLE)
    {
        /* Mark widget as needing to be re-painted.  */
        _gx_system_dirty_mark((GX_WIDGET *)drop_list);
    }

    return GX_SUCCESS;
}

