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
#include "gx_window.h"
#include "gx_system.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_accordion_menu_shift_helper                     PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to shift one accordion menu.               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    accordion                             Pointer to accordion menu     */
/*                                            control block               */
/*    animation_target                      Animation target              */
/*    shift                                 Shift value                   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_scroll_shift               Change the widget position    */
/*    _gx_widget_resize                     Resize the widget             */
/*    _gx_menu_list_shift                   Change the menu list position */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_accordion_menu_shift_helper                                     */
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
static VOID _gx_accordion_menu_shift_helper(GX_ACCORDION_MENU *accordion, GX_WIDGET *animation_target, INT shift)
{
GX_WIDGET   *child;
GX_MENU     *menu;
GX_BOOL      shift_start = GX_FALSE;
GX_RECTANGLE size;

    child = accordion -> gx_widget_first_child;
    while (child)
    {
        switch (child -> gx_widget_type)
        {
        case GX_TYPE_MENU:
            if (shift_start)
            {
                /* Shift the menu item. */
                _gx_widget_scroll_shift(child, 0, shift, GX_TRUE);

                menu = (GX_MENU *)child;

                /* Shift the menu list which is a member of menu item. */
                _gx_widget_scroll_shift((GX_WIDGET *)&menu -> gx_menu_list, 0, shift, GX_TRUE);
            }
            break;

        case GX_TYPE_MENU_LIST:
            /* The shift work has done by the menu list owner. */
            break;

        default:
            if (shift_start)
            {
                /* Shift other types of menu item. */
                _gx_widget_scroll_shift(child, 0, shift, GX_TRUE);
            }
            break;
        }

        if (child == animation_target)
        {
            shift_start = GX_TRUE;
        }

        child = child -> gx_widget_next;
    }

    /* Resize accordion menu. */
    size = accordion -> gx_widget_size;
    size.gx_rectangle_bottom = (GX_VALUE)(size.gx_rectangle_bottom + shift);
    _gx_widget_resize((GX_WIDGET *)accordion, &size);
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_accordion_menu_shift                            PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to shift menu items for an accordion menu. */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    accordion                             Pointer to accordion menu     */
/*                                            control block               */
/*    animation_target                      Animation target              */
/*    shift                                 Shift value                   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_resize                     Resize the widget             */
/*    _gx_accordion_menu_shift_heloer       Change the accordion menu     */
/*                                            position                    */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_accordion_menu_close_animation_update                           */
/*    _gx_accordion_menu_open_animation_update                            */
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
static UINT _gx_accordion_menu_shift(GX_ACCORDION_MENU *accordion, GX_WIDGET *animation_target, INT shift)
{
GX_WIDGET   *temp_target;
GX_WIDGET   *parent;
GX_RECTANGLE size;
GX_WIDGET   *child;

    _gx_accordion_menu_shift_helper(accordion, animation_target, shift);

    temp_target = (GX_WIDGET *)accordion;
    parent = accordion -> gx_widget_parent;

    while (parent)
    {

        if (parent -> gx_widget_type == GX_TYPE_MENU_LIST)
        {
            /* If the accordion menu is the child of a menu list,
               Resize the menu list.*/
            size = parent -> gx_widget_size;
            size.gx_rectangle_bottom = (GX_VALUE)(size.gx_rectangle_bottom + shift);
            _gx_widget_resize(parent, &size);

            /* Get the menu that owns the menu list. */
            child = ((GX_MENU_LIST *)parent) -> gx_menu_list_owner;

            /* Get the parent of the menu*/
            if (child)
            {
                parent = child -> gx_widget_parent;

                /* Check if the menu parent is an accordion menu. */
                if (parent && parent -> gx_widget_type == GX_TYPE_ACCORDION_MENU)
                {
                    _gx_accordion_menu_shift_helper((GX_ACCORDION_MENU *)parent, child, shift);
                }
                else
                {
                    break; /* Break out of the "while(parent)" loop and return. */
                }
            }
            else
            {
                break; /* Break out of the "while(parent)" loop and return. */
            }
        }
        else if (parent -> gx_widget_type == GX_TYPE_ACCORDION_MENU)
        {
            _gx_accordion_menu_shift_helper((GX_ACCORDION_MENU *)parent, temp_target, shift);
        }
        else
        {
            break;
        }

        temp_target = parent;
        parent = parent -> gx_widget_parent;
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_accordion_menu_close_animation_update           PORTABLE C      */
/*                                                           6.3.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to execute one step in menu close          */
/*    animation.                                                          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    accordion                             Pointer to accordin menu      */
/*                                            control block               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_height_get                 Get widget height             */
/*    _gx_system_timer_stop                 Stop a timer for widget       */
/*    _gx_widget_detach                     Detach widget from its parent */
/*    _gx_widget_resize                     Resize a widget               */
/*    _gx_accordion_menu_shift              Change accordion menu position*/
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_accordion_menu_event_process                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  10-31-2023     Ting Zhu                 Modified comment(s),          */
/*                                            added NULL pointer check,   */
/*                                            resulting in version 6.3.0  */
/*                                                                        */
/**************************************************************************/
static UINT _gx_accordion_menu_close_animation_update(GX_ACCORDION_MENU *accordion)
{
GX_MENU     *deselected = (GX_MENU *)accordion -> gx_accordion_menu_collapse_item;
GX_VALUE     list_children_height = 0;
GX_VALUE     height;
GX_VALUE     shift;
GX_RECTANGLE size;
GX_WIDGET   *child;

    if (!deselected)
    {
        return GX_SUCCESS;
    }

    child = deselected -> gx_menu_list.gx_widget_first_child;

    /* Calcualte total height of list children. */
    while (child)
    {
        _gx_widget_height_get(child, &height);
        list_children_height = (GX_VALUE)(list_children_height + height);

        child = child -> gx_widget_next;
    }

    /* Calcualte shift value for each animation step. */
    shift = (GX_VALUE)(list_children_height / 10);
    if (shift == 0)
    {
        shift = list_children_height;
    }

    /* Get menu list height. */
    _gx_widget_height_get((GX_WIDGET *)&deselected -> gx_menu_list, &height);

    if (shift > height)
    {
        shift = height;
        _gx_system_timer_stop((GX_WIDGET *)accordion, GX_MENU_CLOSE_TIMER);
        deselected -> gx_widget_style &= (ULONG)(~GX_STYLE_MENU_EXPANDED);
        accordion -> gx_accordion_menu_animation_status &= (GX_UBYTE)(~GX_MENU_ANIMATION_CLOSE);
        accordion -> gx_accordion_menu_collapse_item = GX_NULL;
        _gx_widget_detach((GX_WIDGET *)&deselected -> gx_menu_list);
    }

    /* Resize menu list. */
    size = deselected -> gx_menu_list.gx_widget_size;
    size.gx_rectangle_bottom = (GX_VALUE)(size.gx_rectangle_bottom - shift);
    _gx_widget_resize((GX_WIDGET *)&deselected -> gx_menu_list, &size);

    /* Shift menus that follow the opening menu. */
    _gx_accordion_menu_shift(accordion, (GX_WIDGET *)deselected, -shift);

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_accordion_menu_open_animation_update            PORTABLE C      */
/*                                                           6.3.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to execute one step in menu open animation.*/
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    accordion                             Pointer to accordin menu      */
/*                                            control block               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_height_get                 Get widget height             */
/*    _gx_system_timer_stop                 Stop a timer for widget       */
/*    _gx_widget_resize                     Resize widget                 */
/*    _gx_accordion_menu_shift              Change accordin menu position */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_accordion_menu_event_process                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  10-31-2023     Ting Zhu                 Modified comment(s),          */
/*                                            added NULL pointer check,   */
/*                                            resulting in version 6.3.0  */
/*                                                                        */
/**************************************************************************/
static UINT _gx_accordion_menu_open_animation_update(GX_ACCORDION_MENU *accordion)
{
GX_MENU     *selected = (GX_MENU *)accordion -> gx_accordion_menu_expand_item;
GX_VALUE     shift;
GX_RECTANGLE size;
GX_VALUE     list_children_height = 0;
GX_VALUE     height = 0;
GX_WIDGET   *child;

    if (!selected)
    {
        return GX_SUCCESS;
    }

    child = selected -> gx_menu_list.gx_widget_first_child;

    /* Calcualte total height of list children. */
    while (child)
    {
        _gx_widget_height_get(child, &height);
        list_children_height = (GX_VALUE)(list_children_height + height);

        child = child -> gx_widget_next;
    }

    /* Calcualte shift value for each animation step. */
    shift = (GX_VALUE)(list_children_height / 10);
    if (shift == 0)
    {
        shift = list_children_height;
    }

    /* Get menu list height. */
    _gx_widget_height_get((GX_WIDGET *)&selected -> gx_menu_list, &height);

    if (height + shift >= list_children_height)
    {
        shift = (GX_VALUE)(list_children_height - height);
        _gx_system_timer_stop((GX_WIDGET *)accordion, GX_MENU_OPEN_TIMER);
        accordion -> gx_accordion_menu_animation_status &= (GX_UBYTE)(~GX_MENU_ANIMATION_OPEN);
        selected -> gx_widget_style |= GX_STYLE_MENU_EXPANDED;
    }

    /* Resize menu list. */
    size = selected -> gx_menu_list.gx_widget_size;
    size.gx_rectangle_bottom = (GX_VALUE)(size.gx_rectangle_bottom + shift);
    _gx_widget_resize((GX_WIDGET *)&selected -> gx_menu_list, &size);

    /* Shift menus that follow the opening menu. */
    _gx_accordion_menu_shift(accordion, (GX_WIDGET *)selected, shift);

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_accordion_menu_pen_down_event_process           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to process pen down event for an accordion */
/*    menu.                                                               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    accordion                             Pointer to accordion menu     */
/*                                            control block               */
/*    event_ptr                             Pointer to event to process   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_top_widget_find            Find toppest widget that      */
/*                                            contain test point          */
/*    _gx_system_input_capture              Temporarily direct all inputs */
/*                                            to specified widget         */
/*    _gx_widget_event_process              Default widget event procses  */
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
static UINT _gx_accordion_menu_pen_down_event_process(GX_ACCORDION_MENU *accordion, GX_EVENT *event_ptr)
{
GX_WIDGET *widget = (GX_WIDGET *)accordion;
GX_WIDGET *child = GX_NULL;

    child = _gx_system_top_widget_find(widget, event_ptr -> gx_event_payload.gx_event_pointdata, GX_STATUS_SELECTABLE);

    if (child && (child -> gx_widget_type == GX_TYPE_MENU) &&
        ((GX_MENU *)child) -> gx_menu_list.gx_widget_first_child)
    {
        if ((accordion -> gx_accordion_menu_animation_status & GX_MENU_ANIMATION_CLOSE) ||
            (accordion -> gx_accordion_menu_animation_status & GX_MENU_ANIMATION_OPEN))
        {
            /* Animation is going on. */
            return GX_SUCCESS;
        }

        /* Set expand and collapse menu items. */
        if (child -> gx_widget_style & GX_STYLE_MENU_EXPANDED)
        {
            accordion -> gx_accordion_menu_collapse_item = child;
            accordion -> gx_accordion_menu_expand_item = GX_NULL;
        }
        else
        {
            if (accordion -> gx_accordion_menu_expand_item != child)
            {
                accordion -> gx_accordion_menu_collapse_item = accordion -> gx_accordion_menu_expand_item;
            }
            accordion -> gx_accordion_menu_expand_item = child;
        }

        _gx_system_input_capture(widget);
    }
    else
    {
        _gx_widget_event_process(widget, event_ptr);
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_accordion_menu_pen_up_event_process             PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to process pen up event for an accordion   */
/*    menu.                                                               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    accordion                             Pointer to accordion menu     */
/*                                            control block               */
/*    event_ptr                             Pointer to event to process   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_input_release              Release event capture         */
/*    _gx_system_timer_start                Start a timer for widget      */
/*    _gx_widget_link                       Link a widget to its parent   */
/*    _gx_widget_shift                      Change a widget's position    */
/*    _gx_widget_event_process              Default widget event process  */
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
static UINT _gx_accordion_menu_pen_up_event_process(GX_ACCORDION_MENU *accordion, GX_EVENT *event_ptr)
{
GX_WIDGET    *widget = (GX_WIDGET *)accordion;
GX_WIDGET    *child = GX_NULL;
GX_MENU_LIST *menu_list;
GX_VALUE      x_shift;
GX_VALUE      y_shift;

    if (accordion -> gx_widget_status & GX_STATUS_OWNS_INPUT)
    {
        _gx_system_input_release(widget);

        if (accordion -> gx_accordion_menu_collapse_item)
        {
            /* Start a timer to collapse a menu. */
            _gx_system_timer_start((GX_WIDGET *)accordion, GX_MENU_CLOSE_TIMER, 1, 1);
            accordion -> gx_accordion_menu_animation_status |= GX_MENU_ANIMATION_CLOSE;
        }

        if (accordion -> gx_accordion_menu_expand_item)
        {

            /* Start a timer to expand a menu. */
            _gx_system_timer_start((GX_WIDGET *)accordion, GX_MENU_OPEN_TIMER, 1, 1);
            accordion -> gx_accordion_menu_animation_status |= GX_MENU_ANIMATION_OPEN;

            child = accordion -> gx_accordion_menu_expand_item;
            menu_list = &((GX_MENU *)child) -> gx_menu_list;

            /* Link menu list to menu's parent. */
            _gx_widget_link(child -> gx_widget_parent, (GX_WIDGET *)menu_list);

            x_shift = (GX_VALUE)(child -> gx_widget_size.gx_rectangle_left - menu_list -> gx_widget_size.gx_rectangle_left);
            y_shift = (GX_VALUE)(child -> gx_widget_size.gx_rectangle_bottom + 1 - menu_list -> gx_widget_size.gx_rectangle_top);

            /* Shift menu list to the bottom of the menu. */
            if (x_shift || y_shift)
            {
                _gx_widget_shift((GX_WIDGET *)menu_list, x_shift, y_shift, GX_FALSE);
            }
        }
    }
    else
    {
        _gx_widget_event_process(widget, event_ptr);
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_accordion_menu_event_process                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service processes an event for the accordion menu.             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    accordion                             Pointer to accordion menu     */
/*                                            control block               */
/*    event_ptr                             Pointer to event to process   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_event_process              Default widget event process  */
/*    _gx_accordion_menu_position           Position an accordion menu    */
/*    _gx_accordion_menu_open_animation_update                            */
/*                                          Execute one step menu open    */
/*                                            animation                   */
/*    _gx_accordion_menu_close_animation_update                           */
/*                                          Execute one step menu close   */
/*                                            animation                   */
/*    _gx_accordion_menu_pen_down_event_process                           */
/*                                          Handle pen down event         */
/*    _gx_accordion_menu_pen_up_event_process                             */
/*                                          Handle pen up event           */
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
UINT _gx_accordion_menu_event_process(GX_ACCORDION_MENU *accordion, GX_EVENT *event_ptr)
{
UINT timer_id;

    switch (event_ptr -> gx_event_type)
    {
    case GX_EVENT_SHOW:
        _gx_widget_event_process((GX_WIDGET *)accordion, event_ptr);

        /* Check if the accordion parent is a menu list, if so, this is not a top level accordion,
           Do not call accordion menu position here. We should only call menu position for a top level accordion menu. */
        if (!(accordion -> gx_widget_parent && accordion -> gx_widget_parent -> gx_widget_type == GX_TYPE_MENU_LIST))
        {
            _gx_accordion_menu_position(accordion);
        }
        break;

    case GX_EVENT_PEN_DOWN:
        _gx_accordion_menu_pen_down_event_process(accordion, event_ptr);
        break;

    case GX_EVENT_PEN_UP:
        _gx_accordion_menu_pen_up_event_process(accordion, event_ptr);
        break;

    case GX_EVENT_TIMER:
        timer_id = event_ptr -> gx_event_payload.gx_event_timer_id;
        if (timer_id == GX_MENU_CLOSE_TIMER)
        {
            _gx_accordion_menu_close_animation_update(accordion);
        }
        else if (timer_id == GX_MENU_OPEN_TIMER)
        {
            _gx_accordion_menu_open_animation_update(accordion);
        }
        break;

    default:
        return _gx_widget_event_process((GX_WIDGET *)accordion, event_ptr);
    }

    return(GX_SUCCESS);
}

