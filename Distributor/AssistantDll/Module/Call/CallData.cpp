#include "pch_module.h"

#ifdef MODULE_CALL

#include "Module/Call/CallData.h"

CallData::CallData()
{
	_rowId              = -1;		//��¼���
	_phoneNumber        = "";	    //�绰����
	_date               = 0;	    //����ͨ����ʱ��
    _flags              = UNKNOW;
	_duration           = 0;	    //����ʱ��
    _contactPersonId    = 0;
    _countryCode        = 0;
}

CallData::CallData(CallData* pCallData)
{
    _rowId              = pCallData->_rowId;
    _phoneNumber        = pCallData->_phoneNumber;
    _date               = pCallData->_date;
    _duration           = pCallData->_duration;

    if (pCallData->_flags == OUTGOING && pCallData->_duration == 0)
        pCallData->_flags = MISSEDOUT;
    else
        _flags = pCallData->_flags;

    _contactPersonId    = pCallData->_contactPersonId;
    _countryCode        = pCallData->_countryCode;
}

CallData::CallData(const char* iosData[6])
{
    _rowId              = atoi(iosData[0]);
    _phoneNumber        = iosData[1];
    _date               = atoi(iosData[2]);
    _duration           = atoi(iosData[3]);

    if ((CALL_TYPE)atoi(iosData[4]) == 4 || (CALL_TYPE)atoi(iosData[4]) == 0)
    {
        if (_duration != 0)
            _flags = INCOMING;
        else
            _flags = MISSED;
    }
    else
    {
        if (_duration != 0)
            _flags = OUTGOING;
        else
            _flags = MISSEDOUT;
    }

    _contactPersonId    = atoi(iosData[5]);
}

CallData::~CallData()
{
}

size_t CallData::ParseAndroidData(const char* pData, size_t nSize)
{
    BinaryBufferReader reader(pData, nSize);
	reader.ReadInt32(_rowId);
	reader.ReadString(_phoneNumber);
	reader.ReadInt64(_date);
    _date /= 1000;
	reader.ReadInt32((int&)_flags);
	reader.ReadInt64(_duration);
	if (_flags == MISSED)
		_duration = 0;
    if (_flags == OUTGOING && _duration == 0)
        _flags = MISSEDOUT;

    return reader.GetOffset();
}



#endif