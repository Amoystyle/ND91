#pragma once

#include "NodeData.h"

namespace ND91Assistant
{
    class ProgressObserver;
    class DownloadFileCommand;
    class DirectoryOperationCommand;

	// �ļ�ϵͳ�ڵ㣺�ļ�
	class CLASSINDLL_CLASS_DECL FileData : public NodeData
    {
        friend class FileHelper;
        friend class DirData;

        virtual bool IsDir() { return false; }

    public:
        FileData();
        virtual ~FileData();

        // ��������·��
        virtual wstring GetFullPath() const;

        // ������
		virtual bool Rename(wstring dest);

        // ��Ȩ��
		virtual bool ChangeMode(string mode);

        // ȡȨ��
        virtual bool GetMode(string& mode, wstring& owner);

        // ˢ������
		virtual bool Refresh();

        // ����һ�����࣬��ָ�����ڵ㣨�¶���ָ���ɵ����߸���
        virtual NodeData* Clone(DirData* pParent) const;

    private:
		
        // ɾ��(�ݹ�ʱ�ã�
		virtual bool DoDelete(DirectoryOperationCommand* pCmd,ProgressObserver* pObserver=NULL);

        virtual TransferResult DoDownload(wstring dest, DownloadFileCommand* pCmd);
    };

}
