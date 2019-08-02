#pragma  once

#include "Common/BinaryBufferWriter.h"

using namespace std;

namespace ND91Assistant
{
    class ContactData;
    class ContactGroupData;

    // ������������ࣨ������������˺͸��������У�
    class ContactCommandHelper
    {

    public:
        ContactCommandHelper(ContactData* pContactData, bool bVersion20, bool bOMS);
        ContactCommandHelper();
        virtual ~ContactCommandHelper();

        // Summary  : д�������˻�����Ϣ��ע��OMSϵͳ�Ͱ�׿2.0����ϵͳ�ڴ˴�д�������������ͱ�ע��Ϣ��
        // Returns  : void
        // Parameter: BinaryBufferWriter & writer д���������
        void WriteAndroidPeople(BinaryBufferWriter& writer);

		// Summary  : д��������ͷ��oms����
		// Returns	: void
		// Parameter: BinaryBufferWriter & writer
		void WriteAndroidPhoto(BinaryBufferWriter& writer);
        
        // Summary  : д��������������Ϣ����׿2.0����ϵͳ��
        // Returns  : void
        // Parameter: BinaryBufferWriter & writer д���������
        void WriteAndroidName(BinaryBufferWriter& writer);
        
        // Summary  : д�������˵绰��Ϣ
        // Returns  : void
        // Parameter: BinaryBufferWriter & writer д���������
        void WriteAndroidPhone(BinaryBufferWriter& writer);
        
        // Summary  : д����������֯��Ϣ
        // Returns  : void
        // Parameter: BinaryBufferWriter & writer д���������
        void WriteAndroidOrganization(BinaryBufferWriter& writer);

        // Summary  : д�������˵���������Ϣ
        // Returns  : void
        // Parameter: BinaryBufferWriter & writer д���������
        void WriteAndroidEmail(BinaryBufferWriter& writer);

        // Summary  : д��������ͨѶ��ַ��Ϣ
        // Returns  : void
        // Parameter: BinaryBufferWriter & writer д���������
        void WriteAndroidAddress(BinaryBufferWriter& writer);

        // Summary  : д�������˼�ʱͨѶ�˻���Ϣ
        // Returns  : void
        // Parameter: BinaryBufferWriter & writer д���������
        void WriteAndroidIm(BinaryBufferWriter& writer);

        // Summary  : д�������˱�ע��Ϣ����׿2.0����ϵͳ��
        // Returns  : void
        // Parameter: BinaryBufferWriter & writer д���������
        void WriteAndroidComment(BinaryBufferWriter& writer);

        string ToBuffer();
        ContactData* FromBuffer(BinaryBufferReader* pReader);
    private:
        ContactData* _pNewContactData;  //�����������ӵ���ϵ��
        ContactData* _pContactData;     //����������˻������������ݹ�������������Ϣ

        bool _bVersion20;           // ��׿��ϵͳ�İ汾�Ƿ�Ϊ2.0����
        bool _bOMS;                 // �Ƿ�ΪOMSϵͳ

    private:
        void ReadAndroidPeople(BinaryBufferReader *pReader);
        void ReadAndroidPhoto(BinaryBufferReader *pReader);
        void ReadAndroidName(BinaryBufferReader *pReader);
        void ReadAndroidPhone(BinaryBufferReader *pReader);
        void ReadAndroidOrganization(BinaryBufferReader *pReader);
        void ReadAndroidEmail(BinaryBufferReader *pReader);
        void ReadAndroidAddress(BinaryBufferReader *pReader);
        void ReadAndroidIm(BinaryBufferReader *pReader);
        void ReadAndroidComment(BinaryBufferReader *pReader);
    };
}