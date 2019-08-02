#include "stdafx.h"

#include "GetSDCardStateCommand.h"
#include "GetSpaceInfoCommand.h"

#include "Core/DeviceData.h"
#include "Module/AndroidDefine.h"

GetSDCardStateCommand::GetSDCardStateCommand(const DeviceData* pDevice)
:BaseCommand(pDevice)
{
	_SDCardState = REMOVED;
	// Android
	_nAndroidChannelID = SYSINFO;
	_nAndroidCommandID = 3;
}

GetSDCardStateCommand::~GetSDCardStateCommand(void)
{
}

void ND91Assistant::GetSDCardStateCommand::Execute()
{
	try
	{

		if(_pDevice->GetDevInfo()->_deviceType == Ios)
		{
			_SDCardState = MOUNTED;
			return;
		}

		_SDCardState = REMOVED;
		if(_pDevice->GetAndroidDeamonMode() == 0)
		{
			GetSdcardPathState();
		}
		else
			DefaultExecute();
		if(_executesuccess == false)
		{
			GetSdcardPathState();
		}

	}
	catch(...)
	{
		_SDCardState = REMOVED;
	}
	/*if(_SDCardState != MOUNTED)
	{
		if(_pDevice->GetRootData()->GetDir()->findChildrenbyName(L"sdcard"))
			_SDCardState = MOUNTED;
	}*/
}
void  ND91Assistant::GetSDCardStateCommand::GetSdcardPathState()
{
	GetSpaceInfoCommand spaceCmd(_pDevice);
	spaceCmd.setPath(L"sdcard");
	spaceCmd.Execute();
	deque<SpaceInfo> infos = spaceCmd.GetDirInfos();
	int64_t total = 0;
	for (size_t i=0; i < infos.size(); i++)
	{
		total += infos[i]._totalsize;
	}
	if (total > 0)
		_SDCardState = MOUNTED;
}

std::string ND91Assistant::GetSDCardStateCommand::GetAndroidBodyData()
{
	BinaryBufferWriter  bufWrite;
	bufWrite.Clear();
	bufWrite.WriteInt(_nAndroidChannelID);
	bufWrite.WriteInt(_nAndroidCommandID);
	return bufWrite.GetBuffer();
}

void ND91Assistant::GetSDCardStateCommand::SetAndroidReturnData( std::string data )
{
    BinaryBufferReader reader(data.c_str(), data.length());
	if(!parseAndroidPackageHead(&reader))
		return;
	string result;
	reader.ReadString(result);

	result = CStrOperation::toLower(result);

	LOG->WriteDebug(L"getSDCardStatus=" + CCodeOperation::s2ws(result));
    if (result == "removed")
		_SDCardState =  REMOVED;
	else 
		if(result == "unmounted")
			_SDCardState = UNMOUNTED;
	else 
		if(result == "checking")
			_SDCardState = CHECKING;
	else 
		if(result == "nofs")
			_SDCardState = NOFS;
	else 
		if(result == "mounted")
			_SDCardState = MOUNTED;
	else 
		if(result == "mounted_ro")
			_SDCardState = MOUNTED_READ_ONLY;
	else 
		if(result ==  "shared")
			_SDCardState = SHARED;
	else 
		if(result == "bad_removal")
			_SDCardState = BAD_REMOVAL;
	else 
		if(result == "unmountable")
			_SDCardState = UNMOUNTABLE;

}

bool ND91Assistant::GetSDCardStateCommand::IsSDCardExist(const DeviceData* pDevice)
{
	if (pDevice->GetDevInfo()->_deviceType == Ios)
		return true;
	GetSDCardStateCommand cmd(pDevice);
	cmd.Execute();	
	return (cmd.GetSDCardState()==MOUNTED);
}

bool GetSDCardStateCommand::parseAndroidPackageHead( BinaryBufferReader* pReader )
{
	pReader->ReadBuffer(_pReturnHead, 4);
	pReader->ReadShort(_nReturnFlag);
	pReader->ReadShort(_nReturnLongConnectSign);
	pReader->ReadInt32(_nReturnBodySize);
	pReader->ReadInt32(_nReturnChannelID);
	//如果返回的数据包信息不符合本Command的信息，则跳过
	if (_nReturnChannelID != _nAndroidChannelID)
		return false;
	return true;
}