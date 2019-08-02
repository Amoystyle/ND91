#pragma  once

#include "Common/BinaryBufferWriter.h"

using namespace std;

namespace ND91Assistant
{
    class ContactData;
    class ContactGroupData;

    // 联络人命令辅助类（用于添加联络人和更新联络中）
    class ContactCommandHelper
    {

    public:
        ContactCommandHelper(ContactData* pContactData, bool bVersion20, bool bOMS);
        ContactCommandHelper();
        virtual ~ContactCommandHelper();

        // Summary  : 写入联络人基本信息（注：OMS系统和安卓2.0以下系统在此处写入联络人姓名和备注信息）
        // Returns  : void
        // Parameter: BinaryBufferWriter & writer 写入的数据流
        void WriteAndroidPeople(BinaryBufferWriter& writer);

		// Summary  : 写入联络人头像，oms除外
		// Returns	: void
		// Parameter: BinaryBufferWriter & writer
		void WriteAndroidPhoto(BinaryBufferWriter& writer);
        
        // Summary  : 写入联络人姓名信息（安卓2.0以上系统）
        // Returns  : void
        // Parameter: BinaryBufferWriter & writer 写入的数据流
        void WriteAndroidName(BinaryBufferWriter& writer);
        
        // Summary  : 写入联络人电话信息
        // Returns  : void
        // Parameter: BinaryBufferWriter & writer 写入的数据流
        void WriteAndroidPhone(BinaryBufferWriter& writer);
        
        // Summary  : 写入联络人组织信息
        // Returns  : void
        // Parameter: BinaryBufferWriter & writer 写入的数据流
        void WriteAndroidOrganization(BinaryBufferWriter& writer);

        // Summary  : 写入联络人电子邮箱信息
        // Returns  : void
        // Parameter: BinaryBufferWriter & writer 写入的数据流
        void WriteAndroidEmail(BinaryBufferWriter& writer);

        // Summary  : 写入联络人通讯地址信息
        // Returns  : void
        // Parameter: BinaryBufferWriter & writer 写入的数据流
        void WriteAndroidAddress(BinaryBufferWriter& writer);

        // Summary  : 写入联络人即时通讯账户信息
        // Returns  : void
        // Parameter: BinaryBufferWriter & writer 写入的数据流
        void WriteAndroidIm(BinaryBufferWriter& writer);

        // Summary  : 写入联络人备注信息（安卓2.0以上系统）
        // Returns  : void
        // Parameter: BinaryBufferWriter & writer 写入的数据流
        void WriteAndroidComment(BinaryBufferWriter& writer);

        string ToBuffer();
        ContactData* FromBuffer(BinaryBufferReader* pReader);
    private:
        ContactData* _pNewContactData;  //解析后新增加的联系人
        ContactData* _pContactData;     //有添加联络人或更新联络命令传递过来的联络人信息

        bool _bVersion20;           // 安卓的系统的版本是否为2.0以上
        bool _bOMS;                 // 是否为OMS系统

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