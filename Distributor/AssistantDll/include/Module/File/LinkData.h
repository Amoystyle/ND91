#pragma once

#include "Module/File/NodeData.h"

using namespace std;

#pragma warning(disable : 4251)

#define TARGET_INIT_VALUE (NodeData*)0xFFFFFFFF

namespace ND91Assistant
{
    class DownloadFileCommand;
    class CopyFileCommand;
    class DirectoryOperationCommand;

	class CLASSINDLL_CLASS_DECL LinkData : public NodeData
	{
    public:
		wstring     _linkTarget;       // ����Ŀ��·����ios�Ͽ��������·����

	public:
		LinkData();
		virtual ~LinkData();

        // �ж�ָ��Ľڵ��Ƿ�Ŀ¼; �޷��ҵ�ָ��ڵ����Ϊ�ļ�����
        virtual bool IsDir();

		// ��������·��
		wstring GetFullPath() const;

        virtual bool Rename(wstring dest);
		virtual bool ChangeMode(string mode);
        virtual bool GetMode(string& mode, wstring& owner);
		virtual bool Refresh();

        // ��������ָ��Ľڵ㣬���Ҳ����򷵻�NULL
        NodeData* GetTarget();

    private:

        // ����һ�����࣬��ָ�����ڵ㣨�¶���ָ���ɵ����߸���
        virtual NodeData* Clone(DirData* pParent) const;

        // ���أ���
        virtual TransferResult DoDownload(wstring dest, DownloadFileCommand* pCmd);

        // ɾ��
		virtual bool DoDelete(DirectoryOperationCommand* pCmd,ProgressObserver* pObserver=NULL);

        NodeData* FindTarget();

    private:
        // ����Ŀ��ڵ�
        // =TARGET_INIT_VALUE ��ʾδ��ʼ����=NULL��ʾ�Ҳ���Ŀ��ڵ�
        NodeData*   _targetNode;       
	};

}