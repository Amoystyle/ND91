#include "pch_module.h"

#include "CreateMomeCommand.h"

#include "Device/IosDaemon.h"
#include "Common/NDBase64.h"
#include "Common/Path.h"

CreateMomeCommand::CreateMomeCommand(const DeviceData* pDevice)
:BaseCommand(pDevice)
{
	_strIosCmdFileName=L"mome."+CFileOperation::GetGuidW();
}

CreateMomeCommand::~CreateMomeCommand(void)
{
}

void ND91Assistant::CreateMomeCommand::Execute()
{
	_success = false;
	DefaultExecute();
}

void ND91Assistant::CreateMomeCommand::SetIosReturnFile( std::wstring filename )
{
	if(_momeCmd == "ThemeDataPath")
	{
		_ios_local_resultFile = filename;
		return;
	}
	if(!filename.empty())
	{
		IosPlistHelper xmlhelp(filename, eXmlType_UTF8);
		XML_NODE_INDEX index;
		index.AddChild(0);
		rapidxml::xml_node<char>* node = xmlhelp.SearchKeyNode(index,"Error","integer");
		string result;
		if(node)
			result=node->value();
		else
		{
			node = xmlhelp.SearchKeyNode(index,"installed","integer");
			if(node)
			{
				result=node->value();
				if(!result.empty() && atoi(result.c_str())==2)
					_success = true;
			}
			else
			{
				node = xmlhelp.SearchKeyNode(index,"ThemeApp","true");
				if(node)
				{
					_success = true;
				}
			}
		}

		CFileOperation::DeleteFileW(filename);
	}

}

std::wstring ND91Assistant::CreateMomeCommand::GetIosCommandFilePath()
{
	IosDaemon daemon(NULL, L"");

	wstring strFileName_OnPhone = daemon.CreatDaemonCypher(_strIosCmdFileName, 
		GetDeviceInfo()->_ios_devicebTurnCypher, 
		GetDeviceInfo()->_ios_deviceszDaemonCypherBytes);

	wstring cmdfile=Path::IosDeviceCommandPath() + strFileName_OnPhone;

	return cmdfile;
}

std::wstring ND91Assistant::CreateMomeCommand::GetIosCommandFileContent()
{
	unsigned char assignKey[] = {0x77,0x09,0xac,0x65,0x33,0xec,0xb8,0x01};
	string desstr=CStrOperation::EncryptDesEBC(_momeCmd,assignKey);
	int len=CNDBase64::Base64encode_len(desstr.length()); 
	char* cmd64 = new char[len+1];
	CNDBase64::Base64encode(cmd64,desstr.c_str(),desstr.length());
	string str= cmd64;
	SAFE_DELETE(cmd64);
	return CCodeOperation::UTF_8ToUnicode(str);
}

int ND91Assistant::CreateMomeCommand::CheckPHInstalled()
{
	int resultcode=CreateMomeError();
	if(resultcode<-1) return resultcode;
	_momeCmd = "PHInstalled";
	Execute();
	if(_success) resultcode = 0;
	return resultcode;
}


int ND91Assistant::CreateMomeCommand::SetPCTheme( wstring themepathOnPhone )
{
	int resultcode=CreateMomeError();
	if(resultcode<-1) return resultcode;
	_momeCmd = "PCTheme,"+CCodeOperation::UnicodeToUTF_8(themepathOnPhone);
	Execute();
	if(_success) resultcode = 0;
	return resultcode;
}

int ND91Assistant::CreateMomeCommand::CreateMomeError()
{
	if(! _pDevice->GetDevInfo()->_ios_bJailBreaked)
		return -3;
	if(! _pDevice->GetDevInfo()->_ios_daemonInstalled )
		return -4;
	if(_pDevice->GetDevInfo()->_deviceProductVersion<L"300")
		return -5;
	if(!DirectoryOperationCommand(_pDevice).Exist(L"/private/var/root/Library/NetDragon/ndDaemon/mome"))
		return -6;
	return -1;
}

int ND91Assistant::CreateMomeCommand::GetThemeDataPath(map<wstring, wstring>& themepathname)
{
	int resultcode=CreateMomeError();
	if(resultcode<-1) return resultcode;
	_momeCmd = "ThemeDataPath";
	Execute();
	if(!_ios_local_resultFile.empty())
	{
		IosPlistHelper xmlhelp(_ios_local_resultFile, eXmlType_UTF8);
		XML_NODE_INDEX index;
		index.AddChild(0);
		vector<wstring> vtValues;
		IosPlistHelper::GetValuesFromNode(xmlhelp.GetFirstNode()->first_node("dict"), vtValues);
		
		for (size_t i = 0; i<vtValues.size();i++)
		{
			wstring themepath = vtValues[i];
			themepathname[vtValues[i]]=vtValues[i+1];
			/*DownloadFileCommand downcmd(_pDevice);
			wstring localpath = _pDevice->GetPath()->GetPThemePath()->GetThemeTempPath()+L"theme.plist";
			if(downcmd.Download(localpath,vtValues[i+1]))
			{
				string strData = CFileOperation::ReadFile(localpath, false);
				xml_document<char> doc;
				doc.parse<0>(const_cast<char *>(strData.c_str()));
				xml_node<char> * themeinfo = doc.first_node()->first_node("array");
				if(themeinfo)
				{
					xml_node<char> * themenode = themeinfo->first_node("string");
					while(themenode)
					{
						string themename=themenode->value();
						themepathname[CCodeOperation::UTF_8ToUnicode(themename)]=themepath;
						themenode = themenode->next_sibling();
					}
				}
				CFileOperation::DeleteFileW(localpath);
			}*/
			i++;
		}
		_success = true;
		CFileOperation::DeleteFileW(_ios_local_resultFile);
		_ios_local_resultFile=L"";
	}
	if(_success) resultcode = 0;
	return resultcode;
}