#pragma once

// Tab管理类，管理CTabItem
class COMMONUI_API CCompositeTabUI
{
public:

	CCompositeTabUI();
	~CCompositeTabUI();
    
    // 注册要列入管理的TabItem
    // containerID ：要放入的容器的id
    // id : 该TabItem的索引id，也是生成界面时使用的xml id
    // pTabItem ：创建的TabItem类，界面还没生成
    // nItemGroup : 该TabItem的分组，用来支持多组Tab管理	
	virtual void RegisterComponent( CContainerUI* pParentCtn, LPCTSTR id, CTabItem* pTabItem, int nGroupID=0);
    virtual void RegisterComponent( CContainerUI* pParentCtn, LPCTSTR id, LPCTSTR xmlid, CTabItem* pTabItem, int nGroupID=0);

    // 创建TabItem, 创建的TabItem不会被列入管理
	//virtual bool CreateItemUI(CTabItem* pTabItem, bool bVisible=true);

	// 重载OnCreate需要调用基类OnCreate
	virtual void OnCreate();

    // 创建注册的TabItem
	virtual void LoadRegisterComponent();

    // 取得TabItem
    CTabItem* GetComponent(tstring id);

    // 隐藏所有TabItem
    // nItemGroup表示TabItem所在的分组，切换的时候按分组操作；-1代表操作所有的Component，下同;
    void HideAllComponent(int nItemGroup=0);

    // 移除TabItem
    void RemoveComponent(tstring id);    
    void RemoveComponent(CTabItem* pTabItem);

    // TabItem被显示触发的事件
    virtual void OnShowComponent() {};

    // 显示某个TabItem
    // id : 该TabItem的索引id，也是生成界面时使用的xml id
    CTabItem* ShowComponent(tstring id, int nItemGroup=0);

    // 设置某个TabItem为显示或隐藏，不影响其他TabItem
    void ShowComponent(CTabItem* pTabItem, int nItemGroup=0, bool bShow=true);

    // 取得当前显示的TabItem
    CTabItem* GetCurrentComponent(int nItemGroup=0);

private:    
	vector<CTabItem*> _pTabItemList;
};