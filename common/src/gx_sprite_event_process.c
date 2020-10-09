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
/**   Sprite Management (Sprite)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_widget.h"
#include "gx_sprite.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_sprite_event_process                            PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function processes events for the sprite widget.               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    sprite                                Pointer to sprite control     */
/*                                            block                       */
/*    event_ptr                             Pointer to event to process   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_event_process              Call widget event processing  */
/*    _gx_sprite_start                      Start the sprite widget       */
/*    _gx_sprite_update                     Update the sprite widget      */
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
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
UINT  _gx_sprite_event_process(GX_SPRITE *sprite, GX_EVENT *event_ptr)
{
/* GX_EVENT   newevent; */
GX_WIDGET *widget = (GX_WIDGET *)sprite;

/* Default status to success.  */
UINT status =  GX_SUCCESS;

    /* Process relative to the type of event.  */
    switch (event_ptr -> gx_event_type)
    {
    case GX_EVENT_SHOW:
        status = _gx_widget_event_process(widget, event_ptr);

        if (sprite -> gx_widget_style & GX_STYLE_SPRITE_AUTO)
        {
            _gx_sprite_start(sprite, sprite -> gx_sprite_current_frame);
        }
        break;

    case GX_EVENT_TIMER:
        if (event_ptr -> gx_event_payload.gx_event_timer_id == GX_SPRITE_TIMER)
        {
            _gx_sprite_update(sprite);
        }
        else
        {
            status = _gx_widget_event_process(widget, event_ptr);
        }
        break;

    default:
        /* Call the widget default processing.  */
        status =  _gx_widget_event_process(widget, event_ptr);
    }

    /* Return completion status.  */
    return(status);
}

