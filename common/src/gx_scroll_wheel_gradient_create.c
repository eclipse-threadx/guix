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
/**   Scroll Wheel Management (Scroll Wheel)                              */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_scroll_wheel.h"
#include "gx_system.h"
#include "gx_utility.h"
#include "gx_widget.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_scroll_wheel_gradient_create                    PORTABLE C      */
/*                                                           6.1.5        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Create the gradient pixelmap used to overlay the scroll wheel       */
/*    widget drawing.                                                     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    wheel                                 Scroll wheel control block    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_gradient_create           Create a gradient pixelmap    */
/*    _gx_utility_gradient_delete           Delete a gradient             */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  12-31-2020     Kenneth Maxwell          Modified comment(s), added    */
/*                                            display rotation support,   */
/*                                            resulting in version 6.1.3  */
/*  03-02-2021     Kenneth Maxwell          Modified comment(s), added    */
/*                                            flip rotation support,      */
/*                                            resulting in version 6.1.5  */
/*                                                                        */
/**************************************************************************/
VOID _gx_scroll_wheel_gradient_create(GX_SCROLL_WHEEL *wheel)
{
GX_UBYTE    gradient_mode;
INT         height;
GX_CANVAS  *canvas;
GX_DISPLAY *display;

    /* test to see if the scroll wheel has an alpha-mask overlay */
    if (wheel -> gx_scroll_wheel_gradient.gx_gradient_alpha_start ||
        wheel -> gx_scroll_wheel_gradient.gx_gradient_alpha_end)
    {
        /* yes, test to see if it has been created */
        if (wheel -> gx_scroll_wheel_gradient.gx_gradient_pixelmap.gx_pixelmap_data == GX_NULL)
        {
            _gx_widget_canvas_get((GX_WIDGET *)wheel, &canvas);

            if (canvas)
            {
                display = canvas -> gx_canvas_display;
                gradient_mode = GX_GRADIENT_TYPE_MIRROR | GX_GRADIENT_TYPE_ALPHA;
                height = wheel -> gx_widget_size.gx_rectangle_bottom - wheel -> gx_widget_size.gx_rectangle_top + 1;

                if (display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_NONE ||
                    display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_FLIP)
                {
                    _gx_utility_gradient_create(&wheel -> gx_scroll_wheel_gradient, 3, (GX_VALUE)height,
                                                gradient_mode | GX_GRADIENT_TYPE_VERTICAL,
                                                wheel -> gx_scroll_wheel_gradient.gx_gradient_alpha_start,
                                                wheel -> gx_scroll_wheel_gradient.gx_gradient_alpha_end);
                }
                else
                {
                    /* Generate rotated gradient map. */
                    _gx_utility_gradient_create(&wheel -> gx_scroll_wheel_gradient, (GX_VALUE)height, 3,
                                                gradient_mode,
                                                wheel -> gx_scroll_wheel_gradient.gx_gradient_alpha_start,
                                                wheel -> gx_scroll_wheel_gradient.gx_gradient_alpha_end);

                    GX_SWAP_VALS(wheel -> gx_scroll_wheel_gradient.gx_gradient_pixelmap.gx_pixelmap_width,
                                 wheel -> gx_scroll_wheel_gradient.gx_gradient_pixelmap.gx_pixelmap_height);
                }
            }
        }
    }
    else
    {
        /* gradient is not used, if it has been created then delete it */
        _gx_utility_gradient_delete(&wheel -> gx_scroll_wheel_gradient);
    }
}

