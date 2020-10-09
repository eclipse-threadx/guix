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
/**   Display Management (Display)                                        */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_utility.h"
#include "gx_display.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_generic_pie_fill                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Display driver to fill a pie.                                       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xcenter                               x-coord of center of circle   */
/*    ycenter                               y-coord of center of circle   */
/*    r                                     Radius of circle              */
/*    start_angle                           The start angle of circle arc */
/*    end_angle                             The end angle of circle arc   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_display_driver_generic_simple_pie_fill                          */
/*                                          Real display driver draw      */
/*                                            filled-pie function         */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_canvas_pie_draw                                                 */
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
#if defined(GX_ARC_DRAWING_SUPPORT)

VOID _gx_display_driver_generic_pie_fill(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r, INT start_angle, INT end_angle)
{
/* The function fills a pie.*/


    if (start_angle < 180)
    {
        if (end_angle < 180)
        {
            _gx_display_driver_generic_simple_pie_fill(context, xcenter, ycenter, r, start_angle, end_angle, GX_FALSE);
        }
        else if (end_angle < 360)
        {
            /* Skip-end parameter should only be set when drawing the above area. 
               It would be set to GX_TRUE to skip the bottom line to avoid case that this line will be drawn twice, which
               is not correct, when brush alpha is set. */
            _gx_display_driver_generic_simple_pie_fill(context, xcenter, ycenter, r, start_angle, 180, GX_TRUE);
            _gx_display_driver_generic_simple_pie_fill(context, xcenter, ycenter, r, 180, end_angle, GX_FALSE);
        }
        else
        {
            _gx_display_driver_generic_simple_pie_fill(context, xcenter, ycenter, r, start_angle, 180, GX_TRUE);
            _gx_display_driver_generic_simple_pie_fill(context, xcenter, ycenter, r, 180, 360, GX_FALSE);
            _gx_display_driver_generic_simple_pie_fill(context, xcenter, ycenter, r, 0, end_angle - 360, GX_TRUE);
        }
    }
    else
    {
        if (end_angle < 360)
        {
            _gx_display_driver_generic_simple_pie_fill(context, xcenter, ycenter, r, start_angle, end_angle, GX_FALSE);
        }
        else if (end_angle < 540)
        {
            _gx_display_driver_generic_simple_pie_fill(context, xcenter, ycenter, r, start_angle, 360, GX_FALSE);
            _gx_display_driver_generic_simple_pie_fill(context, xcenter, ycenter, r, 0, end_angle - 360, GX_TRUE);
        }
        else
        {
            _gx_display_driver_generic_simple_pie_fill(context, xcenter, ycenter, r, start_angle, 360, GX_FALSE);
            _gx_display_driver_generic_simple_pie_fill(context, xcenter, ycenter, r, 0, 180, GX_TRUE);
            _gx_display_driver_generic_simple_pie_fill(context, xcenter, ycenter, r, 180, end_angle - 360, GX_FALSE);
        }
    }
}

#endif

