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
/**   Menu Management (Menu)                                              */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_prompt.h"
#include "gx_menu.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_menu_event_process                              PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function processes events for the specified menu.              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    menu                                  Pointer to menu control       */
/*                                            block                       */
/*    event_ptr                             Incoming event to process     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_delete                     Delete a widget               */
/*    _gx_prompt_event_process              Default prompt event process  */
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
/*  12-31-2020     Kenneth Maxwell          Initial Version 6.1.3         */
/*                                                                        */
/**************************************************************************/
UINT  _gx_menu_event_process(GX_MENU *menu, GX_EVENT *event_ptr)
{

    /* Process relative to the type of event.  */
    switch (event_ptr -> gx_event_type)
    {
    case GX_EVENT_DELETE:
        if (menu -> gx_menu_list.gx_widget_first_child &&
            (menu -> gx_menu_list.gx_widget_parent == GX_NULL))
        {
            _gx_widget_delete((GX_WIDGET *)&menu -> gx_menu_list);
        }
        break;

    default:

        /* Do nothing.  */
        break;
    }

    /* Return completion status.  */
    return _gx_prompt_event_process((GX_PROMPT *)menu, event_ptr);
}

