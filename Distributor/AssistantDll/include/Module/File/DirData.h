#pragma once
#include "Module/File/RootData.h"
#include "Module/File/NodeData.h"
#include "Monitor/ProgressObserver.h"
#include <deque>
#include <string>
#include <vector>

using namespace std;
#ifndef WIN32
#include "Common/winPort.h"
#else
#include <windows.h>
#endif

namespace ND91Assistant
{
    class DeviceData;
	class FileData;
	class LinkData;
    class ListDirectoryCommand;
    class RegexprHelper;
    class UploadFileCommand;
    class BaseCommand;

	class CLASSINDLL_CLASS_DECL DirData : public NodeData
	{
        friend class AFCFileSysemBase;
        friend class IosWiFiFileSystem;
        friend class ListDirectoryCommand;
        friend class RegexprHelper;
		friend class RootData;
	public:
		DirData(const DeviceData* pDevice);
		virtual ~DirData();

        virtual bool IsDir() { return true; }

#pragma region ���豸�ϵ��ļ�ִ�в���(���޸��ڴ�����)

        // �жϽڵ����Ƿ����
        bool IsExist(wstring subName);

        // ������
        virtual bool Rename(wstring dest);

        // ��Ȩ��
		virtual bool ChangeMode(string mode);

        // ȡȨ��
        virtual bool GetMode(string& mode, wstring& owner);

        // ������Ŀ¼
        bool MakeSubDir(wstring subDirName);

        // ˢ������
        virtual bool Refresh();

        // Summary  : �ƶ�����Cut & Paste�����Ѵ��ڽڵ��Ƶ���Ŀ¼��
        // Returns  : bool �Ƿ�ɹ�
        // Parameter: vector<NodeData*>* pNodes ���ƶ��Ľڵ㣨�ƶ����ʧЧ��
        // Parameter: ProgressObserver * pObserver ��ʹ����GetExistAction�������޽�������
        bool Move(vector<NodeData*>* pNodes, ProgressObserver* pObserver);

        // ���ƣ���Copy & Paste�����ļ�/Ŀ¼���Ƶ���Ŀ¼��
		bool Copy(vector<NodeData*>* pNodes, ProgressObserver* pObserver);

        // �豸���Ƿ���ڴ�Ŀ¼
        bool ExistInDevice() const;

		// �ϴ��ļ�
		TransferResult UploadFile(vector<wstring> paths, ProgressObserver* pObserver, bool bSetAttr = true);

        // �ϴ�Ŀ¼
        TransferResult UploadDir(wstring path, ProgressObserver* pObserver, bool bSetAttr = true);

		TransferResult UploadDir(vector<wstring> paths, ProgressObserver* pObserver,bool bSetAttr);

        // �ݹ��ȡ������Ŀ¼����, size=���ļ���С��totalCounts=�������� 
        bool RecursiveLoad(int& totalSize, int& totalCounts, wstring ExtName = L"");

#pragma  endregion

#pragma region ���ڴ������ݽṹִ�в���

        // ����һ�����࣬��ָ�����ڵ㣨�¶���ָ���ɵ����߸���
        virtual NodeData* Clone(DirData* pParent) const;

		// ȡ����·������"/"��β�����Ը�Ŀ¼��_nameӦ���ǿյģ�
		virtual std::wstring GetFullPath() const;

        // ����ڵ��Ƿ�Ŀ¼���ϲ�Ŀ¼
        bool IsParentDir(const NodeData* pNode) const;

        // �Ƿ��Ŀ¼
        bool IsRoot() const;

        // �����ֲ����ӽڵ�
		NodeData* findChildrenbyName(wstring name, bool bFindLinkTarget = true, bool bExactMatch = true);

        // Summary  : ���һ���ڵ�
        // Returns  : �����Ľڵ�ָ�롣����ͬ���ڵ����ʧ�ܡ�����NULL
        // Parameter: const NodeData * pNode Ҫ��ӵĽڵ���Ϣ
        NodeData* Insert(const NodeData* pNode, bool replace = true);

        // ɾ��һ���ӽڵ�
		void RemoveFromChildrens(NodeData* pNode);

        // �Ƿ�����Ŀ¼
        bool HasSubDir();

        // ȡ�������ӽڵ㣨�������飬�������ѭ���У�
		virtual deque<NodeData*> GetChildrens();

		// ȡ�������ӽڵ㣨�������飬�������ѭ���У�
		deque<NodeData*> GetChildrensNoReload(){return _childNodes;}

        // �������ļ���������
        static wstring PhoneFileRename(const wstring name, DirectoryOperationCommand* pCmd);

        // �������ļ�(ǰ�ӡ�������)��������
        static wstring PhoneFileRenameCopy(const wstring name, DirectoryOperationCommand* pCmd);

		RootData::ROOT_TYPE	GetRootType() { return _eRootType;};

		void SetHasLoad(bool hasLoad) { _hasLoad = hasLoad; }

#pragma endregion

    private:

        // ��δ��ȡĿ¼�������ȡ����һ�㣬���ݹ�
        void Load();

        // ��ָ���ļ���Ŀ¼��������Ŀ¼��
        bool DoCopy(NodeData* pNode,
            DirectoryOperationCommand* pDirCmd,
            CopyFileCommand* pFileCmd);

        // ��ָ��Ŀ¼��������Ŀ¼��
        bool DoCopy(DirData* pSourceDir,
            DirectoryOperationCommand* pDirCmd,
            CopyFileCommand* pFileCmd);

        // ��ָ���ļ���������Ŀ¼��
        bool DoCopy(FileData* pSourceFile,
            CopyFileCommand* pFileCmd);

        //ɾ��
        virtual bool DoDelete(DirectoryOperationCommand* pCmd,ProgressObserver* pObserver=NULL);

        // ��ȡĿ¼���ݣ���һ�㣬���ݹ�
        bool DoLoad(ListDirectoryCommand* pCmd);

        // �ݹ��ȡ������Ŀ¼����, size=���ļ���С��totalCounts=�������� 
        bool DoRecursiveLoad(int& totalSize, int& totalCounts, ListDirectoryCommand* pCmd, wstring ExtName = L"");

        // ���ص�ָ��Ŀ¼
        virtual TransferResult DoDownload(wstring destDir, DownloadFileCommand* pCmd);

        // �ϴ��ļ�
        TransferResult DoUploadFile(wstring path, UploadFileCommand* pFileCmd, DirectoryOperationCommand* pDirCmd);

        // �ϴ�Ŀ¼
        TransferResult DoUploadDir(wstring path, UploadFileCommand* pFileCmd, DirectoryOperationCommand* pDirCmd);

        // �豸�ϴ��ڴ�Ŀ¼ʱ������
        wstring PhoneDirRename(const wstring name);

        // �豸�ϴ��ڴ�Ŀ¼ʱ������(ǰ�ӡ�������)
        wstring PhoneDirRenameCopy(const wstring name);

        // ���������ӽڵ㣨��ȡĿ¼����ʱ�ã�
        LinkData* AddLink();

        // �����ļ��ӽڵ㣨��ȡĿ¼����ʱ�ã�
        FileData* AddFile();

        // ����Ŀ¼�ӽڵ㣨��ȡĿ¼����ʱ�ã�
        DirData* AddDir();

        // ����ӽڵ�
        void ClearChilds();

        // ����Ƿ����ͬ��Ŀ¼�����򴴽���DoCopy��ʹ�ã�
        DirData* CheckSubDir(DirectoryOperationCommand* pDirCmd,
            BaseCommand* pCommand, wstring name);

        // �ø������������Ŀ¼
        bool DoMakeSubDir(wstring subDirName, DirectoryOperationCommand* pCmd);

        // Summary  : ���½��Ľڵ���������
        // Parameter: NodeData * pNode �ڵ�ָ��
        // Parameter: wstring name �ڵ���
        void SetNodeAttribute( NodeData* pNode, wstring name);

        // Summary  : ִ��Move����
        // Returns  : �Ƿ�ɹ�
        // Parameter: NodeData * pNode ���ƶ��Ľڵ�
        // Parameter: DirectoryOperationCommand * pCmd ����ָ��
        bool DoMove(NodeData* pNode, DirectoryOperationCommand* pCmd);

        // �ϴ��ļ���/�ļ�������������
        void FileNameCheck( wstring &filename );

    public:
        const DeviceData*   _pDeviceData;

    private:
		// �ڲ��ࣺ���������
		class MyLock
		{
		public:
			MyLock(LPCRITICAL_SECTION	pLock) : _pLock(pLock) {
				EnterCriticalSection(pLock);
			}
			~MyLock() {
				LeaveCriticalSection(_pLock);
			}
		private:
			LPCRITICAL_SECTION	_pLock;
		};
		CRITICAL_SECTION	_lock;
		bool			    _hasLoad;
		RootData::ROOT_TYPE	_eRootType;

#pragma warning(disable : 4251)     // ���ô˾���
        deque<NodeData*> _childNodes;	// �ӽڵ�
#pragma warning(default : 4251)     // �ָ�

	protected:
		void SetRootType(RootData::ROOT_TYPE eRootType);
	};

}
