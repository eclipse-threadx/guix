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
/**   System Management (System)                                          */
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
/*    _gx_system_animation_get                            PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Obtain an animation structure from the system pool.                 */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    animation                             Adress to return pointer      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*   _gx_system_lock                        lock system mutex             */
/*   _gx_system_unlock                      unlock system mutex           */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Application                                                    */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  12-31-2020     Kenneth Maxwell          Modified comment(s), init     */
/*                                            animation canvas to null,   */
/*                                            resulting in version 6.1.3  */
/*                                                                        */
/**************************************************************************/
#if (GX_ANIMATION_POOL_SIZE > 0)

UINT  _gx_system_animation_get(GX_ANIMATION **animation)
{
UINT status = GX_SUCCESS;
GX_ANIMATION *free_block;

    /* lock entering critical section */
    GX_ENTER_CRITICAL

    /* check for already having this timer */
    if (_gx_system_animation_free_list)
    {
        free_block = _gx_system_animation_free_list;
        _gx_system_animation_free_list = free_block -> gx_animation_next;
        free_block -> gx_animation_next = GX_NULL;
        free_block -> gx_animation_system_allocated = GX_TRUE;
        free_block -> gx_animation_status = GX_ANIMATION_IDLE;
        free_block -> gx_animation_canvas = GX_NULL;
        *animation = free_block;
    }
    else
    {
        *animation = GX_NULL;
        status = GX_OUT_OF_ANIMATIONS;
    }
 
    GX_EXIT_CRITICAL
    return status;
}
#endif
