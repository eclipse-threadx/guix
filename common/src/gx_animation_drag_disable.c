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
/**   Animation Management (Animation)                                    */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_animation.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_animation_drag_disable                          PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function removes the screen slide aimation hook from the       */
/*    widget's default event process.                                     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    animation                             Pointer to animation control  */
/*                                            block                       */
/*    widget                                Pointer to widget control     */
/*                                            block                       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_animation_stop                    Stop an animation sequence    */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/**************************************************************************/
#if (GX_ANIMATION_POOL_SIZE > 0)
UINT  _gx_animation_drag_disable(GX_ANIMATION *animation, GX_WIDGET *widget)
{
UINT status;

    /* Replace the widget's event process function with its original event process function. */
    widget -> gx_widget_event_process_function = animation -> gx_animation_original_event_process_function;
    animation -> gx_animation_original_event_process_function = GX_NULL;

    /* Remove a animation from animation list. */
    status =  _gx_animation_stop(animation);
    return status;
}
#endif
