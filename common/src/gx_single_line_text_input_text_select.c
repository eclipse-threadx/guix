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
/**   Text Input Management (Single Line Text Input)                      */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_single_line_text_input.h"
#include "gx_text_input_cursor.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_single_line_text_input_text_select              PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function highlighs text with specified start mark and end mark */
/*    index.                                                              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_input                            Single-line text input widget */
/*                                            control block               */
/*    start_mark                            The index of the first        */
/*                                            highlight character.        */
/*    end_mark                              The index of the last         */
/*                                            highlight character         */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_dirty_mark                Mark widget as drity           */
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
UINT _gx_single_line_text_input_text_select(GX_SINGLE_LINE_TEXT_INPUT *input, UINT start_index, UINT end_index)
{
GX_RECTANGLE dirty_rect;
UINT         start_mark = input -> gx_single_line_text_input_start_mark;
UINT         end_mark = input -> gx_single_line_text_input_end_mark;

    if (start_index <= end_index)
    {
        input -> gx_single_line_text_input_start_mark = start_index;
        input -> gx_single_line_text_input_end_mark = end_index + 1;
    }
    else
    {
        input -> gx_single_line_text_input_start_mark = start_index + 1;
        input -> gx_single_line_text_input_end_mark = end_index;
    }

    if (input -> gx_widget_status & GX_STATUS_VISIBLE)
    {
        if (start_mark != end_mark)
        {
            _gx_single_line_text_input_text_rectangle_get(input, (INT)(start_mark - end_mark), &dirty_rect);

            /* Mark old highlight area as dirty. */
            _gx_system_dirty_partial_add((GX_WIDGET *)input, &dirty_rect);
        }
        else
        {
            _gx_text_input_cursor_dirty_rectangle_get(&input->gx_single_line_text_input_cursor_instance, &dirty_rect);

            /* Mark cursor area as dirty. */
            _gx_system_dirty_partial_add((GX_WIDGET *)input, &dirty_rect);
        }

        start_mark = input -> gx_single_line_text_input_start_mark;
        end_mark = input -> gx_single_line_text_input_end_mark;

        if (input -> gx_single_line_text_input_insert_pos != end_mark)
        {
            input -> gx_single_line_text_input_insert_pos = end_mark;
            _gx_single_line_text_input_position_update(input);
        }

        _gx_single_line_text_input_text_rectangle_get(input, (INT)(start_mark - end_mark), &dirty_rect);

        /* Mark highlight text area as dirty. */
        _gx_system_dirty_partial_add((GX_WIDGET *)input, &dirty_rect);
    }

    return GX_SUCCESS;
}

