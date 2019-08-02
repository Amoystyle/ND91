#include "pch_module.h"

#ifdef MODULE_SCREENSHOT

#include "GetScreenImageInfoCommand.h"
#include "ScreenImageData.h"

#include "Device/AdbScreenShotCommand.h"
#include "Module/AndroidDefine.h"

GetScreenImageInfoCommand::GetScreenImageInfoCommand(const DeviceData* pDevice)
:BaseCommand(pDevice)
{
	_nAndroidChannelID = SHELL;
}

GetScreenImageInfoCommand::~GetScreenImageInfoCommand(void)
{
}

void ND91Assistant::GetScreenImageInfoCommand::Execute()
{
	if(_pDevice->GetDevInfo()->_connectType == WIFI)
		DefaultExecute();
	else if (_pDevice->GetDevInfo()->_deviceType == Android)
	{
		AdbScreenShotCommand adbcmd(_pDevice->GetDevInfo()->_strSerialNumber,_pDevice->GetDevInfo()->_ios_deviceIdentifier);
		int bpp=0;
		string result= adbcmd.GetScreenImageInfoData(bpp);
		BinaryBufferReader reader(result.c_str(), result.length());
		if (_pScreenImage)
		{
			if(result.length()>12)
			{
				_pScreenImage->ParseAndroidData(reader,true);
				if(_pDevice->GetDevInfo()->_deviceHWModel == L"MT15i" || _pDevice->GetDevInfo()->_deviceHWModel == L"LT15i")
				{
					_pScreenImage->_ScreenInfo._red_offset = 0;
					_pScreenImage->_ScreenInfo._red_length = 8;
					_pScreenImage->_ScreenInfo._blue_offset = 16;
					_pScreenImage->_ScreenInfo._blue_length = 8;
					_pScreenImage->_ScreenInfo._green_offset = 8;
					_pScreenImage->_ScreenInfo._green_length = 8;
					_pScreenImage->_ScreenInfo._alpha_offset = 24;
					_pScreenImage->_ScreenInfo._alpha_length = 8;
				}
			}
			else
			{
				_pScreenImage->_ScreenInfo._bpp = bpp;
				int  size;
				reader.ReadInt32(size);
				reader.ReadInt32(_pScreenImage->_ScreenInfo._width);
				reader.ReadInt32(_pScreenImage->_ScreenInfo._height);
			}
		}
	}

}

std::string ND91Assistant::GetScreenImageInfoCommand::GetAndroidBodyData()
{
	BinaryBufferWriter  bufwriter;
	bufwriter.WriteInt(_nAndroidChannelID);
	bufwriter.WriteInt(3);
    return bufwriter.GetBuffer();
}

void ND91Assistant::GetScreenImageInfoCommand::SetAndroidReturnData( std::string data )
{
	BinaryBufferReader reader(data.c_str(), data.length());
	if(!parseAndroidPackageHead(&reader))
		return;
	string okay;
	reader.ReadString(okay,4);
	if(okay == "OKAY")
	{
		if (_pScreenImage)
		{
			_pScreenImage->ParseAndroidData(reader,false);
		}
	}
	
}

bool ND91Assistant::GetScreenImageInfoCommand::parseAndroidPackageHead( BinaryBufferReader* pReader )
{
	pReader->ReadBuffer(_pReturnHead, 4);
	pReader->ReadShort(_nReturnFlag);
	pReader->ReadShort(_nReturnLongConnectSign);
	pReader->ReadInt32(_nReturnBodySize);
	pReader->ReadInt32(_nReturnChannelID);
	if(_nReturnChannelID != _nAndroidChannelID)
		return false;
	return true;
}

void ND91Assistant::GetScreenImageInfoCommand::SetParam( ScreenImageData* pScreenImage )
{
	_pScreenImage  = pScreenImage;
}


#endif