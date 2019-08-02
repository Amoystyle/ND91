///////////////////////////////////////////////////////////
//  PercentProgressData.h
//  Implementation of the Class PercentProgressData
//  Created on:      01-����-2011 9:43:01
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
        * ��ǰ����
        */
        int _current;
        /**
        * �ܸ���
        */
        int _total;
        /**
        * �ӽ������ݡ�ֻ��Ƕ��һ�㣬�ö����е�pSubProgressData����ΪNULL
        */
        ProgressData* _pSubProgressData;

    };
}

#endif // !defined(EA_587B68A3_FC1C_4e74_B09D_4970FA65E160__INCLUDED_)
