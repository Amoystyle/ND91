#pragma once

#include "Monitor/ProgressSubject.h"
#include "IIosFileSystem.h"

namespace ND91Assistant
{
	class IosWiFiFileSystem : public IIosFileSystem
	{
	public:
		IosWiFiFileSystem(int nhandle);
		~IosWiFiFileSystem(void);

		/// ��ǰ�ļ�ϵͳ�ܷ��ʵ���Ŀ¼
		virtual string Ios_GetRootDirectory(eFileSystemType eType);

		virtual bool Ios_Rename(wstring srcPath, wstring dstPath);

		virtual bool Ios_CreateDirectory(wstring strPath);

		virtual bool Ios_DeleteEmptyDirectory(wstring strPath);	

		virtual bool Ios_DeleteFile(wstring strPath);

		virtual bool Ios_Exists(wstring strPath);

		virtual bool Ios_IsDirectory(wstring strPath);

		virtual long Ios_FileSize(wstring strPath);

		//��ȡָ���ļ����µ������ļ���Ϣ������������Ŀ¼�µ���Ϣ
		virtual bool Ios_GetFileInfos(DirData* pDir, eFileGetType eType);

		//�ϴ��ļ�
		virtual TransferResult Ios_CopyToPhone(wstring sourceOnComputer, wstring destinationOnPhone, ProgressSubject& pSubject = NullProgressSubject::Instance());

		//�����ļ�
		virtual TransferResult Ios_DownFromPhone(wstring sourceOnPhone, wstring destinationOnComputer, ProgressSubject& pSubject = NullProgressSubject::Instance());

         // ��USB����״̬��ֱ�����豸�϶�д����(Ŀǰ��ͼƬ�����д����ͼ���ݿ��ļ�����)
        virtual int Ios_OpenFile(wstring strPath, string& buffer);

		virtual TransferResult Ios_CopyToPhoneEx(wstring sourceOnComputer, wstring destinationOnPhone, 
			unsigned __int64 OffSetAppInExe, unsigned __int64 len,
			ProgressSubject& subject = NullProgressSubject::Instance());

        // Summary  : ��ȡ�豸���ļ�SHA1ֵ,�÷�����Ҫ�ļ�����ͬʱ�ļ�����Ϊ��
        //����500MB�����ļ����ʱ�ܳ�,ֱ����ʱ.���Կ�����GetFileSHA1WithRange����
        // Returns  : string �ɹ�20�ֽ�SHA1ֵ
        // Parameter: wstring strPath �ļ�·��
        virtual string Ios_GetFileSHA1(wstring strPath);

        // Summary  : ��ȡ�豸���ļ� ���ļ�ͷ��ָ����β��SHA1ֵ,�÷�����Ҫ�ļ�����ͬʱ�ļ�����Ϊ��
        // Returns  : string �ɹ�20�ֽ�SHA1ֵ
        // Parameter: wstring strPath �ļ�·��
        // Parameter: long endPosition endPosition����С��0���ߴ����ļ���С
        virtual string Ios_GetFileSHA1WithRange(wstring strPath, long endPosition);

	private:
		bool Ios_GetFiles(DirData* pDir);
		bool Ios_GetDirectories(DirData* pDir);

		TransferResult DoCopyToPhone(wstring local_path, wstring remote_path, ProgressSubject& subject);
		TransferResult DoDownFromPhone(wstring remote_path, wstring local_path, ProgressSubject& subject);

        // ���� WiFiGetFiles��WiFiGetDirectories �ķ��ؽ��������pDir���ӽڵ�
        void ParseResult(char* szBuff, int nLen, DirData* pDir, bool isFile);

	private:
		int _nHandle;
	};
}
