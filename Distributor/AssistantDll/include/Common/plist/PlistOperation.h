#pragma once
#include <AssistantDll.h>
#include <map>
#include <vector>
#include <string>
using namespace std;

namespace ND91Assistant
{
    class AppData;
}

class CLASSINDLL_CLASS_DECL PlistOperation
{
public:
	// Summary  : ��ʼ�������ض�̬�⣨MAC��������ã�
	// Returns  : �Ƿ�ɹ�
	// Parameter: string dllPath ��̬��·��
	static bool Init(wchar_t* dllPath);

	// Summary  : �ͷŶ�̬��
	static void Release();

	// Summary  : plist����
	// Returns  : �����plist
	// Parameter: string inputPlistData �����ƻ�XML��ʽplist���ݣ�xml��ʽ������������ΪUTF8)
	// Parameter: bool outputBinary ָ�������ʽ��trueΪ�����ƣ�falseΪXML
	static string ConvertPlist(string inputPlistData, bool outputBinary);

	// Summary  : ��plist�ļ�������һ��key-value
	//            ��key�Ѿ��������滻��value(�ļ����ݱ�����Dict)
	// Returns  : �Ƿ�ɹ�()
	// Parameter: wstring filename   �ļ���
	// Parameter: string key        ��ӵ�key
	// Parameter: string value      ��ӵ�value
	static bool AddDictPlist(wstring filename, string key, string value);

	// Summary  : ��plist�ļ�������һ��key-value
	//			  ��key�Ѿ��������滻��value(�ļ����ݱ�����Dict)
	// Returns  : �Ƿ�ɹ�()
	// Parameter: wstring filename   �ļ���
	// Parameter: string key        ��ӵ�key
	// Parameter: bool value		��ӵ�value
	static bool AddDictPlist(wstring filename, string key, bool value);

	// Summary  : ��plist�ļ�������һ��key-map<string, string>
	//			  ��key�Ѿ��������滻��value(�ļ����ݱ�����Dict)
	// Returns  : �Ƿ�ɹ�()
	// Parameter: wstring filename   �ļ���
	// Parameter: string key        ��ӵ�key
	// Parameter: bool value		��ӵ�value
	static bool AddDictPlist(wstring filename, string key, map<string,string> mapInfos );

	// Summary  : ��ȡplist�ļ�Key����Ӧ��ֵ
	// Returns  : �Ƿ�ɹ�
	// Parameter: wstring filename	�ļ���
	// Parameter: string key		�����key
	// Parameter: string & value	��ȡ��value
	static bool GetDictPlist(wstring filename, string key, string& value);


	// Summary  : ��ȡplist�ļ�Key����Ӧ��boolֵ
	// Returns  : �Ƿ�ɹ�
	// Parameter: wstring filename	�ļ���
	// Parameter: string key		�����key
	// Parameter: bool & value		��ȡ��value
	static bool GetDictPlist(wstring filename, string key, bool& value);

	// Summary  : ��ȡplist�ļ�Key����Ӧ��Dict������subKey��Ӧ��ֵ
	// Returns  : �Ƿ�ɹ�
	// Parameter: wstring filename	�ļ���
	// Parameter: string key		�����key
	// Parameter: string subKey		������Ӽ�subKey
	// Parameter: string & value	��ȡ��value
	static bool GetDictPlist(wstring filename, string key, string subKey, string& value);



	// �ж�plist�Ƿ�������Key
	static bool PlistContainsKey(wstring filename, string key);
	static bool NDCFDictionaryContainsKey( void* pl, string key );

	// �ж�plist���Key��Ӧ��Value�Ƿ����subkey
	static bool PlistContainsKey(wstring filename, string key, string subKey);

	// ��ȡplist�ļ����е����ݣ������������������
	static bool GetPlistAllValuesKeys(wstring filename, vector<string> verDatas, map<string, string> mapInfos, map<string,string> mapDatas );

    // Summary  : ��IPA�ļ��е�info.plist��ȡӦ������
    // Returns  : ���ݣ�ΪNULL��ʾʧ��
    // Parameter: wstring tempDir ��ʱĿ¼���ļ���ѹ�ڴˣ���ɺ�ɾ���ļ�
    // Parameter: wstring ipaFile ipa�ļ�
    static ND91Assistant::AppData* GetIpaAppInfo(wstring tempDir, wstring ipaFile);

    // Summary  : ����info.plist�ļ�
    // Returns  : ND91Assistant::AppData* ���ݣ�ΪNULL��ʾʧ��
    // Parameter: wstring strPath info.plist�ļ�·��
    static ND91Assistant::AppData* ParseIpaAppInfoPlist(wstring strPath, wstring ipaFile = L"");

	//��XML�ļ��ж�ȡkey��ֵ
	static bool XmlPlistGetKey(wstring filename, string key, string& value);

	//������ʱ��plist�ļ�
	static bool GenerateTempPlist(void* pCFDict);
};
