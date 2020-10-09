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
/**   Progress Bar Management (Progress Bar)                              */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */
#include "gx_api.h"
#include "gx_widget.h"
#include "gx_utility.h"
#include "gx_progress_bar.h"

#define GX_MAX_PROGRESS_BAR_TEXT_LENGTH 11

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_progress_bar_text_draw                          PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function draws the specified progress bar with text.           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    progress_bar                          Progress Bar control block    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_ltoa                      Converts a long integer value */
/*                                            into an ASCII string        */
/*    _gx_utility_string_length_check       Test string length            */
/*    _gx_widget_text_draw                  Draw the text on the widget   */
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
VOID  _gx_progress_bar_text_draw(GX_PROGRESS_BAR *progress_bar)
{
GX_WIDGET     *widget = (GX_WIDGET *)progress_bar;
GX_RESOURCE_ID color;
GX_CHAR        pText[GX_MAX_PROGRESS_BAR_TEXT_LENGTH + 1] = "";
INT            cur_value;
INT            range;
INT            min_value;
INT            max_value;
GX_STRING      string;

    /* Pick up the text color.  */
    if (progress_bar -> gx_widget_style & GX_STYLE_ENABLED)
    {
        if (progress_bar -> gx_widget_style & GX_STYLE_DRAW_SELECTED)
        {
            color = progress_bar -> gx_progress_bar_info.gx_progress_bar_selected_text_color;
        }
        else
        {
            color = progress_bar -> gx_progress_bar_info.gx_progress_bar_normal_text_color;
        }
    }
    else
    {
        color = progress_bar -> gx_progress_bar_info.gx_progress_bar_disabled_text_color;
    }

    string.gx_string_ptr = pText;

    cur_value = progress_bar -> gx_progress_bar_info.gx_progress_bar_info_current_val;
    if (progress_bar -> gx_widget_style & GX_STYLE_PROGRESS_PERCENT)
    {
        min_value = progress_bar -> gx_progress_bar_info.gx_progress_bar_info_min_val;
        max_value = progress_bar -> gx_progress_bar_info.gx_progress_bar_info_max_val;
        range = max_value - min_value;
        if (range != 0)
        {
            _gx_utility_ltoa(((cur_value - min_value) * 100) / range, pText, sizeof(pText) - 1);

            /* Calculate string length. */
            _gx_utility_string_length_check(string.gx_string_ptr, &string.gx_string_length, GX_MAX_PROGRESS_BAR_TEXT_LENGTH - 1);

            /* Add "%" to the end of text. */
            pText[string.gx_string_length] = '%';
            pText[string.gx_string_length + 1] = '\0';
            string.gx_string_length ++;
        }
    }
    else
    {
        _gx_utility_ltoa(cur_value, pText, sizeof(pText));
        _gx_utility_string_length_check(string.gx_string_ptr, &string.gx_string_length, GX_MAX_PROGRESS_BAR_TEXT_LENGTH);
    }

    _gx_widget_text_draw_ext(widget, color, progress_bar -> gx_progress_bar_info.gx_progress_bar_font_id, &string, 0, 0);
}

