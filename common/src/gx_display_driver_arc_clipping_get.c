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
/*    _gx_display_driver_arc_clipping_get                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function returns the clipping area of a circle arc.            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    xcenter                               x-coord of center of circle   */
/*    ycenter                               y-coord of center of circle   */
/*    r                                     Radius of circle              */
/*    start_angle                           Start angle for clipping      */
/*    end_argle                             End angle for clipping        */
/*    clip_1                                Clip result                   */
/*    cllp_2                                Clip result                   */
/*    clip_3                                Clip result                   */
/*    clip_4                                Clip result                   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_circle_point_get          Get point coord on a circle   */
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
/*                                                                        */
/**************************************************************************/
#if defined(GX_ARC_DRAWING_SUPPORT)
VOID _gx_display_driver_arc_clipping_get(INT xcenter, INT ycenter, UINT r, INT start_angle, INT end_angle,
                                         GX_RECTANGLE *clip_1, GX_RECTANGLE *clip_2, GX_RECTANGLE *clip_3, GX_RECTANGLE *clip_4)
{

GX_POINT start_point;
GX_POINT end_point;
GX_VALUE ori_r = (GX_VALUE)r;
GX_VALUE neg_r = (GX_VALUE)(-ori_r);

    memset(clip_1, 0, sizeof(GX_RECTANGLE));
    memset(clip_2, 0, sizeof(GX_RECTANGLE));
    memset(clip_3, 0, sizeof(GX_RECTANGLE));
    memset(clip_4, 0, sizeof(GX_RECTANGLE));

    /* Get two endpoint of the arc. */
    _gx_utility_circle_point_get(0, 0, r, start_angle, &start_point);
    _gx_utility_circle_point_get(0, 0, r, end_angle, &end_point);

    /* Calculate clipping. */
    if (start_angle < 90)
    {
        if (end_angle <= 90)
        {
            clip_1 -> gx_rectangle_left = end_point.gx_point_x;
            clip_1 -> gx_rectangle_top = end_point.gx_point_y;
            clip_1 -> gx_rectangle_right = start_point.gx_point_x;
            clip_1 -> gx_rectangle_bottom = start_point.gx_point_y;
        }
        else
        {
            clip_1 -> gx_rectangle_left = 0;
            clip_1 -> gx_rectangle_top = neg_r;
            clip_1 -> gx_rectangle_right = start_point.gx_point_x;
            clip_1 -> gx_rectangle_bottom = start_point.gx_point_y;

            if (end_angle <= 180)
            {
                clip_2 -> gx_rectangle_left = end_point.gx_point_x;
                clip_2 -> gx_rectangle_top = neg_r;
                clip_2 -> gx_rectangle_right = 0;
                clip_2 -> gx_rectangle_bottom = end_point.gx_point_y;
            }
            else
            {
                clip_2 -> gx_rectangle_left = neg_r;
                clip_2 -> gx_rectangle_top = neg_r;
                clip_2 -> gx_rectangle_right = 0;
                clip_2 -> gx_rectangle_bottom = 0;

                if (end_angle <= 270)
                {
                    clip_3 -> gx_rectangle_left = neg_r;
                    clip_3 -> gx_rectangle_top = 0;
                    clip_3 -> gx_rectangle_right = end_point.gx_point_x;
                    clip_3 -> gx_rectangle_bottom = end_point.gx_point_y;
                }
                else
                {
                    clip_2 -> gx_rectangle_bottom = ori_r;

                    if (end_angle <= 360)
                    {
                        clip_3 -> gx_rectangle_left = 0;
                        clip_3 -> gx_rectangle_top = end_point.gx_point_y;
                        clip_3 -> gx_rectangle_right = end_point.gx_point_x;
                        clip_3 -> gx_rectangle_bottom = ori_r;
                    }
                    else
                    {
                        clip_3 -> gx_rectangle_left = 0;
                        clip_3 -> gx_rectangle_top = 0;
                        clip_3 -> gx_rectangle_right = ori_r;
                        clip_3 -> gx_rectangle_bottom = ori_r;

                        clip_4 -> gx_rectangle_left = end_point.gx_point_x;
                        clip_4 -> gx_rectangle_top = end_point.gx_point_y;
                        clip_4 -> gx_rectangle_right = ori_r;
                        clip_4 -> gx_rectangle_bottom = 0;
                    }
                }
            }
        }
    }
    else if (start_angle < 180)
    {
        if (end_angle <= 180)
        {
            clip_1 -> gx_rectangle_left = end_point.gx_point_x;
            clip_1 -> gx_rectangle_top = start_point.gx_point_y;
            clip_1 -> gx_rectangle_right = start_point.gx_point_x;
            clip_1 -> gx_rectangle_bottom = end_point.gx_point_y;
        }
        else
        {
            clip_1 -> gx_rectangle_left = neg_r;
            clip_1 -> gx_rectangle_top = start_point.gx_point_y;
            clip_1 -> gx_rectangle_right = start_point.gx_point_x;
            clip_1 -> gx_rectangle_bottom = 0;

            if (end_angle <= 270)
            {
                clip_2 -> gx_rectangle_left = neg_r;
                clip_2 -> gx_rectangle_top = 0;
                clip_2 -> gx_rectangle_right = end_point.gx_point_x;
                clip_2 -> gx_rectangle_bottom = end_point.gx_point_y;
            }
            else
            {
                clip_2 -> gx_rectangle_left = neg_r;
                clip_2 -> gx_rectangle_top = 0;
                clip_2 -> gx_rectangle_right = 0;
                clip_2 -> gx_rectangle_bottom = ori_r;

                if (end_angle <= 360)
                {
                    clip_3 -> gx_rectangle_left = 0;
                    clip_3 -> gx_rectangle_top = end_point.gx_point_y;
                    clip_3 -> gx_rectangle_right = end_point.gx_point_x;
                    clip_3 -> gx_rectangle_bottom = ori_r;
                }
                else
                {
                    clip_2 -> gx_rectangle_right = ori_r;

                    if (end_angle <= 450)
                    {
                        clip_3 -> gx_rectangle_left = end_point.gx_point_x;
                        clip_3 -> gx_rectangle_top = end_point.gx_point_y;
                        clip_3 -> gx_rectangle_right = ori_r;
                        clip_3 -> gx_rectangle_bottom = 0;
                    }
                    else
                    {
                        clip_3 -> gx_rectangle_left = 0;
                        clip_3 -> gx_rectangle_top = neg_r;
                        clip_3 -> gx_rectangle_right = ori_r;
                        clip_3 -> gx_rectangle_bottom = 0;

                        clip_4 -> gx_rectangle_left = end_point.gx_point_x;
                        clip_4 -> gx_rectangle_top = neg_r;
                        clip_4 -> gx_rectangle_right = 0;
                        clip_4 -> gx_rectangle_bottom = end_point.gx_point_y;
                    }
                }
            }
        }
    }
    else if (start_angle < 270)
    {
        if (end_angle <= 270)
        {
            clip_1 -> gx_rectangle_left = start_point.gx_point_x;
            clip_1 -> gx_rectangle_top = start_point.gx_point_y;
            clip_1 -> gx_rectangle_right = end_point.gx_point_x;
            clip_1 -> gx_rectangle_bottom = end_point.gx_point_y;
        }
        else
        {
            clip_1 -> gx_rectangle_left = start_point.gx_point_x;
            clip_1 -> gx_rectangle_top = start_point.gx_point_y;
            clip_1 -> gx_rectangle_right = 0;
            clip_1 -> gx_rectangle_bottom = ori_r;

            if (end_angle <= 360)
            {
                clip_2 -> gx_rectangle_left = 0;
                clip_2 -> gx_rectangle_top = end_point.gx_point_y;
                clip_2 -> gx_rectangle_right = end_point.gx_point_x;
                clip_2 -> gx_rectangle_bottom = ori_r;
            }
            else
            {
                clip_2 -> gx_rectangle_left = 0;
                clip_2 -> gx_rectangle_top = 0;
                clip_2 -> gx_rectangle_right = ori_r;
                clip_2 -> gx_rectangle_bottom = ori_r;

                if (end_angle <= 450)
                {
                    clip_3 -> gx_rectangle_left = end_point.gx_point_x;
                    clip_3 -> gx_rectangle_top = end_point.gx_point_y;
                    clip_3 -> gx_rectangle_right = ori_r;
                    clip_3 -> gx_rectangle_bottom = ori_r;
                }
                else
                {
                    clip_2 -> gx_rectangle_top = neg_r;

                    if (end_angle <= 540)
                    {
                        clip_3 -> gx_rectangle_left = end_point.gx_point_x;
                        clip_3 -> gx_rectangle_top = neg_r;
                        clip_3 -> gx_rectangle_right = 0;
                        clip_3 -> gx_rectangle_bottom = end_point.gx_point_y;
                    }
                    else
                    {
                        clip_3 -> gx_rectangle_left = neg_r;
                        clip_3 -> gx_rectangle_top = neg_r;
                        clip_3 -> gx_rectangle_right = 0;
                        clip_3 -> gx_rectangle_bottom = 0;

                        clip_4 -> gx_rectangle_left = neg_r;
                        clip_4 -> gx_rectangle_top = 0;
                        clip_4 -> gx_rectangle_right = end_point.gx_point_x;
                        clip_4 -> gx_rectangle_bottom = end_point.gx_point_y;
                    }
                }
            }
        }
    }
    else
    {
        if (end_angle <= 360)
        {
            clip_1 -> gx_rectangle_left = start_point.gx_point_x;
            clip_1 -> gx_rectangle_top = end_point.gx_point_y;
            clip_1 -> gx_rectangle_right = end_point.gx_point_x;
            clip_1 -> gx_rectangle_bottom = start_point.gx_point_y;
        }
        else
        {
            clip_1 -> gx_rectangle_left = start_point.gx_point_x;
            clip_1 -> gx_rectangle_top = 0;
            clip_1 -> gx_rectangle_right = ori_r;
            clip_1 -> gx_rectangle_bottom = start_point.gx_point_y;

            if (end_angle <= 450)
            {
                clip_2 -> gx_rectangle_left = end_point.gx_point_x;
                clip_2 -> gx_rectangle_top = end_point.gx_point_y;
                clip_2 -> gx_rectangle_right = ori_r;
                clip_2 -> gx_rectangle_bottom = 0;
            }
            else
            {
                clip_2 -> gx_rectangle_left = 0;
                clip_2 -> gx_rectangle_top = neg_r;
                clip_2 -> gx_rectangle_right = ori_r;
                clip_2 -> gx_rectangle_bottom = 0;

                if (end_angle <= 540)
                {
                    clip_3 -> gx_rectangle_left = end_point.gx_point_x;
                    clip_3 -> gx_rectangle_top = neg_r;
                    clip_3 -> gx_rectangle_right = 0;
                    clip_3 -> gx_rectangle_bottom = end_point.gx_point_y;
                }
                else
                {
                    clip_2 -> gx_rectangle_left = neg_r;

                    if (end_angle <= 630)
                    {
                        clip_3 -> gx_rectangle_left = neg_r;
                        clip_3 -> gx_rectangle_top = 0;
                        clip_3 -> gx_rectangle_right = end_point.gx_point_x;
                        clip_3 -> gx_rectangle_bottom = end_point.gx_point_y;
                    }
                    else
                    {
                        clip_3 -> gx_rectangle_left = neg_r;
                        clip_3 -> gx_rectangle_top = 0;
                        clip_3 -> gx_rectangle_right = 0;
                        clip_3 -> gx_rectangle_bottom = ori_r;

                        clip_4 -> gx_rectangle_left = 0;
                        clip_4 -> gx_rectangle_top = end_point.gx_point_y;
                        clip_4 -> gx_rectangle_right = end_point.gx_point_x;
                        clip_4 -> gx_rectangle_bottom = ori_r;
                    }
                }
            }
        }
    }

    clip_1 -> gx_rectangle_left = (GX_VALUE)(clip_1 -> gx_rectangle_left + (GX_VALUE)xcenter);
    clip_1 -> gx_rectangle_top = (GX_VALUE)(clip_1 -> gx_rectangle_top + (GX_VALUE)ycenter);
    clip_1 -> gx_rectangle_right = (GX_VALUE)(clip_1 -> gx_rectangle_right + (GX_VALUE)xcenter);
    clip_1 -> gx_rectangle_bottom = (GX_VALUE)(clip_1 -> gx_rectangle_bottom + (GX_VALUE)ycenter);

    clip_2 -> gx_rectangle_left = (GX_VALUE)(clip_2 -> gx_rectangle_left + (GX_VALUE)xcenter);
    clip_2 -> gx_rectangle_top = (GX_VALUE)(clip_2 -> gx_rectangle_top + (GX_VALUE)ycenter);
    clip_2 -> gx_rectangle_right = (GX_VALUE)(clip_2 -> gx_rectangle_right + (GX_VALUE)xcenter);
    clip_2 -> gx_rectangle_bottom = (GX_VALUE)(clip_2 -> gx_rectangle_bottom + (GX_VALUE)ycenter);

    clip_3 -> gx_rectangle_left = (GX_VALUE)(clip_3 -> gx_rectangle_left + (GX_VALUE)xcenter);
    clip_3 -> gx_rectangle_top = (GX_VALUE)(clip_3 -> gx_rectangle_top + (GX_VALUE)ycenter);
    clip_3 -> gx_rectangle_right = (GX_VALUE)(clip_3 -> gx_rectangle_right + (GX_VALUE)xcenter);
    clip_3 -> gx_rectangle_bottom = (GX_VALUE)(clip_3 -> gx_rectangle_bottom + (GX_VALUE)ycenter);

    clip_4 -> gx_rectangle_left = (GX_VALUE)(clip_4 -> gx_rectangle_left + (GX_VALUE)xcenter);
    clip_4 -> gx_rectangle_top = (GX_VALUE)(clip_4 -> gx_rectangle_top + (GX_VALUE)ycenter);
    clip_4 -> gx_rectangle_right = (GX_VALUE)(clip_4 -> gx_rectangle_right + (GX_VALUE)xcenter);
    clip_4 -> gx_rectangle_bottom = (GX_VALUE)(clip_4 -> gx_rectangle_bottom + (GX_VALUE)ycenter);
}
#endif

