#pragma once

#include "Logic/WebResourceInnerMsg.h"

/////summary/////
///用于点击安装时，选择设备列表的界面
class CInstallDeviceListWnd:public CWindowUI
{
public:
	// 消息映射
	UIBEGIN_MSG_MAP

	EVNET_ID_HANDLER(UINOTIFY_CLICK, L"ChkNoTips",OnChkNoTips);//不再提示
	EVNET_ID_HANDLER(UINOTIFY_CLICK, L"AllSuitDevice",OnAllSuitDevice);//全部设备
	EVNET_ID_HANDLER(UINOTIFY_CLICK, L"OnkeyInstall",OnKeyInstall);//一键安装
	EVNET_ID_HANDLER(UINOTIFY_CLICK, L"SelectDevice",OnSelectDevice);//设备
	UIEND_MSG_MAP

	CInstallDeviceListWnd(E_PHONE_TYPE ePlatForm);
	~CInstallDeviceListWnd();
	void OnCreate();
	void ShowDeviceList(E_PHONE_TYPE ePlatForm);
	bool OnChkNoTips(TNotifyUI* pNotify);
	bool OnAllSuitDevice(TNotifyUI* pNotify);
	bool OnSelectDevice(TNotifyUI* pNotify);
	bool OnKeyInstall(TNotifyUI* pNotify);

	static bool IsAnroideNoTips(){return _bAnroidNoTips;}
	static bool IsIosNoTips(){return _bIosNoTips;}
	static void SetAnroideNoTips(bool bNoTips=true){_bAnroidNoTips=bNoTips;}
	static void SetIosNoTips(bool bNoTips=true){_bIosNoTips=bNoTips;}

	//不弹出界面直接安装之前选中的设备
	static void DirectInstall(vector<wstring> vec_Hash, E_PHONE_TYPE ePlatForm);

	//只有一台的设备，不弹出界面直接安装
	static void DirectInstallOne(vector<wstring> vec_Hash, E_PHONE_TYPE ePlatForm);

	void SetHashValue(vector<wstring>& vecHash){_vec_Hash=vecHash;}
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//检查是否安装过cpa软件
	void CheckInstalledCpa(string strHash, string deviceId);
	void CheckCpa(vector<wstring>& vec_Hash, vector<wstring>& vec_sn);

	static vector<wstring> GetSnByPlate(E_PHONE_TYPE ePlate);
	static vector<wstring> GetInstallOne(E_PHONE_TYPE ePlate);

	void  DelNotExistFile(vector<wstring>& vec_Hash);
	static bool  IsOnKeyInstall(){return _bOnKeyInstall;}
private:
	CLayoutUI* _pDeviceLayout;    
	CCheckBoxUI*  _pAllDeviceChkBox;
	CCheckBoxUI*  _pNoTipChkBox;
	int  _nCurChkBoxPosx;//当前chkbox的x坐标
	int  _nCurChkBoxPosy;//当前chkbox的y坐标
	vector<wstring> _vec_Hash;
	static vector<wstring> _g_vec_IosSn;
	static vector<wstring> _g_vec_AnroideSn;
	DEVICE_OS_TYPE _ePlatformType;
	static bool _bAnroidNoTips;
	static bool _bIosNoTips;

	static bool _bOnKeyInstall;
	static CInstallDeviceListWnd* pThis;
};