#include "pch_module.h"

#ifdef MODULE_SYSTEM

#include "Module/Application/UninstallSystemAppCommand.h"
#include "Module/Application/AppData.h"
#include "Module/AndroidDefine.h"
#include "GetNdShStateCommand.h"
#include "GetNDSHAuthCommand.h"

UninstallSystemAppCommand::UninstallSystemAppCommand(const DeviceData* pDevice, const AppData* appData)
: BaseCommand(pDevice)
{
    _nAndroidChannelID = SYSINFO;
    _nAndroidCommandID = UNINSTALL_SYSTEMAPP;
	
	_bSuccess = false;
	_appData  = appData;	
}

UninstallSystemAppCommand::~UninstallSystemAppCommand()
{
}

std::string ND91Assistant::UninstallSystemAppCommand::GetAndroidBodyData()
{
    BinaryBufferWriter writer;

	GetNdShStateCommand ndshCmd(_pDevice);
	ndshCmd.Execute();
	if(ndshCmd.GetHasNdsh())
	{
		if(!ndshCmd.GetSuAuth())
		{
			//通知界面需要客户去手机上确认权限提升。
			SendPrompt(MESSAGE_NEED_SU_AUTH_IN_DEVICE);
			//取得授权
			GetNdShAuthCommand  getAuthCmd(_pDevice);
			getAuthCmd.Execute();
			if(getAuthCmd.GetResult())
			{
				_nAndroidChannelID = APPLICATION;
				_nAndroidCommandID =  UNINSTALL_SYSTEMAPP_BYNDSH;
			}
			//通知界面开始卸载
			SendPrompt(MESSAGE_INSTALL_UNINSTALL);
		}
		else
		{
			_nAndroidChannelID = APPLICATION;
			_nAndroidCommandID =  UNINSTALL_SYSTEMAPP_BYNDSH;
		}
	}
	
    writer.WriteInt(_nAndroidChannelID);
   
	if(_nAndroidCommandID == UNINSTALL_SYSTEMAPP_BYNDSH)
	{
		char id=21;
		writer.WriteChar(id);
		writer.WriteString( CCodeOperation::UnicodeToUTF_8(_appData->_packageName) );
	}
	else
	{
		 writer.WriteInt(_nAndroidCommandID);
		writer.WriteString( CCodeOperation::UnicodeToUTF_8(_appData->_strAppPhonePath) );
	}
    return writer.GetBuffer(); 
}

void ND91Assistant::UninstallSystemAppCommand::SetAndroidReturnData(std::string data)
{
    BinaryBufferReader reader(data.c_str(), data.length());
    if (!parseAndroidPackageHead_Top16(&reader))
        return;
    reader.ReadBool(_bSuccess);
}

void ND91Assistant::UninstallSystemAppCommand::Execute()
{
	if (!_pDevice || !_pDevice->GetDevInfo())
		return;	
		DefaultExecute();
}

#endif