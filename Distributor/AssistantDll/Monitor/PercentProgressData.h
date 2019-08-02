///////////////////////////////////////////////////////////
//  PercentProgressData.h
//  Implementation of the Class PercentProgressData
//  Created on:      01-六月-2011 9:43:01
//  Original author: Administrator
///////////////////////////////////////////////////////////

#if !defined(EA_587B68A3_FC1C_4e74_B09D_4970FA65E160__INCLUDED_)
#define EA_587B68A3_FC1C_4e74_B09D_4970FA65E160__INCLUDED_

#include "ProgressData.h"

namespace ND91Assistant
{

    class PercentProgressData : public ProgressData
    {

    public:
        PercentProgressData();
        virtual ~PercentProgressData();

    private:
        /**
        * 当前个数
        */
        int _current;
        /**
        * 总个数
        */
        int _total;
        /**
        * 子进度数据。只能嵌套一层，该对象中的pSubProgressData必须为NULL
        */
        ProgressData* _pSubProgressData;

    };
}

#endif // !defined(EA_587B68A3_FC1C_4e74_B09D_4970FA65E160__INCLUDED_)
