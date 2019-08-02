#pragma once

// COmpositeTabUI�����Tab��
class COMMONUI_API CTabItem : public CCompositeUI
{
public:
    CTabItem();
    ~CTabItem();

    // ˢ������
    virtual void OnRefreshData() {};
    // ��������ʱ����
    virtual void OnHideComponent() {};
    // ������ʾʱ����
    virtual void OnShowComponent() {};
    // ������������
    virtual void CreateUI(bool bVisible = true);
    virtual void CreateUI(CContainerUI* pParentCtn, bool bVisible = true);
    virtual void CreateUI(tstring id, CContainerUI* pParentCtn, bool bVisible = true);
    // ������ʾ���
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
    CContainerUI* _pParentCtn; //��������
    tstring _containerID;   // container id 
    tstring _id;            // TabItem �� id 
    tstring _xmlid;         // TabItem���õ�xmlid
    int     _groupID;       // Tab����������֧�ֶ��Tab��

};