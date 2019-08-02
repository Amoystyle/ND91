#pragma once

#include "IIosFileSystem.h"

#define Ios_Global_Preferences_File L"/private/var/root/Library/Preferences/.GlobalPreferences.plist"

using namespace std;

namespace ND91Assistant
{

	typedef enum eLocaleType
	{
		eLocaleType_EN, //Ӣ��
		eLocaleType_ZH, //���ļ���
		eLocaleType_TW, //���ķ���
	};

	class IosPublicFunc
	{
	public:
		IosPublicFunc(void);
		~IosPublicFunc(void);

		//��ȡָ��������Ϣ��string�����ΪGB2312
		//pFileStream		:		�ļ�����ָ��
		//strFileOnPhone	:	��Ϣ��ŵ��ֻ��������ļ���
		//strType			:	��Ϣ������
		//bNeedChangeToXML	:	�Ƿ���Ҫת����XML
		//����				:	��ȡ��ָ��������Ϣ
		static wstring GetTypeValue(IIosFileSystem* pFileStream, wstring strFileOnPhone, wstring strType, bool bNeedChangeToXML = false);

		static int GetVerNum(wstring strProductVersion);	

		static bool SendMessageToService(void* pSocket, void* pCFData);

		static void* ReadMssageFromService(void* pSocket);

		static wstring CheckPathByVersion(wstring path, int nProductVer, bool bJailBreaked);

		static wstring RenameDir(const wstring localName, IIosFileSystem* pFileSteam);

		static wstring RenameFile(const wstring localName, IIosFileSystem* pFileSteam);

		static eLocaleType GetCurrentLang(IIosFileSystem* pFileStream);

		// Summary  : ȡ�豸���ַ���
		// Returns  : �ַ�������
		// Parameter: IIosFileSystem * pFileStream �����豸���ļ�������
		// Parameter: wstring tempFolder ��ʱ�ļ����Ŀ¼(��\��β)
		static eLocaleType GetCurrentLang(IIosFileSystem* pFileStream, wstring tempFolder);

		static bool DeleteFolder(const wstring strFolder, IIosFileSystem* pFileStream);
		
		// ��ȡPC������itunes��dll������·��
		static wstring GetItunesDllRealPath(wstring strRegPath, wstring strDllName);

	};
}
