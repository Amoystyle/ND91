#pragma  once

#include "Module/Contact/vCard/vCard.h"

namespace ND91Assistant
{
    class ContactRelatedCollection;
    class IMAddressCollection;
    class AnniversaryCollection;
    class WebPageCollection;
    class OrganizationExCollection;
    class DateCollection;
    class ContactData;

    // vCard Extention.
    class vCardEx : public vCard
    {
    public:
        vCardEx();
        virtual ~vCardEx();

        static bool ExportToVCards(const ContactData* pContact, vector<wstring> groupNames, wstring exportFolder);
        static ContactData* ImportFromVCards(wstring file);// �����߸����ͷ���ϵ������

        ContactRelatedCollection*   GetRelatedConllection() { return _pContactRelateds; }
        IMAddressCollection*        GetIMAddressConllection() { return _pIMAddresses; }
        AnniversaryCollection*      GetAnniversaryConllection() { return _pAnniversaries; }
        WebPageCollection*          GetWebPageConllection() { return _pWebPages; }
        OrganizationExCollection*   GetOrganizationExConllection() { return _pOrgCollection; }
        DateCollection*             GetDateConllection() { return _pDates; }

		//���ɵ�����ϵ���ַ���
		static wstring ContactToVCardString(const ContactData* pContact);
		//���ַ���������ϵ��
		static ContactData* ParseContact(wstring source);
    protected:
        virtual void OnInit();
        void UnInit();

    private:
        static ContactData* GetContactsFromVcards(wstring file);

        ContactRelatedCollection*   _pContactRelateds;
        IMAddressCollection*        _pIMAddresses;
        AnniversaryCollection*      _pAnniversaries;
        WebPageCollection*          _pWebPages;
        OrganizationExCollection*   _pOrgCollection;
        DateCollection*             _pDates;
    };
}