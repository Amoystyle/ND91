///////////////////////////////////////////////////////////
//  MessageProgressData.h
//  Implementation of the Class MessageProgressData
//  Created on:      01-����-2011 9:43:00
//  Original author: Administrator
///////////////////////////////////////////////////////////

#if !defined(EA_95442B02_22F2_4ea3_B2EE_BB33259D3B29__INCLUDED_)
#define EA_95442B02_22F2_4ea3_B2EE_BB33259D3B29__INCLUDED_

#include "ProgressData.h"

namespace ND91Assistant
{

    class MessageProgressData : public ProgressData
    {

    public:
        MessageProgressData();
        virtual ~MessageProgressData();

    private:
        /**
        * ������ʾ��Ϣ�Ĵ��룬���渺�𽫴���ת�����ַ���
        */
        int _msgCode;

    };
}

#endif // !defined(EA_95442B02_22F2_4ea3_B2EE_BB33259D3B29__INCLUDED_)
