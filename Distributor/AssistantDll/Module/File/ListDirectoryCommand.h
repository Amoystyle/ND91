#pragma once

#include "Module/BaseCommand.h"
#include "Common/greta/RegexprHelper.h"

namespace ND91Assistant
{
    // ��ȡĿ¼
    class ListDirectoryCommand : public BaseCommand
    {
    public:
        //************************************
        // Method:    ���캯��
        // Parameter: const DeviceData* pDevice �豸ָ��
        // Parameter: NodeData * pNode  ����Ŀ¼��ѯ���ӽڵ㣬�����ѯ�����ļ�
        //************************************
        ListDirectoryCommand(const DeviceData* pDevice, NodeData* pNode);
        virtual ~ListDirectoryCommand();

        // �ı�ڵ�
        // pNode  ����Ŀ¼��ѯ���ӽڵ㣬�����ѯ�����ļ���Ϣ
        void SetNode(NodeData* pNode);

        // ִ������
        virtual void Execute();

        // ����ִ���Ƿ�ɹ�
		bool  DoneSuccess();

        // ����Android�������ݰ�
        virtual std::string GetAndroidBodyData();

        // ����Android���ؽ�����ݰ�
        virtual void SetAndroidReturnData(std::string data);

        // ����͵�IOS�豸�ϵ��ļ�����ȫ·����
        virtual std::wstring GetIosCommandFilePath() { return L""; }

        // ��IOS�ϻ�ȡ���������ļ�����ȫ·����
        virtual std::wstring GetIosResultFilePath() { return L""; }

        // ����ios�ļ��������ļ����������߸���ɾ���ļ�
        virtual std::wstring GetIosCommandFileContent() { return L""; }

        // ���������ļ���������ɾ���ļ�
        virtual  void SetIosReturnFile(std::wstring filename) { }

    private:
		bool        _success;
        RegexprHelper _rh;

	private:

        // �������ؽ��
        // ���� true ���ɹ�������false��ʧ��
		bool ParseResult(const string& result);
    };
}
