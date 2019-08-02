#pragma once
App_CC_BEGIN
class ApplicationInfoUI;
// �������list�ؼ�item������
class AppListBodyItemData
{

public:

public:
    AppListBodyItemData(CListUI* pList, 
                        const AppData* pApp,        // ��������
                        bool isAndroid,             // �Ƿ�׿�豸
                        bool hasSDCard,             // �Ƿ���SD��
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
	void SetToolTipText(const AppData* pApp,bool isAndroid, CControlUI* item);	//����tooltip

private:
    void InitPointers();
    CControlUI* CreateContainerItemUI();

    CControlUI* CreateContainerUI(CControlUI* pContainerItem);
    void CreateIconControlUI(CControlUI* pContainer, wstring iconPath);// ����ͼ��ؼ�
    void CreateAppNameLabelUI(CControlUI* pContainer, wstring appName, bool isAndroid);// �����������ؼ�
    void CreateAppPhonePathLabelUI(CControlUI* pContainer, wstring appPhonePath);// ��������·���ؼ�

	CControlUI*  CreateVersionContainerUI(CControlUI* pContainerItem);// �����汾�����ؼ�
	void CreateVersionLabelUI(CControlUI* pContainerItem, wstring version);// �����汾�ؼ�
	void CreatePiracyUpBtnUI(CControlUI* pContainerItem, ApplicationInfoUI* pUI);// �������������ؼ�
	void CreateLegalUpBtnUI(CControlUI* pContainerItem, ApplicationInfoUI* pUI);// �������������ؼ�

    void CreateDynamicDiskLabelUI(CControlUI* pContainerItem, wstring staticDiskUsage);// ���������С�ؼ�
    void CreateStaticDiskLabelUI(CControlUI* pContainerItem, wstring dynamicDiskUsage);// �����ĵ���С�ؼ�
    void CreateInstallPosLabelUI(CControlUI* pContainerItem, wstring installPos);// ������װλ�ÿؼ�

    CControlUI* CreateContainerUI1(CControlUI* pContainerItem);
    void CreateUninstallButtonUI(CControlUI* pContainer1, bool isUserApp, bool isAndroid, bool hasSDCard, ApplicationInfoUI* pUI);// ����ж�ؿؼ�
    void CreateMoveToButtonUI(CControlUI* pContainer1, bool bOnSDCard, ApplicationInfoUI* pUI);// ���������ֻ���SD���ؼ�
private:
    CListUI*            _pList;
    wstring             _appId;                      // ����IDҲ��Ϊÿ���ID
    
	CListContainerItemUI*   _containerItem;
    CContainerUI*       _container;
    CControlUI*         _control;                    // ͼ��ؼ�
    CLabelUI*           _labelAppName;               // �������ؼ�
    CLabelUI*           _labelAppPhonePath;	         // ����·�� (Android�ֻ���)              

    CLabelUI*           _labelStaticDiskUsage;       // �����С�ؼ�
    CLabelUI*           _labelDynamicDiskUsage;      // �ĵ���С�ؼ�
    CLabelUI*           _labelVersion;               // �汾�ؼ�
    CLabelUI*           _labelInstallPos;            // ��װλ�ÿؼ�

    CContainerUI*       _container1;
    CButtonUI*          _buttonUninstall;           // ж�ؿؼ�
    CButtonUI*          _buttonMoveTo;              // �����ֻ���SD���ؼ�

	CContainerUI*       _containerversion;			// �汾����
	CButtonUI*			_buttonPiracyUp;			// ��������
	CButtonUI*			_buttonLegalUp;				// ��������

    AppData*      _pApp;                      // ��������
};
App_CC_END