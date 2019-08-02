#include "stdafx.h"

#ifdef MODULE_DRIVER
#include "Module/Driver/GetDriverUrl.h"
#include "Common/NetOperation.h"
GetDriverUrl::GetDriverUrl(string deviceID, string driverID, wstring strFilePath) 
{
    _deviceID = deviceID;
    _driverID = driverID;
    _strFilePath = strFilePath;

    _driverUrl = "";
}
GetDriverUrl::~GetDriverUrl() 
{

}
string GetDriverUrl::GetUrl()
{
    vector<string> names, values;
    time_t t = time(0);
    string strTime = CNetOperation::Get("http://Device.sj.91.com/Controller.ashx?action=getservertime");
    names.push_back("DeviceID"); values.push_back(_deviceID);
    names.push_back("DriverID"); values.push_back(_driverID);
    names.push_back("TimeStamp");  values.push_back(strTime.length() > 0 ? strTime : CCodeOperation::UnicodeToUTF_8(MyStrOperation::FormatTime(t)));

    string strData = GetUrlParam(&names, &values);
    return "http://Device.sj.91.com/Controller.ashx?action=driverdownload&" + strData;
}

bool GetDriverUrl::Action()
{
    string strReturn = CNetOperation::Get(GetUrl());

    if (CStrOperation::CompareUnCase(strReturn,"false") != 0)
    {
        CFileOperation::WriteBinFile(_strFilePath, strReturn.c_str(), strReturn.length());        
        _result = true;
    }
    _result = false;
    return false;
}


#endif