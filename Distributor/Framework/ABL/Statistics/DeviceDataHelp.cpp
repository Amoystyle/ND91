#include "stdafx.h"
#include "DeviceDataHelp.h"
#include "Common/Device/AblDeviceInfo.h"

DeviceDataHelp::DeviceDataHelp(AblDeviceInfo* pdata)
{
	_pData = pdata;
}

DeviceDataHelp::~DeviceDataHelp()
{
}

int DeviceDataHelp::GetDeviceType()
{
	int nType = 0;
	if ( _pData->baseInfo.nOsType == ePhoneOS_Ios )
		nType = _pData->baseInfo.nPlatForm == ePhonePlatForm_iPad ? 7 : 1;
	else if ( _pData->baseInfo.nOsType == ePhoneOS_Android )
		nType = _pData->baseInfo.nPlatForm == ePhonePlatForm_Android  ? 4 : 5;
	else
		return nType;

	return nType;
}

int DeviceDataHelp::GetJailbroken()
{
	int JailBreaked = -1;
	if ( _pData->baseInfo.nOsType == ePhoneOS_Ios )
		JailBreaked = _pData->bIsJailBreak ? 1 : 0;
	else if ( _pData->baseInfo.nOsType == ePhoneOS_Android )
		JailBreaked = _pData->bIsRoot ? 1 : 0;
	else
		return JailBreaked;

	return JailBreaked;
}

// std::string DeviceDataHelp::GetDeviceHardID()
// {INTERFUN;
// 	CAssembleEnvironment* pAssembleEnvironment = NULL;
// 	CSystem *pSystem=CSystem::Create();
// 	// 遍历设备
// 	POSITION pos = pSystem->GetHeadPosition();
// 	for (int i=0; i<pSystem->GetCount(); i++)
// 	{
// 		pAssembleEnvironment = pSystem->GetNext(pos);                    
// 		wstring id = CDriverInstallOperation::GetID(pAssembleEnvironment);			//拿到设备id
// 
// 		if ( _pData->GetId() == id ) break;		
// 	}
// 
// 	RETURN_VALUE_IF(!pAssembleEnvironment,"");
// 	std::string hardId, key;
// 	POSITION pos1 = pAssembleEnvironment->GetHeadPosition();
// 	for (int i=0; i<pAssembleEnvironment->GetCount(); i++)
// 	{
// 		CEquipment* pEquipment = pAssembleEnvironment->GetNext(pos1);
// 		key = pEquipment->GetInterface();
// 		int pos = key.rfind("\\");
// 		string id = key.substr(pos+1);
// 		if (pos >= 0 &&  CCodeOperation::UnicodeToGB2312(_pData->GetId()) == id )
// 		{
// 			hardId = pEquipment->GetHardID();
// 			break;
// 		}	
// 	}
// 
// 	return hardId;
// }

std::string DeviceDataHelp::GetTrueImei()
{
	wstring strVal;
	
	if ( _pData->baseInfo.nOsType == ePhoneOS_Ios )
	{
		strVal = _pData->deviceID.empty() ? _pData->baseInfo.deviceIMEI :  _pData->deviceID;
	}
	else
	{
		//wstring str = CCodeOperation::GB2312ToUnicode(GetDeviceHardID());
		strVal = _pData->baseInfo.deviceIMEI.empty() ? _pData->strMacAddress : _pData->baseInfo.deviceIMEI;
		
		if ( strVal.empty() )
			strVal = _pData->baseInfo.serialNumber;
	}

	return CCodeOperation::UnicodeToGB2312(strVal);
}
