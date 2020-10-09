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
/**   Text View                                                           */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_canvas.h"
#include "gx_context.h"
#include "gx_text_input_cursor.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_text_input_cursor_draw                          PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function calculates the cursor position and draws the cursor   */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    widget                                Widget control block          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_canvas_line_draw                  Draw the the specified line   */
/*                                          on the current context        */
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
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
VOID _gx_text_input_cursor_draw(GX_TEXT_INPUT_CURSOR *cursor_ptr)
{
GX_POINT cursor_pos = cursor_ptr -> gx_text_input_cursor_pos;
GX_VALUE y_start;
GX_VALUE y_end;

    if(!cursor_ptr->gx_text_input_cursor_height)
    {
        return;
    }

    y_start = (GX_VALUE)(cursor_pos.gx_point_y - (cursor_ptr->gx_text_input_cursor_height >> 1));
    y_end = (GX_VALUE)(y_start + cursor_ptr->gx_text_input_cursor_height - 1);

    /* Set brush width to cursor width. */
    _gx_context_brush_width_set((UINT)(cursor_ptr -> gx_text_input_cursor_width));

    /* Draw cursor line. */
    _gx_canvas_line_draw(cursor_pos.gx_point_x, y_start, cursor_pos.gx_point_x, y_end);
}

