#pragma once

#ifdef WIN32
    #include <Windows.h>
    #include <tchar.h>
#endif

#include "AssistantDll.h"
#include <iostream>
#include "Common/StrOperation.h"

using namespace std;
class CVersion;

struct CLASSINDLL_CLASS_DECL FileVersionInfo
{
    wstring _fileVersion;           // �汾��
    wstring _companyName;           // ��˾����
    wstring _fileDescription;       // �ļ�����
    wstring _internalName;          // �ڲ�����
    wstring _legalCopyright;        // �Ϸ���Ȩ
    wstring _originalFilename;      // ԭʼ�ļ���
    wstring _productName;           // ��Ʒ����
    wstring _productVersion;        // ��Ʒ�汾
    wstring _legalTrademarks;       // �̱���
    wstring _comments;              // ע��
    wstring _privateBuild;
    wstring _specialBuild;
};

class CLASSINDLL_CLASS_DECL MyFileOperation  
{
public:

    // �ж��Ƿ���iosϵͳ��plist�ļ�
	static bool IsIosPlist(wstring strPcPath);

    //��ȡini�����ļ���Ϣ
    static wstring GetProfileString(wstring strPath, wstring strGroup, wstring strName);
	// ����ͼƬ�ļ���׺�����ض�Ӧmime
    static string GetImageFileMime(wstring path);

	// ����ͼƬ�ļ���׺�ж��Ƿ�Ϊ֧�ֵ�ͼƬ����
    static bool  IsSupportImageFile(wstring filepath);

    static wstring GetRegValue(/*HKEY*/struct HKEY__* hTopkey, /*TCHAR*/wchar_t* szSubKey, /*TCHAR*/wchar_t* szValueName);
    static wstring GetIniFileValue(wstring strIniFile, wstring strGroup, wstring strKey);
    static bool SetIniFileValue(wstring strIniFile, wstring strGroup, wstring strKey, wstring strValue);
    static wstring GetWindowsDir();
    // ��ȡϵͳ�ļ��� CSIDL_PERSONAL �ҵ��ĵ�
    static wstring GetSystemPath(int nFolder);
    static wstring GetMyDocumentPath();

    // Summary  : ��ȡexe�ļ���Ϣ(�����߸����ͷ�FileVersionInfo*)
    // Returns  : FileVersionInfo* �����ļ���Ϣ���ݽṹ
    // Parameter: wstring strPath �ļ�·��
    static FileVersionInfo* GetExeFileInfo(wstring strPath);

	// Summary  : ios���ݲ��ŵ���id����Ϣid�����丽����sms�ļ����µ�·��
	// Returns	: testing::internal::wstring
	// Parameter: int messageId ��Ϣid
	// Parameter: int groupId ��id
	static wstring ConvertMMSItemFolder(int messageId, int groupId);

    // Summary  : �������ļ�
    // Returns  : void* �ļ����ָ�� 
    // Parameter: wstring dest �ļ����ɵ�·��
    static void* MyCreateFile(wstring dest);

    // Summary  : д�ļ�
    // Returns  : void
    // Parameter: void* hFile �ļ����ָ�� 
    // Parameter: string buffer �ļ�����
    // Parameter: unsigned long nWrite д�볤��
    static void MyWriteFile(void* hFile, string buffer, unsigned long nWrite);

    // Summary  : �ر��ļ�
    // Returns  : void*
    // Parameter: void* handle �ļ����ָ��
    static void MyCloseFile(void* hFile);
    
private:
    static wstring QueryValue(LPVOID pInfoBuf, wstring lang_codepage, wstring name, LPVOID pInfoVal);
};
