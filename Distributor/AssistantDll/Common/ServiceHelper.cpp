#include "stdafx.h"

#include "ServiceHelper.h"


ServiceHelper::ServiceHelper( std::wstring name /*= L"Apple Mobile Device"*/ ) : _name(name)
{
    _interval   = 20;
    _repeat     = 750;
}

bool ServiceHelper::Start()
{
    // �򿪷���������
    SC_HANDLE hHandle = OpenSCManager(NULL, NULL, GENERIC_EXECUTE);
    if (!hHandle)
        return false;

    // ��Apple Mobile Device����
    SC_HANDLE hService = OpenService(hHandle, _name.c_str(), SERVICE_ALL_ACCESS|DELETE);
    if (hService == NULL)
    {
        CloseServiceHandle(hHandle);
        return false;
    }

    bool bSuccess = false;
    SERVICE_STATUS status;

    // ��÷����״̬
    if( QueryServiceStatus(hService, &status) )
    {
        switch (status.dwCurrentState)
        {
        case SERVICE_RUNNING:
            bSuccess = true;
            break;

        case SERVICE_START_PENDING:
        case SERVICE_CONTINUE_PENDING:
            bSuccess = PollServiceStatus(hService, hHandle, SERVICE_RUNNING);
            break;

        default:
            SetStartType(hService);    // ��Ϊ��ֹ��������Ϊ�Զ�����
            if ( ! StartService(hService, NULL, NULL))
                break;
            bSuccess = PollServiceStatus(hService, hHandle, SERVICE_RUNNING);
            break;
        }
    }

    ::CloseServiceHandle( hService);
    ::CloseServiceHandle( hHandle);
    return bSuccess;
}

void ServiceHelper::Stop()
{
    SC_HANDLE hHandle;
    SC_HANDLE hService;
    SERVICE_STATUS status;

    hHandle = OpenSCManager(NULL, NULL, GENERIC_EXECUTE);
    if (hHandle)
    {
        hService = OpenService(hHandle, _name.c_str(), SERVICE_ALL_ACCESS|DELETE);
        if (hService)
        {
            if(QueryServiceStatus(hService, &status) && status.dwCurrentState != SERVICE_STOPPED)
            {
                if (ControlService(hService, SERVICE_CONTROL_STOP, &status))
                {
                    PollServiceStatus(hService, hHandle, SERVICE_STOPPED);
                }				
            }
            CloseServiceHandle(hService);
        }
        CloseServiceHandle(hHandle);
    }		
}

// ��ѯ����״̬��������ָ��״̬����true�����򷵻�false
bool ServiceHelper::PollServiceStatus( SC_HANDLE hService, SC_HANDLE hHandle, DWORD waiting_status )
{
    SERVICE_STATUS status;

    Sleep(_interval);
    int nTime = _repeat;
    
    while(nTime && QueryServiceStatus(hService, &status))
    {
        nTime--;
        Sleep(_interval);
        if(status.dwCurrentState == waiting_status)
        {
            return true;
        }
    }
    return false;
}

// ��Ϊ��ֹ��������Ϊ�Զ�����
bool ServiceHelper::SetStartType( SC_HANDLE hService )
{
    LPQUERY_SERVICE_CONFIG pServicesInfo = NULL;
    unsigned long     bytesNeed = 0;
    
    QueryServiceConfig(hService, pServicesInfo, 0, &bytesNeed);      //ö�ٷ�����Ϣ
    if (bytesNeed <= 0)
        return false;

    pServicesInfo = (LPQUERY_SERVICE_CONFIG) new char[bytesNeed];
    QueryServiceConfig(hService, pServicesInfo, 0, &bytesNeed);      //ö�ٷ�����Ϣ

    if ( pServicesInfo && pServicesInfo->dwStartType == SERVICE_DISABLED)
    {
        if ( ! ChangeServiceConfigW(hService,
        	SERVICE_NO_CHANGE,
            SERVICE_AUTO_START,
            pServicesInfo->dwErrorControl,
            NULL, NULL, NULL, NULL, NULL, NULL, NULL))
        {
        	delete[] pServicesInfo;
            return false;
        }
    }

	delete[] pServicesInfo;
    return true;
}

void ServiceHelper::SetPollParameter( int interval, int repeat )
{
    _interval = interval;
    _repeat = repeat;
}
