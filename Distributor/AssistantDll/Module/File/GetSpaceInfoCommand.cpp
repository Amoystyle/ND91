#include "stdafx.h"

// 守护命令变更，且由于各手机兼容性问题，全部改为守护获取
#include "GetSpaceInfoCommand.h"

#include "Core/DeviceData.h"
#include "Device/AdbDirOperateCommand.h"
#include "Module/AndroidDefine.h"
#include "Monitor/AndroidMonitorThread.h"

GetSpaceInfoCommand::GetSpaceInfoCommand(const DeviceData* pDevice)
: BaseCommand(pDevice)
{
	_Path = L"";

	// Android
// 	_nAndroidChannelID = SHELL;
// 	_nAndroidCommandID = ExecShell;
    _nAndroidChannelID = SYSINFO;
    _nAndroidCommandID = GET_DIRSPACEINFO;

}

GetSpaceInfoCommand::~GetSpaceInfoCommand()
{
}

std::string GetSpaceInfoCommand::GetAndroidBodyData()
{
/*
	string command = "df";
	if(_Path.length() > 0 && _Path != L"/")
	{
		command	= "df \""+ CCodeOperation::UnicodeToUTF_8(_Path) + "\"";
	}

	BinaryBufferWriter BufferWriter;
	BufferWriter.Clear();
	BufferWriter.WriteInt(_nAndroidChannelID);
	BufferWriter.WriteInt(_nAndroidCommandID);
	BufferWriter.WriteInt(command.length());
	BufferWriter.WriteBuffer(command.c_str(), command.length());
*/
    string strUtf8 = CCodeOperation::UnicodeToUTF_8(_Path); 
    BinaryBufferWriter BufferWriter;
    BufferWriter.Clear();
    BufferWriter.WriteInt(_nAndroidChannelID);
    BufferWriter.WriteInt(_nAndroidCommandID);
    BufferWriter.WriteInt(strUtf8.length());
    BufferWriter.WriteBuffer(strUtf8.c_str(), strUtf8.length());
	return BufferWriter.GetBuffer();
}

void ND91Assistant::GetSpaceInfoCommand::Execute()
{
    // 强制使用守护
//	if(_pDevice->GetDevInfo()->_deviceType == Ios || (_pDevice->GetDevInfo()->_deviceType == Android && ((AndroidMonitorThread*)_pDevice->GetMonitorThread())->isDeamonVaid()))
	{
		 DefaultExecute();
		 return;
	}
	if (_pDevice->GetDevInfo()->_connectType == WIFI)
		DefaultExecute();
	else	// 非wifi则走adb，无需连守护
	{
		string command = "df";
		if(_Path.length() > 0 && _Path != L"/")
		{
			command	= "df \""+ CCodeOperation::UnicodeToUTF_8(_Path) + "\"";
		}
		AdbDirOperateCommand adbcmd(_pDevice->GetDevInfo()->_strSerialNumber);
		adbcmd.setParam(command);
		adbcmd.Execute();
		if(!adbcmd.IsOk())
			return;
		string shellresult = adbcmd.GetReturnData();

		if((int)shellresult.find("No such file or directory") >= 0 
			|| (int)shellresult.find("Permission denied") >= 0 
			|| (int)shellresult.find("Not a directory") >= 0)
		{
			return;
		}
		Analyze(shellresult);
	}
}

void GetSpaceInfoCommand::SetAndroidReturnData(std::string data)
{
	string shellresult = "";
	BinaryBufferReader reader(data.c_str(),data.length());
    if( !parseAndroidPackageHead_Top16(&reader))
        return;

    _SpaceInfos.clear();
    SpaceInfo info;
    reader.ReadInt64(info._totalsize);
    reader.ReadInt64(info._Available);
    reader.ReadInt64(info._usedsize);
    int nSize = 0;
    string strUTF8;
    reader.ReadInt32(nSize);
    reader.ReadString(strUTF8, nSize);
    info._path = CCodeOperation::UTF_8ToUnicode(strUTF8);
    _SpaceInfos.push_back(info);
   
/*
	if( !parseAndroidPackageHead(&reader))
		return;
	bool issuccess = false;
	reader.ReadBool(issuccess);
	if(issuccess)
	{
		int lenth = 0;
		reader.ReadInt32(lenth);
		reader.ReadString(shellresult, lenth);

		//LOG->WriteDebug(CCodeOperation::string2wstring(shellresult));

		if((int)shellresult.find("No such file or directory") >= 0 
			|| (int)shellresult.find("Permission denied") >= 0 
			|| (int)shellresult.find("Not a directory") >= 0)
		{
			return;
		}
		Analyze(shellresult);
	}*/
}

void GetSpaceInfoCommand::Analyze(const string& result)
{
//     LOG_BINARY(Log::LOG_DEBUG, result);
	_SpaceInfos.clear();
	vector<string>  lines = CStrOperation::parseLines(result.c_str(), result.length());
	for(vector<string>::iterator it = lines.begin(); it!=lines.end(); ++it)
	{
		SpaceInfo adir = AnalyzeSpaceInfo(*it);
		if (adir._path.length() > 0)
			_SpaceInfos.push_back(adir);
	}
}

SpaceInfo GetSpaceInfoCommand::AnalyzeSpaceInfo(const string& result)
{
    return _rh.AnalyzeSpaceInfo(result);
}