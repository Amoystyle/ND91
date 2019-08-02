#include "stdafx.h"
#include "DDMManager.h"
#include "ConnectionManager.h"


DDMManager::DDMManager(ConnectionManager* mgr)
{
	_pMrg = mgr;
	this->_pFunPhoneCenter_Start = NULL;
	this->_pFunPhoneCenter_Connect = NULL;
	this->_pFunPhoneCenter_Exit = NULL;
	this->_hPhoneCenterLibrary=NULL;
}

DDMManager::~DDMManager()
{

}

bool DDMManager::Init()
{    
	if(!this->LoadPhoneCenter())
		return false;

	if (!_pFunPhoneCenter_Start)
		return false;

	if (_pFunPhoneCenter_Start && _pFunPhoneCenter_Start(EnumConnectMethod_AutoConnectAll // 自动连接所有
		,EnumConnectSuccessSign_ADB       // 以Adb可以连接为依据
		,DDMManager::PhoneCenterCallBack
		,this))
	{
		return false;
	}
	return true;
}

void DDMManager::Release()
{
	if (_pFunPhoneCenter_Exit)
		_pFunPhoneCenter_Exit();

	this->FreePhoneCenter();
}

bool DDMManager::ReloadLib()
{
	this->FreePhoneCenter();

	return this->Init();
}

bool DDMManager::LoadPhoneCenter()
{	
	wstring strPath = CONFIG(_strWorkPath) + L"PhoneCenter.dll";
	_hPhoneCenterLibrary = ::LoadLibrary(strPath.c_str());
	if (_hPhoneCenterLibrary)
	{
		_pFunPhoneCenter_Start = (FunPhoneCenter_Start)GetProcAddress(_hPhoneCenterLibrary, "PhoneCenter_Start");      
		_pFunPhoneCenter_Connect = (FunPhoneCenter_Connect)GetProcAddress(_hPhoneCenterLibrary, "PhoneCenter_Connect");      
		_pFunPhoneCenter_Exit = (FunPhoneCenter_Exit)GetProcAddress(_hPhoneCenterLibrary, "PhoneCenter_Exit");        
	}    
	return true;
}

bool DDMManager::FreePhoneCenter()
{
	if (_hPhoneCenterLibrary)
	{
		::FreeLibrary(_hPhoneCenterLibrary);
		_pFunPhoneCenter_Start = NULL;
		_pFunPhoneCenter_Exit = NULL;
		_hPhoneCenterLibrary = NULL;
		return true;
	}
	return false;
}

void DDMManager::ReConnect(void* key)
{
	if(_pFunPhoneCenter_Connect)
		_pFunPhoneCenter_Connect(key);
}

void DDMManager::PhoneCenterCallBack( EnumEvent eve, void* pParam, void*pOwn )
{
	try
	{
		DDMManager* pThis = (DDMManager*)pOwn;

		if (!pThis || !pParam) 
			return;

		SetCSLockExt(*(pThis->GetLock()));

		if (eve == EnumEventCollectionChange)
		{
			//设备变动

			STCollectionChangeParam* param = (STCollectionChangeParam*)pParam;
			if (!param) 
				return;

			LOG->WriteDebug(L"EnumEventCollectionChange: pPhone->" + CStrOperation::IntToWString((UINT)param->pPhone) + 
				L" PhoneType->" + CStrOperation::IntToWString((UINT)param->PhoneType));

			wstring sn = param->szKey ? CCodeOperation::GB2312ToUnicode(param->szKey) : L"";
			if (param->type == NotifyCollectionChangedAction_Add)
			{
				LOG->WriteDebug(L"NotifyCollectionChangedAction_Add: pPhone->" + CStrOperation::IntToWString((UINT)param->pPhone));

				pThis->_pMrg->OnDeviceLinkIn((void*)(param->pPhone), sn, param->PhoneType, CCodeOperation::GB2312ToUnicode(param->szPhoneKey));
			}
			else if (param->type == NotifyCollectionChangedAction_Remove)
			{
				LOG->WriteDebug(L"NotifyCollectionChangedAction_Remove Begin: sn->" + sn + L"pPhone->" + CStrOperation::IntToWString((UINT)param->pPhone));
				pThis->_pMrg->OnDeviceLinkOut((void*)(param->pPhone), sn, param->PhoneType);
				LOG->WriteDebug(L"NotifyCollectionChangedAction_Remove End: sn->" + sn + L"pPhone->" + CStrOperation::IntToWString((UINT)param->pPhone));
			}
			else
			{
				LOG->WriteDebug(L"NotifyCollectionChangedAction_Unknow pPhone->" + CStrOperation::IntToWString((UINT)param->pPhone));
			}
		}
		else if (eve == EnumEventPropertyChanged)
		{
			
			//属性变化
			STPhonePropertyChanged* param = (STPhonePropertyChanged*)pParam; 
            if (!param || !param->szData) 
				return;

			LOG->WriteDebug(L"EnumEventPropertyChanged: pPhone->" + CStrOperation::IntToWString((UINT)param->pPhone) +
				L" Type->"+ CStrOperation::IntToWString(param->type)	+ 
				L" Data ->"+CCodeOperation::GB2312ToUnicode(param->szData));

			int nValue = param->szData ? atoi(param->szData) : 0;

			//当硬件层准备就绪且设备类型为Android时，进行设备扫描（通过ADB扫描当前可用设备，5.x中不需要）
			if (param->type == EnumPhoneProperty_ConnectState && (nValue == EnumConnectState_ConnectSuccess || nValue == EnumConnectState_UnConnect) &&
				 pThis->_pMrg->GetPhoneType(param->pPhone) == EnumPhoneType_Android)
			{
				LOG->WriteDebug(L"EnumEventPropertyChanged Scan: pPhone->" + CStrOperation::IntToWString((UINT)param->pPhone));
				pThis->_pMrg->NotifyScan();
			}

			pThis->_pMrg->OnDevicePropertyChange(param->pPhone, param->type, param->szData);
		}
		else if(eve == EnumExpetion)
		{
			LOG->WriteDebug(L"DDMManager EnumExpetion");
		}
	}
	catch (...)
	{
		LOG->WriteDebug(L"DDMManager catch");
	}

}