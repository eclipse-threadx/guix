#include "studiox_includes.h"

///////////////////////////////////////////////////////////////////////////////
action_info::action_info()
{
    action_type = 0;
    action_name = "";
    target_widget_name = "";
    parent_widget_name = "";
    animation_id_name = "";
    target_show_child_widgets = FALSE;
    parent_show_child_widgets = FALSE;
    animation = NULL;
}

///////////////////////////////////////////////////////////////////////////////
action_info::action_info(const action_info &other)
{
    copy(other);
}

///////////////////////////////////////////////////////////////////////////////
action_info::~action_info()
{
    if (animation)
    {
        delete animation;
    }
}

///////////////////////////////////////////////////////////////////////////////
action_info &action_info::operator=(const action_info &other)
{
    copy(other);
    return *this;
}

///////////////////////////////////////////////////////////////////////////////
void action_info::copy(const action_info &other)
{
    action_type = other.action_type;
    action_name = other.action_name;
    target_widget_name = other.target_widget_name;
    parent_widget_name = other.parent_widget_name;
    animation_id_name = other.animation_id_name;
    target_show_child_widgets = other.target_show_child_widgets;
    parent_show_child_widgets = other.parent_show_child_widgets;

    if (other.animation)
    {
        animation = new GX_ANIMATION_INFO;
        (*animation) = (*other.animation);
    }
    else
    {
        animation = NULL;
    }
}

///////////////////////////////////////////////////////////////////////////////
void action_info::RemoveAnimationIdFromDictionary(int display_index)
{
    if (!animation_id_name.IsEmpty())
    {
        studiox_project *project = GetOpenProject();
        if (project)
        {
            project->RemoveFromIdDictionary(display_index, ID_TYPE_ANIMATION, animation_id_name);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void action_info::AddAnimationIdToDictionary(int display_index)
{
    if (!animation_id_name.IsEmpty())
    {
        studiox_project *project = GetOpenProject();
        if (project)
        {
            project->AddToIdDictionary(display_index, ID_TYPE_ANIMATION, animation_id_name);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
trigger_info::trigger_info()
{
    trigger_name = "";
    system_event_animat_id_name = "";
    signal_id_name = "";
    user_event_id_name = "";
    trigger_type = 0;
    event_type = 0;
    action_list.RemoveAll();
    next = NULL;
}

///////////////////////////////////////////////////////////////////////////////
trigger_info::trigger_info(const trigger_info &other)
{
    copy(other);
}

///////////////////////////////////////////////////////////////////////////////
trigger_info::~trigger_info()
{
    trigger_info *test;
    trigger_info *temp;

    CleanActionList(action_list);

    test = next;
    while (test)
    {
        temp = test->next;
        test->next = NULL;
        delete test;
        test = temp;
    }
}

///////////////////////////////////////////////////////////////////////////////
trigger_info &trigger_info::operator=(const trigger_info &other)
{
    copy(other);
    return *this;
}

///////////////////////////////////////////////////////////////////////////////
void trigger_info::MergeActionInfo(trigger_info *add_trigger)
{
    if (!add_trigger)
    {
        return;
    }

    CArray<action_info *> *add_action_list = &add_trigger->action_list;
    action_info *add_action;

    for (int index = 0; index < add_action_list->GetCount(); index++)
    {
        add_action = add_action_list->GetAt(index);

        if (FindActionName(action_list, add_action->action_name))
        {
            CreateUniqueActionName(action_list, add_action);
        }

        action_list.Add(add_action);
    }

    add_action_list->RemoveAll();
}

///////////////////////////////////////////////////////////////////////////////
action_info *trigger_info::GetActionInfo(CArray<action_info *> &ActionList, CString &action_name)
{
    action_info *action;

    for (int index = 0; index < ActionList.GetCount(); index++)
    {
        action = ActionList.GetAt(index);

        if (action->action_name == action_name)
        {
            return action;
        }
    }

    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
BOOL trigger_info::FindActionName(CArray<action_info *> &ActionList, CString &name)
{
    action_info *action;
    for (int index = 0; index < ActionList.GetCount(); index++)
    {
        action = ActionList.GetAt(index);

        if (action->action_name == name)
        {
            return TRUE;
        }
    }

    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
void trigger_info::CreateUniqueActionName(CArray<action_info *> &ActionList, action_info *action)
{
    if (!FindActionName(ActionList, action->action_name))
    {
        return;
    }

    CString base = action->action_name;
    CString out;
    int index = 1;


    out.Format(_T("%s_%d"), base, index);
    while (FindActionName(ActionList, out))
    {
        index++;
        out.Format(_T("%s_%d"), base, index);
    }

    action->action_name = out;
}

///////////////////////////////////////////////////////////////////////////////
void trigger_info::CleanActionList(CArray<action_info *> &list)
{
    for (int index = 0; index < list.GetCount(); index++)
    {
        delete list.GetAt(index);
    }
    list.RemoveAll();
}

///////////////////////////////////////////////////////////////////////////////
void trigger_info::copy(const trigger_info &other)
{
    trigger_name = other.trigger_name;
    trigger_type = other.trigger_type;

    system_event_animat_id_name = other.system_event_animat_id_name;
    signal_id_name = other.signal_id_name;
    user_event_id_name = other.user_event_id_name;

    event_type = other.event_type;

    action_info *action;

    for (int index = 0; index < other.action_list.GetCount(); index++)
    {
        action = new action_info(*other.action_list.GetAt(index));

        action_list.Add(action);
    }

    next = NULL;
}

///////////////////////////////////////////////////////////////////////////////
void trigger_info::RemoveAnimationIdFromDictionary(int display_index)
{
    action_info *action;

    for (int index = 0; index < action_list.GetCount(); index++)
    {
        action = action_list.GetAt(index);
        action->RemoveAnimationIdFromDictionary(display_index);
    }
}

///////////////////////////////////////////////////////////////////////////////
void trigger_info::AddAnimationIdToDictionary(int display_index)
{
    action_info *action;

    for (int index = 0; index < action_list.GetCount(); index++)
    {
        action = action_list.GetAt(index);
        action->AddAnimationIdToDictionary(display_index);
    }
}

///////////////////////////////////////////////////////////////////////////////
flow_item::flow_item()
{
    screen_name = "";
    rect = CRect(0, 0, 0, 0);
    trigger_list = NULL;
    enabled = TRUE;
}

///////////////////////////////////////////////////////////////////////////////
flow_item::flow_item(const flow_item &other)
{
    copy(other);
}

///////////////////////////////////////////////////////////////////////////////
flow_item::~flow_item()
{
    if (trigger_list)
    {
        delete trigger_list;
    }
}

///////////////////////////////////////////////////////////////////////////////
flow_item &flow_item::operator=(const flow_item &other)
{
    copy(other);
    return *this;
}

///////////////////////////////////////////////////////////////////////////////
void flow_item::copy(const flow_item &other)
{
    screen_name = other.screen_name;
    rect = other.rect;
    enabled = other.enabled;
    trigger_list = NULL;
    trigger_info *trigger;
    trigger_info *pre = NULL;
    trigger_info *copy;

    copy = other.trigger_list;

    while (copy)
    {
        trigger = new trigger_info(*copy);

        if (pre)
        {
            pre->next = trigger;
        }
        else
        {
            trigger_list = trigger;
        }

        pre = trigger;

        copy = copy->next;
    }
}

///////////////////////////////////////////////////////////////////////////////
void flow_item::RemovedAnimationIdFromDictionary(int display_index)
{
    trigger_info *trigger = trigger_list;

    while (trigger)
    {
        trigger->RemoveAnimationIdFromDictionary(display_index);
        trigger = trigger->next;
    }
}

///////////////////////////////////////////////////////////////////////////////
void flow_item::AddAnimationIdToDictionary(int display_index)
{
    trigger_info *trigger = trigger_list;

    while (trigger)
    {
        trigger->AddAnimationIdToDictionary(display_index);
        trigger = trigger->next;
    }
}

///////////////////////////////////////////////////////////////////////////////
screen_flow::screen_flow()
{
    flow_list.RemoveAll();

    mDiagramScale = 100;
    
    int dpi = GetSystemDPI();
    mScale = MulDiv(mDiagramScale, dpi, DEFAULT_DPI_96);
}

///////////////////////////////////////////////////////////////////////////////
screen_flow::screen_flow(const screen_flow &other)
{
    flow_item *item;

    for (int index = 0; index < other.flow_list.GetCount(); index++)
    {
        item = new flow_item(*other.flow_list.GetAt(index));
        flow_list.Add(item);
    }

    mScale = other.mScale;
    mDiagramScale = other.mDiagramScale;
}

///////////////////////////////////////////////////////////////////////////////
screen_flow::~screen_flow()
{
    flow_item *item;

    for (int index = 0; index < flow_list.GetCount(); index++)
    {
        item = flow_list.GetAt(index);
        delete item;
    }

    flow_list.RemoveAll();
}

///////////////////////////////////////////////////////////////////////////////
void screen_flow::UpdateScreenName(CString screen_name, CString new_name)
{
    flow_item *item;
    trigger_info *trigger;
    CArray<action_info *> *action_list;

    for (int index = 0; index < GetFlowListCount(); index++)
    {
        item = GetFlowItem(index);

        if (item->screen_name == screen_name)
        {
            item->screen_name = new_name;
        }

        trigger = item->trigger_list;
        while (trigger)
        {
            action_list = &trigger->action_list;
            for (int count = 0; count < action_list->GetCount(); count++)
            {
                if (action_list->GetAt(count)->target_widget_name == screen_name)
                {
                    action_list->GetAt(count)->target_widget_name = new_name;
                }
            }

            trigger = trigger->next;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void screen_flow::UpdateIdName(widget_info *info, CString old_id_name, CString new_name)
{
    flow_item *item;
    trigger_info *trigger;
    widget_info *screen_info = NULL;

    studiox_project *project = GetOpenProject();

    screen_info = GetProjectView()->FindTopLevelWidget(info);

    if ((!screen_info) || !project || new_name.IsEmpty())
    {
        return;
    }

    item = GetFlowItem(screen_info->app_name);

    if (item)
    {
        trigger = item->trigger_list;
        CString trigger_name;
        int pos;

        while (trigger)
        {
            if (trigger->signal_id_name == old_id_name)
            {
                trigger->signal_id_name = new_name;

                //update trigger name
                pos = trigger->trigger_name.Find('(');
                trigger_name = trigger->trigger_name.Left(pos + 1);
                trigger_name.Append(new_name.MakeLower());
                pos = trigger->trigger_name.Find(',');
                trigger_name.Append(trigger->trigger_name.Mid(pos));

                trigger->trigger_name = trigger_name;
            }
            trigger = trigger->next;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void screen_flow::CheckAddFlowItem(CString screen_name, int diagram_width, int diagram_height)
{
    flow_item *item = GetFlowItem(screen_name);

    if (item)
    {
        // test invalid screen box size
        if (item->rect.right < item->rect.left + 10)
        {
            item->rect.right = item->rect.left + 10;
        }
        if (item->rect.bottom < item->rect.top + 10)
        {
            item->rect.bottom = item->rect.top + 10;
        }
    }
    else
    {
        flow_item *info = new flow_item;
        CRect rect;
        rect.SetRectEmpty();
        InitFlowItemRect(rect, diagram_width, diagram_height);

        info->screen_name = screen_name;
        info->trigger_list = NULL;
        info->rect.CopyRect(rect);
        flow_list.Add(info);
    }
}

///////////////////////////////////////////////////////////////////////////////
void screen_flow::DeleteFlowItem(CString &screen_name, int display_index)
{
    flow_item *item;
    trigger_info *trigger;
    trigger_info *pre;
    CArray<action_info *> *action_list;
    int count;

    //remove flow item
    int index = GetFlowItemIndex(screen_name);
    if (index >= 0 && index < GetFlowListCount())
    {
        item = GetFlowItem(index);
        item->RemovedAnimationIdFromDictionary(display_index);
        delete item;
        flow_list.RemoveAt(index);
    }

    for (index = 0; index < GetFlowListCount(); index++)
    {
        item = GetFlowItem(index);
        trigger = item->trigger_list;
        pre = NULL;

        while (trigger)
        {
            action_list = &trigger->action_list;

            //remove actions that target to the delete screen
            count = 0;
            while(count < action_list->GetCount())
            {
                if (action_list->GetAt(count)->target_widget_name == screen_name)
                {
                    delete action_list->GetAt(count);
                    action_list->RemoveAt(count);
                }
                else
                {
                    count++;
                }
            }

            //if no action exist, delete the trigger
            if (action_list->GetCount() == 0)
            {
                if (pre)
                {
                    pre->next = trigger->next;
                }
                else
                {
                    item->trigger_list = trigger->next;
                }
                trigger->next = NULL;
                delete trigger;

                if (pre)
                {
                    trigger = pre->next;
                }
                else
                {
                    trigger = item->trigger_list;
                }
            }
            else
            {
                pre = trigger;
                trigger = trigger->next;
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
flow_item *screen_flow::GetFlowItem(CPoint point, BOOL move_front)
{
    flow_item *info;

    point.x = point.x * 100 / mScale;
    point.y = point.y * 100 / mScale;

    for (int index = flow_list.GetCount() - 1; index >= 0; index--)
    {
        info = flow_list.GetAt(index);

        if (info->enabled && info->rect.PtInRect(point))
        {
            if (move_front)
            {
                flow_list.RemoveAt(index);
                flow_list.Add(info);
            }
            return info;
        }
    }

    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
flow_item *screen_flow::GetFlowItem(CString &screen_name)
{
    flow_item *info;

    for (int index = 0; index < flow_list.GetCount(); index++)
    {
        info = flow_list.GetAt(index);

        if (info->screen_name == screen_name)
        {
            return info;
        }
    }

    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
int screen_flow::GetFlowItemIndex(CString screen_name)
{
    flow_item *info;
    for (int index = 0; index < flow_list.GetCount(); index++)
    {
        info = flow_list.GetAt(index);

        if (info->screen_name == screen_name)
        {
            return index;
        }
    }

    return -1;
}

///////////////////////////////////////////////////////////////////////////////
void screen_flow::InitFlowItemRect(CRect &return_rect, int diagram_width, int diagram_heihgt)
{
    int interval_x;
    int interval_y;
    int width;
    int height;
    CRect new_rect;
    CRect rect;

    if (return_rect.IsRectEmpty())
    {
        interval_x = diagram_heihgt / 4;
        interval_y = diagram_heihgt / 4;
        width = (interval_x - 20) * mScale / 100;
        height = (interval_y - 20) * mScale / 100;

        new_rect = CRect(10, 40, 10 + width - 1, 10 + height - 1);
    }
    else
    {
        new_rect = return_rect;

        width = new_rect.Width();
        height = new_rect.Height();
        interval_x = width + 20;
        interval_y = height + 20;
    }

    flow_item *info;
    BOOL overlap = FALSE;

    while (1)
    {
        while (new_rect.left + width < diagram_width)
        {
            for (int index = 0; index < flow_list.GetCount(); index++)
            {
                info = flow_list.GetAt(index);
                rect = info->rect;
                if (info->enabled && rect.IntersectRect(&rect, &new_rect))
                {
                    overlap = TRUE;
                    break;
                }
            }

            if (!overlap)
            {
                return_rect = new_rect;
                return;
            }

            overlap = FALSE;
            new_rect.left += interval_x;
            new_rect.right += interval_x;
        }

        new_rect.left = 10;
        new_rect.right = new_rect.left + width;
        new_rect.top += interval_y;
        new_rect.bottom += interval_y;
    }

    return_rect = CRect(10, 40, 10 + width - 1, 10 + height - 1);
}

///////////////////////////////////////////////////////////////////////////////
void screen_flow::UpdateFlowItemRect(flow_item *item, int diagram_width, int diagram_height)
{
    flow_item *info;
    CRect rect;
    BOOL overlap = FALSE;

    for (int index = 0; index < flow_list.GetCount(); index++)
    {
        info = flow_list.GetAt(index);
        if (info != item)
        {
            rect = info->rect;
            if (info->enabled && rect.IntersectRect(&rect, &item->rect))
            {
                overlap = TRUE;
                break;
            }
        }
    }

    if (overlap)
    {
        rect = item->rect;
        InitFlowItemRect(rect, diagram_width, diagram_height);
        item->rect = rect;
    }
}

///////////////////////////////////////////////////////////////////////////////
trigger_info* screen_flow::FindTrigger(flow_item *item, CString id_name)
{
    if (item)
    {
        trigger_info *trigger = item->trigger_list;
        CString trigger_name;

        while (trigger)
        {
            if (trigger->signal_id_name == id_name)
            {
                return trigger;
            }
            trigger = trigger->next;
        }
    }
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
void screen_flow::RemoveTrigger(flow_item *item, trigger_info *del_trigger)
{
    trigger_info *trigger = item->trigger_list;
    trigger_info *pre = NULL;

    while (trigger)
    {
        if (trigger == del_trigger)
        {
            if (pre)
            {
                pre->next = trigger->next;
            }
            else
            {
                item->trigger_list = trigger->next;
            }

            trigger->next = NULL;
            delete trigger;
            break;
        }

        pre = trigger;
        trigger = trigger->next;
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL screen_flow::CheckAddTrigger(flow_item *item, trigger_info *add_trigger, CWnd *parent)
{
    trigger_info *trigger = item->trigger_list;
    trigger_info *pre = NULL;
    BOOL Add = TRUE;

    while (trigger)
    {
        if (trigger == add_trigger)
        {
            Add = FALSE;
        }
        else if(trigger->trigger_name == add_trigger->trigger_name)
        {
            if (!AskUser("The trigger already exist, do merging?", parent))
            {
                return FALSE;
            }
            else
            {
                trigger->MergeActionInfo(add_trigger);
                add_trigger->event_type = 0;
                return TRUE;
            }
        }
        pre = trigger;
        trigger = trigger->next;
    }

    if (Add)
    {
        if (pre)
        {
            pre->next = add_trigger;
        }
        else
        {
            item->trigger_list = add_trigger;
        }
    }

    return TRUE;
}

void screen_flow::SetScale(int scale)
{
    int dpi = GetSystemDPI();
    mDiagramScale = scale;
    mScale = MulDiv(scale, dpi, DEFAULT_DPI_96);
}

///////////////////////////////////////////////////////////////////////////////
void screen_flow::RebuildAnimationIdDictionary(int display_index)
{
    studiox_project *project = GetOpenProject();

    if (!project)
    {
        return;
    }

    project->CleanupIdDictionary(display_index, ID_TYPE_ANIMATION);

    flow_item *item;
    for (int index = 0; index < flow_list.GetCount(); index++)
    {
        item = flow_list.GetAt(index);
        item->AddAnimationIdToDictionary(display_index);
    }

}

///////////////////////////////////////////////////////////////////////////////
BOOL screen_flow::IsAnimationIdBeReferenced(CString id_name)
{
    trigger_info *trigger;
    flow_item *item;

    for (int index = 0; index < flow_list.GetCount(); index++)
    {
        item = flow_list.GetAt(index);

        trigger = item->trigger_list;
        while (trigger)
        {
            if ((trigger->event_type == GX_EVENT_ANIMATION_COMPLETE) &&
                (trigger->system_event_animat_id_name == id_name))
            {
                return TRUE;
            }
            trigger = trigger->next;
        }
    }

    return FALSE;
}