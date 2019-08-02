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
	//����ISO�豸APP��ʹ�õ�XML�ļ���
	class IIosFileSystem;
	class CLASSINDLL_CLASS_DECL IosAppXmlHelper
	{
	public:
		IosAppXmlHelper(void);
		~IosAppXmlHelper(void);

		// Summary  : ��ȡж��PXL�������ļ�
		// Returns  : std::wstring �������ļ�����
		// Parameter: wstring strIdentifier ��PXL�����Ψһ��ʶ
		static wstring GetPXLUnstallCmdFileContent(wstring strIdentifier);

		// Summary  : ��ȡ��װPXL�������ļ�
		// Returns  : std::wstring �������ļ�����
		static wstring GetPXLInstallCmdFileContent();

		// Summary  : PXL����XMLԪ������
		// Returns  : std::wstring ��Ԫ������
		// Parameter: IosPlistHelper * plistHelper ��XML����ָ��
		// Parameter: string strKey ��key������
		static wstring PxlSearchKeyFromXML(IosPlistHelper* plistHelper, string strKey);

		// Summary  : �����洢PXL������Ϣ��PLIST�ļ�
		// Returns  : void
		// Parameter: wstring strLocalAppPath �������ļ������ļ���
		// Parameter: wstring strPackageName ��������
		// Parameter: wstring strPostflightName ��.Postflight�ļ���
		// Parameter: wstring strPreremoveName ��.Preremove�ļ���
		// Parameter: wstring strVersion ������汾
		static void CreatePxlPlistFile(wstring strLocalAppPath, wstring strAppName, wstring strPostflightName, 
			wstring strPreremoveName, wstring strVersion);

		// Summary  : ͨ��PLIST�ļ�����PXL����İ�װĿ¼
		// Returns  : std::wstring �����ذ�װĿ¼
		// Parameter: IosPlistHelper * plistHelper ��plist�ļ�
		static wstring PxlSearchAppPath(IosPlistHelper* plistHelper);

		// Summary  : ɾ��IPA������Ϣ������ж��
		// Returns  : bool
		// Parameter: string strXmlFile	�����ȫ���������Ϣ
		// Parameter: wstring strIdentifier �������Ψһ��ʶ
		// Parameter: wstring & strAppPathOnPhone �������·��������ɾ������������ļ���
		static bool DeleteIpaAppInfo(wstring strXmlFile, wstring strIdentifier, wstring &strAppPathOnPhone);

		// Summary  : ����Ipaȫ��������Ϣ
		// Returns  : void
		// Parameter: wstring destinationOnComputer ����Ϣ��ŵ��ļ�������·������PC�ϣ�
		// Parameter: vector<AppData * > & vtInfos �����ڴ�ų�����Ϣ
		// Parameter: pFileStream ���ļ������ӿ�
		static void PraseIpaAppInfos(wstring strXmlFile, vector<AppData*> &vtInfos, IIosFileSystem* pFileStream);

		// Summary  : ����PXL������Ϣ
		// Returns  : void
		// Parameter: wstring destinationOnComputer	��PXL��Ϣ�ļ�������·����PC�ϣ�
		// Parameter: vector<AppData * > & vtInfos	�����ڴ洢PXL������Ϣ
		// Parameter: pFileStream ���ļ������ӿ�
		static void PrasePxlAppInfo(wstring destinationOnComputer, vector<AppData*> &vtInfos, IIosFileSystem* pFileStream);


		// Summary  : ���ļ��л�ȡ����������
		// Returns  : std::wstring ���ع���������
		// Parameter: wstring strXmlFileOnPC ���洢��Ϣ��XML�ļ�
		static wstring GetPurchaser(wstring strXmlFileOnPC);

	private:

		// Summary  : ��������Ipa������Ϣ
		// Returns  : void
		// Parameter: rapidxml::xml_node<char>* node ��������ϢXML����
		// Parameter: AppData * pAppData �����ڴ洢������Ϣ
		static void PraseIpaInfoFromXmlNode(rapidxml::xml_node<char>* node, AppData* pAppData, IIosFileSystem* pFileStream);
	};
}
