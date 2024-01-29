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
/**   Sprite Management (Sprite)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE

/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_display.h"
#include "gx_context.h"
#include "gx_canvas.h"
#include "gx_widget.h"
#include "gx_sprite.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_sprite_current_frame_set                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service assigns the frame list to the sprite widget.           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    sprite                                Pointer to sprite widget      */
/*                                            control block               */
/*    frame_index                           The frame index to display    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_dirty_mark                 Mark the widget as dirty      */
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
UINT  _gx_sprite_current_frame_set(GX_SPRITE *sprite, USHORT frame_index)
{
GX_WIDGET *widget = (GX_WIDGET *)sprite;

    if (frame_index < sprite -> gx_sprite_frame_count)
    {
        sprite -> gx_sprite_current_frame = frame_index;
    }

    if (sprite -> gx_widget_status & GX_STATUS_VISIBLE)
    {
        _gx_system_dirty_mark(widget);
    }
    return(GX_SUCCESS);
}

