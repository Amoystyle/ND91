#pragma once

#include "Module/File/FileData.h"
#include <vector>
#include "Common/StrOperation.h"
#include "Monitor/ProgressSubject.h"

using namespace std;

namespace ND91Assistant
{
    class FileData;
    class DirData;

	typedef enum eFileSystemType
	{
		//��ͨ�ļ�ϵͳ
		eFileSystemType_Normal,
		//ý���ļ�ϵͳ
		eFileSystemType_Media,
		//Ӧ�ó���ϵͳ
		eFileSystemType_Application,
	};


	typedef enum eFileGetType
	{
		eFileGetType_Files,
		eFileGetType_Dirs,
		eFileGetType_All
	};

	class IIosFileSystem
	{
	public:
		/// ��ǰ�ļ�ϵͳ�ܷ��ʵ���Ŀ¼
		virtual string Ios_GetRootDirectory(eFileSystemType eType) = 0;

		virtual bool Ios_Rename(wstring srcPath, wstring dstPath) = 0;

		virtual bool Ios_CreateDirectory(wstring strPath) = 0;

		virtual bool Ios_DeleteEmptyDirectory(wstring strPath) = 0;	

		virtual bool Ios_DeleteFile(wstring strPath) = 0;

		virtual bool Ios_Exists(wstring strPath) = 0;

		virtual bool Ios_IsDirectory(wstring strPath) = 0;

		virtual long Ios_FileSize(wstring strPath) = 0;

		//��ȡָ���ļ����µ������ļ���Ϣ������������Ŀ¼�µ���Ϣ
		virtual bool Ios_GetFileInfos(DirData* pDir, eFileGetType eType) = 0;

		//�ϴ��ļ����ļ���
		virtual TransferResult Ios_CopyToPhone(wstring sourceOnComputer, wstring destinationOnPhone, ProgressSubject& pSubject = NullProgressSubject::Instance()) = 0;

		//�����ļ����ļ���
        virtual TransferResult Ios_DownFromPhone(wstring sourceOnPhone, wstring destinationOnComputer,ProgressSubject& pSubject = NullProgressSubject::Instance()) = 0;

        // ��USB����״̬��ֱ�����豸�϶�д����(Ŀǰ��ͼƬ�����д����ͼ���ݿ��ļ�����)
        virtual int Ios_OpenFile(wstring strPath, string& buffer) = 0;

		virtual TransferResult Ios_CopyToPhoneEx(wstring sourceOnComputer, wstring destinationOnPhone, 
			unsigned __int64 OffSetAppInExe, unsigned __int64 len,
			ProgressSubject& subject = NullProgressSubject::Instance()) = 0;

        // Summary  : ��ȡ�豸���ļ�SHA1ֵ,�÷�����Ҫ�ļ�����ͬʱ�ļ�����Ϊ��
        //����500MB�����ļ����ʱ�ܳ�,ֱ����ʱ.���Կ�����GetFileSHA1WithRange����
        // Returns  : string �ɹ�20�ֽ�SHA1ֵ
        // Parameter: wstring strPath �ļ�·��
        virtual string Ios_GetFileSHA1(wstring strPath) = 0;

        // Summary  : ��ȡ�豸���ļ� ���ļ�ͷ��ָ����β��SHA1ֵ,�÷�����Ҫ�ļ�����ͬʱ�ļ�����Ϊ��
        // Returns  : string �ɹ�20�ֽ�SHA1ֵ
        // Parameter: wstring strPath �ļ�·��
        // Parameter: long endPosition endPosition����С��0���ߴ����ļ���С
        virtual string Ios_GetFileSHA1WithRange(wstring strPath, long endPosition) = 0;

    public:

        bool SkipFile(wstring file);

        // ��ȡ�ڵ��ڽ�����ʾ������
        void GetDisplayName(NodeData* pNode);

	};

}
