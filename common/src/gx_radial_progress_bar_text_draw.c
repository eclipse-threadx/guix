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
/**   Progress Bar Management (Radial Progress Bar)                       */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */
#include "gx_api.h"
#include "gx_utility.h"
#include "gx_widget.h"
#include "gx_radial_progress_bar.h"

#define GX_MAX_RAIDAL_PROGRESS_BAR_TEXT_LENGTH 4

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_radial_progress_bar_text_draw                   PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function draws the specified radial progress bar with text.    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    progress_bar                          Radial Progress Bar control   */
/*                                            block                       */
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
VOID  _gx_radial_progress_bar_text_draw(GX_RADIAL_PROGRESS_BAR *progress_bar)
{
GX_RADIAL_PROGRESS_BAR_INFO *info;
GX_CHAR                      text[GX_MAX_RAIDAL_PROGRESS_BAR_TEXT_LENGTH + 1];
INT                          percent;
GX_RESOURCE_ID               color_id;
GX_STRING                    string;

    info = &progress_bar -> gx_radial_progress_bar_info;

    string.gx_string_ptr = text;

    if (progress_bar -> gx_widget_style & GX_STYLE_PROGRESS_PERCENT)
    {
    
        percent = GX_ABS(info -> gx_radial_progress_bar_info_current_val) * 100 / 360;
        _gx_utility_ltoa(percent, text, sizeof(text) - 1);

        /* Calculate text length. */
        _gx_utility_string_length_check(text, &string.gx_string_length, GX_MAX_RAIDAL_PROGRESS_BAR_TEXT_LENGTH - 1);

        /* Add "%" to the end of text. */
        memcpy(text + string.gx_string_length, "%", 2); /* Use case of memcpy is verified. */
        string.gx_string_length++;
    }
    else
    {
        _gx_utility_ltoa(info -> gx_radial_progress_bar_info_current_val, text, sizeof(text));
        _gx_utility_string_length_check(string.gx_string_ptr, &string.gx_string_length, sizeof(text) - 1);
    }

    if (progress_bar -> gx_widget_style & GX_STYLE_ENABLED)
    {
        if (progress_bar -> gx_widget_style & GX_STYLE_DRAW_SELECTED)
        {
            color_id = info -> gx_radial_progress_bar_info_selected_text_color;
        }
        else
        {
            color_id = info -> gx_radial_progress_bar_info_normal_text_color;
        }
    }
    else
    {
        color_id = info -> gx_radial_progress_bar_info_disabled_text_color;
    }

    _gx_widget_text_draw_ext((GX_WIDGET *)progress_bar,
                             color_id,
                             info -> gx_radial_progress_bar_info_font_id, &string, 0, 0);
}

