#pragma once

#include "Logic/WebResourceInnerMsg.h"

/////summary/////
///���ڵ����װʱ��ѡ���豸�б�Ľ���
class CInstallDeviceListWnd:public CWindowUI
{
public:
	// ��Ϣӳ��
	UIBEGIN_MSG_MAP

	EVNET_ID_HANDLER(UINOTIFY_CLICK, L"ChkNoTips",OnChkNoTips);//������ʾ
	EVNET_ID_HANDLER(UINOTIFY_CLICK, L"AllSuitDevice",OnAllSuitDevice);//ȫ���豸
	EVNET_ID_HANDLER(UINOTIFY_CLICK, L"OnkeyInstall",OnKeyInstall);//һ����װ
	EVNET_ID_HANDLER(UINOTIFY_CLICK, L"SelectDevice",OnSelectDevice);//�豸
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

	//����������ֱ�Ӱ�װ֮ǰѡ�е��豸
	static void DirectInstall(vector<wstring> vec_Hash, E_PHONE_TYPE ePlatForm);

	//ֻ��һ̨���豸������������ֱ�Ӱ�װ
	static void DirectInstallOne(vector<wstring> vec_Hash, E_PHONE_TYPE ePlatForm);

	void SetHashValue(vector<wstring>& vecHash){_vec_Hash=vecHash;}
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//����Ƿ�װ��cpa���
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
	int  _nCurChkBoxPosx;//��ǰchkbox��x����
	int  _nCurChkBoxPosy;//��ǰchkbox��y����
	vector<wstring> _vec_Hash;
	static vector<wstring> _g_vec_IosSn;
	static vector<wstring> _g_vec_AnroideSn;
	DEVICE_OS_TYPE _ePlatformType;
	static bool _bAnroidNoTips;
	static bool _bIosNoTips;

	static bool _bOnKeyInstall;
	static CInstallDeviceListWnd* pThis;
};