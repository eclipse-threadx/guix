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
#include "gx_widget.h"
#include "gx_sprite.h"


/* Bring in externs for caller checking code.  */
GX_CALLER_CHECKING_EXTERNS

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_sprite_create                                  PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for error in the sprite create call.           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    sprite                                Pointer to sprite widget      */
/*                                            control block               */
/*    name                                  Logical name of sprite        */
/*    parent                                Pointer to parent widget      */
/*                                            of sprite                   */
/*    frame_list                            The frame_list to be assigned */
/*    frame_count                           Number of frames in the frame */
/*    style                                 Sprite stuyle                 */
/*    sprite_id                             Application-defined ID of     */
/*                                             the sprite                 */
/*    size                                  Size of the sprite            */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_sprite_create                     Actual sprite create call     */
/*    _gx_widget_link                       Link the sprite to its parent */
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
UINT  _gxe_sprite_create(GX_SPRITE *sprite, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                         GX_SPRITE_FRAME *frame_list, USHORT frame_count,
                         ULONG style, USHORT sprite_id, GX_CONST GX_RECTANGLE *size, UINT sprite_control_block_size)
{

UINT status;

    /* Check for appropriate caller.  */
    GX_INIT_AND_THREADS_CALLER_CHECKING

    /* Check for invalid input pointers.  */
    if ((sprite == GX_NULL) || (size == GX_NULL))
    {
        return(GX_PTR_ERROR);
    }

    /* Check for invalid widget control block size.  */
    if (sprite_control_block_size != sizeof(GX_SPRITE))
    {
        return(GX_INVALID_SIZE);
    }

    /* Check for widget already created.  */
    if (sprite -> gx_widget_type != 0)
    {
        return(GX_ALREADY_CREATED);
    }

    /* Check for invalid parent widget. */
    if (parent && (parent->gx_widget_type == 0))
    {
        return(GX_INVALID_WIDGET);
    }

    /* Call the actual sprite create function.  */
    status = _gx_sprite_create(sprite, name, parent, frame_list, frame_count, style, sprite_id, size);

    /* Return completion status.  */
    return status;
}

