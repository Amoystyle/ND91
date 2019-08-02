///////////////////////////////////////////////////////////
//  SendMessageCommand.h
//  Implementation of the Class SendMessageCommand
//  Created on:      01-����-2011 9:43:03
//  Original author: michael
///////////////////////////////////////////////////////////

#if !defined(EA_40988EBC_C77A_4b99_A80E_C737A09B85DA__INCLUDED_)
#define EA_40988EBC_C77A_4b99_A80E_C737A09B85DA__INCLUDED_

#include "Module/BaseCommand.h"
#include "Common/BinaryBufferWriter.h"
#include <string>
#include <vector>
using namespace std;

namespace ND91Assistant
{
    class MessageData;

    class SendMessageCommand : public BaseCommand
    {

    public:
        SendMessageCommand(const DeviceData* pDevice);
        virtual ~SendMessageCommand();

        // ִ������
        virtual void Execute();

        // ����Android�����壬
        virtual std::string GetAndroidBodyData();

        // ����Android�������ݰ�
        virtual void SetAndroidReturnData(std::string data);

        // ����͵�IOS�豸�ϵ��ļ�����ȫ·����
        virtual std::wstring GetIosCommandFilePath();

        // ��IOS�ϻ�ȡ���������ļ�����ȫ·����
        virtual std::wstring GetIosResultFilePath();

        // ����ios�ļ��������ļ����������߸���ɾ���ļ�
        virtual std::wstring GetIosCommandFileContent();

        // ���������ļ���������ɾ���ļ�
        virtual  void SetIosReturnFile(std::wstring filename);

        // ����Ҫ���͵���Ϣ
        void SetDataList( const std::vector<MessageData*>* DataList);

        // ������Ϣ�Ƿ�ɹ�
        bool Success();

    private:

        // ִ��Ios����
        bool IosExecute();

        // ���ػ�ģʽ��ִ��Ios����
        bool IosDeamonExecut();

        // ���ػ�ģʽ��ִ��Ios����
        bool IosNoDeamonExecut();

        // ִ��Android����
        bool AndroidExecute();

        // Summary  : ��ȡ�绰�����ַ���
        // Returns  : string �绰�����ַ���
        string GetPhoneNumberStr();


		void WaiteForResult();


        // Summary  : ���Ͷ��Ż�ִ
        // Parameter: int id ����ID
        // Parameter: string phoneNumber ���Ͷ��ŵĵ绰����
        // Parameter: bool success �����Ƿ�ɹ�
        void ReceiptMessage(int id, string phoneNumber, bool success);


        // Summary  : ��ȡIOS�豸�Ĳ�Ʒ����
        // Returns  : std::wstring ��Ʒ����
        // Parameter: const DeviceData * pDevice �豸��Ϣ
        wstring GetSerial(const DeviceData * pDevice);

    private:
        // ��ǰ���͵Ķ���Ϣ
        MessageData* _pMessageData;

        // ��Ҫ���͵Ķ���Ϣ
        const vector<MessageData*>* _dataList;

        // ���ͳɹ���ID����
        vector<int> _sendSuccessIds;

        // �跢�Ͷ��ŵĵ绰��������
        vector<string> _phoneVec;

        // ����ִ���Ƿ�ɹ�
        bool _success;
    };
}

#endif // !defined(EA_40988EBC_C77A_4b99_A80E_C737A09B85DA__INCLUDED_)
