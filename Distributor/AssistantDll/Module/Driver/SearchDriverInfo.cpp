#include "stdafx.h"

#ifdef MODULE_DRIVER
#include "Module/Driver/SearchDriverInfo.h"
#include "Common/NetOperation.h"

SearchDriverInfo::SearchDriverInfo(string hardID) 
{
    _hardID = hardID;
    Action();
}
SearchDriverInfo::~SearchDriverInfo() 
{

}

bool SearchDriverInfo::Action()
{
    vector<string> names, values;
    time_t t = time(0);
    string strTime = CNetOperation::Get("http://Device.sj.91.com/Controller.ashx?action=getservertime");
    names.push_back("HardwareID"); values.push_back(_hardID);
    names.push_back("TimeStamp");  values.push_back(strTime.length() > 0 ? strTime : CCodeOperation::UnicodeToUTF_8(MyStrOperation::FormatTime(t)));

    string strData = GetUrlParam(&names, &values);

    string strReturn = CNetOperation::Post("http://Device.sj.91.com/Controller.ashx?action=searchdriverinfo", strData);

    _result = (strReturn != "False");
    return _result;
}


#endif