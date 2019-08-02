#pragma once
#include <string>
using namespace std;

namespace ND91Assistant
{  
    // 网络驱动接口
    class HardwareInfo;
	class HttpDriver
	{
	public:
		HttpDriver(const HardwareInfo* pHardware);
		~HttpDriver();

		//获取驱动下载地址
		wstring GetDriverUrl();

		//获取默认驱动下载地址，在GetDriverUrl返回""的情况下获取
		wstring GetDefaultAndroidDriverUrl();

		// 获取iTunes下载地址
		static wstring GetITunesUrl();

	private:
		string Post(string postData);

		const HardwareInfo* _pHardware;
		bool _isX86;
	};
}