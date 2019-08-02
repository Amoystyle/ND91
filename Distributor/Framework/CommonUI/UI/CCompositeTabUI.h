#pragma once

// Tab�����࣬����CTabItem
class COMMONUI_API CCompositeTabUI
{
public:

	CCompositeTabUI();
	~CCompositeTabUI();
    
    // ע��Ҫ��������TabItem
    // containerID ��Ҫ�����������id
    // id : ��TabItem������id��Ҳ�����ɽ���ʱʹ�õ�xml id
    // pTabItem ��������TabItem�࣬���滹û����
    // nItemGroup : ��TabItem�ķ��飬����֧�ֶ���Tab����	
	virtual void RegisterComponent( CContainerUI* pParentCtn, LPCTSTR id, CTabItem* pTabItem, int nGroupID=0);
    virtual void RegisterComponent( CContainerUI* pParentCtn, LPCTSTR id, LPCTSTR xmlid, CTabItem* pTabItem, int nGroupID=0);

    // ����TabItem, ������TabItem���ᱻ�������
	//virtual bool CreateItemUI(CTabItem* pTabItem, bool bVisible=true);

	// ����OnCreate��Ҫ���û���OnCreate
	virtual void OnCreate();

    // ����ע���TabItem
	virtual void LoadRegisterComponent();

    // ȡ��TabItem
    CTabItem* GetComponent(tstring id);

    // ��������TabItem
    // nItemGroup��ʾTabItem���ڵķ��飬�л���ʱ�򰴷��������-1����������е�Component����ͬ;
    void HideAllComponent(int nItemGroup=0);

    // �Ƴ�TabItem
    void RemoveComponent(tstring id);    
    void RemoveComponent(CTabItem* pTabItem);

    // TabItem����ʾ�������¼�
    virtual void OnShowComponent() {};

    // ��ʾĳ��TabItem
    // id : ��TabItem������id��Ҳ�����ɽ���ʱʹ�õ�xml id
    CTabItem* ShowComponent(tstring id, int nItemGroup=0);

    // ����ĳ��TabItemΪ��ʾ�����أ���Ӱ������TabItem
    void ShowComponent(CTabItem* pTabItem, int nItemGroup=0, bool bShow=true);

    // ȡ�õ�ǰ��ʾ��TabItem
    CTabItem* GetCurrentComponent(int nItemGroup=0);

private:    
	vector<CTabItem*> _pTabItemList;
};