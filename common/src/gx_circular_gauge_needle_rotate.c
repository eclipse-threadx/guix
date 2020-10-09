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
/**   Circular Gauge Management (Circular Gauge)                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_system.h"
#include "gx_circular_gauge.h"
#include "gx_utility.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_circular_gauge_needle_rotate                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function rotates the needle of the specified circular gauge.   */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    circular_gauge                        Circular gauge control block  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_context_pixelmap_get              Gets the pixelmap associated  */
/*                                            the supplied resource ID.   */
/*    _gx_utility_pixelmap_rotate           Rotate a pixelmap.            */
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
UINT  _gx_circular_gauge_needle_rotate(GX_CIRCULAR_GAUGE *gauge)
{
UINT                    status = GX_SUCCESS;
GX_CIRCULAR_GAUGE_INFO *info;
INT                    xcor;
INT                    ycor;


    /* Pick up pointer to the gauge information structure.  */
    info = &gauge -> gx_circular_gauge_info;

    if (gauge -> gx_circular_gauge_needle_source)
    {
        xcor = (INT)(info -> gx_circular_gauge_info_needle_xcor);
        ycor = (INT)(info -> gx_circular_gauge_info_needle_ycor);

        gauge -> gx_circular_gauge_current_needle_x = info -> gx_circular_gauge_info_needle_xpos;
        gauge -> gx_circular_gauge_current_needle_y = info -> gx_circular_gauge_info_needle_ypos;

        /* Rotate needle pixelmap. */

        status = _gx_utility_pixelmap_rotate(gauge -> gx_circular_gauge_needle_source, gauge -> gx_circular_gauge_current_angle,
                                                &(gauge -> gx_circular_gauge_needle_rotated),
                                                &xcor, &ycor);

        if (status == GX_SUCCESS)
        {
            /* Calculate start position of needle pixelmap.  */
            gauge -> gx_circular_gauge_current_needle_x = gauge -> gx_widget_size.gx_rectangle_left + info -> gx_circular_gauge_info_needle_xpos;

            gauge -> gx_circular_gauge_current_needle_x -= xcor;

            gauge -> gx_circular_gauge_current_needle_y = gauge -> gx_widget_size.gx_rectangle_top + info -> gx_circular_gauge_info_needle_ypos;
            gauge -> gx_circular_gauge_current_needle_y -= ycor;
        }
    }

    return status;
}

/**************************************************************************/
UINT  _gx_circular_gauge_needle_rotate_callback(VOID *gauge)
{
    return _gx_circular_gauge_needle_rotate((GX_CIRCULAR_GAUGE *)gauge);
}

