///////////////////////////////////////////////////////////
//  GetContactCommand.h
//  Implementation of the Class GetContactCommand
//  Created on:      01-六月-2011 9:42:54
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

    // 获取联络人信息命令
    class GetContactCommand : public BaseCommand
    {
    public:
        GetContactCommand(const DeviceData* pDevice);
        virtual ~GetContactCommand();

        // 设置是否只获取数据数量
        void SetOnlySize(bool onlySize);

		// 执行命令
		virtual void Execute();

        const vector<ContactAccount*>* GetAccountList();

	private:
        // 生成Android数据体，在GetAndroidDatagram()方法中调用本方法
        virtual std::string GetAndroidBodyData();

        // 解析返回数据包
        virtual void SetAndroidReturnData(std::string data);

        // 命令传送到IOS设备上的文件名（全路径）
        virtual std::wstring GetIosCommandFilePath(){ return L""; }

        // 从IOS上获取的命令结果文件名（全路径）
        virtual std::wstring GetIosResultFilePath(){ return L""; }

        // 生成ios文件
        virtual std::wstring GetIosCommandFileContent(){ return L""; }

        // 解析ios设备的返回文件，解析后删除文件
        virtual void SetIosReturnFile(std::wstring filename){}

        void FromBuffer(BinaryBufferReader* pReader);

        // 若是无账户信息的手机，将所有的联络人和分组信息添加进默认账户
        void AddIntoDefaultAccount();
        
        void ParseAndroidPeople(BinaryBufferReader* pReader, ContactData* pContactData);
        void ParseAndroidPhoneName(BinaryBufferReader* pReader);
        void ParseAndroidPhoneTypes(BinaryBufferReader* pReader, Phone* psPhone);
        void ParseAndroidEmailTypes(BinaryBufferReader* pReader, Email* psEmail);
        void ParseAndroidImTypes(BinaryBufferReader* pReader, Im* psIm);
        void ParseAndroidAddress(BinaryBufferReader* pReader, Address* psAddress);
        void ParseAndroidOrganizationTypes(BinaryBufferReader* pReader, Organization* psOrganization);
        void ParseAndroidGroup(BinaryBufferReader* pReader, ContactGroupData* pContactGroup);

        // 创建默认账户
        ContactAccount* CreateDefaultAccount();

        // 执行IOS命令
        bool IosExecute();
		//bool ReloadContact();
		//bool ReloadImage();

    private:	

        map<int, ContactData*> _contactMap;          // 所有联络人数据MAP方便跟据ID检索
        map<int, ContactGroupData*> _groupMap;       // 所有分组数据MAP方便跟据ID检索
        vector<ContactAccount*> _contactAcconutList; // 所有账户信息集合

        bool _bVersion20; // 安卓的系统的版本是否为2.0以上
        bool _bOMS;       // 是否为OMS系统

        bool _onlySize;// 是否只获取数量
    };
}

#endif // !defined(EA_F5566066_F495_4725_8FFC_E03463640DCF__INCLUDED_)
