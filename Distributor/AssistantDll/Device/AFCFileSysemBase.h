#pragma once

#include "Core/DeviceInfo.h"
#include "Monitor/ProgressSubject.h"
#include "IIosFileSystem.h"

#include <map>

namespace ND91Assistant
{
    class LinkData;

	struct sLine{
		string strValue;
		sLine* pNext;
	};

    // ����������ģʽ
    enum LinkMode
    {
        Hard    = 1,    // Ӳ����ģʽ,������һ�ݵ���������Ŀ¼
        Symbol  = 2,    // ����...
    };

    // ����iTunesMobileDevice.dll�⺯��������ļ���ز���
    // �ÿ⺯���ķ�����������Ƿ���0��ʾ�ɹ�
	class AFCFileSysemBase : public IIosFileSystem
	{
	public:
		// Summary  : ���캯��
		// Returns  : 
		// Parameter: void * pAfcHandle ����Ӧ_ios_afcHandle
		// Parameter: void * pAfcHandleApp ����Ӧ_ios_afcHandleApp
		// Parameter: bool bJailBreaked ����Ӧ_ios_bJailBreaked
		AFCFileSysemBase(const DeviceInfo* pDevInfo);
        AFCFileSysemBase(void* pAfcHandle, const DeviceInfo* pDevInfo);// Ӧ���ļ�ϵͳ���ã���Ӧ�ù����ĵ�,IPA��װ/����,video����
		~AFCFileSysemBase(void);

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
		virtual TransferResult Ios_CopyToPhone(wstring sourceOnComputer, wstring destinationOnPhone, ProgressSubject& subject = NullProgressSubject::Instance());

		//�����ļ�
		virtual TransferResult Ios_DownFromPhone(wstring sourceOnPhone, wstring destinationOnComputer, ProgressSubject& subject = NullProgressSubject::Instance());

        // ��USB����״̬��ֱ�����豸�϶�д����(Ŀǰ��ͼƬ�����д����ͼ���ݿ��ļ�����)
        virtual int Ios_OpenFile(wstring strPath, string& buffer);

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

        // Opens a File for reading 
        int OpenRead(wstring strPath, string& buffer);
        // Opens a File for writing 
        bool OpenWrite(wstring strPath, string buffe);

		virtual TransferResult Ios_CopyToPhoneEx(wstring sourceOnComputer, wstring destinationOnPhone, 
			unsigned __int64 OffSetAppInExe, unsigned __int64 len,
			ProgressSubject& subject = NullProgressSubject::Instance());

        // Summary  : ��������
        // Returns  : bool
        // Parameter: wstring target Ҫ�������ӵ�·�� target ·������/��ʼ ���ܳɹ�,��֪���Ƿ���bug
        // Parameter: wstring link ����
        // Parameter: LinkMode mode
        bool LinkPath(wstring target, wstring link, LinkMode mode);

        // Summary  : ��ȡ���һ��AFC����ʧ����Ϣ.����WIN32 GetLastError����TODO:��Щ�쳣����Ҫ����AFC����ſ��Ե�
        // Returns  : string
        string GetAFCLastErrorMsg();

        // Summary  : �ж���AFC�����Ƿ�����,���������,�������᳢������AFC����.
        // <remarks>����һЩ�����ᵼ��AFC�ҵ�,��˺�����Ҫafc�ĵط�������Ҫ����afc����.
        // ���簲װѹ������ʽ���淶 ND591up0712.ipa��ᵼ��AFC����ҵ�,��˺����İ�װ��Ҫ����AFC����
        // ��δԽ���豸��װ�ƽ�����</remarks>
        // Returns  : bool
        bool IsAFCConnectionValid();

        // Summary  : ����AFC�ļ�ϵͳ�������,���δԽ���豸,������AFC2�����ʧ��
        // Returns  : bool �Ƿ�ɹ�
        bool StartAFCFileSystem();

        // ֹͣAFC�ļ�ϵͳ����
        void StopAFCFileSystem();

	private:

		TransferResult DoCopyToPhone(wstring local_path, wstring remote_path, ProgressSubject& subject,
			unsigned __int64 OffSetAppInExe=0, unsigned __int64 len=0);
		TransferResult DoDownFromPhone(wstring remote_path, wstring local_path, ProgressSubject& subject);

        // ����Ŀ¼��ĳ�ڵ㣬�Ծɰ汾
        NodeData* GetFileInfo(DirData* pDir, wstring name, eFileGetType eType);

        // ����Ŀ¼��ĳ�ڵ㣬���°汾
        NodeData* FileInfoOpen( DirData* pDir, wstring name, eFileGetType eType);

        // �����ļ���Ϣ�����ڵ�
        NodeData* CreateNode(DirData* pDir, wstring name, std::map<string, string>& dict, eFileGetType eType);

        // �����ļ���Ϣ����������ɾ�����صĶ���
		NodeData* GetFileNode(wstring path);

	private:
		void*		_pAFCHandle;
		void*		_pAFCHandleApp;
		bool		_bJailBreaked; //�Ƿ�Խ��

        void*		_pAFCSocket;
        DeviceInfo*	_pDevInfo;
	};
}
