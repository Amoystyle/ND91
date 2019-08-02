#pragma once
App_CC_BEGIN
class ApplicationInfoUI;
// 程序管理list控件item数据类
class AppListBodyItemData
{

public:

public:
    AppListBodyItemData(CListUI* pList, 
                        const AppData* pApp,        // 程序数据
                        bool isAndroid,             // 是否安卓设备
                        bool hasSDCard,             // 是否有SD卡
                        ApplicationInfoUI* pUI);            
    ~AppListBodyItemData();

    wstring GetId() { return _appId; }
    CListContainerItemUI* GetContainerItemUI() { return _containerItem; }
    CButtonUI* GetUnistallButton() { return _buttonUninstall; }
    CButtonUI* GetMoveToButton() { return _buttonMoveTo; }
    CControlUI* GetIconControl() { return _control; }
	CLabelUI* GetStaticDiskLabel() { return _labelStaticDiskUsage; };
	CLabelUI* GetDynamicDiskLabel() { return _labelDynamicDiskUsage; };
	CButtonUI* GetPiracyUpButton() { return _buttonPiracyUp; }
	CButtonUI* GetLegalUpButton() { return _buttonLegalUp; }

    const AppData* GetAppData() { return _pApp; }

    void UpdateInfo(AppData* pData, bool isMoveAndroidApp=false);
    void RefreshLine();
	void SetAppDataIconPCPath(wstring strPath){  _pApp->_strIconPCPath = strPath; }

	bool GetPiracyUpVisible(){ return _buttonPiracyUp->IsVisible(); }
	bool GetLegalUpVisible(){ return _buttonLegalUp->IsVisible(); }

	void EnabledPiracyUpBtn(bool enabled);
	void EnabledLegalUpBtn(bool enabled);
	void SetToolTipText(const AppData* pApp,bool isAndroid, CControlUI* item);	//设置tooltip

private:
    void InitPointers();
    CControlUI* CreateContainerItemUI();

    CControlUI* CreateContainerUI(CControlUI* pContainerItem);
    void CreateIconControlUI(CControlUI* pContainer, wstring iconPath);// 创建图标控件
    void CreateAppNameLabelUI(CControlUI* pContainer, wstring appName, bool isAndroid);// 创建程序名控件
    void CreateAppPhonePathLabelUI(CControlUI* pContainer, wstring appPhonePath);// 创建程序路径控件

	CControlUI*  CreateVersionContainerUI(CControlUI* pContainerItem);// 创建版本容器控件
	void CreateVersionLabelUI(CControlUI* pContainerItem, wstring version);// 创建版本控件
	void CreatePiracyUpBtnUI(CControlUI* pContainerItem, ApplicationInfoUI* pUI);// 创建盗版升级控件
	void CreateLegalUpBtnUI(CControlUI* pContainerItem, ApplicationInfoUI* pUI);// 创建正版升级控件

    void CreateDynamicDiskLabelUI(CControlUI* pContainerItem, wstring staticDiskUsage);// 创建程序大小控件
    void CreateStaticDiskLabelUI(CControlUI* pContainerItem, wstring dynamicDiskUsage);// 创建文档大小控件
    void CreateInstallPosLabelUI(CControlUI* pContainerItem, wstring installPos);// 创建安装位置控件

    CControlUI* CreateContainerUI1(CControlUI* pContainerItem);
    void CreateUninstallButtonUI(CControlUI* pContainer1, bool isUserApp, bool isAndroid, bool hasSDCard, ApplicationInfoUI* pUI);// 创建卸载控件
    void CreateMoveToButtonUI(CControlUI* pContainer1, bool bOnSDCard, ApplicationInfoUI* pUI);// 创建移至手机或SD卡控件
private:
    CListUI*            _pList;
    wstring             _appId;                      // 程序ID也作为每项的ID
    
	CListContainerItemUI*   _containerItem;
    CContainerUI*       _container;
    CControlUI*         _control;                    // 图标控件
    CLabelUI*           _labelAppName;               // 程序名控件
    CLabelUI*           _labelAppPhonePath;	         // 程序路径 (Android手机中)              

    CLabelUI*           _labelStaticDiskUsage;       // 程序大小控件
    CLabelUI*           _labelDynamicDiskUsage;      // 文档大小控件
    CLabelUI*           _labelVersion;               // 版本控件
    CLabelUI*           _labelInstallPos;            // 安装位置控件

    CContainerUI*       _container1;
    CButtonUI*          _buttonUninstall;           // 卸载控件
    CButtonUI*          _buttonMoveTo;              // 移至手机或SD卡控件

	CContainerUI*       _containerversion;			// 版本容器
	CButtonUI*			_buttonPiracyUp;			// 盗版升级
	CButtonUI*			_buttonLegalUp;				// 正版升级

    AppData*      _pApp;                      // 程序数据
};
App_CC_END