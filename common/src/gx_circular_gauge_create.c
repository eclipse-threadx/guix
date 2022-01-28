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
#include "gx_icon.h"
#include "gx_window.h"
#include "gx_system.h"
#include "gx_circular_gauge.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_circular_gauge_create                           PORTABLE C      */
/*                                                           6.1.10       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This functon creates a circular gauge with the specified properties.*/
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    gauge                                 Pointer to circular gauge     */
/*                                            control block               */
/*    name                                  Logical name of circular gauge*/
/*                                            widget                      */
/*    parent                                Pointer to the parent widget  */
/*    info                                  Pointer to                    */
/*                                            GX_CIRCULAR_GAUGE_INFO      */
/*                                            structure                   */
/*    style                                 Style of circular gauge.      */
/*    circular_gauge_id                     Application-defined ID of     */
/*                                            circular gauge              */
/*    xpos                                  Gauge x-coordinate position   */
/*    ypos                                  Gauge y-coordinate position   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_create                     Creates a widget.             */
/*    _gx_widget_link                       Links a child widget to its   */
/*                                            parent.                     */
/*    _gx_circular_gauge_angle_increment_calculate                        */
/*                                          Calculate increment angle for */
/*                                            each step.                  */
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
/*  01-31-2022     Ting Zhu                 Modified comment(s),          */
/*                                            added resize notify status, */
/*                                            resulting in version 6.1.10 */
/*                                                                        */
/**************************************************************************/
UINT  _gx_circular_gauge_create(GX_CIRCULAR_GAUGE *gauge,
                                GX_CONST GX_CHAR *name,
                                GX_WIDGET *parent,
                                GX_CIRCULAR_GAUGE_INFO *info,
                                GX_RESOURCE_ID background_id,
                                ULONG style,
                                USHORT circular_gauge_id,
                                GX_VALUE xpos, GX_VALUE ypos)
{
    /* Call the widget create function.  */
    _gx_icon_create((GX_ICON *)gauge, name, GX_NULL, background_id, style, circular_gauge_id, xpos, ypos);

    /* Populate the rest of gauge control block - overriding as necessary.  */
    gauge -> gx_widget_type = GX_TYPE_CIRCULAR_GAUGE;

    gauge -> gx_circular_gauge_info = *info;

    if (gauge -> gx_circular_gauge_info.gx_circular_gauge_info_animation_delay <= 0)
    {
        gauge -> gx_circular_gauge_info.gx_circular_gauge_info_animation_delay = GX_DEFAULT_CIRCULAR_GAUGE_ANIMATION_DELAY;
    }

    gauge -> gx_widget_draw_function = (VOID (*)(GX_WIDGET *))_gx_circular_gauge_draw;
    gauge -> gx_widget_event_process_function = (UINT (*)(GX_WIDGET *, GX_EVENT *))_gx_circular_gauge_event_process;

    /* Initiate rotate needle pixelmap.  */
    memset(&gauge -> gx_circular_gauge_needle_rotated, 0, sizeof(GX_PIXELMAP));
    gauge -> gx_circular_gauge_needle_source = GX_NULL;
    gauge -> gx_circular_gauge_start_angle = 0;
    gauge -> gx_circular_gauge_current_angle = 0;
    gauge -> gx_circular_gauge_target_angle = 0;
    gauge -> gx_widget_status |= GX_STATUS_RESIZE_NOTIFY;

    /* Determine if a parent widget was provided.  */
    if (parent)
    {
        _gx_widget_link(parent, (GX_WIDGET *)gauge);
    }

    /* Return completion status. */
    return(GX_SUCCESS);
}

