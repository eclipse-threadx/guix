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
/**   Display Management (Display)                                        */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_display.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_active_language_set                     PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service sets the active language.                              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    display                               Pointer to display instance.  */
/*    language                              Language table id             */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_all_canvas_dirty           Mark all canvas dirty         */
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
UINT  _gx_display_active_language_set(GX_DISPLAY *display, GX_UBYTE language)
{
GX_EVENT   language_event;
GX_WINDOW_ROOT *root;

    /* Set display active language.  */

    display -> gx_display_active_language = language;
    memset(&language_event, 0, sizeof(GX_EVENT));
    language_event.gx_event_type = GX_EVENT_LANGUAGE_CHANGE;

    root = (GX_WINDOW_ROOT *)_gx_system_root_window_created_list;
    while (root)
    {
        if (root -> gx_window_root_canvas)
        {
            if (root -> gx_window_root_canvas -> gx_canvas_display == display)
            {
                root -> gx_widget_event_process_function((GX_WIDGET *)root, &language_event);
            }
        }
        root = (GX_WINDOW_ROOT *) root -> gx_widget_next;
    }

    _gx_system_all_canvas_dirty();

    /* Return success.  */
    return(GX_SUCCESS);
}

