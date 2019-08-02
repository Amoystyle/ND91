#pragma once

// COmpositeTabUI管理的Tab项
class COMMONUI_API CTabItem : public CCompositeUI
{
public:
    CTabItem();
    ~CTabItem();

    // 刷新数据
    virtual void OnRefreshData() {};
    // 界面隐藏时触发
    virtual void OnHideComponent() {};
    // 界面显示时触发
    virtual void OnShowComponent() {};
    // 真正创建界面
    virtual void CreateUI(bool bVisible = true);
    virtual void CreateUI(CContainerUI* pParentCtn, bool bVisible = true);
    virtual void CreateUI(tstring id, CContainerUI* pParentCtn, bool bVisible = true);
    // 设置显示与否
    virtual void SetItemVisible(bool bShow);

    CContainerUI* GetParentCtn() { return _pParentCtn; };
    tstring GetContainerID() const { return _containerID; }
    tstring GetID() const { return _id; }
    int GetGroupID() const { return _groupID; }

    void SetParentCtn(CContainerUI* p) {_pParentCtn = p;};
    void SetContainerID(tstring str) { _containerID = str;};
    void SetID(tstring str) { _id = str; };
    void SetGroupID(int groupID) { _groupID = groupID;};
    void SetXmlID(tstring str) { _xmlid = str; };
private:
    CContainerUI* _pParentCtn; //所属容器
    tstring _containerID;   // container id 
    tstring _id;            // TabItem 的 id 
    tstring _xmlid;         // TabItem引用的xmlid
    int     _groupID;       // Tab组别，用这个来支持多个Tab组

};