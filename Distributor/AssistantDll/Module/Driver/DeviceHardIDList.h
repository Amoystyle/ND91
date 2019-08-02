#pragma once
#include <string>
#include <vector>
using namespace std;

namespace ND91Assistant
{
	// Ӳ���б������࣬�ĳ�����������ȫ�ֱ������µ��ڴ�й©()
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
		vector<string> Ios_HardID_List;		// Ios Ӳ��ID�б�
		vector<string> Android_HardID_List;	// Android Ӳ��ID�б�
		
		// ����Ios HardID list
		void LoadIosHardID();
		// ����Android HardID list
		void LoadAndroidHardID();
                
	};										   
}											   

