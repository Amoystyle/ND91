#pragma once

#include "Module/BaseData.h"
#include <string>
using namespace std;

namespace ND91Assistant
{
    // 通话记录数据
    class CLASSINDLL_CLASS_DECL CallData : public BaseData
    {
        friend class GetCallDataCommand;

    public:
        CallData();
        CallData(CallData* pCallData);
        CallData(const char* iosData[6]);
        virtual ~CallData();

        // IOS::
        // 呼入数据库字段flag为4，其他为呼出
        // 呼出--未通 数据库duration(通话时间为0)
        // 呼入--未听 数据库duration(通话时间为0)
        enum CALL_TYPE
        {
            UNKNOW      = 0,    // 未知
            INCOMING    = 1,	// 拨进
            OUTGOING    = 2,	// 拨出
            MISSED      = 3,	// 拨进未接
            MISSEDOUT   = 4     // 拨出未通
        };

    public:
        int		            _rowId;			    // 记录编号
        string	            _phoneNumber;	    // 电话号码
        time_t	            _date;			    // 发起通话的时间（单位：毫秒）
        CALL_TYPE           _flags;             // 通话类型
        time_t	            _duration;		    // 持续时间（单位：秒）
        int                 _contactPersonId;   // 联系人ID
        int                 _countryCode;       // 国家代码

    private:
        size_t ParseAndroidData(const char* pData, size_t nSize);
    };
}