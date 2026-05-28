/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation
 * Copyright (c) 2026 Eclipse ThreadX contributors
 *
 * This program and the accompanying materials are made available under the
 * terms of the MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 **************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** GUIX Component                                                        */
/**                                                                       */
/**   Widget Management (Widget)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_window.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_last_client_child_get                           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function get the last client child.                            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    parent                                Pointer to parent widget      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    GX_WIDGET                             Last client child             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_horizontal_list_left_wrap                                       */
/*    _gx_horizontal_list_right_wrap                                      */
/*    _gx_horizontal_list_slide_back_check                                */
/*    _gx_vertical_list_down_wrap                                         */
/*    _gx_vertical_list_slide_back_check                                  */
/*    _gx_vertical_list_up_wrap                                           */
/*                                                                        */
/**************************************************************************/
GX_WIDGET *_gx_widget_last_client_child_get(GX_WIDGET *parent)
{
GX_WIDGET *test = parent -> gx_widget_last_child;

    while (test && (test -> gx_widget_status & GX_STATUS_NONCLIENT))
    {
        test = test -> gx_widget_previous;
    }
    return test;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_widget_last_visible_client_child_get            PORTABLE C      */
/*                                                           6.1.7        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function get the last client child that is visible.            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    parent                                Pointer to parent widget      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    GX_WIDGET                             Last visible client child     */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*   GUIX Internal Code                                                   */
/*                                                                        */
/**************************************************************************/
GX_WIDGET *_gx_widget_last_visible_client_child_get(GX_WIDGET *parent)
{
GX_WIDGET *test = parent -> gx_widget_last_child;

    while (test)
    {
        if (!(test -> gx_widget_status & GX_STATUS_NONCLIENT) &&
            (test -> gx_widget_status & GX_STATUS_VISIBLE))
        {
        	
        	/* Find the last child that is client and visible. */
            return test;
        }

        test = test -> gx_widget_previous;
    }
    return GX_NULL;
}

