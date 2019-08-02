#include "stdafx.h"

#ifdef MODULE_SYSTEM

#include "GetIMEICommand.h"

#include "Core/DeviceData.h"
#include "Device/IosFileIO.h"
#include "Module/AndroidDefine.h"

GetIMEICommand::GetIMEICommand(const DeviceData* pDevice)
: BaseCommand(pDevice)
{
    _nAndroidChannelID = SYSINFO;
    _nAndroidCommandID = GET_IMEI;
	_strIosCmdFileName = L"imei." + CFileOperation::GetGuidW();
	_strIMEI = L"";
}

GetIMEICommand::~GetIMEICommand()
{
}

std::string ND91Assistant::GetIMEICommand::GetAndroidBodyData()
{
    BinaryBufferWriter writer;

    writer.WriteInt(_nAndroidChannelID);
    writer.WriteInt(_nAndroidCommandID);
    writer.WriteString("");
    return writer.GetBuffer();
}

void ND91Assistant::GetIMEICommand::SetAndroidReturnData(std::string data)
{
    LOG->WriteDebug(L"GetIMEICommand::SendAndroidDatagram binary:");
    LOG_BINARY(Log::LOG_DEBUG, data);

    LOG_DEBUG(L"Parse IMEI");
    BinaryBufferReader reader(data.c_str(), data.length());
    RETURN_IF(!parseAndroidPackageHead(&reader));
    reader.ReadStringFromUTF8(_strIMEI);
    LOG_DEBUG(L"IMEI is:"+_strIMEI);
}

void ND91Assistant::GetIMEICommand::Execute()
{
	if (!_pDevice || !_pDevice->GetDevInfo())
		return ;

	switch( _pDevice->GetDevInfo()->_deviceType )
	{
	case Android:		
		if (_pDevice->GetAndroidDeamonMode() == 0) // 无守护模式
			break;
		SetAndroidReturnData(GetIO()->SendAndroidDatagram(GetAndroidDatagram()));
		break;

	case Ios:
		{
			if (_pDevice->GetIosDeamonMode() == 0) // 无守护模式
				break;
			IosFileIO* pFileIO = dynamic_cast<IosFileIO*>(GetIO());
			if ( ! pFileIO )
			{
				THROW_ASSISTANT_EXCEPTION(INVALID_IO_OBJECT);
			}

			SetIosReturnFile(pFileIO->SendIosFile(GetIosCommandFilePath(), GetIosResultFilePath(), GetIosCommandFileContent(), false));
		}
		break;

	default:
		THROW_ASSISTANT_EXCEPTION(UNKNOWN_DEVICE_OR_CONNECTION);
		break;
	}
}

// 解析返回文件，解析后删除文件
void ND91Assistant::GetIMEICommand::SetIosReturnFile(std::wstring filename)
{
	_strIMEI = GetIosDoneInfoFromFileName();
}



#endif