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
/**   Accordion Menu Management (Menu)                                    */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_menu.h"
#include "gx_widget.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_accordion_menu_create                           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates an accordion menu.                            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    menu                                  Pointer to the accordion menu */
/*                                            control block               */
/*    name                                  Name of the menu              */
/*    parent                                Parent widget control block   */
/*    style                                 Style of the widget           */
/*    accordion_menu_id                     Application-defined ID of     */
/*                                          the accordion menu            */
/*    size                                  Accordion menu size           */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_create                     Create a widget               */
/*    _gx_widget_link                       Link a widget to its parent   */
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
UINT  _gx_accordion_menu_create(GX_ACCORDION_MENU *accordion, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                                ULONG style, USHORT accordion_menu_id, GX_CONST GX_RECTANGLE *size)
{

    /* Call the widget create function.  */
    _gx_widget_create((GX_WIDGET *)accordion, name, GX_NULL, style, accordion_menu_id, size);

    /* Populate the rest of the accordion menu control block - overriding as necessary.  */
    accordion -> gx_widget_type = GX_TYPE_ACCORDION_MENU;
    accordion -> gx_widget_event_process_function = (UINT (*)(GX_WIDGET *, GX_EVENT *))_gx_accordion_menu_event_process;
    accordion -> gx_widget_draw_function = (VOID (*)(GX_WIDGET *))_gx_accordion_menu_draw;
    accordion -> gx_accordion_menu_expand_item = GX_NULL;
    accordion -> gx_accordion_menu_collapse_item = GX_NULL;
    accordion -> gx_accordion_menu_animation_status = 0;

    /* Determine if a parent widget was provided.  */
    if (parent)
    {
        _gx_widget_link(parent, (GX_WIDGET *)accordion);
    }

    /* Return completion status code. */
    return(GX_SUCCESS);
}

