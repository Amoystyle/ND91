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


        // 加载数据
        CommandExecuteResult Load(bool bReload = false, bool bOnlySize = false);

        // 释放数据
        void Release();

        // 获得数据
        const vector<ContactAccount*>* GetDatas();


        // 删除手机上的全部记录
        CommandExecuteResult DeleteAll(ProgressObserver* pObserver = NULL);

        // 导入联络人数据
        bool ImportContactData( const vector<ContactData*>* pDataList,ContactAccount* pAccount,ProgressObserver* pObserver = NULL);
        bool ImportContactGroupData( const vector<ContactGroupData*>* pDataList,ContactAccount* pAccount,ProgressObserver* pObserver = NULL);

		//导出outlook联络人
		void ExportToOutlook(ProgressObserver* pObserver = NULL,bool conflict=false);

		// 导入outlook联络人
		void ImportFromOutlook(ProgressObserver* pObserver = NULL,bool conflict=false);

//    protected:
        ContactSynchronizer* GetContactSynchronizer(); 

        // 检查拼音排序版本
        //InstallState CheckPinyinVersion();

        // 安装一键排序
        //bool AutoSort();

        // 卸载一键排序
        //bool UnInstallAutoSort();

        // Summary  : 导出联系人到vCard
        // Returns  : 成功个数
        // Parameter: const vector<ContactAccount* > * contacts 要导出的联系人数据集合
        // Parameter: wstring exportFolder 导出的文件路径
        int ExportContactsToVCards(const vector<ContactAccount*>* contacts, wstring exportFolder, ProgressObserver* pObserver = NULL);

        // Summary  : 从vCard文件导入联系人
        // Returns  : 成功个数
        // Parameter: vector<wstring> files vCard文件集合
        int ImportContactsFromVCards(vector<wstring> files, ProgressObserver* pObserver = NULL);

        int SuccessCount() {return _nSuccessCount;}

		int RepeatCount(){return _nRepeatCount;}


    private:
        // 释放解析vCard文件而生成的联系人数据
        void ReleaseFromVCardContacts(ContactData* pContact);

        // 计算要操作的联系人数量（添加+删除）
        size_t SumTotalContactNum(vector<ContactData*> contacts);

        CRITICAL_SECTION _csCommandLock;

    protected:
        vector<ContactAccount*> _datas;
        
        ContactSynchronizer* _pContactSynchronizer; // 联系人同步器

        int _nSuccessCount;
		int _nRepeatCount;
    };
}