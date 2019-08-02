#include "stdafx.h"

#ifdef MODULE_DRIVER

#include "Module/Driver/GetDriverUrlID.h"
#include "Module/Driver/HardwareInfo.h"
#include "Common/SystemInfo.h"
#include "Common/NetOperation.h"
GetDriverUrlID::GetDriverUrlID(Driver* pDriver) 
{
    _pDriver = pDriver;
    _deviceID = "0";
    _driverID = "0";
    Action();
}
GetDriverUrlID::~GetDriverUrlID() 
{

}

bool GetDriverUrlID::Action()
{
    const HardwareInfo* pInfo = _pDriver->getHardwareInfo();
    vector<string> names, values;
    time_t t = time(0);
    string strTime = CNetOperation::Get("http://Device.sj.91.com/Controller.ashx?action=getservertime");
    ostringstream os;
    os << "USB\\VID_" << pInfo->_strVID << "&PID_" << pInfo->_strPID;
    names.push_back("Hardwareid"); values.push_back(os.str());
    names.push_back("Computeros"); values.push_back(CStrOperation::Int2string(GetOSData()));
    names.push_back("isx86");      values.push_back(CSystemInfo::is64() ? "0" : "1");
    names.push_back("CompositeID");values.push_back(CCodeOperation::UnicodeToUTF_8(pInfo->_strCompareID));
    names.push_back("DeviceTag");  values.push_back("");
    names.push_back("DeviceType"); values.push_back(CStrOperation::Int2string(pInfo->_nType));
    names.push_back("Vendor");     values.push_back(CCodeOperation::UnicodeToUTF_8(pInfo->_strProviderName));
    names.push_back("Model");      values.push_back("");
    names.push_back("CellPhoneOS");values.push_back("");
    names.push_back("TimeStamp");  values.push_back(strTime.length() > 0 ? strTime : CCodeOperation::UnicodeToUTF_8(MyStrOperation::FormatTime(t)));

    string strData = GetUrlParam(&names, &values);

    string strReturn = CNetOperation::Post("http://Device.sj.91.com/Controller.ashx?action=getdriverid", strData);

    int nIndex = strReturn.find(",");
    if (nIndex > 0)
    {
        _driverID = strReturn.substr(0, nIndex);
        _deviceID = strReturn.substr(nIndex + 1);
        return true;
    }
    return false;
}


#endif