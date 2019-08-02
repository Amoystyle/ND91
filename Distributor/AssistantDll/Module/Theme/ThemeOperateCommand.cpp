#include "pch_module.h"

#ifdef MODULE_THEME

#include "ThemeOperateCommand.h"
#include "AndroidThemeXmlHelper.h"
#include "Module/AndroidDefine.h"

ThemeOperateCommand::ThemeOperateCommand(const DeviceData* pDevice)
:BaseCommand(pDevice)
{
	_nAndroidChannelID = THEME;
}

ThemeOperateCommand::~ThemeOperateCommand(void)
{
}

void ND91Assistant::ThemeOperateCommand::Execute()
{
	_success = false;
	DefaultExecute();
}

bool ND91Assistant::ThemeOperateCommand::parseAndroidPackageHead( BinaryBufferReader* pReader )
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

std::string ND91Assistant::ThemeOperateCommand::GetAndroidBodyData()
{
	BinaryBufferWriter  bufwriter;

	bufwriter.WriteInt(_nAndroidChannelID);
	if(_nAndroidChannelID==SMARTTHEME&& _type==4)
	{
		bufwriter.WriteChar(4);
		bufwriter.WriteStringAsUTF8(L"/sdcard/smarthomethemeinfo.xml");
		return bufwriter.GetBuffer();
	}
	bufwriter.WriteChar(_type);
	switch(_type)
	{
		case ESetTheme:
		case EDeleteTheme:
		case EGetThemeDetailInfo:
		case EExportThemePkg:
			bufwriter.WriteStringAsUTF8(_themeId);
			break;
		case EAddTheme:
		case EAddThemeAndSetAsCurTheme:
		case EGetThemeInfos:
		case EGetThemeBriefInfo:
			bufwriter.WriteStringAsUTF8(_path);
			break;
		case EInitUpdateTheme:
			break;
		default:
			break;
	}
	return bufwriter.GetBuffer();
}

void ND91Assistant::ThemeOperateCommand::SetAndroidReturnData( std::string data )
{
	BinaryBufferReader  reader(data.c_str(), data.length());
	if(!parseAndroidPackageHead(&reader))
		return;
	int flag = 0;

	reader.ReadInt32(flag);

	_success = flag == 1?true:false;
}

bool ND91Assistant::ThemeOperateCommand::SetTheme( wstring themeID, bool IsSmart)
{
	if(IsSmart)
		_nAndroidChannelID = SMARTTHEME;
	else
		_nAndroidChannelID=THEME;
	_type = ESetTheme;
	_themeId = themeID;
	Execute();
	return _success;
}

bool ND91Assistant::ThemeOperateCommand::DelTheme( wstring themeID,bool IsSmart)
{
	if(IsSmart)
		_nAndroidChannelID = SMARTTHEME;
	else
		_nAndroidChannelID=THEME;
	_type = EDeleteTheme;
	_themeId = themeID;
	if(IsSmart)
		_themeId =CCodeOperation::UTF_8ToUnicode(AndroidThemeXmlHelper::urldecode(CCodeOperation::UnicodeToUTF_8(themeID)));
	Execute();
	return _success;
}

bool ND91Assistant::ThemeOperateCommand::AddTheme( wstring aptRemotePath,bool IsSmart)
{
	if(IsSmart)
		_nAndroidChannelID = SMARTTHEME;
	else
		_nAndroidChannelID=THEME;
	_type = EAddTheme;
	_path = aptRemotePath;
	Execute();
	return _success;
}

bool ND91Assistant::ThemeOperateCommand::InitUpdateThemes()
{
	_nAndroidChannelID=THEME;
	_type = EInitUpdateTheme;
	Execute();
	return _success;
}

bool ND91Assistant::ThemeOperateCommand::GetThemeBriefInfo()
{
	_nAndroidChannelID=THEME;
	_type = EGetThemeBriefInfo;
	_path = L"/sdcard/nd/pandahome/themeinfo/themebriefinfo.xml";
	Execute();
	return _success;
}

bool ND91Assistant::ThemeOperateCommand::GetThemeDetailInfo( wstring themeID )
{
	_nAndroidChannelID=THEME;
	_type = EGetThemeDetailInfo;
	_themeId = themeID;
	Execute();
	return _success;
}

bool ND91Assistant::ThemeOperateCommand::AddThemeAndSetCurrentTheme( wstring aptRemotePath )
{
	_nAndroidChannelID=THEME;
	_type = EAddThemeAndSetAsCurTheme;
	_path = aptRemotePath;
	Execute();
	return _success;
}

bool ND91Assistant::ThemeOperateCommand::GetThemeInfos( wstring path )
{
	_nAndroidChannelID=THEME;
	_type = EGetThemeInfos;
	_path = path;
	Execute();
	return _success;
}

bool ND91Assistant::ThemeOperateCommand::ExportThemePackage( wstring themeID )
{
	_nAndroidChannelID=THEME;
	_type = EExportThemePkg;
	_themeId = themeID;
	Execute();
	return _success;
}

bool ND91Assistant::ThemeOperateCommand::GetSmartThemeXml()
{
	_nAndroidChannelID = SMARTTHEME;
	_type = EAddThemeAndSetAsCurTheme;
	Execute();
	_nAndroidChannelID = THEME;
	return _success;

}

#endif