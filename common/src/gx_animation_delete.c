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
/**   Animation Management (Animation)                                    */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_animation.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_animation_delete                                PORTABLE C      */
/*                                                           6.1.7        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function deletes an animation sequence if the input animation  */
/*    pointer is not NULL, otherwise, deletes all animations belong to    */
/*    the animation parent.                                               */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    target                                Pointer to animation control  */
/*                                            block                       */
/*    parent                                Pointer to animation parent   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*   _gx_animation_stop                     Deactivate an animation       */
/*   _gx_system_animation_free              Free system animation         */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
/*    Application Code                                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  06-02-2021     Ting Zhu                 Initial Version 6.1.7         */
/*                                                                        */
/**************************************************************************/

UINT _gx_animation_delete(GX_ANIMATION *target, GX_WIDGET *parent)
{
GX_ANIMATION *animation;
GX_ANIMATION *next;

    if (target)
    {
        animation = target;
    }
    else
    {
        animation = _gx_system_animation_list;
    }

    while (animation)
    {
        if (target)
        {
            next = GX_NULL;
            parent = target -> gx_animation_info.gx_animation_parent;
        }
        else
        {
            next = animation -> gx_animation_next;
        }

        if (animation -> gx_animation_info.gx_animation_parent == parent)
        {

            /* Stop animation.  */
            _gx_animation_stop(animation);

            if (animation -> gx_animation_system_allocated)
            {

                /* If this animation came from the system pool, return it.  */
                _gx_system_animation_free(animation);
            }
            else
            {
                /* Invalid animation.  */
                animation -> gx_animation_status = 0;
            }
        }

        animation = next;
    }

    return(GX_SUCCESS);
}

