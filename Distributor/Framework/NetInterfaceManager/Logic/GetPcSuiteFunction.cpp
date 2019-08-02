#include "StdAfx.h"
#include "GetPcSuiteFunction.h"
#include "Common/cJSON.h"
#include "rapidxml/rapidxml.hpp"
#include <algorithm>
#include <functional>

using namespace rapidxml;

const wstring g_AndroidPcSuiteFunctionPath = L"http://pcfirm.sj.91.com/Controller.ashx?Action=getpcsuitefunctionconfig&version=0&platform=android";
const wstring g_IOSPcSuiteFunctionPath = L"http://pcfirm.sj.91.com/Controller.ashx?Action=getpcsuitefunctionconfig&version=0&platform=iphone";

GetPcSuiteFunction::GetPcSuiteFunction(void)
{
	this->GetSystemName();
}

GetPcSuiteFunction::~GetPcSuiteFunction(void)
{
}

void GetPcSuiteFunction::GetPcSuiteFunc(vector<FunctionItem>& And_STPcSuitFunc, vector<FunctionItem>& Ios_STPcSuitFunc)
{
	wstring wsReturnData = this->Request(g_AndroidPcSuiteFunctionPath);
	this->AnalyzeXml(wsReturnData, And_STPcSuitFunc);

	wsReturnData = this->Request(g_IOSPcSuiteFunctionPath);
	this->AnalyzeXml(wsReturnData, Ios_STPcSuitFunc);

	this->CheckAndDownLoadIcon(And_STPcSuitFunc, Ios_STPcSuitFunc);

	return;
}

wstring GetPcSuiteFunction::Request(wstring wsUrl)
{
	string  strData=CNetOperation::Get(CCodeOperation::WstringToString(wsUrl));

	return CCodeOperation::UTF_8ToUnicode(strData);
}

/// 解析xml
bool GetPcSuiteFunction::AnalyzeXml(wstring wsReturnData, vector<FunctionItem>& PcSuitFunction)
{
	xml_document<wchar_t> doc;
	doc.parse<0>((wchar_t*)wsReturnData.c_str());
	rapidxml::xml_node<wchar_t>* root = doc.first_node();            //xml文件的根节点
	RETURN_FALSE_IF (!root);
	xml_attribute<wchar_t> *attribute = root->first_attribute();
	RETURN_FALSE_IF (!attribute);

	for (rapidxml::xml_node<wchar_t>* node = root->first_node(); node!=NULL; node = node->next_sibling())
	{
		if ( !node )
			continue;
		FunctionItem funItem;

		for (xml_attribute<wchar_t> *attribute = node->first_attribute(); attribute; attribute = attribute->next_attribute())
		{
			if ( !attribute )
				continue;
			
			wstring str=CStrOperation::toLowerW(attribute->name());
			if (L"name"==str)
			{
				funItem.st_Name = attribute->value();
			}
			else if (L"icon"==str)
			{
				funItem.st_IconDownPath = attribute->value();
#if _DEBUG
	funItem.st_IconDownPath =  L"http://image.sjpic.91rb.com/soft/2011/lunbo/RootLogo.png";
#endif
			} 
			else if (L"value"==str)
			{
				funItem.st_ID = attribute->value();
			} 
			else if (L"fixtype"==str)
			{
				funItem.st_SuitPCPlate = _wtoi(attribute->value());
			}
			else if (L"exepath"==str)
			{
				funItem.st_wsExeBinPath = attribute->value();
			}
			else if (L"filesize"==str)
			{
				funItem.st_FileSize = attribute->value();
			}
			else if (L"desc"==str)
			{
				funItem.st_Desc = attribute->value();
			}
			else if (L"fileversion"==str)
			{
				funItem.st_FileVersion = attribute->value();
			}
		}

		for (rapidxml::xml_node<wchar_t>* listnode = node->first_node(); listnode!=NULL; listnode = listnode->next_sibling())
		{
			if (!listnode)
			{
				continue;
			}
			funItem.st_InstallPkDownPath = listnode->value();
		}
		if (funItem.st_IconDownPath.empty() || funItem.st_InstallPkDownPath.empty())
		{
			continue;
		}

		//比较当前功能是否是否该PC系统下运行
		if (0 == funItem.st_SuitPCPlate)
		{
			PcSuitFunction.push_back(funItem);
		}
		else
		{
			if (_T("Microsoft Windows 7") == _wsOsname && 2 == funItem.st_SuitPCPlate)
			{
				PcSuitFunction.push_back(funItem);
			}
			else if (_T("Microsoft Windows XP") == _wsOsname && 1 == funItem.st_SuitPCPlate)
			{
				PcSuitFunction.push_back(funItem);
			}
		}

	}

	return true;
}

//读取操作系统的名称   
void GetPcSuiteFunction::GetSystemName()  
{  
	SYSTEM_INFO info;        //用SYSTEM_INFO结构判断64位AMD处理器    
	GetSystemInfo(&info);    //调用GetSystemInfo函数填充结构    
	OSVERSIONINFOEX os;   
	os.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);     

	_wsOsname = _T("unknown OperatingSystem.");  

	if(GetVersionEx((OSVERSIONINFO *)&os))  
	{   
		//下面根据版本信息判断操作系统名称    
		switch(os.dwMajorVersion)//判断主版本号   
		{  
		case 4:  
			switch(os.dwMinorVersion)//判断次版本号    
			{   
			case 0:  
				if(os.dwPlatformId==VER_PLATFORM_WIN32_NT)  
					_wsOsname =_T("Microsoft Windows NT 4.0"); //1996年7月发布    
				else if(os.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS)  
					_wsOsname =_T("Microsoft Windows 95");  
				break;  
			case 10:  
				_wsOsname =_T("Microsoft Windows 98");  
				break;  
			case 90:  
				_wsOsname =_T("Microsoft Windows Me");  
				break;  
			}  
			break;  

		case 5:  
			switch(os.dwMinorVersion)   //再比较dwMinorVersion的值   
			{   
			case 0:  
				_wsOsname =_T("Microsoft Windows 2000");//1999年12月发布   
				break;  

			case 1:  
				_wsOsname =_T("Microsoft Windows XP");//2001年8月发布   
				break;  

			case 2:  
				if(os.wProductType==VER_NT_WORKSTATION   
					&& info.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64)  
				{  
					_wsOsname =_T("Microsoft Windows XP Professional x64 Edition");  
				}  
				else if(GetSystemMetrics(SM_SERVERR2)==0)  
					_wsOsname =_T("Microsoft Windows Server 2003");//2003年3月发布    
				else if(GetSystemMetrics(SM_SERVERR2)!=0)  
					_wsOsname =_T("Microsoft Windows Server 2003 R2");  
				break;  
			}  
			break;  

		case 6:  
			switch(os.dwMinorVersion)  
			{  
			case 0:  
				if(os.wProductType == VER_NT_WORKSTATION)  
					_wsOsname =_T("Microsoft Windows Vista");  
				else  
					_wsOsname =_T("Microsoft Windows Server 2008");//服务器版本    
				break;  
			case 1:  
				if(os.wProductType == VER_NT_WORKSTATION)  
					_wsOsname =_T("Microsoft Windows 7");  
				else  
					_wsOsname =_T("Microsoft Windows Server 2008 R2");  
				break;  
			}  
			break;  
		}  
	}
}   

//add by zjf 优化代码
struct CheckDownloadFile
	: public binary_function<FunctionItem, wstring, void>
{
	void operator()(FunctionItem& velue, wstring wstrPlat) const
	{
		wstring SavePath = DataPath::GetDataDir();
		SavePath  = SavePath + L"\\icon\\";
		SavePath += wstrPlat;
		SavePath += CFileOperation::GetFileNameNoExt(velue.st_IconDownPath);
		SavePath += _T(".png");
	
		if(!CFileOperation::IsFileExist(SavePath.c_str()))
		{
			CNetOperation::DownloadFile(CCodeOperation::UnicodeToGB2312(velue.st_IconDownPath), SavePath);
		}
	
		velue.st_IconSavePath = SavePath;
	}
};
void GetPcSuiteFunction::CheckAndDownLoadIcon(vector<FunctionItem>& And_STPcSuitFunc, vector<FunctionItem>& Ios_STPcSuitFunc)
{
	for_each(And_STPcSuitFunc.begin(), And_STPcSuitFunc.end(), bind2nd(CheckDownloadFile(), L"Android"));

	for_each(Ios_STPcSuitFunc.begin(), Ios_STPcSuitFunc.end(), bind2nd(CheckDownloadFile(), L"IOS"));

	return;
}