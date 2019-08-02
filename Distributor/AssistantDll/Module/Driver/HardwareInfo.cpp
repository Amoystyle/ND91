#include "stdafx.h"

#ifdef MODULE_DRIVER

#include "Module/Driver/HardwareInfo.h"

HardwareInfo::HardwareInfo(HARDWARE_TYPE nType)
:_nType(nType)
{
}

HardwareInfo::HardwareInfo(const HardwareInfo* pInfo)
{
    _nType = pInfo->_nType;

    _strProviderName = pInfo->_strProviderName;    //���쳧��
    _strDriverDesc = pInfo->_strDriverDesc;      //�豸��
    _strHardID = pInfo->_strHardID;          //Ӳ�����к�
    _strCompareID = pInfo->_strCompareID;       //�豸������Ϣ

    _strVID = pInfo->_strVID;
    _strPID = pInfo->_strPID;
    _strMI = pInfo->_strMI;
    _strKey = pInfo->_strKey;

    _vid = pInfo->_vid;
    _pid = pInfo->_pid;	

    _nState = pInfo->_nState;
}

HardwareInfo::~HardwareInfo()
{

}

#endif