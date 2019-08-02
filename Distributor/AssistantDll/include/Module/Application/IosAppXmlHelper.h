#pragma once

#include "Module/Application/AppData.h"
#include "Common/rapidxml/rapidxml.hpp"

#include <vector>
using namespace std;
class IosPlistHelper;

namespace ND91Assistant
{
/*#define XML_PI			L"xml version=\"1.0\" encoding=\"UTF-8\""
#define XML_DOCTYPE		L"plist PUBLIC \"-//Apple Computer//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\""
#define XML_KEY			L"key"
#define XML_STRING		L"string"
#define XML_ARRAY		L"array"
#define XML_DICT		L"dict"
#define XML_FALSE		L"false"
#define XML_TRUE		L"true"*/
	//操作ISO设备APP中使用的XML文件类
	class IIosFileSystem;
	class CLASSINDLL_CLASS_DECL IosAppXmlHelper
	{
	public:
		IosAppXmlHelper(void);
		~IosAppXmlHelper(void);

		// Summary  : 获取卸载PXL的命令文件
		// Returns  : std::wstring ：命令文件内容
		// Parameter: wstring strIdentifier ：PXL程序的唯一标识
		static wstring GetPXLUnstallCmdFileContent(wstring strIdentifier);

		// Summary  : 获取安装PXL的命令文件
		// Returns  : std::wstring ：命令文件内容
		static wstring GetPXLInstallCmdFileContent();

		// Summary  : PXL查找XML元素数据
		// Returns  : std::wstring ：元素数据
		// Parameter: IosPlistHelper * plistHelper ：XML操作指针
		// Parameter: string strKey ：key的名称
		static wstring PxlSearchKeyFromXML(IosPlistHelper* plistHelper, string strKey);

		// Summary  : 创建存储PXL程序信息的PLIST文件
		// Returns  : void
		// Parameter: wstring strLocalAppPath ：程序文件所在文件夹
		// Parameter: wstring strPackageName ：程序名
		// Parameter: wstring strPostflightName ：.Postflight文件名
		// Parameter: wstring strPreremoveName ：.Preremove文件名
		// Parameter: wstring strVersion ：程序版本
		static void CreatePxlPlistFile(wstring strLocalAppPath, wstring strAppName, wstring strPostflightName, 
			wstring strPreremoveName, wstring strVersion);

		// Summary  : 通过PLIST文件查找PXL程序的安装目录
		// Returns  : std::wstring ：返回安装目录
		// Parameter: IosPlistHelper * plistHelper ：plist文件
		static wstring PxlSearchAppPath(IosPlistHelper* plistHelper);

		// Summary  : 删除IPA程序信息，用于卸载
		// Returns  : bool
		// Parameter: string strXmlFile	：存放全部程序的信息
		// Parameter: wstring strIdentifier ：程序的唯一标识
		// Parameter: wstring & strAppPathOnPhone ：程序的路径，用于删除程序的配置文件等
		static bool DeleteIpaAppInfo(wstring strXmlFile, wstring strIdentifier, wstring &strAppPathOnPhone);

		// Summary  : 解析Ipa全部程序信息
		// Returns  : void
		// Parameter: wstring destinationOnComputer ：信息存放的文件名（含路径，在PC上）
		// Parameter: vector<AppData * > & vtInfos ：用于存放程序信息
		// Parameter: pFileStream ：文件操作接口
		static void PraseIpaAppInfos(wstring strXmlFile, vector<AppData*> &vtInfos, IIosFileSystem* pFileStream);

		// Summary  : 解析PXL程序信息
		// Returns  : void
		// Parameter: wstring destinationOnComputer	：PXL信息文件名（含路径，PC上）
		// Parameter: vector<AppData * > & vtInfos	：用于存储PXL程序信息
		// Parameter: pFileStream ：文件操作接口
		static void PrasePxlAppInfo(wstring destinationOnComputer, vector<AppData*> &vtInfos, IIosFileSystem* pFileStream);


		// Summary  : 从文件中获取购买者名称
		// Returns  : std::wstring 返回购买者名称
		// Parameter: wstring strXmlFileOnPC ：存储信息的XML文件
		static wstring GetPurchaser(wstring strXmlFileOnPC);

	private:

		// Summary  : 解析单个Ipa程序信息
		// Returns  : void
		// Parameter: rapidxml::xml_node<char>* node ：程序信息XML内容
		// Parameter: AppData * pAppData ：用于存储程序信息
		static void PraseIpaInfoFromXmlNode(rapidxml::xml_node<char>* node, AppData* pAppData, IIosFileSystem* pFileStream);
	};
}
