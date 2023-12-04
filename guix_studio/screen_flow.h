
#ifndef _SCREEN_FLOW_
#define _SCREEN_FLOW_


class action_info{
public:
    action_info();
    action_info(const action_info &other);
    ~action_info();
    action_info &operator=(const action_info &other);
    void RemoveAnimationIdFromDictionary(int display_index);
    void AddAnimationIdToDictionary(int display_index);

public:
    ULONG           action_type;
    CString         action_name;
    CString         parent_widget_name;
    CString         target_widget_name;
    CString         animation_id_name;
    BOOL            parent_show_child_widgets;
    BOOL            target_show_child_widgets;
    GX_ANIMATION_INFO *animation;

private:
    void copy(const action_info &other);
};

class trigger_info{
    public:
        trigger_info();
        trigger_info(const trigger_info &other);
        ~trigger_info();
        trigger_info &operator=(const trigger_info &other);
        void MergeActionInfo(trigger_info *add_trigger);
        static action_info *GetActionInfo(CArray<action_info *> &ActionList, CString &action_name);
        static BOOL FindActionName(CArray<action_info *> &ActionList, CString &name);
        static void CreateUniqueActionName(CArray<action_info *> &ActionList, action_info *action);
        static void CleanActionList(CArray<action_info *> &list);
        void RemoveAnimationIdFromDictionary(int display_index);
        void AddAnimationIdToDictionary(int display_index);

    public:
        CString               trigger_name;
        CString               system_event_animat_id_name;
        CString               signal_id_name;
        CString               user_event_id_name;
        INT                   trigger_type;
        ULONG                 event_type;
        CArray<action_info *> action_list;
        trigger_info         *next;

    private:
        void copy(const trigger_info &other);
};

class flow_item{
    public:
        flow_item();
        flow_item(const flow_item &other);
        ~flow_item();
        flow_item &operator=(const flow_item &other);
        void RemovedAnimationIdFromDictionary(int display_index);
        void AddAnimationIdToDictionary(int display_index);

    public:
        CString       screen_name;
        CRect         rect;//the size of the box that represent the screen in screen flow dialog
        trigger_info *trigger_list;
        BOOL          enabled;

    private:
        void copy(const flow_item &other);
};

class screen_flow
{
public:
    screen_flow();
    screen_flow(const screen_flow &other);// copy constructer
    ~screen_flow();

    void UpdateScreenName(CString screen_name, CString new_name);
    void UpdateIdName(widget_info *info, CString old_id_name, CString new_name);
    void CheckAddFlowItem(CString screen_name, int diagram_width, int diagram_height);
    void DeleteFlowItem(CString &screen_name, int display_index);
    void AddFlowItem(flow_item *item){ flow_list.Add(item); };
    void InitFlowItemRect(CRect &rect, int diagram_width, int diagram_height);
    void UpdateFlowItemRect(flow_item *item, int diagram_width, int diagram_height);

    trigger_info *FindTrigger(flow_item *item, CString id_name);
    static void RemoveTrigger(flow_item *item, trigger_info *del_trigger);

    static BOOL CheckAddTrigger(flow_item *item, trigger_info *add_trigger, CWnd *parent = NULL);
    INT GetFlowListCount(){ return flow_list.GetCount(); };
    flow_item *GetFlowItem(int index){ return flow_list.GetAt(index); };
    flow_item *GetFlowItem(CPoint point, BOOL move_front = FALSE);
    flow_item *GetFlowItem(CString &screen_name);
    int GetScale(){ return mDiagramScale; };
    void SetScale(int scale);
    void RebuildAnimationIdDictionary(int display_index);
    BOOL IsAnimationIdBeReferenced(CString id_name);

protected:
    int GetFlowItemIndex(CString screen_name);

private:
    CArray<flow_item *> flow_list;
    int mScale;
    int mDiagramScale;
};
#endif

