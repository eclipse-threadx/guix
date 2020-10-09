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


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_sprite_create                                   PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates a sprite, which is a special type of          */
/*    widget.                                                             */
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
/*    button_id                             Application-defined ID of     */
/*                                             the sprite                 */
/*    size                                  Size of the sprite            */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_create                     Create the underlying widget  */
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
UINT  _gx_sprite_create(GX_SPRITE *sprite, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                        GX_SPRITE_FRAME *frame_list, USHORT frame_count,
                        ULONG style, USHORT sprite_id, GX_CONST GX_RECTANGLE *size)
{

    /* Call the widget create function.  */
    _gx_widget_create((GX_WIDGET *)sprite, name, GX_NULL, style, sprite_id, size);

    /* Populate the rest of prompt control block - overriding as necessary.  */
    sprite -> gx_widget_type =                   GX_TYPE_SPRITE;
    sprite -> gx_widget_draw_function =          (VOID (*)(GX_WIDGET *))_gx_sprite_draw;
    sprite -> gx_widget_event_process_function = (UINT (*)(GX_WIDGET *, GX_EVENT *))_gx_sprite_event_process;
    sprite -> gx_sprite_current_frame = 0;
    sprite -> gx_sprite_run_state = GX_SPRITE_IDLE;
    sprite -> gx_sprite_frame_list = frame_list;
    sprite -> gx_sprite_frame_count = frame_count;

    /* Determine if a parent widget was provided.  */
    if (parent)
    {
        _gx_widget_link(parent, (GX_WIDGET *)sprite);
    }

    /* Return the status from prompt create.  */
    return(GX_SUCCESS);
}

