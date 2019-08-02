#pragma once

#include "IIosFileSystem.h"

#define Ios_Global_Preferences_File L"/private/var/root/Library/Preferences/.GlobalPreferences.plist"

using namespace std;

namespace ND91Assistant
{

	typedef enum eLocaleType
	{
		eLocaleType_EN, //英文
		eLocaleType_ZH, //中文简体
		eLocaleType_TW, //中文繁体
	};

	class IosPublicFunc
	{
	public:
		IosPublicFunc(void);
		~IosPublicFunc(void);

		//获取指定类型信息，string编码均为GB2312
		//pFileStream		:		文件操作指针
		//strFileOnPhone	:	信息存放的手机端完整文件名
		//strType			:	信息类型名
		//bNeedChangeToXML	:	是否需要转化成XML
		//返回				:	获取的指定类型信息
		static wstring GetTypeValue(IIosFileSystem* pFileStream, wstring strFileOnPhone, wstring strType, bool bNeedChangeToXML = false);

		static int GetVerNum(wstring strProductVersion);	

		static bool SendMessageToService(void* pSocket, void* pCFData);

		static void* ReadMssageFromService(void* pSocket);

		static wstring CheckPathByVersion(wstring path, int nProductVer, bool bJailBreaked);

		static wstring RenameDir(const wstring localName, IIosFileSystem* pFileSteam);

		static wstring RenameFile(const wstring localName, IIosFileSystem* pFileSteam);

		static eLocaleType GetCurrentLang(IIosFileSystem* pFileStream);

		// Summary  : 取设备的字符集
		// Returns  : 字符集类型
		// Parameter: IIosFileSystem * pFileStream 访问设备的文件访问类
		// Parameter: wstring tempFolder 临时文件存放目录(以\结尾)
		static eLocaleType GetCurrentLang(IIosFileSystem* pFileStream, wstring tempFolder);

		static bool DeleteFolder(const wstring strFolder, IIosFileSystem* pFileStream);
		
		// 获取PC电脑上itunes的dll的真正路径
		static wstring GetItunesDllRealPath(wstring strRegPath, wstring strDllName);

	};
}
