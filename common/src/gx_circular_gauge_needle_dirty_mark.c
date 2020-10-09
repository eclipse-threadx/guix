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
#include "gx_system.h"
#include "gx_utility.h"
#include "gx_circular_gauge.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_circular_gauge_needle_dirty_mark                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function calculates current needle rectangle.                  */
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
/*    _gx_circular_gauge_needle_rectangle_calculate                       */
/*                                          Calculate the needle          */
/*                                            rectangle                   */
/*    _gx_system_dirty_partial_add          Add partial dirty area        */
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
UINT  _gx_circular_gauge_needle_dirty_mark(GX_CIRCULAR_GAUGE *gauge)
{
GX_RECTANGLE            dirty_rect;

    /* Get last needle rectangle. */
    dirty_rect = gauge -> gx_circular_gauge_current_needle_rectangle;

    /*Caculate the new needle rectangle. */
    _gx_circular_gauge_needle_rectangle_calculate(gauge, gauge->gx_circular_gauge_current_angle, &(gauge->gx_circular_gauge_current_needle_rectangle));

    /* Combine last and current needle rectangle.  */
    _gx_utility_rectangle_combine(&dirty_rect, &(gauge->gx_circular_gauge_current_needle_rectangle));

    /* Mark the combine area as dirty.  */
    _gx_system_dirty_partial_add((GX_WIDGET *)gauge, &dirty_rect);

    return(GX_SUCCESS);
}

