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
/**   Button Management (Button)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_context.h"
#include "gx_system.h"
#include "gx_widget.h"
#include "gx_text_input_cursor.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_text_input_cursor_dirty_rectangle_get           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function get the cursor area of the widget.                    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_input                            Single line text input        */
/*                                            control block               */
/*    event_type                            Input type                    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
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
UINT _gx_text_input_cursor_dirty_rectangle_get(GX_TEXT_INPUT_CURSOR *cursor_ptr, GX_RECTANGLE *dirty_rect)
{
GX_VALUE cur_height;
GX_VALUE cur_width;

    cur_height = cursor_ptr -> gx_text_input_cursor_height;
    cur_width = cursor_ptr -> gx_text_input_cursor_width;

    dirty_rect -> gx_rectangle_left = (GX_VALUE)(cursor_ptr -> gx_text_input_cursor_pos.gx_point_x - (cur_width >> 1));
    dirty_rect -> gx_rectangle_right = (GX_VALUE)(dirty_rect -> gx_rectangle_left + cur_width - 1);
    dirty_rect -> gx_rectangle_top = (GX_VALUE)(cursor_ptr -> gx_text_input_cursor_pos.gx_point_y - (cur_height >> 1));
    dirty_rect -> gx_rectangle_bottom = (GX_VALUE)(dirty_rect -> gx_rectangle_top + cur_height - 1);

    return GX_SUCCESS;
}

