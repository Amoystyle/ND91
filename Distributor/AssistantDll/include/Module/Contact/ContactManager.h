#pragma  once

#include "Module/IManager.h"
#include "Module/Contact/ContactAccount.h"
#include "Module/Contact/ContactGroupData.h"
#include "Module/Contact/ContactData.h"

namespace ND91Assistant
{
    class ContactSynchronizer;
    class CLASSINDLL_CLASS_DECL ContactManager : IManager
    {
//     friend class AddContactDataCommand;
//     friend class ContactAndGroupCommand;
//     friend class DeleteContactCommand;
//     friend class DeleteContactGroupCommand;
//     friend class GetContactCommand;
//     friend class UpdateContactCommand;
//     friend class UpdateContactGroupCommand;
    public:
        ContactManager(const DeviceData* pDevice);
        virtual ~ContactManager();


        // ��������
        CommandExecuteResult Load(bool bReload = false, bool bOnlySize = false);

        // �ͷ�����
        void Release();

        // �������
        const vector<ContactAccount*>* GetDatas();


        // ɾ���ֻ��ϵ�ȫ����¼
        CommandExecuteResult DeleteAll(ProgressObserver* pObserver = NULL);

        // ��������������
        bool ImportContactData( const vector<ContactData*>* pDataList,ContactAccount* pAccount,ProgressObserver* pObserver = NULL);
        bool ImportContactGroupData( const vector<ContactGroupData*>* pDataList,ContactAccount* pAccount,ProgressObserver* pObserver = NULL);

		//����outlook������
		void ExportToOutlook(ProgressObserver* pObserver = NULL,bool conflict=false);

		// ����outlook������
		void ImportFromOutlook(ProgressObserver* pObserver = NULL,bool conflict=false);

//    protected:
        ContactSynchronizer* GetContactSynchronizer(); 

        // ���ƴ������汾
        //InstallState CheckPinyinVersion();

        // ��װһ������
        //bool AutoSort();

        // ж��һ������
        //bool UnInstallAutoSort();

        // Summary  : ������ϵ�˵�vCard
        // Returns  : �ɹ�����
        // Parameter: const vector<ContactAccount* > * contacts Ҫ��������ϵ�����ݼ���
        // Parameter: wstring exportFolder �������ļ�·��
        int ExportContactsToVCards(const vector<ContactAccount*>* contacts, wstring exportFolder, ProgressObserver* pObserver = NULL);

        // Summary  : ��vCard�ļ�������ϵ��
        // Returns  : �ɹ�����
        // Parameter: vector<wstring> files vCard�ļ�����
        int ImportContactsFromVCards(vector<wstring> files, ProgressObserver* pObserver = NULL);

        int SuccessCount() {return _nSuccessCount;}

		int RepeatCount(){return _nRepeatCount;}


    private:
        // �ͷŽ���vCard�ļ������ɵ���ϵ������
        void ReleaseFromVCardContacts(ContactData* pContact);

        // ����Ҫ��������ϵ�����������+ɾ����
        size_t SumTotalContactNum(vector<ContactData*> contacts);

        CRITICAL_SECTION _csCommandLock;

    protected:
        vector<ContactAccount*> _datas;
        
        ContactSynchronizer* _pContactSynchronizer; // ��ϵ��ͬ����

        int _nSuccessCount;
		int _nRepeatCount;
    };
}