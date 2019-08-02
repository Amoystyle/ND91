///////////////////////////////////////////////////////////
//  GetMessageCommand.h
//  Implementation of the Class GetMessageCommand
//  Created on:      01-����-2011 9:42:55
//  Original author: michael
///////////////////////////////////////////////////////////

#if !defined(EA_D0425901_C899_407d_B915_875FBE1A48A0__INCLUDED_)
#define EA_D0425901_C899_407d_B915_875FBE1A48A0__INCLUDED_

#include "Module/BaseCommand.h"
#include <vector>
#include <string>
using namespace std;

namespace ND91Assistant
{
    class MessageData;
    class MessageGroup;

    // ȡ����Ϣ����
    class GetMessageCommand : public BaseCommand
    {
    public:
        GetMessageCommand(const DeviceData* pDevice);
        virtual ~GetMessageCommand();

		// ������������
		int GetSize();

        // �����Ƿ�ֻ��ȡ��������
        void SetOnlySize(bool onlySize);

        // ִ������
        virtual void Execute();

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

    public:
		void SetDataList(vector<MessageData*>* dataList, vector<MessageGroup*>* groupList);

    private:

        // ִ��IOS����
        bool IosExecute();

    private:
        vector<MessageData*>* _dataList;

		vector<MessageGroup*>* _groupList;

        bool _onlySize;// �Ƿ�ֻ��ȡ����

        int _size;     // ��������
    };
}

#endif // !defined(EA_D0425901_C899_407d_B915_875FBE1A48A0__INCLUDED_)
