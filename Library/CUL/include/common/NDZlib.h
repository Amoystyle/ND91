#pragma once
#include "../cul.h"
#include <iostream>
#include "zip.h"
#include "unzip.h"
#include <vector>

using namespace std;

#define CASESENSITIVITY (0)
#define WRITEBUFFERSIZE (8192)
#define MAXFILENAME (256)

//zip�ļ�ѹ���ͽ�ѹ����
class CUL_CLASS_DECL NDZlib
{
public:
	NDZlib(void);
	~NDZlib(void);

	// Summary  : ��ѹZIP�ļ������е�ĳ���ļ���ָ��Ŀ¼
	// Returns  : bool �ɹ�����TRUE
	static bool UnZipFile(
        wstring strSrcFileFullName,         // ����ѹ���ļ�������·����
        wstring strDesFullPath,             // ��ѹ���ŵ�Ŀ¼
        wstring strSpecificFileName = L"",  // ָ����ѹ���ļ���
        bool bMatchPath = true,             // true ƥ��·�� false ��ƥ���ļ���
        wstring strPreDirName = L"",		// wstring strPreDirName ָ����ѹ�ļ����ϲ�Ŀ¼��  
		bool IsUtf8=false);					// ָ����ѹ���ڲ��ļ����Ƿ�Ϊutf8����  

	// Summary  : ѹ���ļ���
	// Returns  : bool �ɹ�����TRUE
	// Parameter: wstring strSrcFileFullName ����ѹ�����ļ���������·����
	// Parameter: wstring strDesFileFullName ��ѹ������ļ�������·����
	static bool ZipDirectory(wstring strSrcFileFullName, wstring strDesFileFullName);
    static bool ZipFile(vector<wstring> files, wstring strDesFileFullName);

    static int makedir2(const char* fileFuleName);
    static int makedir (const char *newdir);
    static void change_file_date(const char *filename, uLong dosdate, tm_unz tmu_date);

    // Summary  : ѹ�������Ƿ����ָ���ļ�
    // Returns  : true ���� false ������
    // Parameter: const char * zipFilename  ѹ����(gb2312)
    // Parameter: const char * filename     ָ���ļ���(gb2312)
    static bool Exist(const wchar_t* zipFilename, const char* filename);

    // Summary  : �ṩѹ�������ļ���Ŀ¼���б�ķ���
    // Returns  : true �ɹ� false ʧ��
    // Parameter: wstring zipFilename       ZIP�ļ�
    // Parameter: vector<wstring> & files   �ļ���Ŀ¼���б�
    static bool GetFilesInZip(wstring zipFilename, vector<wstring> &files);

private:
	static int mymkdir(const char* dirname);
	static int do_extract_currentfile(unzFile uf, const int* popt_extract_without_path, int* popt_overwrite, const char* szSavePath);
	static int do_extract(unzFile uf, int opt_extract_without_path, int opt_overwrite, const char* szSavePath);
	static int do_extract_onefile(unzFile uf, const char* filename, int opt_extract_without_path, int opt_overwrite, const char* szSavePath, bool bMatchPath = true, const char* strPreDirName = NULL);

	static bool AddContenToZip(zipFile zf, const wstring strPath, const wstring parentDir);
	static bool AddFileToZip(zipFile zf, const char* fileNameInZip, const char* srcFile, bool bDir = false);
	static uLong filetime(char *f, tm_zip *tmzip, uLong *dt);
};
