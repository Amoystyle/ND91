#pragma once
#include <string>
#include <vector>
using namespace std;

namespace ND91Assistant
{
	// 硬件列表数据类，改成这样是由于全局变量导致的内存泄漏()
	class DeviceHardIDList
	{
    friend class DriverManger;
	public:
		DeviceHardIDList();									   
		~DeviceHardIDList();
		vector<string>* GetIos_HardID_List();
		vector<string>* GetAndroid_HardID_List();
        bool IsSearched(string hardID);

        vector<string> _netSearch_HardID_List;
	private:									   
		vector<string> Ios_HardID_List;		// Ios 硬件ID列表
		vector<string> Android_HardID_List;	// Android 硬件ID列表
		
		// 加载Ios HardID list
		void LoadIosHardID();
		// 加载Android HardID list
		void LoadAndroidHardID();
                
	};										   
}											   

