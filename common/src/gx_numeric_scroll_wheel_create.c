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
/**   Numeric Scroll Wheel Management (Scroll Wheel)                      */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_scroll_wheel.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_numeric_scroll_wheel_create                     PORTABLE C      */
/*                                                           6.1.6        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates a numeric scroll wheel selector widget.       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    wheel                                 Scroll wheel control block    */
/*    name                                  Name of widget                */
/*    parent                                Parent widget control block   */
/*    start_val                             Start value of numeric range  */
/*    end_val                               End value of numeric range    */
/*    style                                 Style of widget               */
/*    Id                                    Application-defined ID of the */
/*                                            the widget                  */
/*    size                                  Widget size                   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_text_scroll_wheel_create          Create a text scroll wheel    */
/*    _gx_widget_link                       Link a widget to parent       */
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
/*  04-02-2021     Kenneth Maxwell          Modified comment(s),          */
/*                                            replace usage of abs(),     */
/*                                            resulting in version 6.1.6  */
/*                                                                        */
/**************************************************************************/
UINT  _gx_numeric_scroll_wheel_create(GX_NUMERIC_SCROLL_WHEEL *wheel,
                                      GX_CONST GX_CHAR *name,
                                      GX_WIDGET *parent, INT start_val, INT end_val,
                                      ULONG style, USHORT Id, GX_CONST GX_RECTANGLE *size)
{

    /* Call text scroll wheel create. */
    _gx_text_scroll_wheel_create((GX_TEXT_SCROLL_WHEEL *)wheel, name, GX_NULL, GX_ABS(start_val - end_val) + 1, style, Id, size);

    wheel -> gx_widget_type = GX_TYPE_NUMERIC_SCROLL_WHEEL;
    wheel -> gx_numeric_scroll_wheel_start_val = start_val;
    wheel -> gx_numeric_scroll_wheel_end_val = end_val;

    wheel -> gx_text_scroll_wheel_text_get = (UINT (*)(GX_TEXT_SCROLL_WHEEL *, INT, GX_STRING *))_gx_numeric_scroll_wheel_text_get;

    /* Determine if a parent widget was provided.  */
    if (parent)
    {
        _gx_widget_link(parent, (GX_WIDGET *)wheel);
    }

    /* Return completion status.  */
    return(GX_SUCCESS);
}

