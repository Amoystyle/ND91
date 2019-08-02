#pragma once
#include "AssistantDll.h"
#include <string>
using namespace std;
namespace ND91Assistant
{
    // һ��Ӳ������Ϣ��
    class CLASSINDLL_CLASS_DECL HardwareInfo
    {
    friend class DriverManager;

    public:
        //HardwareInfo(HARDWARE_TYPE nType, HDEVINFO hDevinfo, SP_DEVINFO_DATA device);
		HardwareInfo(HARDWARE_TYPE nType);
        HardwareInfo(const HardwareInfo* pInfo);
        ~HardwareInfo();

    public:
        HARDWARE_TYPE _nType;

        wstring _strProviderName;    //���쳧��
        wstring _strDriverDesc;      //�豸��
        wstring _strHardID;          //Ӳ�����к�
        wstring _strCompareID;       //�豸������Ϣ

		string	_strVID;
		string  _strPID;
		string	_strMI;
		string  _strKey;

		int		_vid;
		int		_pid;	

		unsigned long	_nState;			 //�豸״̬��0��ʾ����(����Ҫ��װ����) �����ʾ��Ҫ��װ����
		
    private:


    };
}