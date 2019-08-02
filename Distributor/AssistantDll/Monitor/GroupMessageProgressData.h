///////////////////////////////////////////////////////////
//  GroupMessageProgressData.h
//  Implementation of the Class GroupMessageProgressData
//  Created on:      01-六月-2011 9:42:56
//  Original author: Administrator
///////////////////////////////////////////////////////////

#if !defined(EA_BF1FD285_AB56_416b_AB68_3F157229703A__INCLUDED_)
#define EA_BF1FD285_AB56_416b_AB68_3F157229703A__INCLUDED_

#include "ProgressData.h"
#include <string>

namespace ND91Assistant
{

    class GroupMessageProgressData : public ProgressData
    {

    public:
        GroupMessageProgressData();
        virtual ~GroupMessageProgressData();

    private:
        /**
        * 短信群发专用。msgCode表示发送到本号码是否成功
        */
        std::wstring _SmsNumber;
        bool _success;

    };
}

#endif // !defined(EA_BF1FD285_AB56_416b_AB68_3F157229703A__INCLUDED_)
