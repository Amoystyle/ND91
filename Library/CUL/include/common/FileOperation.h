#pragma once

#include "../cul.h"

#ifdef WIN32
    #include <Windows.h>
    #include <tchar.h>
#endif

#include <iostream>
#include <vector>
using namespace std;

class CUL_CLASS_DECL CFileOperation  
{
public:

#pragma region �ļ���ز���

    // Summary  : �������ļ�
    static bool CreateEmptyFile(const wchar_t* lpszFileName);

    // Summary  : ɾ���ļ�
    static bool DeleteFile(wstring strSrc);

    // Summary  : �ƶ��ļ�
    static bool MoveFile( wstring strSrc, wstring strDes );

    // Summary  : �����ļ�
    // Parameter: bool bFailIfExists true�����ļ������򲻸��Ʋ�����ʧ��
    static bool CopyFile( wstring strSrc, wstring strDes, bool bFailIfExists );

    // Summary  : ��ָ���ļ��Ѵ��ڣ����ں������_XX���Զ�������
    // Comment  : ��֧�ֶ��߳�
    static wstring RenameFile(const wstring localName);

    // Summary  : �ж��ļ��Ƿ����
    static bool IsFileExist(const wchar_t* lpszFileName);

	//  Summary  :�ж��Ƿ���iosϵͳ��plist�ļ�
	static bool IsIosPlist(wstring strPcPath);

    // Summary  : ��ȡ�ļ���С
    static int GetFileSize(const string& filename);
    static int GetFileSizeW(const wstring& filename);

    // Summary  : ��ȡ�ļ����ݣ�ָ���Ƿ�����Ʒ�ʽ��
    static string ReadFile(wstring strFilePath, bool bBinary=false);

    // Summary  : ��ȡ�ļ����ݣ������Ʒ�ʽ��
    static string GetFileAllContent(string strFullName);

    // Summary  : д���ļ��������Ʒ�ʽ��
    static bool WriteBinFile(wstring strFilePath, const char* pData, size_t nSize);

    //  Summary  : ����ָ���ļ���MD5У����
    static string GetFileMD5(string strFilePath);
    static string GetFileMD5(wstring strFilePath);

#pragma endregion

#pragma region Ŀ¼��ط���

    // ����Ŀ¼�����Զ�������
    static bool MakeLocalDir(wstring strDir);

    // Summary  : ɾ���ļ����ļ���
    // Parameter: wstring strSrc  : ��ɾ�����ļ����ļ���������·����
    // Parameter: DWORD nFlags    : ѡ�Ĭ��ΪFOF_NOCONFIRMATION | FOF_NOERRORUI
    //                              ��������ȷ����ʾ��ͳͳYES)�ʹ�����ʾ
    //                              �����Զ�����鿴SHFILEOPSTRUCT��FILEOP_FLAGS
    static bool DeleteDir(wstring strSrc, DWORD nFlags = 0);   

    // Summary  : �����ļ����ļ���
    static bool CopyDir(wstring strSrc, wstring strDes);

    // Summary  : ɾ���ļ��м����µ������ļ�
    static bool DeleteDirectory(wstring strDir);

    // Summary  : �ж�Ŀ¼�Ƿ����
    static bool IsDirExist(const wchar_t* lpszFileName);

    // Summary  : ��ָ��Ŀ¼�Ѵ��ڣ����ں������_XX���Զ�������
    // Comment  : ��֧�ֶ��߳�
    static wstring RenameDir(const wstring localName);

    // Summary  : ��ȡ��ǰ���̵�EXE�ļ�����Ŀ¼
    // Commment : ע����ܲ��ǵ�ǰ����Ŀ¼
    static std::wstring GetCurrWorkingDir();

    // Summary  : ȡ��Ŀ¼�µĽڵ��������ļ���С�ܺ�
    // Parameter: const wstring & dir       : Ŀ¼��������·����
    // Parameter: int & nodeSum             : ������ڵ��������ļ�����
    // Parameter: __int64 & sizeSum         : ������ļ���С�ܺ�
    // Parameter: vector<wstring>* fileExts : ֻ����ָ����׺���ļ�
    static void GetDirSizeW(const wstring& dir, int& nodeSum, __int64& sizeSum, vector<wstring>* fileExts = NULL);

    // Summary  : ��ȡĿ¼�µ������ļ�������������Ŀ¼�µ��ļ�
    // Parameter: wstring strDir            : Ŀ¼��������·����
    // Parameter: vector<wstring> & files   : ����������ļ���������·����
    // Parameter: wstring strExt            : ֻ����ָ����׺���ļ�
    static void FindAllFileInDir(wstring strDir, vector<wstring> &files, wstring strExt=L"");

    // Summary  : ��ȡĿ¼�µ������ļ��У�����������Ŀ¼�µ�Ŀ¼
    // Parameter: wstring strDir            : Ŀ¼��������·����
    // Parameter: vector<wstring> & files   : ����������ļ�����������·����
    static void FindAllFolderInDir(wstring strDir, vector<wstring> &folders);

    // Summary  : �����ļ��У��������ƶ��ļ���ƥ����ļ�
    // Parameter: wstring strDir            : ���������ļ���
    // Parameter: wstring strFileName       : ƥ����ļ���
    // Parameter: vector<wstring> & vtGet   : ���ص�ƥ���ļ��б�
    // Parameter: bool bAbsolutelyMatch     : �Ƿ���ȫƥ�䣬ΪFALSE�򲻿��Ǵ�Сд
    static void GetMatchFileFullName(wstring strDir, wstring strFileName, vector<wstring>& vtGet, bool bAbsolutelyMatch = true);

    // Summary  : �����ļ��У����ص�һ���ļ�������strEndName��β���ļ���
    // Parameter: wstring strDir            : ���������ļ���
    // Parameter: wstring strEndName        : ��β�ļ�����
    // Parameter: wstring& strDirFullName   : ��������ص�һ��ƥ���ļ���ȫ��
    static void GetMatchDirFullNameByEndName(wstring strDir, wstring strEndName, wstring& strDirFullName);

#pragma endregion

#pragma region ·����ط���

    // Summary  : ȡ�ļ���
    // Returns  : ���һ��/��\֮����ַ���
    static wstring GetFileName(const wstring strFilePath);

    // Summary  : ȡ�ļ���չ��
    // Returns  : ���һ��/��\֮����ַ������������һ��.֮�������
    static wstring GetFileExt(const wstring strFilePath);

    // Summary  : ȡ�ļ���
    // Returns  : ���һ��/��\֮����ַ�������ȥ�����һ��.֮�������
    static wstring GetFileNameNoExt(const wstring strFilePath);

    // Summary  : ȡ�ļ�·��
    // Returns  : ���һ��/��\֮ǰ���ַ���
    static wstring GetFileDir(const wstring strFilePath);

    // Summary  : ȡָ���ļ���Ŀ¼�ĸ�Ŀ¼
    static wstring GetParentDir(const wstring strPath);

    // Summary  : ƴ��Ŀ¼���ļ��������������ļ�·��
    // Comment  :  �� û����Ŀ¼���ļ��Ƿ���ڵ�У��
    //             �� ����һ�������Ϊ���򷵻ؿ�
    // Returns  : �����ļ�·��
    // Parameter: const wstring dir         Ŀ¼��
    // Parameter: const wstring filename    �ļ���
    static wstring MakeFilePath(const wstring& dir, const wstring& filename);

#pragma endregion

#pragma region ѹ����ѹ����

    // Summary  : ��ѹzip�ļ���ָ��·��
    // wstring strPreDirName ָ����ѹ�ļ����ϲ�Ŀ¼��,��Ŀ¼Ϊ ��/��
    static bool UnZipFile(wstring strSrcFileFullName, wstring strDesFullPath, wstring strSpecificFileName = L"", bool bMatchPath = true, wstring strPreDirName = L"",bool IsUtf8=false);

    // Summary  : ѹ���ļ���
    // Returns  : �ɹ�����TRUE
    // Parameter: wstring strSrcFileFullName ����ѹ�����ļ���������·����
    // Parameter: wstring strDesFileFullName ��ѹ������ļ�������·����
    static bool ZipDirectory(wstring strSrcFileFullName, wstring strDesFileFullName);

    // Summary  : ѹ���ļ�
    // Returns  : �ɹ�����TRUE
    // Parameter: vector<wstring> files         ��ѹ�����ļ���������·����
    // Parameter: wstring strDesFileFullName    ѹ������ļ�������·����
    static bool ZipFile(wstring file, wstring strDesFileFullName);
    static bool ZipFile(vector<wstring> files, wstring strDesFileFullName);

#pragma endregion

#pragma region ��������

    // Summary  : ȡ��GUID
    static string GetGuid();
    static wstring GetGuidW();

    // Summary  : ��ȡini�����ļ�
    // Returns  : ָ��key��ֵ
    // Parameter: wstring strPath   ini�ļ�·��
    // Parameter: wstring strGroup  �������Group
    // Parameter: wstring strName   �������key
    static wstring GetProfileString(wstring strPath, wstring strGroup, wstring strName);

#pragma endregion
};
