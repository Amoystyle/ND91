#include "stdafx.h"
#include "InstallDeviceListWnd.h"
#include "Logic/WebResourceInnerMsg.h"

const int g_ChkHieght = 30;
bool CInstallDeviceListWnd::_bAnroidNoTips=false;
bool CInstallDeviceListWnd::_bIosNoTips=false;
vector<wstring> CInstallDeviceListWnd::_g_vec_IosSn;
vector<wstring> CInstallDeviceListWnd::_g_vec_AnroideSn;

CInstallDeviceListWnd* CInstallDeviceListWnd::pThis = NULL;
bool CInstallDeviceListWnd::_bOnKeyInstall = false;
CInstallDeviceListWnd::CInstallDeviceListWnd(E_PHONE_TYPE ePlatForm)
{
	pThis = this;
	_nCurChkBoxPosx = 0;
	_nCurChkBoxPosy = 0;
	if (E_ANDROID==ePlatForm)
	{
		_g_vec_AnroideSn.clear();
	}
	else
	{
		_g_vec_IosSn.clear();
	}

	_bOnKeyInstall = false;
}

CInstallDeviceListWnd::~CInstallDeviceListWnd()
{

}
void CInstallDeviceListWnd::OnCreate()
{
	_pDeviceLayout = (CLayoutUI*)GetItem(L"Center_Section");
	_pAllDeviceChkBox = (CCheckBoxUI*)GetItem(L"Center_Section.AllSuitDevice");
	_pNoTipChkBox = (CCheckBoxUI*)GetItem(L"Bottom_Section.ChkNoTips");
assert(_pDeviceLayout);
assert(_pAllDeviceChkBox);
assert(_pNoTipChkBox);
	RECT rt = _pAllDeviceChkBox->GetRect();
	_nCurChkBoxPosx = rt.left;
	_nCurChkBoxPosy = rt.top;
}

bool CInstallDeviceListWnd::OnChkNoTips(TNotifyUI* pNotify)
{
	CCheckBoxUI* pNoTips = dynamic_cast<CCheckBoxUI*>(pNotify->pSender);
	if (NULL == pNoTips)
	{
		return false;
	}
	if (Android == _ePlatformType)
	{
		_bAnroidNoTips = pNoTips->IsChecked();
	}
	else
	{
		_bIosNoTips = pNoTips->IsChecked();
	}

	return true;
};

bool CInstallDeviceListWnd::OnSelectDevice(TNotifyUI* pNotify)
{
	CCheckBoxUI* pDevice = dynamic_cast<CCheckBoxUI*>(pNotify->pSender);
	if (NULL == pDevice)
	{
		return false;
	}

	if (!pDevice->IsChecked())
	{
		_pAllDeviceChkBox->SetChecked(false, false);
		return true;
	}

	bool bAllChked=true;
	int nCount = _pDeviceLayout->GetCount();
	for (int i=2; i<nCount; ++i)
	{
		CCheckBoxUI* pChkBox = dynamic_cast<CCheckBoxUI*>(_pDeviceLayout->GetItem(i));
		if (NULL != pChkBox)
		{
			if (!pChkBox->IsChecked())
			{
				bAllChked = false;
			}
		}
	}

	if (bAllChked)
	{
		_pAllDeviceChkBox->SetChecked(true, false);
	}

	return true;
}

bool CInstallDeviceListWnd::OnAllSuitDevice(TNotifyUI* pNotify)
{
	CCheckBoxUI* pAll = dynamic_cast<CCheckBoxUI*>(pNotify->pSender);
	if (NULL == pAll)
	{
		return false;
	}

	int nCount = _pDeviceLayout->GetCount();
	for (int i=2; i<nCount; ++i)
	{
		CCheckBoxUI* pChkBox = dynamic_cast<CCheckBoxUI*>(_pDeviceLayout->GetItem(i));
		if (NULL != pChkBox)
		{
			pChkBox->SetChecked(pAll->IsChecked());
		}
	}
	_pDeviceLayout->Invalidate();

	return true;
};

void CInstallDeviceListWnd::ShowDeviceList(E_PHONE_TYPE ePlatForm)
{INTERFUN;

	vector<wstring> idList = DEVICE_MANAGER->GetAllDevice();
	int size = idList.size();
	if(size > 0)
	{
		for (int i = 0; i < size; i++)
		{
			wstring sn = idList.at(i);
			DeviceData* pDevice = DEVICE_MANAGER->FindDevice(sn);
			if (NULL == pDevice)
			{
				continue;
			}
			DEVICE_OS_TYPE eType=Ios;
			if (E_ANDROID==ePlatForm)
			{
				eType = Android;
			}

			if(pDevice->GetDevInfo()->_deviceType != eType)
				continue;

			_ePlatformType = eType;

			CCheckBoxUI* pChkBox = new CCheckBoxUI;
			pChkBox->SetInitPos(_nCurChkBoxPosx, _nCurChkBoxPosy, 0, _nCurChkBoxPosy+g_ChkHieght, 4);
			pChkBox->SetText(pDevice->GetDevInfo()->GetDeviceName().c_str());
			//pChkBox->SetId(sn.c_str());
			pChkBox->SetId(L"SelectDevice");
			wstring* pStr = new wstring(sn);
			pChkBox->SetUserData((UINT)pStr);
			_nCurChkBoxPosy += g_ChkHieght;
			_pDeviceLayout->Add(pChkBox);
		}
	}
}

bool CInstallDeviceListWnd::OnKeyInstall(TNotifyUI* pNotify)
{
	_g_vec_AnroideSn.clear();
	_g_vec_IosSn.clear();
	_bOnKeyInstall = true;
	int nCount = _pDeviceLayout->GetCount();
	for (int i=2; i<nCount; ++i)
	{
		CCheckBoxUI* pChkBox = dynamic_cast<CCheckBoxUI*>(_pDeviceLayout->GetItem(i));
		if (NULL != pChkBox && pChkBox->IsChecked())
		{
			if (Android == _ePlatformType)
			{
				wstring* pTemp = (wstring*)pChkBox->GetUserData();
				if (pTemp)
				{
					_g_vec_AnroideSn.push_back(*pTemp);
				}
			}
			else
			{
				wstring* pTemp = (wstring*)pChkBox->GetUserData();
				if (pTemp)
				{
					_g_vec_IosSn.push_back(*pTemp);
				}
			}
		}
	}

	this->DelNotExistFile(_vec_Hash);
	if (_vec_Hash.empty())
	{//如果没有存在的软件安装则直接返回
		this->CloseWindow();
		return true;
	}

	WebSoftInstallMsg msg;
	msg._st_Hash = _vec_Hash;
	if (Android == _ePlatformType)
	{
		if (_g_vec_AnroideSn.empty())
		{
			this->CloseWindow();
			return true;
		}
		msg._st_Sn = _g_vec_AnroideSn;
	}
	else
	{	
		if (_g_vec_IosSn.empty())
		{
			this->CloseWindow();
			return true;
		}
		msg._st_Sn = _g_vec_IosSn;
	}
	
	msg.Send();

	this->CloseWindow();

	return true;
}

void CInstallDeviceListWnd::DirectInstall(vector<wstring> vec_Hash, E_PHONE_TYPE ePlatForm)
{
	pThis->DelNotExistFile(vec_Hash);
	if (vec_Hash.empty())
	{//如果没有存在的软件安装则直接返回
		return;
	}
	WebSoftInstallMsg msg;
	msg._st_Hash = vec_Hash;
	if (E_ANDROID == ePlatForm)
	{
		if (_g_vec_AnroideSn.empty())
		{
			return;
		}
		msg._st_Sn = _g_vec_AnroideSn;
	}
	else
	{
		if (_g_vec_IosSn.empty())
		{
			return;
		}
		msg._st_Sn = _g_vec_IosSn;
	}
	msg.Send();
}

void CInstallDeviceListWnd::DirectInstallOne(vector<wstring> vec_Hash, E_PHONE_TYPE ePlatForm)
{
	pThis->DelNotExistFile(vec_Hash);
	if (vec_Hash.empty())
	{//如果没有存在的软件安装则直接返回
		return;
	}
	vector<wstring> vec_Sn;
	vector<wstring> idList = DEVICE_MANAGER->GetAllDevice();
	int size = idList.size();
	for (int i = 0; i < size; i++)
	{
		wstring sn = idList.at(i);
		DeviceData* pDevice = DEVICE_MANAGER->FindDevice(sn);
		if (NULL == pDevice)
		{
			continue;
		}
		DEVICE_OS_TYPE eType=Ios;
		if (E_ANDROID==ePlatForm)
		{
			eType = Android;
		}

		if(pDevice->GetDevInfo()->_deviceType != eType)
			continue;

		vec_Sn.push_back(sn);
		break;
	}

	WebSoftInstallMsg msg;
	msg._st_Hash = vec_Hash;
	msg._st_Sn = vec_Sn;
	msg.Send();
}

LRESULT CInstallDeviceListWnd::WindowProc( UINT message, WPARAM wParam, LPARAM lParam )
{INTERFUN;
	switch (message)
	{
	case WM_CLOSE:
		{
			if (_bOnKeyInstall)
			{
				break;
			}
			if (Android == _ePlatformType)
			{
				_bAnroidNoTips = false;
			}
			else
			{
				_bIosNoTips = false;
			}
		}
		break;
	}
	return CWindowUI::WindowProc(message,wParam,lParam);
}

void CInstallDeviceListWnd::CheckCpa(vector<wstring>& vec_Hash, vector<wstring>& vec_sn)
{
	for (vector<wstring>::iterator iter = vec_sn.begin(); iter != vec_sn.end(); ++iter)
	{
		DeviceData* pData=DEVICE_MANAGER->FindDevice(*iter);
		if (pData)
		{
			DeviceInfo* pInfo = pData->GetMutableDevInfo();
			if (pInfo)
			{
				for (vector<wstring>::iterator hash = vec_Hash.begin(); hash != vec_Hash.end(); ++hash)
				{
					CheckInstalledCpa(CCodeOperation::UnicodeToGB2312(*hash), CCodeOperation::UnicodeToGB2312(pInfo->_deviceIMEI));
				}
			}
		}
	}
}
void CInstallDeviceListWnd::CheckInstalledCpa(string strHash, string deviceId)
{
	//判断该软件是否已经装过该手机
	DeviceData* pData = DEVICE_MANAGER->FindDevice(CCodeOperation::GB2312ToUnicode(deviceId));
	DeviceInfo* pInfo = NULL;
	if (NULL == pData)
	{
		return;

	}
	pInfo = pData->GetMutableDevInfo();
	if (NULL == pInfo)
	{
		return;
	}

	CheckCpaInstalledMsg ChkMsg;
	ChkMsg._strHash = strHash;
	if (Android == _ePlatformType)
	{
		wstring wsTemp = pData->GetId().empty() ? pInfo->_deviceIMEI :  pData->GetId();

		if ( wsTemp.empty() )
			wsTemp = pInfo->_strSerialNumber;

		ChkMsg._strImei = CCodeOperation::UnicodeToGB2312(wsTemp);
	}
	else
	{
		wstring wsTemp = pData->GetId().empty() ? pInfo->_deviceIMEI : pData->GetId();
		ChkMsg._strImei = CCodeOperation::UnicodeToGB2312(wsTemp);
	}
	//ChkMsg._strImei = CCodeOperation::UnicodeToGB2312(pInfo->_deviceIMEI);
	ChkMsg.SendAndGetReturn();
	if (!ChkMsg._strReturn.empty())
	{//吐司
		//MessageDialog(CCodeOperation::GB2312ToUnicode(ChkMsg._strReturn));
		NoticeDialogParam* pParam = new NoticeDialogParam;
		pParam->strMsg = CCodeOperation::GB2312ToUnicode(ChkMsg._strReturn);
		pParam->strIcon = _T("#CommonUi_icon_TipWarning");
		CommonDlgMsg msg;
		msg.commonDlgType = CommDlgTypeNoticeDialog;
		msg.lParam = LPARAM(pParam);
		msg.SendToUI();
		return;
	}

	return;
}

vector<wstring> CInstallDeviceListWnd::GetSnByPlate(E_PHONE_TYPE ePlate)
{
	if (E_ANDROID == ePlate)
	{
		return _g_vec_AnroideSn;
	}
	else
		return _g_vec_IosSn;
}

vector<wstring> CInstallDeviceListWnd::GetInstallOne(E_PHONE_TYPE ePlatForm)
{
	vector<wstring> vec_Sn;
	vector<wstring> idList = DEVICE_MANAGER->GetAllDevice();
	int size = idList.size();
	if(size > 0)
	{
		for (int i = 0; i < size; i++)
		{
			wstring sn = idList.at(i);
			DeviceData* pDevice = DEVICE_MANAGER->FindDevice(sn);
			if (NULL == pDevice)
			{
				continue;
			}
			DEVICE_OS_TYPE eType=Ios;
			if (E_ANDROID==ePlatForm)
			{
				eType = Android;
			}

			if(pDevice->GetDevInfo()->_deviceType != eType)
				continue;

			vec_Sn.push_back(sn);
			break;
		}
	}
	return vec_Sn;
}

void CInstallDeviceListWnd::DelNotExistFile(vector<wstring>& vec_Hash)
{
	LocalCpaPathMsg CpaMsg;
	CpaMsg._vec_Hash = vec_Hash;
	CpaMsg.SendAndGetReturn();
	vector<wstring> vec_files = CpaMsg._vec_Path;
	vector<wstring>::iterator fileiter = vec_files.begin();
	vector<wstring>::iterator Hashiter = vec_Hash.begin();
	for (; fileiter != vec_files.end() || Hashiter != vec_Hash.end(); ++fileiter)
	{
		if (!CFileOperation::IsFileExist((*fileiter).c_str()))
		{
			Hashiter = vec_Hash.erase(Hashiter);
		}
		else
			++Hashiter;
	}
}