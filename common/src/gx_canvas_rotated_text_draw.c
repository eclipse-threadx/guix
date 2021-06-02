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
/**   Canvas Management (Canvas)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE

/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_utility.h"
#include "gx_canvas.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_canvas_rotated_text_draw                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION (deprecated)                                              */
/*                                                                        */
/*    This function draws rotated text.                                   */
/*    This function is superseded by gx_canvas_rotated_text_draw_ext()    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text                                  Pointer to string             */
/*    xcenter                               Center point for text drawing */
/*    ycenter                               Center point for text drawing */
/*    angle                                 Angle at which to rotate text */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_string_length_check       Calculate string length       */
/*    _gx_canvas_rotated_text_draw_ext      Extended rotated text draw    */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Software                                                */
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
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gx_canvas_rotated_text_draw(GX_CONST GX_CHAR *text,
                                  GX_VALUE xcenter,
                                  GX_VALUE ycenter,
                                  INT angle)
{
GX_STRING string;
UINT      status;

    string.gx_string_ptr = text;

    /* Calcualte text length. */
    status = _gx_utility_string_length_check(text, &string.gx_string_length, GX_MAX_STRING_LENGTH);
    if (status != GX_SUCCESS)
    {
        return status;
    }

    status = _gx_canvas_rotated_text_draw_ext(&string, xcenter, ycenter, angle);

    return status;
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_canvas_rotated_text_draw_ext                    PORTABLE C      */
/*                                                           6.1.7        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function draws rotated text.                                   */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text                                  Pointer to string             */
/*    xcenter                               Center point for text drawing */
/*    ycenter                               Center point for text drawing */
/*    angle                                 Angle at which to rotate text */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_string_width_get_ext       Get width of the string in    */
/*                                           pixels                       */
/*    _gx_canvas_text_draw                  Draw glyphs on canvas         */
/*    _gx_utility_string_to_alphamap        Convert string to alpha-map   */
/*    _gx_utiity_pixelmap_rotate            Rotate alphaap to desired     */
/*                                           angle                        */
/*    _gx_canvas_pixelmap_draw              Draw text alphamap            */
/*    _gx_system_memory_free                Free memory used for rotated  */
/*                                           alphamap and canvas          */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Software                                                */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  06-02-2021     Kenneth Maxwell          Modified comment(s),          */
/*                                            rename RENESAS_DAVE2D       */
/*                                            support conditional,        */
/*                                            resulting in version 6.1.7  */
/*                                                                        */
/**************************************************************************/
UINT _gx_canvas_rotated_text_draw_ext(GX_CONST GX_STRING *text,
                                      GX_VALUE xcenter,
                                      GX_VALUE ycenter,
                                      INT angle)
{
GX_PIXELMAP textmap;
GX_PIXELMAP rotated_map;
INT         x_pos;
INT         y_pos;
GX_FONT    *font;
GX_VALUE    alphamap_width;

/* pickup pointer to current context */
GX_DRAW_CONTEXT *context = _gx_system_current_draw_context;

    if (!context)
    {
        return GX_FAILURE;
    }

    /* get pointer to current font */
    font = context -> gx_draw_context_brush.gx_brush_font;

    if (!font)
    {
        return GX_FAILURE;
    }

    while (angle >= 360)
    {
        angle -= 360;
    }
    while (angle <= -360)
    {
        angle += 360;
    }

    if (!angle)
    {
        _gx_system_string_width_get_ext(font, text, &alphamap_width);
        x_pos = xcenter - (alphamap_width / 2);
        y_pos = ycenter - (font -> gx_font_line_height / 2);

        _gx_canvas_text_draw_ext((GX_VALUE)x_pos, (GX_VALUE)y_pos, text);
        return GX_SUCCESS;
    }

    if (_gx_utility_string_to_alphamap_ext(text, font, &textmap) == GX_SUCCESS)
    {
        /* Rotate the alpha map */
        x_pos = y_pos = 0;

        if (_gx_utility_pixelmap_rotate(&textmap, angle, &rotated_map, &x_pos, &y_pos) == GX_SUCCESS)
        {
            x_pos = xcenter - (rotated_map.gx_pixelmap_width / 2);
            y_pos = ycenter - (rotated_map.gx_pixelmap_height / 2);

            /* draw the alpha-map to render the text */
#if defined(GX_RENESAS_DAVE2D_DRAW)
            rotated_map.gx_pixelmap_flags |= GX_PIXELMAP_DYNAMICALLY_ALLOCATED;
#endif
            _gx_canvas_pixelmap_draw((GX_VALUE)x_pos, (GX_VALUE)y_pos, &rotated_map);

            /* free the rotated alphamap memory */
            _gx_system_memory_free((void *)rotated_map.gx_pixelmap_data);
        }

        /* free the temporary canvas memory */
        _gx_system_memory_free((void *)(textmap.gx_pixelmap_data));
    }
    return GX_SUCCESS;
}

