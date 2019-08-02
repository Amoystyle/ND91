#pragma once
class AppDataUpgrade;
App_CC_BEGIN
class ApplicationInfoInterface
{
public:
    // 显示正在加载数据图片
    virtual void ShowLoadingPicture(bool bShow) = 0;

    // 清空界面数据
    virtual void ClearData() = 0;

    // 获取list控件所选择的数据
    virtual vector<AppData*> GetSelectDatas() = 0;

    // 往list控件插入数据
    virtual void InsertToAppView(const vector<AppData*>* pDatas) = 0;
    virtual void InsertToAppViewAction(const vector<AppData*>* pDatas) = 0;
    // 从list控件移除数据
    virtual void RemoveFromAppView(AppData* pApp) = 0;
	virtual void RemoveFromAppViewAction(AppData* pApp) = 0;
	
	// 删除app时从临时搜索app数据队列去除
	virtual void RemoveFromTempListData(AppData* pApp, vector<AppData*>& verDatas) = 0;


    // 显示应用ICON
    virtual void SetAppLogo(AppData* pApp, const DeviceData* pCurDevice) = 0;
    virtual void SetAppLogo(const vector<AppData*>* pDatas, const DeviceData* pCurDevice) = 0;
	//显示应用ICON如果没缓存不加载直接用默认的图片
	virtual void SetAppLogoWithoutLoad(const vector<AppData*>* pDatas, const DeviceData* pCurDevice) = 0;

	// 显示应用大小
	virtual void SetAppSize(const vector<AppData*>* pDatas, const DeviceData* pCurDevice) = 0;
	virtual void SetAppSizeAction(AppData* pApp, const DeviceData* pCurDevice) = 0;

	//设置升级按钮
	virtual void SetUpgradeBtn(const vector<AppDataUpgrade*>* pDatas, const DeviceData* pCurDevice) = 0;
	virtual void SetUpgradeBtnEnabled(AppDataUpgrade* pData, bool enabled) = 0;
	virtual void SetAllUpgradeBtn(bool enabled) = 0;

	//设置升级按钮
	virtual void SetLegalUpgradeBtn(const vector<AppDataUpgrade*>* pDatas, const DeviceData* pCurDevice) = 0;

	//获得打开安装程序的路径
	virtual void GetAppFilePath(vector<wstring>& file) = 0;

    // 安装程序
    virtual bool InstallApp(bool isFile, wstring wsTitle=L"") = 0;

    // 安装程序
    virtual bool InstallApp( vector<wstring> files) = 0;

    // 刷新数据
    virtual void RefreshData() = 0;

	// 更新数据
	virtual void UpdateData() = 0;
	virtual void UpdateDataAction() = 0;

	// 相当于析构
	virtual void Release() = 0;
    //通知更改控件属性
    virtual void SetControlAttribute() = 0;
    virtual bool IsHaveCheckSDCardState() = 0;
    virtual bool GetSDCardState() = 0;

    virtual void SendToUI(int nMessage) = 0;
};
App_CC_END