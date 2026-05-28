/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation
 * Copyright (c) 2026 Eclipse ThreadX contributors
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
/*    _gx_sprite_stop                                     PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service stops a sprite widget.                                 */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    sprite                                Pointer to sprite widget      */
/*                                           control block                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_timer_stop                 Stop an active GUIX timer     */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/**************************************************************************/
UINT  _gx_sprite_stop(GX_SPRITE *sprite)
{
GX_WIDGET *widget = (GX_WIDGET *)sprite;

    if (sprite -> gx_sprite_run_state == GX_SPRITE_RUNNING)
    {
        _gx_system_timer_stop(widget, GX_SPRITE_TIMER);
        sprite -> gx_sprite_run_state = GX_SPRITE_IDLE;
    }

    return(GX_SUCCESS);
}

