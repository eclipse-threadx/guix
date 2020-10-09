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
#include "gx_circular_gauge.h"

GX_CALLER_CHECKING_EXTERNS

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_circular_gauge_create                          PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks errors in circular gauge create call.          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    circular_gauge                        Circular gauge control block  */
/*    name                                  Name of the widget            */
/*    parent                                Parent widget control block   */
/*    circular_gauge_info                   Pointer to circular gauge info*/
/*    style                                 Style of circular gauge       */
/*    circular_gauge_id                     Application-defined ID of     */
/*                                            circular gauge              */
/*    size                                  Dimensions of circular gauge  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_circular_gauge_create             Actual circular gauge create. */
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
UINT  _gxe_circular_gauge_create(GX_CIRCULAR_GAUGE *circular_gauge,
                                 GX_CONST GX_CHAR *name,
                                 GX_WIDGET *parent,
                                 GX_CIRCULAR_GAUGE_INFO *circular_gauge_info,
                                 GX_RESOURCE_ID background,
                                 ULONG style,
                                 USHORT circular_gauge_id,
                                 GX_VALUE xpos, GX_VALUE ypos, UINT checkbox_control_block_size)
{
UINT status;

    /* Check for appropriate caller.  */
    GX_INIT_AND_THREADS_CALLER_CHECKING

    /* Check for invalid input pointers.  */
    if ((circular_gauge == GX_NULL) || (circular_gauge_info == GX_NULL))
    {
        return(GX_PTR_ERROR);
    }

    if (checkbox_control_block_size != sizeof(GX_CIRCULAR_GAUGE))
    {
        return(GX_INVALID_SIZE);
    }

    /* Check for widget already created.  */
    if (circular_gauge -> gx_widget_type != 0)
    {
        return(GX_ALREADY_CREATED);
    }

    /* Call the actual gauge create function.  */
    status = _gx_circular_gauge_create(circular_gauge, name, parent,
                                       circular_gauge_info, background,
                                       style, circular_gauge_id, xpos, ypos);

    return status;
}

