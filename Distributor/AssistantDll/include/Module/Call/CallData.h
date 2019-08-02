#pragma once

#include "Module/BaseData.h"
#include <string>
using namespace std;

namespace ND91Assistant
{
    // ͨ����¼����
    class CLASSINDLL_CLASS_DECL CallData : public BaseData
    {
        friend class GetCallDataCommand;

    public:
        CallData();
        CallData(CallData* pCallData);
        CallData(const char* iosData[6]);
        virtual ~CallData();

        // IOS::
        // �������ݿ��ֶ�flagΪ4������Ϊ����
        // ����--δͨ ���ݿ�duration(ͨ��ʱ��Ϊ0)
        // ����--δ�� ���ݿ�duration(ͨ��ʱ��Ϊ0)
        enum CALL_TYPE
        {
            UNKNOW      = 0,    // δ֪
            INCOMING    = 1,	// ����
            OUTGOING    = 2,	// ����
            MISSED      = 3,	// ����δ��
            MISSEDOUT   = 4     // ����δͨ
        };

    public:
        int		            _rowId;			    // ��¼���
        string	            _phoneNumber;	    // �绰����
        time_t	            _date;			    // ����ͨ����ʱ�䣨��λ�����룩
        CALL_TYPE           _flags;             // ͨ������
        time_t	            _duration;		    // ����ʱ�䣨��λ���룩
        int                 _contactPersonId;   // ��ϵ��ID
        int                 _countryCode;       // ���Ҵ���

    private:
        size_t ParseAndroidData(const char* pData, size_t nSize);
    };
}