#pragma once

#include "AssistantDll.h"
#include <string>

using namespace std;

#pragma warning(disable:4251)

namespace ND91Assistant
{
	class DirData;
    class DeviceData;
    class CopyFileCommand;
    class DownloadFileCommand;
	class DirectoryOperationCommand;
	class ProgressObserver;
	// �ļ�ϵͳ�ڵ�
	// ������Ŀ¼���ļ������ӵ�����
    class CLASSINDLL_CLASS_DECL NodeData
    {
        friend class DirData;
        friend class RootData;
        friend class FileHelper;

    public: // ��Ա����

        // �ڵ�����
        enum NODE_TYPE {
            DIR,
            FILE,
            LINK,
        };

        const NODE_TYPE _type;          // �ڵ�����

        wstring		_name;              // ����
        wstring     _displayName;       // ������ʾ������
        long		_size;				// ��С

        // 3�����֣����2����, �� 755 = 111 101 101   
        // 111 ���� �ɶ� ��д ��ִ��
        // 101 ���� �ɶ� ����д ��ִ��
        // 101 ���� �ɶ� ����д ��ִ��
        string		_mode;              // Ȩ��
        string		_owner;             // ӵ����
        string		_group;             // ��

        time_t		_modifyTime;        // �޸�ʱ��
        time_t		_createTime;		// ����ʱ��

    protected:
        // ����Ŀ¼(���ڵ㣬��root��ֵΪNULL)
        // Ϊ�˲��������޸����ֵ��������
        DirData*	_Parent;			
        wstring     _fileToPC;          // ���ص�PC��·��������ֵ

    public: // ����

        NodeData(NODE_TYPE t);
        virtual ~NodeData(void);

        DirData* GetParent() const { return _Parent; }

        wstring GetFileToPC() { return _fileToPC; };

        // �ڵ��Ƿ����û��ռ�
        // return true:�û��ռ�,  false:ϵͳ�ռ�
        bool IsUserSpace();

        // ˢ�½ڵ���Ϣ
	    virtual bool Refresh() = 0;

        // ȡ����Ŀ¼
        wstring GetParentPath() const;

        // ȡ����·��
		virtual std::wstring GetFullPath() const = 0;

		// ����һ�����࣬��ָ�����ڵ㣨�¶���ָ���ɵ����߸���
		virtual NodeData* Clone(DirData* pParent) const = 0;

		// ������
		virtual bool Rename(wstring dest) = 0;

		// ��Ȩ��
		virtual bool ChangeMode(string mode) = 0;

        // ȡȨ��
        virtual bool GetMode(string& mode, wstring& owner) = 0;

        // �Ƿ�Ŀ¼����Link���ж���ָ��Ľڵ��Ƿ�Ŀ¼��
        virtual bool IsDir() = 0;

    protected:
		// ɾ���������Լ���ע�⣺���ú�ָ�뼴ʧЧ����
		virtual bool DoDelete(DirectoryOperationCommand* pCmd, ProgressObserver* pObserver=NULL) = 0;

        // ���ص�ָ��Ŀ¼
        virtual TransferResult DoDownload(wstring dest, DownloadFileCommand* pCmd) = 0;

    };

    // Ȩ���ַ�����777��ʽ
    class CLASSINDLL_CLASS_DECL Mode
    {
    public:
        Mode(string rwxMode)
        {
            if ( rwxMode.size() != 9 )
            {
                _mode[0] = '7';
                _mode[1] = '7';
                _mode[2] = '7';
                return;
            }

            for(int i = 0; i < 3; ++i )
            {
                string rwx = rwxMode.substr(3*i, 3*i + 3);
                ConvertOne(rwx, i);
            }
        }

        string GetMode()
        {
            return string(_mode, 3);
        }

    private:
        void ConvertOne(string oneMode, int offset)
        {
            int mode = 0;
            for( int i = 0; i < 3; ++i )
            {
                if ( oneMode[i] != '-' )
                    mode += 1 << (2-i);
            }
            _mode[offset] = '0' + mode;
        }

        char _mode[3];	// 777��ʽ

    };

}
