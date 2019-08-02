#include "stdafx.h"

#ifdef MODULE_DRIVER
#include "Module/Driver/BaseDriverController.h"
#include "Common/NetOperation.h"
int BaseDriverController::GetOSData()
{
    OSVERSIONINFO record; 
    record.dwOSVersionInfoSize = sizeof(OSVERSIONINFO); 
    if (!GetVersionEx(&record)) 
        return 8;

    if (record.dwPlatformId == VER_PLATFORM_WIN32_NT) 
    {
        if (record.dwMajorVersion>6)
        {
            return 5;//ENUM_COMPUTEROS_Win7;
        }
        else if (record.dwMajorVersion==6&&record.dwPlatformId==2&&record.dwMinorVersion==1)
        {
            return 5;// ENUM_COMPUTEROS_Win7;
        }
        else if (record.dwMajorVersion==6)
        {
            return 3;//ENUM_COMPUTEROS_WinVista;
        }
        else if (record.dwMajorVersion==5)
        {
            return 1;//ENUM_COMPUTEROS_WinXP;
        }
        else if (record.dwMajorVersion==2)
        {
            return 2;//ENUM_COMPUTEROS_Win2003;
        }
    }
    return 8; // unknow
}

string BaseDriverController::GetUrlParam(vector<string>* pNames, vector<string>* pValues)
{
    string strMDBuf = "";
    string strOut = "";
    for (size_t i=0; i<pNames->size(); i++)
    {
        strMDBuf += pValues->at(i);

        strOut += pNames->at(i);
        strOut += "=";
        strOut += CNetOperation::URLEncode(pValues->at(i));
        strOut += "&";
    }
    strOut += "Checkcode=" + CStrOperation::GetMD5(strMDBuf + "!@#ASD12");
    return strOut;
}


#endif