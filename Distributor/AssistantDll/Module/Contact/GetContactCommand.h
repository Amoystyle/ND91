///////////////////////////////////////////////////////////
//  GetContactCommand.h
//  Implementation of the Class GetContactCommand
//  Created on:      01-����-2011 9:42:54
//  Original author: michael
///////////////////////////////////////////////////////////

#if !defined(EA_F5566066_F495_4725_8FFC_E03463640DCF__INCLUDED_)
#define EA_F5566066_F495_4725_8FFC_E03463640DCF__INCLUDED_

#include "Module/BaseCommand.h"
#include <vector>
#include <map>
#include <string>
using namespace std;

#include "Module/Contact/ContactData.h"

namespace ND91Assistant
{
    class ContactAccount;

    // ��ȡ��������Ϣ����
    class GetContactCommand : public BaseCommand
    {
    public:
        GetContactCommand(const DeviceData* pDevice);
        virtual ~GetContactCommand();

        // �����Ƿ�ֻ��ȡ��������
        void SetOnlySize(bool onlySize);

		// ִ������
		virtual void Execute();

        const vector<ContactAccount*>* GetAccountList();

	private:
        // ����Android�����壬��GetAndroidDatagram()�����е��ñ�����
        virtual std::string GetAndroidBodyData();

        // �����������ݰ�
        virtual void SetAndroidReturnData(std::string data);

        // ����͵�IOS�豸�ϵ��ļ�����ȫ·����
        virtual std::wstring GetIosCommandFilePath(){ return L""; }

        // ��IOS�ϻ�ȡ���������ļ�����ȫ·����
        virtual std::wstring GetIosResultFilePath(){ return L""; }

        // ����ios�ļ�
        virtual std::wstring GetIosCommandFileContent(){ return L""; }

        // ����ios�豸�ķ����ļ���������ɾ���ļ�
        virtual void SetIosReturnFile(std::wstring filename){}

        void FromBuffer(BinaryBufferReader* pReader);

        // �������˻���Ϣ���ֻ��������е������˺ͷ�����Ϣ��ӽ�Ĭ���˻�
        void AddIntoDefaultAccount();
        
        void ParseAndroidPeople(BinaryBufferReader* pReader, ContactData* pContactData);
        void ParseAndroidPhoneName(BinaryBufferReader* pReader);
        void ParseAndroidPhoneTypes(BinaryBufferReader* pReader, Phone* psPhone);
        void ParseAndroidEmailTypes(BinaryBufferReader* pReader, Email* psEmail);
        void ParseAndroidImTypes(BinaryBufferReader* pReader, Im* psIm);
        void ParseAndroidAddress(BinaryBufferReader* pReader, Address* psAddress);
        void ParseAndroidOrganizationTypes(BinaryBufferReader* pReader, Organization* psOrganization);
        void ParseAndroidGroup(BinaryBufferReader* pReader, ContactGroupData* pContactGroup);

        // ����Ĭ���˻�
        ContactAccount* CreateDefaultAccount();

        // ִ��IOS����
        bool IosExecute();
		//bool ReloadContact();
		//bool ReloadImage();

    private:	

        map<int, ContactData*> _contactMap;          // ��������������MAP�������ID����
        map<int, ContactGroupData*> _groupMap;       // ���з�������MAP�������ID����
        vector<ContactAccount*> _contactAcconutList; // �����˻���Ϣ����

        bool _bVersion20; // ��׿��ϵͳ�İ汾�Ƿ�Ϊ2.0����
        bool _bOMS;       // �Ƿ�ΪOMSϵͳ

        bool _onlySize;// �Ƿ�ֻ��ȡ����
    };
}

#endif // !defined(EA_F5566066_F495_4725_8FFC_E03463640DCF__INCLUDED_)
