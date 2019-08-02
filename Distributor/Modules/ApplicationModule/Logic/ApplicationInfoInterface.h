#pragma once
class AppDataUpgrade;
App_CC_BEGIN
class ApplicationInfoInterface
{
public:
    // ��ʾ���ڼ�������ͼƬ
    virtual void ShowLoadingPicture(bool bShow) = 0;

    // ��ս�������
    virtual void ClearData() = 0;

    // ��ȡlist�ؼ���ѡ�������
    virtual vector<AppData*> GetSelectDatas() = 0;

    // ��list�ؼ���������
    virtual void InsertToAppView(const vector<AppData*>* pDatas) = 0;
    virtual void InsertToAppViewAction(const vector<AppData*>* pDatas) = 0;
    // ��list�ؼ��Ƴ�����
    virtual void RemoveFromAppView(AppData* pApp) = 0;
	virtual void RemoveFromAppViewAction(AppData* pApp) = 0;
	
	// ɾ��appʱ����ʱ����app���ݶ���ȥ��
	virtual void RemoveFromTempListData(AppData* pApp, vector<AppData*>& verDatas) = 0;


    // ��ʾӦ��ICON
    virtual void SetAppLogo(AppData* pApp, const DeviceData* pCurDevice) = 0;
    virtual void SetAppLogo(const vector<AppData*>* pDatas, const DeviceData* pCurDevice) = 0;
	//��ʾӦ��ICON���û���治����ֱ����Ĭ�ϵ�ͼƬ
	virtual void SetAppLogoWithoutLoad(const vector<AppData*>* pDatas, const DeviceData* pCurDevice) = 0;

	// ��ʾӦ�ô�С
	virtual void SetAppSize(const vector<AppData*>* pDatas, const DeviceData* pCurDevice) = 0;
	virtual void SetAppSizeAction(AppData* pApp, const DeviceData* pCurDevice) = 0;

	//����������ť
	virtual void SetUpgradeBtn(const vector<AppDataUpgrade*>* pDatas, const DeviceData* pCurDevice) = 0;
	virtual void SetUpgradeBtnEnabled(AppDataUpgrade* pData, bool enabled) = 0;
	virtual void SetAllUpgradeBtn(bool enabled) = 0;

	//����������ť
	virtual void SetLegalUpgradeBtn(const vector<AppDataUpgrade*>* pDatas, const DeviceData* pCurDevice) = 0;

	//��ô򿪰�װ�����·��
	virtual void GetAppFilePath(vector<wstring>& file) = 0;

    // ��װ����
    virtual bool InstallApp(bool isFile, wstring wsTitle=L"") = 0;

    // ��װ����
    virtual bool InstallApp( vector<wstring> files) = 0;

    // ˢ������
    virtual void RefreshData() = 0;

	// ��������
	virtual void UpdateData() = 0;
	virtual void UpdateDataAction() = 0;

	// �൱������
	virtual void Release() = 0;
    //֪ͨ���Ŀؼ�����
    virtual void SetControlAttribute() = 0;
    virtual bool IsHaveCheckSDCardState() = 0;
    virtual bool GetSDCardState() = 0;

    virtual void SendToUI(int nMessage) = 0;
};
App_CC_END