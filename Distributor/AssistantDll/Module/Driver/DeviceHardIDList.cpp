#include "stdafx.h"

#ifdef MODULE_DRIVER

#include "DeviceHardIDList.h"

using namespace ND91Assistant;
DeviceHardIDList::DeviceHardIDList()
{ 
}

DeviceHardIDList::~DeviceHardIDList()					   
{									   
    Ios_HardID_List.clear();
    Android_HardID_List.clear();
}

vector<string>* DeviceHardIDList::GetIos_HardID_List()
{
    LoadIosHardID();
    return &Ios_HardID_List;
}

vector<string>* DeviceHardIDList::GetAndroid_HardID_List()
{
    LoadAndroidHardID();
    return &Android_HardID_List;
}

// 加载Ios HardID list
void DeviceHardIDList::LoadIosHardID()
{
    if (Ios_HardID_List.size() > 0)
        return;
    // iPod
    Ios_HardID_List.push_back("USB\\VID_05AC&PID_1261");
    Ios_HardID_List.push_back("USB\\VID_05AC&PID_1262");
    Ios_HardID_List.push_back("USB\\VID_05AC&PID_1263");
    Ios_HardID_List.push_back("USB\\VID_05AC&PID_1365");
    Ios_HardID_List.push_back("USB\\VID_05AC&PID_1366");
    Ios_HardID_List.push_back("USB\\VID_05AC&PID_1302");
    Ios_HardID_List.push_back("USB\\VID_05AC&PID_1303");

    // iOs Devices
    Ios_HardID_List.push_back("USB\\VID_05AC&PID_1290");
    Ios_HardID_List.push_back("USB\\VID_05AC&PID_1291");
    Ios_HardID_List.push_back("USB\\VID_05AC&PID_1292");
    Ios_HardID_List.push_back("USB\\VID_05AC&PID_1293");
    Ios_HardID_List.push_back("USB\\VID_05AC&PID_1294");
    Ios_HardID_List.push_back("USB\\VID_05AC&PID_1297");
    Ios_HardID_List.push_back("USB\\VID_05AC&PID_1299");
    Ios_HardID_List.push_back("USB\\VID_05AC&PID_129A");
    Ios_HardID_List.push_back("USB\\VID_05AC&PID_129C");
    Ios_HardID_List.push_back("USB\\VID_05AC&PID_129D");
    Ios_HardID_List.push_back("USB\\VID_05AC&PID_129E");
    Ios_HardID_List.push_back("USB\\VID_05AC&PID_129F");
    Ios_HardID_List.push_back("USB\\VID_05AC&PID_12A0");
    Ios_HardID_List.push_back("USB\\VID_05AC&PID_12A1");
    Ios_HardID_List.push_back("USB\\VID_05AC&PID_12A2");
    Ios_HardID_List.push_back("USB\\VID_05AC&PID_12A3");
    Ios_HardID_List.push_back("USB\\VID_05AC&PID_12A4");
    Ios_HardID_List.push_back("USB\\VID_05AC&PID_12A5");
    Ios_HardID_List.push_back("USB\\VID_05AC&PID_12A6");
    Ios_HardID_List.push_back("USB\\VID_05AC&PID_12A7");
    Ios_HardID_List.push_back("USB\\VID_05AC&PID_12A8");
    Ios_HardID_List.push_back("USB\\VID_05AC&PID_12A9");
    Ios_HardID_List.push_back("USB\\VID_05AC&PID_129F");
}

// 加载Android HardID list
void DeviceHardIDList::LoadAndroidHardID()
{
    if (Android_HardID_List.size() > 0)
        return;
    ////"USB\\\\VID_0BB4&PID_0C01&MI_01"   to "USB\\\\VID_0BB4&PID_0EFF&MI_01"   特殊范围(HTC)
    Android_HardID_List.push_back("USB\\VID_0BB4&PID_0C02&MI_01");
    Android_HardID_List.push_back("USB\\VID_12D1&PID_1501&MI_03");
    Android_HardID_List.push_back("USB\\VID_0489&PID_C001&MI_01");
    Android_HardID_List.push_back("USB\\VID_18D1&PID_D00D");
    Android_HardID_List.push_back("USB\\VID_18D1&PID_DEED&MI_01");
    Android_HardID_List.push_back("USB\\VID_05C6&PID_9018&MI_01");
    Android_HardID_List.push_back("USB\\VID_18D1&PID_0002&MI_01");
    Android_HardID_List.push_back("USB\\VID_22B8&PID_41DA&MI_04");
    Android_HardID_List.push_back("USB\\VID_22B8&PID_2D74&MI_04");
    Android_HardID_List.push_back("USB\\VID_22B8&PID_2D75&MI_02");
    Android_HardID_List.push_back("USB\\VID_22B8&PID_2D76&MI_02");
    Android_HardID_List.push_back("USB\\VID_22B8&PID_2D62");
    Android_HardID_List.push_back("USB\\VID_22B8&PID_2D61&MI_01");
    Android_HardID_List.push_back("USB\\VID_22B8&PID_2D64&MI_01");
    Android_HardID_List.push_back("USB\\VID_22B8&PID_2D66&MI_00");
    Android_HardID_List.push_back("USB\\VID_22B8&PID_41D1");
    Android_HardID_List.push_back("USB\\VID_22B8&PID_41DB&MI_01");
    Android_HardID_List.push_back("USB\\VID_22B8&PID_41DC&MI_01");
    Android_HardID_List.push_back("USB\\VID_22B8&PID_41E2&MI_04");
    Android_HardID_List.push_back("USB\\VID_22B8&PID_41EE&MI_04");
    Android_HardID_List.push_back("USB\\VID_04E8&PID_6640&MI_04"); 
    Android_HardID_List.push_back("USB\\VID_17EF&PID_7401&MI_01");
    Android_HardID_List.push_back("USB\\VID_18D1&PID_4E12&MI_01");
    Android_HardID_List.push_back("USB\\VID_0BB4&PID_0C87&MI_01");
    Android_HardID_List.push_back("USB\\VID_0BB4&PID_0C97&MI_01");
    Android_HardID_List.push_back("USB\\VID_1004&PID_6171&MI_01");
    Android_HardID_List.push_back("USB\\VID_0FCE&PID_D12E&MI_01");
    Android_HardID_List.push_back("USB\\VID_04E8&PID_681C&MI_03");
    Android_HardID_List.push_back("USB\\VID_04E8&PID_6877&MI_03");
    Android_HardID_List.push_back("USB\\VID_1EBF&PID_6002&MI_01");
    Android_HardID_List.push_back("USB\\VID_0BB4&PID_0C99&MI_01");
    Android_HardID_List.push_back("USB\\VID_0FCE&PID_E12E&MI_01");
    Android_HardID_List.push_back("USB\\VID_0FCE&PID_212E&MI_01");
    Android_HardID_List.push_back("USB\\VID_0FCE&PID_312E&MI_01");
    Android_HardID_List.push_back("USB\\VID_0FCE&PID_D137&MI_00");
    Android_HardID_List.push_back("USB\\VID_0FCE&PID_E137&MI_00");
    Android_HardID_List.push_back("USB\\VID_0FCE&PID_2137&MI_00");
    Android_HardID_List.push_back("USB\\VID_0FCE&PID_3137&MI_00");
    Android_HardID_List.push_back("USB\\VID_0FCE&PID_D138&MI_00");
    Android_HardID_List.push_back("USB\\VID_0FCE&PID_E138&MI_00");
    Android_HardID_List.push_back("USB\\VID_0FCE&PID_2138&MI_00");
    Android_HardID_List.push_back("USB\\VID_0FCE&PID_3138&MI_00");
    Android_HardID_List.push_back("USB\\VID_19D2&PID_1350&MI_04");
    Android_HardID_List.push_back("USB\\VID_19D2&PID_1351&MI_01");
    Android_HardID_List.push_back("USB\\VID_19D2&PID_1354&MI_02");
    Android_HardID_List.push_back("USB\\VID_22B8&PID_2D7B&MI_03");
    Android_HardID_List.push_back("USB\\VID_22B8&PID_2D68&MI_01");
    Android_HardID_List.push_back("USB\\VID_22B8&PID_2D74");
    Android_HardID_List.push_back("USB\\VID_22B8&PID_2D76");
    Android_HardID_List.push_back("USB\\VID_22B8&PID_2D66");
    Android_HardID_List.push_back("USB\\VID_22B8&PID_41DA");
    Android_HardID_List.push_back("USB\\VID_22B8&PID_41DC");
    Android_HardID_List.push_back("USB\\VID_22B8&PID_41E2");
    Android_HardID_List.push_back("USB\\VID_0489&PID_C001");
    Android_HardID_List.push_back("USB\\VID_0489&PID_C004");
    Android_HardID_List.push_back("USB\\VID_04E8&PID_681C");
    Android_HardID_List.push_back("USB\\VID_04E8&PID_6880");
    Android_HardID_List.push_back("USB\\VID_04E8&PID_6877");
    Android_HardID_List.push_back("USB\\VID_04E8&PID_6850");
    Android_HardID_List.push_back("USB\\VID_04E8&PID_689C");
    Android_HardID_List.push_back("USB\\VID_04E8&PID_689E");
    Android_HardID_List.push_back("USB\\VID_1004&PID_618E");
    Android_HardID_List.push_back("USB\\VID_22B8&PID_2D75");
    Android_HardID_List.push_back("USB\\VID_22B8&PID_2D7B");
    Android_HardID_List.push_back("USB\\VID_22B8&PID_2D68");
    Android_HardID_List.push_back("USB\\VID_22B8&PID_2D61");
    Android_HardID_List.push_back("USB\\VID_22B8&PID_2D64");
    Android_HardID_List.push_back("USB\\VID_22B8&PID_41DB");
    Android_HardID_List.push_back("USB\\VID_22B8&PID_41EE");
    Android_HardID_List.push_back("USB\\VID_12D1&PID_1031");
    Android_HardID_List.push_back("USB\\VID_19D2&PID_0083");
    Android_HardID_List.push_back("USB\\VID_0BB4&PID_0C01");
    Android_HardID_List.push_back("USB\\VID_22B8&PID_2D63");
    Android_HardID_List.push_back("USB\\VID_22B8&PID_41D8");
    Android_HardID_List.push_back("USB\\VID_17EF&PID_7400");
    Android_HardID_List.push_back("USB\\VID_05C6&PID_9017");
    Android_HardID_List.push_back("USB\\VID_18D1&PID_4E11");
    Android_HardID_List.push_back("USB\\VID_0BB4&PID_0FF9");
    Android_HardID_List.push_back("USB\\VID_0FCE&PID_E12E");
    Android_HardID_List.push_back("USB\\VID_04E8&PID_681D");
    Android_HardID_List.push_back("USB\\VID_04E8&PID_6880&MI_02");
    Android_HardID_List.push_back("USB\\VID_18D1&PID_DDDD");
    Android_HardID_List.push_back("USB\\VID_0BB4&PID_0C81");
    Android_HardID_List.push_back("USB\\VID_0525&PID_A4A1");
    Android_HardID_List.push_back("USB\\VID_18D1&PID_0002");
    Android_HardID_List.push_back("USB\\VID_049F&PID_505A");
    Android_HardID_List.push_back("USB\\VID_0547&PID_2720");
    //NEW
    Android_HardID_List.push_back("USB\\VID_12D1&PID_1035&REV_0100&MI_03");    //华为
    Android_HardID_List.push_back("USB\\VID_04E8&PID_681C&MI_03");
    Android_HardID_List.push_back("USB\\VID_1EBF&PID_6045&REV_0216&MI_05");
    Android_HardID_List.push_back("USB\\VID_18D1&PID_4E22&REV_0227");			//三星NS
    Android_HardID_List.push_back("USB\\VID_18D1&PID_0005&MI_01");				//M9	

    Android_HardID_List.push_back("USB\\VID_19D2&PID_1366&REV_0226&MI_04");		//M9
    Android_HardID_List.push_back("USB\\VID_0FCE&PID_5156&REV_0226&MI_01");
}

#endif

bool ND91Assistant::DeviceHardIDList::IsSearched( string hardID )
{
    for (vector<string>::iterator it = _netSearch_HardID_List.begin(); it != _netSearch_HardID_List.end(); it++)
    {
        if (*it == hardID)
            return true;
    }
    return false;
}
