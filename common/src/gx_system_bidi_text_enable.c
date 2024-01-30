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
/**   System Management (System)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_system_bidi_text_enable                         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function enables display of bi-directional text.               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
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
/*                                            added logic to send dynamic */
/*                                            bidi text enable event,     */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
UINT  _gx_system_bidi_text_enable(VOID)
{
GX_WINDOW_ROOT *root;
GX_EVENT        bidi_text_event;

    _gx_system_bidi_text_enabled = GX_TRUE;

    memset(&bidi_text_event, 0, sizeof(GX_EVENT));
    bidi_text_event.gx_event_type = GX_EVENT_DYNAMIC_BIDI_TEXT_ENABLE;

    root = (GX_WINDOW_ROOT *)_gx_system_root_window_created_list;
    while (root)
    {
        if (root -> gx_window_root_canvas)
        {
            root -> gx_widget_event_process_function((GX_WIDGET *)root, &bidi_text_event);
        }
        root = (GX_WINDOW_ROOT *)root -> gx_widget_next;
    }

    _gx_system_all_canvas_dirty();

    return GX_SUCCESS;
}
#endif

