#pragma once

#include <string>
#include <vector>
using namespace std;

namespace ND91Assistant
{    
    //驱动操作类，包含一个驱动所需要的操作
	class HardwareInfo;
    class CLASSINDLL_CLASS_DECL Driver
    {
	friend class DriverManager;
    public:
        Driver(HardwareInfo* pHardware);
        Driver(Driver* pDriver);
        ~Driver();

        //安装驱动 Inf 方式
        bool InstallInf(wstring strInf);

		//安装驱动 Exe 方式
		bool InstallExe(wstring strExe, wstring strParam=L"");

		//安装驱动 Msi 方式
		bool InstallMsi(wstring strMsi, wstring strParam=L"");


		//获取驱动下载地址
		wstring GetDriverUrl();

		//获取默认驱动下载地址
		wstring GetDefaultAndroidDriverUrl();

		const HardwareInfo* getHardwareInfo(){ return _pHardware;};

		// Summary  : 处理Inf文件，使其可以适应新的ID
		// Returns  : bool 是否有处理过
		// Parameter: wstring strInfFile inf文件路径
		bool CheckDefaultInfFile(wstring strInfFile);

		// 有些驱动安装完后不能马上生效，该方法尝试立即激活
		bool WakeupDevice();
    private:
		wstring GetInfHardID( const HardwareInfo* pInfo);
		//该指针由Driver负责释放
        HardwareInfo *_pHardware;
		// 是否是默认驱动
		bool _bDefaultDriver;
		// 设置驱动状态
		void SetDriverState(unsigned long nState);
    };
}